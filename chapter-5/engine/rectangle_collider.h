#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "collider.h"

namespace ng {

/// @brief Represents a rectangular collider for physics interactions.
class RectangleCollider : public Collider {
 public:
  /// @brief Constructs a RectangleCollider with the specified size.
  /// @param app A pointer to the App instance this collider belongs to. This pointer must not be null.
  /// @param size The size of the rectangle collider.
  RectangleCollider(App* app, sf::Vector2f size);

  /// @brief Returns the size of the rectangle collider.
  /// @return A constant reference to the size vector.
  [[nodiscard]] const sf::Vector2f& GetSize() const;

  /// @brief Checks for collision with another Collider. Uses double-dispatch.
  /// @param other A constant reference to the other Collider.
  /// @return True if a collision occurs, false otherwise.
  [[nodiscard]] bool Collides(const Collider& other) const override;

  /// @brief Checks for collision with a CircleCollider.
  /// @param other A constant reference to the other CircleCollider.
  /// @return True if a collision occurs, false otherwise.
  [[nodiscard]] bool Collides(const CircleCollider& other) const override;

  /// @brief Checks for collision with another RectangleCollider.
  /// @param other A constant reference to the other RectangleCollider.
  /// @return True if a collision occurs, false otherwise.
  [[nodiscard]] bool Collides(const RectangleCollider& other) const override;

 protected:
#ifndef NDEBUG
  /// @brief Draw the collider's bounds for debugging purposes.
  /// @param target The SFML RenderTarget to draw to.
  void Draw(sf::RenderTarget& target) override;
#endif

 private:
  // The size of the rectangle collider.
  sf::Vector2f size_;
};

}  // namespace ng