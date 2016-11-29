#include "settings.def.h"
namespace silk
{

namespace settings
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
      void Settings::Input::Sticks_Calibration::set_yaw_center(float const& value)
      {
        m_yaw_center = value;
      }
      void Settings::Input::Sticks_Calibration::set_yaw_center(float&& value)
      {
        m_yaw_center = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_yaw_center() const -> float const& 
      {
        return m_yaw_center;
      }

////////////////////////////////////////////////////////////
      void Settings::Input::Sticks_Calibration::set_yaw_min(float const& value)
      {
        m_yaw_min = value;
      }
      void Settings::Input::Sticks_Calibration::set_yaw_min(float&& value)
      {
        m_yaw_min = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_yaw_min() const -> float const& 
      {
        return m_yaw_min;
      }

////////////////////////////////////////////////////////////
      void Settings::Input::Sticks_Calibration::set_yaw_max(float const& value)
      {
        m_yaw_max = value;
      }
      void Settings::Input::Sticks_Calibration::set_yaw_max(float&& value)
      {
        m_yaw_max = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_yaw_max() const -> float const& 
      {
        return m_yaw_max;
      }

////////////////////////////////////////////////////////////
      void Settings::Input::Sticks_Calibration::set_pitch_center(float const& value)
      {
        m_pitch_center = value;
      }
      void Settings::Input::Sticks_Calibration::set_pitch_center(float&& value)
      {
        m_pitch_center = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_pitch_center() const -> float const& 
      {
        return m_pitch_center;
      }

////////////////////////////////////////////////////////////
      void Settings::Input::Sticks_Calibration::set_pitch_min(float const& value)
      {
        m_pitch_min = value;
      }
      void Settings::Input::Sticks_Calibration::set_pitch_min(float&& value)
      {
        m_pitch_min = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_pitch_min() const -> float const& 
      {
        return m_pitch_min;
      }

////////////////////////////////////////////////////////////
      void Settings::Input::Sticks_Calibration::set_pitch_max(float const& value)
      {
        m_pitch_max = value;
      }
      void Settings::Input::Sticks_Calibration::set_pitch_max(float&& value)
      {
        m_pitch_max = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_pitch_max() const -> float const& 
      {
        return m_pitch_max;
      }

////////////////////////////////////////////////////////////
      void Settings::Input::Sticks_Calibration::set_roll_center(float const& value)
      {
        m_roll_center = value;
      }
      void Settings::Input::Sticks_Calibration::set_roll_center(float&& value)
      {
        m_roll_center = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_roll_center() const -> float const& 
      {
        return m_roll_center;
      }

////////////////////////////////////////////////////////////
      void Settings::Input::Sticks_Calibration::set_roll_min(float const& value)
      {
        m_roll_min = value;
      }
      void Settings::Input::Sticks_Calibration::set_roll_min(float&& value)
      {
        m_roll_min = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_roll_min() const -> float const& 
      {
        return m_roll_min;
      }

////////////////////////////////////////////////////////////
      void Settings::Input::Sticks_Calibration::set_roll_max(float const& value)
      {
        m_roll_max = value;
      }
      void Settings::Input::Sticks_Calibration::set_roll_max(float&& value)
      {
        m_roll_max = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_roll_max() const -> float const& 
      {
        return m_roll_max;
      }

////////////////////////////////////////////////////////////
      void Settings::Input::Sticks_Calibration::set_throttle_center(float const& value)
      {
        m_throttle_center = value;
      }
      void Settings::Input::Sticks_Calibration::set_throttle_center(float&& value)
      {
        m_throttle_center = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_throttle_center() const -> float const& 
      {
        return m_throttle_center;
      }

////////////////////////////////////////////////////////////
      void Settings::Input::Sticks_Calibration::set_throttle_min(float const& value)
      {
        m_throttle_min = value;
      }
      void Settings::Input::Sticks_Calibration::set_throttle_min(float&& value)
      {
        m_throttle_min = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_throttle_min() const -> float const& 
      {
        return m_throttle_min;
      }

////////////////////////////////////////////////////////////
      void Settings::Input::Sticks_Calibration::set_throttle_max(float const& value)
      {
        m_throttle_max = value;
      }
      void Settings::Input::Sticks_Calibration::set_throttle_max(float&& value)
      {
        m_throttle_max = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_throttle_max() const -> float const& 
      {
        return m_throttle_max;
      }

////////////////////////////////////////////////////////////
    void Settings::Input::set_sticks_calibration(Sticks_Calibration const& value)
    {
      m_sticks_calibration = value;
    }
    void Settings::Input::set_sticks_calibration(Sticks_Calibration&& value)
    {
      m_sticks_calibration = std::move(value);
    }
    auto Settings::Input::get_sticks_calibration() const -> Sticks_Calibration const& 
    {
      return m_sticks_calibration;
    }

    auto Settings::Input::get_sticks_calibration() -> Sticks_Calibration& 
    {
      return m_sticks_calibration;
    }

////////////////////////////////////////////////////////////
    void Settings::HW::set_pigpio_period_us(pigpio_period_us_t const& value)
    {
      m_pigpio_period_us = clamp(value, pigpio_period_us_t(1), pigpio_period_us_t(10));
    }
    void Settings::HW::set_pigpio_period_us(pigpio_period_us_t&& value)
    {
      m_pigpio_period_us = clamp(std::move(value), pigpio_period_us_t(1), pigpio_period_us_t(10));
    }
    auto Settings::HW::get_pigpio_period_us() const -> pigpio_period_us_t const& 
    {
      return m_pigpio_period_us;
    }

////////////////////////////////////////////////////////////
    void Settings::HW::set_display_incremental_step_us(uint32_t const& value)
    {
      m_display_incremental_step_us = value;
    }
    void Settings::HW::set_display_incremental_step_us(uint32_t&& value)
    {
      m_display_incremental_step_us = std::move(value);
    }
    auto Settings::HW::get_display_incremental_step_us() const -> uint32_t const& 
    {
      return m_display_incremental_step_us;
    }

////////////////////////////////////////////////////////////
  void Settings::set_input(Input const& value)
  {
    m_input = value;
  }
  void Settings::set_input(Input&& value)
  {
    m_input = std::move(value);
  }
  auto Settings::get_input() const -> Input const& 
  {
    return m_input;
  }

  auto Settings::get_input() -> Input& 
  {
    return m_input;
  }

////////////////////////////////////////////////////////////
  void Settings::set_hw(HW const& value)
  {
    m_hw = value;
  }
  void Settings::set_hw(HW&& value)
  {
    m_hw = std::move(value);
  }
  auto Settings::get_hw() const -> HW const& 
  {
    return m_hw;
  }

  auto Settings::get_hw() -> HW& 
  {
    return m_hw;
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
ts::Result<void> deserialize(Settings::Input::Sticks_Calibration& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("yaw_center");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'yaw_center'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_yaw_center())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_yaw_center(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("yaw_min");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'yaw_min'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_yaw_min())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_yaw_min(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("yaw_max");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'yaw_max'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_yaw_max())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_yaw_max(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("pitch_center");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'pitch_center'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_pitch_center())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_pitch_center(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("pitch_min");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'pitch_min'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_pitch_min())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_pitch_min(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("pitch_max");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'pitch_max'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_pitch_max())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_pitch_max(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("roll_center");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'roll_center'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_roll_center())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_roll_center(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("roll_min");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'roll_min'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_roll_min())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_roll_min(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("roll_max");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'roll_max'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_roll_max())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_roll_max(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("throttle_center");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'throttle_center'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_throttle_center())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_throttle_center(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("throttle_min");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'throttle_min'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_throttle_min())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_throttle_min(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("throttle_max");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'throttle_max'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_throttle_max())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_throttle_max(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Settings::Input::Sticks_Calibration const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(12);
  sz_value.add_object_member("yaw_center", serialize(value.get_yaw_center()));
  sz_value.add_object_member("yaw_min", serialize(value.get_yaw_min()));
  sz_value.add_object_member("yaw_max", serialize(value.get_yaw_max()));
  sz_value.add_object_member("pitch_center", serialize(value.get_pitch_center()));
  sz_value.add_object_member("pitch_min", serialize(value.get_pitch_min()));
  sz_value.add_object_member("pitch_max", serialize(value.get_pitch_max()));
  sz_value.add_object_member("roll_center", serialize(value.get_roll_center()));
  sz_value.add_object_member("roll_min", serialize(value.get_roll_min()));
  sz_value.add_object_member("roll_max", serialize(value.get_roll_max()));
  sz_value.add_object_member("throttle_center", serialize(value.get_throttle_center()));
  sz_value.add_object_member("throttle_min", serialize(value.get_throttle_min()));
  sz_value.add_object_member("throttle_max", serialize(value.get_throttle_max()));
  return sz_value;
}
ts::Result<void> deserialize(Settings::Input& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("sticks_calibration");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'sticks_calibration'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_sticks_calibration())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_sticks_calibration(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Settings::Input const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("sticks_calibration", serialize(value.get_sticks_calibration()));
  return sz_value;
}
ts::Result<void> deserialize(Settings::HW& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("pigpio_period_us");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'pigpio_period_us'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_pigpio_period_us())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_pigpio_period_us(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("display_incremental_step_us");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'display_incremental_step_us'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_display_incremental_step_us())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_display_incremental_step_us(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Settings::HW const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("pigpio_period_us", serialize(value.get_pigpio_period_us()));
  sz_value.add_object_member("display_incremental_step_us", serialize(value.get_display_incremental_step_us()));
  return sz_value;
}
ts::Result<void> deserialize(Settings& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("input");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'input'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_input())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_input(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("hw");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'hw'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_hw())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_hw(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Settings const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("input", serialize(value.get_input()));
  sz_value.add_object_member("hw", serialize(value.get_hw()));
  return sz_value;
}
}
}
