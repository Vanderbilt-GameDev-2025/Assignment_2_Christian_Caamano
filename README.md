# Proximity-based optimization using GDExtension

![Picture of the demo scene.](gameplay_thumbnail.png)

Author: Christian Caamano

Email: christian.d.caamano@vanderbilt.edu

VUNetID: caamancd

Class: CS 3981 - Game Engine Design

Instructor: Bobby Bodenheimer


#### Link to demo video: https://youtu.be/1ow7wiF7yVE

## Description & Functionality

Module name: ProximityOptimization

Description: This Godot module optimizes the game loop by selectively processing nodes within a certain distance from a reference in a 3D scene. In other words, given a reference Node3D, processing will only occur for Node3Ds located within a certain radius of that reference. Two Node3Ds are exempted from selective processing: (1) the node in the scene that acts as the container for the ProximityOptimization module, and (2) the reference node. This module does not alter how these two nodes are processed in any way.

## Testing

The module was tested by setting up a scene with a playable character initially at the center of a floor, surrounded by blocks spaced 5 units apart along all major axes. The playable character was set as the reference node. An animation script was made and attached to each block that continuously changes its color whenever it is processed. The expected result during runtime was that only blocks within the processing radius (set to 20 units in the module code) would be changing their colors, while the rest would be static (no color change over time).

To confirm that the processing radius remained locked to the reference node, the player was moved around the scene using WASD. The expected result was that the static cubes the player moved toward would now dynamically change their colors when in the right proximity, while the ones the player moved away from would become static.

## Compilation Instructions

Compilation instructions are for MacOS devices.

To run the demo, perform the following steps:
1. Clone this repository to your device.
2. In a new terminal, cd to the root directory of the repository (Assignment_2_Christian_Caamano).
3. Run the command "scons platform=macos".
4. Open Godot, click "Import" and open the folder named "Proximity_Optimization".
5. Open this project in the editor and click play. Use WASD to move, spacebar to jump, and mouse to look around.