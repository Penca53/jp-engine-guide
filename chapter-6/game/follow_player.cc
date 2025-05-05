#include "follow_player.h"

#include <SFML/System/Vector2.hpp>
#include <algorithm>

#include "engine/app.h"
#include "engine/node.h"
#include "engine/scene.h"
#include "engine/tilemap.h"
#include "player.h"

namespace game {

FollowPlayer::FollowPlayer(ng::App* app, const Player* player,
                           const ng::Tilemap* tilemap)
    : ng::Node(app), player_(player), tilemap_(tilemap) {}

void FollowPlayer::OnAdd() {
  Follow();
}

void FollowPlayer::Update() {
  Follow();
}

void FollowPlayer::Follow() {
  if (!GetScene()->IsValid(player_)) {
    return;
  }

  sf::Vector2f tilemap_size = sf::Vector2f(tilemap_->GetSize());
  sf::Vector2f tile_size = sf::Vector2f(tilemap_->GetTileSize());
  sf::Vector2f window_size = sf::Vector2f(GetApp()->GetWindow().getSize());
  sf::Vector2f player_pos = player_->GetGlobalTransform().getPosition();
  sf::Vector2f new_pos(
      std::min(
          std::max(player_pos.x, tile_size.x * window_size.x / tile_size.x / 2),
          tile_size.x * (tilemap_size.x - window_size.x / tile_size.x / 2)),
      std::min(
          std::max(player_pos.y, tile_size.y * window_size.y / tile_size.y / 2),
          tile_size.y * (tilemap_size.y - window_size.y / tile_size.y / 2)));
  GetParent()->SetLocalPosition(new_pos);
}

}  // namespace game