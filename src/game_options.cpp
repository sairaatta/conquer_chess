#include "game_options.h"

#include "layout.h"

#include <cassert>
#include <iostream>
#include <sstream>

game_options::game_options(
) : m_click_distance{get_default_click_distance()},
    m_game_speed{get_default_game_speed()},
    m_margin_width{get_default_margin_width()},
    m_starting_position{get_default_starting_position()},
    m_music_volume{get_default_music_volume()},
    m_sound_effects_volume{get_default_sound_effects_volume()}
{
  assert(m_click_distance > 0.0);
  assert(m_margin_width >= 0);
}

void decrease_game_speed() noexcept
{
  game_options::get().set_game_speed(get_previous(game_options::get().get_game_speed()));
}

void decrease_music_volume() noexcept
{
  game_options::get().set_music_volume(get_previous(game_options::get().get_music_volume()));
}

void decrease_sound_effects_volume() noexcept
{
  game_options::get().set_sound_effects_volume(get_previous(game_options::get().get_sound_effects_volume()));
}

void decrease_starting_position() noexcept
{
  game_options::get().set_starting_position(get_previous(get_starting_position()));
}

bool do_show_selected() noexcept
{
  return game_options::get().get_show_selected();
}

volume get_default_music_volume() noexcept
{
  return volume(10);
}

volume get_default_sound_effects_volume() noexcept
{
  return volume(20);
}

const volume& get_music_volume() noexcept
{
  return game_options::get().get_music_volume();
}

double get_music_volume_as_percentage() noexcept
{
  return get_music_volume().get_percentage();
}

const volume& get_sound_effects_volume() noexcept
{
  return game_options::get().get_sound_effects_volume();
}

starting_position_type get_starting_position() noexcept
{
  return game_options::get().get_starting_position();
}

void increase_game_speed() noexcept
{
  game_options::get().set_game_speed(get_next(game_options::get().get_game_speed()));
}

void increase_music_volume() noexcept
{
  game_options::get().set_music_volume(get_next(game_options::get().get_music_volume()));
}

void increase_sound_effects_volume() noexcept
{
  game_options::get().set_sound_effects_volume(get_next(game_options::get().get_sound_effects_volume()));
}

void increase_starting_position() noexcept
{
  game_options::get().set_starting_position(get_next(get_starting_position()));
}

void game_options::reset()
{
  m_click_distance = get_default_click_distance();
  m_game_speed = get_default_game_speed();
  m_margin_width = get_default_margin_width();
  m_starting_position = get_default_starting_position();
  m_music_volume = get_default_music_volume();
  m_sound_effects_volume = get_default_sound_effects_volume();
}

void test_game_options()
{
#ifndef NDEBUG
  // decrease_game_speed and increase_game_speed
  {
    const auto s{game_speed::normal};
    game_options::get().set_game_speed(s);
    decrease_game_speed();
    assert(s != game_options::get().get_game_speed());
    increase_game_speed();
    assert(s == game_options::get().get_game_speed());
  }
  // decrease_music_volume and increase_music_volume
  {
    const auto s{volume(50)};
    game_options::get().set_music_volume(s);
    decrease_music_volume();
    assert(s != game_options::get().get_music_volume());
    increase_music_volume();
    assert(s == game_options::get().get_music_volume());
  }
  // decrease_sound_effects_volume and increase_sound_effects_volume
  {
    const auto s{volume(50)};
    game_options::get().set_sound_effects_volume(s);
    decrease_sound_effects_volume();
    assert(s != game_options::get().get_sound_effects_volume());
    increase_sound_effects_volume();
    assert(s == game_options::get().get_sound_effects_volume());
  }
  // decrease_starting_position and increase_starting_position
  {
    const auto s{starting_position_type::kings_only};
    game_options::get().set_starting_position(s);
    decrease_starting_position();
    assert(s != game_options::get().get_starting_position());
    increase_starting_position();
    assert(s == game_options::get().get_starting_position());
  }
  // game_options::do_show_occupied
  {
    assert(!game_options::get().get_show_occupied());
  }
  // game_options::do_show_selected
  {
    assert(!do_show_selected());
  }
  // game_options::set_music_volume
  {
    const auto volume_before{get_music_volume()};
    assert(volume_before == get_music_volume());
    const volume volume_after(get_next(volume_before));
    assert(volume_before != volume_after);
    game_options::get().set_music_volume(volume_after);
    assert(get_music_volume() == volume_after);
  }
  // get_music_volume_as_percentage
  {
    const auto volume_as_percentage{get_music_volume_as_percentage()};
    assert(volume_as_percentage >= 0.0);
    assert(volume_as_percentage <= 100.001);
  }
  // get_sound_effects_volume
  {
    assert(get_sound_effects_volume() == game_options::get().get_sound_effects_volume());
  }
  // reset
  {
    game_options::get().reset();
    assert(game_options::get().get_click_distance() == get_default_click_distance());
    assert(game_options::get().get_game_speed() == get_default_game_speed());
    assert(game_options::get().get_margin_width() == get_default_margin_width());
    assert(game_options::get().get_starting_position() == get_default_starting_position());
    assert(game_options::get().get_music_volume() == get_default_music_volume());
    assert(game_options::get().get_sound_effects_volume() == get_default_sound_effects_volume());
  }
  // set_game_speed
  {
    for (const auto s: { game_speed::slowest, game_speed::fastest })
    {
      game_options::get().set_game_speed(s);
      assert(game_options::get().get_game_speed() == s);
    }
  }
  // set_music_volume
  {
    for (const auto v: { volume(10), volume(20) })
    {
      game_options::get().set_music_volume(v);
      assert(game_options::get().get_music_volume() == v);
    }
  }
  // set_sound_effects_volume
  {
    for (const auto v: { volume(10), volume(20) })
    {
      game_options::get().set_sound_effects_volume(v);
      assert(game_options::get().get_sound_effects_volume() == v);
    }
  }
  // set_starting_position
  {
    for (const auto p: { starting_position_type::before_en_passant, starting_position_type::standard })
    {
      game_options::get().set_starting_position(p);
      assert(game_options::get().get_starting_position() == p);
    }
  }
  // 40: operator<<
  {
    std::stringstream s;
    s << game_options::get();
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

std::ostream& operator<<(std::ostream& os, const game_options& options) noexcept
{
  os
    << "click distance: " << options.get_click_distance() << '\n'
    << "damage per chess move: " << options.get_damage_per_chess_move() << '\n'
    << "game speed: " << options.get_game_speed() << '\n'
    << "Margin width: " << options.get_margin_width() << '\n'
    << "Message display time (sec): " << options.get_message_display_time_secs() << '\n'
    << "Starting position: " << options.get_starting_position() << '\n'
    << "Music volume: " << options.get_music_volume() << '\n'
    << "Sound effects volume: " << options.get_sound_effects_volume()
  ;
  return os;
}
