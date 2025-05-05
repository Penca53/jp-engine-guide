#include "tile.h"

#include <SFML/Graphics/Rect.hpp>
#include <optional>

namespace ng {

Tile::Tile(TileID id) : id_(id) {}

Tile::Tile(TileID id, sf::IntRect texture_coords)
    : id_(id), texture_coords_(texture_coords) {}

TileID Tile::GetID() const {
  return id_;
}

const std::optional<sf::IntRect>& Tile::GetTextureCoords() const {
  return texture_coords_;
}

}  // namespace ng