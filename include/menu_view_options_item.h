#ifndef MENU_VIEW_OPTIONS_ITEM_H
#define MENU_VIEW_OPTIONS_ITEM_H

#include <vector>

/// The sub items in the Options of the Menu dialog.
enum class menu_view_options_item
{
  controls,
  laws,
  sound_and_video
};

std::vector<menu_view_options_item> get_all_menu_view_options_items() noexcept;

menu_view_options_item get_next(const menu_view_options_item there);
menu_view_options_item get_previous(const menu_view_options_item& there);

void test_menu_view_options_item();

#endif // MENU_VIEW_OPTIONS_ITEM_H
