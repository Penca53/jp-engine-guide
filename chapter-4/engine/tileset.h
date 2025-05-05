#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <unordered_map>

#include "tile.h"

namespace ng {

/// @brief Manages a collection of tiles and their associated texture within a texture atlas.
class Tileset {
 public:
  /// @brief Constructs a Tileset with a specified tile size and texture.
  /// @param tile_size The dimensions (width and height) of each individual tile in the texture.
  /// @param texture A pointer to the SFML Texture containing the tiles. This pointer must not be null and the Texture's lifetime should be managed externally to Tileset.
  Tileset(sf::Vector2u tile_size, const sf::Texture* texture);

  /// @brief Retrieves a specific tile from the tileset based on its ID.
  /// @param id The TileID of the tile to retrieve.
  /// @return A constant reference to the Tile object with the given ID.
  [[nodiscard]] const Tile& GetTile(TileID id) const;

  /// @brief Adds a new tile to the tileset. If a tile with the same ID already exists, it will be overwritten.
  /// @param tile The Tile object to add to the tileset.
  void AddTile(Tile tile);

  /// @brief Returns the size of individual tiles in the tileset.
  /// @return The tile dimensions in pixels.
  [[nodiscard]] sf::Vector2u GetTileSize() const;

  /// @brief Returns the texture atlas associated with this tileset.
  /// @return A constant pointer to the SFML Texture. This pointer is never null after construction.
  [[nodiscard]] const sf::Texture* GetTexture() const;

 private:
  // The dimensions of each tile in the texture atlas.
  sf::Vector2u tile_size_;
  // Pointer to the texture atlas containing the tiles. This pointer is never null after construction.
  const sf::Texture* texture_ = nullptr;
  // Stores the tiles, indexed by their unique ID.
  std::unordered_map<TileID, Tile> tiles_;
};

}  // namespace ng