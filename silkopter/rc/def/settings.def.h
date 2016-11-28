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
      void set_yaw_center(float const& value);
      void set_yaw_center(float&& value);
      auto get_yaw_center() const -> float const&;

      void set_yaw_min(float const& value);
      void set_yaw_min(float&& value);
      auto get_yaw_min() const -> float const&;

      void set_yaw_max(float const& value);
      void set_yaw_max(float&& value);
      auto get_yaw_max() const -> float const&;

      void set_pitch_center(float const& value);
      void set_pitch_center(float&& value);
      auto get_pitch_center() const -> float const&;

      void set_pitch_min(float const& value);
      void set_pitch_min(float&& value);
      auto get_pitch_min() const -> float const&;

      void set_pitch_max(float const& value);
      void set_pitch_max(float&& value);
      auto get_pitch_max() const -> float const&;

      void set_roll_center(float const& value);
      void set_roll_center(float&& value);
      auto get_roll_center() const -> float const&;

      void set_roll_min(float const& value);
      void set_roll_min(float&& value);
      auto get_roll_min() const -> float const&;

      void set_roll_max(float const& value);
      void set_roll_max(float&& value);
      auto get_roll_max() const -> float const&;

      void set_throttle_center(float const& value);
      void set_throttle_center(float&& value);
      auto get_throttle_center() const -> float const&;

      void set_throttle_min(float const& value);
      void set_throttle_min(float&& value);
      auto get_throttle_min() const -> float const&;

      void set_throttle_max(float const& value);
      void set_throttle_max(float&& value);
      auto get_throttle_max() const -> float const&;

    private:
      float m_yaw_center = {0.500000f};
      float m_yaw_min = {0};
      float m_yaw_max = {1.000000f};
      float m_pitch_center = {0.500000f};
      float m_pitch_min = {0};
      float m_pitch_max = {1.000000f};
      float m_roll_center = {0.500000f};
      float m_roll_min = {0};
      float m_roll_max = {1.000000f};
      float m_throttle_center = {0.500000f};
      float m_throttle_min = {0};
      float m_throttle_max = {1.000000f};
    };

    virtual ~Input() = default;
    void set_sticks_calibration(Sticks_Calibration const& value);
    void set_sticks_calibration(Sticks_Calibration&& value);
    auto get_sticks_calibration() const -> Sticks_Calibration const&;
    auto get_sticks_calibration() -> Sticks_Calibration&;

  private:
    Sticks_Calibration m_sticks_calibration;
  };

  virtual ~Settings() = default;
private:
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
ts::Result<void> deserialize(Settings& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Settings const& value);
}
}
