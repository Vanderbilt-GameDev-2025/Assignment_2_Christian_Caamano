#ifndef PROXIMITY_OPTIMIZATION_H
#define PROXIMITY_OPTIMIZATION_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

namespace godot {

class ProximityOptimization : public Node3D {
    GDCLASS(ProximityOptimization, Node3D)

private:
    // Core processing properties
    double process_radius = 20.0;  // Default radius in units
    bool is_debug_enabled = false;
    NodePath target_path;  // Usually the player
    Node3D* target_node = nullptr;
    
    // Performance tracking
    uint32_t active_nodes = 0;
    double last_process_time = 0.0;
    
    // Internal methods
    void update_processing_states();
    void debug_draw() const;

protected:
    static void _bind_methods();
    
public:
    ProximityOptimization();
    ~ProximityOptimization();
    
    // Core processing methods that modify the game loop
    virtual void _process(double delta) override;
    virtual void _ready() override;
    
    // Configuration methods
    void set_process_radius(double radius);
    double get_process_radius() const;
    
    void set_target_path(const NodePath& path);
    NodePath get_target_path() const;
    
    void set_debug_enabled(bool enabled);
    bool get_debug_enabled() const;
    
    // Utility methods
    bool is_node_in_range(const Node3D* node) const;
    void force_update_states();
    
    // Performance monitoring
    uint32_t get_active_node_count() const;
    double get_last_process_time() const;
};

}  // namespace godot

#endif // PROXIMITY_OPTIMIZATION_H