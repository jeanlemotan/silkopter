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

namespace uav
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
private:
  template<class U> friend class Poly;
  std::shared_ptr<T> ptr;
};
// Returns the ast json from which a ast root node can be serialized
std::string const& get_ast_json();
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
typedef float servo_signal_t;
typedef float pwm_signal_t;
struct IUAV_Descriptor
{
public:
  typedef float mass_t;
  virtual ~IUAV_Descriptor() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

  void set_mass(mass_t const& value);
  void set_mass(mass_t&& value);
  auto get_mass() const -> mass_t const&;

  void set_moment_of_inertia(ufloat const& value);
  void set_moment_of_inertia(ufloat&& value);
  auto get_moment_of_inertia() const -> ufloat const&;

private:
  std::string m_name;
  mass_t m_mass = {0.010000f};
  ufloat m_moment_of_inertia = {0};
};

typedef Poly<IUAV_Descriptor> Poly_IUAV_Descriptor;
struct IBus_Descriptor
{
public:
  virtual ~IBus_Descriptor() = default;
private:
};

typedef Poly<IBus_Descriptor> Poly_IBus_Descriptor;
struct INode_Descriptor
{
public:
  virtual ~INode_Descriptor() = default;
private:
};

typedef Poly<INode_Descriptor> Poly_INode_Descriptor;
struct INode_Config
{
public:
  virtual ~INode_Config() = default;
private:
};

typedef Poly<INode_Config> Poly_INode_Config;
struct Settings
{
public:
  struct Bus_Data
  {
  public:
    virtual ~Bus_Data() = default;
    void set_name(std::string const& value);
    void set_name(std::string&& value);
    auto get_name() const -> std::string const&;

    void set_type(std::string const& value);
    void set_type(std::string&& value);
    auto get_type() const -> std::string const&;

    void set_descriptor(Poly<const IBus_Descriptor> const& value);
    void set_descriptor(Poly<const IBus_Descriptor>&& value);
    auto get_descriptor() const -> Poly<const IBus_Descriptor> const&;
    auto get_descriptor() -> Poly<const IBus_Descriptor>&;

  private:
    std::string m_name;
    std::string m_type;
    Poly<const IBus_Descriptor> m_descriptor;
  };

  struct Node_Data
  {
  public:
    virtual ~Node_Data() = default;
    void set_name(std::string const& value);
    void set_name(std::string&& value);
    auto get_name() const -> std::string const&;

    void set_type(std::string const& value);
    void set_type(std::string&& value);
    auto get_type() const -> std::string const&;

    void set_descriptor(Poly<const INode_Descriptor> const& value);
    void set_descriptor(Poly<const INode_Descriptor>&& value);
    auto get_descriptor() const -> Poly<const INode_Descriptor> const&;
    auto get_descriptor() -> Poly<const INode_Descriptor>&;

    void set_config(Poly<const INode_Config> const& value);
    void set_config(Poly<const INode_Config>&& value);
    auto get_config() const -> Poly<const INode_Config> const&;
    auto get_config() -> Poly<const INode_Config>&;

    void set_input_paths(std::vector<std::string> const& value);
    void set_input_paths(std::vector<std::string>&& value);
    auto get_input_paths() const -> std::vector<std::string> const&;
    auto get_input_paths() -> std::vector<std::string>&;

  private:
    std::string m_name;
    std::string m_type;
    Poly<const INode_Descriptor> m_descriptor;
    Poly<const INode_Config> m_config;
    std::vector<std::string> m_input_paths;
  };

  virtual ~Settings() = default;
  void set_uav_descriptor(Poly<const IUAV_Descriptor> const& value);
  void set_uav_descriptor(Poly<const IUAV_Descriptor>&& value);
  auto get_uav_descriptor() const -> Poly<const IUAV_Descriptor> const&;
  auto get_uav_descriptor() -> Poly<const IUAV_Descriptor>&;

  void set_buses(std::vector<Settings::Bus_Data> const& value);
  void set_buses(std::vector<Settings::Bus_Data>&& value);
  auto get_buses() const -> std::vector<Settings::Bus_Data> const&;
  auto get_buses() -> std::vector<Settings::Bus_Data>&;

  void set_nodes(std::vector<Settings::Node_Data> const& value);
  void set_nodes(std::vector<Settings::Node_Data>&& value);
  auto get_nodes() const -> std::vector<Settings::Node_Data> const&;
  auto get_nodes() -> std::vector<Settings::Node_Data>&;

private:
  Poly<const IUAV_Descriptor> m_uav_descriptor;
  std::vector<Settings::Bus_Data> m_buses;
  std::vector<Settings::Node_Data> m_nodes;
};

struct Multirotor_Descriptor : public IUAV_Descriptor
{
public:
  typedef float height_t;
  typedef float radius_t;
  typedef float torque_t;
  typedef float thrust_t;
  typedef float acceleration_t;
  virtual ~Multirotor_Descriptor() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

  void set_mass(IUAV_Descriptor::mass_t const& value);
  void set_mass(IUAV_Descriptor::mass_t&& value);
  auto get_mass() const -> IUAV_Descriptor::mass_t const&;

  void set_moment_of_inertia(ufloat const& value);
  void set_moment_of_inertia(ufloat&& value);
  auto get_moment_of_inertia() const -> ufloat const&;

  void set_radius(radius_t const& value);
  void set_radius(radius_t&& value);
  auto get_radius() const -> radius_t const&;

  void set_height(height_t const& value);
  void set_height(height_t&& value);
  auto get_height() const -> height_t const&;

  void set_motor_z_torque(torque_t const& value);
  void set_motor_z_torque(torque_t&& value);
  auto get_motor_z_torque() const -> torque_t const&;

  void set_motor_thrust(thrust_t const& value);
  void set_motor_thrust(thrust_t&& value);
  auto get_motor_thrust() const -> thrust_t const&;

  void set_motor_acceleration(acceleration_t const& value);
  void set_motor_acceleration(acceleration_t&& value);
  auto get_motor_acceleration() const -> acceleration_t const&;

  void set_motor_deceleration(acceleration_t const& value);
  void set_motor_deceleration(acceleration_t&& value);
  auto get_motor_deceleration() const -> acceleration_t const&;

private:
  std::string m_name;
  IUAV_Descriptor::mass_t m_mass = {0.010000f};
  ufloat m_moment_of_inertia = {0};
  radius_t m_radius = {1.000000f};
  height_t m_height = {1.000000f};
  torque_t m_motor_z_torque = {1.000000f};
  thrust_t m_motor_thrust = {1.000000f};
  acceleration_t m_motor_acceleration = {10.000000f};
  acceleration_t m_motor_deceleration = {10.000000f};
};

struct Tri_Descriptor : public Multirotor_Descriptor
{
public:
  virtual ~Tri_Descriptor() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

  void set_mass(IUAV_Descriptor::mass_t const& value);
  void set_mass(IUAV_Descriptor::mass_t&& value);
  auto get_mass() const -> IUAV_Descriptor::mass_t const&;

  void set_moment_of_inertia(ufloat const& value);
  void set_moment_of_inertia(ufloat&& value);
  auto get_moment_of_inertia() const -> ufloat const&;

  void set_radius(Multirotor_Descriptor::radius_t const& value);
  void set_radius(Multirotor_Descriptor::radius_t&& value);
  auto get_radius() const -> Multirotor_Descriptor::radius_t const&;

  void set_height(Multirotor_Descriptor::height_t const& value);
  void set_height(Multirotor_Descriptor::height_t&& value);
  auto get_height() const -> Multirotor_Descriptor::height_t const&;

  void set_motor_z_torque(Multirotor_Descriptor::torque_t const& value);
  void set_motor_z_torque(Multirotor_Descriptor::torque_t&& value);
  auto get_motor_z_torque() const -> Multirotor_Descriptor::torque_t const&;

