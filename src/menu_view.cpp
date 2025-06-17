#include "menu_view.h"

#ifndef LOGIC_ONLY

#include "draw.h"
#include "game_resources.h"
#include "game_options.h"
#include "render_window.h"
#include "screen_coordinate.h"
#include "sfml_helper.h"

#include <SFML/Window/Window.hpp>
#include <SFML/Window/ContextSettings.hpp>

#include <cassert>
#include <cmath>
#include <iostream>

menu_view::menu_view(
) :
    m_background_image_index{
      create_seedless_random_background_image_index()
    },
    m_selected{menu_view_item::start}
{
}

int create_random_background_image_index(
  std::default_random_engine& rng_engine
) {
  std::uniform_int_distribution<int> distribution{
    0,
    4 - 1 // -1 as inclusive
  };
  const auto i{distribution(rng_engine)};
  assert(i >= 0);
  assert(i < 4);
  return i;
}

int create_seedless_random_background_image_index() {
  std::random_device device;
  std::default_random_engine rng_engine(device());
  return create_random_background_image_index(rng_engine);
}

bool menu_view::process_event(sf::Event& event)
{
  if (event.type == sf::Event::Closed)
  {
    return true; // Close the program
  }
  if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Escape)
    {
      return true; // Close the program
    }
    else if (key_pressed == sf::Keyboard::Key::F4)
    {
      return true; // Close the program
    }
  }

  if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Up)
    {
      set_selected(get_previous(m_selected));
    }
    else if (key_pressed == sf::Keyboard::Key::Right)
    {
      set_selected(get_next(m_selected));
    }
    else if (key_pressed == sf::Keyboard::Key::Down)
    {
      set_selected(get_next(m_selected));
    }
    else if (key_pressed == sf::Keyboard::Key::Left)
    {
      set_selected(get_previous(m_selected));
    }
    else if (key_pressed == sf::Keyboard::Key::Space
      || key_pressed == sf::Keyboard::Key::Return
    )
    {
      if (m_selected == menu_view_item::start)
      {
        m_next_state = program_state::lobby;
      }
      else if (m_selected == menu_view_item::options)
      {
        m_next_state = program_state::options;
      }
      else if (m_selected == menu_view_item::about)
      {
        m_next_state = program_state::about;
      }
      else if (m_selected == menu_view_item::quit)
      {
        return true;
      }
    }
    else if (key_pressed == sf::Keyboard::Key::A)
    {
      m_next_state = program_state::about;
    }
    else if (key_pressed == sf::Keyboard::Key::O)
    {
      m_next_state = program_state::options;
    }
    else if (key_pressed == sf::Keyboard::Key::S)
    {
      m_next_state = program_state::lobby;
    }
    else if (key_pressed == sf::Keyboard::Key::Q)
    {
      return true;
    }
    else if (key_pressed == sf::Keyboard::Key::F1)
    {
      std::clog
        << "=============================================\n"
        << "= NOW                                       =\n"
        << "=============================================\n"
      ;
    }
  }
  if (event.type == sf::Event::MouseMoved)
  {
    const auto mouse_screen_pos{
      screen_coordinate(event.mouseMove.x, event.mouseMove.y)
    };
    if (is_in(mouse_screen_pos, m_layout.get_start())) set_selected(menu_view_item::start);
    else if (is_in(mouse_screen_pos, m_layout.get_options())) set_selected(menu_view_item::options);
    else if (is_in(mouse_screen_pos, m_layout.get_about())) set_selected(menu_view_item::about);
    else if (is_in(mouse_screen_pos, m_layout.get_quit())) set_selected(menu_view_item::quit);
  }
  else if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      const auto mouse_screen_pos{
        screen_coordinate(event.mouseButton.x, event.mouseButton.y)
      };
      if (is_in(mouse_screen_pos, m_layout.get_start()))
      {
        m_next_state = program_state::lobby;
      }
      else if (is_in(mouse_screen_pos, m_layout.get_options()))
      {
        m_next_state = program_state::options;
      }
      else if (is_in(mouse_screen_pos, m_layout.get_about()))
      {
        m_next_state = program_state::about;

      }
      else if (is_in(mouse_screen_pos, m_layout.get_quit()))
      {
        return true;
      }
    }
  }
  return false;
}

void menu_view::process_resize_event(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  m_layout = menu_view_layout(
    screen_coordinate(event.size.width, event.size.height),
    get_default_margin_width()
  );
}

void menu_view::set_selected(const menu_view_item i)
{
  if (m_selected != i)
  {
    game_resources::get().get_sound_effects().play_hide();
  }
  m_selected = i;
}

void menu_view::stop()
{
  m_next_state.reset();
}

void menu_view::tick()
{
  // Nothing to do yet
}

void menu_view::draw()
{
  draw_background_image(*this);

  draw_menu_outline(*this);

  draw_layout_panels(*this);

  draw_title_panel(*this);
  draw_subtitle_panel(*this);
  draw_start_panel(*this);
  draw_options_panel(*this);
  draw_about_panel(*this);
  draw_quit_panel(*this);
  draw_selected_panel(*this);
}

void draw_about_panel(menu_view& v)
{
  draw_about_button(v.get_layout().get_about());
}

void draw_background_image(menu_view& v)
{
  draw_texture(
    game_resources::get().get_textures().get_all_races(v.get_background_image_index()),
    v.get_layout().get_background_image()
  );
}


void draw_menu_outline(menu_view& v)
{
  draw_outline(v.get_layout().get_menu_panel());
}

void draw_options_panel(menu_view& v)
{
  draw_options_button(v.get_layout().get_options());
}

void draw_layout_panels(menu_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    get_render_window().draw(rectangle);
  }

}

void draw_quit_panel(menu_view& v)
{
  draw_quit_button(v.get_layout().get_quit());
}

void draw_selected_panel(menu_view& v)
{
  draw_outline(v.get_layout().get_selectable_rect(v.get_selected()));
}

void draw_start_panel(menu_view& v)
{
  draw_start_button(v.get_layout().get_start());
}

void draw_subtitle_panel(menu_view& v)
{
  draw_texture(
    get_subtitle_texture(),
    v.get_layout().get_subtitle()
  );
}

void draw_title_panel(menu_view& v)
{
  draw_texture(
    get_title_texture(),
    v.get_layout().get_title()
  );
}

void menu_view::start()
{
  get_render_window().setTitle("Conquer Chess: Main Menu");
  game_resources::get().get_songs().get_bliss().setVolume(
    get_music_volume_as_percentage()
  );
  game_resources::get().get_sound_effects().set_master_volume(
    game_options::get().get_sound_effects_volume()
  );
  game_resources::get().get_songs().get_bliss().play();
}

#endif // LOGIC_ONLY
