#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "engine/app.h"

namespace game {

class Player {
 public:
  explicit Player(ng::App* app);

  void Update();
  void Draw(sf::RenderTarget& target);

 private:
  ng::App* app_ = nullptr;
  sf::Sprite sprite_;
};

}  // namespace game
