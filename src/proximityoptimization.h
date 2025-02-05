#ifndef PROXIMITY_OPTIMIZATION_H
#define PROXIMITY_OPTIMIZATION_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

namespace godot {

/**
 * Optimize the game loop by selectively processing nodes within a certain radius of a reference node.
 */
class ProximityOptimization : public Node3D {
    GDCLASS(ProximityOptimization, Node3D)

private:
    /**
     * Radius of the spherical volume within which node processing will occur.
     */
    double process_radius = 20.0;

    /**
     * Path in the scene tree of the node used as a reference point for all proximity checks.
     */
    NodePath reference_node_path;

    /**
     * The node used as a reference point for all proximity checks.
     * This is usually the player node.
     */
    Node3D* reference_node = nullptr;

    /**
     * Recursively process all nodes in the scene tree that are within the process radius.
     * @param node The current node in the scene tree being processed.
     * 
     * Post: All nodes in the scene tree within the process radius have been processed once more,
     *       while all nodes outside the radius have not been updated.
     */
    void process_proximal(Node* node);

protected:
    /**
     * Bind methods and register properties for the editor.
     */
    static void _bind_methods();
    
public:
    // Default constructor/destructor
    ProximityOptimization() = default;
    ~ProximityOptimization() = default;
    
    // Override the _ready and _process methods to modify the game loop
    virtual void _ready() override;
    virtual void _process(double delta) override;
    
    /**
     * Get the path in the scene tree hierarchy to the reference node.
     * @return The path in the scene tree hierarchy to the reference node.
     */
    NodePath get_reference_node_path() const;

    /**
     * Set the path in the scene tree hierarchy to the reference node.
     * @param path The path in the scene tree hierarchy to the reference node.
     */
    void set_reference_node_path(const NodePath& path);
    
    /**
     * Determine if the given node is within the process radius.
     * @param node The node under consideration.
     * @return True if the node is within the process radius, false otherwise.
     */
    bool is_node_in_range(const Node3D* node) const;
};

}  // namespace godot

#endif // PROXIMITY_OPTIMIZATION_H