  void set_motor_thrust(Multirotor_Descriptor::thrust_t const& value);
  void set_motor_thrust(Multirotor_Descriptor::thrust_t&& value);
  auto get_motor_thrust() const -> Multirotor_Descriptor::thrust_t const&;

  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_acceleration() const -> Multirotor_Descriptor::acceleration_t const&;

  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_deceleration() const -> Multirotor_Descriptor::acceleration_t const&;

private:
  std::string m_name;
  IUAV_Descriptor::mass_t m_mass = {0.010000f};
  ufloat m_moment_of_inertia = {0};
  Multirotor_Descriptor::radius_t m_radius = {1.000000f};
  Multirotor_Descriptor::height_t m_height = {1.000000f};
  Multirotor_Descriptor::torque_t m_motor_z_torque = {1.000000f};
  Multirotor_Descriptor::thrust_t m_motor_thrust = {1.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_acceleration = {10.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_deceleration = {10.000000f};
};

struct Quad_Descriptor : public Multirotor_Descriptor
{
public:
  virtual ~Quad_Descriptor() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

  void set_mass(IUAV_Descriptor::mass_t const& value);
  void set_mass(IUAV_Descriptor::mass_t&& value);
  auto get_mass() const -> IUAV_Descriptor::mass_t const&;

  void set_moment_of_inertia(ufloat const& value);
  void set_moment_of_inertia(ufloat&& value);
  auto get_moment_of_inertia() const -> ufloat const&;

  void set_radius(Multirotor_Descriptor::radius_t const& value);
  void set_radius(Multirotor_Descriptor::radius_t&& value);
  auto get_radius() const -> Multirotor_Descriptor::radius_t const&;

  void set_height(Multirotor_Descriptor::height_t const& value);
  void set_height(Multirotor_Descriptor::height_t&& value);
  auto get_height() const -> Multirotor_Descriptor::height_t const&;

  void set_motor_z_torque(Multirotor_Descriptor::torque_t const& value);
  void set_motor_z_torque(Multirotor_Descriptor::torque_t&& value);
  auto get_motor_z_torque() const -> Multirotor_Descriptor::torque_t const&;

  void set_motor_thrust(Multirotor_Descriptor::thrust_t const& value);
  void set_motor_thrust(Multirotor_Descriptor::thrust_t&& value);
  auto get_motor_thrust() const -> Multirotor_Descriptor::thrust_t const&;

  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_acceleration() const -> Multirotor_Descriptor::acceleration_t const&;

  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_deceleration() const -> Multirotor_Descriptor::acceleration_t const&;

  void set_plus_configuration(bool const& value);
  void set_plus_configuration(bool&& value);
  auto get_plus_configuration() const -> bool const&;

private:
  std::string m_name;
  IUAV_Descriptor::mass_t m_mass = {0.010000f};
  ufloat m_moment_of_inertia = {0};
  Multirotor_Descriptor::radius_t m_radius = {1.000000f};
  Multirotor_Descriptor::height_t m_height = {1.000000f};
  Multirotor_Descriptor::torque_t m_motor_z_torque = {1.000000f};
  Multirotor_Descriptor::thrust_t m_motor_thrust = {1.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_acceleration = {10.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_deceleration = {10.000000f};
  bool m_plus_configuration = {false};
};

struct Hexa_Descriptor : public Multirotor_Descriptor
{
public:
  virtual ~Hexa_Descriptor() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

  void set_mass(IUAV_Descriptor::mass_t const& value);
  void set_mass(IUAV_Descriptor::mass_t&& value);
  auto get_mass() const -> IUAV_Descriptor::mass_t const&;

  void set_moment_of_inertia(ufloat const& value);
  void set_moment_of_inertia(ufloat&& value);
  auto get_moment_of_inertia() const -> ufloat const&;

  void set_radius(Multirotor_Descriptor::radius_t const& value);
  void set_radius(Multirotor_Descriptor::radius_t&& value);
  auto get_radius() const -> Multirotor_Descriptor::radius_t const&;

  void set_height(Multirotor_Descriptor::height_t const& value);
  void set_height(Multirotor_Descriptor::height_t&& value);
  auto get_height() const -> Multirotor_Descriptor::height_t const&;

  void set_motor_z_torque(Multirotor_Descriptor::torque_t const& value);
  void set_motor_z_torque(Multirotor_Descriptor::torque_t&& value);
  auto get_motor_z_torque() const -> Multirotor_Descriptor::torque_t const&;

  void set_motor_thrust(Multirotor_Descriptor::thrust_t const& value);
  void set_motor_thrust(Multirotor_Descriptor::thrust_t&& value);
  auto get_motor_thrust() const -> Multirotor_Descriptor::thrust_t const&;

  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_acceleration() const -> Multirotor_Descriptor::acceleration_t const&;

  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_deceleration() const -> Multirotor_Descriptor::acceleration_t const&;

  void set_plus_configuration(bool const& value);
  void set_plus_configuration(bool&& value);
  auto get_plus_configuration() const -> bool const&;

private:
  std::string m_name;
  IUAV_Descriptor::mass_t m_mass = {0.010000f};
  ufloat m_moment_of_inertia = {0};
  Multirotor_Descriptor::radius_t m_radius = {1.000000f};
  Multirotor_Descriptor::height_t m_height = {1.000000f};
  Multirotor_Descriptor::torque_t m_motor_z_torque = {1.000000f};
  Multirotor_Descriptor::thrust_t m_motor_thrust = {1.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_acceleration = {10.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_deceleration = {10.000000f};
  bool m_plus_configuration = {false};
};

struct Hexatri_Descriptor : public Multirotor_Descriptor
{
public:
  virtual ~Hexatri_Descriptor() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

  void set_mass(IUAV_Descriptor::mass_t const& value);
  void set_mass(IUAV_Descriptor::mass_t&& value);
  auto get_mass() const -> IUAV_Descriptor::mass_t const&;

  void set_moment_of_inertia(ufloat const& value);
  void set_moment_of_inertia(ufloat&& value);
  auto get_moment_of_inertia() const -> ufloat const&;

  void set_radius(Multirotor_Descriptor::radius_t const& value);
  void set_radius(Multirotor_Descriptor::radius_t&& value);
  auto get_radius() const -> Multirotor_Descriptor::radius_t const&;

  void set_height(Multirotor_Descriptor::height_t const& value);
  void set_height(Multirotor_Descriptor::height_t&& value);
  auto get_height() const -> Multirotor_Descriptor::height_t const&;

  void set_motor_z_torque(Multirotor_Descriptor::torque_t const& value);
  void set_motor_z_torque(Multirotor_Descriptor::torque_t&& value);
  auto get_motor_z_torque() const -> Multirotor_Descriptor::torque_t const&;

  void set_motor_thrust(Multirotor_Descriptor::thrust_t const& value);
  void set_motor_thrust(Multirotor_Descriptor::thrust_t&& value);
  auto get_motor_thrust() const -> Multirotor_Descriptor::thrust_t const&;

  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_acceleration() const -> Multirotor_Descriptor::acceleration_t const&;

  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_deceleration() const -> Multirotor_Descriptor::acceleration_t const&;

private:
  std::string m_name;
  IUAV_Descriptor::mass_t m_mass = {0.010000f};
  ufloat m_moment_of_inertia = {0};
  Multirotor_Descriptor::radius_t m_radius = {1.000000f};
  Multirotor_Descriptor::height_t m_height = {1.000000f};
  Multirotor_Descriptor::torque_t m_motor_z_torque = {1.000000f};
  Multirotor_Descriptor::thrust_t m_motor_thrust = {1.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_acceleration = {10.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_deceleration = {10.000000f};
};

struct Octo_Descriptor : public Multirotor_Descriptor
{
public:
  virtual ~Octo_Descriptor() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

  void set_mass(IUAV_Descriptor::mass_t const& value);
  void set_mass(IUAV_Descriptor::mass_t&& value);
  auto get_mass() const -> IUAV_Descriptor::mass_t const&;

  void set_moment_of_inertia(ufloat const& value);
  void set_moment_of_inertia(ufloat&& value);
  auto get_moment_of_inertia() const -> ufloat const&;

  void set_radius(Multirotor_Descriptor::radius_t const& value);
  void set_radius(Multirotor_Descriptor::radius_t&& value);
  auto get_radius() const -> Multirotor_Descriptor::radius_t const&;

  void set_height(Multirotor_Descriptor::height_t const& value);
  void set_height(Multirotor_Descriptor::height_t&& value);
  auto get_height() const -> Multirotor_Descriptor::height_t const&;

  void set_motor_z_torque(Multirotor_Descriptor::torque_t const& value);
  void set_motor_z_torque(Multirotor_Descriptor::torque_t&& value);
  auto get_motor_z_torque() const -> Multirotor_Descriptor::torque_t const&;

  void set_motor_thrust(Multirotor_Descriptor::thrust_t const& value);
  void set_motor_thrust(Multirotor_Descriptor::thrust_t&& value);
  auto get_motor_thrust() const -> Multirotor_Descriptor::thrust_t const&;

  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_acceleration() const -> Multirotor_Descriptor::acceleration_t const&;

  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_deceleration() const -> Multirotor_Descriptor::acceleration_t const&;

  void set_plus_configuration(bool const& value);
  void set_plus_configuration(bool&& value);
  auto get_plus_configuration() const -> bool const&;

private:
  std::string m_name;
  IUAV_Descriptor::mass_t m_mass = {0.010000f};
  ufloat m_moment_of_inertia = {0};
  Multirotor_Descriptor::radius_t m_radius = {1.000000f};
  Multirotor_Descriptor::height_t m_height = {1.000000f};
  Multirotor_Descriptor::torque_t m_motor_z_torque = {1.000000f};
  Multirotor_Descriptor::thrust_t m_motor_thrust = {1.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_acceleration = {10.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_deceleration = {10.000000f};
  bool m_plus_configuration = {false};
};

struct Octaquad_Descriptor : public Multirotor_Descriptor
{
public:
  virtual ~Octaquad_Descriptor() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

  void set_mass(IUAV_Descriptor::mass_t const& value);
  void set_mass(IUAV_Descriptor::mass_t&& value);
  auto get_mass() const -> IUAV_Descriptor::mass_t const&;

  void set_moment_of_inertia(ufloat const& value);
  void set_moment_of_inertia(ufloat&& value);
  auto get_moment_of_inertia() const -> ufloat const&;

  void set_radius(Multirotor_Descriptor::radius_t const& value);
  void set_radius(Multirotor_Descriptor::radius_t&& value);
  auto get_radius() const -> Multirotor_Descriptor::radius_t const&;

  void set_height(Multirotor_Descriptor::height_t const& value);
  void set_height(Multirotor_Descriptor::height_t&& value);
  auto get_height() const -> Multirotor_Descriptor::height_t const&;

  void set_motor_z_torque(Multirotor_Descriptor::torque_t const& value);
  void set_motor_z_torque(Multirotor_Descriptor::torque_t&& value);
  auto get_motor_z_torque() const -> Multirotor_Descriptor::torque_t const&;

  void set_motor_thrust(Multirotor_Descriptor::thrust_t const& value);
  void set_motor_thrust(Multirotor_Descriptor::thrust_t&& value);
  auto get_motor_thrust() const -> Multirotor_Descriptor::thrust_t const&;

  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_acceleration() const -> Multirotor_Descriptor::acceleration_t const&;

  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_deceleration() const -> Multirotor_Descriptor::acceleration_t const&;

  void set_plus_configuration(bool const& value);
  void set_plus_configuration(bool&& value);
  auto get_plus_configuration() const -> bool const&;

private:
  std::string m_name;
  IUAV_Descriptor::mass_t m_mass = {0.010000f};
  ufloat m_moment_of_inertia = {0};
  Multirotor_Descriptor::radius_t m_radius = {1.000000f};
  Multirotor_Descriptor::height_t m_height = {1.000000f};
  Multirotor_Descriptor::torque_t m_motor_z_torque = {1.000000f};
  Multirotor_Descriptor::thrust_t m_motor_thrust = {1.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_acceleration = {10.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_deceleration = {10.000000f};
  bool m_plus_configuration = {false};
};

struct Custom_Multirotor_Descriptor : public Multirotor_Descriptor
{
public:
  struct Motor
  {
  public:
    virtual ~Motor() = default;
    void set_position(vec3f const& value);
    void set_position(vec3f&& value);
    auto get_position() const -> vec3f const&;

    void set_thrust_vector(direction_t const& value);
    void set_thrust_vector(direction_t&& value);
    auto get_thrust_vector() const -> direction_t const&;

    void set_clockwise(bool const& value);
    void set_clockwise(bool&& value);
    auto get_clockwise() const -> bool const&;

  private:
    vec3f m_position = {0, 0, 0};
    direction_t m_thrust_vector = {0, 0, 1.000000f};
    bool m_clockwise = {false};
  };

  virtual ~Custom_Multirotor_Descriptor() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

  void set_mass(IUAV_Descriptor::mass_t const& value);
  void set_mass(IUAV_Descriptor::mass_t&& value);
  auto get_mass() const -> IUAV_Descriptor::mass_t const&;

  void set_moment_of_inertia(ufloat const& value);
  void set_moment_of_inertia(ufloat&& value);
  auto get_moment_of_inertia() const -> ufloat const&;

  void set_radius(Multirotor_Descriptor::radius_t const& value);
  void set_radius(Multirotor_Descriptor::radius_t&& value);
  auto get_radius() const -> Multirotor_Descriptor::radius_t const&;

  void set_height(Multirotor_Descriptor::height_t const& value);
  void set_height(Multirotor_Descriptor::height_t&& value);
  auto get_height() const -> Multirotor_Descriptor::height_t const&;

  void set_motor_z_torque(Multirotor_Descriptor::torque_t const& value);
  void set_motor_z_torque(Multirotor_Descriptor::torque_t&& value);
  auto get_motor_z_torque() const -> Multirotor_Descriptor::torque_t const&;

  void set_motor_thrust(Multirotor_Descriptor::thrust_t const& value);
  void set_motor_thrust(Multirotor_Descriptor::thrust_t&& value);
  auto get_motor_thrust() const -> Multirotor_Descriptor::thrust_t const&;

  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_acceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_acceleration() const -> Multirotor_Descriptor::acceleration_t const&;

  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t const& value);
  void set_motor_deceleration(Multirotor_Descriptor::acceleration_t&& value);
  auto get_motor_deceleration() const -> Multirotor_Descriptor::acceleration_t const&;

  void set_motors(std::vector<Custom_Multirotor_Descriptor::Motor> const& value);
  void set_motors(std::vector<Custom_Multirotor_Descriptor::Motor>&& value);
  auto get_motors() const -> std::vector<Custom_Multirotor_Descriptor::Motor> const&;
  auto get_motors() -> std::vector<Custom_Multirotor_Descriptor::Motor>&;

private:
  std::string m_name;
  IUAV_Descriptor::mass_t m_mass = {0.010000f};
  ufloat m_moment_of_inertia = {0};
  Multirotor_Descriptor::radius_t m_radius = {1.000000f};
  Multirotor_Descriptor::height_t m_height = {1.000000f};
  Multirotor_Descriptor::torque_t m_motor_z_torque = {1.000000f};
  Multirotor_Descriptor::thrust_t m_motor_thrust = {1.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_acceleration = {10.000000f};
  Multirotor_Descriptor::acceleration_t m_motor_deceleration = {10.000000f};
  std::vector<Custom_Multirotor_Descriptor::Motor> m_motors;
};

struct UART_Linux_Descriptor : public IBus_Descriptor
{
public:
  enum class baud_t
  {
    _9600 = 0,
    _19200 = 1,
    _38400 = 2,
    _57600 = 3,
    _115200 = 4,
    _230400 = 5,
  };

  virtual ~UART_Linux_Descriptor() = default;
  void set_dev(std::string const& value);
  void set_dev(std::string&& value);
  auto get_dev() const -> std::string const&;

  void set_baud(baud_t const& value);
  void set_baud(baud_t&& value);
  auto get_baud() const -> baud_t const&;

private:
  std::string m_dev = {"/dev/ttyAMA0"};
  baud_t m_baud = {UART_Linux_Descriptor::baud_t::_115200};
};

struct UART_BBang_Descriptor : public IBus_Descriptor
{
public:
  virtual ~UART_BBang_Descriptor() = default;
  void set_rx_pin(uint32_t const& value);
  void set_rx_pin(uint32_t&& value);
  auto get_rx_pin() const -> uint32_t const&;

  void set_baud(uint32_t const& value);
  void set_baud(uint32_t&& value);
  auto get_baud() const -> uint32_t const&;

  void set_invert(bool const& value);
  void set_invert(bool&& value);
  auto get_invert() const -> bool const&;

private:
  uint32_t m_rx_pin = {0};
  uint32_t m_baud = {115200};
  bool m_invert = {false};
};

struct I2C_BCM_Descriptor : public IBus_Descriptor
{
public:
  typedef uint32_t dev_t;
  typedef uint32_t baud_t;
  virtual ~I2C_BCM_Descriptor() = default;
  void set_dev(dev_t const& value);
  void set_dev(dev_t&& value);
  auto get_dev() const -> dev_t const&;

  void set_baud(baud_t const& value);
  void set_baud(baud_t&& value);
  auto get_baud() const -> baud_t const&;

private:
  dev_t m_dev = {0};
  baud_t m_baud = {400000};
};

struct I2C_Linux_Descriptor : public IBus_Descriptor
{
public:
  virtual ~I2C_Linux_Descriptor() = default;
  void set_dev(std::string const& value);
  void set_dev(std::string&& value);
  auto get_dev() const -> std::string const&;

private:
  std::string m_dev = {"/dev/i2c0"};
};

struct SPI_BCM_Descriptor : public IBus_Descriptor
{
public:
  typedef uint32_t dev_t;
  typedef uint32_t mode_t;
  typedef uint32_t speed_t;
  virtual ~SPI_BCM_Descriptor() = default;
  void set_dev(dev_t const& value);
  void set_dev(dev_t&& value);
  auto get_dev() const -> dev_t const&;

  void set_baud(uint32_t const& value);
  void set_baud(uint32_t&& value);
  auto get_baud() const -> uint32_t const&;

  void set_mode(mode_t const& value);
  void set_mode(mode_t&& value);
  auto get_mode() const -> mode_t const&;

  void set_speed(speed_t const& value);
  void set_speed(speed_t&& value);
  auto get_speed() const -> speed_t const&;

private:
  dev_t m_dev = {0};
  uint32_t m_baud = {1000000};
  mode_t m_mode = {0};
  speed_t m_speed = {1000000};
};

struct SPI_Linux_Descriptor : public IBus_Descriptor
{
public:
  typedef uint32_t speed_t;
  virtual ~SPI_Linux_Descriptor() = default;
  void set_dev(std::string const& value);
  void set_dev(std::string&& value);
  auto get_dev() const -> std::string const&;

  void set_speed(speed_t const& value);
  void set_speed(speed_t&& value);
  auto get_speed() const -> speed_t const&;

private:
  std::string m_dev = {"/dev/ttyAMA0"};
  speed_t m_speed = {1000000};
};

struct P_Controller_Descriptor
{
public:
  virtual ~P_Controller_Descriptor() = default;
  void set_kp(ufloat const& value);
  void set_kp(ufloat&& value);
  auto get_kp() const -> ufloat const&;

private:
  ufloat m_kp = {0};
};

struct PI_Controller_Descriptor : public P_Controller_Descriptor
{
public:
  virtual ~PI_Controller_Descriptor() = default;
  void set_kp(ufloat const& value);
  void set_kp(ufloat&& value);
  auto get_kp() const -> ufloat const&;

  void set_ki(ufloat const& value);
  void set_ki(ufloat&& value);
  auto get_ki() const -> ufloat const&;

  void set_max_i(ufloat const& value);
  void set_max_i(ufloat&& value);
  auto get_max_i() const -> ufloat const&;

private:
  ufloat m_kp = {0};
  ufloat m_ki = {0};
  ufloat m_max_i = {0};
};

struct PD_Controller_Descriptor : public P_Controller_Descriptor
{
public:
  virtual ~PD_Controller_Descriptor() = default;
  void set_kp(ufloat const& value);
  void set_kp(ufloat&& value);
  auto get_kp() const -> ufloat const&;

  void set_kd(ufloat const& value);
  void set_kd(ufloat&& value);
  auto get_kd() const -> ufloat const&;

  void set_d_filter(muf_t const& value);
  void set_d_filter(muf_t&& value);
  auto get_d_filter() const -> muf_t const&;

private:
  ufloat m_kp = {0};
  ufloat m_kd = {0};
  muf_t m_d_filter = {0};
};

struct PID_Controller_Descriptor : public PD_Controller_Descriptor
{
public:
  virtual ~PID_Controller_Descriptor() = default;
  void set_kp(ufloat const& value);
  void set_kp(ufloat&& value);
  auto get_kp() const -> ufloat const&;

  void set_kd(ufloat const& value);
  void set_kd(ufloat&& value);
  auto get_kd() const -> ufloat const&;

  void set_d_filter(muf_t const& value);
  void set_d_filter(muf_t&& value);
  auto get_d_filter() const -> muf_t const&;

  void set_ki(ufloat const& value);
  void set_ki(ufloat&& value);
  auto get_ki() const -> ufloat const&;

  void set_max_i(ufloat const& value);
  void set_max_i(ufloat&& value);
  auto get_max_i() const -> ufloat const&;

private:
  ufloat m_kp = {0};
  ufloat m_kd = {0};
  muf_t m_d_filter = {0};
  ufloat m_ki = {0};
  ufloat m_max_i = {0};
};

struct ADC_Ammeter_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~ADC_Ammeter_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {100};
};

struct ADC_Ammeter_Config : public INode_Config
{
public:
  virtual ~ADC_Ammeter_Config() = default;
  void set_scale(float const& value);
  void set_scale(float&& value);
  auto get_scale() const -> float const&;

  void set_bias(float const& value);
  void set_bias(float&& value);
  auto get_bias() const -> float const&;

private:
  float m_scale = {1.000000f};
  float m_bias = {0};
};

struct ADC_Voltmeter_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~ADC_Voltmeter_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {100};
};

struct ADC_Voltmeter_Config : public INode_Config
{
public:
  virtual ~ADC_Voltmeter_Config() = default;
  void set_scale(float const& value);
  void set_scale(float&& value);
  auto get_scale() const -> float const&;

  void set_bias(float const& value);
  void set_bias(float&& value);
  auto get_bias() const -> float const&;

private:
  float m_scale = {1.000000f};
  float m_bias = {0};
};

struct ADS1115_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~ADS1115_Descriptor() = default;
  void set_bus(std::string const& value);
  void set_bus(std::string&& value);
  auto get_bus() const -> std::string const&;

  void set_adc0_rate(uint32_t const& value);
  void set_adc0_rate(uint32_t&& value);
  auto get_adc0_rate() const -> uint32_t const&;

  void set_adc1_rate(uint32_t const& value);
  void set_adc1_rate(uint32_t&& value);
  auto get_adc1_rate() const -> uint32_t const&;

  void set_adc2_rate(uint32_t const& value);
  void set_adc2_rate(uint32_t&& value);
  auto get_adc2_rate() const -> uint32_t const&;

  void set_adc3_rate(uint32_t const& value);
  void set_adc3_rate(uint32_t&& value);
  auto get_adc3_rate() const -> uint32_t const&;

private:
  std::string m_bus;
  uint32_t m_adc0_rate = {200};
  uint32_t m_adc1_rate = {200};
  uint32_t m_adc2_rate = {200};
  uint32_t m_adc3_rate = {200};
};

struct ADS1115_Config : public INode_Config
{
public:
  virtual ~ADS1115_Config() = default;
private:
};

struct AVRADC_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~AVRADC_Descriptor() = default;
  void set_bus(std::string const& value);
  void set_bus(std::string&& value);
  auto get_bus() const -> std::string const&;

  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  std::string m_bus;
  uint32_t m_rate = {200};
};

struct AVRADC_Config : public INode_Config
{
public:
  virtual ~AVRADC_Config() = default;
private:
};

struct Comp_AHRS_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Comp_AHRS_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {200};
};

struct Comp_AHRS_Config : public INode_Config
{
public:
  virtual ~Comp_AHRS_Config() = default;
  void set_drift_correction_factor(muf_t const& value);
  void set_drift_correction_factor(muf_t&& value);
  auto get_drift_correction_factor() const -> muf_t const&;

private:
  muf_t m_drift_correction_factor = {0.300000f};
};

struct Combiner_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Combiner_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {200};
};

struct Combiner_Config : public INode_Config
{
public:
  virtual ~Combiner_Config() = default;
private:
};

struct Gravity_Filter_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Gravity_Filter_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {1};
};

struct Gravity_Filter_Config : public INode_Config
{
public:
  virtual ~Gravity_Filter_Config() = default;
private:
};

struct KF_ECEF_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~KF_ECEF_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {1};
};

struct KF_ECEF_Config : public INode_Config
{
public:
  virtual ~KF_ECEF_Config() = default;
  void set_gps_position_lag(ufloat const& value);
  void set_gps_position_lag(ufloat&& value);
  auto get_gps_position_lag() const -> ufloat const&;

  void set_gps_position_accuracy(ufloat const& value);
  void set_gps_position_accuracy(ufloat&& value);
  auto get_gps_position_accuracy() const -> ufloat const&;

  void set_gps_velocity_lag(ufloat const& value);
  void set_gps_velocity_lag(ufloat&& value);
  auto get_gps_velocity_lag() const -> ufloat const&;

  void set_gps_velocity_accuracy(ufloat const& value);
  void set_gps_velocity_accuracy(ufloat&& value);
  auto get_gps_velocity_accuracy() const -> ufloat const&;

  void set_acceleration_lag(ufloat const& value);
  void set_acceleration_lag(ufloat&& value);
  auto get_acceleration_lag() const -> ufloat const&;

  void set_acceleration_accuracy(ufloat const& value);
  void set_acceleration_accuracy(ufloat&& value);
  auto get_acceleration_accuracy() const -> ufloat const&;

private:
  ufloat m_gps_position_lag = {0};
  ufloat m_gps_position_accuracy = {2.000000f};
  ufloat m_gps_velocity_lag = {0};
  ufloat m_gps_velocity_accuracy = {0.200000f};
  ufloat m_acceleration_lag = {0};
  ufloat m_acceleration_accuracy = {2.000000f};
};

struct ENU_Frame_System_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~ENU_Frame_System_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {1};
};

struct ENU_Frame_System_Config : public INode_Config
{
public:
  virtual ~ENU_Frame_System_Config() = default;
private:
};

struct LPF_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~LPF_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {1};
};

struct LPF_Config : public INode_Config
{
public:
  typedef uint32_t poles_t;
  virtual ~LPF_Config() = default;
  void set_poles(poles_t const& value);
  void set_poles(poles_t&& value);
  auto get_poles() const -> poles_t const&;

  void set_cutoff_frequency(ufloat const& value);
  void set_cutoff_frequency(ufloat&& value);
  auto get_cutoff_frequency() const -> ufloat const&;

private:
  poles_t m_poles = {1};
  ufloat m_cutoff_frequency = {1.000000f};
};

struct MaxSonar_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~MaxSonar_Descriptor() = default;
  void set_bus(std::string const& value);
  void set_bus(std::string&& value);
  auto get_bus() const -> std::string const&;

  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  std::string m_bus;
  uint32_t m_rate = {1};
};

struct MaxSonar_Config : public INode_Config
{
public:
  virtual ~MaxSonar_Config() = default;
  void set_direction(direction_t const& value);
  void set_direction(direction_t&& value);
  auto get_direction() const -> direction_t const&;

