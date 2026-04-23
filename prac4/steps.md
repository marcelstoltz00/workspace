

Step-by-step plan:

1. Scene structure for Prac 4 objects
1. Keep floor and ball.
2. Add initial glass object as a separate node above/around the ball in SceneManager.cpp.
3. Store pointers for floor, ball, glass, and light marker in SceneManager.h so we can target behavior cleanly later.
4. Checkpoint: render shows floor + ball + glass + light marker with current controls.

2. Point light projection on floor only
1. Extend shader uniforms in simple.vert and simple.frag: light position, light color, object flags.
2. Pass world position from vertex shader to fragment shader.
3. Apply point-light equation only when drawing floor; keep ball unaffected for now (as required).
4. Send uniforms per object draw in MeshNode.cpp and from scene state in SceneManager.cpp.
5. Checkpoint: moving arrow keys changes floor lighting pattern, ball color remains visually unaffected by light.

3. Glass translucency and floor color influence
1. Enable blending in scene draw setup in main.cpp or SceneManager.cpp.
2. Render order: opaque first (floor, ball), translucent glass after.
3. Add glass alpha and tint uniforms in shaders.
4. Implement floor color combination with light + glass tint contribution in fragment shader logic.
5. Checkpoint: changing glass color visibly influences floor result color where expected.

4. Color system and required palette controls
1. Add color presets for light, glass, floor, and at least five additional distinct colors in SceneManager.cpp.
2. Add key cycling for these colors (we will pick non-conflicting keys).
3. Ensure wireframe uses same object colors.
4. Checkpoint: all required color combinations are demonstrable quickly during demo.

5. Texture pipeline (three texture maps)
1. Add UV-capable geometry path (new shape data with UVs) in ShapeFactory.h and ShapeFactory.cpp.
2. Add texture coordinates to mesh buffers in MeshNode.cpp.
3. Add texture sampling support in simple.vert and simple.frag.
4. Create and load three custom texture maps from assets folder.
5. Checkpoint: three textures are visible and switchable/assigned per requirement.

6. Glass geometry refinement and wireframe readability
1. Refine glass into clear multi-face geometry so face boundaries are obvious.
2. Keep GL_LINES wireframe behavior consistent with transformations.
3. Optional bonus-ready hook: explicit glass face edges as separate line meshes.
4. Checkpoint: glass is visually clear in both filled and wireframe modes.

7. Reset and behavior hardening
1. Ensure Space fully restores initial transforms and colors in SceneManager.cpp.
2. Confirm light starts at ball center every reset.
3. Verify key debounce behavior for Enter toggle remains stable.
4. Checkpoint: repeated test loop passes with no drift or stuck state.

8. Final rubric pass
1. Walk through every required key and visual requirement against spec.md.
2. Remove unused code, clean naming, and keep only Prac 4 relevant logic.
3. Do final build and run check with makefile.
