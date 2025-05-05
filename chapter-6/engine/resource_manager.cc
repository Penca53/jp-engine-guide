#include "resource_manager.h"

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
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

sf::SoundBuffer& ResourceManager::LoadSoundBuffer(
    const std::filesystem::path& filename) {
  std::filesystem::path full_path =
      std::filesystem::absolute(kPrefix_ / filename);

  auto it = sound_buffers_.find(full_path);
  if (it == sound_buffers_.end()) {
    it = sound_buffers_.insert({full_path, sf::SoundBuffer(full_path)}).first;
  }

  return it->second;
}

sf::Font& ResourceManager::LoadFont(const std::filesystem::path& filename) {
  std::filesystem::path full_path =
      std::filesystem::absolute(kPrefix_ / filename);

  auto it = fonts_.find(full_path);
  if (it == fonts_.end()) {
    it = fonts_.insert({full_path, sf::Font(full_path)}).first;
  }

  return it->second;
}

}  // namespace ng