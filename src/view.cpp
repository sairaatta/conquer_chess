#include "view.h"

#include "helper.h"

#include <cassert>
#include <iostream>

view::view()
{

}

view::~view()
{


}

void view::clear_next_state()
{
  // A window can be closed programmatically too,
  // hence do not do:
  //
  // assert(m_next_state.has_value());

  m_next_state.reset();
  assert(!m_next_state.has_value());
}

void view::set_is_active(const bool is_active)
{
  #ifndef NDEBUG
  if (m_is_active == is_active)
  {
    std::clog << "Class " << typeid(*this).name()
      << " attempts to again set m_is_active to "
      << bool_to_str(is_active) << '\n';
  }
  #endif // NDEBUG
  assert(m_is_active != is_active);
  m_is_active = is_active;
}

void view::set_next_state(const program_state next_state)
{
  #ifndef NDEBUG
  if (m_next_state.has_value())
  {
    std::clog
      << "Class " << typeid(*this).name() << " attempts to overwrite its next state. "
      << "Current next_state: " << to_str(m_next_state.value()) << '\n'
      << "Next next_state: " << to_str(next_state) << '\n'
    ;
  }
  #endif // NDEBUG
  assert(!m_next_state.has_value());
  m_next_state = next_state;
}
