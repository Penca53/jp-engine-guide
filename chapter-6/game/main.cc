#include "default_scene.h"
#include "engine/app.h"

#include <SFML/System/Vector2.hpp>
#include <cstdlib>

int main() {
  static constexpr sf::Vector2u kWindowSize = {832U, 640U};
  ng::App app(kWindowSize, "Platformer", 60, 60);
  app.LoadScene(game::MakeDefaultScene(&app)).Run();
  return EXIT_SUCCESS;
}
