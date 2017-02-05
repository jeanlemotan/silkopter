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
}
}
