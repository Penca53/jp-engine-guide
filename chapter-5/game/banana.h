#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "engine/circle_collider.h"
#include "engine/node.h"

namespace game {

class Banana : public ng::Node {
 public:
  explicit Banana(ng::App* app);

  bool GetIsCollected() const;
  void Collect();

 protected:
  void Draw(sf::RenderTarget& target) override;

 private:
  sf::Sprite sprite_;
  bool is_collected_ = false;
};

}  // namespace game