#include "physics.h"

#include <cassert>
#include <unordered_set>
#include <vector>

#include "collider.h"

namespace ng {

std::vector<const Collider*> Physics::Overlap(const Collider& collider) const {
  std::vector<const Collider*> collisions;
  for (const auto* other : colliders_) {
    if (other == &collider) {
      continue;
    }

    if (collider.Collides(*other)) {
      collisions.push_back(other);
    }
  }

  return collisions;
}

void Physics::AddCollider(const Collider* collider) {
  assert(collider);
  colliders_.insert(collider);
}

void Physics::RemoveCollider(const Collider* collider) {
  assert(collider);
  colliders_.erase(collider);
}

}  // namespace ng