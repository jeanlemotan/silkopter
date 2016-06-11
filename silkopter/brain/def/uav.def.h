#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <boost/variant.hpp>
#include <def_lang/Result.h>
#include <def_lang/Serialization.h>
#include "gen_support.h"
// Returns the ast json from which a ast root node can be serialized
std::string const& get_ast_json();
namespace math
{

}
namespace silk
{

struct IUAV_Descriptor
{
public:

  IUAV_Descriptor() noexcept {};
  virtual ~IUAV_Descriptor() noexcept {};

  void set_name(std::string const& value);
  auto get_name() const -> std::string const&;

  void set_mass(float const& value);
  auto get_mass() const -> float const&;

  void set_moment_of_inertia(float const& value);
  auto get_moment_of_inertia() const -> float const&;


private:

  std::string m_name = {};
  float m_mass = {0.000000f};
  float m_moment_of_inertia = {0.000000f};
};

struct IBus_Descriptor
{
public:

  IBus_Descriptor() noexcept {};
  virtual ~IBus_Descriptor() noexcept {};


private:

};

struct INode_Descriptor
{
public:

  INode_Descriptor() noexcept {};
  virtual ~INode_Descriptor() noexcept {};


private:

};

struct INode_Config
{
public:

  INode_Config() noexcept {};
  virtual ~INode_Config() noexcept {};


private:

};

struct Settings
{
public:

  struct Bus_Data
  {
  public:

    Bus_Data() noexcept {};
    virtual ~Bus_Data() noexcept {};

    void set_name(std::string const& value);
    auto get_name() const -> std::string const&;

    void set_type(std::string const& value);
    auto get_type() const -> std::string const&;

    void set_descriptor(std::shared_ptr<::silk::IBus_Descriptor> const& value);
    auto get_descriptor() const -> std::shared_ptr<::silk::IBus_Descriptor> const&;
    auto get_descriptor() -> std::shared_ptr<::silk::IBus_Descriptor>&;


  private:

    std::string m_name = {};
    std::string m_type = {};
    std::shared_ptr<::silk::IBus_Descriptor> m_descriptor = {};
  };

  struct Node_Data
  {
  public:

    Node_Data() noexcept {};
    virtual ~Node_Data() noexcept {};

    void set_name(std::string const& value);
    auto get_name() const -> std::string const&;

    void set_type(std::string const& value);
    auto get_type() const -> std::string const&;

    void set_descriptor(std::shared_ptr<::silk::INode_Descriptor> const& value);
    auto get_descriptor() const -> std::shared_ptr<::silk::INode_Descriptor> const&;
    auto get_descriptor() -> std::shared_ptr<::silk::INode_Descriptor>&;

    void set_config(std::shared_ptr<::silk::INode_Config> const& value);
    auto get_config() const -> std::shared_ptr<::silk::INode_Config> const&;
    auto get_config() -> std::shared_ptr<::silk::INode_Config>&;

    void set_input_paths(std::vector<std::string> const& value);
    auto get_input_paths() const -> std::vector<std::string> const&;
    auto get_input_paths() -> std::vector<std::string>&;


  private:

    std::string m_name = {};
    std::string m_type = {};
    std::shared_ptr<::silk::INode_Descriptor> m_descriptor = {};
    std::shared_ptr<::silk::INode_Config> m_config = {};
    std::vector<std::string> m_input_paths = {};
  };

  Settings() noexcept {};
  virtual ~Settings() noexcept {};

  void set_uav_descriptor(std::shared_ptr<::silk::IUAV_Descriptor> const& value);
  auto get_uav_descriptor() const -> std::shared_ptr<::silk::IUAV_Descriptor> const&;
  auto get_uav_descriptor() -> std::shared_ptr<::silk::IUAV_Descriptor>&;

  void set_buses(std::vector<::silk::Settings::Bus_Data> const& value);
  auto get_buses() const -> std::vector<::silk::Settings::Bus_Data> const&;
  auto get_buses() -> std::vector<::silk::Settings::Bus_Data>&;

  void set_nodes(std::vector<::silk::Settings::Node_Data> const& value);
  auto get_nodes() const -> std::vector<::silk::Settings::Node_Data> const&;
  auto get_nodes() -> std::vector<::silk::Settings::Node_Data>&;


private:

  std::shared_ptr<::silk::IUAV_Descriptor> m_uav_descriptor = {};
  std::vector<::silk::Settings::Bus_Data> m_buses = {};
  std::vector<::silk::Settings::Node_Data> m_nodes = {};
};

struct Multirotor_Descriptor : public ::silk::IUAV_Descriptor
{
public:

  struct Motor
  {
  public:

    Motor() noexcept {};
    virtual ~Motor() noexcept {};

    void set_position(math::vec3<float> const& value);
    auto get_position() const -> math::vec3<float> const&;

    void set_thrust_vector(math::vec3<float> const& value);
    auto get_thrust_vector() const -> math::vec3<float> const&;

    void set_clockwise(bool const& value);
    auto get_clockwise() const -> bool const&;


  private:

    math::vec3<float> m_position = {0.000000f, 0.000000f, 0.000000f};
    math::vec3<float> m_thrust_vector = {0.000000f, 0.000000f, 1.000000f};
    bool m_clockwise = {false};
  };

  Multirotor_Descriptor() noexcept {};
  virtual ~Multirotor_Descriptor() noexcept {};

  void set_name(std::string const& value);
  auto get_name() const -> std::string const&;

  void set_mass(float const& value);
  auto get_mass() const -> float const&;

  void set_moment_of_inertia(float const& value);
  auto get_moment_of_inertia() const -> float const&;

  void set_radius(float const& value);
  auto get_radius() const -> float const&;

  void set_height(float const& value);
  auto get_height() const -> float const&;

  void set_motor_z_torque(float const& value);
  auto get_motor_z_torque() const -> float const&;

  void set_motor_thrust(float const& value);
  auto get_motor_thrust() const -> float const&;

  void set_motor_acceleration(float const& value);
  auto get_motor_acceleration() const -> float const&;

  void set_motor_deceleration(float const& value);
  auto get_motor_deceleration() const -> float const&;

  void set_motors(std::vector<::silk::Multirotor_Descriptor::Motor> const& value);
  auto get_motors() const -> std::vector<::silk::Multirotor_Descriptor::Motor> const&;
  auto get_motors() -> std::vector<::silk::Multirotor_Descriptor::Motor>&;


private:

  std::string m_name = {};
  float m_mass = {0.000000f};
  float m_moment_of_inertia = {0.000000f};
  float m_radius = {1.000000f};
  float m_height = {1.000000f};
  float m_motor_z_torque = {1.000000f};
  float m_motor_thrust = {1.000000f};
  float m_motor_acceleration = {10.000000f};
  float m_motor_deceleration = {10.000000f};
  std::vector<::silk::Multirotor_Descriptor::Motor> m_motors = {};
};

struct UART_Linux_Descriptor : public ::silk::IBus_Descriptor
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

  UART_Linux_Descriptor() noexcept {};
  virtual ~UART_Linux_Descriptor() noexcept {};

  void set_dev(std::string const& value);
  auto get_dev() const -> std::string const&;

  void set_baud(baud_t const& value);
  auto get_baud() const -> baud_t const&;


private:

  std::string m_dev = {"/dev/ttyAMA0"};
  baud_t m_baud = {::silk::UART_Linux_Descriptor::baud_t::_115200};
};

struct UART_BBang_Descriptor : public ::silk::IBus_Descriptor
{
public:

  UART_BBang_Descriptor() noexcept {};
  virtual ~UART_BBang_Descriptor() noexcept {};

  void set_rx_pin(uint32_t const& value);
  auto get_rx_pin() const -> uint32_t const&;

  void set_baud(uint32_t const& value);
  auto get_baud() const -> uint32_t const&;

  void set_invert(bool const& value);
  auto get_invert() const -> bool const&;


private:

  uint32_t m_rx_pin = {0};
  uint32_t m_baud = {115200};
  bool m_invert = {false};
};

struct I2C_BCM_Descriptor : public ::silk::IBus_Descriptor
{
public:

  I2C_BCM_Descriptor() noexcept {};
  virtual ~I2C_BCM_Descriptor() noexcept {};

  void set_dev(uint32_t const& value);
  auto get_dev() const -> uint32_t const&;

  void set_baud(uint32_t const& value);
  auto get_baud() const -> uint32_t const&;


private:

  uint32_t m_dev = {0};
  uint32_t m_baud = {400000};
};

struct I2C_Linux_Descriptor : public ::silk::IBus_Descriptor
{
public:

  I2C_Linux_Descriptor() noexcept {};
  virtual ~I2C_Linux_Descriptor() noexcept {};

  void set_dev(std::string const& value);
  auto get_dev() const -> std::string const&;


private:

  std::string m_dev = {"/dev/i2c0"};
};

struct SPI_BCM_Descriptor : public ::silk::IBus_Descriptor
{
public:

  SPI_BCM_Descriptor() noexcept {};
  virtual ~SPI_BCM_Descriptor() noexcept {};

  void set_dev(uint32_t const& value);
  auto get_dev() const -> uint32_t const&;

  void set_baud(uint32_t const& value);
  auto get_baud() const -> uint32_t const&;

  void set_mode(uint32_t const& value);
  auto get_mode() const -> uint32_t const&;

  void set_speed(uint32_t const& value);
  auto get_speed() const -> uint32_t const&;


private:

  uint32_t m_dev = {0};
  uint32_t m_baud = {1000000};
  uint32_t m_mode = {0};
  uint32_t m_speed = {1000000};
};

struct SPI_Linux_Descriptor : public ::silk::IBus_Descriptor
{
public:

  SPI_Linux_Descriptor() noexcept {};
  virtual ~SPI_Linux_Descriptor() noexcept {};

  void set_dev(std::string const& value);
  auto get_dev() const -> std::string const&;

  void set_speed(uint32_t const& value);
  auto get_speed() const -> uint32_t const&;


private:

  std::string m_dev = {"/dev/ttyAMA0"};
  uint32_t m_speed = {1000000};
};

struct P_Controller_Descriptor
{
public:

  P_Controller_Descriptor() noexcept {};
  virtual ~P_Controller_Descriptor() noexcept {};

