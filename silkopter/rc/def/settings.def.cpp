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
      void Settings::Input::Sticks_Calibration::set_yaw_curve(float const& value)
      {
        m_yaw_curve = value;
      }
      void Settings::Input::Sticks_Calibration::set_yaw_curve(float&& value)
      {
        m_yaw_curve = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_yaw_curve() const -> float const& 
      {
        return m_yaw_curve;
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
      void Settings::Input::Sticks_Calibration::set_pitch_curve(float const& value)
      {
        m_pitch_curve = value;
      }
      void Settings::Input::Sticks_Calibration::set_pitch_curve(float&& value)
      {
        m_pitch_curve = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_pitch_curve() const -> float const& 
      {
        return m_pitch_curve;
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
      void Settings::Input::Sticks_Calibration::set_roll_curve(float const& value)
      {
        m_roll_curve = value;
      }
      void Settings::Input::Sticks_Calibration::set_roll_curve(float&& value)
      {
        m_roll_curve = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_roll_curve() const -> float const& 
      {
        return m_roll_curve;
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
      void Settings::Input::Sticks_Calibration::set_throttle_curve(float const& value)
      {
        m_throttle_curve = value;
      }
      void Settings::Input::Sticks_Calibration::set_throttle_curve(float&& value)
      {
        m_throttle_curve = std::move(value);
      }
      auto Settings::Input::Sticks_Calibration::get_throttle_curve() const -> float const& 
      {
        return m_throttle_curve;
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
    void Settings::Comms::set_channel(channel_t const& value)
    {
      m_channel = clamp(value, channel_t(1), channel_t(11));
    }
    void Settings::Comms::set_channel(channel_t&& value)
    {
      m_channel = clamp(std::move(value), channel_t(1), channel_t(11));
    }
    auto Settings::Comms::get_channel() const -> channel_t const& 
    {
      return m_channel;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_tx_power(tx_power_t const& value)
    {
      m_tx_power = clamp(value, tx_power_t(0), tx_power_t(20.500000f));
    }
    void Settings::Comms::set_tx_power(tx_power_t&& value)
    {
      m_tx_power = clamp(std::move(value), tx_power_t(0), tx_power_t(20.500000f));
    }
    auto Settings::Comms::get_tx_power() const -> tx_power_t const& 
    {
      return m_tx_power;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_mtu(mtu_t const& value)
    {
      m_mtu = clamp(value, mtu_t(128), mtu_t(1360));
    }
    void Settings::Comms::set_mtu(mtu_t&& value)
    {
      m_mtu = clamp(std::move(value), mtu_t(128), mtu_t(1360));
    }
    auto Settings::Comms::get_mtu() const -> mtu_t const& 
    {
      return m_mtu;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_rate(rate_t const& value)
    {
      m_rate = value;
    }
    void Settings::Comms::set_rate(rate_t&& value)
    {
      m_rate = std::move(value);
    }
    auto Settings::Comms::get_rate() const -> rate_t const& 
    {
      return m_rate;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_fec_coding_k(fec_coding_k_t const& value)
    {
      m_fec_coding_k = clamp(value, fec_coding_k_t(1), fec_coding_k_t(32));
    }
    void Settings::Comms::set_fec_coding_k(fec_coding_k_t&& value)
    {
      m_fec_coding_k = clamp(std::move(value), fec_coding_k_t(1), fec_coding_k_t(32));
    }
    auto Settings::Comms::get_fec_coding_k() const -> fec_coding_k_t const& 
    {
      return m_fec_coding_k;
    }

////////////////////////////////////////////////////////////
    void Settings::Comms::set_fec_coding_n(fec_coding_n_t const& value)
    {
      m_fec_coding_n = clamp(value, fec_coding_n_t(1), fec_coding_n_t(32));
    }
    void Settings::Comms::set_fec_coding_n(fec_coding_n_t&& value)
    {
      m_fec_coding_n = clamp(std::move(value), fec_coding_n_t(1), fec_coding_n_t(32));
    }
    auto Settings::Comms::get_fec_coding_n() const -> fec_coding_n_t const& 
    {
      return m_fec_coding_n;
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
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "yaw_min")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_yaw_min())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_yaw_min(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "yaw_center")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_yaw_center())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_yaw_center(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "yaw_max")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_yaw_max())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_yaw_max(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "yaw_deadband")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_yaw_deadband())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_yaw_deadband(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "yaw_curve")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_yaw_curve())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_yaw_curve(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "pitch_min")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_pitch_min())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_pitch_min(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "pitch_center")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_pitch_center())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_pitch_center(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "pitch_max")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_pitch_max())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_pitch_max(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "pitch_deadband")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_pitch_deadband())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_pitch_deadband(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "pitch_curve")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_pitch_curve())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_pitch_curve(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "roll_min")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_roll_min())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_roll_min(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "roll_center")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_roll_center())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_roll_center(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "roll_max")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_roll_max())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_roll_max(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "roll_deadband")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_roll_deadband())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_roll_deadband(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "roll_curve")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_roll_curve())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_roll_curve(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "throttle_min")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_throttle_min())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_throttle_min(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "throttle_center")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_throttle_center())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_throttle_center(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "throttle_max")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_throttle_max())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_throttle_max(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "throttle_deadband")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_throttle_deadband())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_throttle_deadband(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "throttle_curve")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_throttle_curve())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_throttle_curve(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Settings::Input::Sticks_Calibration const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(20);
  sz_value.add_object_member("yaw_min", serialize(value.get_yaw_min()));
  sz_value.add_object_member("yaw_center", serialize(value.get_yaw_center()));
  sz_value.add_object_member("yaw_max", serialize(value.get_yaw_max()));
  sz_value.add_object_member("yaw_deadband", serialize(value.get_yaw_deadband()));
  sz_value.add_object_member("yaw_curve", serialize(value.get_yaw_curve()));
  sz_value.add_object_member("pitch_min", serialize(value.get_pitch_min()));
  sz_value.add_object_member("pitch_center", serialize(value.get_pitch_center()));
  sz_value.add_object_member("pitch_max", serialize(value.get_pitch_max()));
  sz_value.add_object_member("pitch_deadband", serialize(value.get_pitch_deadband()));
  sz_value.add_object_member("pitch_curve", serialize(value.get_pitch_curve()));
  sz_value.add_object_member("roll_min", serialize(value.get_roll_min()));
  sz_value.add_object_member("roll_center", serialize(value.get_roll_center()));
  sz_value.add_object_member("roll_max", serialize(value.get_roll_max()));
  sz_value.add_object_member("roll_deadband", serialize(value.get_roll_deadband()));
  sz_value.add_object_member("roll_curve", serialize(value.get_roll_curve()));
  sz_value.add_object_member("throttle_min", serialize(value.get_throttle_min()));
  sz_value.add_object_member("throttle_center", serialize(value.get_throttle_center()));
  sz_value.add_object_member("throttle_max", serialize(value.get_throttle_max()));
  sz_value.add_object_member("throttle_deadband", serialize(value.get_throttle_deadband()));
  sz_value.add_object_member("throttle_curve", serialize(value.get_throttle_curve()));
  return sz_value;
}
ts::Result<void> deserialize(Settings::Input& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "sticks_calibration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_sticks_calibration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_sticks_calibration(std::move(v));
    }
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
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "pigpio_period_us")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_pigpio_period_us())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_pigpio_period_us(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "display_incremental_step_us")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_display_incremental_step_us())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_display_incremental_step_us(std::move(v));
    }
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
ts::Result<void> deserialize(Settings::Comms::rate_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  std::string const& key = sz_value.get_as_string();
  typedef Settings::Comms::rate_t _etype;
  static std::map<std::string, _etype> s_map = {
    { "B_1M_CCK", _etype::B_1M_CCK },
    { "B_2M_CCK", _etype::B_2M_CCK },
    { "B_2M_CCK_SHORT_PREAMBLE", _etype::B_2M_CCK_SHORT_PREAMBLE },
    { "B_5_5M_CCK", _etype::B_5_5M_CCK },
    { "B_5_5M_CCK_SHORT_PREAMBLE", _etype::B_5_5M_CCK_SHORT_PREAMBLE },
    { "B_11M_CCK", _etype::B_11M_CCK },
    { "B_11M_CCK_SHORT_PREAMBLE", _etype::B_11M_CCK_SHORT_PREAMBLE },
    { "G_6M_ODFM", _etype::G_6M_ODFM },
    { "G_9M_ODFM", _etype::G_9M_ODFM },
    { "G_12M_ODFM", _etype::G_12M_ODFM },
    { "G_18M_ODFM", _etype::G_18M_ODFM },
    { "G_24M_ODFM", _etype::G_24M_ODFM },
    { "G_36M_ODFM", _etype::G_36M_ODFM },
    { "G_48M_ODFM", _etype::G_48M_ODFM },
    { "G_54M_ODFM", _etype::G_54M_ODFM },
  };
  auto it = s_map.find(key);
  if (it == s_map.end()) { return ts::Error("Cannot find item " + key + " when deserializing"); }
  value = it->second;
  return ts::success;
}
ts::sz::Value serialize(Settings::Comms::rate_t const& value)
{
  typedef Settings::Comms::rate_t _etype;
  static std::map<_etype, std::string> s_map = {
    { _etype::B_1M_CCK, "B_1M_CCK" },
    { _etype::B_2M_CCK, "B_2M_CCK" },
    { _etype::B_2M_CCK_SHORT_PREAMBLE, "B_2M_CCK_SHORT_PREAMBLE" },
    { _etype::B_5_5M_CCK, "B_5_5M_CCK" },
    { _etype::B_5_5M_CCK_SHORT_PREAMBLE, "B_5_5M_CCK_SHORT_PREAMBLE" },
    { _etype::B_11M_CCK, "B_11M_CCK" },
    { _etype::B_11M_CCK_SHORT_PREAMBLE, "B_11M_CCK_SHORT_PREAMBLE" },
    { _etype::G_6M_ODFM, "G_6M_ODFM" },
    { _etype::G_9M_ODFM, "G_9M_ODFM" },
    { _etype::G_12M_ODFM, "G_12M_ODFM" },
    { _etype::G_18M_ODFM, "G_18M_ODFM" },
    { _etype::G_24M_ODFM, "G_24M_ODFM" },
    { _etype::G_36M_ODFM, "G_36M_ODFM" },
    { _etype::G_48M_ODFM, "G_48M_ODFM" },
    { _etype::G_54M_ODFM, "G_54M_ODFM" },
  };
  auto it = s_map.find(value);
  if (it == s_map.end()) { TS_ASSERT(false); return ts::sz::Value(); }
  return ts::sz::Value(it->second);
}
ts::Result<void> deserialize(Settings::Comms& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "channel")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_channel())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_channel(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "tx_power")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_tx_power())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_tx_power(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "mtu")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_mtu())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_mtu(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "fec_coding_k")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_fec_coding_k())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_fec_coding_k(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "fec_coding_n")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_fec_coding_n())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_fec_coding_n(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Settings::Comms const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(6);
  sz_value.add_object_member("channel", serialize(value.get_channel()));
  sz_value.add_object_member("tx_power", serialize(value.get_tx_power()));
  sz_value.add_object_member("mtu", serialize(value.get_mtu()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  sz_value.add_object_member("fec_coding_k", serialize(value.get_fec_coding_k()));
  sz_value.add_object_member("fec_coding_n", serialize(value.get_fec_coding_n()));
  return sz_value;
}
ts::Result<void> deserialize(Settings::Battery_Info& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "low_voltage")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_low_voltage())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_low_voltage(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "critical_voltage")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_critical_voltage())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_critical_voltage(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "calibration_bias")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_calibration_bias())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_calibration_bias(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "calibration_scale")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_calibration_scale())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_calibration_scale(std::move(v));
    }
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
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "input")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_input())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_input(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "hw")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_hw())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_hw(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "comms")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_comms())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_comms(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "battery_info")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_battery_info())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_battery_info(std::move(v));
    }
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
}
}
