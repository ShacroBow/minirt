#!/usr/bin/env python3
"""Writes the example .obj / .mtl models in this directory.

    python3 tools/models/generate_models.py

Each model exercises a different part of the .obj loader:
  cube_colors  one material per face (usemtl), quads, negative indices
  crate        per-face texture coordinates and a map_Kd texture (.ppm)
  torus_knot   no normals at all: smooth normals are computed on load
  glass_drop   smoothing groups, a hard crease and a transparent material
"""
import math
import os

HERE = os.path.dirname(os.path.abspath(__file__))


def write(name, lines):
    with open(os.path.join(HERE, name), "w") as f:
        f.write("\n".join(lines) + "\n")


def cube_colors():
    write("cube_colors.mtl", [
        "# one color per side of the cube",
        "newmtl right", "Kd 0.85 0.10 0.10",
        "newmtl left", "Kd 0.10 0.65 0.20",
        "newmtl top", "Kd 0.15 0.30 0.90",
        "newmtl bottom", "Kd 0.95 0.80 0.10",
        "newmtl front", "Kd 0.95 0.95 0.95",
        "newmtl back", "Kd 0.70 0.15 0.70",
    ])
    write("cube_colors.obj", [
        "# Cube with one material per side: +x red, -x green, +y blue,",
        "# -y yellow, +z white, -z purple.",
        "mtllib cube_colors.mtl",
        "o cube",
        "v -1 -1 -1", "v 1 -1 -1", "v 1 1 -1", "v -1 1 -1",
        "v -1 -1 1", "v 1 -1 1", "v 1 1 1", "v -1 1 1",
        "usemtl right", "f 2 3 7 6",
        "usemtl left", "f 1 5 8 4",
        "usemtl top", "f -5 -1 -2 -6",
        "usemtl bottom", "f 1 2 6 5",
        "usemtl front", "f 5 6 7 8",
        "usemtl back", "f 1 4 3 2",
    ])


def crate():
    write("crate.mtl", [
        "newmtl bricks",
        "Kd 1 1 1",
        "# texture paths are relative to this file",
        "map_Kd ../texture_brick.ppm",
    ])
    faces = [  # corner positions of each side, counter-clockwise from outside
        [(1, -1, -1), (1, 1, -1), (1, 1, 1), (1, -1, 1)],
        [(-1, -1, -1), (-1, -1, 1), (-1, 1, 1), (-1, 1, -1)],
        [(-1, 1, -1), (-1, 1, 1), (1, 1, 1), (1, 1, -1)],
        [(-1, -1, -1), (1, -1, -1), (1, -1, 1), (-1, -1, 1)],
        [(-1, -1, 1), (1, -1, 1), (1, 1, 1), (-1, 1, 1)],
        [(-1, -1, -1), (-1, 1, -1), (1, 1, -1), (1, -1, -1)],
    ]
    lines = ["# Textured crate: 4 vertices and 4 texture coordinates per side",
             "mtllib crate.mtl", "usemtl bricks",
             "vt 0 0", "vt 1 0", "vt 1 1", "vt 0 1"]
    for i, face in enumerate(faces):
        lines += ["v %d %d %d" % p for p in face]
        b = i * 4
        lines.append("f %d/1 %d/2 %d/3 %d/4" % (b + 1, b + 2, b + 3, b + 4))
    write("crate.obj", lines)


def torus_knot(p=2, q=3, steps=180, sides=16, tube=0.28):
    def curve(t):
        r = 1.0 + 0.45 * math.cos(q * t)
        return (r * math.cos(p * t), 0.45 * math.sin(q * t), r * math.sin(p * t))

    def sub(a, b):
        return tuple(x - y for x, y in zip(a, b))

    def cross(a, b):
        return (a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2],
                a[0] * b[1] - a[1] * b[0])

    def norm(a):
        n = math.sqrt(sum(x * x for x in a))
        return tuple(x / n for x in a)

    lines = ["# (%d,%d) torus knot without normals: the loader smooths it" % (p, q)]
    h = 1e-4
    for i in range(steps):
        t = 2 * math.pi * i / steps
        c = curve(t)
        d1 = sub(curve(t + h), curve(t - h))
        d2 = sub(sub(curve(t + h), c), sub(c, curve(t - h)))
        tan = norm(d1)
        bin_ = norm(cross(d1, d2))
        nor = cross(bin_, tan)
        for j in range(sides):
            a = 2 * math.pi * j / sides
            ca, sa = math.cos(a) * tube, math.sin(a) * tube
            lines.append("v %.5f %.5f %.5f" % tuple(
                c[k] + ca * nor[k] + sa * bin_[k] for k in range(3)))
    for i in range(steps):
        for j in range(sides):
            a = i * sides + j + 1
            b = i * sides + (j + 1) % sides + 1
            c = ((i + 1) % steps) * sides + j + 1
            d = ((i + 1) % steps) * sides + (j + 1) % sides + 1
            lines.append("f %d %d %d %d" % (a, b, d, c))
    write("torus_knot.obj", lines)


def glass_drop(segments=48):
    write("glass_drop.mtl", [
        "newmtl glass",
        "Kd 0.85 0.95 1.0",
        "# d = opacity, Ni = index of refraction",
        "d 0.15",
        "Ni 1.5",
    ])
    # profile from the bottom center to the tip: (radius, height)
    prof = [(0.0, 0.0), (0.55, 0.0)]
    for k in range(1, 15):
        a = math.pi * k / 15
        prof.append((0.55 + 0.25 * math.sin(a), 0.6 - 0.6 * math.cos(a)))
    for k in range(1, 9):
        s = k / 9
        prof.append((0.55 * (1 - s) ** 1.6, 1.2 + 1.1 * s))
    prof.append((0.0, 2.35))
    lines = ["# Drop of glass (surface of revolution), material from .mtl",
             "mtllib glass_drop.mtl", "usemtl glass"]
    for r, y in prof:
        for s in range(segments):
            a = 2 * math.pi * s / segments
            lines.append("v %.5f %.5f %.5f" % (r * math.cos(a), y, r * math.sin(a)))
    lines.append("s off")
    ring = lambda k, s: k * segments + s % segments + 1
    lines.append("f " + " ".join(str(ring(1, s)) for s in range(segments)))
    lines.append("s 1")
    for k in range(1, len(prof) - 1):
        for s in range(segments):
            lines.append("f %d %d %d %d" % (ring(k, s), ring(k + 1, s),
                                            ring(k + 1, s + 1), ring(k, s + 1)))
    write("glass_drop.obj", lines)


if __name__ == "__main__":
    cube_colors()
    crate()
    torus_knot()
    glass_drop()
    print("models written to", HERE)
