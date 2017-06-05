#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <cfloat>
#include <boost/variant.hpp>
#include <def_lang/Result.h>
#include <def_lang/Serialization.h>
#include "gen_support.h"
namespace silk
{

namespace settings
{

template<class T>
struct Poly
{
  Poly() = default;
  explicit Poly(T* ptr) : ptr(ptr) {}
  explicit Poly(std::shared_ptr<T> ptr) : ptr(ptr) {}
  Poly(Poly const& other) = default;
  template <class U> Poly(Poly<U> const& other) : ptr(std::dynamic_pointer_cast<U>(other.ptr)) {}
  Poly(Poly&& other) = default;
  Poly& operator=(Poly const& other) = default;
  Poly& operator=(Poly&& other) = default;
  explicit operator bool() const { return ptr != nullptr; }
  T* operator->() { return ptr.get(); }
  T const* operator->() const { return ptr.get(); }
  T& operator*() { return *ptr; }
  T const& operator*() const { return *ptr; }
  T* get() { return ptr.get(); }
  T const* get() const { return ptr.get(); }
  std::shared_ptr<T> get_shared_ptr() { return ptr; }
  std::shared_ptr<const T> get_shared_ptr() const { return ptr; }
private:
  template<class U> friend class Poly;
  std::shared_ptr<T> ptr;
};
typedef int8_t int8_t;
typedef uint8_t uint8_t;
typedef int16_t int16_t;
typedef uint16_t uint16_t;
typedef int32_t int32_t;
typedef uint32_t uint32_t;
typedef float ufloat;
typedef float muf_t;
typedef vec3f direction_t;
typedef float euler_t;
typedef vec3f euler3_t;
typedef uint32_t gpio_t;
struct Settings
{
public:
  struct Input
  {
  public:
    struct Sticks_Calibration
    {
    public:
      virtual ~Sticks_Calibration() = default;
      void set_yaw_min(float const& value);
      void set_yaw_min(float&& value);
      auto get_yaw_min() const -> float const&;

      void set_yaw_center(float const& value);
      void set_yaw_center(float&& value);
      auto get_yaw_center() const -> float const&;

      void set_yaw_deadband(float const& value);
      void set_yaw_deadband(float&& value);
      auto get_yaw_deadband() const -> float const&;

      void set_yaw_max(float const& value);
      void set_yaw_max(float&& value);
      auto get_yaw_max() const -> float const&;

      void set_pitch_min(float const& value);
      void set_pitch_min(float&& value);
      auto get_pitch_min() const -> float const&;

      void set_pitch_center(float const& value);
      void set_pitch_center(float&& value);
      auto get_pitch_center() const -> float const&;

      void set_pitch_deadband(float const& value);
      void set_pitch_deadband(float&& value);
      auto get_pitch_deadband() const -> float const&;

      void set_pitch_max(float const& value);
      void set_pitch_max(float&& value);
      auto get_pitch_max() const -> float const&;

      void set_roll_min(float const& value);
      void set_roll_min(float&& value);
      auto get_roll_min() const -> float const&;

      void set_roll_center(float const& value);
      void set_roll_center(float&& value);
      auto get_roll_center() const -> float const&;

      void set_roll_deadband(float const& value);
      void set_roll_deadband(float&& value);
      auto get_roll_deadband() const -> float const&;

      void set_roll_max(float const& value);
      void set_roll_max(float&& value);
      auto get_roll_max() const -> float const&;

      void set_throttle_min(float const& value);
      void set_throttle_min(float&& value);
      auto get_throttle_min() const -> float const&;

      void set_throttle_center(float const& value);
      void set_throttle_center(float&& value);
      auto get_throttle_center() const -> float const&;

      void set_throttle_deadband(float const& value);
      void set_throttle_deadband(float&& value);
      auto get_throttle_deadband() const -> float const&;

      void set_throttle_max(float const& value);
      void set_throttle_max(float&& value);
      auto get_throttle_max() const -> float const&;

