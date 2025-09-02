#ifndef DIAGNOSTICS_FILE_H
#define DIAGNOSTICS_FILE_H

#include "ccfwd.h"

#include <string>

std::string get_default_diagnostics_filename()  noexcept;

/// Work on a diagnostics file
///
/// Conquer Chess produces a diagnostics file to help fix bugs quicker.
///
/// This file is always appended, so that a user never looses information
class diagnostics_file
{
public:
  diagnostics_file(const std::string filename = get_default_diagnostics_filename());

  /// Add the command-line options to the file
  void add_cli_options(const cc_cli_options& options);

  /// Add the footer, i.e. show the game closed successfully, to the file
  void add_footer();

  /// Add the header, i.e. the most basic info, to the file
  void add_header();

  /// Add the screen size to the file.
  ///
  /// Called upon a screen resize
  void add_screen_size(const int width, const int height);

private:
  std::string m_filename;
};

/// Test the about_view_layout class
void test_diagnostics_file();

#endif // DIAGNOSTICS_FILE_H
