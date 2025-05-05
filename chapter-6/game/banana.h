#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "engine/circle_collider.h"
#include "engine/fsm.h"
#include "engine/node.h"
#include "engine/sprite_sheet_animation.h"
#include "engine/state.h"

namespace game {

class Banana : public ng::Node {
 public:
  explicit Banana(ng::App* app);

  bool GetIsCollected() const;
  void Collect();

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  struct Context {};

  class IdleState : public ng::State<Context> {
   public:
    IdleState(ng::State<Context>::ID id, ng::SpriteSheetAnimation animation);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    ng::SpriteSheetAnimation animation_;
  };

  sf::Sprite sprite_;
  bool is_collected_ = false;
  Context context_;
  ng::FSM<Context> animator_;
};

}  // namespace game