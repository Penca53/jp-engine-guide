#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "engine/app.h"
#include "engine/fsm.h"
#include "engine/node.h"
#include "engine/rectangle_collider.h"
#include "engine/sprite_sheet_animation.h"
#include "engine/state.h"
#include "engine/tilemap.h"

namespace game {

class Mushroom : public ng::Node {
 public:
  Mushroom(ng::App* app, const ng::Tilemap* tilemap);
  bool GetIsDead() const;
  void TakeDamage();

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  struct Context {
    bool is_dead = false;
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

  class HitState : public ng::State<Context> {
   public:
    HitState(ng::State<Context>::ID id, ng::SpriteSheetAnimation animation,
             const sf::SoundBuffer* sound_buffer, ng::Node* node);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    void Die();

    ng::SpriteSheetAnimation animation_;
    sf::Sound sound_;
    ng::Node* node_ = nullptr;
  };

  sf::Vector2f direction_{-1, 0};
  sf::Vector2f velocity_;
  const ng::Tilemap* tilemap_ = nullptr;
  const ng::RectangleCollider* collider_ = nullptr;
  bool is_on_ground_ = false;
  sf::Sprite sprite_;
  Context context_;
  ng::FSM<Context> animator_;
};

}  // namespace game