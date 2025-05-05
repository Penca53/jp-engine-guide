#include "player.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "banana.h"
#include "end.h"
#include "engine/app.h"
#include "engine/collider.h"
#include "engine/fsm.h"
#include "engine/input.h"
#include "engine/node.h"
#include "engine/rectangle_collider.h"
#include "engine/resource_manager.h"
#include "engine/sprite_sheet_animation.h"
#include "engine/state.h"
#include "engine/tilemap.h"
#include "engine/transition.h"
#include "game_manager.h"
#include "mushroom.h"
#include "plant.h"
#include "score_manager.h"
#include "tile_id.h"

namespace game {

namespace {

bool DoesCollide(sf::Vector2f position, const ng::Tilemap& tilemap) {
  TileID id = tilemap.GetWorldTile(position).GetID();
  return id == TileID::kInvisibleBarrier ||
         (id >= TileID::kDirtTopLeft && id <= TileID::kDirtBottomRight) ||
         (id >= TileID::kStoneHorizontalLeft &&
          id <= TileID::kStoneVerticalBottom) ||
         id == TileID::kPlasticBlock;
}

}  // namespace

static constexpr int32_t kAnimationTPF = 4;

Player::IdleState::IdleState(ng::State<Context>::ID id,
                             ng::SpriteSheetAnimation animation)
    : ng::State<Context>(std::move(id)), animation_(std::move(animation)) {}

void Player::IdleState::OnEnter() {
  animation_.Start();
}

void Player::IdleState::Update() {
  animation_.Update();
}

Player::RunState::RunState(ng::State<Context>::ID id,
                           ng::SpriteSheetAnimation animation)
    : ng::State<Context>(std::move(id)), animation_(std::move(animation)) {}

void Player::RunState::OnEnter() {
  animation_.Start();
}

void Player::RunState::Update() {
  animation_.Update();
}

Player::JumpState::JumpState(ng::State<Context>::ID id,
                             ng::SpriteSheetAnimation animation,
                             const sf::SoundBuffer* sound_buffer)
    : ng::State<Context>(std::move(id)),
      animation_(std::move(animation)),
      sound_(*sound_buffer) {}

void Player::JumpState::OnEnter() {
  animation_.Start();
  sound_.play();
}

void Player::JumpState::Update() {
  animation_.Update();
}

Player::FallState::FallState(ng::State<Context>::ID id,
                             ng::SpriteSheetAnimation animation)
    : ng::State<Context>(std::move(id)), animation_(std::move(animation)) {}

void Player::FallState::OnEnter() {
  animation_.Start();
}

void Player::FallState::Update() {
  animation_.Update();
}

Player::HitState::HitState(ng::State<Context>::ID id,
                           ng::SpriteSheetAnimation animation, ng::Node* node,
                           GameManager* game_manager)
    : ng::State<Context>(std::move(id)),
      animation_(std::move(animation)),
      node_(node),
      game_manager_(game_manager) {
  animation_.RegisterOnEndCallback([this]() { Die(); });
}

void Player::HitState::OnEnter() {
  animation_.Start();
}

void Player::HitState::Update() {
  animation_.Update();
}

void Player::HitState::Die() {
  game_manager_->Lose();
  node_->Destroy();
}

Player::Player(ng::App* app, ng::Tilemap* tilemap, GameManager* game_manager,
               ScoreManager* score_manager)
    : ng::Node(app),
      tilemap_(tilemap),
      game_manager_(game_manager),
      score_manager_(score_manager),
      sprite_(GetApp()->GetResourceManager().LoadTexture(
          "Player/Idle (32x32).png")),
      animator_(&context_, std::make_unique<IdleState>(
                               "idle", ng::SpriteSheetAnimation(
                                           &sprite_, &sprite_.getTexture(),
                                           kAnimationTPF))),
      plastic_block_sound_(
          GetApp()->GetResourceManager().LoadSoundBuffer("Hit_1.wav")),
      banana_sound_(GetApp()->GetResourceManager().LoadSoundBuffer(
          "Banana/Collectibles_2.wav")) {
  SetName("Player");

  sprite_.setScale({2, 2});
  sprite_.setOrigin({16, 16});
  sprite_.setTextureRect(sf::IntRect({0, 0}, {32, 32}));

  auto& collider = MakeChild<ng::RectangleCollider>(sf::Vector2f(32, 48));
  collider.SetLocalPosition({0, 8});
  collider_ = &collider;

  animator_.AddState(std::make_unique<RunState>(
      "run",
      ng::SpriteSheetAnimation(
          &sprite_,
          &GetApp()->GetResourceManager().LoadTexture("Player/Run (32x32).png"),
          kAnimationTPF)));
  animator_.AddState(std::make_unique<JumpState>(
      "jump",
      ng::SpriteSheetAnimation(&sprite_,
                               &GetApp()->GetResourceManager().LoadTexture(
                                   "Player/Jump (32x32).png"),
                               kAnimationTPF),
      &GetApp()->GetResourceManager().LoadSoundBuffer("Player/Jump_2.wav")));
  animator_.AddState(std::make_unique<FallState>(
      "fall",
      ng::SpriteSheetAnimation(&sprite_,
                               &GetApp()->GetResourceManager().LoadTexture(
                                   "Player/Fall (32x32).png"),
                               kAnimationTPF)));
  animator_.AddState(std::make_unique<HitState>(
      "hit",
      ng::SpriteSheetAnimation(
          &sprite_,
          &GetApp()->GetResourceManager().LoadTexture("Player/Hit (32x32).png"),
          kAnimationTPF),
      this, game_manager_));

  animator_.AddTransition({"idle", "run", [](Context& context) -> bool {
                             return context.velocity.x != 0;
                           }});
  animator_.AddTransition({"run", "idle", [](Context& context) -> bool {
                             return context.velocity.x == 0;
                           }});

  animator_.AddTransition({"idle", "jump", [](Context& context) -> bool {
                             return context.velocity.y < 0;
                           }});
  animator_.AddTransition({"run", "jump", [](Context& context) -> bool {
                             return context.velocity.y < 0;
                           }});

  animator_.AddTransition({"jump", "fall", [](Context& context) -> bool {
                             return context.velocity.y > 0 &&
                                    !context.is_on_ground;
                           }});

  animator_.AddTransition({"jump", "idle", [](Context& context) -> bool {
                             return context.is_on_ground;
                           }});
  animator_.AddTransition({"fall", "idle", [](Context& context) -> bool {
                             return context.is_on_ground;
                           }});

  animator_.AddTransition({"idle", "hit", [](Context& context) -> bool {
                             return context.is_dead;
                           }});
  animator_.AddTransition({"run", "hit", [](Context& context) -> bool {
                             return context.is_dead;
                           }});
  animator_.AddTransition({"jump", "hit", [](Context& context) -> bool {
                             return context.is_dead;
                           }});
  animator_.AddTransition({"fall", "hit", [](Context& context) -> bool {
                             return context.is_dead;
                           }});
}

sf::Vector2f Player::GetVelocity() const {
  return context_.velocity;
}

void Player::TakeDamage() {
  context_.is_dead = true;
}

void Player::Update() {  // NOLINT
  animator_.Update();

  if (context_.is_dead) {
    return;
  }

  sf::Vector2f direction;
  if (!has_won_) {
    if (GetApp()->GetInput().GetKey(sf::Keyboard::Scancode::A)) {
      direction.x += -1;
      sprite_.setScale(sf::Vector2f{-2.F, 2.F});
    }
    if (GetApp()->GetInput().GetKey(sf::Keyboard::Scancode::D)) {
      direction.x += 1;
      sprite_.setScale(sf::Vector2f{2.F, 2.F});
    }
  }

  static constexpr float kMovementSpeed = 4.F;
  context_.velocity.x = direction.x * kMovementSpeed;
  context_.velocity.y += 1;

  if (!has_won_ && context_.is_on_ground &&
      GetApp()->GetInput().GetKeyDown(sf::Keyboard::Scancode::Space)) {
    context_.velocity.y -= 15;
  }

  sf::Vector2f old_pos = collider_->GetGlobalTransform().getPosition();
  sf::Vector2f new_pos = old_pos + context_.velocity;

  sf::Vector2f col_half_size = collider_->GetSize() / 2.F;
  sf::Vector2f tilemap_size = sf::Vector2f(tilemap_->GetTileSize());

  static constexpr float kEps = 0.001F;
  sf::Vector2f top_left = {new_pos.x - (col_half_size.x - kEps),
                           old_pos.y - (col_half_size.y - kEps)};
  sf::Vector2f middle_left = {new_pos.x + (col_half_size.x - kEps),
                              old_pos.y + (0 - kEps)};
  sf::Vector2f bottom_left = {new_pos.x - (col_half_size.x - kEps),
                              old_pos.y + (col_half_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(top_left) ||
      !tilemap_->IsWithinWorldBounds(middle_left) ||
      !tilemap_->IsWithinWorldBounds(bottom_left)) {
    TakeDamage();
    return;
  }

  if (context_.velocity.x < 0 && (DoesCollide(top_left, *tilemap_) ||
                                  DoesCollide(middle_left, *tilemap_) ||
                                  DoesCollide(bottom_left, *tilemap_))) {
    new_pos.x = std::ceil(top_left.x / tilemap_size.x) * tilemap_size.x +
                col_half_size.x;
    context_.velocity.x = 0;
  }

  sf::Vector2f top_right = {new_pos.x + (col_half_size.x - kEps),
                            old_pos.y - (col_half_size.y - kEps)};
  sf::Vector2f middle_right = {new_pos.x + (col_half_size.x - kEps),
                               old_pos.y + (0 - kEps)};
  sf::Vector2f bottom_right = {new_pos.x + (col_half_size.x - kEps),
                               old_pos.y + (col_half_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(top_right) ||
      !tilemap_->IsWithinWorldBounds(middle_right) ||
      !tilemap_->IsWithinWorldBounds(bottom_right)) {
    TakeDamage();
    return;
  }

  if (context_.velocity.x > 0 && (DoesCollide(top_right, *tilemap_) ||
                                  DoesCollide(middle_right, *tilemap_) ||
                                  DoesCollide(bottom_right, *tilemap_))) {
    new_pos.x = std::floor(top_right.x / tilemap_size.x) * tilemap_size.x -
                col_half_size.x;
    context_.velocity.x = 0;
  }

  top_left = {new_pos.x - (col_half_size.x - kEps),
              new_pos.y - (col_half_size.y - kEps)};
  top_right = {new_pos.x + (col_half_size.x - kEps),
               new_pos.y - (col_half_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(top_left) ||
      !tilemap_->IsWithinWorldBounds(top_right)) {
    TakeDamage();
    return;
  }

  if (context_.velocity.y < 0 &&
      (DoesCollide(top_left, *tilemap_) || DoesCollide(top_right, *tilemap_))) {
    {
      if (tilemap_->GetWorldTile(top_left).GetID() == TileID::kPlasticBlock) {
        tilemap_->SetWorldTile(top_left, TileID::kVoid);
        plastic_block_sound_.play();
      }

      if (tilemap_->GetWorldTile(top_right).GetID() == TileID::kPlasticBlock) {
        tilemap_->SetWorldTile(top_right, TileID::kVoid);
        plastic_block_sound_.play();
      }
    }

    new_pos.y = std::ceil(top_left.y / tilemap_size.y) * tilemap_size.y +
                col_half_size.y;
    context_.velocity.y = 0;
  }

  bottom_left = {new_pos.x - (col_half_size.x - kEps),
                 new_pos.y + (col_half_size.y - kEps)};
  bottom_right = {new_pos.x + (col_half_size.x - kEps),
                  new_pos.y + (col_half_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(bottom_left) ||
      !tilemap_->IsWithinWorldBounds(bottom_right)) {
    TakeDamage();
    return;
  }

  if (context_.velocity.y > 0 && (DoesCollide(bottom_left, *tilemap_) ||
                                  DoesCollide(bottom_right, *tilemap_))) {
    new_pos.y = std::floor(bottom_left.y / tilemap_size.y) * tilemap_size.y -
                col_half_size.y;
    context_.velocity.y = 0;
    context_.is_on_ground = true;
  } else {
    context_.is_on_ground = false;
  }

  SetLocalPosition(new_pos - collider_->GetLocalTransform().getPosition());

  std::vector<const ng::Collider*> others =
      GetScene()->GetPhysics().Overlap(*collider_);
  for (const auto* other : others) {
    if (other->GetParent()->GetName() == "Mushroom") {
      if (context_.velocity.y > 0) {
        auto* mushroom = dynamic_cast<Mushroom*>(other->GetParent());
        if (!mushroom->GetIsDead()) {
          mushroom->TakeDamage();
          context_.velocity.y = -10;
          score_manager_->AddScore(100);
        }
      }
    } else if (other->GetParent()->GetName() == "Plant") {
      if (context_.velocity.y > 0) {
        auto* plant = dynamic_cast<Plant*>(other->GetParent());
        if (!plant->GetIsDead()) {
          plant->TakeDamage();
          context_.velocity.y = -10;
          score_manager_->AddScore(150);
        }
      }
    } else if (other->GetParent()->GetName() == "Banana") {
      auto* banana = dynamic_cast<Banana*>(other->GetParent());
      if (!banana->GetIsCollected()) {
        banana->Collect();
        score_manager_->AddScore(500);
        banana_sound_.play();
      }
    } else if (other->GetParent()->GetName() == "End") {
      if (!has_won_) {
        context_.velocity.y = -15;
        dynamic_cast<End*>(other->GetParent())->EndGame();
        has_won_ = true;
      }
    }
  }
}

void Player::Draw(sf::RenderTarget& target) {
  target.draw(sprite_, GetGlobalTransform().getTransform());
}

}  // namespace game