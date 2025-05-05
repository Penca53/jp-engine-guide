#pragma once

#include "engine/node.h"
#include "engine/tilemap.h"
#include "player.h"

namespace game {

class FollowPlayer : public ng::Node {
 public:
  FollowPlayer(ng::App* app, const Player* player, const ng::Tilemap* tilemap);

 protected:
  void OnAdd() override;
  void Update() override;

 private:
  void Follow();

  const Player* player_ = nullptr;
  const ng::Tilemap* tilemap_ = nullptr;
};

}  // namespace game