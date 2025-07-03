#ifndef DRAW_BOARD_H
#define DRAW_BOARD_H

#include "ccfwd.h"

#include "piece.h"

#include <vector>

/// Draw the pieces
void draw_pieces(
  const std::vector<piece>& pieces,
  const board_layout& layout
);

/// Draw the squares of a chessboard at the window target rectangle's location
void draw_squares(
  const board_layout& layout,
  const bool semi_transparent = true
);

/// Show the pieces' health bars on-screen
void draw_unit_health_bars(
  const game& g,
  const board_layout& layout
);

#endif // DRAW_BOARD_H
