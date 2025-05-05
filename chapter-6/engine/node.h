#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "derived.h"
#include "layer.h"

namespace ng {

class App;
class Camera;
class Scene;

/// @brief The base class for all entities in the game world, forming a scene graph.
///        Manages local and global transformations, parent-child relationships, and rendering layers.
class Node {
 public:
  // Scene needs to be able to call InternalOnAdd, InternalUpdate,
  // InternalDraw, and InternalOnDestroy.
  friend class Scene;

  /// @brief Constructs a Node associated with a specific App instance.
  /// @param app A pointer to the App instance this node belongs to. This pointer must not be null.
  explicit Node(App* app);
  virtual ~Node() = default;

  Node(const Node& other) = delete;
  Node& operator=(const Node& other) = delete;
  Node(Node&& other) = delete;
  Node& operator=(Node&& other) = delete;

  /// @brief Returns the name of the node.
  /// @return A constant reference to the node's name.
  [[nodiscard]] const std::string& GetName() const;

  /// @brief Sets the name of the node.
  /// @param name The new name for the node.
  void SetName(std::string name);

  /// @brief Returns the App instance associated with this node.
  /// @return A pointer to the App instance. This pointer is never null after construction.
  [[nodiscard]] App* GetApp() const;

  /// @brief Returns the Scene this node belongs to. Can be null if the node is not yet added to a scene.
  /// @return A pointer to the Scene, or null if the node is not part of a scene.
  [[nodiscard]] Scene* GetScene() const;

  /// @brief Returns the parent node in the scene graph. Can be null if this is the root node or not yet added as a child.
  /// @return A pointer to the parent Node, or null if there is no parent.
  [[nodiscard]] Node* GetParent() const;

  /// @brief Returns the rendering layer of this node.
  /// @return The Layer this node belongs to.
  [[nodiscard]] Layer GetLayer() const;

  /// @brief Sets the rendering layer of this node.
  /// @param layer The new Layer for this node.
  void SetLayer(Layer layer);

  /// @brief Adds a new child node to this node. Ownership of the child is transferred.
  /// @param new_child A unique pointer to the Node to be added. This pointer must not be null.
  void AddChild(std::unique_ptr<Node> new_child);

  /// @brief Creates and adds a new child node of the specified type to this node.
  /// @tparam T The type of the Node to create, must derive from Node.
  /// @tparam Args The constructor arguments for the Node type T.
  /// @param args The arguments to forward to the constructor of T.
  /// @return A reference to the newly created and added Node.
  template <Derived<Node> T, typename... Args>
  T& MakeChild(Args&&... args) {
    auto child = std::make_unique<T>(app_, std::forward<Args>(args)...);
    T& ref = *child;
    AddChild(std::move(child));
    return ref;
  }

  /// @brief Schedules a child node for destruction. The actual removal happens at the beginning of the next frame.
  /// @param child_to_destroy A constant reference to the child Node to be destroyed.
  void DestroyChild(const Node& child_to_destroy);

  /// @brief Schedules this node for destruction. The actual removal happens at the beginning of the next frame by its parent.
  void Destroy();

  /// @brief Returns the local transformation of this node.
  /// @return A constant reference to the local SFML Transformable.
  [[nodiscard]] const sf::Transformable& GetLocalTransform() const;

  /// @brief Returns the global transformation of this node, taking into account its parent's transformations.
  ///        This is calculated lazily and cached until the local transform changes.
  /// @return A constant reference to the global SFML Transformable.
  [[nodiscard]] const sf::Transformable& GetGlobalTransform() const;

  /// @brief Sets the local position of the node.
  /// @param position The new local position.
  void SetLocalPosition(sf::Vector2f position);

  /// @brief Sets the local rotation of the node.
  /// @param rotation The new local rotation.
  void SetLocalRotation(sf::Angle rotation);

  /// @brief Sets the local scale of the node.
  /// @param scale The new local scale.
  void SetLocalScale(sf::Vector2f scale);

  /// @brief Translates the node by a given delta vector in its local space.
  /// @param delta The translation vector.
  void Translate(sf::Vector2f delta);

  /// @brief Returns the first child node of a specific type.
  /// @tparam T The type of the child Node to retrieve, must derive from Node.
  /// @return A pointer to the first child of type T, or nullptr if no such child exists.
  template <Derived<Node> T>
  [[nodiscard]] T* GetChild() {
    for (const auto& child : children_) {
      T* c = dynamic_cast<T*>(child.get());
      if (c != nullptr) {
        return c;
      }
    }
    return nullptr;
  }

 protected:
  /// @brief Called when the node is added to a scene graph.
  virtual void OnAdd();
  /// @brief Called during the update phase of the game loop.
  virtual void Update();
  /// @brief Called during the draw phase of the game loop.
  /// @param target The SFML RenderTarget to draw to.
  virtual void Draw(sf::RenderTarget& target);
  /// @brief Called when the node is about to be destroyed or removed from the scene graph.
  virtual void OnDestroy();

 private:
  /// @brief Removes children that were scheduled for destruction in the previous frame.
  void EraseDestroyedChildren();
  /// @brief Adds children that were queued to be added in the previous frame.
  void AddQueuedChildren();

  /// @brief Internal method called when the node is added to a scene. Notifies the node and its children.
  /// @param scene A pointer to the Scene this node is being added to. This pointer must not be null.
  void InternalOnAdd(Scene* scene);
  /// @brief Internal method called during the update phase. Updates the node and its children.
  void InternalUpdate();
  /// @brief Internal method called during the draw phase. Draws the node and its children if they belong to the camera's render layers.
  /// @param camera The Camera used for rendering.
  /// @param target The SFML RenderTarget to draw to.
  void InternalDraw(const Camera& camera, sf::RenderTarget& target);
  /// @brief Internal method called when the node is about to be destroyed. Notifies the node and its children.
  void InternalOnDestroy();

  /// @brief Marks the global transform as dirty, forcing a recalculation on the next GetGlobalTransform call and propagating the dirty flag to children.
  void DirtyGlobalTransform();

  // The name of the node.
  std::string name_;
  // The local transformation of the node.
  sf::Transformable local_transform_;
  // The cached global transformation of the node. Mutable for lazy evaluation.
  mutable sf::Transformable global_transform_;
  // Flag indicating if the global transform needs to be recalculated. Mutable for lazy evaluation.
  mutable bool is_global_transform_dirty_ = false;

  // Pointer to the App instance. Never null after construction.
  App* app_ = nullptr;

  // Pointer to the parent node in the scene graph. Can be null for the root.
  Node* parent_ = nullptr;
  // Pointer to the Scene this node belongs to. Can be null if not yet added to a scene.
  Scene* scene_ = nullptr;

  // Vector of child nodes. Ownership is managed by this node.
  std::vector<std::unique_ptr<Node>> children_;
  // Indices of children to be erased in the next update cycle.
  std::vector<size_t> children_to_erase_;
  // Vector of children to be added in the next update cycle.
  std::vector<std::unique_ptr<Node>> children_to_add_;
  // The rendering layer of this node.
  Layer layer_ = Layer::kDefault;
};

}  // namespace ng