  void set_max_distance(ufloat const& value);
  void set_max_distance(ufloat&& value);
  auto get_max_distance() const -> ufloat const&;

  void set_min_distance(ufloat const& value);
  void set_min_distance(ufloat&& value);
  auto get_min_distance() const -> ufloat const&;

private:
  direction_t m_direction = {0, 0, 0};
  ufloat m_max_distance = {10.000000f};
  ufloat m_min_distance = {0.200000f};
};

struct Motor_Mixer_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Motor_Mixer_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {1};
};

struct Motor_Mixer_Config : public INode_Config
{
public:
  virtual ~Motor_Mixer_Config() = default;
  void set_armed_thrust(ufloat const& value);
  void set_armed_thrust(ufloat&& value);
  auto get_armed_thrust() const -> ufloat const&;

private:
  ufloat m_armed_thrust = {0};
};

struct Acceleration_Calibration_Point
{
public:
  virtual ~Acceleration_Calibration_Point() = default;
  void set_temperature(float const& value);
  void set_temperature(float&& value);
  auto get_temperature() const -> float const&;

  void set_bias(vec3f const& value);
  void set_bias(vec3f&& value);
  auto get_bias() const -> vec3f const&;

  void set_scale(vec3f const& value);
  void set_scale(vec3f&& value);
  auto get_scale() const -> vec3f const&;

private:
  float m_temperature = {0};
  vec3f m_bias = {0, 0, 0};
  vec3f m_scale = {1.000000f, 1.000000f, 1.000000f};
};

struct Angular_Velocity_Calibration_Point
{
public:
  virtual ~Angular_Velocity_Calibration_Point() = default;
  void set_temperature(float const& value);
  void set_temperature(float&& value);
  auto get_temperature() const -> float const&;

  void set_bias(vec3f const& value);
  void set_bias(vec3f&& value);
  auto get_bias() const -> vec3f const&;

private:
  float m_temperature = {0};
  vec3f m_bias = {0, 0, 0};
};

struct Magnetic_Field_Calibration_Point
{
public:
  virtual ~Magnetic_Field_Calibration_Point() = default;
  void set_temperature(float const& value);
  void set_temperature(float&& value);
  auto get_temperature() const -> float const&;

  void set_bias(vec3f const& value);
  void set_bias(vec3f&& value);
  auto get_bias() const -> vec3f const&;

