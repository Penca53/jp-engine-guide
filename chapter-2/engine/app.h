#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/String.hpp>
#include <cstdint>

#include "input.h"
#include "resource_manager.h"

namespace ng {

/// @brief The core application class, managing the window, resources, and input.
class App {
 public:
  /// @brief Constructs an App instance with the specified window size and title, and frames per second.
  /// @param window_size The initial size of the game window.
  /// @param window_title The title of the game window.
  /// @param fps The target frames per second (rendering updates).
  App(sf::Vector2u window_size, const sf::String& window_title, uint32_t fps);

  /// @brief Returns a reference to the SFML RenderWindow.
  /// @return A reference to the game window object.
  [[nodiscard]] sf::RenderWindow& GetWindow();

  /// @brief Returns a reference to the ResourceManager for managing game assets.
  /// @return A reference to the ResourceManager.
  [[nodiscard]] ResourceManager& GetResourceManager();

  /// @brief Returns a reference to the Input manager.
  /// @return A reference to the Input manager.
  [[nodiscard]] Input& GetInput();

 private:
  // The main SFML render window.
  sf::RenderWindow window_;
  // Manages game resources like textures and sounds.
  ResourceManager resource_manager_;
  // Handles user input events.
  Input input_;

  // Target frames per second for rendering.
  uint32_t fps_ = 0;
};

}  // namespace ng