  void set_kp(float const& value);
  auto get_kp() const -> float const&;


private:

  float m_kp = {0.000000f};
};

struct PI_Controller_Descriptor : public ::silk::P_Controller_Descriptor
{
public:

  PI_Controller_Descriptor() noexcept {};
  virtual ~PI_Controller_Descriptor() noexcept {};

  void set_kp(float const& value);
  auto get_kp() const -> float const&;

  void set_ki(float const& value);
  auto get_ki() const -> float const&;

  void set_max_i(float const& value);
  auto get_max_i() const -> float const&;


private:

  float m_kp = {0.000000f};
  float m_ki = {0.000000f};
  float m_max_i = {0.000000f};
};

struct PD_Controller_Descriptor : public ::silk::P_Controller_Descriptor
{
public:

  PD_Controller_Descriptor() noexcept {};
  virtual ~PD_Controller_Descriptor() noexcept {};

  void set_kp(float const& value);
  auto get_kp() const -> float const&;

  void set_kd(float const& value);
  auto get_kd() const -> float const&;

  void set_d_filter(float const& value);
  auto get_d_filter() const -> float const&;


private:

  float m_kp = {0.000000f};
  float m_kd = {0.000000f};
  float m_d_filter = {0.000000f};
};

struct PID_Controller_Descriptor : public ::silk::PD_Controller_Descriptor
{
public:

  PID_Controller_Descriptor() noexcept {};
  virtual ~PID_Controller_Descriptor() noexcept {};

  void set_kp(float const& value);
  auto get_kp() const -> float const&;

  void set_kd(float const& value);
  auto get_kd() const -> float const&;

  void set_d_filter(float const& value);
  auto get_d_filter() const -> float const&;

  void set_ki(float const& value);
  auto get_ki() const -> float const&;

  void set_max_i(float const& value);
  auto get_max_i() const -> float const&;


private:

  float m_kp = {0.000000f};
  float m_kd = {0.000000f};
  float m_d_filter = {0.000000f};
  float m_ki = {0.000000f};
  float m_max_i = {0.000000f};
};

struct ADC_Ammeter_Descriptor : public ::silk::INode_Descriptor
{
public:

  ADC_Ammeter_Descriptor() noexcept {};
  virtual ~ADC_Ammeter_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {100};
};

struct ADC_Ammeter_Config : public ::silk::INode_Config
{
public:

  ADC_Ammeter_Config() noexcept {};
  virtual ~ADC_Ammeter_Config() noexcept {};

  void set_scale(float const& value);
  auto get_scale() const -> float const&;

  void set_bias(float const& value);
  auto get_bias() const -> float const&;


private:

  float m_scale = {1.000000f};
  float m_bias = {0.000000f};
};

struct ADC_Voltmeter_Descriptor : public ::silk::INode_Descriptor
{
public:

  ADC_Voltmeter_Descriptor() noexcept {};
  virtual ~ADC_Voltmeter_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {100};
};

struct ADC_Voltmeter_Config : public ::silk::INode_Config
{
public:

  ADC_Voltmeter_Config() noexcept {};
  virtual ~ADC_Voltmeter_Config() noexcept {};

  void set_scale(float const& value);
  auto get_scale() const -> float const&;

  void set_bias(float const& value);
  auto get_bias() const -> float const&;


private:

  float m_scale = {1.000000f};
  float m_bias = {0.000000f};
};

struct ADS1115_Descriptor : public ::silk::INode_Descriptor
{
public:

  ADS1115_Descriptor() noexcept {};
  virtual ~ADS1115_Descriptor() noexcept {};

  void set_bus(std::string const& value);
  auto get_bus() const -> std::string const&;

  void set_adc0_rate(uint32_t const& value);
  auto get_adc0_rate() const -> uint32_t const&;

  void set_adc1_rate(uint32_t const& value);
  auto get_adc1_rate() const -> uint32_t const&;

  void set_adc2_rate(uint32_t const& value);
  auto get_adc2_rate() const -> uint32_t const&;

  void set_adc3_rate(uint32_t const& value);
  auto get_adc3_rate() const -> uint32_t const&;


private:

  std::string m_bus = {};
  uint32_t m_adc0_rate = {200};
  uint32_t m_adc1_rate = {200};
  uint32_t m_adc2_rate = {200};
  uint32_t m_adc3_rate = {200};
};

struct ADS1115_Config : public ::silk::INode_Config
{
public:

  ADS1115_Config() noexcept {};
  virtual ~ADS1115_Config() noexcept {};


private:

};

struct AVRADC_Descriptor : public ::silk::INode_Descriptor
{
public:

  AVRADC_Descriptor() noexcept {};
  virtual ~AVRADC_Descriptor() noexcept {};

  void set_bus(std::string const& value);
  auto get_bus() const -> std::string const&;

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  std::string m_bus = {};
  uint32_t m_rate = {200};
};

struct AVRADC_Config : public ::silk::INode_Config
{
public:

  AVRADC_Config() noexcept {};
  virtual ~AVRADC_Config() noexcept {};


private:

};

struct Comp_AHRS_Descriptor : public ::silk::INode_Descriptor
{
public:

  Comp_AHRS_Descriptor() noexcept {};
  virtual ~Comp_AHRS_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {200};
};

struct Comp_AHRS_Config : public ::silk::INode_Config
{
public:

  Comp_AHRS_Config() noexcept {};
  virtual ~Comp_AHRS_Config() noexcept {};

  void set_drift_correction_factor(float const& value);
  auto get_drift_correction_factor() const -> float const&;


private:

  float m_drift_correction_factor = {0.300000f};
};

struct Combiner_Descriptor : public ::silk::INode_Descriptor
{
public:

  Combiner_Descriptor() noexcept {};
  virtual ~Combiner_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {200};
};

struct Combiner_Config : public ::silk::INode_Config
{
public:

  Combiner_Config() noexcept {};
  virtual ~Combiner_Config() noexcept {};


private:

};

struct Gravity_Filter_Descriptor : public ::silk::INode_Descriptor
{
public:

  Gravity_Filter_Descriptor() noexcept {};
  virtual ~Gravity_Filter_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {1};
};

struct Gravity_Filter_Config : public ::silk::INode_Config
{
public:

  Gravity_Filter_Config() noexcept {};
  virtual ~Gravity_Filter_Config() noexcept {};


private:

};

struct KF_ECEF_Descriptor : public ::silk::INode_Descriptor
{
public:

  KF_ECEF_Descriptor() noexcept {};
  virtual ~KF_ECEF_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {1};
};

struct KF_ECEF_Config : public ::silk::INode_Config
{
public:

  KF_ECEF_Config() noexcept {};
  virtual ~KF_ECEF_Config() noexcept {};

  void set_gps_position_lag(float const& value);
  auto get_gps_position_lag() const -> float const&;

  void set_gps_position_accuracy(float const& value);
  auto get_gps_position_accuracy() const -> float const&;

  void set_gps_velocity_lag(float const& value);
  auto get_gps_velocity_lag() const -> float const&;

  void set_gps_velocity_accuracy(float const& value);
  auto get_gps_velocity_accuracy() const -> float const&;

  void set_acceleration_lag(float const& value);
  auto get_acceleration_lag() const -> float const&;

  void set_acceleration_accuracy(float const& value);
  auto get_acceleration_accuracy() const -> float const&;


private:

  float m_gps_position_lag = {0.000000f};
  float m_gps_position_accuracy = {2.000000f};
  float m_gps_velocity_lag = {0.000000f};
  float m_gps_velocity_accuracy = {0.200000f};
  float m_acceleration_lag = {0.000000f};
  float m_acceleration_accuracy = {2.000000f};
};

struct ENU_Frame_System_Descriptor : public ::silk::INode_Descriptor
{
public:

  ENU_Frame_System_Descriptor() noexcept {};
  virtual ~ENU_Frame_System_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {1};
};

struct ENU_Frame_System_Config : public ::silk::INode_Config
{
public:

  ENU_Frame_System_Config() noexcept {};
  virtual ~ENU_Frame_System_Config() noexcept {};


private:

};

struct LPF_Descriptor : public ::silk::INode_Descriptor
{
public:

  LPF_Descriptor() noexcept {};
  virtual ~LPF_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {1};
};

struct LPF_Config : public ::silk::INode_Config
{
public:

  LPF_Config() noexcept {};
  virtual ~LPF_Config() noexcept {};

  void set_poles(uint32_t const& value);
  auto get_poles() const -> uint32_t const&;

  void set_cutoff_frequency(float const& value);
  auto get_cutoff_frequency() const -> float const&;


private:

  uint32_t m_poles = {1};
  float m_cutoff_frequency = {1.000000f};
};

struct MaxSonar_Descriptor : public ::silk::INode_Descriptor
{
public:

  MaxSonar_Descriptor() noexcept {};
  virtual ~MaxSonar_Descriptor() noexcept {};

  void set_bus(std::string const& value);
  auto get_bus() const -> std::string const&;

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  std::string m_bus = {};
  uint32_t m_rate = {1};
};

struct MaxSonar_Config : public ::silk::INode_Config
{
public:

  MaxSonar_Config() noexcept {};
  virtual ~MaxSonar_Config() noexcept {};

  void set_direction(math::vec3<float> const& value);
  auto get_direction() const -> math::vec3<float> const&;

  void set_max_distance(float const& value);
  auto get_max_distance() const -> float const&;

  void set_min_distance(float const& value);
  auto get_min_distance() const -> float const&;


private:

  math::vec3<float> m_direction = {0.000000f, 0.000000f, 0.000000f};
  float m_max_distance = {10.000000f};
  float m_min_distance = {0.200000f};
};

struct Motor_Mixer_Descriptor : public ::silk::INode_Descriptor
{
public:

  Motor_Mixer_Descriptor() noexcept {};
  virtual ~Motor_Mixer_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {1};
};

struct Motor_Mixer_Config : public ::silk::INode_Config
{
public:

  Motor_Mixer_Config() noexcept {};
  virtual ~Motor_Mixer_Config() noexcept {};

  void set_armed_thrust(float const& value);
  auto get_armed_thrust() const -> float const&;


private:

  float m_armed_thrust = {0.000000f};
};

struct Acceleration_Calibration_Point
{
public:

