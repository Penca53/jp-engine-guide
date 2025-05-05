#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

#include "engine/fsm.h"
#include "engine/node.h"
#include "engine/rectangle_collider.h"
#include "engine/sprite_sheet_animation.h"
#include "engine/tilemap.h"

namespace game {

class Plant : public ng::Node {
 public:
  Plant(ng::App* app, const ng::Tilemap* tilemap);
  bool GetIsDead() const;
  void TakeDamage();

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  struct Context {
    bool is_attacking = false;
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

  class AttackState : public ng::State<Context> {
   public:
    AttackState(ng::State<Context>::ID id, ng::SpriteSheetAnimation animation,
                const Plant* plant, const ng::Tilemap* tilemap,
                sf::Vector2f direction);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    void Attack();

    ng::SpriteSheetAnimation animation_;
    const Plant* plant_ = nullptr;
    const ng::Tilemap* tilemap_ = nullptr;
    sf::Vector2f direction_;
  };

  class HitState : public ng::State<Context> {
   public:
    HitState(ng::State<Context>::ID id, ng::SpriteSheetAnimation animation,
             const sf::SoundBuffer* sound_buffer, Plant* plant);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    void Die();

    ng::SpriteSheetAnimation animation_;
    sf::Sound sound_;
    Plant* plant_ = nullptr;
  };

  sf::Vector2f direction_{-1, 0};
  const ng::Tilemap* tilemap_ = nullptr;
  const ng::RectangleCollider* collider_ = nullptr;
  sf::Sprite sprite_;
  int32_t attack_timer_ = 0;
  Context context_;
  ng::FSM<Context> animator_;
};

}  // namespace game