#!/bin/env python
#
# Convert a WKT to PNG
#
# Usage:
#
#  ./scripts/convert_wkt_to_png.py from.wkt to.png
#
#
#

import sys
from shapely import wkt
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon as MplPolygon

def read_wkt_polygons(filename):
    polygons = []
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if line:
                geom = wkt.loads(line)
                if geom.geom_type == 'Polygon':
                    polygons.append(geom)
    return polygons

def get_extent(polygons):
    min_x, min_y, max_x, max_y = None, None, None, None
    for poly in polygons:
        xs, ys = poly.exterior.xy
        poly_min_x, poly_max_x = min(xs), max(xs)
        poly_min_y, poly_max_y = min(ys), max(ys)
        min_x = poly_min_x if min_x is None else min(min_x, poly_min_x)
        max_x = poly_max_x if max_x is None else max(max_x, poly_max_x)
        min_y = poly_min_y if min_y is None else min(min_y, poly_min_y)
        max_y = poly_max_y if max_y is None else max(max_y, poly_max_y)
    return min_x, min_y, max_x, max_y

def draw_pixel_perfect(polygons, png_file):
    min_x, min_y, max_x, max_y = get_extent(polygons)
    width = int(max_x - min_x)
    height = int(max_y - min_y)

    # Set DPI so figure size * dpi = pixel size
    dpi = 100
    fig_width = width / dpi
    fig_height = height / dpi

    fig = plt.figure(figsize=(fig_width, fig_height), dpi=dpi)
    ax = fig.add_axes([0, 0, 1, 1])
    ax.set_xlim(min_x, max_x)
    ax.set_ylim(min_y, max_y)
    ax.set_aspect('equal')
    plt.axis('off')

    for poly in polygons:
        x, y = poly.exterior.xy
        ax.add_patch(MplPolygon(list(zip(x, y)), closed=True, fill=True, edgecolor='black', facecolor='lightblue'))
        for interior in poly.interiors:
            xh, yh = interior.xy
            ax.add_patch(MplPolygon(list(zip(xh, yh)), closed=True, fill=True, edgecolor='black', facecolor='white'))

    # Invert y-axis if you want (0,0) at top-left (like image coordinates)
    ax.invert_yaxis()

    plt.savefig(png_file, dpi=dpi, bbox_inches=None, pad_inches=0)
    plt.close()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Friendly error: Please specify the input WKT filename and output PNG filename.")
        print("Example usage: python wkt_pixel_perfect.py input.wkt output.png")
        sys.exit(1)
    wkt_file = sys.argv[1]
    png_file = sys.argv[2]
    polygons = read_wkt_polygons(wkt_file)
    if not polygons:
        print("Friendly error: No polygons found in the input file.")
        sys.exit(1)
    try:
        draw_pixel_perfect(polygons, png_file)
        print(f"Successfully converted '{wkt_file}' to '{png_file}' with a 1-to-1 pixel match.")
    except Exception as e:
        print(f"Friendly error: {e}")
        sys.exit(1)

