#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdlib>
#include <optional>

int main() {
  static constexpr sf::Vector2u kWindowSize = {800U, 600U};
  auto window = sf::RenderWindow(sf::VideoMode(kWindowSize), "Platformer");
  window.setFramerateLimit(60);

  static constexpr float kCircleRadius = 32.F;
  sf::CircleShape circle(kCircleRadius);
  circle.setOrigin({kCircleRadius, kCircleRadius});
  circle.setPosition(sf::Vector2f(kWindowSize) / 2.F);
  circle.setFillColor(sf::Color::Green);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    window.clear();
    window.draw(circle);
    window.display();
  }

  return EXIT_SUCCESS;
}
