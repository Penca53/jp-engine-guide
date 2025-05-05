#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <cstdlib>
#include <optional>

#include "engine/app.h"
#include "engine/input.h"
#include "player.h"

int main() {
  static constexpr sf::Vector2u kWindowSize = {800U, 600U};
  ng::App app(kWindowSize, "Platformer", 60);
  sf::RenderWindow& window = app.GetWindow();
  ng::Input& input = app.GetInput();

  static constexpr float kCircleRadius = 32.F;
  sf::CircleShape circle(kCircleRadius);
  circle.setOrigin({kCircleRadius, kCircleRadius});
  circle.setFillColor(sf::Color::Green);

  game::Player player(&app);

  while (window.isOpen()) {
    input.Advance();
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }

      input.Handle(*event);
    }

    player.Update();

    window.clear();

    window.draw(circle);
    player.Draw(window);

    window.display();
  }

  return EXIT_SUCCESS;
}
