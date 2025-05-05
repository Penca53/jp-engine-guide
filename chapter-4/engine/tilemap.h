#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "app.h"
#include "node.h"
#include "tile.h"
#include "tileset.h"

namespace ng {

/// @brief Represents a grid-based map composed of tiles from a Tileset.
class Tilemap : public Node {
 public:
  /// @brief Constructs a Tilemap with the specified size and tileset.
  /// @param app A pointer to the App instance this tilemap belongs to. This pointer must not be null.
  /// @param size The dimensions of the tilemap in tiles (width and height).
  /// @param tileset The Tileset to use for rendering the tiles. Ownership is transferred to the Tilemap.
  Tilemap(App* app, sf::Vector2u size, Tileset tileset);

  /// @brief Returns the size of the tilemap in tiles.
  /// @return The dimensions of the tilemap as an sf::Vector2u.
  [[nodiscard]] sf::Vector2u GetSize() const;

  /// @brief Returns the size of individual tiles used by this tilemap.
  /// @return The tile dimensions as an sf::Vector2u.
  [[nodiscard]] sf::Vector2u GetTileSize() const;

  /// @brief Checks if a given tile position (in tile coordinates) is within the bounds of the tilemap.
  /// @param position The tile coordinates to check.
  /// @return True if the position is within the bounds, false otherwise.
  [[nodiscard]] bool IsWithinBounds(sf::Vector2u position) const;

  /// @brief Returns the Tile at the specified tile coordinates.
  /// @param position The tile coordinates to retrieve the tile from.
  /// @return A constant reference to the Tile at the given position. Throws std::out_of_range if the position is out of bounds.
  [[nodiscard]] const Tile& GetTile(sf::Vector2u position) const;

  /// @brief Sets the Tile at the specified tile coordinates using its TileID.
  /// @param position The tile coordinates to set the tile at.
  /// @param tile_id The ID of the tile to set.
  void SetTile(sf::Vector2u position, TileID tile_id);

  /// @brief Checks if a given world position is within the bounds of the tilemap.
  /// @param world_position The world coordinates to check.
  /// @return True if the world position corresponds to a tile within the bounds, false otherwise.
  [[nodiscard]] bool IsWithinWorldBounds(sf::Vector2f world_position) const;

  /// @brief Returns the Tile at the specified world coordinates.
  /// @param world_position The world coordinates to retrieve the tile from.
  /// @return A constant reference to the Tile at the given world position.
  [[nodiscard]] const Tile& GetWorldTile(sf::Vector2f world_position) const;

  /// @brief Sets the Tile at the specified world coordinates using its TileID.
  /// @param world_position The world coordinates to set the tile at.
  /// @param tile_id The ID of the tile to set.
  void SetWorldTile(sf::Vector2f world_position, TileID tile_id);

  /// @brief Converts world coordinates to tile coordinates.
  /// @param world_position The world coordinates to convert.
  /// @return The corresponding tile coordinates.
  [[nodiscard]] sf::Vector2u WorldToTileSpace(
      sf::Vector2f world_position) const;

 protected:
  /// @brief Renders the tilemap.
  /// @param target The SFML RenderTarget to draw to.
  void Draw(sf::RenderTarget& target) override;

 private:
  // The dimensions of the tilemap in tiles.
  sf::Vector2u size_;
  // The tileset used by this tilemap. Ownership is held by the Tilemap.
  Tileset tileset_;
  // A vector storing the TileID for each tile in the map.
  std::vector<TileID> tiles_;
  // The vertex array used for rendering the tilemap efficiently.
  sf::VertexArray vertices_;
};

}  // namespace ng