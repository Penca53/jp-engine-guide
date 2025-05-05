#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "engine/app.h"
#include "engine/tilemap.h"

namespace game {

class Player {
 public:
  Player(ng::App* app, const ng::Tilemap* tilemap);

  void Update();
  void Draw(sf::RenderTarget& target);

 private:
  ng::App* app_ = nullptr;
  const ng::Tilemap* tilemap_ = nullptr;
  sf::Sprite sprite_;
  sf::Vector2f player_velocity_;
  bool is_on_ground_ = false;
  sf::Sound jump_sound_;
};

}  // namespace game
