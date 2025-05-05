#pragma once

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

#include "layer.h"
#include "node.h"

namespace ng {

/// @brief Represents a camera in the game world, defining the viewport and visible layers.
class Camera : public Node {
 public:
  /// @brief Constructs a Camera with a default draw order of 0 and rendering the default layer.
  /// @param app A pointer to the App instance this camera belongs to. This pointer must not be null.
  explicit Camera(App* app);

  /// @brief Constructs a Camera with a specified draw order and rendering layers.
  /// @param app A pointer to the App instance this camera belongs to. This pointer must not be null.
  /// @param draw_order The order in which this camera should be processed for drawing (lower values are processed first).
  /// @param layers A bitmask of Layer flags indicating which layers this camera should render.
  Camera(App* app, int32_t draw_order, Layer layers);

  /// @brief Returns the SFML View associated with this camera.
  /// @return A constant reference to the SFML View.
  [[nodiscard]] const sf::View& GetView() const;

  /// @brief Returns the draw order of this camera.
  /// @return The draw order value.
  [[nodiscard]] int32_t GetDrawOrder() const;

  /// @brief Returns the bitmask of layers that this camera renders.
  /// @return The Layer flags for rendering.
  [[nodiscard]] Layer GetRenderLayers() const;

  /// @brief Sets the size of the camera's view.
  /// @param size The new size of the view.
  void SetViewSize(sf::Vector2f size);

 protected:
  void OnAdd() override;
  void Update() override;
  void OnDestroy() override;

 private:
  // The SFML View representing the camera's viewport.
  sf::View view_;
  // The order in which this camera is processed for drawing.
  int32_t draw_order_ = 0;
  // Bitmask of layers this camera should render.
  Layer render_layers_ = Layer::kDefault;
};

}  // namespace ng