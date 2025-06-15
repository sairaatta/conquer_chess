#include "resource_loader.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"
#include <cassert>

resource_loader::resource_loader()
  : m_index{0}
{

}

double get_progress(const resource_loader& loader) noexcept
{
  assert(loader.get_n_items() > 0);
  const double f{
    static_cast<double>(loader.get_index())
    / static_cast<double>(loader.get_n_items())
  };
  assert(f >= 0.0);
  assert(f <= 1.0);
  return f;
}

bool resource_loader::is_done() const noexcept
{
  assert(m_index <= get_n_items());
  return m_index == get_n_items();
}

void resource_loader::process_next()
{
  if (is_done()) return;
  game_resources& resources{game_resources::get()};

  // The loading takes place when the number of resources in that
  // group is calculated
  switch (m_index)
  {
    case 0:
      m_descriptor = "Start loading";
      break;
    case 1:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_loading_screen_fonts())
        + " loading screen fonts";
      break;
    case 2:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_loading_screen_songs())
        + " loading screen songs";
      break;
    case 3:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_loading_screen_textures())
        + " loading screen textures";
      break;
    case 4:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_fonts())
        + " fonts";
      break;
    case 5:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_songs())
        + " songs";
      break;
    case 6:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_songs())
        + " songs";
      break;
    case 7:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_sound_effects())
        + " sound effects";
      break;
    case 8:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_options_menu_textures())
        + " game options menu textures";
      break;
    case 9:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_map_textures())
        + " maps";
      break;
    case 10:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_piece_textures())
        + " piece textures";
      break;
    case 11:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_piece_action_textures())
        + " piece actions";
      break;
    case 12:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_piece_portrait_textures())
        + " piece portraits";
      break;
    case 13:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_lobby_menu_textures())
        + " lobby menu textures";
      break;
    default:
    case 14:
      assert(m_index == 14);
      assert(m_index + 1 == get_n_items()); // If not, update get_m_items
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_textures())
        + " textures";
      break;
  }
  ++m_index;
  assert(m_index <= get_n_items());
}

void test_resource_loader()
{
  resource_loader r;
  assert(get_progress(r) == 0.0);
  assert(r.get_index() == 0);
  assert(!r.is_done());
  for (int i{0}; i != r.get_n_items(); ++i)
  {
    assert(r.get_index() == i);
    r.process_next();
  }
  assert(get_progress(r) == 1.0);
  assert(r.get_index() == r.get_n_items());
  assert(r.is_done());
}


#endif // LOGIC_ONLY
