#include "win_canvas.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "engine/app.h"
#include "engine/layer.h"
#include "engine/node.h"

namespace game {

WinCanvas::WinCanvas(ng::App* app)
    : ng::Node(app),
      title_text_(
          GetApp()->GetResourceManager().LoadFont("Roboto-Regular.ttf")),
      restart_text_(
          GetApp()->GetResourceManager().LoadFont("Roboto-Regular.ttf")) {
  SetName("WinCanvas");
  SetLayer(ng::Layer::kUI);

  background_.setFillColor(sf::Color(50, 50, 50, 200));

  title_text_.setString("YOU WON");
  title_text_.setCharacterSize(42);
  title_text_.setStyle(sf::Text::Bold);
  title_text_.setOrigin(title_text_.getGlobalBounds().size / 2.F);

  restart_text_.setString("(press ENTER to restart)");
  restart_text_.setCharacterSize(24);
  restart_text_.setStyle(sf::Text::Italic);
  restart_text_.setOrigin(restart_text_.getGlobalBounds().size / 2.F);
  restart_text_.setPosition(
      sf::Vector2f(0, title_text_.getGlobalBounds().size.y * 2));
}

void WinCanvas::Enable() {
  is_enabled_ = true;
}

void WinCanvas::Disable() {
  is_enabled_ = false;
}

void WinCanvas::Draw(sf::RenderTarget& target) {
  if (!is_enabled_) {
    return;
  }

  background_.setSize(sf::Vector2f(GetApp()->GetWindow().getSize()));
  background_.setOrigin(sf::Vector2f(GetApp()->GetWindow().getSize()) / 2.F);

  target.draw(background_, GetGlobalTransform().getTransform());
  target.draw(title_text_, GetGlobalTransform().getTransform());
  target.draw(restart_text_, GetGlobalTransform().getTransform());
}

}  // namespace game