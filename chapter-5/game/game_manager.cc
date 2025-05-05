#include "game_manager.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "default_scene.h"
#include "engine/app.h"
#include "engine/node.h"
#include "lose_canvas.h"

namespace game {

GameManager::GameManager(ng::App* app)
    : ng::Node(app),
      lose_sound_(
          GetApp()->GetResourceManager().LoadSoundBuffer("Loose_2.wav")) {}

void GameManager::OnAdd() {
  auto& lose_canvas = GetParent()->MakeChild<LoseCanvas>();
  lose_canvas_ = &lose_canvas;

  state_ = State::PLAY;
}

void GameManager::Update() {
  if (state_ == State::WON || state_ == State::LOST) {
    if (GetApp()->GetInput().GetKeyDown(sf::Keyboard::Scancode::Enter)) {
      GetApp()->LoadScene(MakeDefaultScene(GetApp()));
      return;
    }
  }
}

void GameManager::Win() {
  if (state_ != State::PLAY) {
    return;
  }

  state_ = State::WON;
}

void GameManager::Lose() {
  if (state_ != State::PLAY) {
    return;
  }

  state_ = State::LOST;
  lose_canvas_->Enable();
  lose_sound_.play();
}

GameManager::State GameManager::GetState() const {
  return state_;
}

}  // namespace game