#ifndef DIAGNOSTICS_FILE_H
#define DIAGNOSTICS_FILE_H

#include <string>

std::string get_default_diagnostics_filename()  noexcept;

/// Work on a diagnostics file
///
/// Conquer Chess produces a diagnostics file to help fix bugs quicker
class diagnostics_file
{
public:
  diagnostics_file(const std::string filename = get_default_diagnostics_filename());
  void add_header();
  void add_screen_size(const int width, const int height);

private:

  std::string m_filename;
};

/// Test the about_view_layout class
void test_diagnostics_file();

#endif // DIAGNOSTICS_FILE_H
