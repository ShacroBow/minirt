#!/usr/bin/env python3
"""Writes tools/concrete_wall.ppm and tools/concrete_wall.bump.ppm.

    python3 tools/gen_concrete_wall.py

A tileable wall of dark cast concrete panels (2 x 4 panels per tile) with
seams and form-tie holes: the color texture plus a matching normal map for
bump=. Deterministic, so the files can always be regenerated.
"""
import math
import os
import random

SIZE = 512
COLS, ROWS = 2, 4
HERE = os.path.dirname(os.path.abspath(__file__))


def value_noise(cells, seed):
    """Tileable smooth noise in 0..1 with cells x cells random values."""
    rnd = random.Random(seed)
    grid = [[rnd.random() for _ in range(cells)] for _ in range(cells)]

    def at(x, y):
        fx, fy = x * cells / SIZE, y * cells / SIZE
        ix, iy = int(fx), int(fy)
        tx, ty = fx - ix, fy - iy
        tx, ty = tx * tx * (3 - 2 * tx), ty * ty * (3 - 2 * ty)
        a = grid[iy % cells][ix % cells]
        b = grid[iy % cells][(ix + 1) % cells]
        c = grid[(iy + 1) % cells][ix % cells]
        d = grid[(iy + 1) % cells][(ix + 1) % cells]
        return (a + (b - a) * tx) + ((c + (d - c) * tx) - (a + (b - a) * tx)) * ty
    return at


def panel_shape(x, y):
    """(seam, hole): how deep in a seam / a form-tie hole, each 0..1."""
    pw, ph = SIZE / COLS, SIZE / ROWS
    px, py = x % pw, y % ph
    edge = min(px, pw - px, py, ph - py)
    seam = max(0.0, 1.0 - edge / 2.5)
    hole = 0.0
    for hx in (pw / 6, pw / 2, 5 * pw / 6):
        for hy in (ph / 4, 3 * ph / 4):
            d = math.hypot(px - hx, py - hy)
            hole = max(hole, max(0.0, 1.0 - (d / 6.0) ** 2))
    return seam, hole


def main():
    coarse = value_noise(8, 1)
    fine = value_noise(64, 2)
    grain = random.Random(3)
    height = [[0.0] * SIZE for _ in range(SIZE)]
    color = bytearray()
    for y in range(SIZE):
        for x in range(SIZE):
            seam, hole = panel_shape(x, y)
            mottle = 0.7 * coarse(x, y) + 0.3 * fine(x, y)
            g = grain.random()
            height[y][x] = -0.9 * seam - 0.8 * hole + 0.05 * fine(x, y) + 0.02 * g
            shade = (0.13 + 0.045 * mottle + 0.01 * g) * (1 - 0.45 * seam) * (1 - 0.6 * hole)
            color += bytes((int(255 * shade), int(255 * shade * 0.99), int(255 * shade * 1.03)))
    normal = bytearray()
    k = 1.6
    for y in range(SIZE):
        for x in range(SIZE):
            dx = (height[y][(x + 1) % SIZE] - height[y][x - 1]) * 0.5 * k
            dy = (height[y - 1][x] - height[(y + 1) % SIZE][x]) * 0.5 * k
            n = (-dx, -dy, 1.0)
            ln = math.sqrt(sum(c * c for c in n))
            normal += bytes(int(255 * (c / ln * 0.5 + 0.5)) for c in n)
    header = b'P6\n%d %d\n255\n' % (SIZE, SIZE)
    open(os.path.join(HERE, 'concrete_wall.ppm'), 'wb').write(header + bytes(color))
    open(os.path.join(HERE, 'concrete_wall.bump.ppm'), 'wb').write(header + bytes(normal))
    print('wrote concrete_wall.ppm and concrete_wall.bump.ppm in', HERE)


if __name__ == '__main__':
    main()