    private:
      float m_yaw_min = float{0};
      float m_yaw_center = float{0.500000f};
      float m_yaw_deadband = float{0};
      float m_yaw_max = float{1.000000f};
      float m_pitch_min = float{0};
      float m_pitch_center = float{0.500000f};
      float m_pitch_deadband = float{0};
      float m_pitch_max = float{1.000000f};
      float m_roll_min = float{0};
      float m_roll_center = float{0.500000f};
      float m_roll_deadband = float{0};
      float m_roll_max = float{1.000000f};
      float m_throttle_min = float{0};
      float m_throttle_center = float{0.500000f};
      float m_throttle_deadband = float{0};
      float m_throttle_max = float{1.000000f};
    };

    virtual ~Input() = default;
    void set_sticks_calibration(Sticks_Calibration const& value);
    void set_sticks_calibration(Sticks_Calibration&& value);
    auto get_sticks_calibration() const -> Sticks_Calibration const&;
    auto get_sticks_calibration() -> Sticks_Calibration&;

  private:
    Sticks_Calibration m_sticks_calibration;
  };

  struct HW
  {
  public:
    typedef uint32_t pigpio_period_us_t;
    virtual ~HW() = default;
    void set_pigpio_period_us(pigpio_period_us_t const& value);
    void set_pigpio_period_us(pigpio_period_us_t&& value);
    auto get_pigpio_period_us() const -> pigpio_period_us_t const&;

    void set_display_incremental_step_us(uint32_t const& value);
    void set_display_incremental_step_us(uint32_t&& value);
    auto get_display_incremental_step_us() const -> uint32_t const&;

  private:
    pigpio_period_us_t m_pigpio_period_us = pigpio_period_us_t{1};
    uint32_t m_display_incremental_step_us = uint32_t{1000};
  };

  struct Comms
  {
  public:
    virtual ~Comms() = default;
    void set_video_wlan_interfaces(std::vector<std::string> const& value);
    void set_video_wlan_interfaces(std::vector<std::string>&& value);
    auto get_video_wlan_interfaces() const -> std::vector<std::string> const&;
    auto get_video_wlan_interfaces() -> std::vector<std::string>&;

    void set_video_fec_coding_k(uint32_t const& value);
    void set_video_fec_coding_k(uint32_t&& value);
    auto get_video_fec_coding_k() const -> uint32_t const&;

    void set_video_fec_coding_n(uint32_t const& value);
    void set_video_fec_coding_n(uint32_t&& value);
    auto get_video_fec_coding_n() const -> uint32_t const&;

    void set_video_max_latency_ms(uint32_t const& value);
    void set_video_max_latency_ms(uint32_t&& value);
    auto get_video_max_latency_ms() const -> uint32_t const&;

    void set_video_reset_duration_ms(uint32_t const& value);
    void set_video_reset_duration_ms(uint32_t&& value);
    auto get_video_reset_duration_ms() const -> uint32_t const&;

    void set_rc_sdn_gpio(gpio_t const& value);
    void set_rc_sdn_gpio(gpio_t&& value);
    auto get_rc_sdn_gpio() const -> gpio_t const&;

    void set_rc_spi_device(std::string const& value);
    void set_rc_spi_device(std::string&& value);
    auto get_rc_spi_device() const -> std::string const&;

    void set_rc_spi_speed(uint32_t const& value);
    void set_rc_spi_speed(uint32_t&& value);
    auto get_rc_spi_speed() const -> uint32_t const&;

    void set_rc_channel(uint8_t const& value);
    void set_rc_channel(uint8_t&& value);
    auto get_rc_channel() const -> uint8_t const&;

    void set_rc_xtal_adjustment(float const& value);
    void set_rc_xtal_adjustment(float&& value);
    auto get_rc_xtal_adjustment() const -> float const&;

