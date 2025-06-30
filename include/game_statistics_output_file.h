#ifndef GAME_STATISTICS_OUTPUT_FILE_H
#define GAME_STATISTICS_OUTPUT_FILE_H

#include "ccfwd.h"

#include <string>
#include <vector>

/// File to save the game statistics too
class game_statistics_output_file
{
public:
  game_statistics_output_file(const std::string& filename);

  void add_to_file(const game& g);

private:

  std::string m_filename;
};

/// Get the column headers as one, comma-seperated string
std::string get_column_headers_as_str();

void test_game_statistics_output_file();


// No templates, to reduce compile-time
std::string to_comma_seperated_str(const std::vector<double>& v);

// No templates, to reduce compile-time
std::string to_comma_seperated_str(const std::vector<std::string>& v);


#endif // GAME_STATISTICS_OUTPUT_FILE_H

