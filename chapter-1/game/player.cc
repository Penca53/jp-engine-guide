#include "player.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cassert>

#include "engine/app.h"
#include "engine/input.h"

namespace game {

Player::Player(ng::App* app)
    : app_(app),
      sprite_(
          app_->GetResourceManager().LoadTexture("Player/Idle (32x32).png")) {

  assert(app);

  sprite_.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
  sprite_.setOrigin({16, 16});
  sprite_.setScale({2, 2});
}

void Player::Update() {
  sf::Vector2f direction;
  const ng::Input& input = app_->GetInput();
  if (input.GetKey(sf::Keyboard::Scancode::A)) {
    direction.x -= 1;
    sprite_.setScale(sf::Vector2f{-2.F, 2.F});
  }
  if (input.GetKey(sf::Keyboard::Scancode::D)) {
    direction.x += 1;
    sprite_.setScale(sf::Vector2f{2.F, 2.F});
  }
  if (input.GetKey(sf::Keyboard::Scancode::W)) {
    direction.y -= 1;
  }
  if (input.GetKey(sf::Keyboard::Scancode::S)) {
    direction.y += 1;
  }

  if (direction.lengthSquared() > 0) {
    sf::Vector2f new_pos = sprite_.getPosition();
    static constexpr float kMovementSpeed = 4.F;
    new_pos += direction.normalized() * kMovementSpeed;
    sprite_.setPosition(new_pos);
  }
}

void Player::Draw(sf::RenderTarget& target) {
  sf::View v = target.getView();
  v.setCenter(sprite_.getPosition());
  target.setView(v);

  target.draw(sprite_);
}

}  // namespace game