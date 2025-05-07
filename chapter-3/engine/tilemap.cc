#include "tilemap.h"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>

#include "app.h"
#include "node.h"
#include "tile.h"
#include "tileset.h"

namespace ng {

static constexpr size_t kTriangleVertexCount = 3;
static constexpr size_t kTrisInQuad = 2 * kTriangleVertexCount;

Tilemap::Tilemap(App* app, sf::Vector2u size, Tileset tileset)
    : Node(app),
      size_(size),
      tileset_(std::move(tileset)),
      vertices_(sf::PrimitiveType::Triangles, static_cast<size_t>(size_.x) *
                                                  static_cast<size_t>(size_.y) *
                                                  kTrisInQuad) {
  tiles_.resize(static_cast<size_t>(size_.x) * static_cast<size_t>(size_.y));

  for (uint32_t y = 0; y < size_.y; ++y) {
    for (uint32_t x = 0; x < size_.x; ++x) {
      std::span<sf::Vertex> triangles =
          std::span(&vertices_[(y * size_.x + x) * kTrisInQuad], kTrisInQuad);

      sf::Vector2f tile_size = sf::Vector2f(tileset_.GetTileSize());
      auto fx = static_cast<float>(x);
      auto fy = static_cast<float>(y);

      triangles[0].position = sf::Vector2f(fx * tile_size.x, fy * tile_size.y);
      triangles[1].position =
          sf::Vector2f((fx + 1) * tile_size.x, fy * tile_size.y);
      triangles[2].position =
          sf::Vector2f(fx * tile_size.x, (fy + 1) * tile_size.y);
      triangles[3].position =
          sf::Vector2f(fx * tile_size.x, (fy + 1) * tile_size.y);
      triangles[4].position =
          sf::Vector2f((fx + 1) * tile_size.x, fy * tile_size.y);
      triangles[5].position =
          sf::Vector2f((fx + 1) * tile_size.x, (fy + 1) * tile_size.y);

      triangles[0].color = sf::Color::Transparent;
      triangles[1].color = sf::Color::Transparent;
      triangles[2].color = sf::Color::Transparent;
      triangles[3].color = sf::Color::Transparent;
      triangles[4].color = sf::Color::Transparent;
      triangles[5].color = sf::Color::Transparent;
    }
  }
}

sf::Vector2u Tilemap::GetSize() const {
  return size_;
}

sf::Vector2u Tilemap::GetTileSize() const {
  return tileset_.GetTileSize();
}

bool Tilemap::IsWithinBounds(sf::Vector2u position) const {
  return position.x < size_.x && position.y < size_.y;
}

const Tile& Tilemap::GetTile(sf::Vector2u position) const {
  return tileset_.GetTile(tiles_[(position.y * size_.x) + position.x]);
}

void Tilemap::SetTile(sf::Vector2u position, TileID tile_id) {
  tiles_[(position.y * size_.x) + position.x] = tile_id;

  std::span<sf::Vertex> triangles =
      std::span(&vertices_[(position.y * size_.x + position.x) * kTrisInQuad],
                kTrisInQuad);

  auto texture_coords = GetTile(position).GetTextureCoords();
  if (texture_coords.has_value()) {
    auto pos = sf::Vector2f(texture_coords->position);
    auto size = sf::Vector2f(texture_coords->size);

    triangles[0].texCoords = sf::Vector2f(pos.x, pos.y);
    triangles[1].texCoords = sf::Vector2f(pos.x + size.x, pos.y);
    triangles[2].texCoords = sf::Vector2f(pos.x, pos.y + size.y);
    triangles[3].texCoords = sf::Vector2f(pos.x, pos.y + size.y);
    triangles[4].texCoords = sf::Vector2f(pos.x + size.x, pos.y);
    triangles[5].texCoords = sf::Vector2f(pos.x + size.x, pos.y + size.y);

    triangles[0].color = sf::Color::White;
    triangles[1].color = sf::Color::White;
    triangles[2].color = sf::Color::White;
    triangles[3].color = sf::Color::White;
    triangles[4].color = sf::Color::White;
    triangles[5].color = sf::Color::White;
  } else {
    triangles[0].color = sf::Color::Transparent;
    triangles[1].color = sf::Color::Transparent;
    triangles[2].color = sf::Color::Transparent;
    triangles[3].color = sf::Color::Transparent;
    triangles[4].color = sf::Color::Transparent;
    triangles[5].color = sf::Color::Transparent;
  }
}

bool Tilemap::IsWithinWorldBounds(sf::Vector2f world_position) const {
  sf::Vector2f tilemap_relative_position =
      (world_position - GetGlobalTransform().getPosition());

  if (tilemap_relative_position.x < 0 || tilemap_relative_position.y < 0) {
    return false;
  }

  sf::Vector2u tile_position(tilemap_relative_position.componentWiseDiv(
      sf::Vector2f(tileset_.GetTileSize())));

  return IsWithinBounds(tile_position);
}

const Tile& Tilemap::GetWorldTile(sf::Vector2f world_position) const {
  return GetTile(WorldToTileSpace(world_position));
}

void Tilemap::SetWorldTile(sf::Vector2f world_position, TileID tile_id) {
  SetTile(WorldToTileSpace(world_position), tile_id);
}

sf::Vector2u Tilemap::WorldToTileSpace(sf::Vector2f world_position) const {
  return sf::Vector2u(
      (world_position - GetGlobalTransform().getPosition())
          .componentWiseDiv(sf::Vector2f(tileset_.GetTileSize())));
}

void Tilemap::Draw(sf::RenderTarget& target) {
  sf::RenderStates state;
  state.transform = GetGlobalTransform().getTransform();
  state.texture = tileset_.GetTexture();
  target.draw(vertices_, state);
}

}  // namespace ng