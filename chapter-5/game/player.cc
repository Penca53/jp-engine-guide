#include "player.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <vector>

#include "banana.h"
#include "engine/app.h"
#include "engine/collider.h"
#include "engine/input.h"
#include "engine/node.h"
#include "engine/rectangle_collider.h"
#include "engine/tilemap.h"
#include "game_manager.h"
#include "mushroom.h"
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

Player::Player(ng::App* app, ng::Tilemap* tilemap, GameManager* game_manager,
               ScoreManager* score_manager)
    : ng::Node(app),
      tilemap_(tilemap),
      game_manager_(game_manager),
      score_manager_(score_manager),
      sprite_(GetApp()->GetResourceManager().LoadTexture(
          "Player/Idle (32x32).png")),
      jump_sound_(
          GetApp()->GetResourceManager().LoadSoundBuffer("Player/Jump_2.wav")),
      banana_sound_(GetApp()->GetResourceManager().LoadSoundBuffer(
          "Banana/Collectibles_2.wav")),
      plastic_block_sound_(
          GetApp()->GetResourceManager().LoadSoundBuffer("Hit_1.wav")) {
  SetName("Player");

  sprite_.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
  sprite_.setOrigin({16, 16});
  sprite_.setScale({2, 2});

  auto& collider = MakeChild<ng::RectangleCollider>(sf::Vector2f(32, 48));
  collider.SetLocalPosition({0, 8});
  collider_ = &collider;
}

sf::Vector2f Player::GetVelocity() const {
  return velocity_;
}

void Player::TakeDamage() {
  if (is_dead_) {
    return;
  }

  is_dead_ = true;
  Destroy();
  game_manager_->Lose();
}

void Player::Update() {  // NOLINT
  if (is_dead_) {
    return;
  }

  sf::Vector2f direction;
  if (GetApp()->GetInput().GetKey(sf::Keyboard::Scancode::A)) {
    direction.x -= 1;
    sprite_.setScale(sf::Vector2f{-2.F, 2.F});
  }
  if (GetApp()->GetInput().GetKey(sf::Keyboard::Scancode::D)) {
    direction.x += 1;
    sprite_.setScale(sf::Vector2f{2.F, 2.F});
  }

  static constexpr float kMovementSpeed = 4.F;
  velocity_.x = direction.x * kMovementSpeed;
  velocity_.y += 1;

  if (is_on_ground_ &&
      GetApp()->GetInput().GetKeyDown(sf::Keyboard::Scancode::Space)) {
    velocity_.y -= 15;
    jump_sound_.play();
  }

  sf::Vector2f old_pos = collider_->GetGlobalTransform().getPosition();
  sf::Vector2f new_pos = old_pos + velocity_;

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

  if (velocity_.x < 0 && (DoesCollide(top_left, *tilemap_) ||
                          DoesCollide(middle_left, *tilemap_) ||
                          DoesCollide(bottom_left, *tilemap_))) {
    new_pos.x = std::ceil(top_left.x / tilemap_size.x) * tilemap_size.x +
                col_half_size.x;
    velocity_.x = 0;
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

  if (velocity_.x > 0 && (DoesCollide(top_right, *tilemap_) ||
                          DoesCollide(middle_right, *tilemap_) ||
                          DoesCollide(bottom_right, *tilemap_))) {
    new_pos.x = std::floor(top_right.x / tilemap_size.x) * tilemap_size.x -
                col_half_size.x;
    velocity_.x = 0;
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

  if (velocity_.y < 0 &&
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
    velocity_.y = 0;
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

  if (velocity_.y > 0 && (DoesCollide(bottom_left, *tilemap_) ||
                          DoesCollide(bottom_right, *tilemap_))) {
    new_pos.y = std::floor(bottom_left.y / tilemap_size.y) * tilemap_size.y -
                col_half_size.y;
    velocity_.y = 0;
    is_on_ground_ = true;
  } else {
    is_on_ground_ = false;
  }

  SetLocalPosition(new_pos - collider_->GetLocalTransform().getPosition());

  std::vector<const ng::Collider*> others =
      GetScene()->GetPhysics().Overlap(*collider_);
  for (const auto* other : others) {
    if (other->GetParent()->GetName() == "Mushroom") {
      if (velocity_.y > 0) {
        auto* mushroom = dynamic_cast<Mushroom*>(other->GetParent());
        if (!mushroom->GetIsDead()) {
          mushroom->TakeDamage();
          score_manager_->AddScore(100);
          velocity_.y = -10;
        }
      }
    } else if (other->GetParent()->GetName() == "Banana") {
      auto* banana = dynamic_cast<Banana*>(other->GetParent());
      if (!banana->GetIsCollected()) {
        banana->Collect();
        score_manager_->AddScore(500);
        banana_sound_.play();
      }
    }
  }
}

void Player::Draw(sf::RenderTarget& target) {
  target.draw(sprite_, GetGlobalTransform().getTransform());
}

}  // namespace game