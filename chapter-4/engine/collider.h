#pragma once

#include "node.h"

namespace ng {

class CircleCollider;
class RectangleCollider;

/// @brief An abstract base class for all types of colliders used for physics interactions.
class Collider : public Node {
 public:
  /// @brief Constructs a Collider associated with a specific App instance.
  /// @param app A pointer to the App instance this collider belongs to. This pointer must not be null.
  explicit Collider(App* app);

  /// @brief Checks for collision with another Collider.
  /// This method uses double-dispatch to call the correct overload based on the runtime type of 'other'.
  /// @param other A constant reference to the other Collider.
  /// @return True if a collision occurs, false otherwise.
  [[nodiscard]] virtual bool Collides(const Collider& other) const = 0;

  /// @brief Checks for collision with a CircleCollider.
  /// @param other A constant reference to the other CircleCollider.
  /// @return True if a collision occurs, false otherwise.
  [[nodiscard]] virtual bool Collides(const CircleCollider& other) const = 0;

  /// @brief Checks for collision with a RectangleCollider.
  /// @param other A constant reference to the other RectangleCollider.
  /// @return True if a collision occurs, false otherwise.
  [[nodiscard]] virtual bool Collides(const RectangleCollider& other) const = 0;

 protected:
  void OnAdd() override;
  void OnDestroy() override;
};

}  // namespace ng