"""Research Red: tiny PNG reader/writer (no third-party modules).
read_png(path) -> (w, h, rows) with rows of (r, g, b, a) tuples for
truecolor/RGBA/indexed images; write_png_rgb / write_png_indexed for output."""
import struct
import zlib


def _chunks(data):
    pos = 8
    while pos < len(data):
        length, ctype = struct.unpack(">I4s", data[pos:pos + 8])
        yield ctype, data[pos + 8:pos + 8 + length]
        pos += 12 + length


def _unfilter(raw, stride, bpp, h):
    out = bytearray(stride * h)
    prev = bytearray(stride)
    p = 0
    for y in range(h):
        f = raw[p]
        line = bytearray(raw[p + 1:p + 1 + stride])
        p += 1 + stride
        if f == 1:
            for x in range(bpp, stride):
                line[x] = (line[x] + line[x - bpp]) & 255
        elif f == 2:
            for x in range(stride):
                line[x] = (line[x] + prev[x]) & 255
        elif f == 3:
            for x in range(stride):
                a = line[x - bpp] if x >= bpp else 0
                line[x] = (line[x] + ((a + prev[x]) >> 1)) & 255
        elif f == 4:
            for x in range(stride):
                a = line[x - bpp] if x >= bpp else 0
                b = prev[x]
                c = prev[x - bpp] if x >= bpp else 0
                pa = abs(b - c)
                pb = abs(a - c)
                pc = abs(a + b - 2 * c)
                line[x] = (line[x] + (a if pa <= pb and pa <= pc else (b if pb <= pc else c))) & 255
        out[y * stride:(y + 1) * stride] = line
        prev = line
    return out


def read_png(path):
    data = open(path, "rb").read()
    idat, plte, trns = b"", b"", b""
    for t, c in _chunks(data):
        if t == b"IHDR":
            w, h, bd, ct = struct.unpack(">IIBB", c[:10])
        elif t == b"PLTE":
            plte = c
        elif t == b"tRNS":
            trns = c
        elif t == b"IDAT":
            idat += c
    raw = zlib.decompress(idat)
    if ct == 3:
        stride = (w * bd + 7) // 8
        px = _unfilter(raw, stride, 1, h)
        pal = [tuple(plte[i:i + 3]) for i in range(0, len(plte), 3)]
        alpha = list(trns) + [255] * (256 - len(trns))
        rows = []
        for y in range(h):
            row = []
            for x in range(w):
                if bd == 8:
                    i = px[y * stride + x]
                else:
                    b = px[y * stride + (x * bd) // 8]
                    shift = 8 - bd - (x * bd) % 8
                    i = (b >> shift) & ((1 << bd) - 1)
                row.append(pal[i] + (alpha[i],))
            rows.append(row)
        return w, h, rows
    bpp = {2: 3, 6: 4}[ct]
    stride = w * bpp
    px = _unfilter(raw, stride, bpp, h)
    rows = []
    for y in range(h):
        b = px[y * stride:(y + 1) * stride]
        if bpp == 4:
            rows.append([tuple(b[i:i + 4]) for i in range(0, stride, 4)])
        else:
            rows.append([tuple(b[i:i + 3]) + (255,) for i in range(0, stride, 3)])
    return w, h, rows


def _chunk(t, b):
    return struct.pack(">I", len(b)) + t + b + struct.pack(">I", zlib.crc32(t + b))


def write_png_rgb(path, w, h, rows):
    raw = b"".join(b"\x00" + bytes(v for p in r for v in p[:3]) for r in rows)
    open(path, "wb").write(b"\x89PNG\r\n\x1a\n" + _chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0))
                           + _chunk(b"IDAT", zlib.compress(raw, 9)) + _chunk(b"IEND", b""))


def write_png_indexed4(path, w, h, idx_rows, palette):
    """4-bit indexed PNG; palette is a list of up to 16 (r, g, b)."""
    raw = bytearray()
    for r in idx_rows:
        raw.append(0)
        for x in range(0, w, 2):
            raw.append((r[x] << 4) | (r[x + 1] if x + 1 < w else 0))
    pal = b"".join(bytes(c) for c in palette) + bytes(3 * (16 - len(palette)))
    open(path, "wb").write(b"\x89PNG\r\n\x1a\n" + _chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 4, 3, 0, 0, 0))
                           + _chunk(b"PLTE", pal) + _chunk(b"IDAT", zlib.compress(bytes(raw), 9)) + _chunk(b"IEND", b""))
