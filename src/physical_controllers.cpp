#include "physical_controllers.h"

#include <algorithm>
#include <cassert>

physical_controllers::physical_controllers(
  const std::vector<physical_controller> controllers
)
{
  assert(controllers.size() == 2);
  m_physical_controllers[side::lhs] = controllers[0];
  m_physical_controllers[side::rhs] = controllers[1];
}

int count_keyboard_controllers(const physical_controllers& controllers) noexcept
{
  const auto& cs{controllers.get_controllers()};
  return std::count_if(
    std::begin(cs),
    std::end(cs),
    [](const auto& p)
    {
      return p.second.get_type() == physical_controller_type::keyboard;
    }
  );
}

int count_mouse_controllers(const physical_controllers& controllers) noexcept
{
  const auto& cs{controllers.get_controllers()};
  return std::count_if(
    std::begin(cs),
    std::end(cs),
    [](const auto& p)
    {
      return p.second.get_type() == physical_controller_type::mouse;
    }
  );
}

int count_n_controllers(const physical_controllers& controllers) noexcept
{
  return controllers.get_controllers().size();
}

physical_controllers create_keyboard_mouse_controllers() noexcept
{
  return physical_controllers(
    { create_left_keyboard_controller(), create_default_mouse_controller() }
  );
}

physical_controllers create_mouse_keyboard_controllers() noexcept
{
  return physical_controllers(
    { create_default_mouse_controller(), create_right_keyboard_controller() }
  );
}

physical_controllers create_two_keyboard_controllers() noexcept
{
  return physical_controllers(
    { create_left_keyboard_controller(), create_right_keyboard_controller() }
  );
}

const physical_controller& physical_controllers::get_controller(const side player_side) const noexcept
{
  return m_physical_controllers.at(player_side);
}

physical_controller& physical_controllers::get_controller(const side player_side) noexcept
{
  return m_physical_controllers.at(player_side);
}

bool has_keyboard_controller(const physical_controllers& controllers) noexcept
{
  return count_keyboard_controllers(controllers);
}

bool has_mouse_controller(const physical_controllers& controllers) noexcept
{
  return count_mouse_controllers(controllers);
}

void physical_controllers::set(const side player_side, const physical_controller& controller)
{
  m_physical_controllers[player_side] = controller;
}

void test_physical_controllers()
{
#ifndef NDEBUG
  // count_keyboard_controllers
  {
    const physical_controllers pc_km{create_keyboard_mouse_controllers()};
    assert(count_keyboard_controllers(pc_km) == 1);
    const physical_controllers pc_kk{create_two_keyboard_controllers()};
    assert(count_keyboard_controllers(pc_kk) == 2);
  }
  // count_mouse_controllers
  {
    const physical_controllers pc_km{create_keyboard_mouse_controllers()};
    assert(count_mouse_controllers(pc_km) == 1);
    const physical_controllers pc_mk{create_mouse_keyboard_controllers()};
    assert(count_mouse_controllers(pc_mk) == 1);
    const physical_controllers pc_kk{create_two_keyboard_controllers()};
    assert(count_mouse_controllers(pc_kk) == 0);
  }
  // get_controller, const
  {
    const physical_controllers pc{create_keyboard_mouse_controllers()};
    const auto& lhs{pc.get_controller(side::lhs)};
    const auto& rhs{pc.get_controller(side::rhs)};
    assert(lhs != rhs);
  }
  // get_controller, non-const
  {
    physical_controllers pc{create_two_keyboard_controllers()};
    assert(count_mouse_controllers(pc) == 0);
    pc.get_controller(side::lhs).set_type(physical_controller_type::mouse);
    assert(count_mouse_controllers(pc) == 1);
  }
  // get_two_keyboard_controllers
  {
    const physical_controllers pc{create_two_keyboard_controllers()};
    assert(count_n_controllers(pc) == 2);
  }
  // has_keyboard_controllers
  {
    const physical_controllers pc_km{create_keyboard_mouse_controllers()};
    assert(has_keyboard_controller(pc_km));
    const physical_controllers pc_mk{create_mouse_keyboard_controllers()};
    assert(has_keyboard_controller(pc_mk) == 1);
    const physical_controllers pc_kk{create_two_keyboard_controllers()};
    assert(has_keyboard_controller(pc_kk));
  }
  // has_mouse_controller
  {
    const physical_controllers pc_km{create_keyboard_mouse_controllers()};
    assert(has_mouse_controller(pc_km));
    const physical_controllers pc_mk{create_mouse_keyboard_controllers()};
    assert(has_mouse_controller(pc_mk) == 1);
    const physical_controllers pc_kk{create_two_keyboard_controllers()};
    assert(!has_mouse_controller(pc_kk));
  }
#endif // NDEBUG
}
