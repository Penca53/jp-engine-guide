#include "camera_manager.h"

#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <set>

#include "camera.h"

namespace ng {

void CameraManager::OnWindowResize(sf::Vector2f size) {
  for (Camera* camera : cameras_) {
    camera->SetViewSize(size);
  }
}

const std::set<Camera*, CameraManager::DrawOrderCompare>&
CameraManager::GetCameras() const {
  return cameras_;
}

void CameraManager::AddCamera(Camera* camera) {
  assert(camera);
  cameras_.insert(camera);
}

void CameraManager::RemoveCamera(Camera* camera) {
  assert(camera);
  cameras_.erase(camera);
}

}  // namespace ng