  void set_scale(vec3f const& value);
  void set_scale(vec3f&& value);
  auto get_scale() const -> vec3f const&;

private:
  float m_temperature = {0};
  vec3f m_bias = {0, 0, 0};
  vec3f m_scale = {1.000000f, 1.000000f, 1.000000f};
};

struct MPU9250_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  enum class imu_rate_t
  {
    _250 = 250,
    _500 = 500,
    _1000 = 1000,
  };

  enum class acceleration_range_t
  {
    _2 = 2,
    _4 = 4,
    _8 = 8,
    _16 = 16,
  };

  enum class angular_velocity_range_t
  {
    _250 = 250,
    _500 = 500,
    _1000 = 1000,
    _2000 = 2000,
  };

  virtual ~MPU9250_Descriptor() = default;
  void set_bus(std::string const& value);
  void set_bus(std::string&& value);
  auto get_bus() const -> std::string const&;

  void set_imu_rate(imu_rate_t const& value);
  void set_imu_rate(imu_rate_t&& value);
  auto get_imu_rate() const -> imu_rate_t const&;

  void set_magnetometer_rate(uint32_t const& value);
  void set_magnetometer_rate(uint32_t&& value);
  auto get_magnetometer_rate() const -> uint32_t const&;

  void set_thermometer_rate(uint32_t const& value);
  void set_thermometer_rate(uint32_t&& value);
  auto get_thermometer_rate() const -> uint32_t const&;

  void set_acceleration_range(acceleration_range_t const& value);
  void set_acceleration_range(acceleration_range_t&& value);
  auto get_acceleration_range() const -> acceleration_range_t const&;

  void set_angular_velocity_range(angular_velocity_range_t const& value);
  void set_angular_velocity_range(angular_velocity_range_t&& value);
  auto get_angular_velocity_range() const -> angular_velocity_range_t const&;

private:
  std::string m_bus;
  imu_rate_t m_imu_rate = {MPU9250_Descriptor::imu_rate_t::_1000};
  uint32_t m_magnetometer_rate = {100};
  uint32_t m_thermometer_rate = {10};
  acceleration_range_t m_acceleration_range = {MPU9250_Descriptor::acceleration_range_t::_8};
  angular_velocity_range_t m_angular_velocity_range = {MPU9250_Descriptor::angular_velocity_range_t::_500};
};

struct MPU9250_Config : public INode_Config
{
public:
  struct Calibration
  {
  public:
    virtual ~Calibration() = default;
    void set_acceleration(std::vector<Acceleration_Calibration_Point> const& value);
    void set_acceleration(std::vector<Acceleration_Calibration_Point>&& value);
    auto get_acceleration() const -> std::vector<Acceleration_Calibration_Point> const&;
    auto get_acceleration() -> std::vector<Acceleration_Calibration_Point>&;

    void set_angular_velocity(std::vector<Angular_Velocity_Calibration_Point> const& value);
    void set_angular_velocity(std::vector<Angular_Velocity_Calibration_Point>&& value);
    auto get_angular_velocity() const -> std::vector<Angular_Velocity_Calibration_Point> const&;
    auto get_angular_velocity() -> std::vector<Angular_Velocity_Calibration_Point>&;

    void set_magnetic_field(std::vector<Magnetic_Field_Calibration_Point> const& value);
    void set_magnetic_field(std::vector<Magnetic_Field_Calibration_Point>&& value);
    auto get_magnetic_field() const -> std::vector<Magnetic_Field_Calibration_Point> const&;
    auto get_magnetic_field() -> std::vector<Magnetic_Field_Calibration_Point>&;

  private:
    std::vector<Acceleration_Calibration_Point> m_acceleration;
    std::vector<Angular_Velocity_Calibration_Point> m_angular_velocity;
    std::vector<Magnetic_Field_Calibration_Point> m_magnetic_field;
  };

  virtual ~MPU9250_Config() = default;
  void set_rotation(euler3_t const& value);
  void set_rotation(euler3_t&& value);
  auto get_rotation() const -> euler3_t const&;

  void set_calibration(Calibration const& value);
  void set_calibration(Calibration&& value);
  auto get_calibration() const -> Calibration const&;
  auto get_calibration() -> Calibration&;

private:
  euler3_t m_rotation = {0, 0, 0};
  Calibration m_calibration;
};

struct MS5611_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~MS5611_Descriptor() = default;
  void set_bus(std::string const& value);
  void set_bus(std::string&& value);
  auto get_bus() const -> std::string const&;

  void set_pressure_rate(uint32_t const& value);
  void set_pressure_rate(uint32_t&& value);
  auto get_pressure_rate() const -> uint32_t const&;

  void set_temperature_rate(uint32_t const& value);
  void set_temperature_rate(uint32_t&& value);
  auto get_temperature_rate() const -> uint32_t const&;

private:
  std::string m_bus;
  uint32_t m_pressure_rate = {100};
  uint32_t m_temperature_rate = {10};
};

struct MS5611_Config : public INode_Config
{
public:
  virtual ~MS5611_Config() = default;
private:
};

struct Multirotor_Brain_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Multirotor_Brain_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

  void set_commands_rate(uint32_t const& value);
  void set_commands_rate(uint32_t&& value);
  auto get_commands_rate() const -> uint32_t const&;

  void set_state_rate(uint32_t const& value);
  void set_state_rate(uint32_t&& value);
  auto get_state_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {100};
  uint32_t m_commands_rate = {30};
  uint32_t m_state_rate = {30};
};

struct Multirotor_Brain_Config : public INode_Config
{
public:
  struct Horizontal_Angle
  {
  public:
    typedef float speed_deg_t;
    typedef PID_Controller_Descriptor Combined_XY_PIDs;
    struct Separate_XY_PIDs
    {
    public:
      virtual ~Separate_XY_PIDs() = default;
      void set_x_pid(PID_Controller_Descriptor const& value);
      void set_x_pid(PID_Controller_Descriptor&& value);
      auto get_x_pid() const -> PID_Controller_Descriptor const&;
      auto get_x_pid() -> PID_Controller_Descriptor&;

      void set_y_pid(PID_Controller_Descriptor const& value);
      void set_y_pid(PID_Controller_Descriptor&& value);
      auto get_y_pid() const -> PID_Controller_Descriptor const&;
      auto get_y_pid() -> PID_Controller_Descriptor&;

    private:
      PID_Controller_Descriptor m_x_pid;
      PID_Controller_Descriptor m_y_pid;
    };

    virtual ~Horizontal_Angle() = default;
    void set_max_speed_deg(speed_deg_t const& value);
    void set_max_speed_deg(speed_deg_t&& value);
    auto get_max_speed_deg() const -> speed_deg_t const&;

    void set_xy_pids(boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const& value);
    void set_xy_pids(boost::variant<Combined_XY_PIDs,Separate_XY_PIDs>&& value);
    auto get_xy_pids() const -> boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const&;
    auto get_xy_pids() -> boost::variant<Combined_XY_PIDs,Separate_XY_PIDs>&;

  private:
    speed_deg_t m_max_speed_deg = {180.000000f};
    boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> m_xy_pids;
  };

  struct Horizontal_Position
  {
  public:
    typedef float speed_t;
    virtual ~Horizontal_Position() = default;
    void set_max_speed(speed_t const& value);
    void set_max_speed(speed_t&& value);
    auto get_max_speed() const -> speed_t const&;

    void set_lpf(LPF_Config const& value);
    void set_lpf(LPF_Config&& value);
    auto get_lpf() const -> LPF_Config const&;
    auto get_lpf() -> LPF_Config&;

    void set_velocity_pi(PI_Controller_Descriptor const& value);
    void set_velocity_pi(PI_Controller_Descriptor&& value);
    auto get_velocity_pi() const -> PI_Controller_Descriptor const&;
    auto get_velocity_pi() -> PI_Controller_Descriptor&;

    void set_position_p(P_Controller_Descriptor const& value);
    void set_position_p(P_Controller_Descriptor&& value);
    auto get_position_p() const -> P_Controller_Descriptor const&;
    auto get_position_p() -> P_Controller_Descriptor&;

  private:
    speed_t m_max_speed = {2.000000f};
    LPF_Config m_lpf;
    PI_Controller_Descriptor m_velocity_pi;
    P_Controller_Descriptor m_position_p;
  };

  struct Yaw_Angle
  {
  public:
    typedef float speed_deg_t;
    virtual ~Yaw_Angle() = default;
    void set_max_speed_deg(speed_deg_t const& value);
    void set_max_speed_deg(speed_deg_t&& value);
    auto get_max_speed_deg() const -> speed_deg_t const&;

    void set_pid(PID_Controller_Descriptor const& value);
    void set_pid(PID_Controller_Descriptor&& value);
    auto get_pid() const -> PID_Controller_Descriptor const&;
    auto get_pid() -> PID_Controller_Descriptor&;

  private:
    speed_deg_t m_max_speed_deg = {180.000000f};
    PID_Controller_Descriptor m_pid;
  };

  struct Altitude
  {
  public:
    typedef float speed_t;
    virtual ~Altitude() = default;
    void set_max_speed(speed_t const& value);
    void set_max_speed(speed_t&& value);
    auto get_max_speed() const -> speed_t const&;

    void set_lpf(LPF_Config const& value);
    void set_lpf(LPF_Config&& value);
    auto get_lpf() const -> LPF_Config const&;
    auto get_lpf() -> LPF_Config&;

    void set_speed_pi(PI_Controller_Descriptor const& value);
    void set_speed_pi(PI_Controller_Descriptor&& value);
    auto get_speed_pi() const -> PI_Controller_Descriptor const&;
    auto get_speed_pi() -> PI_Controller_Descriptor&;

    void set_position_p(P_Controller_Descriptor const& value);
    void set_position_p(P_Controller_Descriptor&& value);
    auto get_position_p() const -> P_Controller_Descriptor const&;
    auto get_position_p() -> P_Controller_Descriptor&;

  private:
    speed_t m_max_speed = {2.000000f};
    LPF_Config m_lpf;
    PI_Controller_Descriptor m_speed_pi;
    P_Controller_Descriptor m_position_p;
  };

  virtual ~Multirotor_Brain_Config() = default;
  void set_max_thrust(ufloat const& value);
  void set_max_thrust(ufloat&& value);
  auto get_max_thrust() const -> ufloat const&;

  void set_min_thrust(ufloat const& value);
  void set_min_thrust(ufloat&& value);
  auto get_min_thrust() const -> ufloat const&;

  void set_horizontal_angle(Horizontal_Angle const& value);
  void set_horizontal_angle(Horizontal_Angle&& value);
  auto get_horizontal_angle() const -> Horizontal_Angle const&;
  auto get_horizontal_angle() -> Horizontal_Angle&;

  void set_horizontal_position(Horizontal_Position const& value);
  void set_horizontal_position(Horizontal_Position&& value);
  auto get_horizontal_position() const -> Horizontal_Position const&;
  auto get_horizontal_position() -> Horizontal_Position&;

  void set_yaw_angle(Yaw_Angle const& value);
  void set_yaw_angle(Yaw_Angle&& value);
  auto get_yaw_angle() const -> Yaw_Angle const&;
  auto get_yaw_angle() -> Yaw_Angle&;

  void set_altitude(Altitude const& value);
  void set_altitude(Altitude&& value);
  auto get_altitude() const -> Altitude const&;
  auto get_altitude() -> Altitude&;

private:
  ufloat m_max_thrust = {2.000000f};
  ufloat m_min_thrust = {1.000000f};
  Horizontal_Angle m_horizontal_angle;
  Horizontal_Position m_horizontal_position;
  Yaw_Angle m_yaw_angle;
  Altitude m_altitude;
};

struct Multirotor_Pilot_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Multirotor_Pilot_Descriptor() = default;
  void set_commands_rate(uint32_t const& value);
  void set_commands_rate(uint32_t&& value);
  auto get_commands_rate() const -> uint32_t const&;

  void set_state_rate(uint32_t const& value);
  void set_state_rate(uint32_t&& value);
  auto get_state_rate() const -> uint32_t const&;

  void set_video_rate(uint32_t const& value);
  void set_video_rate(uint32_t&& value);
  auto get_video_rate() const -> uint32_t const&;

private:
  uint32_t m_commands_rate = {30};
  uint32_t m_state_rate = {30};
  uint32_t m_video_rate = {30};
};

struct Multirotor_Pilot_Config : public INode_Config
{
public:
  virtual ~Multirotor_Pilot_Config() = default;
private:
};

