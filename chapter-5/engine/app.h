#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/String.hpp>
#include <cstdint>
#include <memory>

#include "input.h"
#include "resource_manager.h"
#include "scene.h"

namespace ng {

/// @brief The core application class, managing the game loop, window, resources, input, and scenes.
class App {
 public:
  /// @brief Constructs an App instance with the specified window size and title, ticks and frames per second.
  /// @param window_size The initial size of the game window.
  /// @param window_title The title of the game window.
  /// @param tps The target ticks per second (game logic updates).
  /// @param fps The target frames per second (rendering updates).
  App(sf::Vector2u window_size, const sf::String& window_title, uint32_t tps,
      uint32_t fps);
  ~App() = default;

  App(const App& other) = delete;
  App& operator=(const App& other) = delete;
  App(App&& other) = delete;
  App& operator=(App&& other) = delete;

  /// @brief Runs the main game loop.
  void Run();

  /// @brief Returns the duration of a single game tick in seconds.
  /// @return The time elapsed per tick.
  [[nodiscard]] std::chrono::duration<float> SecondsPerTick() const;

  /// @brief Returns the duration of a single game tick in nanoseconds.
  /// @return The time elapsed per tick.
  [[nodiscard]] std::chrono::nanoseconds NanosecondsPerTick() const;

  /// @brief Returns a constant reference to the SFML RenderWindow.
  /// @return A constant reference to the game window object.
  [[nodiscard]] const sf::RenderWindow& GetWindow() const;

  /// @brief Returns a reference to the ResourceManager for managing game assets.
  /// @return A reference to the ResourceManager.
  [[nodiscard]] ResourceManager& GetResourceManager();

  /// @brief Returns a constant reference to the Input manager.
  /// @return A constant reference to the Input manager.
  [[nodiscard]] const Input& GetInput() const;

  /// @brief Loads a new scene, replacing the currently active one. The old scene (if any) will be unloaded in the next frame.
  /// @param scene A unique pointer to the new Scene to load. Ownership is transferred to the App. This pointer must not be null.
  /// @return A reference to the App instance for method chaining.
  App& LoadScene(std::unique_ptr<Scene> scene);

  /// @brief Unloads the currently active scene. The unloading process will happen at the beginning of the next frame.
  void UnloadScene();

 private:
  /// @brief Polls for SFML window events and updates the input state.
  void PollInput();

  // The main SFML render window.
  sf::RenderWindow window_;

  // Target ticks per second for game logic updates.
  uint32_t tps_ = 0;
  // Target frames per second for rendering.
  uint32_t fps_ = 0;

  // Manages game resources like textures and sounds.
  ResourceManager resource_manager_;
  // Handles user input events.
  Input input_;

  // The currently active game scene. Can be null if no scene is loaded. Ownership is managed by the App.
  std::unique_ptr<Scene> scene_;
  // A scene scheduled to be loaded in the next frame. Ownership is managed by the App. Can be null.
  std::unique_ptr<Scene> scheduled_scene_to_load_;
  // Flag indicating if the current scene is scheduled for unloading.
  bool is_scene_unloading_scheduled_ = false;
};

}  // namespace ng