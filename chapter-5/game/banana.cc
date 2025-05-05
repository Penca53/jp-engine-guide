#include "banana.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "engine/app.h"
#include "engine/circle_collider.h"
#include "engine/node.h"

namespace game {

Banana::Banana(ng::App* app)
    : ng::Node(app),
      sprite_(
          GetApp()->GetResourceManager().LoadTexture("Banana/Bananas.png")) {
  SetName("Banana");
  sprite_.setScale({2, 2});
  sprite_.setOrigin({16, 16});
  sprite_.setTextureRect(sf::IntRect({0, 0}, {32, 32}));

  MakeChild<ng::CircleCollider>(16.F);
}

bool Banana::GetIsCollected() const {
  return is_collected_;
}

void Banana::Collect() {
  if (is_collected_) {
    return;
  }

  is_collected_ = true;
  Destroy();
}

void Banana::Draw(sf::RenderTarget& target) {
  target.draw(sprite_, GetGlobalTransform().getTransform());
}

}  // namespace game
