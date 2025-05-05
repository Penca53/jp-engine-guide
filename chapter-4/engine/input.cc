#include "input.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <utility>

namespace ng {

void Input::Advance() {
  old_key_states_ = key_states_;
}

void Input::Handle(const sf::Event& event) {
  if (const auto* pressed = event.getIf<sf::Event::KeyPressed>()) {
    key_states_[std::to_underlying(pressed->scancode)] = true;
  } else if (const auto* released = event.getIf<sf::Event::KeyReleased>()) {
    key_states_[std::to_underlying(released->scancode)] = false;
  }
}

bool Input::GetKeyDown(sf::Keyboard::Scancode key) const {
  return (key_states_[std::to_underlying(key)] &&
          !old_key_states_[std::to_underlying(key)]);
}

bool Input::GetKey(sf::Keyboard::Scancode key) const {
  return key_states_[std::to_underlying(key)];
}

bool Input::GetKeyUp(sf::Keyboard::Scancode key) const {
  return (!key_states_[std::to_underlying(key)] &&
          old_key_states_[std::to_underlying(key)]);
}

}  // namespace ng