  private:
    std::vector<std::string> m_video_wlan_interfaces;
    uint32_t m_video_fec_coding_k = uint32_t{12};
    uint32_t m_video_fec_coding_n = uint32_t{20};
    uint32_t m_video_max_latency_ms = uint32_t{500};
    uint32_t m_video_reset_duration_ms = uint32_t{1000};
    gpio_t m_rc_sdn_gpio = gpio_t{6};
    std::string m_rc_spi_device = std::string{"/dev/spidev0.0"};
    uint32_t m_rc_spi_speed = uint32_t{16000000};
    uint8_t m_rc_channel = uint8_t{0};
    float m_rc_xtal_adjustment = float{0};
  };

  struct Battery_Info
  {
  public:
    virtual ~Battery_Info() = default;
    void set_low_voltage(float const& value);
    void set_low_voltage(float&& value);
    auto get_low_voltage() const -> float const&;

    void set_critical_voltage(float const& value);
    void set_critical_voltage(float&& value);
    auto get_critical_voltage() const -> float const&;

    void set_calibration_bias(float const& value);
    void set_calibration_bias(float&& value);
    auto get_calibration_bias() const -> float const&;

    void set_calibration_scale(float const& value);
    void set_calibration_scale(float&& value);
    auto get_calibration_scale() const -> float const&;

  private:
    float m_low_voltage = float{7.000000f};
    float m_critical_voltage = float{6.000000f};
    float m_calibration_bias = float{0};
    float m_calibration_scale = float{1.000000f};
  };

  virtual ~Settings() = default;
  void set_input(Input const& value);
  void set_input(Input&& value);
  auto get_input() const -> Input const&;
  auto get_input() -> Input&;

  void set_hw(HW const& value);
  void set_hw(HW&& value);
  auto get_hw() const -> HW const&;
  auto get_hw() -> HW&;

  void set_comms(Comms const& value);
  void set_comms(Comms&& value);
  auto get_comms() const -> Comms const&;
  auto get_comms() -> Comms&;

  void set_battery_info(Battery_Info const& value);
  void set_battery_info(Battery_Info&& value);
  auto get_battery_info() const -> Battery_Info const&;
  auto get_battery_info() -> Battery_Info&;

private:
  Input m_input;
  HW m_hw;
  Comms m_comms;
  Battery_Info m_battery_info;
};

ts::Result<void> deserialize(std::string& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::string const& value);
ts::Result<void> deserialize(bool& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(bool const& value);
ts::Result<void> deserialize(int64_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(int64_t const& value);
ts::Result<void> deserialize(float& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(float const& value);
ts::Result<void> deserialize(double& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(double const& value);
ts::Result<void> deserialize(vec2f& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(vec2f const& value);
ts::Result<void> deserialize(vec2d& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(vec2d const& value);
ts::Result<void> deserialize(vec2i& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(vec2i const& value);
ts::Result<void> deserialize(vec3f& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(vec3f const& value);
ts::Result<void> deserialize(vec3d& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(vec3d const& value);
ts::Result<void> deserialize(vec3i& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(vec3i const& value);
ts::Result<void> deserialize(vec4f& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(vec4f const& value);
ts::Result<void> deserialize(vec4d& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(vec4d const& value);
ts::Result<void> deserialize(vec4i& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(vec4i const& value);
ts::Result<void> deserialize(int8_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(int8_t const& value);
ts::Result<void> deserialize(uint8_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(uint8_t const& value);
ts::Result<void> deserialize(int16_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(int16_t const& value);
ts::Result<void> deserialize(uint16_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(uint16_t const& value);
ts::Result<void> deserialize(int32_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(int32_t const& value);
ts::Result<void> deserialize(uint32_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(uint32_t const& value);
ts::Result<void> deserialize(Settings::Input::Sticks_Calibration& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Settings::Input::Sticks_Calibration const& value);
ts::Result<void> deserialize(Settings::Input& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Settings::Input const& value);
ts::Result<void> deserialize(Settings::HW& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Settings::HW const& value);
ts::Result<void> deserialize(Settings::Comms& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Settings::Comms const& value);
ts::Result<void> deserialize(Settings::Battery_Info& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Settings::Battery_Info const& value);
ts::Result<void> deserialize(Settings& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Settings const& value);
ts::Result<void> deserialize(std::vector<std::string>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<std::string> const& value);
}
}
