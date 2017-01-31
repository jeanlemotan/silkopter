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

namespace messages
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
typedef int64_t int64_t;
typedef float ufloat;
struct INode_Message
{
public:
  virtual ~INode_Message() = default;
private:
};

typedef Poly<INode_Message> Poly_INode_Message;
struct Multirotor_Simulator_Reset_Message : public INode_Message
{
public:
  virtual ~Multirotor_Simulator_Reset_Message() = default;
private:
};

struct Multirotor_Simulator_Stop_Motion_Message : public INode_Message
{
public:
  virtual ~Multirotor_Simulator_Stop_Motion_Message() = default;
private:
};

struct Multirotor_Simulator_Set_Gravity_Enabled_Message : public INode_Message
{
public:
  virtual ~Multirotor_Simulator_Set_Gravity_Enabled_Message() = default;
  void set_enabled(bool const& value);
  void set_enabled(bool&& value);
  auto get_enabled() const -> bool const&;

private:
  bool m_enabled = bool{true};
};

struct Multirotor_Simulator_Set_Ground_Enabled_Message : public INode_Message
{
public:
  virtual ~Multirotor_Simulator_Set_Ground_Enabled_Message() = default;
  void set_enabled(bool const& value);
  void set_enabled(bool&& value);
  auto get_enabled() const -> bool const&;

private:
  bool m_enabled = bool{true};
};

struct Multirotor_Simulator_Set_Simulation_Enabled_Message : public INode_Message
{
public:
  virtual ~Multirotor_Simulator_Set_Simulation_Enabled_Message() = default;
  void set_enabled(bool const& value);
  void set_enabled(bool&& value);
  auto get_enabled() const -> bool const&;

private:
  bool m_enabled = bool{true};
};

struct Multirotor_Simulator_Set_Drag_Enabled_Message : public INode_Message
{
public:
  virtual ~Multirotor_Simulator_Set_Drag_Enabled_Message() = default;
  void set_enabled(bool const& value);
  void set_enabled(bool&& value);
  auto get_enabled() const -> bool const&;

private:
  bool m_enabled = bool{true};
};

struct Multirotor_Simulator_Get_State_Message : public INode_Message
{
public:
  virtual ~Multirotor_Simulator_Get_State_Message() = default;
private:
};

struct Multirotor_Simulation_State
{
public:
  struct Motor_State
  {
  public:
    virtual ~Motor_State() = default;
    void set_drag_factor(float const& value);
    void set_drag_factor(float&& value);
    auto get_drag_factor() const -> float const&;

    void set_throttle(float const& value);
    void set_throttle(float&& value);
    auto get_throttle() const -> float const&;

    void set_thrust(float const& value);
    void set_thrust(float&& value);
    auto get_thrust() const -> float const&;

  private:
    float m_drag_factor = float{0};
    float m_throttle = float{0};
    float m_thrust = float{0};
  };

  typedef vec4f quatf;
  virtual ~Multirotor_Simulation_State() = default;
  void set_enu_position(vec3f const& value);
  void set_enu_position(vec3f&& value);
  auto get_enu_position() const -> vec3f const&;

  void set_local_to_enu_rotation(quatf const& value);
  void set_local_to_enu_rotation(quatf&& value);
  auto get_local_to_enu_rotation() const -> quatf const&;

  void set_enu_velocity(vec3f const& value);
  void set_enu_velocity(vec3f&& value);
  auto get_enu_velocity() const -> vec3f const&;

  void set_enu_linear_acceleration(vec3f const& value);
  void set_enu_linear_acceleration(vec3f&& value);
  auto get_enu_linear_acceleration() const -> vec3f const&;

  void set_acceleration(vec3f const& value);
  void set_acceleration(vec3f&& value);
  auto get_acceleration() const -> vec3f const&;

  void set_angular_velocity(vec3f const& value);
  void set_angular_velocity(vec3f&& value);
  auto get_angular_velocity() const -> vec3f const&;

  void set_magnetic_field(vec3f const& value);
  void set_magnetic_field(vec3f&& value);
  auto get_magnetic_field() const -> vec3f const&;

  void set_pressure(float const& value);
  void set_pressure(float&& value);
  auto get_pressure() const -> float const&;

  void set_temperature(float const& value);
  void set_temperature(float&& value);
  auto get_temperature() const -> float const&;

  void set_proximity_distance(vec3f const& value);
  void set_proximity_distance(vec3f&& value);
  auto get_proximity_distance() const -> vec3f const&;

  void set_motors(std::vector<Multirotor_Simulation_State::Motor_State> const& value);
  void set_motors(std::vector<Multirotor_Simulation_State::Motor_State>&& value);
  auto get_motors() const -> std::vector<Multirotor_Simulation_State::Motor_State> const&;
  auto get_motors() -> std::vector<Multirotor_Simulation_State::Motor_State>&;

private:
  vec3f m_enu_position = vec3f{0, 0, 0};
  quatf m_local_to_enu_rotation = quatf{0, 0, 0, 1.000000f};
  vec3f m_enu_velocity = vec3f{0, 0, 0};
  vec3f m_enu_linear_acceleration = vec3f{0, 0, 0};
  vec3f m_acceleration = vec3f{0, 0, 0};
  vec3f m_angular_velocity = vec3f{0, 0, 0};
  vec3f m_magnetic_field = vec3f{0, 0, 0};
  float m_pressure = float{0};
  float m_temperature = float{0};
  vec3f m_proximity_distance = vec3f{0, 0, 0};
  std::vector<Multirotor_Simulation_State::Motor_State> m_motors;
};

struct Multirotor_Simulator_State_Message : public INode_Message
{
public:
  virtual ~Multirotor_Simulator_State_Message() = default;
  void set_state(Multirotor_Simulation_State const& value);
  void set_state(Multirotor_Simulation_State&& value);
  auto get_state() const -> Multirotor_Simulation_State const&;
  auto get_state() -> Multirotor_Simulation_State&;

private:
  Multirotor_Simulation_State m_state;
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
ts::Result<void> deserialize(INode_Message& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(INode_Message const& value);
ts::Result<void> deserialize(Poly<INode_Message>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Poly<INode_Message> const& value);
ts::Result<void> deserialize(Multirotor_Simulator_Reset_Message& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulator_Reset_Message const& value);
ts::Result<void> deserialize(Multirotor_Simulator_Stop_Motion_Message& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulator_Stop_Motion_Message const& value);
ts::Result<void> deserialize(Multirotor_Simulator_Set_Gravity_Enabled_Message& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulator_Set_Gravity_Enabled_Message const& value);
ts::Result<void> deserialize(Multirotor_Simulator_Set_Ground_Enabled_Message& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulator_Set_Ground_Enabled_Message const& value);
ts::Result<void> deserialize(Multirotor_Simulator_Set_Simulation_Enabled_Message& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulator_Set_Simulation_Enabled_Message const& value);
ts::Result<void> deserialize(Multirotor_Simulator_Set_Drag_Enabled_Message& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulator_Set_Drag_Enabled_Message const& value);
ts::Result<void> deserialize(Multirotor_Simulator_Get_State_Message& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulator_Get_State_Message const& value);
ts::Result<void> deserialize(Multirotor_Simulation_State::Motor_State& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulation_State::Motor_State const& value);
ts::Result<void> deserialize(Multirotor_Simulation_State& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulation_State const& value);
ts::Result<void> deserialize(std::vector<Multirotor_Simulation_State::Motor_State>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<Multirotor_Simulation_State::Motor_State> const& value);
ts::Result<void> deserialize(Multirotor_Simulator_State_Message& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulator_State_Message const& value);
}
}