struct Multirotor_Simulator_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Multirotor_Simulator_Descriptor() = default;
  void set_throttle_rate(uint32_t const& value);
  void set_throttle_rate(uint32_t&& value);
  auto get_throttle_rate() const -> uint32_t const&;

  void set_acceleration_rate(uint32_t const& value);
  void set_acceleration_rate(uint32_t&& value);
  auto get_acceleration_rate() const -> uint32_t const&;

  void set_angular_velocity_rate(uint32_t const& value);
  void set_angular_velocity_rate(uint32_t&& value);
  auto get_angular_velocity_rate() const -> uint32_t const&;

  void set_magnetic_field_rate(uint32_t const& value);
  void set_magnetic_field_rate(uint32_t&& value);
  auto get_magnetic_field_rate() const -> uint32_t const&;

  void set_pressure_rate(uint32_t const& value);
  void set_pressure_rate(uint32_t&& value);
  auto get_pressure_rate() const -> uint32_t const&;

  void set_temperature_rate(uint32_t const& value);
  void set_temperature_rate(uint32_t&& value);
  auto get_temperature_rate() const -> uint32_t const&;

  void set_distance_rate(uint32_t const& value);
  void set_distance_rate(uint32_t&& value);
  auto get_distance_rate() const -> uint32_t const&;

  void set_gps_rate(uint32_t const& value);
  void set_gps_rate(uint32_t&& value);
  auto get_gps_rate() const -> uint32_t const&;

private:
  uint32_t m_throttle_rate = {100};
  uint32_t m_acceleration_rate = {1000};
  uint32_t m_angular_velocity_rate = {1000};
  uint32_t m_magnetic_field_rate = {100};
  uint32_t m_pressure_rate = {100};
  uint32_t m_temperature_rate = {10};
  uint32_t m_distance_rate = {12};
  uint32_t m_gps_rate = {5};
};

struct Multirotor_Simulator_Config : public INode_Config
{
public:
  struct Noise
  {
  public:
    virtual ~Noise() = default;
    void set_gps_position(ufloat const& value);
    void set_gps_position(ufloat&& value);
    auto get_gps_position() const -> ufloat const&;

    void set_gps_velocity(ufloat const& value);
    void set_gps_velocity(ufloat&& value);
    auto get_gps_velocity() const -> ufloat const&;

    void set_gps_pacc(ufloat const& value);
    void set_gps_pacc(ufloat&& value);
    auto get_gps_pacc() const -> ufloat const&;

    void set_gps_vacc(ufloat const& value);
    void set_gps_vacc(ufloat&& value);
    auto get_gps_vacc() const -> ufloat const&;

    void set_acceleration(ufloat const& value);
    void set_acceleration(ufloat&& value);
    auto get_acceleration() const -> ufloat const&;

    void set_angular_velocity(ufloat const& value);
    void set_angular_velocity(ufloat&& value);
    auto get_angular_velocity() const -> ufloat const&;

    void set_magnetic_field(ufloat const& value);
    void set_magnetic_field(ufloat&& value);
    auto get_magnetic_field() const -> ufloat const&;

    void set_pressure(ufloat const& value);
    void set_pressure(ufloat&& value);
    auto get_pressure() const -> ufloat const&;

    void set_temperature(ufloat const& value);
    void set_temperature(ufloat&& value);
    auto get_temperature() const -> ufloat const&;

    void set_ground_distance(ufloat const& value);
    void set_ground_distance(ufloat&& value);
    auto get_ground_distance() const -> ufloat const&;

  private:
    ufloat m_gps_position = {2.000000f};
    ufloat m_gps_velocity = {0.200000f};
    ufloat m_gps_pacc = {0.500000f};
    ufloat m_gps_vacc = {0.100000f};
    ufloat m_acceleration = {0.010000f};
    ufloat m_angular_velocity = {0.010000f};
    ufloat m_magnetic_field = {0.100000f};
    ufloat m_pressure = {0.100000f};
    ufloat m_temperature = {0.100000f};
    ufloat m_ground_distance = {0.010000f};
  };

  virtual ~Multirotor_Simulator_Config() = default;
  void set_simulation_enabled(bool const& value);
  void set_simulation_enabled(bool&& value);
  auto get_simulation_enabled() const -> bool const&;

  void set_drag_enabled(bool const& value);
  void set_drag_enabled(bool&& value);
  auto get_drag_enabled() const -> bool const&;

  void set_ground_enabled(bool const& value);
  void set_ground_enabled(bool&& value);
  auto get_ground_enabled() const -> bool const&;

  void set_gravity_enabled(bool const& value);
  void set_gravity_enabled(bool&& value);
  auto get_gravity_enabled() const -> bool const&;

  void set_noise(Noise const& value);
  void set_noise(Noise&& value);
  auto get_noise() const -> Noise const&;
  auto get_noise() -> Noise&;

private:
  bool m_simulation_enabled = {true};
  bool m_drag_enabled = {true};
  bool m_ground_enabled = {true};
  bool m_gravity_enabled = {true};
  Noise m_noise;
};

struct Oscillator_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  typedef int32_t component_count_t;
  virtual ~Oscillator_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

  void set_component_count(component_count_t const& value);
  void set_component_count(component_count_t&& value);
  auto get_component_count() const -> component_count_t const&;

private:
  uint32_t m_rate = {30};
  component_count_t m_component_count = {1};
};

struct Oscillator_Config : public INode_Config
{
public:
  struct Component
  {
  public:
    virtual ~Component() = default;
    void set_frequency(ufloat const& value);
    void set_frequency(ufloat&& value);
    auto get_frequency() const -> ufloat const&;

    void set_amplitude(ufloat const& value);
    void set_amplitude(ufloat&& value);
    auto get_amplitude() const -> ufloat const&;

    void set_square(bool const& value);
    void set_square(bool&& value);
    auto get_square() const -> bool const&;

  private:
    ufloat m_frequency = {1.000000f};
    ufloat m_amplitude = {1.000000f};
    bool m_square = {false};
  };

  virtual ~Oscillator_Config() = default;
  void set_amplitude(ufloat const& value);
  void set_amplitude(ufloat&& value);
  auto get_amplitude() const -> ufloat const&;

  void set_noise(ufloat const& value);
  void set_noise(ufloat&& value);
  auto get_noise() const -> ufloat const&;

  void set_components(std::vector<Oscillator_Config::Component> const& value);
  void set_components(std::vector<Oscillator_Config::Component>&& value);
  auto get_components() const -> std::vector<Oscillator_Config::Component> const&;
  auto get_components() -> std::vector<Oscillator_Config::Component>&;

private:
  ufloat m_amplitude = {1.000000f};
  ufloat m_noise = {0};
  std::vector<Oscillator_Config::Component> m_components;
};

struct PCA9685_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  struct Channel
  {
  public:
    virtual ~Channel() = default;
    void set_enabled(bool const& value);
    void set_enabled(bool&& value);
    auto get_enabled() const -> bool const&;

    void set_servo_signal(bool const& value);
    void set_servo_signal(bool&& value);
    auto get_servo_signal() const -> bool const&;

  private:
    bool m_enabled = {false};
    bool m_servo_signal = {false};
  };

  virtual ~PCA9685_Descriptor() = default;
  void set_bus(std::string const& value);
  void set_bus(std::string&& value);
  auto get_bus() const -> std::string const&;

  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

  void set_address(uint8_t const& value);
  void set_address(uint8_t&& value);
  auto get_address() const -> uint8_t const&;

  void set_channels(std::vector<PCA9685_Descriptor::Channel> const& value);
  void set_channels(std::vector<PCA9685_Descriptor::Channel>&& value);
  auto get_channels() const -> std::vector<PCA9685_Descriptor::Channel> const&;
  auto get_channels() -> std::vector<PCA9685_Descriptor::Channel>&;

private:
  std::string m_bus;
  uint32_t m_rate = {30};
  uint8_t m_address = {64};
  std::vector<PCA9685_Descriptor::Channel> m_channels;
};

struct PCA9685_Config : public INode_Config
{
public:
  struct IChannel
  {
  public:
    virtual ~IChannel() = default;
  private:
  };

  struct Servo_Channel : public PCA9685_Config::IChannel
  {
  public:
    virtual ~Servo_Channel() = default;
    void set_min(servo_signal_t const& value);
    void set_min(servo_signal_t&& value);
    auto get_min() const -> servo_signal_t const&;

    void set_max(servo_signal_t const& value);
    void set_max(servo_signal_t&& value);
    auto get_max() const -> servo_signal_t const&;

  private:
    servo_signal_t m_min = {1.000000f};
    servo_signal_t m_max = {2.000000f};
  };

  struct PWM_Channel : public PCA9685_Config::IChannel
  {
  public:
    virtual ~PWM_Channel() = default;
    void set_min(pwm_signal_t const& value);
    void set_min(pwm_signal_t&& value);
    auto get_min() const -> pwm_signal_t const&;

    void set_max(pwm_signal_t const& value);
    void set_max(pwm_signal_t&& value);
    auto get_max() const -> pwm_signal_t const&;

  private:
    pwm_signal_t m_min = {0};
    pwm_signal_t m_max = {1.000000f};
  };

  virtual ~PCA9685_Config() = default;
  void set_channels(std::vector<Poly<PCA9685_Config::IChannel>> const& value);
  void set_channels(std::vector<Poly<PCA9685_Config::IChannel>>&& value);
  auto get_channels() const -> std::vector<Poly<PCA9685_Config::IChannel>> const&;
  auto get_channels() -> std::vector<Poly<PCA9685_Config::IChannel>>&;

private:
  std::vector<Poly<PCA9685_Config::IChannel>> m_channels;
};

struct PIGPIO_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  struct Channel
  {
  public:
    virtual ~Channel() = default;
    void set_enabled(bool const& value);
    void set_enabled(bool&& value);
    auto get_enabled() const -> bool const&;

    void set_servo_signal(bool const& value);
    void set_servo_signal(bool&& value);
    auto get_servo_signal() const -> bool const&;

    void set_rate(uint32_t const& value);
    void set_rate(uint32_t&& value);
    auto get_rate() const -> uint32_t const&;

  private:
    bool m_enabled = {false};
    bool m_servo_signal = {false};
    uint32_t m_rate = {400};
  };

  virtual ~PIGPIO_Descriptor() = default;
  void set_gpio_2(Channel const& value);
  void set_gpio_2(Channel&& value);
  auto get_gpio_2() const -> Channel const&;
  auto get_gpio_2() -> Channel&;

  void set_gpio_3(Channel const& value);
  void set_gpio_3(Channel&& value);
  auto get_gpio_3() const -> Channel const&;
  auto get_gpio_3() -> Channel&;

  void set_gpio_4(Channel const& value);
  void set_gpio_4(Channel&& value);
  auto get_gpio_4() const -> Channel const&;
  auto get_gpio_4() -> Channel&;

  void set_gpio_5(Channel const& value);
  void set_gpio_5(Channel&& value);
  auto get_gpio_5() const -> Channel const&;
  auto get_gpio_5() -> Channel&;

  void set_gpio_6(Channel const& value);
  void set_gpio_6(Channel&& value);
  auto get_gpio_6() const -> Channel const&;
  auto get_gpio_6() -> Channel&;

  void set_gpio_7(Channel const& value);
  void set_gpio_7(Channel&& value);
  auto get_gpio_7() const -> Channel const&;
  auto get_gpio_7() -> Channel&;

  void set_gpio_8(Channel const& value);
  void set_gpio_8(Channel&& value);
  auto get_gpio_8() const -> Channel const&;
  auto get_gpio_8() -> Channel&;

  void set_gpio_9(Channel const& value);
  void set_gpio_9(Channel&& value);
  auto get_gpio_9() const -> Channel const&;
  auto get_gpio_9() -> Channel&;

  void set_gpio_10(Channel const& value);
  void set_gpio_10(Channel&& value);
  auto get_gpio_10() const -> Channel const&;
  auto get_gpio_10() -> Channel&;

  void set_gpio_11(Channel const& value);
  void set_gpio_11(Channel&& value);
  auto get_gpio_11() const -> Channel const&;
  auto get_gpio_11() -> Channel&;

  void set_gpio_12(Channel const& value);
  void set_gpio_12(Channel&& value);
  auto get_gpio_12() const -> Channel const&;
  auto get_gpio_12() -> Channel&;

  void set_gpio_13(Channel const& value);
  void set_gpio_13(Channel&& value);
  auto get_gpio_13() const -> Channel const&;
  auto get_gpio_13() -> Channel&;

  void set_gpio_14(Channel const& value);
  void set_gpio_14(Channel&& value);
  auto get_gpio_14() const -> Channel const&;
  auto get_gpio_14() -> Channel&;

  void set_gpio_15(Channel const& value);
  void set_gpio_15(Channel&& value);
  auto get_gpio_15() const -> Channel const&;
  auto get_gpio_15() -> Channel&;

  void set_gpio_16(Channel const& value);
  void set_gpio_16(Channel&& value);
  auto get_gpio_16() const -> Channel const&;
  auto get_gpio_16() -> Channel&;

  void set_gpio_17(Channel const& value);
  void set_gpio_17(Channel&& value);
  auto get_gpio_17() const -> Channel const&;
  auto get_gpio_17() -> Channel&;

  void set_gpio_18(Channel const& value);
  void set_gpio_18(Channel&& value);
  auto get_gpio_18() const -> Channel const&;
  auto get_gpio_18() -> Channel&;

  void set_gpio_19(Channel const& value);
  void set_gpio_19(Channel&& value);
  auto get_gpio_19() const -> Channel const&;
  auto get_gpio_19() -> Channel&;

  void set_gpio_20(Channel const& value);
  void set_gpio_20(Channel&& value);
  auto get_gpio_20() const -> Channel const&;
  auto get_gpio_20() -> Channel&;

  void set_gpio_21(Channel const& value);
  void set_gpio_21(Channel&& value);
  auto get_gpio_21() const -> Channel const&;
  auto get_gpio_21() -> Channel&;

  void set_gpio_22(Channel const& value);
  void set_gpio_22(Channel&& value);
  auto get_gpio_22() const -> Channel const&;
  auto get_gpio_22() -> Channel&;

  void set_gpio_23(Channel const& value);
  void set_gpio_23(Channel&& value);
  auto get_gpio_23() const -> Channel const&;
  auto get_gpio_23() -> Channel&;

  void set_gpio_24(Channel const& value);
  void set_gpio_24(Channel&& value);
  auto get_gpio_24() const -> Channel const&;
  auto get_gpio_24() -> Channel&;

  void set_gpio_25(Channel const& value);
  void set_gpio_25(Channel&& value);
  auto get_gpio_25() const -> Channel const&;
  auto get_gpio_25() -> Channel&;

  void set_gpio_26(Channel const& value);
  void set_gpio_26(Channel&& value);
  auto get_gpio_26() const -> Channel const&;
  auto get_gpio_26() -> Channel&;

  void set_gpio_27(Channel const& value);
  void set_gpio_27(Channel&& value);
  auto get_gpio_27() const -> Channel const&;
  auto get_gpio_27() -> Channel&;

