#include "resource_manager.h"

#include <SFML/Graphics/Texture.hpp>
#include <filesystem>

namespace ng {

sf::Texture& ResourceManager::LoadTexture(
    const std::filesystem::path& filename) {
  std::filesystem::path full_path =
      std::filesystem::absolute(kPrefix_ / filename);

  auto it = textures_.find(full_path);
  if (it == textures_.end()) {
    it = textures_.insert({full_path, sf::Texture(full_path)}).first;
  }

  return it->second;
}

}  // namespace ng