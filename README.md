# miniRT

Ray tracer in C. Renders spheres, planes, cylinders, and cones with textures, reflections, and bump maps.

## What it does

- Renders basic shapes (sphere, plane, cylinder, cone)
- Multiple lights with colors
- Phong shading
- Reflections
- Checkerboard patterns
- Texture mapping from PPM files
- Bump maps
- Anti-aliasing
- Move camera around in real-time

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
sudo apt-get install build-essential libx11-dev libxext-dev
make
```

If you did not clone recursively, initialize the submodule once:
```bash
git submodule update --init --recursive
```
To update it later:
```bash
git submodule update --remote --merge .mlx_linux
```

### macOS
```bash
xcode-select --install
make
```

The Makefile detects your OS automatically.

## Issues

**Build fails with "cannot find -lX11"**
```bash
sudo apt-get install libx11-dev libxext-dev
```

**Window doesn't show up**
- Linux: make sure you're in a GUI.
- WSL: need X server like VcXsrv.
- macOS: should just work.

**Performance**
- Use `1` to increase pixel size for smoother movement.
- Use `P` only when you want a final screenshot-quality render.
- Fewer lights/objects = faster.

## Running

```bash
./minirt tools/*.rt
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
```

### Optional Parameters
- `r=<value>` - Reflectivity (0.0 to 1.0)
- `ch=R,G,B` - Checkerboard pattern with alternate color
- `tx=<file.ppm>` - Texture file path
- `bump=<file.ppm>` - Bump map file path
- `u=<scale>` - Horizontal texture scale
- `v=<scale>` - Vertical texture scale

## Controls

**Camera:**
- `WASD` - move position
- `PG UP/DOWN` - move up/down
- `Arrow keys` - rotate
- `SPACE` - reset position
- `+/-` - change movement speed

**Rendering:**
- `1`/`2` - decrease/increase resolution (pixel size)
- `O` - toggle anti-aliasing (always on/off)
- `P` - single high-quality render with AA
- `ESC` - quit

**Objects:**
- `3` - select next object
- `0` - select camera
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

to see transparancy and soft shadows + tinted shadows git switch into the experimental branch.

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