  Acceleration_Calibration_Point() noexcept {};
  virtual ~Acceleration_Calibration_Point() noexcept {};

  void set_temperature(float const& value);
  auto get_temperature() const -> float const&;

  void set_bias(math::vec3<float> const& value);
  auto get_bias() const -> math::vec3<float> const&;

  void set_scale(math::vec3<float> const& value);
  auto get_scale() const -> math::vec3<float> const&;


private:

  float m_temperature = {0.000000f};
  math::vec3<float> m_bias = {0.000000f, 0.000000f, 0.000000f};
  math::vec3<float> m_scale = {1.000000f, 1.000000f, 1.000000f};
};

struct Angular_Velocity_Calibration_Point
{
public:

  Angular_Velocity_Calibration_Point() noexcept {};
  virtual ~Angular_Velocity_Calibration_Point() noexcept {};

  void set_temperature(float const& value);
  auto get_temperature() const -> float const&;

  void set_bias(math::vec3<float> const& value);
  auto get_bias() const -> math::vec3<float> const&;


private:

  float m_temperature = {0.000000f};
  math::vec3<float> m_bias = {0.000000f, 0.000000f, 0.000000f};
};

struct Magnetic_Field_Calibration_Point
{
public:

  Magnetic_Field_Calibration_Point() noexcept {};
  virtual ~Magnetic_Field_Calibration_Point() noexcept {};

  void set_temperature(float const& value);
  auto get_temperature() const -> float const&;

  void set_bias(math::vec3<float> const& value);
  auto get_bias() const -> math::vec3<float> const&;

  void set_scale(math::vec3<float> const& value);
  auto get_scale() const -> math::vec3<float> const&;


private:

  float m_temperature = {0.000000f};
  math::vec3<float> m_bias = {0.000000f, 0.000000f, 0.000000f};
  math::vec3<float> m_scale = {1.000000f, 1.000000f, 1.000000f};
};

struct MPU9250_Descriptor : public ::silk::INode_Descriptor
{
public:

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

  MPU9250_Descriptor() noexcept {};
  virtual ~MPU9250_Descriptor() noexcept {};

  void set_bus(std::string const& value);
  auto get_bus() const -> std::string const&;

  void set_imu_rate(imu_rate_t const& value);
  auto get_imu_rate() const -> imu_rate_t const&;

  void set_magnetometer_rate(uint32_t const& value);
  auto get_magnetometer_rate() const -> uint32_t const&;

  void set_thermometer_rate(uint32_t const& value);
  auto get_thermometer_rate() const -> uint32_t const&;

  void set_acceleration_range(acceleration_range_t const& value);
  auto get_acceleration_range() const -> acceleration_range_t const&;

  void set_angular_velocity_range(angular_velocity_range_t const& value);
  auto get_angular_velocity_range() const -> angular_velocity_range_t const&;


private:

  std::string m_bus = {};
  imu_rate_t m_imu_rate = {::silk::MPU9250_Descriptor::imu_rate_t::_1000};
  uint32_t m_magnetometer_rate = {100};
  uint32_t m_thermometer_rate = {10};
  acceleration_range_t m_acceleration_range = {::silk::MPU9250_Descriptor::acceleration_range_t::_8};
  angular_velocity_range_t m_angular_velocity_range = {::silk::MPU9250_Descriptor::angular_velocity_range_t::_500};
};

struct MPU9250_Config : public ::silk::INode_Config
{
public:

  struct Calibration
  {
  public:

    Calibration() noexcept {};
    virtual ~Calibration() noexcept {};

    void set_acceleration(std::vector<::silk::Acceleration_Calibration_Point> const& value);
    auto get_acceleration() const -> std::vector<::silk::Acceleration_Calibration_Point> const&;
    auto get_acceleration() -> std::vector<::silk::Acceleration_Calibration_Point>&;

    void set_angular_velocity(std::vector<::silk::Angular_Velocity_Calibration_Point> const& value);
    auto get_angular_velocity() const -> std::vector<::silk::Angular_Velocity_Calibration_Point> const&;
    auto get_angular_velocity() -> std::vector<::silk::Angular_Velocity_Calibration_Point>&;

    void set_magnetic_field(std::vector<::silk::Magnetic_Field_Calibration_Point> const& value);
    auto get_magnetic_field() const -> std::vector<::silk::Magnetic_Field_Calibration_Point> const&;
    auto get_magnetic_field() -> std::vector<::silk::Magnetic_Field_Calibration_Point>&;


  private:

    std::vector<::silk::Acceleration_Calibration_Point> m_acceleration = {};
    std::vector<::silk::Angular_Velocity_Calibration_Point> m_angular_velocity = {};
    std::vector<::silk::Magnetic_Field_Calibration_Point> m_magnetic_field = {};
  };

  MPU9250_Config() noexcept {};
  virtual ~MPU9250_Config() noexcept {};

  void set_rotation(math::vec3<float> const& value);
  auto get_rotation() const -> math::vec3<float> const&;

  void set_calibration(Calibration const& value);
  auto get_calibration() const -> Calibration const&;
  auto get_calibration() -> Calibration&;


private:

  math::vec3<float> m_rotation = {0.000000f, 0.000000f, 0.000000f};
  Calibration m_calibration = {};
};

struct MS5611_Descriptor : public ::silk::INode_Descriptor
{
public:

  MS5611_Descriptor() noexcept {};
  virtual ~MS5611_Descriptor() noexcept {};

  void set_bus(std::string const& value);
  auto get_bus() const -> std::string const&;

  void set_pressure_rate(uint32_t const& value);
  auto get_pressure_rate() const -> uint32_t const&;

  void set_temperature_rate(uint32_t const& value);
  auto get_temperature_rate() const -> uint32_t const&;


private:

  std::string m_bus = {};
  uint32_t m_pressure_rate = {100};
  uint32_t m_temperature_rate = {10};
};

struct MS5611_Config : public ::silk::INode_Config
{
public:

  MS5611_Config() noexcept {};
  virtual ~MS5611_Config() noexcept {};


private:

};

struct Multirotor_Brain_Descriptor : public ::silk::INode_Descriptor
{
public:

  Multirotor_Brain_Descriptor() noexcept {};
  virtual ~Multirotor_Brain_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;

  void set_commands_rate(uint32_t const& value);
  auto get_commands_rate() const -> uint32_t const&;

  void set_state_rate(uint32_t const& value);
  auto get_state_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {100};
  uint32_t m_commands_rate = {30};
  uint32_t m_state_rate = {30};
};

struct Multirotor_Brain_Config : public ::silk::INode_Config
{
public:

  struct Horizontal_Angle
  {
  public:

    struct Combined_XY_PIDs : public ::silk::PD_Controller_Descriptor
    {
    public:

      Combined_XY_PIDs() noexcept {};
      virtual ~Combined_XY_PIDs() noexcept {};

      void set_kp(float const& value);
      auto get_kp() const -> float const&;

      void set_kd(float const& value);
      auto get_kd() const -> float const&;

      void set_d_filter(float const& value);
      auto get_d_filter() const -> float const&;

      void set_ki(float const& value);
      auto get_ki() const -> float const&;

      void set_max_i(float const& value);
      auto get_max_i() const -> float const&;


    private:

      float m_kp = {0.000000f};
      float m_kd = {0.000000f};
      float m_d_filter = {0.000000f};
      float m_ki = {0.000000f};
      float m_max_i = {0.000000f};
    };

    struct Separate_XY_PIDs
    {
    public:

      Separate_XY_PIDs() noexcept {};
      virtual ~Separate_XY_PIDs() noexcept {};

      void set_x_pid(::silk::PID_Controller_Descriptor const& value);
      auto get_x_pid() const -> ::silk::PID_Controller_Descriptor const&;
      auto get_x_pid() -> ::silk::PID_Controller_Descriptor&;

      void set_y_pid(::silk::PID_Controller_Descriptor const& value);
      auto get_y_pid() const -> ::silk::PID_Controller_Descriptor const&;
      auto get_y_pid() -> ::silk::PID_Controller_Descriptor&;


    private:

      ::silk::PID_Controller_Descriptor m_x_pid = {};
      ::silk::PID_Controller_Descriptor m_y_pid = {};
    };

    Horizontal_Angle() noexcept {};
    virtual ~Horizontal_Angle() noexcept {};

    void set_max_speed_deg(float const& value);
    auto get_max_speed_deg() const -> float const&;

    void set_xy_pids(boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const& value);
    auto get_xy_pids() const -> boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const&;
    auto get_xy_pids() -> boost::variant<Combined_XY_PIDs,Separate_XY_PIDs>&;


  private:

    float m_max_speed_deg = {180.000000f};
    boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> m_xy_pids = {};
  };

  struct Horizontal_Position
  {
  public:

    Horizontal_Position() noexcept {};
    virtual ~Horizontal_Position() noexcept {};

    void set_max_speed(float const& value);
    auto get_max_speed() const -> float const&;

    void set_lpf(::silk::LPF_Config const& value);
    auto get_lpf() const -> ::silk::LPF_Config const&;
    auto get_lpf() -> ::silk::LPF_Config&;

    void set_velocity_pi(::silk::PI_Controller_Descriptor const& value);
    auto get_velocity_pi() const -> ::silk::PI_Controller_Descriptor const&;
    auto get_velocity_pi() -> ::silk::PI_Controller_Descriptor&;

    void set_position_p(::silk::P_Controller_Descriptor const& value);
    auto get_position_p() const -> ::silk::P_Controller_Descriptor const&;
    auto get_position_p() -> ::silk::P_Controller_Descriptor&;


  private:

    float m_max_speed = {2.000000f};
    ::silk::LPF_Config m_lpf = {};
    ::silk::PI_Controller_Descriptor m_velocity_pi = {};
    ::silk::P_Controller_Descriptor m_position_p = {};
  };

  struct Yaw_Angle
  {
  public:

    Yaw_Angle() noexcept {};
    virtual ~Yaw_Angle() noexcept {};

    void set_max_speed_deg(float const& value);
    auto get_max_speed_deg() const -> float const&;

    void set_pid(::silk::PID_Controller_Descriptor const& value);
    auto get_pid() const -> ::silk::PID_Controller_Descriptor const&;
    auto get_pid() -> ::silk::PID_Controller_Descriptor&;


