#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>

namespace ng {

/// @brief Manages the loading and caching of game resources such as textures, sound buffers, and fonts.
///        Ensures that resources are loaded only once and provides access to them.
class ResourceManager {
 public:
  ResourceManager() = default;
  ~ResourceManager() = default;

  ResourceManager(const ResourceManager& other) = delete;
  ResourceManager& operator=(const ResourceManager& other) = delete;
  ResourceManager(ResourceManager&& other) = delete;
  ResourceManager& operator=(ResourceManager&& other) = delete;

  /// @brief Loads a texture from the specified file path. If the texture is already loaded, returns the cached instance.
  /// @param filename The relative path to the texture file.
  /// @return A reference to the loaded SFML Texture. Lifetime is bound to the resource manager instance.
  sf::Texture& LoadTexture(const std::filesystem::path& filename);

 private:
  /// @brief The prefix for all resource file paths.
  static constexpr std::string_view kPrefix_ = "resources/";

  /// @brief Cache for loaded textures, mapping file paths to SFML Textures.
  std::unordered_map<std::filesystem::path, sf::Texture> textures_;
};

}  // namespace ng