#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

#include "engine/node.h"

namespace game {

class WinCanvas : public ng::Node {
 public:
  explicit WinCanvas(ng::App* app);

  void Enable();
  void Disable();

 protected:
  void Draw(sf::RenderTarget& target) override;

 private:
  bool is_enabled_ = false;
  sf::RectangleShape background_;
  sf::Text title_text_;
  sf::Text restart_text_;
};

}  // namespace game