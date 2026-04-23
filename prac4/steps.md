Prac 4 spec-driven implementation checklist

Goal:
Render a semi-translucent 3D glass golf ball resting on a flat plane, with a point light inside the ball shining onto the floor below. Every task below maps directly to `spec.md` so we can verify nothing is missed.

1. Core scene setup
1. Keep the scene to only the required shapes plus the light marker:
   sphere = glass golf ball
   plane = floor
   small marker = visible light position helper
2. Ensure the ball rests on the floor and the light starts in the centre of the ball.
3. Keep the scene fully 3D and driven by our custom math/transforms.
4. Checkpoint:
   render shows floor + translucent ball + light marker only

2. Shape requirements
1. Sphere vertex-count control:
   add runtime controls to increase and decrease sphere subdivision
2. Floor vertex-count control:
   build the floor from connected smaller plane tiles / a tessellated grid
   add runtime controls to increase and decrease floor subdivision
3. Make both controls visibly affect mesh quality during runtime.
4. Checkpoint:
   we can demonstrate coarse and fine versions of both sphere and floor without restarting

3. Texture system foundation
1. Extend shape data and mesh upload path to support texture coordinates for the sphere.
2. Add CPU-side texture loading support for three custom texture maps:
   colour map
   displacement map
   alpha map
3. Keep the texture workflow CPU-only:
   load the PNG image data on the CPU
   sample colour / displacement / alpha values on the CPU
   update sphere vertex positions, colours, and alpha-related attributes on the CPU
   do not rely on GPU texture sampling as the main implementation path for these three maps
4. Add toggles for enabling/disabling each texture feature.
   shaders may still consume CPU-prepared attributes/uniforms, but not sample the image maps directly
4. Ensure the sphere can still render correctly when all texture toggles are off.
5. Checkpoint:
   texture pipeline is wired and ready for actual golf-ball maps

4. Required golf-ball texture maps
1. Colour texture map:
   create our own map for darker dimple regions
   apply its effect from CPU-side image sampling
2. Displacement texture map:
   create our own map that actually changes sphere surface geometry to form dimples
   no bump-mapping shortcut
   displacement values must be sampled and applied on the CPU
3. Alpha texture map:
   create our own map so dimples become transparent while the rest stays opaque
   alpha values must be sampled and applied on the CPU
4. Include all created maps in the project submission area.
5. Prepare a short demo explanation of how each map was created.
6. Checkpoint:
   all three maps exist locally and are used in rendering

5. Texture toggle behavior from spec
1. `B` toggles colour texture:
   off = uniform translucent ball colour
   on = dimples appear darker
   implementation note: CPU re-evaluates the colour contribution when toggled
2. `N` toggles displacement texture:
   off = smooth ball
   on = actual surface geometry shows dimples
   implementation note: CPU rebuilds sphere vertex positions when toggled
3. `M` toggles alpha texture:
   off = whole ball uses shared alpha value
   on = only dimple regions use shared alpha, rest of ball fully opaque
   implementation note: CPU updates per-vertex alpha-related data when toggled
4. `+` and `-` control the shared alpha value used by the ball
   this same alpha value must be respected by the alpha texture mode too
5. Checkpoint:
   each toggle independently changes exactly the behavior described in `spec.md`

6. Colour requirements
1. Define required floor palette:
   red, green, blue, white, black, plus 5 other distinct colours
2. Define required ball palette:
   red, green, blue, white, black, plus 5 other distinct colours
3. Define required light palette:
   red, green, blue, white, plus 5 other distinct colours
4. Assign two keys for floor colour cycling.
5. Assign two different keys for ball colour cycling.
6. Assign two different keys for light colour cycling.
7. Ensure ball alpha is RGBA-based, not hacked separately from colour state.
8. Checkpoint:
   all required colours can be cycled live during demo

7. Lighting requirements
1. Keep the light as a point light.
2. Use the prescribed point-light style calculation for floor lighting.
3. The light projection must affect the floor only.
4. The golf ball itself must not receive visible light shading for the base requirement.
5. The floor colour result must combine:
   floor colour
   light colour
   ball / glass colour
   light intensity at the specific floor position
6. Checkpoint:
   changing floor colour, ball colour, or light colour all changes the floor result in an understandable way

8. Transformation requirements
1. Scene rotation controls:
   `W` / `S` rotate about x-axis
   `A` / `D` rotate about y-axis
   `E` / `Q` rotate about z-axis
2. Light translation controls:
   `Up` / `Down` move light along local y
   `Left` / `Right` move light along local x
   `>` / `<` move light along local z
3. Keep motion rates small enough for repeated demo presses.
4. Ensure moving the light does not move the rest of the scene.
5. Checkpoint:
   rotations and local light motion match the spec exactly

9. Reset requirements
1. `Space` resets:
   scene position
   scene rotation
   light position
   sphere subdivision
   floor subdivision
   ball alpha
   all current colours
   texture toggles
2. Reset must restore the initial render state exactly.
3. Checkpoint:
   repeated modification + reset cycles always return to the original scene

10. Wireframe requirements
1. `Enter` toggles between normal render and wireframe.
2. Wireframe must use `GL_LINES`, not `glPolygonMode`.
3. Wireframe must preserve object colour identity.
4. Wireframe must respect all transformations and runtime subdivision changes.
5. Add debounce behavior so toggle input is stable.
6. Checkpoint:
   wireframe mode is demo-safe and clearly shows the active geometry density

11. Code cleanup against implementation restrictions
1. Confirm OpenGL 3.3 usage.
2. Confirm custom math is used instead of forbidden glm transformation helpers.
3. Confirm shapes are generated from first principles.
4. Confirm transformations are done with our own matrix arithmetic.
5. Checkpoint:
   codebase stays inside practical rules before final polish

12. Demo-readiness pass
1. Write down the final chosen key map in code comments or a short help note.
2. Verify there are no key conflicts:
   `B`, `N`, `M`, `+`, `-`, `Enter`, `Space`, rotation keys, light movement keys, and all colour/subdivision keys must coexist cleanly
3. Demo script:
   show sphere subdivision
   show floor subdivision
   show floor colour cycling
   show ball colour cycling
   show light colour cycling
   show alpha changes
   show `B`, `N`, `M`
   show light movement
   show wireframe
   show reset
4. Final checkpoint:
   every requirement in sections 3.1 to 3.7 of `spec.md` is demonstrable on demand

13. Stretch / bonus only after all required marks are safe
1. Glass face sorting / BSP
2. Extra glass edges if still useful visually
3. Light affecting ball appearance
4. Stained-glass style regions
5. Additional bonus texture / material ideas
