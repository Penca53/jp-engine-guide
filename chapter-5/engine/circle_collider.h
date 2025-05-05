#pragma once

#include "collider.h"

namespace ng {

/// @brief Represents a circular collider for physics interactions.
class CircleCollider : public Collider {
 public:
  /// @brief Constructs a CircleCollider with the specified radius.
  /// @param app A pointer to the App instance this collider belongs to. This pointer must not be null.
  /// @param radius The radius of the circle collider.
  CircleCollider(App* app, float radius);

  /// @brief Returns the radius of the circle collider.
  /// @return The radius of the circle.
  [[nodiscard]] float GetRadius() const;

  /// @brief Checks for collision with another Collider. Uses double-dispatch.
  /// @param other A constant reference to the other Collider.
  /// @return True if a collision occurs, false otherwise.
  [[nodiscard]] bool Collides(const Collider& other) const override;

  /// @brief Checks for collision with another CircleCollider.
  /// @param other A constant reference to the other CircleCollider.
  /// @return True if a collision occurs, false otherwise.
  [[nodiscard]] bool Collides(const CircleCollider& other) const override;

  /// @brief Checks for collision with a RectangleCollider.
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
  // The radius of the circle collider.
  float radius_ = 0;
};

}  // namespace ng