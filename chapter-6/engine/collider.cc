#include "collider.h"

#include "node.h"
#include "scene.h"

namespace ng {

Collider::Collider(App* app) : Node(app) {}

void Collider::OnAdd() {
  GetScene()->GetMutablePhysics().AddCollider(this);
}

void Collider::OnDestroy() {
  GetScene()->GetMutablePhysics().RemoveCollider(this);
}

}  // namespace ng