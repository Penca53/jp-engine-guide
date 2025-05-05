#include "app.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdint>

#include "input.h"
#include "resource_manager.h"

namespace ng {

App::App(sf::Vector2u window_size, const sf::String& window_title, uint32_t fps)
    : window_(sf::RenderWindow(sf::VideoMode(window_size), window_title)),
      fps_(fps) {
  window_.setFramerateLimit(fps_);
}

sf::RenderWindow& App::GetWindow() {
  return window_;
}

ResourceManager& App::GetResourceManager() {
  return resource_manager_;
}

Input& App::GetInput() {
  return input_;
}

}  // namespace ng