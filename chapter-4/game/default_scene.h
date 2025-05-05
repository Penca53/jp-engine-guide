#pragma once

#include <memory>

#include "engine/app.h"
#include "engine/scene.h"

namespace game {

std::unique_ptr<ng::Scene> MakeDefaultScene(ng::App* app);

}  // namespace game