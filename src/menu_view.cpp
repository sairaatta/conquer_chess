#include "menu_view.h"

#ifndef LOGIC_ONLY

#include "render_window.h"
#include "about_view.h"
#include "screen_coordinate.h"
#include "game_view.h"
#include "lobby_view.h"
#include "played_game_view.h"
#include "options_view.h"
#include "sfml_helper.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/ContextSettings.hpp>

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

void menu_view::exec_about()
{
  /*
  const auto cur_pos{m_window.getPosition()};
  m_window.setVisible(false);
  about_view view;
  view.exec();
  m_window.setVisible(true);
  m_window.setPosition(cur_pos);
  */
  m_next_state = program_state::about;
}

void menu_view::exec_game()
{
  /*
  const auto cur_pos{m_window.getPosition()};
  m_window.setVisible(false);
  game_view view{
    game(m_game_options),
    game_controller(m_physical_controllers)
  };
  view.exec();
  m_window.setVisible(true);
  m_window.setPosition(cur_pos);
  */
}

void menu_view::exec_lobby()
{
  /*
  m_resources.get_songs().get_bliss().stop();
  const auto cur_pos{m_window.getPosition()};
  m_window.setVisible(false);
  lobby_view view(
    m_game_options,
    m_physical_controllers
  );
  view.exec();
  m_window.setVisible(true);
  m_window.setPosition(cur_pos);
  m_resources.get_songs().get_bliss().play();
  */
}

void menu_view::exec_options()
{
  /*
  const auto cur_pos{m_window.getPosition()};
  m_window.setVisible(false);
  options_view view(m_game_options);
  assert(!to_str(get_starting_position(view)).empty());
  view.exec();
  assert(!to_str(get_starting_position(view)).empty());
  m_game_options = view.get_options();
  m_window.setVisible(true);
  m_window.setPosition(cur_pos);
  */
  m_next_state = program_state::options;
}

void menu_view::exec_played_game()
{
  /*
  // Approx 1 in 500 is a valid move
  played_game_view view(create_randomly_played_game(2000));
  view.exec();
  */
}

void menu_view::exec_replay()
{
  m_next_state = program_state::replay;

  /*
  const auto cur_pos{m_window.getPosition()};
  m_window.setVisible(false);
  game_options options{m_game_options};
  options.set_game_speed(game_speed::slowest);
  options.set_replayer(
    replayer(
      replay(
        get_scholars_mate_as_pgn_str()
      )
    )
  );
  game_view view{game(options)};
  view.exec();
  m_window.setVisible(true);
  m_window.setPosition(cur_pos);
  */
}

void menu_view::exec_start()
{
  m_next_state = program_state::game;
}

sf::Text get_menu_screen_text() noexcept
{
  sf::Text text;

  return text;
}

bool menu_view::process_event(sf::Event& event)
{
  if (event.type == sf::Event::Closed)
  {
    get_render_window().close();
    return true; // Close the program
  }
  if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Escape)
    {
      get_render_window().close();
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
      if (m_selected == menu_view_item::start) exec_start();
      else if (m_selected == menu_view_item::options) exec_options();
      else if (m_selected == menu_view_item::about) exec_about();
      else if (m_selected == menu_view_item::quit)
      {
        return true;
      }
    }
    else if (key_pressed == sf::Keyboard::Key::A)
    {
      exec_about();
    }
    else if (key_pressed == sf::Keyboard::Key::O)
    {
      exec_options();
    }
    else if (key_pressed == sf::Keyboard::Key::S)
    {
      exec_start();
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
      exec_replay();
    }
    else if (key_pressed == sf::Keyboard::Key::F2)
    {
      exec_played_game();
    }
    else if (key_pressed == sf::Keyboard::Key::F4)
    {
      exec_game();
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
      if (is_in(mouse_screen_pos, m_layout.get_start())) exec_start();
      else if (is_in(mouse_screen_pos, m_layout.get_options())) exec_options();
      else if (is_in(mouse_screen_pos, m_layout.get_about())) exec_about();
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

sf::Text menu_view::get_styled_text()
{
  sf::Text text;
  text.setFont(get_arial_font());
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(m_layout.get_font_size());
  text.setFillColor(sf::Color::Black);
  return text;
}

void menu_view::draw()
{
  draw_background_image(*this);

  draw_menu_panel(*this);

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
  const auto screen_rect{v.get_layout().get_about()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_strip(chess_color::white)
  );
  get_render_window().draw(rectangle);

  sf::Text text{v.get_styled_text()};
  text.setString("About");
  set_text_position(text, screen_rect);
  get_render_window().draw(text);
}

void draw_background_image(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_background_image()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &game_resources::get().get_textures().get_all_races(v.get_background_image_index())
  );
  get_render_window().draw(rectangle);
}


void draw_menu_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_menu_panel()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setOutlineThickness(5);
  get_render_window().draw(rectangle);
}

void draw_options_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_options()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_strip(chess_color::black)
  );
  get_render_window().draw(rectangle);

  sf::Text text{v.get_styled_text()};
  text.setString("Options");
  set_text_position(text, screen_rect);
  get_render_window().draw(text);
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
  const auto screen_rect{v.get_layout().get_quit()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_strip(chess_color::black)
  );
  get_render_window().draw(rectangle);

  sf::Text text{v.get_styled_text()};
  text.setString("Quit");
  text.setFillColor(sf::Color::Black);
  set_text_position(text, screen_rect);
  get_render_window().draw(text);
}

void draw_selected_panel(menu_view& v)
{

  const auto select_rect{v.get_layout().get_selectable_rect(v.get_selected())};
  sf::RectangleShape rectangle;
  set_rect(rectangle, select_rect);
  rectangle.setOrigin(
    get_width(select_rect) / 2,
    get_height(select_rect) / 2
  );
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setOutlineThickness(5);
  get_render_window().draw(rectangle);
}

void draw_start_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_start()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_strip(chess_color::white)
  );
  get_render_window().draw(rectangle);

  sf::Text text{v.get_styled_text()};
  text.setString("Start");
  set_text_position(text, screen_rect);
  get_render_window().draw(text);
}

void draw_subtitle_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_subtitle()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_subtitle()
  );
  get_render_window().draw(rectangle);
}

void draw_title_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_title()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_title()
  );
  get_render_window().draw(rectangle);
}

void menu_view::start()
{
  get_render_window().setTitle("Conquer Chess: Main Menu");
  game_resources::get().get_songs().get_bliss().setVolume(
    get_music_volume_as_percentage(game_options::get())
  );
  game_resources::get().get_sound_effects().set_master_volume(
    game_options::get().get_sound_effects_volume()
  );
  game_resources::get().get_songs().get_bliss().setLoop(true);
  game_resources::get().get_songs().get_bliss().play();
}

#endif // LOGIC_ONLY
