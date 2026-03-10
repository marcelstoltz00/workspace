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

## 17) File-by-File Explanation (Beginner Friendly)

This section explains every project file as if you are new to graphics programming.

### Build and entry files

#### makefile
What it is:
- A recipe file that tells your computer how to compile and run your program.

What you do here:
- Add new .cpp files to the `files = ...` list when you create them.
- Run `make` to build and `make run` to execute.

Why it matters:
- If a file is missing here, your code can be correct but the program still fails to build.

#### main.cpp
What it is:
- The control center of your app.
- Think of it as the stage manager: it opens the window, loads shaders, listens for keys, and tells OpenGL what to draw every frame.

What you do here:
- Build your scene objects (floor, ball, hole, river, obstacles, barriers).
- Handle key input for selection and transformations.
- Toggle wireframe mode.
- Send vertex data and colors to the GPU for drawing.

Why it matters:
- Almost all marks come from behavior you implement in this file.

### Shader files (GPU programs)

#### shader.hpp
What it is:
- Header declaration for the shader loader helper function.

What you do here:
- Usually nothing major; it declares `LoadShaders(...)`.

Why it matters:
- Lets `main.cpp` call shader loading code from `shader.cpp`.

#### shader.cpp
What it is:
- Utility code that reads shader text files, compiles them, and links a GPU program.

What you do here:
- Usually keep as-is unless you need better logging.

Why it matters:
- Your GPU cannot draw with custom color logic until shaders are compiled and linked.

#### simple.vert
What it is:
- Vertex shader (runs once per vertex on GPU).

What it should do (simple version):
- Read 2D position input.
- Output clip-space position (`gl_Position`).

Why it matters:
- If this is wrong, geometry appears in the wrong place or not at all.

#### simple.frag
What it is:
- Fragment shader (runs for pixels of your shapes).

What it should do (simple version):
- Read uniform color (`inColor`).
- Output final pixel color.

Why it matters:
- All your distinct color requirements are visually controlled here.

### Math foundation files

#### Vector.h / Vector.cpp
What it is:
- A small math type representing a list of numbers (for example x,y coordinates).

What it does:
- Supports vector operations like add, subtract, scalar multiply, magnitude.

Why it matters:
- Object points and centers are stored as vectors.

#### Matrix.h / Matrix.cpp
What it is:
- A math type representing transformation tables.

What it does:
- Supports matrix operations used to move/rotate/scale points.

Why it matters:
- Transformations in graphics are easiest and safest with matrices.

### Shape abstraction and concrete shapes

#### Shape.h
What it is:
- Abstract base interface for all shapes.

What it does:
- Forces each shape to provide:
  - transform operators
  - point access
  - number of points

Why it matters:
- Gives one consistent way to handle different shape types.

#### Square.h / Square.cpp
What it is:
- Rectangle/square shape implementation.

What it does:
- Stores 4 corner points.
- Can be transformed with matrices.
- Exposes points for drawing.

Why it matters:
- Used for floor, start area, river blocks, barriers, and many obstacles.

#### Triangle.h / Triangle.cpp
What it is:
- Triangle shape implementation.

What it does:
- Stores 3 points and supports transforms.

Why it matters:
- Helps satisfy required distinct shape types and obstacle variety.

#### Circle.h / Circle.cpp
What it is:
- Circle shape implementation you added.

What it should do:
- Build circle vertices from center, radius, and vertex count.
- Support transforms and return points for drawing.

Why it matters:
- Needed for golf hole and golf ball.
- Needed to satisfy both high-poly and low-poly circle requirements.

### Optional scene-object wrapper files

#### RenderObject.h / RenderObject.cpp
What it is:
- Optional wrapper around a drawable scene item.
- Think of it as a container for "everything about one object".

What it should contain:
- Geometry data
- Color and pastel color
- Selection flag
- Transform state (position/scale/rotation/pivot)
- Optional wireframe index data

Why it matters:
- Keeps `main.cpp` clean and makes debugging easier.
- Makes cumulative transforms and selection logic easier to manage.

### Documentation and support files

#### README.md
What it is:
- Your grading checklist and progress tracker.

What you do here:
- Tick items as features are verified, not just coded.

Why it matters:
- Prevents missing hidden rubric details.

#### TECHNICAL_GUIDE.md
What it is:
- Your implementation playbook (this file).

What you do here:
- Follow step order while coding.
- Keep it as reference when stuck.

Why it matters:
- Reduces rework and confusion when integrating many features.

#### compile_flags.txt
What it is:
- Editor/tooling compile hints (for diagnostics/autocomplete tools).

What you do here:
- Usually nothing unless include paths change.

Why it matters:
- Better editor feedback while coding.

#### COS_344_2026_P2.pdf
What it is:
- Official assignment specification.

What you do here:
- Cross-check wording whenever uncertain about rubric.

Why it matters:
- The PDF is the source of truth during marking.

### Debug artifact

#### main.dSYM/
What it is:
- macOS debug symbols generated after build.

What you do here:
- Nothing for assignment logic.

Why it matters:
- Helps debugger show meaningful stack traces if the app crashes.

### The full picture in plain language

You can think of your program like this:
- `main.cpp` = director of the show.
- Shape files (`Square`, `Triangle`, `Circle`) = blueprints for object geometry.
- Math files (`Vector`, `Matrix`) = movement and rotation calculators.
- Shader files (`simple.vert`, `simple.frag`) = paint and placement logic running on GPU.
- `RenderObject` = optional box that keeps each object's state together.
- `makefile` = build/run instructions.

When it runs, your app repeats this loop:
1. Read keyboard input.
2. Update selected object's transform state.
3. Convert shape data to current frame vertices.
4. Draw with normal or wireframe mode.
5. Repeat.

