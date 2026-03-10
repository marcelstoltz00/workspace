# COS344 Practical 2 Technical Implementation Guide

This guide explains exactly what to build, in what order, and which files to touch so you can satisfy every objective in README.

## 1) Current Project Baseline

You already have:
- OpenGL/GLFW/GLEW startup and render loop scaffold in main.cpp
- Math and shape templates in Matrix.h/.cpp, Vector.h/.cpp, Shape.h, Square.h/.cpp, Triangle.h/.cpp
- Shader loader in shader.cpp and shader.hpp
- Build script in makefile

What is still missing:
- Actual scene data and object model
- Circle implementation
- Shader source files (vertex + fragment)
- GPU upload/draw pipeline for multiple colored shapes
- Selection logic (1,2,3,4,0)
- Transform logic (W,A,S,D,+,-,Q,E)
- Wireframe toggle with GL_LINES and debounce

## 2) File Plan (What to Add / Edit)

### Existing files you will edit
- main.cpp: all runtime logic (scene setup, rendering, keyboard controls, transforms, wireframe mode)
- makefile: add any newly created .cpp files to files = ... if needed
- README.md: keep checklist updated while implementing

### New files you should create
- Circle.h
- Circle.cpp
- simple.vert
- simple.frag

Optional but recommended (for cleaner code):
- RenderObject.h
- RenderObject.cpp

If you keep everything inside main.cpp you can still pass, but optional RenderObject files make debugging easier.

## 3) Data Model You Need

Create one runtime struct/class to represent each drawable object in the scene.

Required fields:
- object id/name (example: ball, hole, obstacleWood, obstacleRock)
- category/type (ball, hole, obstacle1, obstacle2, decor)
- base geometry points (model space)
- transformed geometry points (world space)
- color and pastelColor
- selected flag
- primitive mode for normal rendering (GL_TRIANGLES for filled geometry)
- wireframe line index list for GL_LINES drawing
- transform state:
  - position (tx, ty)
  - scale (sx, sy)
  - rotation angle
  - pivot/center (cx, cy)

Why this matters:
- You must apply transforms cumulatively and in arbitrary order.
- You must switch selected color to pastel quickly.
- Wireframe should draw the same object data in GL_LINES.

## 4) Implement Circle Geometry

Create Circle.h/Circle.cpp similar to Square/Triangle template style.

Minimum API to support:
- constructor(center, radius, vertexCount)
- getPoints()
- getNumPoints()
- operator*= and operator* with Matrix

Implementation detail:
- Generate vertexCount points around center using angle step: 2*pi/vertexCount
- For filled circle rendering in OpenGL, store triangles as a fan:
  - center point
  - point i
  - point i+1

Required for marks:
- One circle with >= 50 vertices (hole or ball)
- One circle with 6-10 vertices (another circle object)

## 5) Create GLSL Shader Files

Create these files in project root:
- simple.vert
- simple.frag

Vertex shader responsibilities:
- receive vec2 position (location 0)
- pass through clip-space position

Fragment shader responsibilities:
- uniform vec3 inColor
- output vec4(inColor, 1.0)

In main.cpp:
- call LoadShaders("simple.vert", "simple.frag")
- cache uniform location for inColor

## 6) Build Scene Layout (Marks: Layout + Colours)

In main.cpp, create a function like buildScene() and instantiate all required objects:

Required layout objects:
- concrete floor (grey rectangle, not full screen)
- start point (maroon rectangle)
- water/river (blue rectangle)
- golf hole (circle)
- golf ball (white circle)
- barrier rectangles between grass and floor (dark brown)
- at least 3 obstacle types with unique shape+color combinations

Suggested object assignment:
- obstacle type 1: square posts (light brown)
- obstacle type 2: triangles (orange)
- obstacle type 3: low-poly circle rocks (purple/red/brown)

Distinct color count target:
- Keep at least 9 solid colors across all categories.

Background color:
- set once with glClearColor(...) to a color not used by objects.

## 7) Input Handling (Selection)

Implement per-frame key polling in main.cpp with edge detection (press once behavior):
- 1 -> select ball
- 2 -> select obstacle type A object
- 3 -> select obstacle type B object
- 4 -> select hole
- 0 -> deselect all

Rules:
- Only one selected object needed for base marks.
- On selection, draw with pastel version of original color.

