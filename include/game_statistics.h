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
  game_statistics(const game& g);

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