That loop is exactly how you satisfy the selection, transformation, color, and wireframe requirements.

## 18) First 2-Hour Implementation Plan (No Guesswork)

This is a practical "do this now" schedule. Follow it in order.

### Before you start (5 minutes)

1. Open these files side by side:
  - `main.cpp`
  - `simple.vert`
  - `simple.frag`
  - `README.md`
2. Keep `README.md` visible so you can tick items as soon as they work.
3. Build once (`make`) to confirm your baseline still compiles.

Expected outcome:
- You know your starting point is clean.

### Block A: Get one shape on screen with color (20 minutes)

Goal:
- Prove the rendering pipeline works end-to-end.

Steps:
1. In `simple.vert`, implement minimal vertex shader logic:
  - input: vec2 position
  - output: `gl_Position = vec4(position, 0.0, 1.0);`
2. In `simple.frag`, implement minimal fragment shader logic:
  - uniform: `inColor`
  - output: `fragColor = vec4(inColor, 1.0);`
3. In `main.cpp`:
  - load shaders with `LoadShaders("simple.vert", "simple.frag")`
  - create VAO + VBO
  - upload one rectangle's vertices
  - set uniform color
  - call `glDrawArrays`

How to verify:
- You see one colored shape in the window.

If this fails:
- Fix this first before writing any scene/object system.

### Block B: Build static full layout (35 minutes)

Goal:
- Finish the non-interactive marks first (layout + many colors).

Steps:
1. In `main.cpp`, create a simple local struct for now, e.g. `SceneDrawCall`:
  - `std::vector<float> vertices`
  - `glm::vec3 color`
  - `GLenum mode`
2. Add helper functions in `main.cpp`:
  - `makeRect(cx, cy, w, h)`
  - `makeTriangle(...)`
  - temporary `makeCircleApprox(cx, cy, r, segments)`
3. Add all required objects to a vector of draw calls:
  - floor, grass, river, start zone, barriers, ball, hole, 3 obstacle types
4. Draw all objects each frame in a loop.
5. Set distinct background color with `glClearColor`.

How to verify:
- Scene visually contains all required components.
- At least 9 clearly distinct colors are visible.

Tick now in `README.md`:
- Most of Layout Requirements
- Most of Colour Requirements

### Block C: Add selection system first (20 minutes)

Goal:
- Implement key-to-object selection and visual highlight.

Steps:
1. Give each object an ID string and category.
2. Track `selectedObjectIndex` (or `-1` for none).
3. Map keys:
  - `1` -> golf ball
  - `2` -> obstacle A
  - `3` -> obstacle B
  - `4` -> hole
  - `0` -> deselect
4. Implement pastel color helper:
  - `pastel = base * 0.5f + vec3(1.0f) * 0.5f`
5. During draw, if object is selected, use pastel color.

How to verify:
- Pressing keys changes selected object and color highlight.

Tick now in `README.md`:
- Selection Requirements

### Block D: Add translation transforms (15 minutes)

Goal:
- Make selected object move with W/A/S/D.

Steps:
1. Add transform state per object:
  - `position` (x,y)
2. On key press, update selected object's position by small step.
3. Apply translation before sending vertices to GPU.

How to verify:
- Selected object moves and stays where moved (no reset).

### Block E: Add scale + rotation transforms (20 minutes)

Goal:
- Complete the remaining transform keys.

Steps:
1. Add `scale` and `rotation` per object.
2. Compute object center once (or from base vertices).
3. Apply transform in this order:
  - translate to center
  - scale/rotate
  - translate back
  - then apply object position
4. Map keys:
  - `+` and `-` for scale
  - `Q` and `E` for rotation

How to verify:
- Repeated key presses continue from current state.
- Works after moving object away from origin.

Tick now in `README.md`:
- Transformation Requirements (except maybe wireframe)

### Block F: Add wireframe toggle with debounce (15 minutes)

Goal:
- Implement Enter toggle and GL_LINES path.

Steps:
1. Add `wireframeEnabled` boolean.
2. Add Enter key debounce timer (150-250 ms).
3. Build line-vertex versions for each shape.
4. Draw lines with `GL_LINES` when wireframe is enabled.
5. Keep same object colors in wireframe mode.

How to verify:
- Enter toggles once per press.
- Wireframe still responds to selection and transforms.

Tick now in `README.md`:
- Wireframe Requirements

### Last 10 minutes: fast regression test

Run this exact manual test:
1. Select ball (`1`)
2. Press `D D W + E Q - A S`
3. Deselect (`0`)
4. Select obstacle (`2`) and do different order
5. Toggle wireframe with Enter and repeat movement
6. Exit with ESC

Must all be true:
- No crashes
- No transform reset
- Correct object selected by each number key
- Pastel highlight visible
- Wireframe uses lines and preserves colors

## 19) Where Beginners Usually Get Stuck (And What To Do)

Stuck: "Nothing appears on screen"
- Check shader compile log output first.
- Confirm VAO/VBO bind order and `glVertexAttribPointer` setup.
- Confirm positions are inside clip range (roughly -1 to 1).

Stuck: "Object moves but snaps back"
- You are recomputing from original without storing updated transform state.
- Keep per-object state and update it cumulatively.

Stuck: "Rotation is weird"
- You are rotating around world origin.
- Rotate around object center (pivot), not (0,0).

Stuck: "Enter toggles too fast"
- Add time-based debounce using `glfwGetTime()`.

Stuck: "Circle looks wrong"
- Ensure angle step uses radians and loops correctly.
- For smooth circle use many segments (50+).
