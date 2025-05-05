#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

#include "engine/node.h"

namespace game {

class Background : public ng::Node {
 public:
  Background(ng::App* app, sf::Vector2u size);

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  sf::Vector2u size_;
  sf::Texture* texture_ = nullptr;
  sf::VertexArray image_vertices_;
  int32_t t_ = 0;
};

}  // namespace game