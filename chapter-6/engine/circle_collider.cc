#include "circle_collider.h"

#ifndef NDEBUG
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#endif
#include <SFML/System/Vector2.hpp>
#include <algorithm>

#include "app.h"
#include "collider.h"
#include "rectangle_collider.h"

namespace ng {

CircleCollider::CircleCollider(App* app, float radius)
    : Collider(app), radius_(radius) {
  SetName("CircleCollider");
}

float CircleCollider::GetRadius() const {
  return radius_;
}

bool CircleCollider::Collides(const Collider& other) const {
  return other.Collides(*this);
}

bool CircleCollider::Collides(const CircleCollider& other) const {
  float distanceSquared = (GetGlobalTransform().getPosition() -
                           other.GetGlobalTransform().getPosition())
                              .lengthSquared();
  float combinedRadius =
      (radius_ * std::max(GetGlobalTransform().getScale().x,
                          GetGlobalTransform().getScale().y)) +
      (other.radius_ * std::max(other.GetGlobalTransform().getScale().x,
                                other.GetGlobalTransform().getScale().y));
  return distanceSquared <= combinedRadius * combinedRadius;
}

bool CircleCollider::Collides(const RectangleCollider& other) const {
  sf::Vector2f pos = GetGlobalTransform().getPosition();
  sf::Vector2f other_pos = other.GetGlobalTransform().getPosition();
  // Calculate the half-extents of the rectangle in world space.
  float half_x =
      (other.GetSize().x * other.GetGlobalTransform().getScale().x) / 2;
  float half_y =
      (other.GetSize().y * other.GetGlobalTransform().getScale().y) / 2;

  // Find the closest point on the rectangle to the circle's center.
  float closest_x =
      std::max(other_pos.x - half_x, std::min(pos.x, other_pos.x + half_x));
  float closest_y =
      std::max(other_pos.y - half_y, std::min(pos.y, other_pos.y + half_y));

  float diff_x = pos.x - closest_x;
  float diff_y = pos.y - closest_y;
  float distance_squared = (diff_x * diff_x) + (diff_y * diff_y);

  float radius = radius_ * std::max(GetGlobalTransform().getScale().x,
                                    GetGlobalTransform().getScale().y);
  return distance_squared <= radius * radius;
}

#ifndef NDEBUG
void CircleCollider::Draw(sf::RenderTarget& target) {
  sf::CircleShape shape(radius_);
  shape.setOutlineColor(sf::Color(0, 255, 0, 150));
  shape.setOutlineThickness(2);
  shape.setFillColor(sf::Color::Transparent);
  shape.setOrigin(sf::Vector2f(radius_, radius_));
  target.draw(shape, GetGlobalTransform().getTransform());
}
#endif

}  // namespace ng