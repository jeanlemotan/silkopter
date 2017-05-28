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
      void Settings::Input::Sticks_Calibration::set_yaw_deadband(float const& value)
      {
        m_yaw_deadband = value;
      }
      void Settings::Input::Sticks_Calibration::set_yaw_deadband(float&& value)
      {
        m_yaw_deadband = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_yaw_deadband() const -> float const& 
      {
        return m_yaw_deadband;
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
      void Settings::Input::Sticks_Calibration::set_pitch_deadband(float const& value)
      {
        m_pitch_deadband = value;
      }
      void Settings::Input::Sticks_Calibration::set_pitch_deadband(float&& value)
      {
        m_pitch_deadband = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_pitch_deadband() const -> float const& 
      {
        return m_pitch_deadband;
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
      void Settings::Input::Sticks_Calibration::set_roll_deadband(float const& value)
      {
        m_roll_deadband = value;
      }
      void Settings::Input::Sticks_Calibration::set_roll_deadband(float&& value)
      {
        m_roll_deadband = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_roll_deadband() const -> float const& 
      {
        return m_roll_deadband;
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
      void Settings::Input::Sticks_Calibration::set_throttle_deadband(float const& value)
      {
        m_throttle_deadband = value;
      }
      void Settings::Input::Sticks_Calibration::set_throttle_deadband(float&& value)
      {
        m_throttle_deadband = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_throttle_deadband() const -> float const& 
      {
        return m_throttle_deadband;
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
    void Settings::Comms::set_video_wlan_interfaces(std::vector<std::string> const& value)
    {
      m_video_wlan_interfaces = value;
    }
    void Settings::Comms::set_video_wlan_interfaces(std::vector<std::string>&& value)
    {
      m_video_wlan_interfaces = std::move(value);
    }
    auto Settings::Comms::get_video_wlan_interfaces() const -> std::vector<std::string> const& 
    {
      return m_video_wlan_interfaces;
    }

    auto Settings::Comms::get_video_wlan_interfaces() -> std::vector<std::string>& 
    {
      return m_video_wlan_interfaces;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_video_fec_coding_k(uint32_t const& value)
    {
      m_video_fec_coding_k = value;
    }
    void Settings::Comms::set_video_fec_coding_k(uint32_t&& value)
    {
      m_video_fec_coding_k = std::move(value);
    }
    auto Settings::Comms::get_video_fec_coding_k() const -> uint32_t const& 
    {
      return m_video_fec_coding_k;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_video_fec_coding_n(uint32_t const& value)
    {
      m_video_fec_coding_n = value;
    }
    void Settings::Comms::set_video_fec_coding_n(uint32_t&& value)
    {
      m_video_fec_coding_n = std::move(value);
    }
    auto Settings::Comms::get_video_fec_coding_n() const -> uint32_t const& 
    {
      return m_video_fec_coding_n;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_video_max_latency_ms(uint32_t const& value)
    {
      m_video_max_latency_ms = value;
    }
    void Settings::Comms::set_video_max_latency_ms(uint32_t&& value)
    {
      m_video_max_latency_ms = std::move(value);
    }
    auto Settings::Comms::get_video_max_latency_ms() const -> uint32_t const& 
    {
      return m_video_max_latency_ms;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_video_reset_duration_ms(uint32_t const& value)
    {
      m_video_reset_duration_ms = value;
    }
    void Settings::Comms::set_video_reset_duration_ms(uint32_t&& value)
    {
      m_video_reset_duration_ms = std::move(value);
    }
    auto Settings::Comms::get_video_reset_duration_ms() const -> uint32_t const& 
    {
      return m_video_reset_duration_ms;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_rc_sdn_gpio(gpio_t const& value)
    {
      m_rc_sdn_gpio = clamp(value, gpio_t(0), gpio_t(40));
    }
    void Settings::Comms::set_rc_sdn_gpio(gpio_t&& value)
    {
      m_rc_sdn_gpio = clamp(std::move(value), gpio_t(0), gpio_t(40));
    }
    auto Settings::Comms::get_rc_sdn_gpio() const -> gpio_t const& 
    {
      return m_rc_sdn_gpio;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_rc_nirq_gpio(gpio_t const& value)
    {
      m_rc_nirq_gpio = clamp(value, gpio_t(0), gpio_t(40));
    }
    void Settings::Comms::set_rc_nirq_gpio(gpio_t&& value)
    {
      m_rc_nirq_gpio = clamp(std::move(value), gpio_t(0), gpio_t(40));
    }
    auto Settings::Comms::get_rc_nirq_gpio() const -> gpio_t const& 
    {
      return m_rc_nirq_gpio;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_rc_spi_device(std::string const& value)
    {
      m_rc_spi_device = value;
    }
    void Settings::Comms::set_rc_spi_device(std::string&& value)
    {
      m_rc_spi_device = std::move(value);
    }
    auto Settings::Comms::get_rc_spi_device() const -> std::string const& 
    {
      return m_rc_spi_device;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_rc_spi_speed(uint32_t const& value)
    {
      m_rc_spi_speed = value;
    }
    void Settings::Comms::set_rc_spi_speed(uint32_t&& value)
    {
      m_rc_spi_speed = std::move(value);
    }
    auto Settings::Comms::get_rc_spi_speed() const -> uint32_t const& 
    {
      return m_rc_spi_speed;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_rc_channel(uint8_t const& value)
    {
      m_rc_channel = value;
    }
    void Settings::Comms::set_rc_channel(uint8_t&& value)
    {
      m_rc_channel = std::move(value);
    }
    auto Settings::Comms::get_rc_channel() const -> uint8_t const& 
    {
      return m_rc_channel;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_rc_xtal_adjustment(float const& value)
    {
      m_rc_xtal_adjustment = value;
    }
    void Settings::Comms::set_rc_xtal_adjustment(float&& value)
    {
      m_rc_xtal_adjustment = std::move(value);
    }
    auto Settings::Comms::get_rc_xtal_adjustment() const -> float const& 
    {
      return m_rc_xtal_adjustment;
    }

////////////////////////////////////////////////////////////
    void Settings::Battery_Info::set_low_voltage(float const& value)
    {
      m_low_voltage = value;
    }
    void Settings::Battery_Info::set_low_voltage(float&& value)
    {
      m_low_voltage = std::move(value);
    }
    auto Settings::Battery_Info::get_low_voltage() const -> float const& 
    {
      return m_low_voltage;
    }

////////////////////////////////////////////////////////////
    void Settings::Battery_Info::set_critical_voltage(float const& value)
    {
      m_critical_voltage = value;
    }
    void Settings::Battery_Info::set_critical_voltage(float&& value)
    {
      m_critical_voltage = std::move(value);
    }
    auto Settings::Battery_Info::get_critical_voltage() const -> float const& 
    {
      return m_critical_voltage;
    }

////////////////////////////////////////////////////////////
    void Settings::Battery_Info::set_calibration_bias(float const& value)
    {
      m_calibration_bias = value;
    }
    void Settings::Battery_Info::set_calibration_bias(float&& value)
    {
      m_calibration_bias = std::move(value);
    }
    auto Settings::Battery_Info::get_calibration_bias() const -> float const& 
    {
      return m_calibration_bias;
    }

////////////////////////////////////////////////////////////
    void Settings::Battery_Info::set_calibration_scale(float const& value)
    {
      m_calibration_scale = value;
    }
    void Settings::Battery_Info::set_calibration_scale(float&& value)
    {
      m_calibration_scale = std::move(value);
    }
    auto Settings::Battery_Info::get_calibration_scale() const -> float const& 
    {
      return m_calibration_scale;
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
  void Settings::set_comms(Comms const& value)
  {
    m_comms = value;
  }
  void Settings::set_comms(Comms&& value)
  {
    m_comms = std::move(value);
  }
  auto Settings::get_comms() const -> Comms const& 
  {
    return m_comms;
  }

  auto Settings::get_comms() -> Comms& 
  {
    return m_comms;
  }

////////////////////////////////////////////////////////////
  void Settings::set_battery_info(Battery_Info const& value)
  {
    m_battery_info = value;
  }
  void Settings::set_battery_info(Battery_Info&& value)
  {
    m_battery_info = std::move(value);
  }
  auto Settings::get_battery_info() const -> Battery_Info const& 
  {
    return m_battery_info;
  }

  auto Settings::get_battery_info() -> Battery_Info& 
  {
    return m_battery_info;
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("yaw_min");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'yaw_min'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_yaw_min())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_yaw_min(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("yaw_center");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'yaw_center'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_yaw_center())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_yaw_center(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("yaw_deadband");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'yaw_deadband'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_yaw_deadband())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_yaw_deadband(std::move(v));
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("pitch_min");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'pitch_min'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_pitch_min())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_pitch_min(std::move(v));
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("pitch_deadband");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'pitch_deadband'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_pitch_deadband())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_pitch_deadband(std::move(v));
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("roll_min");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'roll_min'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_roll_min())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_roll_min(std::move(v));
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("roll_deadband");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'roll_deadband'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_roll_deadband())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_roll_deadband(std::move(v));
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("throttle_min");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'throttle_min'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_throttle_min())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_throttle_min(std::move(v));
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("throttle_deadband");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'throttle_deadband'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_throttle_deadband())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_throttle_deadband(std::move(v));
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
  sz_value.reserve_object_members(16);
  sz_value.add_object_member("yaw_min", serialize(value.get_yaw_min()));
  sz_value.add_object_member("yaw_center", serialize(value.get_yaw_center()));
  sz_value.add_object_member("yaw_deadband", serialize(value.get_yaw_deadband()));
  sz_value.add_object_member("yaw_max", serialize(value.get_yaw_max()));
  sz_value.add_object_member("pitch_min", serialize(value.get_pitch_min()));
  sz_value.add_object_member("pitch_center", serialize(value.get_pitch_center()));
  sz_value.add_object_member("pitch_deadband", serialize(value.get_pitch_deadband()));
  sz_value.add_object_member("pitch_max", serialize(value.get_pitch_max()));
  sz_value.add_object_member("roll_min", serialize(value.get_roll_min()));
  sz_value.add_object_member("roll_center", serialize(value.get_roll_center()));
  sz_value.add_object_member("roll_deadband", serialize(value.get_roll_deadband()));
  sz_value.add_object_member("roll_max", serialize(value.get_roll_max()));
  sz_value.add_object_member("throttle_min", serialize(value.get_throttle_min()));
  sz_value.add_object_member("throttle_center", serialize(value.get_throttle_center()));
  sz_value.add_object_member("throttle_deadband", serialize(value.get_throttle_deadband()));
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
ts::Result<void> deserialize(Settings::Comms& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("video_wlan_interfaces");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'video_wlan_interfaces'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_video_wlan_interfaces())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_video_wlan_interfaces(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("video_fec_coding_k");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'video_fec_coding_k'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_video_fec_coding_k())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_video_fec_coding_k(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("video_fec_coding_n");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'video_fec_coding_n'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_video_fec_coding_n())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_video_fec_coding_n(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("video_max_latency_ms");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'video_max_latency_ms'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_video_max_latency_ms())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_video_max_latency_ms(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("video_reset_duration_ms");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'video_reset_duration_ms'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_video_reset_duration_ms())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_video_reset_duration_ms(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rc_sdn_gpio");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rc_sdn_gpio'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rc_sdn_gpio())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rc_sdn_gpio(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rc_nirq_gpio");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rc_nirq_gpio'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rc_nirq_gpio())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rc_nirq_gpio(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rc_spi_device");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rc_spi_device'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rc_spi_device())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rc_spi_device(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rc_spi_speed");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rc_spi_speed'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rc_spi_speed())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rc_spi_speed(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rc_channel");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rc_channel'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rc_channel())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rc_channel(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rc_xtal_adjustment");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rc_xtal_adjustment'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rc_xtal_adjustment())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rc_xtal_adjustment(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Settings::Comms const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(11);
  sz_value.add_object_member("video_wlan_interfaces", serialize(value.get_video_wlan_interfaces()));
  sz_value.add_object_member("video_fec_coding_k", serialize(value.get_video_fec_coding_k()));
  sz_value.add_object_member("video_fec_coding_n", serialize(value.get_video_fec_coding_n()));
  sz_value.add_object_member("video_max_latency_ms", serialize(value.get_video_max_latency_ms()));
  sz_value.add_object_member("video_reset_duration_ms", serialize(value.get_video_reset_duration_ms()));
  sz_value.add_object_member("rc_sdn_gpio", serialize(value.get_rc_sdn_gpio()));
  sz_value.add_object_member("rc_nirq_gpio", serialize(value.get_rc_nirq_gpio()));
  sz_value.add_object_member("rc_spi_device", serialize(value.get_rc_spi_device()));
  sz_value.add_object_member("rc_spi_speed", serialize(value.get_rc_spi_speed()));
  sz_value.add_object_member("rc_channel", serialize(value.get_rc_channel()));
  sz_value.add_object_member("rc_xtal_adjustment", serialize(value.get_rc_xtal_adjustment()));
  return sz_value;
}
ts::Result<void> deserialize(Settings::Battery_Info& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("low_voltage");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'low_voltage'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_low_voltage())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_low_voltage(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("critical_voltage");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'critical_voltage'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_critical_voltage())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_critical_voltage(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("calibration_bias");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'calibration_bias'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_calibration_bias())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_calibration_bias(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("calibration_scale");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'calibration_scale'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_calibration_scale())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_calibration_scale(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Settings::Battery_Info const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("low_voltage", serialize(value.get_low_voltage()));
  sz_value.add_object_member("critical_voltage", serialize(value.get_critical_voltage()));
  sz_value.add_object_member("calibration_bias", serialize(value.get_calibration_bias()));
  sz_value.add_object_member("calibration_scale", serialize(value.get_calibration_scale()));
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
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("comms");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'comms'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_comms())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_comms(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("battery_info");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'battery_info'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_battery_info())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_battery_info(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(Settings const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("input", serialize(value.get_input()));
  sz_value.add_object_member("hw", serialize(value.get_hw()));
  sz_value.add_object_member("comms", serialize(value.get_comms()));
  sz_value.add_object_member("battery_info", serialize(value.get_battery_info()));
  return sz_value;
}
ts::Result<void> deserialize(std::vector<std::string>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<std::string> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
}
}
