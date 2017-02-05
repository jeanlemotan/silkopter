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
}
}
