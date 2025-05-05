#include "app.h"

#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>
#include <utility>

#include "input.h"
#include "resource_manager.h"
#include "scene.h"

namespace ng {

App::App(sf::Vector2u window_size, const sf::String& window_title, uint32_t tps,
         uint32_t fps)
    : window_(sf::RenderWindow(sf::VideoMode(window_size), window_title)),
      tps_(tps),
      fps_(fps) {
  window_.setFramerateLimit(fps_);
}

void App::Run() {
  auto previous = std::chrono::steady_clock::now();
  // Accumulator for unprocessed time.
  std::chrono::nanoseconds lag(0);
  while (window_.isOpen()) {
    auto current = std::chrono::steady_clock::now();

    std::chrono::duration elapsed = (current - previous);

    previous = current;
    lag += elapsed;

    if (is_scene_unloading_scheduled_) {
      scene_->InternalOnDestroy();
      scene_ = nullptr;
      is_scene_unloading_scheduled_ = false;
    }

    if (scheduled_scene_to_load_) {
      scene_ = std::move(scheduled_scene_to_load_);
      scene_->InternalOnAdd();
      scheduled_scene_to_load_ = nullptr;
    }

    PollInput();

    // Process game logic updates based on the target TPS.
    while (lag >= NanosecondsPerTick()) {
      if (scene_) {
        scene_->InternalUpdate();
      }
      lag -= NanosecondsPerTick();
    }

    window_.clear();

    // Draw the current scene if it exists.
    if (scene_) {
      scene_->InternalDraw(window_);
    }

    window_.display();
  }
}

std::chrono::duration<float> App::SecondsPerTick() const {
  using namespace std::chrono_literals;
  return std::chrono::duration<float>(1s) / tps_;  // NOLINT
}

std::chrono::nanoseconds App::NanosecondsPerTick() const {
  using namespace std::chrono_literals;
  return std::chrono::nanoseconds(1s) / tps_;  // NOLINT
}

const sf::RenderWindow& App::GetWindow() const {
  return window_;
}

ResourceManager& App::GetResourceManager() {
  return resource_manager_;
}

const Input& App::GetInput() const {
  return input_;
}

App& App::LoadScene(std::unique_ptr<Scene> scene) {
  scheduled_scene_to_load_ = std::move(scene);
  return *this;
}

void App::UnloadScene() {
  is_scene_unloading_scheduled_ = true;
}

void App::PollInput() {
  // Prepare the input handler for new events.
  input_.Advance();

  while (std::optional event = window_.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window_.close();
    } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
      if (scene_) {
        scene_->OnWindowResize(resized->size);
      }
    }

    // Pass the event to the input handler for processing.
    input_.Handle(*event);
  }
}

}  // namespace ng