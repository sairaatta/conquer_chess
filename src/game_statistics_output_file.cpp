#include "game_statistics_output_file.h"

#include "game_statistics.h"

#include <cassert>
#include <fstream>
#include <filesystem>

game_statistics_output_file::game_statistics_output_file(const std::string& filename)
  : m_filename{filename}
{
  std::ofstream f(filename);
  f << get_column_headers_as_str() << '\n';
}

void game_statistics_output_file::add_to_file(const game& g)
{
  const game_statistics s(g);
  std::ofstream f(m_filename, std::ios::app);
  f << to_comma_seperated_str(flatten_to_row(s)) << '\n';
}

std::string get_column_headers_as_str()
{
  return to_comma_seperated_str(get_column_headers());
}

void test_game_statistics_output_file()
{
#ifndef NDEBUG
  // Constructor creates a file
  {
    const std::string filename("tmp.csv");
    assert(!std::filesystem::exists(filename));
  }
  // column_headers_to_str
  {
    assert(!get_column_headers_as_str().empty());
  }
#endif
}

template<class T>
std::string to_comma_seperated_str_impl(const std::vector<T>& v)
{
  std::stringstream s;
  for (const auto& t: v)
  {
    s << t << ',';
  }
  std::string t{s.str()};
  if (!t.empty()) t.pop_back();
  return t;
}


// No templates, to reduce compile-time
std::string to_comma_seperated_str(const std::vector<double>& v)
{
  return to_comma_seperated_str_impl(v);
}

// No templates, to reduce compile-time
std::string to_comma_seperated_str(const std::vector<std::string>& v)
{
  return to_comma_seperated_str_impl(v);
}
