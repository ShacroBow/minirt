#!/usr/bin/env python3
"""Converts a glTF 2.0 model (.gltf or .glb) into .obj + .mtl for miniRT.

    python3 tools/models/gltf_to_obj.py in.gltf out_dir/name [options]
      --skip-texture N.png  do not use this image (e.g. a logo)
      --skip-part NAME      leave out a part (a node name), e.g. hidden ones
      --flat-dark           no stored normals for dark, dull materials:
                            miniRT computes smooth ones, the file shrinks
      --digits N            decimals of vertex positions (default 4)
      --credit TEXT         comment line for both files (author, license)

Geometry: every mesh of the default scene, with its node transforms applied,
normals and texture coordinates (KHR_texture_transform on base colors).

Materials (PBR -> .mtl, the subset miniRT reads):
  baseColorFactor             -> Kd
  KHR_materials_transmission  -> d (opacity) and Ni: 1.0 for thin sheets
                                 like car windows, the model's ior when it
                                 has KHR_materials_volume (solid glass)
  smooth metal                -> illum 3 + Ks: mirror
  clearcoat, polished metal   -> illum 5 + Ks: reflects more at grazing
                                 angles (Fresnel), like car paint
  emissiveFactor (+ strength) -> Ke: glow (emissive textures are skipped)
  baseColorTexture            -> map_Kd with the image name ending in .ppm:
                                 convert the image to a binary .ppm of that
                                 name (ImageMagick: convert a.png a.ppm)
Normal, occlusion and roughness maps are ignored.

The car in tools/models/car was made with:
    python3 tools/models/gltf_to_obj.py CarConcept.gltf tools/models/car/concept_car \
        --skip-texture Khronos_C.png --flat-dark --skip-part BodyHoodInterior01 \
        --skip-part BodyHoodInterior02 --skip-part BodyHoodUnder --credit "Car \
        Concept by Eric Chadwick, Darmstadt Graphics Group GmbH, 2024, CC BY 4.0. \
        Converted for miniRT, see tools/models/CREDITS.md"
"""
import json
import math
import os
import struct
import sys

COMPONENT = {5120: ('b', 1), 5121: ('B', 1), 5122: ('h', 2), 5123: ('H', 2),
             5125: ('I', 4), 5126: ('f', 4)}
COUNT = {'SCALAR': 1, 'VEC2': 2, 'VEC3': 3, 'VEC4': 4, 'MAT4': 16}


def load(path):
    if path.endswith('.glb'):
        data = open(path, 'rb').read()
        json_len = struct.unpack_from('<I', data, 12)[0]
        gltf = json.loads(data[20:20 + json_len])
        bin_start = 20 + json_len + 8
        buffers = [data[bin_start:]]
    else:
        gltf = json.load(open(path))
        base = os.path.dirname(path)
        buffers = [open(os.path.join(base, b['uri']), 'rb').read()
                   for b in gltf['buffers']]
    return gltf, buffers


def read_accessor(gltf, buffers, index):
    acc = gltf['accessors'][index]
    view = gltf['bufferViews'][acc['bufferView']]
    fmt, size = COMPONENT[acc['componentType']]
    n = COUNT[acc['type']]
    stride = view.get('byteStride', size * n)
    start = view.get('byteOffset', 0) + acc.get('byteOffset', 0)
    buf = buffers[view['buffer']]
    out = []
    for i in range(acc['count']):
        vals = struct.unpack_from('<%d%s' % (n, fmt), buf, start + i * stride)
        if acc.get('normalized'):
            vals = tuple(v / float((1 << (8 * size - (fmt.islower()))) - 1)
                         for v in vals)
        out.append(vals)
    return out


def mat_mul(a, b):
    return [[sum(a[i][k] * b[k][j] for k in range(4)) for j in range(4)]
            for i in range(4)]


def node_matrix(node):
    if 'matrix' in node:
        m = node['matrix']
        return [[m[c * 4 + r] for c in range(4)] for r in range(4)]
    tx, ty, tz = node.get('translation', [0, 0, 0])
    qx, qy, qz, qw = node.get('rotation', [0, 0, 0, 1])
    sx, sy, sz = node.get('scale', [1, 1, 1])
    r = [[1 - 2 * (qy * qy + qz * qz), 2 * (qx * qy - qz * qw), 2 * (qx * qz + qy * qw)],
         [2 * (qx * qy + qz * qw), 1 - 2 * (qx * qx + qz * qz), 2 * (qy * qz - qx * qw)],
         [2 * (qx * qz - qy * qw), 2 * (qy * qz + qx * qw), 1 - 2 * (qx * qx + qy * qy)]]
    return [[r[0][0] * sx, r[0][1] * sy, r[0][2] * sz, tx],
            [r[1][0] * sx, r[1][1] * sy, r[1][2] * sz, ty],
            [r[2][0] * sx, r[2][1] * sy, r[2][2] * sz, tz],
            [0, 0, 0, 1]]


