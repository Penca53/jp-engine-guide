#include "background.h"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <cstddef>
#include <cstdint>

#include "engine/app.h"

namespace game {

static constexpr size_t kTriangleVertexCount = 3;
static constexpr size_t kTrisInQuad = 2 * kTriangleVertexCount;

Background::Background(ng::App* app, sf::Vector2u size)
    : app_(app),
      size_(size),
      texture_(&app_->GetResourceManager().LoadTexture("Gray.png")),
      image_vertices_(sf::PrimitiveType::Triangles, kTrisInQuad) {
  assert(app);

  texture_->setRepeated(true);

  sf::Vector2f fsize(size_);
  image_vertices_[0].position = sf::Vector2f(0, 0);
  image_vertices_[1].position = sf::Vector2f(fsize.x, 0);
  image_vertices_[2].position = sf::Vector2f(0, fsize.y);
  image_vertices_[3].position = sf::Vector2f(0, fsize.y);
  image_vertices_[4].position = sf::Vector2f(fsize.x, 0);
  image_vertices_[5].position = sf::Vector2f(fsize.x, fsize.y);
}

void Background::Update() {
  static constexpr int32_t kScrollTicksPerPixel = 4;
  float offset = -static_cast<float>(t_) / kScrollTicksPerPixel;

  sf::Vector2f fsize(size_);
  image_vertices_[0].texCoords = sf::Vector2f(0, 0 + offset);
  image_vertices_[1].texCoords = sf::Vector2f(fsize.x, 0 + offset);
  image_vertices_[2].texCoords = sf::Vector2f(0, fsize.y + offset);
  image_vertices_[3].texCoords = sf::Vector2f(0, fsize.y + offset);
  image_vertices_[4].texCoords = sf::Vector2f(fsize.x, 0 + offset);
  image_vertices_[5].texCoords = sf::Vector2f(fsize.x, fsize.y + offset);

  t_ = (t_ + 1) %
       (static_cast<int32_t>(texture_->getSize().y) * kScrollTicksPerPixel);
}

void Background::Draw(sf::RenderTarget& target) {
  sf::RenderStates state;
  state.texture = texture_;
  target.draw(image_vertices_, state);
}

}  // namespace game