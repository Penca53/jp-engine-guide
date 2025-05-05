#include "banana.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cstdint>
#include <memory>
#include <utility>

#include "engine/app.h"
#include "engine/circle_collider.h"
#include "engine/node.h"
#include "engine/sprite_sheet_animation.h"
#include "engine/state.h"

namespace game {

static constexpr int32_t kAnimationTPF = 4;

Banana::IdleState::IdleState(ng::State<Context>::ID id,
                             ng::SpriteSheetAnimation animation)
    : ng::State<Context>(std::move(id)), animation_(std::move(animation)) {}

void Banana::IdleState::OnEnter() {
  animation_.Start();
}

void Banana::IdleState::Update() {
  animation_.Update();
}

Banana::Banana(ng::App* app)
    : ng::Node(app),
      sprite_(GetApp()->GetResourceManager().LoadTexture("Banana/Bananas.png")),
      animator_(&context_, std::make_unique<IdleState>(
                               "run", ng::SpriteSheetAnimation(
                                          &sprite_, &sprite_.getTexture(),
                                          kAnimationTPF))) {
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

void Banana::Update() {
  animator_.Update();
}

void Banana::Draw(sf::RenderTarget& target) {
  target.draw(sprite_, GetGlobalTransform().getTransform());
}

}  // namespace game
