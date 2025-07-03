#ifndef GAME_STATISTICS_H
#define GAME_STATISTICS_H

#include "ccfwd.h"
#include "game_statistic_type.h"
#include "chess_color.h"
#include "side.h"

#include <map>

/// The statistics of a point in time of the game
class game_statistics
{
public:
  game_statistics(const game_controller& c);
  game_statistics(const std::map<game_statistic_type, std::map<side, double>>& s);

  /// Calculate the relative value between the two players for a statistic,
  /// where 0.5 denotes 'equally much'.
  ///
  /// LHS|RHS|f
  /// ---|---|---
  /// 0.0|0.0|0.5
  /// 0.0|0.5|0.0
  /// 0.5|0.0|1.0
  /// 0.5|0.5|0.5
  double calc_relative(const game_statistic_type s) const;

  double get(const game_statistic_type stat, const side s) const;

private:
  std::map<game_statistic_type, std::map<side, double>> m_statistics;
};

/// Create a game statistics to be used in testing
game_statistics create_test_game_statistics();

/// Put the values in a row, to be saved to a file
///
/// ```console
/// time, value_lhs, value_rhs, activity_lhs, activity_rhs, protectedness_lhs, protectedness_rhs
/// ```
///
/// @seealso \link{get_column_headers} gets the column headers
std::vector<double> flatten_to_row(const game_statistics& s);

/// Put the values in a row, to be saved to a file
///
/// ```console
/// time, value_lhs, value_rhs, activity_lhs, activity_rhs, protectedness_lhs, protectedness_rhs
/// ```
///
/// @seealso \link{flatten_to_row} gets the values in a row
std::vector<std::string> get_column_headers() noexcept;

/// Get the fraction of pieces that is doing an action
double get_f_active(
  const std::vector<piece>& pieces,
  const chess_color c
);

/// Get the fraction of pieces that is protected by another
double get_f_protected(
  const std::vector<piece>& pieces,
  const chess_color c
);

/// Get the fraction of the current pieces' value from the maximum
double get_f_value(
  const std::vector<piece>& pieces,
  const chess_color c
);


void test_game_statistics();

#endif // GAME_STATISTICS_H
