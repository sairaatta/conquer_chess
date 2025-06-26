#include "physical_controllers.h"

#include <algorithm>
#include <cassert>

physical_controllers::physical_controllers(
  const std::vector<physical_controller> controllers
) : m_physical_controllers{controllers}
{

}

int count_keyboard_controllers(const physical_controllers& controllers) noexcept
{
  const auto& cs{controllers.get_controllers()};
  return std::count_if(
    std::begin(cs),
    std::end(cs),
    [](const auto& c)
    {
      return c.get_type() == physical_controller_type::keyboard;
    }
  );
}

int count_mouse_controllers(const physical_controllers& controllers) noexcept
{
  const auto& cs{controllers.get_controllers()};
  return std::count_if(
    std::begin(cs),
    std::end(cs),
    [](const auto& c)
    {
      return c.get_type() == physical_controller_type::mouse;
    }
  );
}

int count_n_controllers(const physical_controllers& controllers) noexcept
{
  return controllers.get_controllers().size();
}

/*
void use_default_controllers() noexcept
{
  use_two_keyboard_controllers();
}
*/

void use_keyboard_mouse_controllers() noexcept
{
  physical_controllers::get().set(
    side::lhs,
    create_left_keyboard_controller()
  );
  physical_controllers::get().set(
    side::rhs,
    create_default_mouse_controller()
  );
}

void use_mouse_keyboard_controllers() noexcept
{
  physical_controllers::get().set(
    side::lhs,
    create_default_mouse_controller()
  );
  physical_controllers::get().set(
    side::rhs,
    create_right_keyboard_controller()
  );
}

void use_two_keyboard_controllers() noexcept
{
  physical_controllers::get().set(
    side::lhs,
    create_left_keyboard_controller()
  );
  physical_controllers::get().set(
    side::rhs,
    create_right_keyboard_controller()
  );
}

const physical_controller& physical_controllers::get_controller(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    assert(count_n_controllers(m_physical_controllers) >= 1);
    return m_physical_controllers[0];
  }
  assert(player_side == side::rhs);
  assert(count_n_controllers(m_physical_controllers) >= 2);
  return m_physical_controllers[1];
}

physical_controller& physical_controllers::get_controller(const side player_side) noexcept
{
  if (player_side == side::lhs)
  {
    assert(count_n_controllers(m_physical_controllers) >= 1);
    return m_physical_controllers[0];
  }
  assert(player_side == side::rhs);
  assert(count_n_controllers(m_physical_controllers) >= 2);
  return m_physical_controllers[1];
}

/*
side get_keyboard_user_player_side(const physical_controllers& controllers)
{
  const auto& cs{controllers.get_controllers()};
  if (cs[0].get_type() == physical_controller_type::keyboard)
  {
    return side::lhs;
  }
  assert(cs[1].get_type() == physical_controller_type::keyboard);
  return side::rhs;
}

side get_mouse_user_player_side(const physical_controllers& controllers)
{
  const auto& cs{controllers.get_controllers()};
  if (cs[0].get_type() == physical_controller_type::mouse)
  {
    return side::lhs;
  }
  assert(cs[1].get_type() == physical_controller_type::mouse);
  return side::rhs;
}
*/

bool has_keyboard_controller(const physical_controllers& controllers) noexcept
{
  return count_keyboard_controllers(controllers);
}

bool has_mouse_controller(const physical_controllers& controllers) noexcept
{
  return count_mouse_controllers(controllers);
}

const std::vector<physical_controller>& physical_controllers::get_controllers() const noexcept
{
  return m_physical_controllers;
}

void physical_controllers::set(const side player_side, const physical_controller& controller)
{
  if (player_side == side::lhs)
  {
    assert(m_physical_controllers.size() >= 1);
    m_physical_controllers[0] = controller;
    return;
  }
  assert(player_side == side::rhs);
  assert(m_physical_controllers.size() >= 2);
  m_physical_controllers[1] = controller;
}

void test_physical_controllers()
{
#ifndef NDEBUG
  {
    physical_controllers::get();
  }
  // count_keyboard_controllers
  {
    use_keyboard_mouse_controllers();
    assert(count_keyboard_controllers(physical_controllers::get()) == 1);
    use_two_keyboard_controllers();
    assert(count_keyboard_controllers(physical_controllers::get()) == 2);
  }
  // count_mouse_controllers
  {
    use_keyboard_mouse_controllers();
    assert(count_mouse_controllers(physical_controllers::get()) == 1);
    use_two_keyboard_controllers();
    assert(count_mouse_controllers(physical_controllers::get()) == 0);
  }
  // physical_controller::get_controller, const
  {
    use_keyboard_mouse_controllers();
    const auto& p{physical_controllers::get()};
    const auto& lhs{p.get_controller(side::lhs)};
    const auto& rhs{p.get_controller(side::rhs)};
    assert(lhs != rhs);
  }
  // physical_controller::get_controller, non-const
  {
    use_keyboard_mouse_controllers();
    auto& p{physical_controllers::get()};
    const auto& lhs{p.get_controller(side::lhs)};
    const auto& rhs{p.get_controller(side::rhs)};
    assert(lhs != rhs);
  }
  /*
  // get_keyboard_user_player_side
  {
    use_keyboard_mouse_controllers();
    assert(get_keyboard_user_player_side(physical_controllers::get()) == side::lhs);
  }
  // get_mouse_user_player_side
  {
    use_keyboard_mouse_controllers();
    assert(get_mouse_user_player_side(physical_controllers::get()) == side::rhs);
    use_mouse_keyboard_controllers();
    assert(get_mouse_user_player_side(physical_controllers::get()) == side::lhs);
  }
  */
  // get_two_keyboard_controllers
  {
    use_two_keyboard_controllers();
    assert(count_n_controllers(physical_controllers::get()) == 2);
  }
  // has_keyboard_controllers
  {
    use_keyboard_mouse_controllers();
    assert(has_keyboard_controller(physical_controllers::get()));
    use_two_keyboard_controllers();
    assert(has_keyboard_controller(physical_controllers::get()));
  }
  // has_mouse_controller
  {
    use_keyboard_mouse_controllers();
    assert(has_mouse_controller(physical_controllers::get()));
    use_two_keyboard_controllers();
    assert(!has_mouse_controller(physical_controllers::get()));
  }
#endif // NDEBUG
}
