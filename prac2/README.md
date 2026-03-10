# COS344 Practical 2 - 2D Rendering Checklist

**Due Date:** 23-03-2026 at 10:00  
**Total Marks:** 110 (+ 10 bonus)
 
Technical implementation walkthrough: see TECHNICAL_GUIDE.md

## Requirements Checklist

### Layout Requirements (30 marks)

| Requirement | Points | Status | Notes |
|-------------|--------|--------|-------|
| Concrete floor shape (grey) with border from window edges | 4 | ☐ | Must not occupy entire screen |
| Starting point (maroon rectangle) | 4 | ☐ | Distinctive maroon colour |
| River/stream (blue rectangle) | 4 | ☐ | Running through course |
| Obstacle Type 1 | 4 | ☐ | Unique colour/shape combo |
| Obstacle Type 2 | 4 | ☐ | Different from Type 1 |
| Obstacle Type 3 | 4 | ☐ | Different from Types 1 & 2 |
| Golf hole (pink circle) | 3 | ☐ | Circle shape |
| Golf ball (white circle) | 3 | ☐ | Circle shape, white |
| Barrier rectangles (dark brown) between grass and floor | 4 | ☐ | Complete border |

### Shape Requirements (12 marks)

| Requirement | Points | Status | Notes |
|-------------|--------|--------|-------|
| At least 2 distinct rectangles/squares | 4 | ☐ | Different instances |
| At least 2 distinct triangles | 4 | ☐ | Different instances |
| Circle with 50+ vertices | 2 | ☐ | High polygon circle |
| Circle with 6-10 vertices | 2 | ☐ | Low polygon circle |

### Colour Requirements (18 marks)

| Requirement | Points | Status | Notes |
|-------------|--------|--------|-------|
| 3 distinct colours for obstacle types | 4 | ☐ | Different for each type |
| Distinctive starting point colour | 2 | ☐ | Maroon suggested |
| Distinctive golf hole colour | 2 | ☐ | Pink suggested |
| Distinctive blue for water | 2 | ☐ | Must be blue |
| Distinctive white for golf ball | 2 | ☐ | Must be white |
| Distinctive grass colour | 2 | ☐ | Green suggested |
| Distinctive grey concrete floor | 2 | ☐ | Must be grey |
| Distinctive background colour | 2 | ☐ | Use OpenGL function |
| **Total unique colours: 9+** | - | ☐ | All solid colours |

### Selection Requirements (10 marks)

| Requirement | Points | Status | Notes |
|-------------|--------|--------|-------|
| Key `1`: Select golf ball | 2 | ☐ | Shows pastel version |
| Key `2`: Select obstacle type 1 | 2 | ☐ | Shows pastel version |
| Key `3`: Select obstacle type 2 | 2 | ☐ | Shows pastel version |
| Key `4`: Select golf hole | 2 | ☐ | Shows pastel version |
| Key `0`: Deselect current object | 2 | ☐ | Remove highlight |

### Transformation Requirements (32 marks)

| Requirement | Points | Status | Notes |
|-------------|--------|--------|-------|
| Key `W`: Move selected object up | 4 | ☐ | Cumulative, not reset |
| Key `S`: Move selected object down | 4 | ☐ | Cumulative, not reset |
| Key `A`: Move selected object left | 4 | ☐ | Cumulative, not reset |
| Key `D`: Move selected object right | 4 | ☐ | Cumulative, not reset |
| Key `+`: Scale up from center | 4 | ☐ | From object center |
| Key `-`: Scale down from center | 4 | ☐ | From object center |
| Key `E`: Rotate clockwise | 4 | ☐ | From object center |
| Key `Q`: Rotate counter-clockwise | 4 | ☐ | From object center |
| **Transformations are cumulative** | - | ☐ | No reset to original position |
| **Works in arbitrary order** | - | ☐ | Any combination of transforms |

### Wireframe Requirements (8 marks)

| Requirement | Points | Status | Notes |
|-------------|--------|--------|-------|
| Key `Enter`: Toggle wireframe mode | 2 | ☐ | May need time delay |
| Wireframe maintains colour scheme | 2 | ☐ | Same colours as normal |
| Wireframe uses GL_LINES | 2 | ☐ | NOT glPolygonMode |
| Wireframe conforms to all transformations | 2 | ☐ | Move, scale, rotate work |

