#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "engine/fsm.h"
#include "engine/node.h"
#include "engine/rectangle_collider.h"
#include "engine/sprite_sheet_animation.h"
#include "engine/tilemap.h"
#include "game_manager.h"
#include "score_manager.h"

namespace game {

class Player : public ng::Node {
 public:
  Player(ng::App* app, ng::Tilemap* tilemap, GameManager* game_manager,
         ScoreManager* score_manager);
  sf::Vector2f GetVelocity() const;
  void TakeDamage();

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  struct Context {
    sf::Vector2f velocity;
    bool is_on_ground = false;
    bool is_dead = false;
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

  class RunState : public ng::State<Context> {
   public:
    RunState(ng::State<Context>::ID id, ng::SpriteSheetAnimation animation);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    ng::SpriteSheetAnimation animation_;
  };

  class JumpState : public ng::State<Context> {
   public:
    JumpState(ng::State<Context>::ID id, ng::SpriteSheetAnimation animation,
              const sf::SoundBuffer* sound_buffer);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    ng::SpriteSheetAnimation animation_;
    sf::Sound sound_;
  };

  class FallState : public ng::State<Context> {
   public:
    FallState(ng::State<Context>::ID id, ng::SpriteSheetAnimation animation);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    ng::SpriteSheetAnimation animation_;
  };

  class HitState : public ng::State<Context> {
   public:
    HitState(ng::State<Context>::ID id, ng::SpriteSheetAnimation animation,
             ng::Node* node, GameManager* game_manager);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    void Die();

    ng::SpriteSheetAnimation animation_;
    ng::Node* node_ = nullptr;
    GameManager* game_manager_ = nullptr;
  };

  ng::Tilemap* tilemap_ = nullptr;
  GameManager* game_manager_ = nullptr;
  ScoreManager* score_manager_ = nullptr;
  const ng::RectangleCollider* collider_ = nullptr;
  sf::Sprite sprite_;
  bool has_won_ = false;
  Context context_;
  ng::FSM<Context> animator_;
  sf::Sound plastic_block_sound_;
  sf::Sound banana_sound_;
};

}  // namespace game