  private:

    float m_max_speed_deg = {180.000000f};
    ::silk::PID_Controller_Descriptor m_pid = {};
  };

  struct Altitude
  {
  public:

    Altitude() noexcept {};
    virtual ~Altitude() noexcept {};

    void set_max_speed(float const& value);
    auto get_max_speed() const -> float const&;

    void set_lpf(::silk::LPF_Config const& value);
    auto get_lpf() const -> ::silk::LPF_Config const&;
    auto get_lpf() -> ::silk::LPF_Config&;

    void set_speed_pi(::silk::PI_Controller_Descriptor const& value);
    auto get_speed_pi() const -> ::silk::PI_Controller_Descriptor const&;
    auto get_speed_pi() -> ::silk::PI_Controller_Descriptor&;

    void set_position_p(::silk::P_Controller_Descriptor const& value);
    auto get_position_p() const -> ::silk::P_Controller_Descriptor const&;
    auto get_position_p() -> ::silk::P_Controller_Descriptor&;


  private:

    float m_max_speed = {2.000000f};
    ::silk::LPF_Config m_lpf = {};
    ::silk::PI_Controller_Descriptor m_speed_pi = {};
    ::silk::P_Controller_Descriptor m_position_p = {};
  };

  Multirotor_Brain_Config() noexcept {};
  virtual ~Multirotor_Brain_Config() noexcept {};

  void set_max_thrust(float const& value);
  auto get_max_thrust() const -> float const&;

  void set_min_thrust(float const& value);
  auto get_min_thrust() const -> float const&;

  void set_horizontal_angle(Horizontal_Angle const& value);
  auto get_horizontal_angle() const -> Horizontal_Angle const&;
  auto get_horizontal_angle() -> Horizontal_Angle&;

  void set_horizontal_position(Horizontal_Position const& value);
  auto get_horizontal_position() const -> Horizontal_Position const&;
  auto get_horizontal_position() -> Horizontal_Position&;

  void set_yaw_angle(Yaw_Angle const& value);
  auto get_yaw_angle() const -> Yaw_Angle const&;
  auto get_yaw_angle() -> Yaw_Angle&;

  void set_altitude(Altitude const& value);
  auto get_altitude() const -> Altitude const&;
  auto get_altitude() -> Altitude&;


private:

  float m_max_thrust = {2.000000f};
  float m_min_thrust = {1.000000f};
  Horizontal_Angle m_horizontal_angle = {};
  Horizontal_Position m_horizontal_position = {};
  Yaw_Angle m_yaw_angle = {};
  Altitude m_altitude = {};
};

struct Multirotor_Pilot_Descriptor : public ::silk::INode_Descriptor
{
public:

  Multirotor_Pilot_Descriptor() noexcept {};
  virtual ~Multirotor_Pilot_Descriptor() noexcept {};

  void set_commands_rate(uint32_t const& value);
  auto get_commands_rate() const -> uint32_t const&;

  void set_state_rate(uint32_t const& value);
  auto get_state_rate() const -> uint32_t const&;

  void set_video_rate(uint32_t const& value);
  auto get_video_rate() const -> uint32_t const&;


private:

  uint32_t m_commands_rate = {30};
  uint32_t m_state_rate = {30};
  uint32_t m_video_rate = {30};
};

struct Multirotor_Pilot_Config : public ::silk::INode_Config
{
public:

  Multirotor_Pilot_Config() noexcept {};
  virtual ~Multirotor_Pilot_Config() noexcept {};


private:

};

struct Multirotor_Simulator_Descriptor : public ::silk::INode_Descriptor
{
public:

  Multirotor_Simulator_Descriptor() noexcept {};
  virtual ~Multirotor_Simulator_Descriptor() noexcept {};

  void set_throttle_rate(uint32_t const& value);
  auto get_throttle_rate() const -> uint32_t const&;

  void set_acceleration_rate(uint32_t const& value);
  auto get_acceleration_rate() const -> uint32_t const&;

  void set_angular_velocity_rate(uint32_t const& value);
  auto get_angular_velocity_rate() const -> uint32_t const&;

  void set_magnetic_field_rate(uint32_t const& value);
  auto get_magnetic_field_rate() const -> uint32_t const&;

  void set_pressure_rate(uint32_t const& value);
  auto get_pressure_rate() const -> uint32_t const&;

  void set_temperature_rate(uint32_t const& value);
  auto get_temperature_rate() const -> uint32_t const&;

  void set_distance_rate(uint32_t const& value);
  auto get_distance_rate() const -> uint32_t const&;

  void set_gps_rate(uint32_t const& value);
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

struct Multirotor_Simulator_Config : public ::silk::INode_Config
{
public:

  struct Noise
  {
  public:

    Noise() noexcept {};
    virtual ~Noise() noexcept {};

    void set_gps_position(float const& value);
    auto get_gps_position() const -> float const&;

    void set_gps_velocity(float const& value);
    auto get_gps_velocity() const -> float const&;

    void set_gps_pacc(float const& value);
    auto get_gps_pacc() const -> float const&;

    void set_gps_vacc(float const& value);
    auto get_gps_vacc() const -> float const&;

    void set_acceleration(float const& value);
    auto get_acceleration() const -> float const&;

    void set_angular_velocity(float const& value);
    auto get_angular_velocity() const -> float const&;

    void set_magnetic_field(float const& value);
    auto get_magnetic_field() const -> float const&;

    void set_pressure(float const& value);
    auto get_pressure() const -> float const&;

    void set_temperature(float const& value);
    auto get_temperature() const -> float const&;

    void set_ground_distance(float const& value);
    auto get_ground_distance() const -> float const&;


  private:

    float m_gps_position = {2.000000f};
    float m_gps_velocity = {0.200000f};
    float m_gps_pacc = {0.500000f};
    float m_gps_vacc = {0.100000f};
    float m_acceleration = {0.010000f};
    float m_angular_velocity = {0.010000f};
    float m_magnetic_field = {0.100000f};
    float m_pressure = {0.100000f};
    float m_temperature = {0.100000f};
    float m_ground_distance = {0.010000f};
  };

  Multirotor_Simulator_Config() noexcept {};
  virtual ~Multirotor_Simulator_Config() noexcept {};

  void set_simulation_enabled(bool const& value);
  auto get_simulation_enabled() const -> bool const&;

  void set_drag_enabled(bool const& value);
  auto get_drag_enabled() const -> bool const&;

  void set_ground_enabled(bool const& value);
  auto get_ground_enabled() const -> bool const&;

  void set_gravity_enabled(bool const& value);
  auto get_gravity_enabled() const -> bool const&;

  void set_noise(Noise const& value);
  auto get_noise() const -> Noise const&;
  auto get_noise() -> Noise&;


private:

  bool m_simulation_enabled = {true};
  bool m_drag_enabled = {true};
  bool m_ground_enabled = {true};
  bool m_gravity_enabled = {true};
  Noise m_noise = {};
};

struct Oscillator_Descriptor : public ::silk::INode_Descriptor
{
public:

  Oscillator_Descriptor() noexcept {};
  virtual ~Oscillator_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;

  void set_component_count(int32_t const& value);
  auto get_component_count() const -> int32_t const&;


private:

  uint32_t m_rate = {30};
  int32_t m_component_count = {1};
};

struct Oscillator_Config : public ::silk::INode_Config
{
public:

  struct Component
  {
  public:

    Component() noexcept {};
    virtual ~Component() noexcept {};

    void set_frequency(float const& value);
    auto get_frequency() const -> float const&;

    void set_amplitude(float const& value);
    auto get_amplitude() const -> float const&;

    void set_square(bool const& value);
    auto get_square() const -> bool const&;


  private:

    float m_frequency = {1.000000f};
    float m_amplitude = {1.000000f};
    bool m_square = {false};
  };

  Oscillator_Config() noexcept {};
  virtual ~Oscillator_Config() noexcept {};

  void set_amplitude(float const& value);
  auto get_amplitude() const -> float const&;

  void set_noise(float const& value);
  auto get_noise() const -> float const&;

  void set_components(std::vector<::silk::Oscillator_Config::Component> const& value);
  auto get_components() const -> std::vector<::silk::Oscillator_Config::Component> const&;
  auto get_components() -> std::vector<::silk::Oscillator_Config::Component>&;


private:

  float m_amplitude = {1.000000f};
  float m_noise = {0.000000f};
  std::vector<::silk::Oscillator_Config::Component> m_components = {};
};

struct PCA9685_Descriptor : public ::silk::INode_Descriptor
{
public:

  struct Channel
  {
  public:

    Channel() noexcept {};
    virtual ~Channel() noexcept {};

    void set_enabled(bool const& value);
    auto get_enabled() const -> bool const&;

    void set_servo_signal(bool const& value);
    auto get_servo_signal() const -> bool const&;


  private:

    bool m_enabled = {false};
    bool m_servo_signal = {false};
  };

  PCA9685_Descriptor() noexcept {};
  virtual ~PCA9685_Descriptor() noexcept {};

  void set_bus(std::string const& value);
  auto get_bus() const -> std::string const&;

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;

  void set_address(uint8_t const& value);
  auto get_address() const -> uint8_t const&;

  void set_channels(std::vector<::silk::PCA9685_Descriptor::Channel> const& value);
  auto get_channels() const -> std::vector<::silk::PCA9685_Descriptor::Channel> const&;
  auto get_channels() -> std::vector<::silk::PCA9685_Descriptor::Channel>&;


private:

  std::string m_bus = {};
  uint32_t m_rate = {30};
  uint8_t m_address = {64};
  std::vector<::silk::PCA9685_Descriptor::Channel> m_channels = {};
};

struct PCA9685_Config : public ::silk::INode_Config
{
public:

  struct IChannel
  {
  public:

    IChannel() noexcept {};
    virtual ~IChannel() noexcept {};


  private:

  };

  struct Servo_Channel : public ::silk::PCA9685_Config::IChannel
  {
  public:

    Servo_Channel() noexcept {};
    virtual ~Servo_Channel() noexcept {};

    void set_min(float const& value);
    auto get_min() const -> float const&;

    void set_max(float const& value);
    auto get_max() const -> float const&;


  private:

    float m_min = {1.000000f};
    float m_max = {2.000000f};
  };

  struct PWM_Channel : public ::silk::PCA9685_Config::IChannel
  {
  public:

    PWM_Channel() noexcept {};
    virtual ~PWM_Channel() noexcept {};

    void set_min(float const& value);
    auto get_min() const -> float const&;

    void set_max(float const& value);
    auto get_max() const -> float const&;


  private:

    float m_min = {0.000000f};
    float m_max = {1.000000f};
  };

  PCA9685_Config() noexcept {};
  virtual ~PCA9685_Config() noexcept {};

  void set_channels(std::vector<std::shared_ptr<::silk::PCA9685_Config::IChannel>> const& value);
  auto get_channels() const -> std::vector<std::shared_ptr<::silk::PCA9685_Config::IChannel>> const&;
  auto get_channels() -> std::vector<std::shared_ptr<::silk::PCA9685_Config::IChannel>>&;


private:

  std::vector<std::shared_ptr<::silk::PCA9685_Config::IChannel>> m_channels = {};
};

struct PIGPIO_Descriptor : public ::silk::INode_Descriptor
{
public:

  struct Channel
  {
  public:

    Channel() noexcept {};
    virtual ~Channel() noexcept {};

    void set_enabled(bool const& value);
    auto get_enabled() const -> bool const&;

    void set_servo_signal(bool const& value);
    auto get_servo_signal() const -> bool const&;

    void set_rate(uint32_t const& value);
    auto get_rate() const -> uint32_t const&;


  private:

    bool m_enabled = {false};
    bool m_servo_signal = {false};
    uint32_t m_rate = {400};
  };

  PIGPIO_Descriptor() noexcept {};
  virtual ~PIGPIO_Descriptor() noexcept {};

  void set_gpio_2(Channel const& value);
  auto get_gpio_2() const -> Channel const&;
  auto get_gpio_2() -> Channel&;

  void set_gpio_3(Channel const& value);
  auto get_gpio_3() const -> Channel const&;
  auto get_gpio_3() -> Channel&;

  void set_gpio_4(Channel const& value);
  auto get_gpio_4() const -> Channel const&;
  auto get_gpio_4() -> Channel&;

  void set_gpio_5(Channel const& value);
  auto get_gpio_5() const -> Channel const&;
  auto get_gpio_5() -> Channel&;

  void set_gpio_6(Channel const& value);
  auto get_gpio_6() const -> Channel const&;
  auto get_gpio_6() -> Channel&;

  void set_gpio_7(Channel const& value);
  auto get_gpio_7() const -> Channel const&;
  auto get_gpio_7() -> Channel&;

  void set_gpio_8(Channel const& value);
  auto get_gpio_8() const -> Channel const&;
  auto get_gpio_8() -> Channel&;

  void set_gpio_9(Channel const& value);
  auto get_gpio_9() const -> Channel const&;
  auto get_gpio_9() -> Channel&;

  void set_gpio_10(Channel const& value);
  auto get_gpio_10() const -> Channel const&;
  auto get_gpio_10() -> Channel&;

  void set_gpio_11(Channel const& value);
  auto get_gpio_11() const -> Channel const&;
  auto get_gpio_11() -> Channel&;

  void set_gpio_12(Channel const& value);
  auto get_gpio_12() const -> Channel const&;
  auto get_gpio_12() -> Channel&;

  void set_gpio_13(Channel const& value);
  auto get_gpio_13() const -> Channel const&;
  auto get_gpio_13() -> Channel&;

  void set_gpio_14(Channel const& value);
  auto get_gpio_14() const -> Channel const&;
  auto get_gpio_14() -> Channel&;

  void set_gpio_15(Channel const& value);
  auto get_gpio_15() const -> Channel const&;
  auto get_gpio_15() -> Channel&;

  void set_gpio_16(Channel const& value);
  auto get_gpio_16() const -> Channel const&;
  auto get_gpio_16() -> Channel&;

  void set_gpio_17(Channel const& value);
  auto get_gpio_17() const -> Channel const&;
  auto get_gpio_17() -> Channel&;

  void set_gpio_18(Channel const& value);
  auto get_gpio_18() const -> Channel const&;
  auto get_gpio_18() -> Channel&;

  void set_gpio_19(Channel const& value);
  auto get_gpio_19() const -> Channel const&;
  auto get_gpio_19() -> Channel&;

  void set_gpio_20(Channel const& value);
  auto get_gpio_20() const -> Channel const&;
  auto get_gpio_20() -> Channel&;

  void set_gpio_21(Channel const& value);
  auto get_gpio_21() const -> Channel const&;
  auto get_gpio_21() -> Channel&;

  void set_gpio_22(Channel const& value);
  auto get_gpio_22() const -> Channel const&;
  auto get_gpio_22() -> Channel&;

  void set_gpio_23(Channel const& value);
  auto get_gpio_23() const -> Channel const&;
  auto get_gpio_23() -> Channel&;

  void set_gpio_24(Channel const& value);
  auto get_gpio_24() const -> Channel const&;
  auto get_gpio_24() -> Channel&;

  void set_gpio_25(Channel const& value);
  auto get_gpio_25() const -> Channel const&;
  auto get_gpio_25() -> Channel&;

  void set_gpio_26(Channel const& value);
  auto get_gpio_26() const -> Channel const&;
  auto get_gpio_26() -> Channel&;

  void set_gpio_27(Channel const& value);
  auto get_gpio_27() const -> Channel const&;
  auto get_gpio_27() -> Channel&;


private:

  Channel m_gpio_2 = {};
  Channel m_gpio_3 = {};
  Channel m_gpio_4 = {};
  Channel m_gpio_5 = {};
  Channel m_gpio_6 = {};
  Channel m_gpio_7 = {};
  Channel m_gpio_8 = {};
  Channel m_gpio_9 = {};
  Channel m_gpio_10 = {};
  Channel m_gpio_11 = {};
  Channel m_gpio_12 = {};
  Channel m_gpio_13 = {};
  Channel m_gpio_14 = {};
  Channel m_gpio_15 = {};
  Channel m_gpio_16 = {};
  Channel m_gpio_17 = {};
  Channel m_gpio_18 = {};
  Channel m_gpio_19 = {};
  Channel m_gpio_20 = {};
  Channel m_gpio_21 = {};
  Channel m_gpio_22 = {};
  Channel m_gpio_23 = {};
  Channel m_gpio_24 = {};
  Channel m_gpio_25 = {};
  Channel m_gpio_26 = {};
  Channel m_gpio_27 = {};
};

struct PIGPIO_Config : public ::silk::INode_Config
{
public:

  struct IChannel
  {
  public:

    IChannel() noexcept {};
    virtual ~IChannel() noexcept {};


  private:

  };

  struct Servo_Channel : public ::silk::PIGPIO_Config::IChannel
  {
  public:

    Servo_Channel() noexcept {};
    virtual ~Servo_Channel() noexcept {};

    void set_min(float const& value);
    auto get_min() const -> float const&;

    void set_max(float const& value);
    auto get_max() const -> float const&;


  private:

    float m_min = {1.000000f};
    float m_max = {2.000000f};
  };

  struct PWM_Channel : public ::silk::PIGPIO_Config::IChannel
  {
  public:

    PWM_Channel() noexcept {};
    virtual ~PWM_Channel() noexcept {};

    void set_min(float const& value);
    auto get_min() const -> float const&;

    void set_max(float const& value);
    auto get_max() const -> float const&;


  private:

    float m_min = {0.000000f};
    float m_max = {1.000000f};
  };

  PIGPIO_Config() noexcept {};
  virtual ~PIGPIO_Config() noexcept {};