def normal_matrix(m):
    a = [row[:3] for row in m[:3]]
    det = (a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
           - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0])
           + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]))
    # inverse transpose = cofactor matrix / det
    cof = [[a[1][1] * a[2][2] - a[1][2] * a[2][1], -(a[1][0] * a[2][2] - a[1][2] * a[2][0]), a[1][0] * a[2][1] - a[1][1] * a[2][0]],
           [-(a[0][1] * a[2][2] - a[0][2] * a[2][1]), a[0][0] * a[2][2] - a[0][2] * a[2][0], -(a[0][0] * a[2][1] - a[0][1] * a[2][0])],
           [a[0][1] * a[1][2] - a[0][2] * a[1][1], -(a[0][0] * a[1][2] - a[0][2] * a[1][0]), a[0][0] * a[1][1] - a[0][1] * a[1][0]]]
    return [[c / det for c in row] for row in cof]


def transform(m, p, w):
    return tuple(m[i][0] * p[0] + m[i][1] * p[1] + m[i][2] * p[2] + m[i][3] * w
                 for i in range(3))


def walk(gltf, index, parent, out):
    node = gltf['nodes'][index]
    world = mat_mul(parent, node_matrix(node))
    if 'mesh' in node:
        out.append((node.get('name', 'node%d' % index), node['mesh'], world))
    for child in node.get('children', []):
        walk(gltf, child, world, out)


def uv_transform(info):
    t = info.get('extensions', {}).get('KHR_texture_transform', {})
    ox, oy = t.get('offset', [0, 0])
    sx, sy = t.get('scale', [1, 1])
    rot = t.get('rotation', 0.0)
    c, s = math.cos(rot), math.sin(rot)
    return lambda u, v: (c * sx * u + s * sy * v + ox, -s * sx * u + c * sy * v + oy)


def shine_lines(m):
    """Kd plus glass, reflection and glow lines of one material."""
    pbr = m.get('pbrMetallicRoughness', {})
    ext = m.get('extensions', {})
    r, g, b = pbr.get('baseColorFactor', [1, 1, 1, 1])[:3]
    metal = pbr.get('metallicFactor', 1.0)
    rough = pbr.get('roughnessFactor', 1.0)
    coat = ext.get('KHR_materials_clearcoat', {}).get('clearcoatFactor', 0)
    trans = ext.get('KHR_materials_transmission', {}).get('transmissionFactor', 0)
    kd = (r, g, b)
    shine = []
    if trans > 0:
        # without KHR_materials_volume the glass is a thin sheet (like car
        # windows, modeled as one surface): light passes without bending
        ior = 1.0
        if 'KHR_materials_volume' in ext:
            ior = ext.get('KHR_materials_ior', {}).get('ior', 1.5)
        shine = ['d %.3f' % max(0.05, 1 - trans * 0.92), 'Ni %.3f' % ior]
    elif metal > 0.5 and rough < 0.15:
        # mirror-like metal: its look comes from what it reflects
        kd = (r * 0.15, g * 0.15, b * 0.15)
        shine = ['illum 3', 'Ks %.3f' % (0.85 * (1 - rough))]
    else:
        gloss = min(1.0, max(0.0, (0.6 - rough) / 0.6))
        f0 = (0.05 + 0.15 * gloss if metal > 0.5 else 0.0) + 0.05 * coat
        if f0 > 0.05:
            shine = ['illum 5', 'Ks %.3f' % f0]
    lines = ['Kd %.4f %.4f %.4f' % kd] + shine
    if glows(m):
        strength = ext.get('KHR_materials_emissive_strength', {}).get('emissiveStrength', 1)
        ke = [e * strength for e in m['emissiveFactor']]
        if max(ke) > 1.5:
            ke = [e * 1.5 / max(ke) for e in ke]
        lines.append('Ke %.3f %.3f %.3f' % tuple(ke))
    return lines


def glows(m):
    """Emissive textures are skipped, so only a plain emissive color glows."""
    return ('emissiveTexture' not in m
            and max(m.get('emissiveFactor', [0, 0, 0])) > 0)


def write_mtl(gltf, path, skip_textures, header):
    lines = list(header)
    images = [im.get('uri', '') for im in gltf.get('images', [])]
    textures = gltf.get('textures', [])
    for i, m in enumerate(gltf.get('materials', [])):
        lines += ['', 'newmtl m%d_%s' % (i, m.get('name', '').replace(' ', '_'))]
        lines += shine_lines(m)
        tex = m.get('pbrMetallicRoughness', {}).get('baseColorTexture')
        if tex is not None:
            uri = images[textures[tex['index']]['source']]
            if os.path.basename(uri) not in skip_textures:
                lines.append('map_Kd %s' % os.path.basename(uri).replace('.png', '.ppm'))
    open(path, 'w').write('\n'.join(lines) + '\n')