private:
  Channel m_gpio_2;
  Channel m_gpio_3;
  Channel m_gpio_4;
  Channel m_gpio_5;
  Channel m_gpio_6;
  Channel m_gpio_7;
  Channel m_gpio_8;
  Channel m_gpio_9;
  Channel m_gpio_10;
  Channel m_gpio_11;
  Channel m_gpio_12;
  Channel m_gpio_13;
  Channel m_gpio_14;
  Channel m_gpio_15;
  Channel m_gpio_16;
  Channel m_gpio_17;
  Channel m_gpio_18;
  Channel m_gpio_19;
  Channel m_gpio_20;
  Channel m_gpio_21;
  Channel m_gpio_22;
  Channel m_gpio_23;
  Channel m_gpio_24;
  Channel m_gpio_25;
  Channel m_gpio_26;
  Channel m_gpio_27;
};

struct PIGPIO_Config : public INode_Config
{
public:
  struct IChannel
  {
  public:
    virtual ~IChannel() = default;
  private:
  };

  struct Servo_Channel : public PIGPIO_Config::IChannel
  {
  public:
    virtual ~Servo_Channel() = default;
    void set_min(servo_signal_t const& value);
    void set_min(servo_signal_t&& value);
    auto get_min() const -> servo_signal_t const&;

    void set_max(servo_signal_t const& value);
    void set_max(servo_signal_t&& value);
    auto get_max() const -> servo_signal_t const&;

  private:
    servo_signal_t m_min = {1.000000f};
    servo_signal_t m_max = {2.000000f};
  };

  struct PWM_Channel : public PIGPIO_Config::IChannel
  {
  public:
    virtual ~PWM_Channel() = default;
    void set_min(pwm_signal_t const& value);
    void set_min(pwm_signal_t&& value);
    auto get_min() const -> pwm_signal_t const&;

    void set_max(pwm_signal_t const& value);
    void set_max(pwm_signal_t&& value);
    auto get_max() const -> pwm_signal_t const&;

  private:
    pwm_signal_t m_min = {0};
    pwm_signal_t m_max = {1.000000f};
  };

