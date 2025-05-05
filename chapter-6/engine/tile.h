#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <cstdint>
#include <optional>

/// @brief The unique identifier for all the tiles. The user of the engine must define the enum
//         to use Tile related operations (i.e. Tilemap).
enum class TileID : uint64_t;

namespace ng {

/// @brief Represents a single tile in a tilemap, containing its ID and optional texture coordinates.
class Tile {
 public:
  /// @brief Constructs a Tile with only an ID. The texture coordinates will be empty.
  /// @param id The unique identifier for this tile.
  explicit Tile(TileID id);

  /// @brief Constructs a Tile with an ID and its corresponding texture coordinates.
  /// @param id The unique identifier for this tile.
  /// @param texture_coords The rectangular coordinates within a texture atlas for this tile.
  Tile(TileID id, sf::IntRect texture_coords);

  /// @brief Returns the unique identifier of the tile.
  /// @return The TileID of this tile.
  [[nodiscard]] TileID GetID() const;

  /// @brief Returns the optional texture coordinates of the tile within a texture atlas.
  /// @return A constant reference to an optional sf::IntRect. It will contain the texture coordinates if set, or be empty otherwise.
  [[nodiscard]] const std::optional<sf::IntRect>& GetTextureCoords() const;

 private:
  // The unique identifier of the tile.
  TileID id_{};
  // Optional texture coordinates within a texture atlas. Empty if the tile doesn't have specific texture coordinates.
  std::optional<sf::IntRect> texture_coords_;
};

}  // namespace ng