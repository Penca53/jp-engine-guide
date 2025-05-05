#include "player.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

#include "engine/app.h"
#include "engine/input.h"
#include "engine/node.h"
#include "engine/tilemap.h"
#include "tile_id.h"

namespace game {

namespace {

bool DoesCollide(sf::Vector2f position, const ng::Tilemap& tilemap) {
  TileID id = tilemap.GetWorldTile(position).GetID();
  return id == TileID::kDirt;
}

}  // namespace

Player::Player(ng::App* app, const ng::Tilemap* tilemap)
    : ng::Node(app),
      tilemap_(tilemap),
      sprite_(GetApp()->GetResourceManager().LoadTexture(
          "Player/Idle (32x32).png")),
      jump_sound_(
          GetApp()->GetResourceManager().LoadSoundBuffer("Player/Jump_2.wav")) {
  SetName("Player");
  sprite_.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
  sprite_.setOrigin({16, 16});
  sprite_.setScale({2, 2});
}

void Player::Update() {  // NOLINT
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
  player_velocity_.x = direction.x * kMovementSpeed;
  player_velocity_.y += 1;

  if (is_on_ground_ &&
      GetApp()->GetInput().GetKeyDown(sf::Keyboard::Scancode::Space)) {
    player_velocity_.y -= 15;
    jump_sound_.play();
  }

  sf::Vector2f collision_offset(0, 8);
  sf::Vector2f old_pos = GetLocalTransform().getPosition() + collision_offset;
  sf::Vector2f new_pos = old_pos + player_velocity_;

  sf::Vector2f col_half_size = {16, 24};
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
    return;
  }

  if (player_velocity_.x < 0 && (DoesCollide(top_left, *tilemap_) ||
                                 DoesCollide(middle_left, *tilemap_) ||
                                 DoesCollide(bottom_left, *tilemap_))) {
    new_pos.x = std::ceil(top_left.x / tilemap_size.x) * tilemap_size.x +
                col_half_size.x;
    player_velocity_.x = 0;
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
    return;
  }

  if (player_velocity_.x > 0 && (DoesCollide(top_right, *tilemap_) ||
                                 DoesCollide(middle_right, *tilemap_) ||
                                 DoesCollide(bottom_right, *tilemap_))) {
    new_pos.x = std::floor(top_right.x / tilemap_size.x) * tilemap_size.x -
                col_half_size.x;
    player_velocity_.x = 0;
  }

  top_left = {new_pos.x - (col_half_size.x - kEps),
              new_pos.y - (col_half_size.y - kEps)};
  top_right = {new_pos.x + (col_half_size.x - kEps),
               new_pos.y - (col_half_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(top_left) ||
      !tilemap_->IsWithinWorldBounds(top_right)) {
    return;
  }

  if (player_velocity_.y < 0 &&
      (DoesCollide(top_left, *tilemap_) || DoesCollide(top_right, *tilemap_))) {
    new_pos.y = std::ceil(top_left.y / tilemap_size.y) * tilemap_size.y +
                col_half_size.y;
    player_velocity_.y = 0;
  }

  bottom_left = {new_pos.x - (col_half_size.x - kEps),
                 new_pos.y + (col_half_size.y - kEps)};
  bottom_right = {new_pos.x + (col_half_size.x - kEps),
                  new_pos.y + (col_half_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(bottom_left) ||
      !tilemap_->IsWithinWorldBounds(bottom_right)) {
    return;
  }

  if (player_velocity_.y > 0 && (DoesCollide(bottom_left, *tilemap_) ||
                                 DoesCollide(bottom_right, *tilemap_))) {
    new_pos.y = std::floor(bottom_left.y / tilemap_size.y) * tilemap_size.y -
                col_half_size.y;
    player_velocity_.y = 0;
    is_on_ground_ = true;
  } else {
    is_on_ground_ = false;
  }

  SetLocalPosition(new_pos - collision_offset);
}

void Player::Draw(sf::RenderTarget& target) {
  sf::View v = target.getView();
  v.setCenter(GetLocalTransform().getPosition());
  target.setView(v);

  target.draw(sprite_, GetGlobalTransform().getTransform());
}

}  // namespace game