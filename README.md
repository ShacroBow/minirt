# miniRT

Ray tracer in C. Renders spheres, planes, cylinders, cones and `.obj` models with textures, reflections, transparency, and bump maps.

## What it does

- Renders basic shapes (sphere, plane, cylinder, cone)
- Wavefront `.obj` models with `.mtl` materials and textures (BVH accelerated)
- Multiple lights with colors
- Phong shading
- Soft shadows, tinted by transparent objects
- Reflections
- Transparency with refraction
- Checkerboard patterns
- Texture mapping from PPM files
- Bump maps
- Anti-aliasing
- Multithreaded rendering
- Move camera and objects around in real-time

## Building

### First time
```bash
git clone --recursive <repo-url>
```
If you already cloned without `--recursive`, run once:
```bash
git submodule update --init --recursive
```

### Linux
```bash
sudo apt-get install build-essential libx11-dev libxext-dev libbsd-dev
make
```

### macOS
```bash
xcode-select --install
make
```
The Metal MiniLibX in `.mlx_macos` is built from source by `make`, and `./minirt` runs from any directory. No XQuartz needed.

The Makefile detects your OS automatically.

## Issues

**Build fails with "cannot find -lX11" or "cannot find -lbsd"**
```bash
sudo apt-get install libx11-dev libxext-dev libbsd-dev
```

**Build fails with ".mlx_linux/ is empty"**
```bash
git submodule update --init
```

**Window doesn't show up**
- Linux: make sure you're in a GUI.
- WSL: need X server like VcXsrv.
- macOS: should just work.

**Performance**
- Use `1` to increase pixel size for smoother movement.
- Use `P` only when you want a final screenshot-quality render.
- Fewer lights/objects = faster.
- Glass is the expensive part: rays split into reflection and refraction up to `MAX_DEPTH` (10) times. Lower it in `include/minirt.h` for faster previews.

## Running

```bash
./minirt tools/scene.rt
```

## Scene Files

`.rt` files define what gets rendered:

```
# Ambient lighting: ratio (0.0-1.0) and RGB color
A 0.2 255,255,255

# Camera: position, orientation, FOV (0-180)
C 0,0,20 0,0,-1 70

# Light: position, brightness ratio (0.0-1.0), RGB color
L 5,10,0 0.6 255,255,255

# Sphere: center, diameter, color, [optional parameters]
sp 0,0,0 5 255,0,0 r=0.7 ch=0,0,0

# Plane: point, normal vector, color, [optional parameters]
pl 0,-5,0 0,1,0 100,100,100 r=0.3

# Cylinder: center, axis, diameter, height, color, [optional parameters]
cy 10,0,0 0,1,0 2 4 0,0,255

# Cone: center, axis, diameter, height, color, [optional parameters]
co 5,5,0 0,1,0 2 2 220,0,255 r=0.4

# Model: .obj file, center, size (longest side), color, [optional parameters]
obj tools/models/torus_knot.obj 0,1,0 3 220,80,55 r=0.25 rot=10,0,0
```

### Optional Parameters
- `r=<value>` - Reflectivity (0.0 to 1.0)
- `ch=R,G,B` - Checkerboard pattern with alternate color
- `tx=<file.ppm>` - Texture file path
- `bump=<file.ppm>` - Bump map file path
- `u=<scale>` - Horizontal texture scale
- `v=<scale>` - Vertical texture scale
- `tr=<value>` - Transparency (0.0 to 1.0, 1 = clear glass)
- `ior=<value>` - Index of refraction of transparent objects (default 1.5, glass)
- `rot=x,y,z` - Rotation in degrees around x, then y, then z (`obj` only)

### .obj models
- Supports `v`, `vt`, `vn`, faces with any number of corners (`v`, `v/vt`, `v//vn`, `v/vt/vn`) and negative indices.
- Materials from `.mtl` files: `Kd` color, `d`/`Tr` transparency, `Ni`, `map_Kd` texture. The color on the `obj` line tints them, use `255,255,255` to keep them as they are.
- Models without normals are smoothed automatically, edges sharper than 60 degrees stay sharp.
- `tx=`, `ch=` and `bump=` use the model's texture coordinates (`vt`).
- Textures (`map_Kd`, `tx=`) have to be binary PPM files, see below.
- `tools/models/` has example models (made by `generate_models.py`), `tools/obj_test.rt` shows them.

## Controls

**Camera:**
- `WASD` - move position
- `PG UP/DOWN` - move up/down
- `Arrow keys` - rotate
- `X` - reset position
- `+/-` - change movement speed

**Rendering:**
- `1`/`2` - decrease/increase resolution (pixel size)
- `O` - toggle anti-aliasing (always on/off)
- `P` - single high-quality render with AA
- `Enter` - render again
- `ESC` - quit

**Objects:**
- `3` - select next object (shapes, lights, models)
- `0` - select camera
- `WASD`, `PG UP/DOWN` - move the selected object
- `Arrow keys` - resize the selected shape (a light: `UP`/`DOWN` brightness), a model: `UP`/`DOWN` scale, `LEFT`/`RIGHT` turn
- `B` - toggle bump map on selected object
- `4`/`5` - scale texture U
- `6`/`7` - scale texture V


## Example scenes

Check `tools/` for scene files:
- `scene.rt` - basic shapes
- `checker_test.rt` - checkerboard demo
- `texture_test.rt` - textures
- `glossy_test.rt` - reflective stuff
- `mirror_room.rt` - mirrors everywhere
- `transparency_test.rt` - glass, refraction and tinted shadows
- `obj_test.rt` - `.obj` models with materials, a texture and glass

## Screenshots

<div style="display:flex; flex-wrap:wrap; gap:12px;">
	<img src="tools/Screenshot%20from%202026-01-02%2018-37-48.png" alt="Render 2026-01-02 18:37:48" width="48%">
	<img src="tools/Screenshot%20from%202026-01-02%2018-31-21.png" alt="Render 2026-01-02 18:31:21" width="48%">
	<img src="tools/Screenshot%20from%202026-01-02%2018-27-52.png" alt="Render 2026-01-02 18:27:52" width="48%">
	<img src="tools/Screenshot%20from%202026-01-02%2001-07-40.png" alt="Render 2026-01-02 01:07:40" width="48%">
	<img src="tools/Screenshot%20from%202026-01-01%2023-13-24.png" alt="Render 2026-01-01 23:13:24" width="48%">
</div>

### your own textures.
Textures need to be PPM format. Use ImageMagick to convert:
```bash
convert image.png image.ppm
```

## Links

- [Ray Tracing in One Weekend](https://raytracing.github.io/)
- [Scratchapixel](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing)