#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <memory>
#include <string>
#include <unordered_set>

#include "derived.h"
#include "node.h"
#include "physics.h"

namespace ng {

class App;

/// @brief Represents a self-contained part of the game world managing nodes.
class Scene {
 public:
  // App needs to be able to call InternalOnAdd, InternalUpdate,
  // InternalDraw, and InternalOnDestroy.
  friend class App;
  // Node needs to be able to call RegisterNode, and UnregisterNode.
  friend class Node;

  /// @brief Constructs a Scene associated with a specific App instance.
  /// @param app A pointer to the App instance this scene belongs to. This pointer must not be null.
  explicit Scene(App* app);

  /// @brief Returns the name of the scene.
  /// @return A constant reference to the scene's name.
  [[nodiscard]] const std::string& GetName() const;

  /// @brief Sets the name of the scene.
  /// @param name The new name for the scene.
  void SetName(std::string name);

  /// @brief Returns a constant reference to the Physics engine for the scene.
  /// @return A constant reference to the Physics engine.
  [[nodiscard]] const Physics& GetPhysics();

  /// @brief Returns a mutable reference to the Physics engine for the scene.
  /// @return A mutable reference to the Physics engine.
  [[nodiscard]] Physics& GetMutablePhysics();

  /// @brief Adds a new child node to the root of the scene. Ownership of the node is transferred to the scene.
  /// @param new_child A unique pointer to the Node to be added. This pointer must not be null.
  void AddChild(std::unique_ptr<Node> new_child);

  /// @brief Creates and adds a new child node of the specified type to the root of the scene.
  /// @tparam T The type of the Node to create, must derive from Node.
  /// @tparam Args The constructor arguments for the Node type T.
  /// @param args The arguments to forward to the constructor of T.
  /// @return A reference to the newly created and added Node.
  template <Derived<Node> T, typename... Args>
  T& MakeChild(Args&&... args) {
    return root_->MakeChild<T>(std::forward<Args>(args)...);
  }

  /// @brief Checks if a given Node pointer is currently registered within this scene.
  /// @param node A pointer to the Node to check. Can be null (no-op).
  /// @return True if the Node is valid and belongs to this scene, false otherwise.
  bool IsValid(const Node* node) const;

 private:
  /// @brief Internal method called when the scene is added to the App. Notifies the root node.
  void InternalOnAdd();
  /// @brief Internal method called during the game loop to update the scene's logic. Updates the root node.
  void InternalUpdate();
  /// @brief Internal method called during the game loop to draw the scene. Draws the root node.
  /// @param target The SFML RenderTarget to draw to.
  void InternalDraw(sf::RenderTarget& target);
  /// @brief Internal method called when the scene is about to be destroyed or unloaded. Notifies the root node.
  void InternalOnDestroy();

  /// @brief Registers a Node with the scene, allowing for quick validation. Called by Node during its addition to the hierarchy.
  /// @param node A pointer to the Node being registered. This pointer must not be null.
  void RegisterNode(const Node* node);
  /// @brief Unregisters a Node from the scene. Called by Node during its removal from the hierarchy.
  /// @param node A pointer to the Node being unregistered. This pointer must not be null.
  void UnregisterNode(const Node* node);

  // The name of the scene.
  std::string name_;

  // Handles the physics simulation for the scene.
  Physics physics_;

  // A set containing all Nodes currently registered in the scene for fast lookup.
  std::unordered_set<const Node*> scene_nodes_;

  // The root node of the scene graph. All entities in the scene are descendants of this node.
  // Ownership is managed by the Scene. This pointer is never null after construction.
  std::unique_ptr<Node> root_;
};

}  // namespace ng