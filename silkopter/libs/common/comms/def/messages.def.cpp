#include "messages.def.h"
namespace silk
{

namespace messages
{

template <typename T>
T clamp(T v, T min, T max)
{
  return std::min(std::max(v, min), max);
}
template <typename T>
T min(T v, T min)
{
  return std::min(v, min);
}
template <typename T>
T max(T v, T max)
{
  return std::max(v, max);
}
  void Multirotor_Simulator_Set_Gravity_Enabled_Message::set_enabled(bool const& value)
  {
    m_enabled = value;
  }
  void Multirotor_Simulator_Set_Gravity_Enabled_Message::set_enabled(bool&& value)
  {
    m_enabled = std::move(value);
  }
  auto Multirotor_Simulator_Set_Gravity_Enabled_Message::get_enabled() const -> bool const& 
  {
    return m_enabled;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulator_Set_Ground_Enabled_Message::set_enabled(bool const& value)
  {
    m_enabled = value;
  }
  void Multirotor_Simulator_Set_Ground_Enabled_Message::set_enabled(bool&& value)
  {
    m_enabled = std::move(value);
  }
  auto Multirotor_Simulator_Set_Ground_Enabled_Message::get_enabled() const -> bool const& 
  {
    return m_enabled;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulator_Set_Simulation_Enabled_Message::set_enabled(bool const& value)
  {
    m_enabled = value;
  }
  void Multirotor_Simulator_Set_Simulation_Enabled_Message::set_enabled(bool&& value)
  {
    m_enabled = std::move(value);
  }
  auto Multirotor_Simulator_Set_Simulation_Enabled_Message::get_enabled() const -> bool const& 
  {
    return m_enabled;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulator_Set_Drag_Enabled_Message::set_enabled(bool const& value)
  {
    m_enabled = value;
  }
  void Multirotor_Simulator_Set_Drag_Enabled_Message::set_enabled(bool&& value)
  {
    m_enabled = std::move(value);
  }
  auto Multirotor_Simulator_Set_Drag_Enabled_Message::get_enabled() const -> bool const& 
  {
    return m_enabled;
  }

////////////////////////////////////////////////////////////
    void Multirotor_Simulation_State::Motor_State::set_drag_factor(float const& value)
    {
      m_drag_factor = value;
    }
    void Multirotor_Simulation_State::Motor_State::set_drag_factor(float&& value)
    {
      m_drag_factor = std::move(value);
    }
    auto Multirotor_Simulation_State::Motor_State::get_drag_factor() const -> float const& 
    {
      return m_drag_factor;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulation_State::Motor_State::set_throttle(float const& value)
    {
      m_throttle = value;
    }
    void Multirotor_Simulation_State::Motor_State::set_throttle(float&& value)
    {
      m_throttle = std::move(value);
    }
    auto Multirotor_Simulation_State::Motor_State::get_throttle() const -> float const& 
    {
      return m_throttle;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulation_State::Motor_State::set_thrust(float const& value)
    {
      m_thrust = value;
    }
    void Multirotor_Simulation_State::Motor_State::set_thrust(float&& value)
    {
      m_thrust = std::move(value);
    }
    auto Multirotor_Simulation_State::Motor_State::get_thrust() const -> float const& 
    {
      return m_thrust;
    }

////////////////////////////////////////////////////////////
  void Multirotor_Simulation_State::set_enu_position(vec3f const& value)
  {
    m_enu_position = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Multirotor_Simulation_State::set_enu_position(vec3f&& value)
  {
    m_enu_position = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Multirotor_Simulation_State::get_enu_position() const -> vec3f const& 
  {
    return m_enu_position;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulation_State::set_local_to_enu_rotation(quatf const& value)
  {
    m_local_to_enu_rotation = clamp(value, quatf(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX), quatf(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Multirotor_Simulation_State::set_local_to_enu_rotation(quatf&& value)
  {
    m_local_to_enu_rotation = clamp(std::move(value), quatf(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX), quatf(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Multirotor_Simulation_State::get_local_to_enu_rotation() const -> quatf const& 
  {
    return m_local_to_enu_rotation;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulation_State::set_enu_velocity(vec3f const& value)
  {
    m_enu_velocity = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Multirotor_Simulation_State::set_enu_velocity(vec3f&& value)
  {
    m_enu_velocity = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Multirotor_Simulation_State::get_enu_velocity() const -> vec3f const& 
  {
    return m_enu_velocity;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulation_State::set_enu_linear_acceleration(vec3f const& value)
  {
    m_enu_linear_acceleration = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Multirotor_Simulation_State::set_enu_linear_acceleration(vec3f&& value)
  {
    m_enu_linear_acceleration = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Multirotor_Simulation_State::get_enu_linear_acceleration() const -> vec3f const& 
  {
    return m_enu_linear_acceleration;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulation_State::set_acceleration(vec3f const& value)
  {
    m_acceleration = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Multirotor_Simulation_State::set_acceleration(vec3f&& value)
  {
    m_acceleration = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Multirotor_Simulation_State::get_acceleration() const -> vec3f const& 
  {
    return m_acceleration;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulation_State::set_angular_velocity(vec3f const& value)
  {
    m_angular_velocity = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Multirotor_Simulation_State::set_angular_velocity(vec3f&& value)
  {
    m_angular_velocity = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Multirotor_Simulation_State::get_angular_velocity() const -> vec3f const& 
  {
    return m_angular_velocity;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulation_State::set_magnetic_field(vec3f const& value)
  {
    m_magnetic_field = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Multirotor_Simulation_State::set_magnetic_field(vec3f&& value)
  {
    m_magnetic_field = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Multirotor_Simulation_State::get_magnetic_field() const -> vec3f const& 
  {
    return m_magnetic_field;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulation_State::set_pressure(float const& value)
  {
    m_pressure = value;
  }
  void Multirotor_Simulation_State::set_pressure(float&& value)
  {
    m_pressure = std::move(value);
  }
  auto Multirotor_Simulation_State::get_pressure() const -> float const& 
  {
    return m_pressure;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulation_State::set_temperature(float const& value)
  {
    m_temperature = value;
  }
  void Multirotor_Simulation_State::set_temperature(float&& value)
  {
    m_temperature = std::move(value);
  }
  auto Multirotor_Simulation_State::get_temperature() const -> float const& 
  {
    return m_temperature;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulation_State::set_proximity_distance(vec3f const& value)
  {
    m_proximity_distance = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Multirotor_Simulation_State::set_proximity_distance(vec3f&& value)
  {
    m_proximity_distance = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Multirotor_Simulation_State::get_proximity_distance() const -> vec3f const& 
  {
    return m_proximity_distance;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulation_State::set_motors(std::vector<Multirotor_Simulation_State::Motor_State> const& value)
  {
    m_motors = value;
  }
  void Multirotor_Simulation_State::set_motors(std::vector<Multirotor_Simulation_State::Motor_State>&& value)
  {
    m_motors = std::move(value);
  }
  auto Multirotor_Simulation_State::get_motors() const -> std::vector<Multirotor_Simulation_State::Motor_State> const& 
  {
    return m_motors;
  }

  auto Multirotor_Simulation_State::get_motors() -> std::vector<Multirotor_Simulation_State::Motor_State>& 
  {
    return m_motors;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulator_State_Message::set_state(Multirotor_Simulation_State const& value)
  {
    m_state = value;
  }
  void Multirotor_Simulator_State_Message::set_state(Multirotor_Simulation_State&& value)
  {
    m_state = std::move(value);
  }
  auto Multirotor_Simulator_State_Message::get_state() const -> Multirotor_Simulation_State const& 
  {
    return m_state;
  }

  auto Multirotor_Simulator_State_Message::get_state() -> Multirotor_Simulation_State& 
  {
    return m_state;
  }

////////////////////////////////////////////////////////////
ts::Result<void> deserialize(std::string& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  value = sz_value.get_as_string();
  return ts::success;
}
ts::sz::Value serialize(std::string const& value)
{
  return ts::sz::Value(value);
}
ts::Result<void> deserialize(bool& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_bool()) { return ts::Error("Expected bool value when deserializing"); }
  value = sz_value.get_as_bool();
  return ts::success;
}
ts::sz::Value serialize(bool const& value)
{
  return ts::sz::Value(value);
}
ts::Result<void> deserialize(int64_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::sz::Value serialize(int64_t const& value)
{
  return ts::sz::Value(value);
}
ts::Result<void> deserialize(float& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_real_number()) { return ts::Error("Expected real number value when deserializing"); }
  value = (float)sz_value.get_as_real_number();
  return ts::success;
}
ts::sz::Value serialize(float const& value)
{
  return ts::sz::Value(value);
}
ts::Result<void> deserialize(double& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_real_number()) { return ts::Error("Expected real number value when deserializing"); }
  value = sz_value.get_as_real_number();
  return ts::success;
}
ts::sz::Value serialize(double const& value)
{
  return ts::sz::Value(value);
}
ts::Result<void> deserialize(vec2f& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("x");
    if (!sz_v) { return ts::Error("Cannot find component 'x'"); }
    auto result = deserialize(value.x, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("y");
    if (!sz_v) { return ts::Error("Cannot find component 'y'"); }
    auto result = deserialize(value.y, *sz_v);
    if (result != ts::success) { return result; }
  }
  return ts::success;
}
ts::sz::Value serialize(vec2f const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("x", serialize(value.x));
  sz_value.add_object_member("y", serialize(value.y));
  return sz_value;
}
ts::Result<void> deserialize(vec2d& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("x");
    if (!sz_v) { return ts::Error("Cannot find component 'x'"); }
    auto result = deserialize(value.x, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("y");
    if (!sz_v) { return ts::Error("Cannot find component 'y'"); }
    auto result = deserialize(value.y, *sz_v);
    if (result != ts::success) { return result; }
  }
  return ts::success;
}
ts::sz::Value serialize(vec2d const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("x", serialize(value.x));
  sz_value.add_object_member("y", serialize(value.y));
  return sz_value;
}
ts::Result<void> deserialize(vec2i& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("x");
    if (!sz_v) { return ts::Error("Cannot find component 'x'"); }
    auto result = deserialize(value.x, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("y");
    if (!sz_v) { return ts::Error("Cannot find component 'y'"); }
    auto result = deserialize(value.y, *sz_v);
    if (result != ts::success) { return result; }
  }
  return ts::success;
}
ts::sz::Value serialize(vec2i const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("x", serialize(value.x));
  sz_value.add_object_member("y", serialize(value.y));
  return sz_value;
}
ts::Result<void> deserialize(vec3f& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("x");
    if (!sz_v) { return ts::Error("Cannot find component 'x'"); }
    auto result = deserialize(value.x, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("y");
    if (!sz_v) { return ts::Error("Cannot find component 'y'"); }
    auto result = deserialize(value.y, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("z");
    if (!sz_v) { return ts::Error("Cannot find component 'z'"); }
    auto result = deserialize(value.z, *sz_v);
    if (result != ts::success) { return result; }
  }
  return ts::success;
}
ts::sz::Value serialize(vec3f const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("x", serialize(value.x));
  sz_value.add_object_member("y", serialize(value.y));
  sz_value.add_object_member("z", serialize(value.z));
  return sz_value;
}
ts::Result<void> deserialize(vec3d& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("x");
    if (!sz_v) { return ts::Error("Cannot find component 'x'"); }
    auto result = deserialize(value.x, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("y");
    if (!sz_v) { return ts::Error("Cannot find component 'y'"); }
    auto result = deserialize(value.y, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("z");
    if (!sz_v) { return ts::Error("Cannot find component 'z'"); }
    auto result = deserialize(value.z, *sz_v);
    if (result != ts::success) { return result; }
  }
  return ts::success;
}
ts::sz::Value serialize(vec3d const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("x", serialize(value.x));
  sz_value.add_object_member("y", serialize(value.y));
  sz_value.add_object_member("z", serialize(value.z));
  return sz_value;
}
ts::Result<void> deserialize(vec3i& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("x");
    if (!sz_v) { return ts::Error("Cannot find component 'x'"); }
    auto result = deserialize(value.x, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("y");
    if (!sz_v) { return ts::Error("Cannot find component 'y'"); }
    auto result = deserialize(value.y, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("z");
    if (!sz_v) { return ts::Error("Cannot find component 'z'"); }
    auto result = deserialize(value.z, *sz_v);
    if (result != ts::success) { return result; }
  }
  return ts::success;
}
ts::sz::Value serialize(vec3i const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("x", serialize(value.x));
  sz_value.add_object_member("y", serialize(value.y));
  sz_value.add_object_member("z", serialize(value.z));
  return sz_value;
}
ts::Result<void> deserialize(vec4f& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("x");
    if (!sz_v) { return ts::Error("Cannot find component 'x'"); }
    auto result = deserialize(value.x, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("y");
    if (!sz_v) { return ts::Error("Cannot find component 'y'"); }
    auto result = deserialize(value.y, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("z");
    if (!sz_v) { return ts::Error("Cannot find component 'z'"); }
    auto result = deserialize(value.z, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("w");
    if (!sz_v) { return ts::Error("Cannot find component 'w'"); }
    auto result = deserialize(value.w, *sz_v);
    if (result != ts::success) { return result; }
  }
  return ts::success;
}
ts::sz::Value serialize(vec4f const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("x", serialize(value.x));
  sz_value.add_object_member("y", serialize(value.y));
  sz_value.add_object_member("z", serialize(value.z));
  sz_value.add_object_member("w", serialize(value.w));
  return sz_value;
}
ts::Result<void> deserialize(vec4d& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("x");
    if (!sz_v) { return ts::Error("Cannot find component 'x'"); }
    auto result = deserialize(value.x, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("y");
    if (!sz_v) { return ts::Error("Cannot find component 'y'"); }
    auto result = deserialize(value.y, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("z");
    if (!sz_v) { return ts::Error("Cannot find component 'z'"); }
    auto result = deserialize(value.z, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("w");
    if (!sz_v) { return ts::Error("Cannot find component 'w'"); }
    auto result = deserialize(value.w, *sz_v);
    if (result != ts::success) { return result; }
  }
  return ts::success;
}
ts::sz::Value serialize(vec4d const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("x", serialize(value.x));
  sz_value.add_object_member("y", serialize(value.y));
  sz_value.add_object_member("z", serialize(value.z));
  sz_value.add_object_member("w", serialize(value.w));
  return sz_value;
}
ts::Result<void> deserialize(vec4i& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("x");
    if (!sz_v) { return ts::Error("Cannot find component 'x'"); }
    auto result = deserialize(value.x, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("y");
    if (!sz_v) { return ts::Error("Cannot find component 'y'"); }
    auto result = deserialize(value.y, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("z");
    if (!sz_v) { return ts::Error("Cannot find component 'z'"); }
    auto result = deserialize(value.z, *sz_v);
    if (result != ts::success) { return result; }
  }
  {
    auto const* sz_v = sz_value.find_object_member_by_name("w");
    if (!sz_v) { return ts::Error("Cannot find component 'w'"); }
    auto result = deserialize(value.w, *sz_v);
    if (result != ts::success) { return result; }
  }
  return ts::success;
}
ts::sz::Value serialize(vec4i const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("x", serialize(value.x));
  sz_value.add_object_member("y", serialize(value.y));
  sz_value.add_object_member("z", serialize(value.z));
  sz_value.add_object_member("w", serialize(value.w));
  return sz_value;
}
ts::Result<void> deserialize(int8_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::sz::Value serialize(int8_t const& value)
{
  return ts::sz::Value(value);
}
ts::Result<void> deserialize(uint8_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::sz::Value serialize(uint8_t const& value)
{
  return ts::sz::Value(value);
}
ts::Result<void> deserialize(int16_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::sz::Value serialize(int16_t const& value)
{
  return ts::sz::Value(value);
}
ts::Result<void> deserialize(uint16_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::sz::Value serialize(uint16_t const& value)
{
  return ts::sz::Value(value);
}
ts::Result<void> deserialize(int32_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::sz::Value serialize(int32_t const& value)
{
  return ts::sz::Value(value);
}
ts::Result<void> deserialize(uint32_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::sz::Value serialize(uint32_t const& value)
{
  return ts::sz::Value(value);
}
ts::Result<void> deserialize(INode_Message& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(INode_Message const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Poly<INode_Message>& value, ts::sz::Value const& sz_value)
{
  if (sz_value.is_empty()) { value = Poly<INode_Message>(); return ts::success; }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::Multirotor_Simulator_Reset_Message")
  {
    value = Poly<INode_Message>(new Multirotor_Simulator_Reset_Message());
    return deserialize((Multirotor_Simulator_Reset_Message&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Simulator_Stop_Motion_Message")
  {
    value = Poly<INode_Message>(new Multirotor_Simulator_Stop_Motion_Message());
    return deserialize((Multirotor_Simulator_Stop_Motion_Message&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Simulator_Set_Gravity_Enabled_Message")
  {
    value = Poly<INode_Message>(new Multirotor_Simulator_Set_Gravity_Enabled_Message());
    return deserialize((Multirotor_Simulator_Set_Gravity_Enabled_Message&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Simulator_Set_Ground_Enabled_Message")
  {
    value = Poly<INode_Message>(new Multirotor_Simulator_Set_Ground_Enabled_Message());
    return deserialize((Multirotor_Simulator_Set_Ground_Enabled_Message&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Simulator_Set_Simulation_Enabled_Message")
  {
    value = Poly<INode_Message>(new Multirotor_Simulator_Set_Simulation_Enabled_Message());
    return deserialize((Multirotor_Simulator_Set_Simulation_Enabled_Message&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Simulator_Set_Drag_Enabled_Message")
  {
    value = Poly<INode_Message>(new Multirotor_Simulator_Set_Drag_Enabled_Message());
    return deserialize((Multirotor_Simulator_Set_Drag_Enabled_Message&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Simulator_Get_State_Message")
  {
    value = Poly<INode_Message>(new Multirotor_Simulator_Get_State_Message());
    return deserialize((Multirotor_Simulator_Get_State_Message&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Simulator_State_Message")
  {
    value = Poly<INode_Message>(new Multirotor_Simulator_State_Message());
    return deserialize((Multirotor_Simulator_State_Message&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Poly<INode_Message> const& value)
{
  if (!value) { return ts::sz::Value(); }
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(Multirotor_Simulator_Reset_Message))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Simulator_Reset_Message");
    sz_value.add_object_member("value", serialize((Multirotor_Simulator_Reset_Message&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Simulator_Stop_Motion_Message))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Simulator_Stop_Motion_Message");
    sz_value.add_object_member("value", serialize((Multirotor_Simulator_Stop_Motion_Message&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Simulator_Set_Gravity_Enabled_Message))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Simulator_Set_Gravity_Enabled_Message");
    sz_value.add_object_member("value", serialize((Multirotor_Simulator_Set_Gravity_Enabled_Message&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Simulator_Set_Ground_Enabled_Message))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Simulator_Set_Ground_Enabled_Message");
    sz_value.add_object_member("value", serialize((Multirotor_Simulator_Set_Ground_Enabled_Message&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Simulator_Set_Simulation_Enabled_Message))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Simulator_Set_Simulation_Enabled_Message");
    sz_value.add_object_member("value", serialize((Multirotor_Simulator_Set_Simulation_Enabled_Message&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Simulator_Set_Drag_Enabled_Message))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Simulator_Set_Drag_Enabled_Message");
    sz_value.add_object_member("value", serialize((Multirotor_Simulator_Set_Drag_Enabled_Message&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Simulator_Get_State_Message))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Simulator_Get_State_Message");
    sz_value.add_object_member("value", serialize((Multirotor_Simulator_Get_State_Message&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Simulator_State_Message))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Simulator_State_Message");
    sz_value.add_object_member("value", serialize((Multirotor_Simulator_State_Message&)*value));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(Multirotor_Simulator_Reset_Message& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulator_Reset_Message const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulator_Stop_Motion_Message& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulator_Stop_Motion_Message const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulator_Set_Gravity_Enabled_Message& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("enabled");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'enabled'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_enabled())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_enabled(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulator_Set_Gravity_Enabled_Message const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("enabled", serialize(value.get_enabled()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulator_Set_Ground_Enabled_Message& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("enabled");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'enabled'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_enabled())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_enabled(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulator_Set_Ground_Enabled_Message const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("enabled", serialize(value.get_enabled()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulator_Set_Simulation_Enabled_Message& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("enabled");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'enabled'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_enabled())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_enabled(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulator_Set_Simulation_Enabled_Message const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("enabled", serialize(value.get_enabled()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulator_Set_Drag_Enabled_Message& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("enabled");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'enabled'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_enabled())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_enabled(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulator_Set_Drag_Enabled_Message const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("enabled", serialize(value.get_enabled()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulator_Get_State_Message& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulator_Get_State_Message const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulation_State::Motor_State& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("drag_factor");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'drag_factor'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_drag_factor())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_drag_factor(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("throttle");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'throttle'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_throttle())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_throttle(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("thrust");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'thrust'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_thrust())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_thrust(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulation_State::Motor_State const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("drag_factor", serialize(value.get_drag_factor()));
  sz_value.add_object_member("throttle", serialize(value.get_throttle()));
  sz_value.add_object_member("thrust", serialize(value.get_thrust()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulation_State& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("enu_position");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'enu_position'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_enu_position())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_enu_position(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("local_to_enu_rotation");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'local_to_enu_rotation'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_local_to_enu_rotation())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_local_to_enu_rotation(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("enu_velocity");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'enu_velocity'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_enu_velocity())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_enu_velocity(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("enu_linear_acceleration");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'enu_linear_acceleration'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_enu_linear_acceleration())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_enu_linear_acceleration(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("acceleration");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'acceleration'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_acceleration())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_acceleration(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("angular_velocity");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'angular_velocity'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_angular_velocity())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_angular_velocity(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("magnetic_field");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'magnetic_field'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_magnetic_field())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_magnetic_field(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("pressure");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'pressure'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_pressure())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_pressure(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("temperature");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'temperature'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_temperature())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_temperature(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("proximity_distance");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'proximity_distance'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_proximity_distance())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_proximity_distance(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("motors");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'motors'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_motors())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_motors(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulation_State const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(11);
  sz_value.add_object_member("enu_position", serialize(value.get_enu_position()));
  sz_value.add_object_member("local_to_enu_rotation", serialize(value.get_local_to_enu_rotation()));
  sz_value.add_object_member("enu_velocity", serialize(value.get_enu_velocity()));
  sz_value.add_object_member("enu_linear_acceleration", serialize(value.get_enu_linear_acceleration()));
  sz_value.add_object_member("acceleration", serialize(value.get_acceleration()));
  sz_value.add_object_member("angular_velocity", serialize(value.get_angular_velocity()));
  sz_value.add_object_member("magnetic_field", serialize(value.get_magnetic_field()));
  sz_value.add_object_member("pressure", serialize(value.get_pressure()));
  sz_value.add_object_member("temperature", serialize(value.get_temperature()));
  sz_value.add_object_member("proximity_distance", serialize(value.get_proximity_distance()));
  sz_value.add_object_member("motors", serialize(value.get_motors()));
  return sz_value;
}
ts::Result<void> deserialize(std::vector<Multirotor_Simulation_State::Motor_State>& value, ts::sz::Value const& sz_value)
{
  value.clear();
  if (!sz_value.is_array()) { return ts::Error("Expected array value when deserializing"); }
  value.resize(sz_value.get_array_element_count());
  for (size_t i = 0; i < value.size(); i++)
  {
    auto result = deserialize(value[i], sz_value.get_array_element_value(i));
    if (result != ts::success) { return result; }
  }
  return ts::success;
}
ts::sz::Value serialize(std::vector<Multirotor_Simulation_State::Motor_State> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulator_State_Message& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("state");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'state'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_state())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_state(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulator_State_Message const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("state", serialize(value.get_state()));
  return sz_value;
}
}
}
