#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>

namespace ng {

/// @brief Manages the animation of an SFML Sprite using a sprite sheet texture.
class SpriteSheetAnimation {
 public:
  /// @brief Constructs a SpriteSheetAnimation with default frame size based on texture height.
  /// @param sprite A pointer to the SFML Sprite to animate. This pointer must not be null.
  /// @param texture A pointer to the SFML Texture containing the sprite sheet. This pointer must not be null.
  /// @param ticks_per_frame The number of game ticks to wait before advancing to the next frame.
  SpriteSheetAnimation(sf::Sprite* sprite, const sf::Texture* texture,
                       int32_t ticks_per_frame);

  /// @brief Constructs a SpriteSheetAnimation with a specified frame size.
  /// @param sprite A pointer to the SFML Sprite to animate. This pointer must not be null.
  /// @param texture A pointer to the SFML Texture containing the sprite sheet. This pointer must not be null.
  /// @param ticks_per_frame The number of game ticks to wait before advancing to the next frame.
  /// @param frame_size The size of each individual frame in the sprite sheet.
  SpriteSheetAnimation(sf::Sprite* sprite, const sf::Texture* texture,
                       int32_t ticks_per_frame, sf::Vector2i frame_size);

  /// @brief Returns the current frame index of the animation.
  /// @return The index of the currently displayed frame (0-based).
  [[nodiscard]] int32_t GetFrameIndex() const;

  /// @brief Returns the current value of the ticks counter for the current frame.
  /// @return The number of ticks elapsed since the current frame started displaying.
  [[nodiscard]] int32_t GetTicksCounter() const;

  /// @brief Registers a callback function to be executed when the animation completes a full cycle (returns to the first frame).
  /// @param on_end_callback A function object (e.g., std::function, lambda) to be called when the animation loops. Can be empty to unregister a previous callback.
  void RegisterOnEndCallback(std::function<void()> on_end_callback);

  /// @brief Starts the animation from the first frame and sets the sprite's texture.
  void Start();

  /// @brief Updates the animation, advancing to the next frame if the ticks per frame limit is reached.
  void Update();

 private:
  // Pointer to the SFML Sprite being animated. Never null after construction.
  sf::Sprite* sprite_ = nullptr;
  // Pointer to the SFML Texture containing the sprite sheet. Never null after construction.
  const sf::Texture* texture_ = nullptr;
  // Number of game ticks per animation frame.
  int32_t ticks_per_frame_ = 0;
  // The current frame index of the animation.
  int32_t frame_index_ = 0;
  // Counter for the number of ticks elapsed for the current frame.
  int32_t ticks_counter_ = 0;
  // The size of each individual frame in the sprite sheet.
  sf::Vector2i frame_size_;
  // The total number of frames in the sprite sheet animation.
  int32_t frames_count_ = 0;
  // Optional callback function to be executed when the animation loops.
  std::optional<std::function<void()>> on_end_callback_;
};

}  // namespace ng