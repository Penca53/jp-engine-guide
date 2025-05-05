#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "engine/app.h"
#include "engine/node.h"
#include "engine/rectangle_collider.h"
#include "engine/tilemap.h"

namespace game {

class Mushroom : public ng::Node {
 public:
  Mushroom(ng::App* app, const ng::Tilemap* tilemap);

  bool GetIsDead() const;
  void TakeDamage();

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  sf::Vector2f direction_{-1, 0};
  sf::Vector2f velocity_;
  const ng::Tilemap* tilemap_ = nullptr;
  const ng::RectangleCollider* collider_ = nullptr;
  sf::Sprite sprite_;
  bool is_on_ground_ = false;
  bool is_dead_ = false;
};

}  // namespace game