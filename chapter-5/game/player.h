#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "engine/app.h"
#include "engine/node.h"
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
  ng::Tilemap* tilemap_ = nullptr;
  GameManager* game_manager_ = nullptr;
  ScoreManager* score_manager_ = nullptr;
  const ng::RectangleCollider* collider_ = nullptr;
  sf::Sprite sprite_;
  sf::Vector2f velocity_;
  bool is_on_ground_ = false;
  bool is_dead_ = false;
  sf::Sound jump_sound_;
  sf::Sound banana_sound_;
  sf::Sound plastic_block_sound_;
};

}  // namespace game
