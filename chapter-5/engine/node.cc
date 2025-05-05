#include "node.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <span>
#include <string>
#include <utility>

#include "camera.h"
#include "layer.h"
#include "scene.h"

namespace ng {

Node::Node(App* app) : app_(app) {
  assert(app);
}

const std::string& Node::GetName() const {
  return name_;
}

void Node::SetName(std::string name) {
  name_ = std::move(name);
}

App* Node::GetApp() const {
  return app_;
}

Scene* Node::GetScene() const {
  return scene_;
}

Node* Node::GetParent() const {
  return parent_;
}

Layer Node::GetLayer() const {
  return layer_;
}

void Node::SetLayer(Layer layer) {
  layer_ = layer;
}

void Node::AddChild(std::unique_ptr<Node> new_child) {
  new_child->parent_ = this;
  new_child->DirtyGlobalTransform();
  children_to_add_.push_back(std::move(new_child));
}

void Node::DestroyChild(const Node& child_to_destroy) {
  for (size_t i = 0; i < children_.size(); ++i) {
    if (children_[i].get() == &child_to_destroy) {
      children_to_erase_.push_back(i);
      break;
    }
  }
}

void Node::Destroy() {
  if (parent_ != nullptr) {
    parent_->DestroyChild(*this);
  }
}

const sf::Transformable& Node::GetLocalTransform() const {
  return local_transform_;
}

const sf::Transformable& Node::GetGlobalTransform() const {
  if (is_global_transform_dirty_) {
    if (parent_ != nullptr) {
      sf::Transform new_transform =
          parent_->GetGlobalTransform().getTransform() *
          GetLocalTransform().getTransform();

      auto matrix = std::span(new_transform.getMatrix(), 16);

      float a00 = matrix[0];
      float a01 = matrix[4];
      float a02 = matrix[12];
      float a10 = matrix[1];
      float a11 = matrix[5];
      float a12 = matrix[13];

      sf::Angle rotation(sf::radians(std::atan2(-a01, a00)));
      sf::Vector2 scale(std::sqrt((a00 * a00) + (a10 * a10)),
                        std::sqrt((a01 * a01) + (a11 * a11)));
      sf::Vector2f position(a02, a12);

      global_transform_.setRotation(rotation);
      global_transform_.setScale(scale);
      global_transform_.setPosition(position);
    } else {
      global_transform_ = GetLocalTransform();
    }

    is_global_transform_dirty_ = false;
  }

  return global_transform_;
}

void Node::SetLocalPosition(sf::Vector2f position) {
  local_transform_.setPosition(position);
  DirtyGlobalTransform();
}

void Node::SetLocalRotation(sf::Angle rotation) {
  local_transform_.setRotation(rotation);
  DirtyGlobalTransform();
}

void Node::SetLocalScale(sf::Vector2f scale) {
  local_transform_.setScale(scale);
  DirtyGlobalTransform();
}

void Node::Translate(sf::Vector2f delta) {
  local_transform_.move(delta);
  DirtyGlobalTransform();
}

void Node::OnAdd() {}

void Node::Update() {}

void Node::Draw([[maybe_unused]] sf::RenderTarget& target) {}

void Node::OnDestroy() {}

void Node::EraseDestroyedChildren() {
  if (children_to_erase_.empty()) {
    return;
  }

  // This is required because a newly destroyed child may destroy a new child
  // from its parent (this node), invalidating the current children_to_erase_ vector.
  auto prev_frame_children_to_erase = std::move(children_to_erase_);
  std::ranges::sort(prev_frame_children_to_erase, std::greater<>());
  for (size_t to_erase : prev_frame_children_to_erase) {
    children_[to_erase]->InternalOnDestroy();
    children_.erase(children_.begin() + static_cast<int64_t>(to_erase));
  }
}

void Node::AddQueuedChildren() {
  // This is required because a newly added child may add a new child
  // to its parent (this node), invalidating the current children_to_add_ vector.
  auto prev_frame_children_to_add = std::move(children_to_add_);
  for (auto& to_add : prev_frame_children_to_add) {
    Node* tmp = to_add.get();
    children_.push_back(std::move(to_add));
    tmp->InternalOnAdd(scene_);
  }
}

void Node::InternalOnAdd(Scene* scene) {
  scene_ = scene;
  scene_->RegisterNode(this);
  OnAdd();
}

void Node::InternalUpdate() {
  EraseDestroyedChildren();
  AddQueuedChildren();

  Update();
  for (auto& child : children_) {
    child->InternalUpdate();
  }
}

void Node::InternalDraw(const Camera& camera, sf::RenderTarget& target) {
  if ((std::to_underlying(layer_) &
       std::to_underlying(camera.GetRenderLayers())) == 0) {
    return;
  }

  Draw(target);
  for (auto& child : children_) {
    child->InternalDraw(camera, target);
  }
}

void Node::InternalOnDestroy() {
  scene_->UnregisterNode(this);
  OnDestroy();
  for (auto& child : children_) {
    child->InternalOnDestroy();
  }
}

void Node::DirtyGlobalTransform() {
  if (is_global_transform_dirty_) {
    return;
  }

  is_global_transform_dirty_ = true;
  for (auto& child : children_) {
    child->DirtyGlobalTransform();
  }
}

}  // namespace ng