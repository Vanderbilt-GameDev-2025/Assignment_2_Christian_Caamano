using Godot;
using System;

/// <summary>
/// Cycles the hue of a given MeshInstance3D's material over time. (Animation)
/// </summary>
public partial class ColorStrobe : MeshInstance3D
{
    /// <summary>
	/// The material to animate
	/// </summary>
	private ShaderMaterial material;

    /// <summary>
	/// Time elapsed since the start of _process
	/// </summary>
    private float timeElapsed = 0.0f;
    
    /// <summary>
	/// Control how fast the hue changes
	/// </summary>
    [Export] public float cycleSpeed = 0.5f;

	// Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        // Obtain the material or null.
		material = (ShaderMaterial)GetSurfaceOverrideMaterial(0);

		// Create new material if null.
        if (material == null)
        {
            material = new ShaderMaterial();
            string shader = @"
                shader_type spatial;
                
                uniform vec4 albedo : source_color = vec4(1.0);
                uniform float emission_strength : hint_range(0, 5) = 1.0;
                
                void fragment() {
                    ALBEDO = albedo.rgb;
                    EMISSION = albedo.rgb * emission_strength;
                }
            ";
            
            material.Shader = new Shader() { Code = shader };
            SetSurfaceOverrideMaterial(0, material);
        }
    }

	// Called once per frame.
    public override void _Process(double delta)
    {
        // Update time elapsed
        timeElapsed += (float)delta;
        
        // Calculate new hue and update material's albedo with this hue
		float hue = 1.0f - 2.0f * Mathf.Abs((timeElapsed * cycleSpeed % 1.0f) - 0.5f);
		Color color = Color.FromHsv(hue, 1.0f, 1.0f);
        material.SetShaderParameter("albedo", color);
    }
}