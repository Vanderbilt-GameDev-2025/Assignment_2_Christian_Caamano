#include "proximityoptimization.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/viewport.hpp>

using namespace godot;

ProximityOptimization::ProximityOptimization() {
    // Constructor is minimal since most setup happens in _ready
}

ProximityOptimization::~ProximityOptimization() {
    // Clean up if needed
}

void ProximityOptimization::_bind_methods() {
    // Property bindings
    ClassDB::bind_method(D_METHOD("set_process_radius", "radius"), &ProximityOptimization::set_process_radius);
    ClassDB::bind_method(D_METHOD("get_process_radius"), &ProximityOptimization::get_process_radius);
    ClassDB::bind_method(D_METHOD("set_target_path", "path"), &ProximityOptimization::set_target_path);
    ClassDB::bind_method(D_METHOD("get_target_path"), &ProximityOptimization::get_target_path);
    ClassDB::bind_method(D_METHOD("set_debug_enabled", "enabled"), &ProximityOptimization::set_debug_enabled);
    ClassDB::bind_method(D_METHOD("get_debug_enabled"), &ProximityOptimization::get_debug_enabled);
    ClassDB::bind_method(D_METHOD("get_active_node_count"), &ProximityOptimization::get_active_node_count);
    ClassDB::bind_method(D_METHOD("get_last_process_time"), &ProximityOptimization::get_last_process_time);
    ClassDB::bind_method(D_METHOD("force_update_states"), &ProximityOptimization::force_update_states);

    // Register properties for the editor
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "process_radius"), "set_process_radius", "get_process_radius");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "target_path"), "set_target_path", "get_target_path");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "debug_enabled"), "set_debug_enabled", "get_debug_enabled");
}

void ProximityOptimization::_ready() {
    // Initial setup when node enters scene tree
    if (!target_path.is_empty()) {
        target_node = Object::cast_to<Node3D>(get_node_or_null(target_path));
        if (!target_node) {
            UtilityFunctions::print("ProximityOptimization: Target node not found or not a Node3D!", "proximityoptimization.cpp", __LINE__);
        }
    }
}

void ProximityOptimization::_process(double delta) {
    if (!target_node) {
        return;
    }

    uint64_t start_time = Engine::get_singleton()->get_physics_frames();
    
    update_processing_states();
    
    if (is_debug_enabled) {
        debug_draw();
    }

    last_process_time = (Engine::get_singleton()->get_physics_frames() - start_time) * delta;
}

void ProximityOptimization::update_processing_states() {
    active_nodes = 0;
    
    // Get all nodes in the scene
    SceneTree* scene_tree = Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
    if (!scene_tree) return;

    Viewport* root = (Viewport*)scene_tree->get_root();
    if (!root) return;

    // Recursively process all Node3D nodes
    TypedArray<Node> children = root->get_children();
    for (int i = 0; i < children.size(); i++) {
        Node3D* spatial = Object::cast_to<Node3D>(children[i]);
        if (spatial && spatial != target_node && spatial != this) {
            bool should_process = is_node_in_range(spatial);
            spatial->set_process(should_process);
            spatial->set_physics_process(should_process);
            
            if (should_process) {
                active_nodes++;
            }
        }
    }
}

bool ProximityOptimization::is_node_in_range(const Node3D* node) const {
    if (!target_node || !node) {
        return false;
    }

    Vector3 distance = target_node->get_global_position() - node->get_global_position();
    return distance.length() <= process_radius;
}

void ProximityOptimization::debug_draw() const {
    // Note: In a real implementation, you would want to draw debug visualization
    // using immediate geometry or debug drawing utilities
    // This is just a placeholder for where you'd implement visualization
    if (is_debug_enabled && target_node) {
        // Draw debug sphere or boundary visualization
        UtilityFunctions::print_verbose("Debug: Active nodes: " + String::num(active_nodes));
    }
}

// Getters and setters
void ProximityOptimization::set_process_radius(double radius) {
    process_radius = radius;
    force_update_states();
}

double ProximityOptimization::get_process_radius() const {
    return process_radius;
}

void ProximityOptimization::set_target_path(const NodePath& path) {
    target_path = path;

    if (is_inside_tree()) {
        target_node = Object::cast_to<Node3D>(get_node_or_null(target_path));
    }
}

NodePath ProximityOptimization::get_target_path() const {
    return target_path;
}

void ProximityOptimization::set_debug_enabled(bool enabled) {
    is_debug_enabled = enabled;
}

bool ProximityOptimization::get_debug_enabled() const {
    return is_debug_enabled;
}

uint32_t ProximityOptimization::get_active_node_count() const {
    return active_nodes;
}

double ProximityOptimization::get_last_process_time() const {
    return last_process_time;
}

void ProximityOptimization::force_update_states() {
    if (is_inside_tree()) {
        update_processing_states();
    }
}