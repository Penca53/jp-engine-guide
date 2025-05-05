#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "engine/fsm.h"
#include "engine/node.h"
#include "engine/sprite_sheet_animation.h"
#include "game_manager.h"

namespace game {

class End : public ng::Node {
 public:
  End(ng::App* app, GameManager* game_manager);

  void EndGame();

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  struct Context {
    bool is_pressed_ = false;
  };

  class IdleState : public ng::State<Context> {
   public:
    IdleState(ng::State<Context>::ID id, ng::SpriteSheetAnimation animation);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    ng::SpriteSheetAnimation animation_;
  };

  class PressedState : public ng::State<Context> {
   public:
    PressedState(ng::State<Context>::ID id, ng::SpriteSheetAnimation animation,
                 GameManager* game_manager);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    ng::SpriteSheetAnimation animation_;
    GameManager* game_manager_ = nullptr;
  };

  sf::Sprite sprite_;
  Context context_;
  ng::FSM<Context> animator_;
  GameManager* game_manager_ = nullptr;
};

}  // namespace game