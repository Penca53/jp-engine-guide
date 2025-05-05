#include "scene.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <memory>
#include <string>
#include <utility>

#include "app.h"
#include "camera.h"
#include "camera_manager.h"
#include "layer.h"
#include "node.h"
#include "physics.h"

namespace ng {

Scene::Scene(App* app) : root_(std::make_unique<Node>(app)) {
  assert(app);
  root_->SetName("SceneRoot");
  // Render all layers by default on the root node.
  root_->SetLayer(static_cast<Layer>(~0ULL));
}

const std::string& Scene::GetName() const {
  return name_;
}

void Scene::SetName(std::string name) {
  name_ = std::move(name);
}

CameraManager& Scene::GetCameraManager() {
  return camera_manager_;
}

const Physics& Scene::GetPhysics() {
  return physics_;
}

Physics& Scene::GetMutablePhysics() {
  return physics_;
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
  for (const Camera* camera : camera_manager_.GetCameras()) {
    target.setView(camera->GetView());
    root_->InternalDraw(*camera, target);
  }
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

void Scene::OnWindowResize(sf::Vector2u size) {
  camera_manager_.OnWindowResize(sf::Vector2f(size));
}

}  // namespace ng