def dark_and_dull(mat):
    pbr = mat.get('pbrMetallicRoughness', {})
    base = pbr.get('baseColorFactor', [1, 1, 1, 1])
    rough = pbr.get('roughnessFactor', 1.0)
    return (max(base[:3]) < 0.06 and rough > 0.3 and not glows(mat)
            and 'KHR_materials_transmission' not in mat.get('extensions', {}))


def num(x, digits):
    """Shortest text for x with at most digits decimals."""
    t = ('%.*f' % (digits, x)).rstrip('0').rstrip('.')
    return '0' if t in ('', '-0') else t


def weld(pos, idx):
    """Merges corners at the same position, so miniRT can smooth across
    texture seams, and drops the triangles that collapse."""
    seen, out_pos, remap = {}, [], []
    for p in pos:
        if p not in seen:
            seen[p] = len(out_pos)
            out_pos.append(p)
        remap.append(seen[p])
    tris = []
    for t in range(0, len(idx) - 2, 3):
        a, b, c = remap[idx[t]], remap[idx[t + 1]], remap[idx[t + 2]]
        if a != b and b != c and a != c:
            tris += [a, b, c]
    return out_pos, tris


def main():
    src, dst = sys.argv[1], sys.argv[2]
    skip = [sys.argv[i + 1] for i, a in enumerate(sys.argv) if a == '--skip-texture']
    skip_parts = [sys.argv[i + 1] for i, a in enumerate(sys.argv) if a == '--skip-part']
    flat_dark = '--flat-dark' in sys.argv
    digits = int(sys.argv[sys.argv.index('--digits') + 1]) if '--digits' in sys.argv else 4
    header = ['# Converted from glTF by gltf_to_obj.py']
    if '--credit' in sys.argv:
        header.append('# ' + sys.argv[sys.argv.index('--credit') + 1])
    gltf, buffers = load(src)
    scene = gltf['scenes'][gltf.get('scene', 0)]
    ident = [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]
    parts = []
    for root in scene['nodes']:
        walk(gltf, root, ident, parts)
    name = os.path.basename(dst)
    write_mtl(gltf, dst + '.mtl', skip, header)
    out = header + ['mtllib %s.mtl' % name]
    nv = nt = nn = 0
    for part, mesh_index, world in parts:
        if part in skip_parts:
            continue
        nmat = normal_matrix(world)
        for prim in gltf['meshes'][mesh_index]['primitives']:
            attr = prim['attributes']
            pos = read_accessor(gltf, buffers, attr['POSITION'])
            nrm = read_accessor(gltf, buffers, attr['NORMAL']) if 'NORMAL' in attr else None
            uvs = read_accessor(gltf, buffers, attr['TEXCOORD_0']) if 'TEXCOORD_0' in attr else None
            idx = [i[0] for i in read_accessor(gltf, buffers, prim['indices'])] \
                if 'indices' in prim else list(range(len(pos)))
            mi = prim.get('material')
            mat = gltf['materials'][mi] if mi is not None else {}
            tex = mat.get('pbrMetallicRoughness', {}).get('baseColorTexture')
            if tex is None:
                uvs = None
            if flat_dark and dark_and_dull(mat):
                nrm = None
                uvs = None
                pos, idx = weld(pos, idx)
            out += ['o %s' % part.replace(' ', '_'),
                    'usemtl m%d_%s' % (mi, mat.get('name', '').replace(' ', '_'))]
            for p in pos:
                out.append('v ' + ' '.join(num(c, digits) for c in transform(world, p, 1)))
            if nrm:
                for n in nrm:
                    v = [sum(nmat[i][k] * n[k] for k in range(3)) for i in range(3)]
                    l = math.sqrt(sum(c * c for c in v)) or 1
                    out.append('vn ' + ' '.join(num(c / l, 3) for c in v))
            if uvs:
                fix = uv_transform(tex)
                for u, v in uvs:
                    tu, tv = fix(u, v)
                    out.append('vt %s %s' % (num(tu, 4), num(1 - tv, 4)))
            for t in range(0, len(idx) - 2, 3):
                c = [idx[t] + 1, idx[t + 1] + 1, idx[t + 2] + 1]
                if nrm and uvs:
                    f = ['%d/%d/%d' % (nv + i, nt + i, nn + i) for i in c]
                elif nrm:
                    f = ['%d//%d' % (nv + i, nn + i) for i in c]
                elif uvs:
                    f = ['%d/%d' % (nv + i, nt + i) for i in c]
                else:
                    f = ['%d' % (nv + i) for i in c]
                out.append('f ' + ' '.join(f))
            nv += len(pos)
            nt += len(uvs) if uvs else 0
            nn += len(nrm) if nrm else 0
    open(dst + '.obj', 'w').write('\n'.join(out) + '\n')
    print('%s.obj: %d parts, %d vertices' % (dst, len(parts), nv))


if __name__ == '__main__':
    main()
