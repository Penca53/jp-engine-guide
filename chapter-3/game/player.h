#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "engine/app.h"
#include "engine/node.h"
#include "engine/tilemap.h"

namespace game {

class Player : public ng::Node {
 public:
  Player(ng::App* app, const ng::Tilemap* tilemap);

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  const ng::Tilemap* tilemap_ = nullptr;
  sf::Sprite sprite_;
  sf::Vector2f player_velocity_;
  bool is_on_ground_ = false;
  sf::Sound jump_sound_;
};

}  // namespace game
