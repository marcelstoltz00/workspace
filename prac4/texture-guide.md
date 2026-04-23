Golf Ball Texture Map Guide

Goal:
Create the three texture maps required by `spec.md` yourself, so you can explain them during the demo and include them in your submission.

Recommended tools:
- Krita
- GIMP
- Photoshop
- Procreate
- Any drawing tool that lets you paint grayscale and export PNG

Recommended output:
- square PNG files
- start with `1024x1024`
- keep all three maps aligned to the same UV layout

Project file names:
- `scene/colour.png`
- `scene/displace.png`
- `scene/alpha.png`

1. First decide on your UV layout
- Your sphere will need a UV mapping where `u` wraps around longitude and `v` goes from top to bottom.
- Before painting final textures, export or sketch a simple UV guide image:
  vertical rectangle layout
  left/right edges wrap together
  top and bottom are poles
- Keep this guide as a separate layer while painting.

2. Plan the dimple pattern
- Real golf balls use repeated circular dimples spread across the surface.
- You do not need physically perfect placement for the practical, but it should look intentional and repeated.
- A practical student-friendly approach is:
  paint evenly spaced circular dimples in rows
  offset every second row slightly
  keep dimple size mostly consistent
- Avoid random blobs. Repetition will read more convincingly as a manufactured ball.

3. Colour texture map
Purpose:
When enabled, the dimples should look darker than the rest of the ball.

How to create it:
- Start with a white or slightly off-white base.
- Add a repeated pattern of soft darker circles for the dimples.
- Use gentle shading, not harsh black holes.
- Keep the rest of the surface close to the base golf-ball colour so your runtime colour multiplier can still affect it.

Good values:
- base regions: light gray to white
- dimple regions: slightly darker gray

Important:
- Do not bake strong lighting from one direction into the texture.
- Keep the colour texture mostly "material colour", not fake scene lighting.

4. Displacement texture map
Purpose:
This map should actually deform the sphere surface so the ball gains real dimples.

How to create it:
- Use a grayscale image.
- Bright values should mean "keep surface high" and dark values should mean "push inward", or the reverse if your shader/code uses the opposite convention.
- Make the non-dimple regions near white or mid-gray.
- Make dimple centres darker.
- Use soft radial gradients for each dimple instead of hard-edged circles.

Practical painting recipe:
- Fill the map with a flat gray or white base.
- For each dimple, paint a soft circular depression.
- Blur slightly if needed so transitions are smooth.

Important:
- This map is for geometry displacement, not bump mapping.
- Your code should move vertices along the sphere normal or radial direction based on the sampled value.
- Keep the displacement subtle. If it is too strong, the golf ball will look melted.

5. Alpha texture map
Purpose:
When enabled, only the dimples should become transparent while the rest of the ball remains opaque.

How to create it:
- Use a grayscale image.
- White should represent opaque regions.
- Darker values should represent transparent dimple regions.
- Match the dimple layout exactly to the colour and displacement maps.

Practical painting recipe:
- Start with a white image.
- Paint the dimple circles darker.
- Use soft transitions if you want partially transparent dimple edges.
- If your implementation expects crisp transparency, keep the dimple region darker with less blur.

Important:
- This map should use the same dimple positions as the other two maps.
- The shared runtime alpha value from `+` and `-` should scale the transparency, not replace the map layout.

6. Best workflow: create one master dimple layout first
- Make one high-quality dimple pattern layer.
- Reuse that same pattern to derive all three textures.

Suggested workflow:
1. Paint a clean grayscale dimple pattern master.
2. Duplicate it for displacement.
3. Duplicate it for alpha.
4. Create the colour map by combining the pattern with a white golf-ball base.

Why this helps:
- all three maps stay aligned
- easier to explain in the demo
- fewer mistakes at the poles and seams

7. Handling seams and poles
- The left and right edges of the texture must tile cleanly because the sphere wraps there.
- Test by placing guide circles across the left and right border to ensure they connect.
- Near the poles, keep dimple spacing simpler if needed.
- It is okay if the very top and bottom are slightly less detailed than the middle, as long as the map still looks deliberate.

8. Demo explanation you can give
- Colour map:
  "I painted a white golf-ball texture and darkened the dimple regions so the dimples appear shaded darker when the colour map is enabled."
- Displacement map:
  "I created a grayscale height map where the dimple regions are darker, and I use it to push the sphere surface inward so the dimples are real geometry."
- Alpha map:
  "I created a grayscale transparency map aligned to the same dimple pattern so only the dimple regions become transparent when alpha texturing is enabled."

9. Quick quality checklist
- All three files are your own work
- All three files line up to the same UV layout
- Dimples repeat consistently
- Colour map darkens dimples
- Displacement map creates real depressions
- Alpha map affects only dimples
- Left/right texture seam wraps cleanly
- Files are included in the project for submission

10. Recommended implementation order in code
1. Add UVs to the sphere
2. Load and sample the colour map
3. Add displacement using the grayscale height map
4. Add alpha map behavior
5. Bind the required keys:
   `B` colour texture toggle
   `N` displacement toggle
   `M` alpha texture toggle

11. Practical tip
- Do one small test texture first, even if it looks rough.
- Once the mapping works on the sphere, improve the artwork.
- That is much safer than spending time painting detailed textures before confirming the UV pipeline is correct.
