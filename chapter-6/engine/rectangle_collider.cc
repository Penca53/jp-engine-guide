#include "rectangle_collider.h"

#ifndef NDEBUG
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#endif
#include <SFML/System/Vector2.hpp>

#include "app.h"
#include "circle_collider.h"
#include "collider.h"

namespace ng {

RectangleCollider::RectangleCollider(App* app, sf::Vector2f size)
    : Collider(app), size_(size) {
  SetName("RectangleCollider");
}

const sf::Vector2f& RectangleCollider::GetSize() const {
  return size_;
}

bool RectangleCollider::Collides(const Collider& other) const {
  return other.Collides(*this);
}

bool RectangleCollider::Collides(const CircleCollider& other) const {
  // Delegate collision check to the CircleCollider's specific implementation.
  return other.Collides(*this);
}

bool RectangleCollider::Collides(const RectangleCollider& other) const {
  sf::Vector2f pos = GetGlobalTransform().getPosition();
  sf::Vector2f otherPos = other.GetGlobalTransform().getPosition();
  // Calculate the extents of both rectangles in world space.
  sf::Vector2f halfSize =
      size_.componentWiseMul(GetGlobalTransform().getScale()) / 2.F;
  sf::Vector2f otherHalfSize =
      other.size_.componentWiseMul(other.GetGlobalTransform().getScale()) / 2.F;

  // Check for non-overlapping conditions on both x and y axes.
  bool AisToTheRightOfB = pos.x - halfSize.x > otherPos.x + otherHalfSize.x;
  bool AisToTheLeftOfB = pos.x + halfSize.x < otherPos.x - otherHalfSize.x;
  bool AisBelowB = pos.y - halfSize.y > otherPos.y + otherHalfSize.y;
  bool AisAboveB = pos.y + halfSize.y < otherPos.y - otherHalfSize.y;

  // If any of the non-overlapping conditions are true, then they are not colliding.
  return !(AisToTheRightOfB || AisToTheLeftOfB || AisAboveB || AisBelowB);
}

#ifndef NDEBUG
void RectangleCollider::Draw(sf::RenderTarget& target) {
  sf::RectangleShape shape(size_);
  shape.setOutlineColor(sf::Color(0, 255, 0, 150));
  shape.setOutlineThickness(2);
  shape.setFillColor(sf::Color::Transparent);
  shape.setOrigin(size_ / 2.F);
  target.draw(shape, GetGlobalTransform().getTransform());
}
#endif

}  // namespace ng