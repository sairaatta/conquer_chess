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

# Start of AI generated code, by GitHub Copilot
import sys
from shapely import wkt
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon as MplPolygon

def wkt_polygon_to_png(wkt_file, png_file):
    with open(wkt_file, 'r') as f:
        geom = wkt.loads(f.read().strip())

    if geom.geom_type != 'Polygon':
        raise ValueError(f"Geometry type is {geom.geom_type}, expected 'Polygon'.")

    fig, ax = plt.subplots()
    ax.set_aspect('equal')
    plt.axis('off')

    # Draw the exterior
    x, y = geom.exterior.xy
    ax.add_patch(MplPolygon(list(zip(x, y)), closed=True, fill=True, edgecolor='black', facecolor='lightblue'))

    # Draw interior holes, if any
    for interior in geom.interiors:
        x, y = interior.xy
        ax.add_patch(MplPolygon(list(zip(x, y)), closed=True, fill=True, edgecolor='black', facecolor='white'))

    plt.savefig(png_file, bbox_inches='tight', pad_inches=0)
    plt.close()

if len(sys.argv) != 3:
    print(
      "FATAL ERROR.\n",
      "\n",
      "Please add the filename of the input and output file.\n",
      "\n",
      "For example:\n",
      "\n",
      "  ./wkt_polygon_to_png.py from.wkt to.png \n",
      "\n",
      sep = ''
    )
    sys.exit(42)

wkt_file = sys.argv[1]
png_file = sys.argv[2]
wkt_polygon_to_png(wkt_file, png_file)


