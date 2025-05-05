#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cstdint>

#include "engine/node.h"

namespace game {

class ScoreManager : public ng::Node {
 public:
  explicit ScoreManager(ng::App* app);

  void AddScore(int32_t score);

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  void UpdateUI();

  sf::Text score_text_;
  int32_t score_ = 0;
};

}  // namespace game