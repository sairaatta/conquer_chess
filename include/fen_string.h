#ifndef FEN_STRING_H
#define FEN_STRING_H

#include "chess_color.h"
#include "starting_position_type.h"

#include <string>

/// Forsyth–Edwards Notation string.
///
/// A way to capture a board state as a string,
/// for example, for the starting state:
///
/// ```text
/// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
/// ```
///
/// @seealso \url{https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation}
class fen_string
{
public:
  fen_string(const std::string& fen_str);

  const auto& get() const noexcept { return m_fen_str; }

private:

  std::string m_fen_str;
};

/// Create the FEN string of the standard starting position
///
/// The active player can be changed to black.
/// This would be equal to say that black starts.
fen_string create_fen_string_of_standard_starting_position(
  const chess_color active_player = chess_color::white
);

fen_string create_fen_string_with_starting_position(
  const starting_position_type t,
  const chess_color active_player = chess_color::white
);

/// Extract the color from a FEN string
chess_color get_color(const fen_string& s);

/// Get an FEN string for a position when doing a scholar's mate.
///
/// This is the initial position.
///
/// The scholar's mate as a PGN is \link{get_scholars_mate_as_pgn_str}
/// and the PGN string of the scholar's mate is similar to this:
///
/// ```text
/// 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
/// ```
fen_string get_fen_string_scholars_mate_0() noexcept;

/// Get an FEN string for a position when doing a scholar's mate.
///
/// This is after `1. e4`.
///
/// The scholar's mate as a PGN is \link{get_scholars_mate_as_pgn_str}
/// and the PGN string of the scholar's mate is similar to this:
///
/// ```text
/// 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
/// ```
fen_string get_fen_string_scholars_mate_1() noexcept;

/// Get an FEN string for a position when doing a scholar's mate.
///
/// This is after `1. e4 e5`.
///
/// The scholar's mate as a PGN is \link{get_scholars_mate_as_pgn_str}
/// and the PGN string of the scholar's mate is similar to this:
///
/// ```text
/// 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
/// ```
fen_string get_fen_string_scholars_mate_2() noexcept;

/// Get an FEN string for a position when doing a scholar's mate.
///
/// This is after `1. e4 e5 2. Qh5`.
///
/// The scholar's mate as a PGN is \link{get_scholars_mate_as_pgn_str}
/// and the PGN string of the scholar's mate is similar to this:
///
/// ```text
/// 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
/// ```
fen_string get_fen_string_scholars_mate_3() noexcept;

/// Get an FEN string for a position when doing a scholar's mate.
///
/// This is after `1. e4 e5 2. Qh5 Nc6`.
///
/// The scholar's mate as a PGN is \link{get_scholars_mate_as_pgn_str}
/// and the PGN string of the scholar's mate is similar to this:
///
/// ```text
/// 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
/// ```
fen_string get_fen_string_scholars_mate_4() noexcept;

/// Get an FEN string for a position when doing a scholar's mate.
///
/// This is after `1. e4 e5 2. Qh5 Nc6 3. Bc4`.
///
/// The scholar's mate as a PGN is \link{get_scholars_mate_as_pgn_str}
/// and the PGN string of the scholar's mate is similar to this:
///
/// ```text
/// 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
/// ```
fen_string get_fen_string_scholars_mate_5() noexcept;

/// Get an FEN string for a position when doing a scholar's mate.
///
/// This is after `1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6`.
///
/// The scholar's mate as a PGN is \link{get_scholars_mate_as_pgn_str}
/// and the PGN string of the scholar's mate is similar to this:
///
/// ```text
/// 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
/// ```
fen_string get_fen_string_scholars_mate_6() noexcept;

/// Get an FEN string for a position when doing a scholar's mate.
///
/// This is after `1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 4. Qxf7#`.
///
/// The scholar's mate as a PGN is \link{get_scholars_mate_as_pgn_str}
/// and the PGN string of the scholar's mate is similar to this:
///
/// ```text
/// 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
/// ```
fen_string get_fen_string_scholars_mate_7() noexcept;

/// Get an example FEN string from Wikipedia.
///
/// It is the starting position.
///
/// From \url{https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation#Examples}
fen_string get_fen_string_wikipedia_0() noexcept;

/// Get an example FEN string from Wikipedia.
///
/// It is the position after e4.
///
/// From \url{https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation#Examples}
fen_string get_fen_string_wikipedia_1() noexcept;

/// Get an example FEN string from Wikipedia.
///
/// It is the position after 1. e4. c5
///
/// From \url{https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation#Examples}
fen_string get_fen_string_wikipedia_2() noexcept;

/// Get an example FEN string from Wikipedia.
///
/// It is the position after 1. e4. c5 2. Nf3
///
/// From \url{https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation#Examples}
fen_string get_fen_string_wikipedia_4() noexcept;

void test_fen_string();

bool operator==(const fen_string& lhs, const fen_string& rhs) noexcept;


#endif // FEN_STRING_H
