#include "scene.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <cassert>
#include <memory>
#include <string>
#include <utility>

#include "app.h"
#include "node.h"

namespace ng {

Scene::Scene(App* app) : root_(std::make_unique<Node>(app)) {
  assert(app);
  root_->SetName("SceneRoot");
}

const std::string& Scene::GetName() const {
  return name_;
}

void Scene::SetName(std::string name) {
  name_ = std::move(name);
}

void Scene::AddChild(std::unique_ptr<Node> new_child) {
  root_->AddChild(std::move(new_child));
}

bool Scene::IsValid(const Node* node) const {
  return scene_nodes_.contains(node);
}

void Scene::InternalOnAdd() {
  root_->InternalOnAdd(this);
}

void Scene::InternalUpdate() {
  root_->InternalUpdate();
}

void Scene::InternalDraw(sf::RenderTarget& target) {
  root_->InternalDraw(target);
}

void Scene::InternalOnDestroy() {
  root_->InternalOnDestroy();
}

void Scene::RegisterNode(const Node* node) {
  assert(node);
  scene_nodes_.insert(node);
}

void Scene::UnregisterNode(const Node* node) {
  assert(node);
  scene_nodes_.erase(node);
}

}  // namespace ng
