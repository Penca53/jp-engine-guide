#pragma once

#include <unordered_set>
#include <vector>

#include "collider.h"

namespace ng {

/// @brief Manages the physics simulation within a scene, primarily handling collision detection.
class Physics {
  // Collider needs to be able to call AddCollider, and RemoveCollider.
  friend class Collider;

 public:
  /// @brief Checks if a given collider overlaps with any other collider currently in the physics world.
  /// @param collider The Collider to check for overlaps.
  /// @return A vector of pointers to the Colliders that overlaps with the given collider, empty if no overlap is found.
  [[nodiscard]] std::vector<const Collider*> Overlap(
      const Collider& collider) const;

 private:
  /// @brief Adds a collider to the physics world for collision detection. Called by Collider during its addition to a scene.
  /// @param collider A pointer to the Collider to add. This pointer must not be null and the Collider's lifetime should be managed externally to this class.
  void AddCollider(const Collider* collider);

  /// @brief Removes a collider from the physics world. Called by Collider during its removal from a scene.
  /// @param collider A pointer to the Collider to remove. This pointer must not be null and the Collider's lifetime should be managed externally to this class.
  void RemoveCollider(const Collider* collider);

  // A set containing pointers to all colliders in the physics world. The Physics class does not own these pointers.
  std::unordered_set<const Collider*> colliders_;
};

}  // namespace ng