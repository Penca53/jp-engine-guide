#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <bitset>

namespace ng {

/// @brief Manages keyboard input, tracking key presses, holds, and releases.
class Input {
 public:
  /// @brief Advances the input state, copying the current key states to the old key states for detecting key down and up events.
  void Advance();

  /// @brief Handles SFML window events, specifically KeyPressed and KeyReleased events, to update the current key states.
  /// @param event The SFML event to process.
  void Handle(const sf::Event& event);

  /// @brief Checks if a specific key was pressed down (went from not pressed to pressed) in the current frame.
  /// @param key The SFML scancode of the key to check.
  /// @return True if the key was pressed down, false otherwise.
  [[nodiscard]] bool GetKeyDown(sf::Keyboard::Scancode key) const;

  /// @brief Checks if a specific key is currently being held down.
  /// @param key The SFML scancode of the key to check.
  /// @return True if the key is currently pressed, false otherwise.
  [[nodiscard]] bool GetKey(sf::Keyboard::Scancode key) const;

  /// @brief Checks if a specific key was released (went from pressed to not pressed) in the current frame.
  /// @param key The SFML scancode of the key to check.
  /// @return True if the key was released, false otherwise.
  [[nodiscard]] bool GetKeyUp(sf::Keyboard::Scancode key) const;

 private:
  // Stores the current state (pressed or not pressed) of each keyboard key.
  std::bitset<sf::Keyboard::ScancodeCount> key_states_;
  // Stores the previous frame's state of each keyboard key, used for detecting key down and up events.
  std::bitset<sf::Keyboard::ScancodeCount> old_key_states_;
};

}  // namespace ng