Pastel conversion formula:
- pastel = base * 0.5 + vec3(1.0,1.0,1.0) * 0.5

## 8) Input Handling (Transformations)

For selected object only, map keys:
- W/S/A/D -> translate
- + / - -> scale
- E -> clockwise rotate
- Q -> counter-clockwise rotate

Important grading requirement:
- Transformations must be cumulative. Never reset object to original state.

Recommended transform math order for each draw:
- M = T(position) * T(center) * R(angle) * S(scale) * T(-center)

This keeps scaling and rotation around object center as required.

Animation speed recommendation:
- Translation step around 0.02
- Scale factor around 1.05 and 0.95
- Rotation step around 4-7 degrees per key press

## 9) Wireframe Mode (GL_LINES Only)

Implement boolean wireframeEnabled in main.cpp.

Toggle behavior:
- Enter key toggles mode
- Add debounce/cooldown (example 150-250 ms) so one press does one toggle

Rendering behavior:
- Normal mode: draw filled geometry (GL_TRIANGLES)
- Wireframe mode: draw line list using GL_LINES for each object

Critical rule:
- Do not use glPolygonMode (forfeits wireframe marks)

How to generate line data:
- For rectangles: edges (v0-v1, v1-v2, v2-v3, v3-v0)
- For triangles: edges (v0-v1, v1-v2, v2-v0)
- For circles: connect ring point i to i+1 and close loop

## 10) Rendering Pipeline Steps in main.cpp

1. Initialize window and OpenGL
2. Compile shaders
3. Build scene objects
4. For each frame:
   - clear buffers
   - process keyboard state (selection + transforms + wireframe toggle)
   - for each object:
     - recompute transformed vertices from base vertices and transform state
     - upload vertex buffer
     - set inColor uniform (pastel if selected)
     - draw either filled or GL_LINES based on mode
   - swap buffers and poll events
5. On exit: delete OpenGL buffers/program and terminate GLFW

## 11) Makefile Checklist

If you add new .cpp files, append to:
- files = Matrix.cpp Vector.cpp Square.cpp Triangle.cpp ...

Likely add:
- Circle.cpp
- RenderObject.cpp (if used)

Run sequence:
- make clean
- make
- make run

## 12) Objective-to-File Mapping

- Layout requirements: main.cpp scene construction
- Shape requirements: Square/Triangle existing + new Circle.h/.cpp + scene composition in main.cpp
- Colour requirements: main.cpp color definitions + glClearColor
- Selection requirements: main.cpp keyboard handling and selected state
- Transformation requirements: main.cpp transform state and matrix application
- Wireframe requirements: main.cpp alternate GL_LINES rendering path

## 13) Suggested Build Order (Do This Exactly)

1. Create shaders (simple.vert/simple.frag) and confirm program renders one static rectangle.
2. Build all static layout objects with correct colors.
3. Add Circle implementation and place hole + ball + low-poly circle.
4. Add selection keys and pastel highlight.
5. Add translation keys W/A/S/D.
6. Add scale keys + and - around center.
7. Add rotation keys Q and E around center.
8. Add wireframe toggle with Enter and debounce.
9. Run full checklist in README.md and tick each item.
10. Final regression test: arbitrary transformation order on selected object without reset.

## 14) Demo-Ready Validation Script

Before demo, manually test this exact sequence:
- Launch app
- Press 1, then D, D, W, +, E, Q, -, A, S
- Press 0
- Press 2 and repeat a different order
- Press Enter (wireframe), repeat transforms, press Enter again
- Press ESC to exit cleanly

Pass criteria:
- no crash
- transforms are cumulative
- selection color changes to pastel
- wireframe uses lines and preserves object colors

## 15) Common Implementation Pitfalls

- Recreating objects every frame (loses cumulative transforms)
- Applying transforms around world origin instead of object center
- Holding Enter toggles rapidly (missing debounce)
- Using one color for too many categories (fails distinctive color requirements)
- Forgetting to include new .cpp files in makefile
- Missing shader source files at runtime path

## 16) What You Need Prepared for Submission

- Source files only (no prebuilt object assets/textures)
- Working makefile build on your target machine
- Program exits properly
- All required controls implemented and testable during demo

Use this guide together with README checklist: implement each section, then immediately tick matching rows in README.md.