### Bonus Features (up to 10 marks)

| Feature | Points | Status | Notes |
|---------|--------|--------|-------|
| Multiple object selection | 2 | ☐ | Transform multiple at once |
| Click-to-select objects | 2 | ☐ | Mouse interaction |
| Bounds checking (off-screen) | 2 | ☐ | Objects can't leave window |
| Bounds checking (obstacles) | 2 | ☐ | Ball can't move through |
| Ball bouncing off obstacles | 2 | ☐ | Physics simulation |
| Dynamic add/remove objects | 2 | ☐ | Runtime modification |
| Extra details/patterns | 2 | ☐ | No texture mapping allowed |
| Save layout to text file | 2 | ☐ | Export functionality |
| Load layout from text file | 2 | ☐ | Import functionality |
| Save screenshot (OpenGL) | 2 | ☐ | Must use OpenGL |

## Technical Requirements

| Requirement | Status | Notes |
|-------------|--------|-------|
| OpenGL version 3.3 | ☐ | Must use this version |
| C++11 | ☐ | No later standards |
| No additional libraries | ☐ | Only allowed libraries |
| Code compiles successfully | ☐ | 0 marks if doesn't compile |
| Program exits successfully | ☐ | 0 marks if doesn't exit |
| Functions in .cpp files | ☐ | No inline in headers |
| No pre-built objects/textures | ☐ | Create everything yourself |
| No AI-generated code | ☐ | Plagiarism rules apply |

## Pre-Demo Checklist

- [ ] Code compiles without errors
- [ ] Code compiles without warnings
- [ ] All required files submitted
- [ ] Makefile works correctly
- [ ] Program runs and displays window
- [ ] All keyboard controls tested
- [ ] Wireframe toggle tested
- [ ] Transformations are cumulative (tested)
- [ ] Selection highlighting works
- [ ] Program exits cleanly (no crashes)

## Testing Procedure

1. **Initial Render**
   - [ ] Window opens successfully
   - [ ] Layout is visible
   - [ ] All required shapes present
   - [ ] All required colours present
   - [ ] Border visible around floor

2. **Selection Tests**
   - [ ] Press `1` - golf ball highlighted
   - [ ] Press `2` - obstacle 1 highlighted
   - [ ] Press `3` - obstacle 2 highlighted
   - [ ] Press `4` - golf hole highlighted
   - [ ] Press `0` - deselection works

3. **Transformation Tests** (for each selected object)
   - [ ] `W` key - moves up (multiple presses)
   - [ ] `S` key - moves down (multiple presses)
   - [ ] `A` key - moves left (multiple presses)
   - [ ] `D` key - moves right (multiple presses)
   - [ ] `+` key - scales up (multiple presses)
   - [ ] `-` key - scales down (multiple presses)
   - [ ] `E` key - rotates clockwise (multiple presses)
   - [ ] `Q` key - rotates counter-clockwise (multiple presses)

4. **Cumulative Transform Test**
   - [ ] Select object
   - [ ] Move right (D)
   - [ ] Move up (W)
   - [ ] Rotate (E)
   - [ ] Scale up (+)
   - [ ] Verify object maintains all transformations

5. **Wireframe Test**
   - [ ] Press `Enter` - wireframe appears
   - [ ] Colours maintained in wireframe
   - [ ] Press `Enter` - back to normal
   - [ ] Select object and transform in wireframe mode
   - [ ] Transformations work in wireframe

## Common Mistakes to Avoid

- ❌ Don't reset object position before applying new transformation
- ❌ Don't use glPolygonMode for wireframe
- ❌ Don't make floor occupy entire window (needs border)
- ❌ Don't forget pastel colour for selection highlight
- ❌ Don't use texture mapping for bonus patterns
- ❌ Don't make transformations relative to world origin (use object center)

## Mark Distribution Summary

| Category | Marks |
|----------|-------|
| Layout Requirements | 30 |
| Shape Requirements | 12 |
| Colour Requirements | 18 |
| Selection Requirements | 10 |
| Transformation Requirements | 32 |
| Wireframe Requirements | 8 |
| **Total** | **110** |
| Bonus (optional) | +10 |

## Notes

- Keep animation rates low for testing
- Implement time delay for Enter key toggle
- Test on both your computer and lab computer before demo
- Have backup of working version before attempting bonus marks
- -1 mark per transformation if object resets to center before applying new transform
