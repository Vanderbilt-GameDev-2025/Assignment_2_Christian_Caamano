#include "proximityoptimization.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/viewport.hpp>

using namespace godot;

void ProximityOptimization::_bind_methods() {
    // Property bindings
    ClassDB::bind_method(D_METHOD("set_reference_node_path", "path"), &ProximityOptimization::set_reference_node_path);
    ClassDB::bind_method(D_METHOD("get_reference_node_path"), &ProximityOptimization::get_reference_node_path);

    // Register properties for the editor
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "reference_node_path"), "set_reference_node_path", "get_reference_node_path");
}

void ProximityOptimization::_ready() {
    // Initialize target node.
    if (!reference_node_path.is_empty()) {
        reference_node = Object::cast_to<Node3D>(get_node_or_null(reference_node_path));
        if (!reference_node) {
            UtilityFunctions::print("ProximityOptimization: Reference node not found or not a Node3D!", "proximityoptimization.cpp", __LINE__);
        }
    }
}

void ProximityOptimization::_process(double delta) {
    if (!reference_node) {
        return;
    }
    
    // Get the root of the scene tree
    SceneTree* scene_tree = Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
    if (!scene_tree) return;

    Viewport* root = (Viewport*)scene_tree->get_root();
    if (!root) return;

    // Start recursive processing from the root
    process_proximal(root);
}

void ProximityOptimization::process_proximal(Node* node) {
    if (!node) return;
    
    Node3D* node3D = Object::cast_to<Node3D>(node);

    // Node must be a valid Node3D.
    if (node3D) {
        // Only selectively process Node3D's that are not the reference node and not the Node3D representing this GDExtension class.
        // (The reference node should always be processed no matter what.)
        if (node3D != reference_node && node3D != this) {
            bool should_process = is_node_in_range(node3D);
            node3D->set_process(should_process);
            node3D->set_physics_process(should_process);
        }
    }
    
    // Recursively process all children.
    TypedArray<Node> children = node->get_children();
    for (int i = 0; i < children.size(); i++) {
        process_proximal((Object::cast_to<Node3D>)(children[i]));
    }
}

bool ProximityOptimization::is_node_in_range(const Node3D* node) const {
    if (!reference_node || !node) {
        return false;
    }

    Vector3 distance = reference_node->get_global_position() - node->get_global_position();
    return distance.length() <= process_radius;
}

NodePath ProximityOptimization::get_reference_node_path() const {
    return reference_node_path;
}

void ProximityOptimization::set_reference_node_path(const NodePath& path) {
    reference_node_path = path;

    if (is_inside_tree()) {
        reference_node = Object::cast_to<Node3D>(get_node_or_null(reference_node_path));
    }
}