  virtual ~PIGPIO_Config() = default;
  void set_gpio_2(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_2(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_2() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_2() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_3(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_3(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_3() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_3() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_4(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_4(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_4() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_4() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_5(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_5(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_5() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_5() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_6(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_6(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_6() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_6() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_7(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_7(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_7() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_7() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_8(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_8(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_8() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_8() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_9(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_9(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_9() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_9() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_10(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_10(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_10() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_10() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_11(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_11(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_11() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_11() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_12(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_12(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_12() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_12() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_13(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_13(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_13() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_13() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_14(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_14(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_14() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_14() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_15(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_15(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_15() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_15() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_16(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_16(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_16() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_16() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_17(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_17(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_17() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_17() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_18(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_18(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_18() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_18() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_19(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_19(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_19() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_19() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_20(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_20(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_20() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_20() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_21(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_21(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_21() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_21() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_22(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_22(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_22() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_22() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_23(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_23(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_23() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_23() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_24(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_24(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_24() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_24() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_25(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_25(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_25() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_25() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_26(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_26(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_26() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_26() -> Poly<PIGPIO_Config::IChannel>&;

  void set_gpio_27(Poly<PIGPIO_Config::IChannel> const& value);
  void set_gpio_27(Poly<PIGPIO_Config::IChannel>&& value);
  auto get_gpio_27() const -> Poly<PIGPIO_Config::IChannel> const&;
  auto get_gpio_27() -> Poly<PIGPIO_Config::IChannel>&;

private:
  Poly<PIGPIO_Config::IChannel> m_gpio_2;
  Poly<PIGPIO_Config::IChannel> m_gpio_3;
  Poly<PIGPIO_Config::IChannel> m_gpio_4;
  Poly<PIGPIO_Config::IChannel> m_gpio_5;
  Poly<PIGPIO_Config::IChannel> m_gpio_6;
  Poly<PIGPIO_Config::IChannel> m_gpio_7;
  Poly<PIGPIO_Config::IChannel> m_gpio_8;
  Poly<PIGPIO_Config::IChannel> m_gpio_9;
  Poly<PIGPIO_Config::IChannel> m_gpio_10;
  Poly<PIGPIO_Config::IChannel> m_gpio_11;
  Poly<PIGPIO_Config::IChannel> m_gpio_12;
  Poly<PIGPIO_Config::IChannel> m_gpio_13;
  Poly<PIGPIO_Config::IChannel> m_gpio_14;
  Poly<PIGPIO_Config::IChannel> m_gpio_15;
  Poly<PIGPIO_Config::IChannel> m_gpio_16;
  Poly<PIGPIO_Config::IChannel> m_gpio_17;
  Poly<PIGPIO_Config::IChannel> m_gpio_18;
  Poly<PIGPIO_Config::IChannel> m_gpio_19;
  Poly<PIGPIO_Config::IChannel> m_gpio_20;
  Poly<PIGPIO_Config::IChannel> m_gpio_21;
  Poly<PIGPIO_Config::IChannel> m_gpio_22;
  Poly<PIGPIO_Config::IChannel> m_gpio_23;
  Poly<PIGPIO_Config::IChannel> m_gpio_24;
  Poly<PIGPIO_Config::IChannel> m_gpio_25;
  Poly<PIGPIO_Config::IChannel> m_gpio_26;
  Poly<PIGPIO_Config::IChannel> m_gpio_27;
};

struct Pressure_Velocity_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Pressure_Velocity_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {1};
};

struct Pressure_Velocity_Config : public INode_Config
{
public:
  virtual ~Pressure_Velocity_Config() = default;
private:
};

struct Proximity_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  typedef int32_t channel_count_t;
  virtual ~Proximity_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

  void set_channel_count(channel_count_t const& value);
  void set_channel_count(channel_count_t&& value);
  auto get_channel_count() const -> channel_count_t const&;

private:
  uint32_t m_rate = {1};
  channel_count_t m_channel_count = {1};
};

struct Proximity_Config : public INode_Config
{
public:
  virtual ~Proximity_Config() = default;
private:
};

struct Rate_Controller_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Rate_Controller_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {1};
};

struct Rate_Controller_Config : public INode_Config
{
public:
  struct Feedback
  {
  public:
    typedef PID_Controller_Descriptor Combined_XY_PIDs;
    struct Separate_XY_PIDs
    {
    public:
      virtual ~Separate_XY_PIDs() = default;
      void set_x_pid(PID_Controller_Descriptor const& value);
      void set_x_pid(PID_Controller_Descriptor&& value);
      auto get_x_pid() const -> PID_Controller_Descriptor const&;
      auto get_x_pid() -> PID_Controller_Descriptor&;

      void set_y_pid(PID_Controller_Descriptor const& value);
      void set_y_pid(PID_Controller_Descriptor&& value);
      auto get_y_pid() const -> PID_Controller_Descriptor const&;
      auto get_y_pid() -> PID_Controller_Descriptor&;

    private:
      PID_Controller_Descriptor m_x_pid;
      PID_Controller_Descriptor m_y_pid;
    };

    virtual ~Feedback() = default;
    void set_weight(muf_t const& value);
    void set_weight(muf_t&& value);
    auto get_weight() const -> muf_t const&;

    void set_xy_pids(boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const& value);
    void set_xy_pids(boost::variant<Combined_XY_PIDs,Separate_XY_PIDs>&& value);
    auto get_xy_pids() const -> boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const&;
    auto get_xy_pids() -> boost::variant<Combined_XY_PIDs,Separate_XY_PIDs>&;

    void set_z_pid(PID_Controller_Descriptor const& value);
    void set_z_pid(PID_Controller_Descriptor&& value);
    auto get_z_pid() const -> PID_Controller_Descriptor const&;
    auto get_z_pid() -> PID_Controller_Descriptor&;

  private:
    muf_t m_weight = {1.000000f};
    boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> m_xy_pids;
    PID_Controller_Descriptor m_z_pid;
  };

  struct Feedforward
  {
  public:
    typedef float torque_t;
    virtual ~Feedforward() = default;
    void set_weight(muf_t const& value);
    void set_weight(muf_t&& value);
    auto get_weight() const -> muf_t const&;

    void set_max_torque(torque_t const& value);
    void set_max_torque(torque_t&& value);
    auto get_max_torque() const -> torque_t const&;

  private:
    muf_t m_weight = {0};
    torque_t m_max_torque = {1.000000f};
  };

  virtual ~Rate_Controller_Config() = default;
  void set_feedback(Feedback const& value);
  void set_feedback(Feedback&& value);
  auto get_feedback() const -> Feedback const&;
  auto get_feedback() -> Feedback&;

  void set_feedforward(Feedforward const& value);
  void set_feedforward(Feedforward&& value);
  auto get_feedforward() const -> Feedforward const&;
  auto get_feedforward() -> Feedforward&;

private:
  Feedback m_feedback;
  Feedforward m_feedforward;
};

struct Raspicam_Descriptor : public INode_Descriptor
{
public:
  struct Quality
  {
  public:
    typedef vec2i resolution_t;
    typedef uint32_t bitrate_t;
    virtual ~Quality() = default;
    void set_resolution(resolution_t const& value);
    void set_resolution(resolution_t&& value);
    auto get_resolution() const -> resolution_t const&;

    void set_bitrate(bitrate_t const& value);
    void set_bitrate(bitrate_t&& value);
    auto get_bitrate() const -> bitrate_t const&;

  private:
    resolution_t m_resolution = {8, 8};
    bitrate_t m_bitrate = {10000};
  };

  typedef int32_t fps_t;
  virtual ~Raspicam_Descriptor() = default;
  void set_fps(fps_t const& value);
  void set_fps(fps_t&& value);
  auto get_fps() const -> fps_t const&;

  void set_streaming_low(Quality const& value);
  void set_streaming_low(Quality&& value);
  auto get_streaming_low() const -> Quality const&;
  auto get_streaming_low() -> Quality&;

  void set_streaming_high(Quality const& value);
  void set_streaming_high(Quality&& value);
  auto get_streaming_high() const -> Quality const&;
  auto get_streaming_high() -> Quality&;

  void set_recording(Quality const& value);
  void set_recording(Quality&& value);
  auto get_recording() const -> Quality const&;
  auto get_recording() -> Quality&;

private:
  fps_t m_fps = {10};
  Quality m_streaming_low;
  Quality m_streaming_high;
  Quality m_recording;
};

struct Raspicam_Config : public INode_Config
{
public:
  typedef int32_t iso_t;
  typedef float shutter_speed_t;
  typedef int32_t ev_t;
  typedef int32_t sharpness_t;
  typedef int32_t contrast_t;
  typedef int32_t brightness_t;
  typedef int32_t saturation_t;
  enum class quality_t
  {
    LOW = 0,
    HIGH = 0,
  };

  enum class awb_mode_t
  {
    OFF = 0,
    AUTO = 1,
    SUNLIGHT = 2,
    CLOUDY = 3,
    SHADE = 4,
    TUNGSTEN = 5,
    FLUORESCENT = 6,
    INCANDESCENT = 7,
    FLASH = 8,
    HORIZON = 9,
  };

  virtual ~Raspicam_Config() = default;
  void set_iso(iso_t const& value);
  void set_iso(iso_t&& value);
  auto get_iso() const -> iso_t const&;

  void set_shutter_speed(shutter_speed_t const& value);
  void set_shutter_speed(shutter_speed_t&& value);
  auto get_shutter_speed() const -> shutter_speed_t const&;

  void set_ev(ev_t const& value);
  void set_ev(ev_t&& value);
  auto get_ev() const -> ev_t const&;

  void set_sharpness(sharpness_t const& value);
  void set_sharpness(sharpness_t&& value);
  auto get_sharpness() const -> sharpness_t const&;

  void set_contrast(contrast_t const& value);
  void set_contrast(contrast_t&& value);
  auto get_contrast() const -> contrast_t const&;

  void set_brightness(brightness_t const& value);
  void set_brightness(brightness_t&& value);
  auto get_brightness() const -> brightness_t const&;

  void set_saturation(saturation_t const& value);
  void set_saturation(saturation_t&& value);
  auto get_saturation() const -> saturation_t const&;

  void set_awb_mode(awb_mode_t const& value);
  void set_awb_mode(awb_mode_t&& value);
  auto get_awb_mode() const -> awb_mode_t const&;

  void set_quality(quality_t const& value);
  void set_quality(quality_t&& value);
  auto get_quality() const -> quality_t const&;

  void set_recording(bool const& value);
  void set_recording(bool&& value);
  auto get_recording() const -> bool const&;

private:
  iso_t m_iso = {0};
  shutter_speed_t m_shutter_speed = {0};
  ev_t m_ev = {0};
  sharpness_t m_sharpness = {0};
  contrast_t m_contrast = {0};
  brightness_t m_brightness = {0};
  saturation_t m_saturation = {0};
  awb_mode_t m_awb_mode = {Raspicam_Config::awb_mode_t::AUTO};
  quality_t m_quality = {Raspicam_Config::quality_t::HIGH};
  bool m_recording = {false};
};

struct RC5T619_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~RC5T619_Descriptor() = default;
  void set_bus(std::string const& value);
  void set_bus(std::string&& value);
  auto get_bus() const -> std::string const&;

  void set_adc0_rate(uint32_t const& value);
  void set_adc0_rate(uint32_t&& value);
  auto get_adc0_rate() const -> uint32_t const&;

  void set_adc1_rate(uint32_t const& value);
  void set_adc1_rate(uint32_t&& value);
  auto get_adc1_rate() const -> uint32_t const&;

private:
  std::string m_bus;
  uint32_t m_adc0_rate = {50};
  uint32_t m_adc1_rate = {5};
};

struct RC5T619_Config : public INode_Config
{
public:
  virtual ~RC5T619_Config() = default;
private:
};

struct Resampler_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Resampler_Descriptor() = default;
  void set_input_rate(uint32_t const& value);
  void set_input_rate(uint32_t&& value);
  auto get_input_rate() const -> uint32_t const&;

  void set_output_rate(uint32_t const& value);
  void set_output_rate(uint32_t&& value);
  auto get_output_rate() const -> uint32_t const&;

private:
  uint32_t m_input_rate = {1};
  uint32_t m_output_rate = {1};
};

struct Resampler_Config : public INode_Config
{
public:
  virtual ~Resampler_Config() = default;
  void set_lpf(LPF_Config const& value);
  void set_lpf(LPF_Config&& value);
  auto get_lpf() const -> LPF_Config const&;
  auto get_lpf() -> LPF_Config&;

private:
  LPF_Config m_lpf;
};

struct Scalar_Generator_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Scalar_Generator_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {1};
};

struct Scalar_Generator_Config : public INode_Config
{
public:
  virtual ~Scalar_Generator_Config() = default;
  void set_value(float const& value);
  void set_value(float&& value);
  auto get_value() const -> float const&;

private:
  float m_value = {0};
};

struct Servo_Gimbal_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Servo_Gimbal_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

  void set_commands_rate(uint32_t const& value);
  void set_commands_rate(uint32_t&& value);
  auto get_commands_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {1};
  uint32_t m_commands_rate = {1};
};

struct Servo_Gimbal_Config : public INode_Config
{
public:
  struct Channel
  {
  public:
    virtual ~Channel() = default;
    void set_min_angle(euler_t const& value);
    void set_min_angle(euler_t&& value);
    auto get_min_angle() const -> euler_t const&;

    void set_max_angle(euler_t const& value);
    void set_max_angle(euler_t&& value);
    auto get_max_angle() const -> euler_t const&;

    void set_min_pwm(pwm_signal_t const& value);
    void set_min_pwm(pwm_signal_t&& value);
    auto get_min_pwm() const -> pwm_signal_t const&;

    void set_max_pwm(pwm_signal_t const& value);
    void set_max_pwm(pwm_signal_t&& value);
    auto get_max_pwm() const -> pwm_signal_t const&;

  private:
    euler_t m_min_angle = {-180.000000f};
    euler_t m_max_angle = {180.000000f};
    pwm_signal_t m_min_pwm = {0};
    pwm_signal_t m_max_pwm = {1.000000f};
  };

  virtual ~Servo_Gimbal_Config() = default;
  void set_x_channel(Channel const& value);
  void set_x_channel(Channel&& value);
  auto get_x_channel() const -> Channel const&;
  auto get_x_channel() -> Channel&;

  void set_y_channel(Channel const& value);
  void set_y_channel(Channel&& value);
  auto get_y_channel() const -> Channel const&;
  auto get_y_channel() -> Channel&;

  void set_z_channel(Channel const& value);
  void set_z_channel(Channel&& value);
  auto get_z_channel() const -> Channel const&;
  auto get_z_channel() -> Channel&;

private:
  Channel m_x_channel;
  Channel m_y_channel;
  Channel m_z_channel;
};

struct SRF01_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~SRF01_Descriptor() = default;
  void set_bus(std::string const& value);
  void set_bus(std::string&& value);
  auto get_bus() const -> std::string const&;

  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  std::string m_bus;
  uint32_t m_rate = {1};
};

struct SRF01_Config : public INode_Config
{
public:
  virtual ~SRF01_Config() = default;
  void set_direction(direction_t const& value);
  void set_direction(direction_t&& value);
  auto get_direction() const -> direction_t const&;

  void set_max_distance(ufloat const& value);
  void set_max_distance(ufloat&& value);
  auto get_max_distance() const -> ufloat const&;

  void set_min_distance(ufloat const& value);
  void set_min_distance(ufloat&& value);
  auto get_min_distance() const -> ufloat const&;

private:
  direction_t m_direction = {0, 0, 0};
  ufloat m_max_distance = {6.000000f};
  ufloat m_min_distance = {0.200000f};
};

struct SRF02_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~SRF02_Descriptor() = default;
  void set_bus(std::string const& value);
  void set_bus(std::string&& value);
  auto get_bus() const -> std::string const&;

  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  std::string m_bus;
  uint32_t m_rate = {1};
};

struct SRF02_Config : public INode_Config
{
public:
  virtual ~SRF02_Config() = default;
  void set_direction(direction_t const& value);
  void set_direction(direction_t&& value);
  auto get_direction() const -> direction_t const&;

  void set_max_distance(ufloat const& value);
  void set_max_distance(ufloat&& value);
  auto get_max_distance() const -> ufloat const&;

  void set_min_distance(ufloat const& value);
  void set_min_distance(ufloat&& value);
  auto get_min_distance() const -> ufloat const&;

private:
  direction_t m_direction = {0, 0, 0};
  ufloat m_max_distance = {6.000000f};
  ufloat m_min_distance = {0.200000f};
};

struct Throttle_To_PWM_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  typedef int32_t channel_count_t;
  virtual ~Throttle_To_PWM_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

  void set_channel_count(channel_count_t const& value);
  void set_channel_count(channel_count_t&& value);
  auto get_channel_count() const -> channel_count_t const&;

private:
  uint32_t m_rate = {1};
  channel_count_t m_channel_count = {1};
};

struct Throttle_To_PWM_Config : public INode_Config
{
public:
  virtual ~Throttle_To_PWM_Config() = default;
private:
};

struct Transformer_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Transformer_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {1};
};

struct Transformer_Config : public INode_Config
{
public:
  virtual ~Transformer_Config() = default;
private:
};

struct UBLOX_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~UBLOX_Descriptor() = default;
  void set_bus(std::string const& value);
  void set_bus(std::string&& value);
  auto get_bus() const -> std::string const&;

  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  std::string m_bus;
  uint32_t m_rate = {1};
};

struct UBLOX_Config : public INode_Config
{
public:
  virtual ~UBLOX_Config() = default;
private:
};

struct Vec3_Generator_Descriptor : public INode_Descriptor
{
public:
  typedef uint32_t rate_t;
  virtual ~Vec3_Generator_Descriptor() = default;
  void set_rate(uint32_t const& value);
  void set_rate(uint32_t&& value);
  auto get_rate() const -> uint32_t const&;

private:
  uint32_t m_rate = {1};
};

struct Vec3_Generator_Config : public INode_Config
{
public:
  virtual ~Vec3_Generator_Config() = default;
  void set_value(vec3f const& value);
  void set_value(vec3f&& value);
  auto get_value() const -> vec3f const&;

private:
  vec3f m_value = {0, 0, 0};
};

ts::Result<void> deserialize(bool& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(bool const& value);
ts::Result<void> deserialize(std::string& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::string const& value);
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
ts::Result<void> deserialize(IUAV_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(IUAV_Descriptor const& value);
ts::Result<void> deserialize(Poly<IUAV_Descriptor>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Poly<IUAV_Descriptor> const& value);
ts::Result<void> deserialize(IBus_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(IBus_Descriptor const& value);
ts::Result<void> deserialize(Poly<IBus_Descriptor>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Poly<IBus_Descriptor> const& value);
ts::Result<void> deserialize(INode_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(INode_Descriptor const& value);
ts::Result<void> deserialize(Poly<INode_Descriptor>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Poly<INode_Descriptor> const& value);
ts::Result<void> deserialize(INode_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(INode_Config const& value);
ts::Result<void> deserialize(Poly<INode_Config>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Poly<INode_Config> const& value);
ts::Result<void> deserialize(Settings::Bus_Data& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Settings::Bus_Data const& value);
ts::Result<void> deserialize(Settings::Node_Data& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Settings::Node_Data const& value);
ts::Result<void> deserialize(Settings& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Settings const& value);
ts::Result<void> deserialize(Poly<const IBus_Descriptor>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Poly<const IBus_Descriptor> const& value);
ts::Result<void> deserialize(Poly<const INode_Descriptor>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Poly<const INode_Descriptor> const& value);
ts::Result<void> deserialize(Poly<const INode_Config>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Poly<const INode_Config> const& value);
ts::Result<void> deserialize(std::vector<std::string>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<std::string> const& value);
ts::Result<void> deserialize(Poly<const IUAV_Descriptor>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Poly<const IUAV_Descriptor> const& value);
ts::Result<void> deserialize(std::vector<Settings::Bus_Data>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<Settings::Bus_Data> const& value);
ts::Result<void> deserialize(std::vector<Settings::Node_Data>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<Settings::Node_Data> const& value);
ts::Result<void> deserialize(Multirotor_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Descriptor const& value);
ts::Result<void> deserialize(Tri_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Tri_Descriptor const& value);
ts::Result<void> deserialize(Quad_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Quad_Descriptor const& value);
ts::Result<void> deserialize(Hexa_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Hexa_Descriptor const& value);
ts::Result<void> deserialize(Hexatri_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Hexatri_Descriptor const& value);
ts::Result<void> deserialize(Octo_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Octo_Descriptor const& value);
ts::Result<void> deserialize(Octaquad_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Octaquad_Descriptor const& value);
ts::Result<void> deserialize(Custom_Multirotor_Descriptor::Motor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Custom_Multirotor_Descriptor::Motor const& value);
ts::Result<void> deserialize(Custom_Multirotor_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Custom_Multirotor_Descriptor const& value);
ts::Result<void> deserialize(std::vector<Custom_Multirotor_Descriptor::Motor>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<Custom_Multirotor_Descriptor::Motor> const& value);
ts::Result<void> deserialize(UART_Linux_Descriptor::baud_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(UART_Linux_Descriptor::baud_t const& value);
ts::Result<void> deserialize(UART_Linux_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(UART_Linux_Descriptor const& value);
ts::Result<void> deserialize(UART_BBang_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(UART_BBang_Descriptor const& value);
ts::Result<void> deserialize(I2C_BCM_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(I2C_BCM_Descriptor const& value);
ts::Result<void> deserialize(I2C_Linux_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(I2C_Linux_Descriptor const& value);
ts::Result<void> deserialize(SPI_BCM_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(SPI_BCM_Descriptor const& value);
ts::Result<void> deserialize(SPI_Linux_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(SPI_Linux_Descriptor const& value);
ts::Result<void> deserialize(P_Controller_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(P_Controller_Descriptor const& value);
ts::Result<void> deserialize(PI_Controller_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PI_Controller_Descriptor const& value);
ts::Result<void> deserialize(PD_Controller_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PD_Controller_Descriptor const& value);
ts::Result<void> deserialize(PID_Controller_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PID_Controller_Descriptor const& value);
ts::Result<void> deserialize(ADC_Ammeter_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(ADC_Ammeter_Descriptor const& value);
ts::Result<void> deserialize(ADC_Ammeter_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(ADC_Ammeter_Config const& value);
ts::Result<void> deserialize(ADC_Voltmeter_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(ADC_Voltmeter_Descriptor const& value);
ts::Result<void> deserialize(ADC_Voltmeter_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(ADC_Voltmeter_Config const& value);
ts::Result<void> deserialize(ADS1115_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(ADS1115_Descriptor const& value);
ts::Result<void> deserialize(ADS1115_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(ADS1115_Config const& value);
ts::Result<void> deserialize(AVRADC_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(AVRADC_Descriptor const& value);
ts::Result<void> deserialize(AVRADC_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(AVRADC_Config const& value);
ts::Result<void> deserialize(Comp_AHRS_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Comp_AHRS_Descriptor const& value);
ts::Result<void> deserialize(Comp_AHRS_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Comp_AHRS_Config const& value);
ts::Result<void> deserialize(Combiner_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Combiner_Descriptor const& value);
ts::Result<void> deserialize(Combiner_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Combiner_Config const& value);
ts::Result<void> deserialize(Gravity_Filter_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Gravity_Filter_Descriptor const& value);
ts::Result<void> deserialize(Gravity_Filter_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Gravity_Filter_Config const& value);
ts::Result<void> deserialize(KF_ECEF_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(KF_ECEF_Descriptor const& value);
ts::Result<void> deserialize(KF_ECEF_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(KF_ECEF_Config const& value);
ts::Result<void> deserialize(ENU_Frame_System_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(ENU_Frame_System_Descriptor const& value);
ts::Result<void> deserialize(ENU_Frame_System_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(ENU_Frame_System_Config const& value);
ts::Result<void> deserialize(LPF_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(LPF_Descriptor const& value);
ts::Result<void> deserialize(LPF_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(LPF_Config const& value);
ts::Result<void> deserialize(MaxSonar_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(MaxSonar_Descriptor const& value);
ts::Result<void> deserialize(MaxSonar_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(MaxSonar_Config const& value);
ts::Result<void> deserialize(Motor_Mixer_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Motor_Mixer_Descriptor const& value);
ts::Result<void> deserialize(Motor_Mixer_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Motor_Mixer_Config const& value);
ts::Result<void> deserialize(Acceleration_Calibration_Point& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Acceleration_Calibration_Point const& value);
ts::Result<void> deserialize(Angular_Velocity_Calibration_Point& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Angular_Velocity_Calibration_Point const& value);
ts::Result<void> deserialize(Magnetic_Field_Calibration_Point& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Magnetic_Field_Calibration_Point const& value);
ts::Result<void> deserialize(MPU9250_Descriptor::imu_rate_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(MPU9250_Descriptor::imu_rate_t const& value);
ts::Result<void> deserialize(MPU9250_Descriptor::acceleration_range_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(MPU9250_Descriptor::acceleration_range_t const& value);
ts::Result<void> deserialize(MPU9250_Descriptor::angular_velocity_range_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(MPU9250_Descriptor::angular_velocity_range_t const& value);
ts::Result<void> deserialize(MPU9250_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(MPU9250_Descriptor const& value);
ts::Result<void> deserialize(MPU9250_Config::Calibration& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(MPU9250_Config::Calibration const& value);
ts::Result<void> deserialize(MPU9250_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(MPU9250_Config const& value);
ts::Result<void> deserialize(std::vector<Acceleration_Calibration_Point>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<Acceleration_Calibration_Point> const& value);
ts::Result<void> deserialize(std::vector<Angular_Velocity_Calibration_Point>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<Angular_Velocity_Calibration_Point> const& value);
ts::Result<void> deserialize(std::vector<Magnetic_Field_Calibration_Point>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<Magnetic_Field_Calibration_Point> const& value);
ts::Result<void> deserialize(MS5611_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(MS5611_Descriptor const& value);
ts::Result<void> deserialize(MS5611_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(MS5611_Config const& value);
ts::Result<void> deserialize(Multirotor_Brain_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Brain_Descriptor const& value);
ts::Result<void> deserialize(Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs const& value);
ts::Result<void> deserialize(Multirotor_Brain_Config::Horizontal_Angle& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Brain_Config::Horizontal_Angle const& value);
ts::Result<void> deserialize(Multirotor_Brain_Config::Horizontal_Position& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Brain_Config::Horizontal_Position const& value);
ts::Result<void> deserialize(Multirotor_Brain_Config::Yaw_Angle& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Brain_Config::Yaw_Angle const& value);
ts::Result<void> deserialize(Multirotor_Brain_Config::Altitude& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Brain_Config::Altitude const& value);
ts::Result<void> deserialize(Multirotor_Brain_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Brain_Config const& value);
ts::Result<void> deserialize(boost::variant<Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs,Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(boost::variant<Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs,Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs> const& value);
ts::Result<void> deserialize(Multirotor_Pilot_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Pilot_Descriptor const& value);
ts::Result<void> deserialize(Multirotor_Pilot_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Pilot_Config const& value);
ts::Result<void> deserialize(Multirotor_Simulator_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulator_Descriptor const& value);
ts::Result<void> deserialize(Multirotor_Simulator_Config::Noise& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulator_Config::Noise const& value);
ts::Result<void> deserialize(Multirotor_Simulator_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Multirotor_Simulator_Config const& value);
ts::Result<void> deserialize(Oscillator_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Oscillator_Descriptor const& value);
ts::Result<void> deserialize(Oscillator_Config::Component& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Oscillator_Config::Component const& value);
ts::Result<void> deserialize(Oscillator_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Oscillator_Config const& value);
ts::Result<void> deserialize(std::vector<Oscillator_Config::Component>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<Oscillator_Config::Component> const& value);
ts::Result<void> deserialize(PCA9685_Descriptor::Channel& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PCA9685_Descriptor::Channel const& value);
ts::Result<void> deserialize(PCA9685_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PCA9685_Descriptor const& value);
ts::Result<void> deserialize(std::vector<PCA9685_Descriptor::Channel>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<PCA9685_Descriptor::Channel> const& value);
ts::Result<void> deserialize(PCA9685_Config::IChannel& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PCA9685_Config::IChannel const& value);
ts::Result<void> deserialize(PCA9685_Config::Servo_Channel& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PCA9685_Config::Servo_Channel const& value);
ts::Result<void> deserialize(PCA9685_Config::PWM_Channel& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PCA9685_Config::PWM_Channel const& value);
ts::Result<void> deserialize(PCA9685_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PCA9685_Config const& value);
ts::Result<void> deserialize(Poly<PCA9685_Config::IChannel>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Poly<PCA9685_Config::IChannel> const& value);
ts::Result<void> deserialize(std::vector<Poly<PCA9685_Config::IChannel>>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<Poly<PCA9685_Config::IChannel>> const& value);
ts::Result<void> deserialize(PIGPIO_Descriptor::Channel& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PIGPIO_Descriptor::Channel const& value);
ts::Result<void> deserialize(PIGPIO_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PIGPIO_Descriptor const& value);
ts::Result<void> deserialize(PIGPIO_Config::IChannel& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PIGPIO_Config::IChannel const& value);
ts::Result<void> deserialize(PIGPIO_Config::Servo_Channel& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PIGPIO_Config::Servo_Channel const& value);
ts::Result<void> deserialize(PIGPIO_Config::PWM_Channel& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PIGPIO_Config::PWM_Channel const& value);
ts::Result<void> deserialize(PIGPIO_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(PIGPIO_Config const& value);
ts::Result<void> deserialize(Poly<PIGPIO_Config::IChannel>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Poly<PIGPIO_Config::IChannel> const& value);
ts::Result<void> deserialize(Pressure_Velocity_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Pressure_Velocity_Descriptor const& value);
ts::Result<void> deserialize(Pressure_Velocity_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Pressure_Velocity_Config const& value);
ts::Result<void> deserialize(Proximity_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Proximity_Descriptor const& value);
ts::Result<void> deserialize(Proximity_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Proximity_Config const& value);
ts::Result<void> deserialize(Rate_Controller_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Rate_Controller_Descriptor const& value);
ts::Result<void> deserialize(Rate_Controller_Config::Feedback::Separate_XY_PIDs& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Rate_Controller_Config::Feedback::Separate_XY_PIDs const& value);
ts::Result<void> deserialize(Rate_Controller_Config::Feedback& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Rate_Controller_Config::Feedback const& value);
ts::Result<void> deserialize(Rate_Controller_Config::Feedforward& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Rate_Controller_Config::Feedforward const& value);
ts::Result<void> deserialize(Rate_Controller_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Rate_Controller_Config const& value);
ts::Result<void> deserialize(boost::variant<Rate_Controller_Config::Feedback::Combined_XY_PIDs,Rate_Controller_Config::Feedback::Separate_XY_PIDs>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(boost::variant<Rate_Controller_Config::Feedback::Combined_XY_PIDs,Rate_Controller_Config::Feedback::Separate_XY_PIDs> const& value);
ts::Result<void> deserialize(Raspicam_Descriptor::Quality& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Raspicam_Descriptor::Quality const& value);
ts::Result<void> deserialize(Raspicam_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Raspicam_Descriptor const& value);
ts::Result<void> deserialize(Raspicam_Config::quality_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Raspicam_Config::quality_t const& value);
ts::Result<void> deserialize(Raspicam_Config::awb_mode_t& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Raspicam_Config::awb_mode_t const& value);
ts::Result<void> deserialize(Raspicam_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Raspicam_Config const& value);
ts::Result<void> deserialize(RC5T619_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(RC5T619_Descriptor const& value);
ts::Result<void> deserialize(RC5T619_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(RC5T619_Config const& value);
ts::Result<void> deserialize(Resampler_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Resampler_Descriptor const& value);
ts::Result<void> deserialize(Resampler_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Resampler_Config const& value);
ts::Result<void> deserialize(Scalar_Generator_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Scalar_Generator_Descriptor const& value);
ts::Result<void> deserialize(Scalar_Generator_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Scalar_Generator_Config const& value);
ts::Result<void> deserialize(Servo_Gimbal_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Servo_Gimbal_Descriptor const& value);
ts::Result<void> deserialize(Servo_Gimbal_Config::Channel& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Servo_Gimbal_Config::Channel const& value);
ts::Result<void> deserialize(Servo_Gimbal_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Servo_Gimbal_Config const& value);
ts::Result<void> deserialize(SRF01_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(SRF01_Descriptor const& value);
ts::Result<void> deserialize(SRF01_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(SRF01_Config const& value);
ts::Result<void> deserialize(SRF02_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(SRF02_Descriptor const& value);
ts::Result<void> deserialize(SRF02_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(SRF02_Config const& value);
ts::Result<void> deserialize(Throttle_To_PWM_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Throttle_To_PWM_Descriptor const& value);
ts::Result<void> deserialize(Throttle_To_PWM_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Throttle_To_PWM_Config const& value);
ts::Result<void> deserialize(Transformer_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Transformer_Descriptor const& value);
ts::Result<void> deserialize(Transformer_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Transformer_Config const& value);
ts::Result<void> deserialize(UBLOX_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(UBLOX_Descriptor const& value);
ts::Result<void> deserialize(UBLOX_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(UBLOX_Config const& value);
ts::Result<void> deserialize(Vec3_Generator_Descriptor& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Vec3_Generator_Descriptor const& value);
ts::Result<void> deserialize(Vec3_Generator_Config& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(Vec3_Generator_Config const& value);
}
}
