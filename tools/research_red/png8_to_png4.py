#!/usr/bin/env python3
"""Research Red: convert an 8-bit indexed PNG (indices 0-15 only) to a
4-bit, 16-color indexed PNG so gbagfx emits a 16-color .gbapal.

Usage: png8_to_png4.py IN.png OUT.png
Index 0 stays the transparent color. No third-party modules needed.
"""
import struct
import sys
import zlib


def read_chunks(data):
    pos = 8
    while pos < len(data):
        length, ctype = struct.unpack(">I4s", data[pos:pos + 8])
        yield ctype, data[pos + 8:pos + 8 + length]
        pos += 12 + length


def unfilter(raw, width, height):
    rows, prev, pos = [], bytearray(width), 0
    for _ in range(height):
        ftype = raw[pos]
        line = bytearray(raw[pos + 1:pos + 1 + width])
        pos += 1 + width
        for x in range(width):
            a = line[x - 1] if x else 0
            b = prev[x]
            c = prev[x - 1] if x else 0
            if ftype == 1:
                line[x] = (line[x] + a) & 0xFF
            elif ftype == 2:
                line[x] = (line[x] + b) & 0xFF
            elif ftype == 3:
                line[x] = (line[x] + (a + b) // 2) & 0xFF
            elif ftype == 4:
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                pred = a if pa <= pb and pa <= pc else (b if pb <= pc else c)
                line[x] = (line[x] + pred) & 0xFF
        rows.append(line)
        prev = line
    return rows


def chunk(ctype, body):
    return struct.pack(">I", len(body)) + ctype + body + struct.pack(">I", zlib.crc32(ctype + body))


def main(src, dst):
    data = open(src, "rb").read()
    idat, plte = b"", b""
    for ctype, body in read_chunks(data):
        if ctype == b"IHDR":
            width, height, depth, color = struct.unpack(">IIBB", body[:10])
        elif ctype == b"PLTE":
            plte = body
        elif ctype == b"IDAT":
            idat += body
    if depth != 8 or color != 3:
        sys.exit(f"{src}: expected 8-bit indexed PNG")
    rows = unfilter(zlib.decompress(idat), width, height)
    if max(max(r) for r in rows) > 15:
        sys.exit(f"{src}: uses palette index > 15")
    out = bytearray()
    for r in rows:
        out.append(0)
        for x in range(0, width, 2):
            out.append((r[x] << 4) | (r[x + 1] if x + 1 < width else 0))
    pal = (plte + bytes(48))[:48]
    png = b"\x89PNG\r\n\x1a\n"
    png += chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 4, 3, 0, 0, 0))
    png += chunk(b"PLTE", pal)
    png += chunk(b"IDAT", zlib.compress(bytes(out), 9))
    png += chunk(b"IEND", b"")
    open(dst, "wb").write(png)


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2])
