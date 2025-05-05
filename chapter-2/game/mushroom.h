#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "engine/app.h"
#include "engine/tilemap.h"

namespace game {

class Mushroom {
 public:
  Mushroom(ng::App* app, const ng::Tilemap* tilemap);

  void Update();
  void Draw(sf::RenderTarget& target);

 private:
  ng::App* app_ = nullptr;
  sf::Vector2f direction_{-1, 0};
  sf::Vector2f velocity_;
  const ng::Tilemap* tilemap_ = nullptr;
  sf::Sprite sprite_;
  bool is_on_ground_ = false;
};

}  // namespace game