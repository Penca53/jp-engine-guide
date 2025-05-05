#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "engine/circle_collider.h"
#include "engine/node.h"
#include "engine/tilemap.h"

namespace game {

class PlantBullet : public ng::Node {
 public:
  PlantBullet(ng::App* app, const ng::Tilemap* tilemap, sf::Vector2f direction);

  bool GetIsDead() const;

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  const ng::Tilemap* tilemap_ = nullptr;
  sf::Vector2f direction_{-1, 0};
  const ng::CircleCollider* collider_ = nullptr;
  sf::Sprite sprite_;
  bool is_dead_ = false;
};

}  // namespace game