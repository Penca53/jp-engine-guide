#include "mushroom.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <cmath>

#include "engine/app.h"
#include "engine/tilemap.h"
#include "tile_id.h"

namespace game {

namespace {

bool DoesCollide(sf::Vector2f position, const ng::Tilemap& tilemap) {
  TileID id = tilemap.GetWorldTile(position).GetID();
  return id == TileID::kDirt;
}

}  // namespace

Mushroom::Mushroom(ng::App* app, const ng::Tilemap* tilemap)
    : app_(app),
      tilemap_(tilemap),
      sprite_(
          app_->GetResourceManager().LoadTexture("Mushroom/Run (32x32).png")) {
  assert(app);
  sprite_.setScale({2, 2});
  sprite_.setOrigin({16, 16});
  sprite_.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
  sprite_.setPosition({64, 64});
}

void Mushroom::Update() {  // NOLINT
  static constexpr float kMovementSpeed = 2.F;
  velocity_.x = direction_.x * kMovementSpeed;
  velocity_.y += 1;

  sf::Vector2f old_pos = sprite_.getPosition();
  sf::Vector2f new_pos = old_pos + velocity_;

  sf::Vector2f col_size = {32, 32};
  sf::Vector2f tilemap_size = sf::Vector2f(tilemap_->GetTileSize());

  static constexpr float kEps = 0.001F;
  sf::Vector2f top_left = {new_pos.x - (col_size.x - kEps),
                           old_pos.y - (col_size.y - kEps)};
  sf::Vector2f bottom_left = {new_pos.x - (col_size.x - kEps),
                              old_pos.y + (col_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(top_left) ||
      !tilemap_->IsWithinWorldBounds(bottom_left)) {
    return;
  }

  if (velocity_.x < 0 && (DoesCollide(top_left, *tilemap_) ||
                          DoesCollide(bottom_left, *tilemap_))) {
    new_pos.x =
        std::ceil(top_left.x / tilemap_size.x) * tilemap_size.x + col_size.x;
    velocity_.x = 0;
    direction_.x = 1;
  }

  sf::Vector2f top_right = {new_pos.x + (col_size.x - kEps),
                            old_pos.y - (col_size.y - kEps)};
  sf::Vector2f bottom_right = {new_pos.x + (col_size.x - kEps),
                               old_pos.y + (col_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(top_right) ||
      !tilemap_->IsWithinWorldBounds(bottom_right)) {
    return;
  }

  if (velocity_.x > 0 && (DoesCollide(top_right, *tilemap_) ||
                          DoesCollide(bottom_right, *tilemap_))) {
    new_pos.x =
        std::floor(top_right.x / tilemap_size.x) * tilemap_size.x - col_size.x;
    velocity_.x = 0;
    direction_.x = -1;
  }

  top_left = {new_pos.x - (col_size.x - kEps), new_pos.y - (col_size.y - kEps)};
  top_right = {new_pos.x + (col_size.x - kEps),
               new_pos.y - (col_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(top_left) ||
      !tilemap_->IsWithinWorldBounds(top_right)) {
    return;
  }

  if (velocity_.y < 0 &&
      (DoesCollide(top_left, *tilemap_) || DoesCollide(top_right, *tilemap_))) {
    new_pos.y =
        std::ceil(top_left.y / tilemap_size.y) * tilemap_size.y + col_size.y;
    velocity_.y = 0;
  }

  bottom_left = {new_pos.x - (col_size.x - kEps),
                 new_pos.y + (col_size.y - kEps)};
  bottom_right = {new_pos.x + (col_size.x - kEps),
                  new_pos.y + (col_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(bottom_left) ||
      !tilemap_->IsWithinWorldBounds(bottom_right)) {
    return;
  }

  if (velocity_.y > 0 && (DoesCollide(bottom_left, *tilemap_) ||
                          DoesCollide(bottom_right, *tilemap_))) {
    new_pos.y = std::floor(bottom_left.y / tilemap_size.y) * tilemap_size.y -
                col_size.y;
    velocity_.y = 0;
    is_on_ground_ = true;
  } else {
    is_on_ground_ = false;
  }

  sprite_.setPosition(new_pos);
}

void Mushroom::Draw(sf::RenderTarget& target) {
  sprite_.setScale(sf::Vector2f{-direction_.x * 2, 2.F});
  target.draw(sprite_);
}

}  // namespace game