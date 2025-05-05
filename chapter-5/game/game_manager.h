#pragma once

#include <SFML/Audio/Sound.hpp>
#include <cstdint>

#include "engine/node.h"
#include "lose_canvas.h"

namespace game {

class GameManager : public ng::Node {
 public:
  enum class State : uint8_t {
    BEGIN = 0,
    PLAY,
    WON,
    LOST,
  };

  explicit GameManager(ng::App* app);

  void Win();
  void Lose();

  State GetState() const;

 protected:
  void OnAdd() override;
  void Update() override;

 private:
  State state_{};
  sf::Sound lose_sound_;

  LoseCanvas* lose_canvas_ = nullptr;
};

}  // namespace game