  void set_gpio_2(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_2() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_2() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_3(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_3() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_3() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_4(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_4() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_4() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_5(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_5() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_5() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_6(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_6() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_6() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_7(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_7() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_7() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_8(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_8() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_8() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_9(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_9() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_9() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_10(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_10() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_10() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_11(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_11() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_11() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_12(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_12() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_12() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_13(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_13() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_13() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_14(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_14() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_14() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_15(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_15() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_15() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_16(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_16() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_16() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_17(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_17() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_17() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_18(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_18() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_18() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_19(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_19() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_19() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_20(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_20() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_20() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_21(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_21() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_21() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_22(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_22() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_22() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_23(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_23() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_23() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_24(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_24() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_24() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_25(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_25() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_25() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_26(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_26() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_26() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;

  void set_gpio_27(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
  auto get_gpio_27() const -> std::shared_ptr<::silk::PIGPIO_Config::IChannel> const&;
  auto get_gpio_27() -> std::shared_ptr<::silk::PIGPIO_Config::IChannel>&;


private:

  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_2 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_3 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_4 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_5 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_6 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_7 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_8 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_9 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_10 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_11 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_12 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_13 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_14 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_15 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_16 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_17 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_18 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_19 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_20 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_21 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_22 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_23 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_24 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_25 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_26 = {};
  std::shared_ptr<::silk::PIGPIO_Config::IChannel> m_gpio_27 = {};
};

struct Pressure_Velocity_Descriptor : public ::silk::INode_Descriptor
{
public:

  Pressure_Velocity_Descriptor() noexcept {};
  virtual ~Pressure_Velocity_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {1};
};

struct Pressure_Velocity_Config : public ::silk::INode_Config
{
public:

  Pressure_Velocity_Config() noexcept {};
  virtual ~Pressure_Velocity_Config() noexcept {};


private:

};

struct Proximity_Descriptor : public ::silk::INode_Descriptor
{
public:

  Proximity_Descriptor() noexcept {};
  virtual ~Proximity_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;

  void set_channel_count(int32_t const& value);
  auto get_channel_count() const -> int32_t const&;


private:

  uint32_t m_rate = {1};
  int32_t m_channel_count = {1};
};

struct Proximity_Config : public ::silk::INode_Config
{
public:

  Proximity_Config() noexcept {};
  virtual ~Proximity_Config() noexcept {};


private:

};

struct Rate_Controller_Descriptor : public ::silk::INode_Descriptor
{
public:

  Rate_Controller_Descriptor() noexcept {};
  virtual ~Rate_Controller_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {1};
};

struct Rate_Controller_Config : public ::silk::INode_Config
{
public:

  struct Feedback
  {
  public:

    struct Combined_XY_PIDs : public ::silk::PD_Controller_Descriptor
    {
    public:

      Combined_XY_PIDs() noexcept {};
      virtual ~Combined_XY_PIDs() noexcept {};

      void set_kp(float const& value);
      auto get_kp() const -> float const&;

      void set_kd(float const& value);
      auto get_kd() const -> float const&;

      void set_d_filter(float const& value);
      auto get_d_filter() const -> float const&;

      void set_ki(float const& value);
      auto get_ki() const -> float const&;

      void set_max_i(float const& value);
      auto get_max_i() const -> float const&;


    private:

      float m_kp = {0.000000f};
      float m_kd = {0.000000f};
      float m_d_filter = {0.000000f};
      float m_ki = {0.000000f};
      float m_max_i = {0.000000f};
    };

    struct Separate_XY_PIDs
    {
    public:

      Separate_XY_PIDs() noexcept {};
      virtual ~Separate_XY_PIDs() noexcept {};

      void set_x_pid(::silk::PID_Controller_Descriptor const& value);
      auto get_x_pid() const -> ::silk::PID_Controller_Descriptor const&;
      auto get_x_pid() -> ::silk::PID_Controller_Descriptor&;

      void set_y_pid(::silk::PID_Controller_Descriptor const& value);
      auto get_y_pid() const -> ::silk::PID_Controller_Descriptor const&;
      auto get_y_pid() -> ::silk::PID_Controller_Descriptor&;


    private:

      ::silk::PID_Controller_Descriptor m_x_pid = {};
      ::silk::PID_Controller_Descriptor m_y_pid = {};
    };

    Feedback() noexcept {};
    virtual ~Feedback() noexcept {};

    void set_weight(float const& value);
    auto get_weight() const -> float const&;

    void set_xy_pids(boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const& value);
    auto get_xy_pids() const -> boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const&;
    auto get_xy_pids() -> boost::variant<Combined_XY_PIDs,Separate_XY_PIDs>&;

    void set_z_pid(::silk::PID_Controller_Descriptor const& value);
    auto get_z_pid() const -> ::silk::PID_Controller_Descriptor const&;
    auto get_z_pid() -> ::silk::PID_Controller_Descriptor&;


  private:

    float m_weight = {1.000000f};
    boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> m_xy_pids = {};
    ::silk::PID_Controller_Descriptor m_z_pid = {};
  };

  struct Feedforward
  {
  public:

    Feedforward() noexcept {};
    virtual ~Feedforward() noexcept {};

    void set_weight(float const& value);
    auto get_weight() const -> float const&;

    void set_max_torque(float const& value);
    auto get_max_torque() const -> float const&;


  private:

    float m_weight = {0.000000f};
    float m_max_torque = {1.000000f};
  };

  Rate_Controller_Config() noexcept {};
  virtual ~Rate_Controller_Config() noexcept {};

  void set_feedback(Feedback const& value);
  auto get_feedback() const -> Feedback const&;
  auto get_feedback() -> Feedback&;

  void set_feedforward(Feedforward const& value);
  auto get_feedforward() const -> Feedforward const&;
  auto get_feedforward() -> Feedforward&;


private:

  Feedback m_feedback = {};
  Feedforward m_feedforward = {};
};

struct Raspicam_Descriptor : public ::silk::INode_Descriptor
{
public:

  struct Quality
  {
  public:

    Quality() noexcept {};
    virtual ~Quality() noexcept {};

    void set_resolution(math::vec2<int64_t> const& value);
    auto get_resolution() const -> math::vec2<int64_t> const&;

    void set_bitrate(int32_t const& value);
    auto get_bitrate() const -> int32_t const&;


  private:

    math::vec2<int64_t> m_resolution = {0, 0};
    int32_t m_bitrate = {0};
  };

  Raspicam_Descriptor() noexcept {};
  virtual ~Raspicam_Descriptor() noexcept {};

  void set_fps(int32_t const& value);
  auto get_fps() const -> int32_t const&;

  void set_streaming_low(Quality const& value);
  auto get_streaming_low() const -> Quality const&;
  auto get_streaming_low() -> Quality&;

  void set_streaming_high(Quality const& value);
  auto get_streaming_high() const -> Quality const&;
  auto get_streaming_high() -> Quality&;

  void set_recording(Quality const& value);
  auto get_recording() const -> Quality const&;
  auto get_recording() -> Quality&;


private:

  int32_t m_fps = {0};
  Quality m_streaming_low = {};
  Quality m_streaming_high = {};
  Quality m_recording = {};
};

struct Raspicam_Config : public ::silk::INode_Config
{
public:

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

  Raspicam_Config() noexcept {};
  virtual ~Raspicam_Config() noexcept {};

  void set_iso(int32_t const& value);
  auto get_iso() const -> int32_t const&;

  void set_shutter_speed(float const& value);
  auto get_shutter_speed() const -> float const&;

  void set_ev(int32_t const& value);
  auto get_ev() const -> int32_t const&;

  void set_sharpness(int32_t const& value);
  auto get_sharpness() const -> int32_t const&;

  void set_contrast(int32_t const& value);
  auto get_contrast() const -> int32_t const&;

  void set_brightness(int32_t const& value);
  auto get_brightness() const -> int32_t const&;

  void set_saturation(int32_t const& value);
  auto get_saturation() const -> int32_t const&;

  void set_awb_mode(awb_mode_t const& value);
  auto get_awb_mode() const -> awb_mode_t const&;

  void set_quality(int32_t const& value);
  auto get_quality() const -> int32_t const&;

  void set_recording(bool const& value);
  auto get_recording() const -> bool const&;


private:

  int32_t m_iso = {0};
  float m_shutter_speed = {0.000000f};
  int32_t m_ev = {0};
  int32_t m_sharpness = {0};
  int32_t m_contrast = {0};
  int32_t m_brightness = {0};
  int32_t m_saturation = {0};
  awb_mode_t m_awb_mode = {::silk::Raspicam_Config::awb_mode_t::AUTO};
  int32_t m_quality = {0};
  bool m_recording = {false};
};

struct RC5T619_Descriptor : public ::silk::INode_Descriptor
{
public:

  RC5T619_Descriptor() noexcept {};
  virtual ~RC5T619_Descriptor() noexcept {};

  void set_bus(std::string const& value);
  auto get_bus() const -> std::string const&;

  void set_adc0_rate(uint32_t const& value);
  auto get_adc0_rate() const -> uint32_t const&;

  void set_adc1_rate(uint32_t const& value);
  auto get_adc1_rate() const -> uint32_t const&;


private:

  std::string m_bus = {};
  uint32_t m_adc0_rate = {50};
  uint32_t m_adc1_rate = {5};
};

struct RC5T619_Config : public ::silk::INode_Config
{
public:

  RC5T619_Config() noexcept {};
  virtual ~RC5T619_Config() noexcept {};


private:

};

struct Resampler_Descriptor : public ::silk::INode_Descriptor
{
public:

  Resampler_Descriptor() noexcept {};
  virtual ~Resampler_Descriptor() noexcept {};

  void set_input_rate(uint32_t const& value);
  auto get_input_rate() const -> uint32_t const&;

  void set_output_rate(uint32_t const& value);
  auto get_output_rate() const -> uint32_t const&;


private:

  uint32_t m_input_rate = {1};
  uint32_t m_output_rate = {1};
};

struct Resampler_Config : public ::silk::INode_Config
{
public:

  Resampler_Config() noexcept {};
  virtual ~Resampler_Config() noexcept {};

  void set_lpf(::silk::LPF_Config const& value);
  auto get_lpf() const -> ::silk::LPF_Config const&;
  auto get_lpf() -> ::silk::LPF_Config&;


private:

  ::silk::LPF_Config m_lpf = {};
};

struct Scalar_Generator_Descriptor : public ::silk::INode_Descriptor
{
public:

  Scalar_Generator_Descriptor() noexcept {};
  virtual ~Scalar_Generator_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {1};
};

struct Scalar_Generator_Config : public ::silk::INode_Config
{
public:

  Scalar_Generator_Config() noexcept {};
  virtual ~Scalar_Generator_Config() noexcept {};

  void set_value(float const& value);
  auto get_value() const -> float const&;


private:

  float m_value = {0.000000f};
};

struct Servo_Gimbal_Descriptor : public ::silk::INode_Descriptor
{
public:

  Servo_Gimbal_Descriptor() noexcept {};
  virtual ~Servo_Gimbal_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;

  void set_commands_rate(uint32_t const& value);
  auto get_commands_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {1};
  uint32_t m_commands_rate = {1};
};

struct Servo_Gimbal_Config : public ::silk::INode_Config
{
public:

  struct Channel
  {
  public:

    Channel() noexcept {};
    virtual ~Channel() noexcept {};

    void set_min_angle(float const& value);
    auto get_min_angle() const -> float const&;

    void set_max_angle(float const& value);
    auto get_max_angle() const -> float const&;

    void set_min_pwm(float const& value);
    auto get_min_pwm() const -> float const&;

    void set_max_pwm(float const& value);
    auto get_max_pwm() const -> float const&;


  private:

    float m_min_angle = {-180.000000f};
    float m_max_angle = {180.000000f};
    float m_min_pwm = {0.000000f};
    float m_max_pwm = {1.000000f};
  };

  Servo_Gimbal_Config() noexcept {};
  virtual ~Servo_Gimbal_Config() noexcept {};

  void set_x_channel(Channel const& value);
  auto get_x_channel() const -> Channel const&;
  auto get_x_channel() -> Channel&;

  void set_y_channel(Channel const& value);
  auto get_y_channel() const -> Channel const&;
  auto get_y_channel() -> Channel&;

  void set_z_channel(Channel const& value);
  auto get_z_channel() const -> Channel const&;
  auto get_z_channel() -> Channel&;


private:

  Channel m_x_channel = {};
  Channel m_y_channel = {};
  Channel m_z_channel = {};
};

struct SRF01_Descriptor : public ::silk::INode_Descriptor
{
public:

  SRF01_Descriptor() noexcept {};
  virtual ~SRF01_Descriptor() noexcept {};

  void set_bus(std::string const& value);
  auto get_bus() const -> std::string const&;

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  std::string m_bus = {};
  uint32_t m_rate = {1};
};

struct SRF01_Config : public ::silk::INode_Config
{
public:

  SRF01_Config() noexcept {};
  virtual ~SRF01_Config() noexcept {};

  void set_direction(math::vec3<float> const& value);
  auto get_direction() const -> math::vec3<float> const&;

  void set_max_distance(float const& value);
  auto get_max_distance() const -> float const&;

  void set_min_distance(float const& value);
  auto get_min_distance() const -> float const&;


private:

  math::vec3<float> m_direction = {0.000000f, 0.000000f, 0.000000f};
  float m_max_distance = {6.000000f};
  float m_min_distance = {0.200000f};
};

struct SRF02_Descriptor : public ::silk::INode_Descriptor
{
public:

  SRF02_Descriptor() noexcept {};
  virtual ~SRF02_Descriptor() noexcept {};

  void set_bus(std::string const& value);
  auto get_bus() const -> std::string const&;

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  std::string m_bus = {};
  uint32_t m_rate = {1};
};

struct SRF02_Config : public ::silk::INode_Config
{
public:

  SRF02_Config() noexcept {};
  virtual ~SRF02_Config() noexcept {};

  void set_direction(math::vec3<float> const& value);
  auto get_direction() const -> math::vec3<float> const&;

  void set_max_distance(float const& value);
  auto get_max_distance() const -> float const&;

  void set_min_distance(float const& value);
  auto get_min_distance() const -> float const&;


private:

  math::vec3<float> m_direction = {0.000000f, 0.000000f, 0.000000f};
  float m_max_distance = {6.000000f};
  float m_min_distance = {0.200000f};
};

struct Throttle_To_PWM_Descriptor : public ::silk::INode_Descriptor
{
public:

  Throttle_To_PWM_Descriptor() noexcept {};
  virtual ~Throttle_To_PWM_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;

  void set_channel_count(int32_t const& value);
  auto get_channel_count() const -> int32_t const&;


private:

  uint32_t m_rate = {1};
  int32_t m_channel_count = {1};
};

struct Throttle_To_PWM_Config : public ::silk::INode_Config
{
public:

  Throttle_To_PWM_Config() noexcept {};
  virtual ~Throttle_To_PWM_Config() noexcept {};


private:

};

struct Transformer_Descriptor : public ::silk::INode_Descriptor
{
public:

  Transformer_Descriptor() noexcept {};
  virtual ~Transformer_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {1};
};

struct Transformer_Config : public ::silk::INode_Config
{
public:

  Transformer_Config() noexcept {};
  virtual ~Transformer_Config() noexcept {};


private:

};

struct UBLOX_Descriptor : public ::silk::INode_Descriptor
{
public:

  UBLOX_Descriptor() noexcept {};
  virtual ~UBLOX_Descriptor() noexcept {};

  void set_bus(std::string const& value);
  auto get_bus() const -> std::string const&;

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  std::string m_bus = {};
  uint32_t m_rate = {1};
};

struct UBLOX_Config : public ::silk::INode_Config
{
public:

  UBLOX_Config() noexcept {};
  virtual ~UBLOX_Config() noexcept {};


private:

};

struct Vec3_Generator_Descriptor : public ::silk::INode_Descriptor
{
public:

  Vec3_Generator_Descriptor() noexcept {};
  virtual ~Vec3_Generator_Descriptor() noexcept {};

  void set_rate(uint32_t const& value);
  auto get_rate() const -> uint32_t const&;


private:

  uint32_t m_rate = {1};
};

struct Vec3_Generator_Config : public ::silk::INode_Config
{
public:

  Vec3_Generator_Config() noexcept {};
  virtual ~Vec3_Generator_Config() noexcept {};

  void set_value(math::vec3<float> const& value);
  auto get_value() const -> math::vec3<float> const&;


private:

  math::vec3<float> m_value = {0.000000f, 0.000000f, 0.000000f};
};

}
ts::Result<void> deserialize(bool& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(bool const& value);
ts::Result<void> deserialize(std::string& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::string const& value);
ts::Result<void> deserialize(int64_t& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(int64_t const& value);
ts::Result<void> deserialize(float& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(float const& value);
ts::Result<void> deserialize(double& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(double const& value);
ts::Result<void> deserialize(math::vec2<float>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(math::vec2<float> const& value);
ts::Result<void> deserialize(math::vec2<double>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(math::vec2<double> const& value);
ts::Result<void> deserialize(math::vec2<int64_t>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(math::vec2<int64_t> const& value);
ts::Result<void> deserialize(math::vec3<float>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(math::vec3<float> const& value);
ts::Result<void> deserialize(math::vec3<double>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(math::vec3<double> const& value);
ts::Result<void> deserialize(math::vec3<int64_t>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(math::vec3<int64_t> const& value);
ts::Result<void> deserialize(math::vec4<float>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(math::vec4<float> const& value);
ts::Result<void> deserialize(math::vec4<double>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(math::vec4<double> const& value);
ts::Result<void> deserialize(math::vec4<int64_t>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(math::vec4<int64_t> const& value);
ts::Result<void> deserialize(int8_t& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(int8_t const& value);
ts::Result<void> deserialize(uint8_t& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(uint8_t const& value);
ts::Result<void> deserialize(int16_t& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(int16_t const& value);
ts::Result<void> deserialize(uint16_t& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(uint16_t const& value);
ts::Result<void> deserialize(int32_t& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(int32_t const& value);
ts::Result<void> deserialize(uint32_t& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(uint32_t const& value);
ts::Result<void> deserialize(::silk::IUAV_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::IUAV_Descriptor const& value);
ts::Result<void> deserialize(::silk::IBus_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::IBus_Descriptor const& value);
ts::Result<void> deserialize(::silk::INode_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::INode_Descriptor const& value);
ts::Result<void> deserialize(::silk::INode_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::INode_Config const& value);
ts::Result<void> deserialize(::silk::Settings::Bus_Data& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Settings::Bus_Data const& value);
ts::Result<void> deserialize(::silk::Settings::Node_Data& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Settings::Node_Data const& value);
ts::Result<void> deserialize(::silk::Settings& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Settings const& value);
ts::Result<void> deserialize(::silk::Multirotor_Descriptor::Motor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Descriptor::Motor const& value);
ts::Result<void> deserialize(::silk::Multirotor_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Descriptor const& value);
ts::Result<void> deserialize(::silk::UART_Linux_Descriptor::baud_t& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::UART_Linux_Descriptor::baud_t const& value);
ts::Result<void> deserialize(::silk::UART_Linux_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::UART_Linux_Descriptor const& value);
ts::Result<void> deserialize(::silk::UART_BBang_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::UART_BBang_Descriptor const& value);
ts::Result<void> deserialize(::silk::I2C_BCM_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::I2C_BCM_Descriptor const& value);
ts::Result<void> deserialize(::silk::I2C_Linux_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::I2C_Linux_Descriptor const& value);
ts::Result<void> deserialize(::silk::SPI_BCM_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::SPI_BCM_Descriptor const& value);
ts::Result<void> deserialize(::silk::SPI_Linux_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::SPI_Linux_Descriptor const& value);
ts::Result<void> deserialize(::silk::P_Controller_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::P_Controller_Descriptor const& value);
ts::Result<void> deserialize(::silk::PI_Controller_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PI_Controller_Descriptor const& value);
ts::Result<void> deserialize(::silk::PD_Controller_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PD_Controller_Descriptor const& value);
ts::Result<void> deserialize(::silk::PID_Controller_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PID_Controller_Descriptor const& value);
ts::Result<void> deserialize(::silk::ADC_Ammeter_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::ADC_Ammeter_Descriptor const& value);
ts::Result<void> deserialize(::silk::ADC_Ammeter_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::ADC_Ammeter_Config const& value);
ts::Result<void> deserialize(::silk::ADC_Voltmeter_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::ADC_Voltmeter_Descriptor const& value);
ts::Result<void> deserialize(::silk::ADC_Voltmeter_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::ADC_Voltmeter_Config const& value);
ts::Result<void> deserialize(::silk::ADS1115_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::ADS1115_Descriptor const& value);
ts::Result<void> deserialize(::silk::ADS1115_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::ADS1115_Config const& value);
ts::Result<void> deserialize(::silk::AVRADC_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::AVRADC_Descriptor const& value);
ts::Result<void> deserialize(::silk::AVRADC_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::AVRADC_Config const& value);
ts::Result<void> deserialize(::silk::Comp_AHRS_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Comp_AHRS_Descriptor const& value);
ts::Result<void> deserialize(::silk::Comp_AHRS_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Comp_AHRS_Config const& value);
ts::Result<void> deserialize(::silk::Combiner_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Combiner_Descriptor const& value);
ts::Result<void> deserialize(::silk::Combiner_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Combiner_Config const& value);
ts::Result<void> deserialize(::silk::Gravity_Filter_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Gravity_Filter_Descriptor const& value);
ts::Result<void> deserialize(::silk::Gravity_Filter_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Gravity_Filter_Config const& value);
ts::Result<void> deserialize(::silk::KF_ECEF_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::KF_ECEF_Descriptor const& value);
ts::Result<void> deserialize(::silk::KF_ECEF_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::KF_ECEF_Config const& value);
ts::Result<void> deserialize(::silk::ENU_Frame_System_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::ENU_Frame_System_Descriptor const& value);
ts::Result<void> deserialize(::silk::ENU_Frame_System_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::ENU_Frame_System_Config const& value);
ts::Result<void> deserialize(::silk::LPF_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::LPF_Descriptor const& value);
ts::Result<void> deserialize(::silk::LPF_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::LPF_Config const& value);
ts::Result<void> deserialize(::silk::MaxSonar_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::MaxSonar_Descriptor const& value);
ts::Result<void> deserialize(::silk::MaxSonar_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::MaxSonar_Config const& value);
ts::Result<void> deserialize(::silk::Motor_Mixer_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Motor_Mixer_Descriptor const& value);
ts::Result<void> deserialize(::silk::Motor_Mixer_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Motor_Mixer_Config const& value);
ts::Result<void> deserialize(::silk::Acceleration_Calibration_Point& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Acceleration_Calibration_Point const& value);
ts::Result<void> deserialize(::silk::Angular_Velocity_Calibration_Point& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Angular_Velocity_Calibration_Point const& value);
ts::Result<void> deserialize(::silk::Magnetic_Field_Calibration_Point& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Magnetic_Field_Calibration_Point const& value);
ts::Result<void> deserialize(::silk::MPU9250_Descriptor::imu_rate_t& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::MPU9250_Descriptor::imu_rate_t const& value);
ts::Result<void> deserialize(::silk::MPU9250_Descriptor::acceleration_range_t& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::MPU9250_Descriptor::acceleration_range_t const& value);
ts::Result<void> deserialize(::silk::MPU9250_Descriptor::angular_velocity_range_t& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::MPU9250_Descriptor::angular_velocity_range_t const& value);
ts::Result<void> deserialize(::silk::MPU9250_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::MPU9250_Descriptor const& value);
ts::Result<void> deserialize(::silk::MPU9250_Config::Calibration& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::MPU9250_Config::Calibration const& value);
ts::Result<void> deserialize(::silk::MPU9250_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::MPU9250_Config const& value);
ts::Result<void> deserialize(::silk::MS5611_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::MS5611_Descriptor const& value);
ts::Result<void> deserialize(::silk::MS5611_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::MS5611_Config const& value);
ts::Result<void> deserialize(::silk::Multirotor_Brain_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Descriptor const& value);
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs const& value);
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs const& value);
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config::Horizontal_Angle& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config::Horizontal_Angle const& value);
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config::Horizontal_Position& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config::Horizontal_Position const& value);
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config::Yaw_Angle& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config::Yaw_Angle const& value);
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config::Altitude& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config::Altitude const& value);
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config const& value);
ts::Result<void> deserialize(::silk::Multirotor_Pilot_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Pilot_Descriptor const& value);
ts::Result<void> deserialize(::silk::Multirotor_Pilot_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Pilot_Config const& value);
ts::Result<void> deserialize(::silk::Multirotor_Simulator_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Simulator_Descriptor const& value);
ts::Result<void> deserialize(::silk::Multirotor_Simulator_Config::Noise& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Simulator_Config::Noise const& value);
ts::Result<void> deserialize(::silk::Multirotor_Simulator_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Simulator_Config const& value);
ts::Result<void> deserialize(::silk::Oscillator_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Oscillator_Descriptor const& value);
ts::Result<void> deserialize(::silk::Oscillator_Config::Component& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Oscillator_Config::Component const& value);
ts::Result<void> deserialize(::silk::Oscillator_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Oscillator_Config const& value);
ts::Result<void> deserialize(::silk::PCA9685_Descriptor::Channel& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PCA9685_Descriptor::Channel const& value);
ts::Result<void> deserialize(::silk::PCA9685_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PCA9685_Descriptor const& value);
ts::Result<void> deserialize(::silk::PCA9685_Config::IChannel& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PCA9685_Config::IChannel const& value);
ts::Result<void> deserialize(::silk::PCA9685_Config::Servo_Channel& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PCA9685_Config::Servo_Channel const& value);
ts::Result<void> deserialize(::silk::PCA9685_Config::PWM_Channel& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PCA9685_Config::PWM_Channel const& value);
ts::Result<void> deserialize(::silk::PCA9685_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PCA9685_Config const& value);
ts::Result<void> deserialize(::silk::PIGPIO_Descriptor::Channel& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PIGPIO_Descriptor::Channel const& value);
ts::Result<void> deserialize(::silk::PIGPIO_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PIGPIO_Descriptor const& value);
ts::Result<void> deserialize(::silk::PIGPIO_Config::IChannel& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PIGPIO_Config::IChannel const& value);
ts::Result<void> deserialize(::silk::PIGPIO_Config::Servo_Channel& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PIGPIO_Config::Servo_Channel const& value);
ts::Result<void> deserialize(::silk::PIGPIO_Config::PWM_Channel& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PIGPIO_Config::PWM_Channel const& value);
ts::Result<void> deserialize(::silk::PIGPIO_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::PIGPIO_Config const& value);
ts::Result<void> deserialize(::silk::Pressure_Velocity_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Pressure_Velocity_Descriptor const& value);
ts::Result<void> deserialize(::silk::Pressure_Velocity_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Pressure_Velocity_Config const& value);
ts::Result<void> deserialize(::silk::Proximity_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Proximity_Descriptor const& value);
ts::Result<void> deserialize(::silk::Proximity_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Proximity_Config const& value);
ts::Result<void> deserialize(::silk::Rate_Controller_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Rate_Controller_Descriptor const& value);
ts::Result<void> deserialize(::silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs const& value);
ts::Result<void> deserialize(::silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs const& value);
ts::Result<void> deserialize(::silk::Rate_Controller_Config::Feedback& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Rate_Controller_Config::Feedback const& value);
ts::Result<void> deserialize(::silk::Rate_Controller_Config::Feedforward& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Rate_Controller_Config::Feedforward const& value);
ts::Result<void> deserialize(::silk::Rate_Controller_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Rate_Controller_Config const& value);
ts::Result<void> deserialize(::silk::Raspicam_Descriptor::Quality& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Raspicam_Descriptor::Quality const& value);
ts::Result<void> deserialize(::silk::Raspicam_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Raspicam_Descriptor const& value);
ts::Result<void> deserialize(::silk::Raspicam_Config::awb_mode_t& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Raspicam_Config::awb_mode_t const& value);
ts::Result<void> deserialize(::silk::Raspicam_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Raspicam_Config const& value);
ts::Result<void> deserialize(::silk::RC5T619_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::RC5T619_Descriptor const& value);
ts::Result<void> deserialize(::silk::RC5T619_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::RC5T619_Config const& value);
ts::Result<void> deserialize(::silk::Resampler_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Resampler_Descriptor const& value);
ts::Result<void> deserialize(::silk::Resampler_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Resampler_Config const& value);
ts::Result<void> deserialize(::silk::Scalar_Generator_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Scalar_Generator_Descriptor const& value);
ts::Result<void> deserialize(::silk::Scalar_Generator_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Scalar_Generator_Config const& value);
ts::Result<void> deserialize(::silk::Servo_Gimbal_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Servo_Gimbal_Descriptor const& value);
ts::Result<void> deserialize(::silk::Servo_Gimbal_Config::Channel& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Servo_Gimbal_Config::Channel const& value);
ts::Result<void> deserialize(::silk::Servo_Gimbal_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Servo_Gimbal_Config const& value);
ts::Result<void> deserialize(::silk::SRF01_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::SRF01_Descriptor const& value);
ts::Result<void> deserialize(::silk::SRF01_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::SRF01_Config const& value);
ts::Result<void> deserialize(::silk::SRF02_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::SRF02_Descriptor const& value);
ts::Result<void> deserialize(::silk::SRF02_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::SRF02_Config const& value);
ts::Result<void> deserialize(::silk::Throttle_To_PWM_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Throttle_To_PWM_Descriptor const& value);
ts::Result<void> deserialize(::silk::Throttle_To_PWM_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Throttle_To_PWM_Config const& value);
ts::Result<void> deserialize(::silk::Transformer_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Transformer_Descriptor const& value);
ts::Result<void> deserialize(::silk::Transformer_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Transformer_Config const& value);
ts::Result<void> deserialize(::silk::UBLOX_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::UBLOX_Descriptor const& value);
ts::Result<void> deserialize(::silk::UBLOX_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::UBLOX_Config const& value);
ts::Result<void> deserialize(::silk::Vec3_Generator_Descriptor& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Vec3_Generator_Descriptor const& value);
ts::Result<void> deserialize(::silk::Vec3_Generator_Config& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::Vec3_Generator_Config const& value);
ts::Result<void> deserialize(std::shared_ptr<::silk::IBus_Descriptor>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::shared_ptr<::silk::IBus_Descriptor> const& value);
ts::Result<void> deserialize(std::shared_ptr<::silk::INode_Descriptor>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::shared_ptr<::silk::INode_Descriptor> const& value);
ts::Result<void> deserialize(std::shared_ptr<::silk::INode_Config>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::shared_ptr<::silk::INode_Config> const& value);
ts::Result<void> deserialize(std::vector<std::string>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<std::string> const& value);
ts::Result<void> deserialize(std::shared_ptr<::silk::IUAV_Descriptor>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::shared_ptr<::silk::IUAV_Descriptor> const& value);
ts::Result<void> deserialize(std::vector<::silk::Settings::Bus_Data>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Settings::Bus_Data> const& value);
ts::Result<void> deserialize(std::vector<::silk::Settings::Node_Data>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Settings::Node_Data> const& value);
ts::Result<void> deserialize(std::vector<::silk::Multirotor_Descriptor::Motor>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Multirotor_Descriptor::Motor> const& value);
ts::Result<void> deserialize(std::vector<::silk::Acceleration_Calibration_Point>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Acceleration_Calibration_Point> const& value);
ts::Result<void> deserialize(std::vector<::silk::Angular_Velocity_Calibration_Point>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Angular_Velocity_Calibration_Point> const& value);
ts::Result<void> deserialize(std::vector<::silk::Magnetic_Field_Calibration_Point>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Magnetic_Field_Calibration_Point> const& value);
ts::Result<void> deserialize(boost::variant<silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs,silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(boost::variant<silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs,silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs> const& value);
ts::Result<void> deserialize(std::vector<::silk::Oscillator_Config::Component>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Oscillator_Config::Component> const& value);
ts::Result<void> deserialize(std::vector<::silk::PCA9685_Descriptor::Channel>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::PCA9685_Descriptor::Channel> const& value);
ts::Result<void> deserialize(std::shared_ptr<::silk::PCA9685_Config::IChannel>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::shared_ptr<::silk::PCA9685_Config::IChannel> const& value);
ts::Result<void> deserialize(std::vector<std::shared_ptr<::silk::PCA9685_Config::IChannel>>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<std::shared_ptr<::silk::PCA9685_Config::IChannel>> const& value);
ts::Result<void> deserialize(std::shared_ptr<::silk::PIGPIO_Config::IChannel>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::shared_ptr<::silk::PIGPIO_Config::IChannel> const& value);
ts::Result<void> deserialize(boost::variant<silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs,silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(boost::variant<silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs,silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs> const& value);
