#include "uav.def.h"
template <typename T>
T clamp(T v, T min, T max)
{
  return std::min(std::max(v, min), max);
}
std::string const& get_ast_json()
{
  static const std::string s_json = R"xxx({"type":1,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"int8_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":-128}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":127}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"int8_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"uint8_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":255}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"uint8_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"int16_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":-32768}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":32767}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"int16_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"uint16_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":65535}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"uint16_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":-2147483648}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":2147483647}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"int32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":4294967295}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]}]}]},{"type":10,"children":[{"type":6,"attributes":[{"type":0,"value":"silk"}]},{"type":11,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"mass_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"muf_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"direction_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"math::vec3f"}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"euler_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":-180.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":180.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"euler3_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"math::vec3f"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":-180.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":-180.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":-180.000000}]}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":180.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":180.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":180.000000}]}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"servo_signal_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":2.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"pwm_signal_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"UAV_Config_Base"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"name"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Name"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"mass_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"mass"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Mass (Kg)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"moment_of_inertia"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Moment Of Inertia"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Multirotor_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"UAV_Config_Base"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Motor"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"math::vec3f"}]}]},{"type":6,"attributes":[{"type":0,"value":"position"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"direction_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"thrust_vector"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"type":0,"value":"clockwise"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":1,"value":false}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"radius"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"height"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"motor_z_torque"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"motor_thrust"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"motor_acceleration"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":10.000000}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"motor_deceleration"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":10.000000}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Motor"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"motors"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Bus_Descriptor_Base"}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Bus"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"type"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Bus_Descriptor_Base"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"descriptor"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Node"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"type"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"descriptor"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"config"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"input_paths"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Settings"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"UAV_Config_Base"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"config"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Bus"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"buses"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"nodes"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"UART_Linux_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Bus_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"dev"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"/dev/ttyAMA0"}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"baud"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":115200}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"UART_BBang_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Bus_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rx_pin"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"baud"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":115200}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"type":0,"value":"invert"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":1,"value":false}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"I2C_BCM_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Bus_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"dev"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"I2C_Linux_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Bus_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"dev"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"/dev/i2c0"}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"SPI_BCM_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Bus_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"mode_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":3}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"dev"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"baud"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1000000}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"mode_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"mode"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"SPI_Linux_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Bus_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"dev"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"/dev/ttyAMA0"}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"speed"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1000000}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"P_Controller_Descriptor"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"kp"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"kP"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"PI_Controller_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"P_Controller_Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"ki"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"kI"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_i"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max I"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"PD_Controller_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"P_Controller_Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"kd"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"kD"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"muf_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"d_filter"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"D Filter"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"PID_Controller_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PD_Controller_Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"ki"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"kI"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_i"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max I"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"ADC_Ammeter_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"ADC_Ammeter_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":6,"attributes":[{"type":0,"value":"scale"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Scale"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":6,"attributes":[{"type":0,"value":"bias"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Scale"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"ADC_Voltmeter_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"ADC_Voltmeter_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":6,"attributes":[{"type":0,"value":"scale"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Scale"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":6,"attributes":[{"type":0,"value":"bias"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Scale"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"ADS1115_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"adc0_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":200}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"ADC0 Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"adc1_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":200}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"ADC1 Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"adc2_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":200}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"ADC2 Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"adc3_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":200}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"ADC3 Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"ADS1115_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"AVRADC_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":200}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"AVRADC_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Comp_AHRS_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":200}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Comp_AHRS_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"muf_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"drift_correction_factor"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.300000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Drift Correction Factor"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Combiner_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":200}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Combiner_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Gravity_Filter_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Gravity_Filter_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"ECEF_KF_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"ECEF_KF_Filter_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"gps_position_lag"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPS Position Lag (s)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"gps_position_accuracy"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPS Position Accuracy (CEP m)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"gps_velocity_lag"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPS Velocity Lag (s)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"gps_velocity_accuracy"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.200000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPS Velocity Accuracy (m/s)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"acceleration_lag"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Acceleration Lag (s)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"acceleration_accuracy"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Acceleration Accuracy (m/s^2)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"LPF_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"LPF_Filter_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"lpf_poles_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":6}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"lpf_poles_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"lpf_poles"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"LPF Poles"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"lpf_cutoff_frequency"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"LPF Cutoff Frequency (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"MaxSonar_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"MaxSonar_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"direction_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"direction"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Direction"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":6.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Maximum Distance (m)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"min_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.200000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Minimum Distance (m)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Motor_Mixer_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Motor_Mixer_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"armed_thrust"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Armed Thrust (N)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Acceleration_Calibration_Point"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":6,"attributes":[{"type":0,"value":"temperature"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Temperature"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"math::vec3f"}]}]},{"type":6,"attributes":[{"type":0,"value":"bias"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bias"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"math::vec3f"}]}]},{"type":6,"attributes":[{"type":0,"value":"scale"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Scale"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Angular_Velocity_Calibration_Point"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":6,"attributes":[{"type":0,"value":"temperature"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Temperature"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"math::vec3f"}]}]},{"type":6,"attributes":[{"type":0,"value":"bias"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bias"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Magnetic_Field_Calibration_Point"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":6,"attributes":[{"type":0,"value":"temperature"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Temperature"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"math::vec3f"}]}]},{"type":6,"attributes":[{"type":0,"value":"bias"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bias"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"math::vec3f"}]}]},{"type":6,"attributes":[{"type":0,"value":"scale"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Scale"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"MPU9250_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":15,"children":[{"type":6,"attributes":[{"type":0,"value":"imu_rate_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"type":0,"value":"_250"},{"type":2,"value":250}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"250"}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"_500"},{"type":2,"value":500}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"500"}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"_1000"},{"type":2,"value":1000}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"1000"}]}]}]}]}]}]},{"type":15,"children":[{"type":6,"attributes":[{"type":0,"value":"acceleration_range_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"type":0,"value":"_2"},{"type":2,"value":2}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"2"}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"_4"},{"type":2,"value":4}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"4"}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"_8"},{"type":2,"value":8}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"8"}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"_16"},{"type":2,"value":16}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"16"}]}]}]}]}]}]},{"type":15,"children":[{"type":6,"attributes":[{"type":0,"value":"angular_velocity_range_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"type":0,"value":"_250"},{"type":2,"value":250}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"250"}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"_500"},{"type":2,"value":500}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"500"}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"_1000"},{"type":2,"value":1000}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"1000"}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"_2000"},{"type":2,"value":2000}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"2000"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"imu_rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"imu_rate"}]},{"type":20,"children":[{"type":6,"attributes":[{"type":0,"value":"imu_rate_t::_1000"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"IMU Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"magnetometer_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Magnetometer Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"thermometer_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":10}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Thermometer Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"acceleration_range_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"acceleration_range"}]},{"type":20,"children":[{"type":6,"attributes":[{"type":0,"value":"acceleration_range_t::_8"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Acceleration Range (G)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"angular_velocity_range_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"angular_velocity_range"}]},{"type":20,"children":[{"type":6,"attributes":[{"type":0,"value":"angular_velocity_range_t::_500"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Angular Velocity Range (DPS)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"MPU9250_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Calibration"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Acceleration_Calibration_Point"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"acceleration"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Acceleration"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Angular_Velocity_Calibration_Point"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"angular_velocity"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Angular Velocity"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Magnetic_Field_Calibration_Point"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"magnetic_field"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Magnetic Field"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"euler3_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rotation"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rotation (deg)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Calibration"}]}]},{"type":6,"attributes":[{"type":0,"value":"calibration"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Calibration"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"MS5611_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"pressure_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Pressure Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"temperature_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":10}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Temperature Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"MS5611_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Multirotor_Brain_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"commands_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Commands Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"state_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Commands Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Multirotor_Brain_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Horizontal_Angle"}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"speed_deg_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":500.000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"speed_deg_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_speed_deg"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":180.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max Speed (deg/s)"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"Combined_XY_PIDs"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Combined"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Separate_XY_PIDs"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Separate"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"type":0,"value":"x_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"X"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"type":0,"value":"y_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Y"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Combined_XY_PIDs"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Separate_XY_PIDs"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"xy_pids"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"PIDs"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Horizontal_Position"}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"speed_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":100.000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"speed_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_speed"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max Speed (m/s)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"LPF_Filter_Config"}]}]},{"type":6,"attributes":[{"type":0,"value":"lpf"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"LPF"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PI_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"type":0,"value":"velocity_pi"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Velocity PI"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"P_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"type":0,"value":"position_p"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Position P"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Yaw_Angle"}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"speed_deg_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":500.000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"speed_deg_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_speed_deg"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":180.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max Speed (deg/s)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"type":0,"value":"pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"PID"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Altitude"}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"speed_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":100.000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"speed_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_speed"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max Speed (m/s)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"LPF_Filter_Config"}]}]},{"type":6,"attributes":[{"type":0,"value":"lpf"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"LPF"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PI_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"type":0,"value":"speed_pi"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Speed PI"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"P_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"type":0,"value":"position_p"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Position P"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Horizontal_Angle"}]}]},{"type":6,"attributes":[{"type":0,"value":"horizontal_angle"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Horizontal Angle"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Horizontal_Position"}]}]},{"type":6,"attributes":[{"type":0,"value":"horizontal_position"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Horizontal Position"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Yaw_Angle"}]}]},{"type":6,"attributes":[{"type":0,"value":"yaw_angle"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Yaw Angle"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Altitude"}]}]},{"type":6,"attributes":[{"type":0,"value":"altitude"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Altitude"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Multirotor_Pilot_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"commands_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Commands Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"state_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"State Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"video_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Video Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Multirotor_Pilot_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Multirotor_Simulator_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"throttle_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Throttle Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"acceleration_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Acceleration Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"angular_velocity_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Angular Velocity Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"magnetic_field_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Magnetic Field Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"pressure_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Pressure Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"temperature_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":10}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Temperature Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"distance_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":12}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Distance Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"gps_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":5}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPS Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Multirotor_Simulator_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Noise"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"gps_position"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPS Position"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"gps_velocity"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.200000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPS Velocity"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"gps_pacc"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.500000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPS PACC"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"gps_vacc"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.100000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPS VACC"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"acceleration"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.010000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Acceleration"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"angular_velocity"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.010000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Angular Velocity"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"magnetic_field"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.100000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Magnetic Field"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"pressure"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.100000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Pressure"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"temperature"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.100000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Temperature"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"ground_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.010000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Ground Distance"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"type":0,"value":"simulation_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Simulation Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"type":0,"value":"drag_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Drag Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"type":0,"value":"ground_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Ground Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"type":0,"value":"gravity_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Gravity Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Noise"}]}]},{"type":6,"attributes":[{"type":0,"value":"noise"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Noise"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Oscillator_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"component_count_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":32}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"component_count_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"component_count"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Components"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Oscillator_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Component"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"frequency"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Frequency (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"amplitude"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Amplitude"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"type":0,"value":"square"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Square"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"amplitude"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Amplitude"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"noise"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Noise"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Component"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"components"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Components"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"PCA9685_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint8_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"address"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":64}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Address"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"PCA9685_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"type":0,"value":"servo_signal"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Servo Signal"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"servo_signal_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"min_servo"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Min Servo (ms)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"servo_signal_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_servo"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max Servo (ms)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"pwm_signal_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"min_pwm"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Min PWM"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"pwm_signal_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_pwm"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max PWM"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"channels"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Channels"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"PIGPIO_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"type":0,"value":"enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"type":0,"value":"servo_signal"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Servo Signal"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":400}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_2"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 2"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_3"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 3"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_4"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 4"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_5"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 5"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_6"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 6"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_7"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 7"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_8"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 8"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_9"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 9"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_10"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 10"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_11"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 11"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_12"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 12"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_13"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 13"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_14"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 14"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_15"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 15"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_16"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 16"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_17"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 17"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_18"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 18"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_19"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 19"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_20"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 20"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_21"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 21"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_22"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 22"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_23"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 23"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_24"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 24"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_25"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 25"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_26"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 26"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_27"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 27"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"PIGPIO_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"servo_signal_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Min (ms)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"servo_signal_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max (ms)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"pwm_signal_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Min"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"pwm_signal_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_2"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 2"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_3"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 3"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_4"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 4"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_5"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 5"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_6"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 6"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_7"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 7"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_8"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 8"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_9"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 9"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_10"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 10"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_11"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 11"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_12"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 12"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_13"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 13"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_14"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 14"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_15"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 15"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_16"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 16"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_17"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 17"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_18"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 18"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_19"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 19"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_20"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 20"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_21"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 21"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_22"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 22"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_23"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 23"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_24"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 24"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_25"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 25"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_26"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 26"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Channel"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PWM_Channel"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"gpio_27"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"GPIO 27"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Pressure_Velocity_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Pressure_Velocity_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Proximity_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"channel_count_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":128}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"channel_count_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"channel_count"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Channel Count"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Proximity_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Rate_Controller_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Rate_Controller_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Feedback"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"muf_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"weight"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Weight"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"Combined_XY_PIDs"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Combined"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Separate_XY_PIDs"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Separate"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"type":0,"value":"x_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"X"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"type":0,"value":"y_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Y"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Combined_XY_PIDs"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Separate_XY_PIDs"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"xy_pids"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"XY PIDs"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"type":0,"value":"z_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Z PID"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Feedforward"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"muf_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"weight"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Weight"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_torque"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max Torque (N)"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Feedback"}]}]},{"type":6,"attributes":[{"type":0,"value":"feedback"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Feedback"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Feedforward"}]}]},{"type":6,"attributes":[{"type":0,"value":"feedforward"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Feedforward"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Raspicam_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Quality"}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"resolution_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"math::vec2i"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":8}]}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":8}]}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1980}]}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1080}]}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"bitrate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":32000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"resolution_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"resolution"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Resolution"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"bitrate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"bitrate"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bitrate"}]}]}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"fps_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":10}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":60}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"fps_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"fps"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"FPS"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Quality"}]}]},{"type":6,"attributes":[{"type":0,"value":"streaming_low"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Streaming Low"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Quality"}]}]},{"type":6,"attributes":[{"type":0,"value":"streaming_high"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Streaming High"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Quality"}]}]},{"type":6,"attributes":[{"type":0,"value":"recording"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Recording"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Raspicam_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"iso_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1600}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"shutter_speed_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1000.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"ev_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":-10}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":10}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"sharpness_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":100}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"contrast_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":100}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"brightness_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":100}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"saturation_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":100}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"quality_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]}]}]},{"type":15,"children":[{"type":6,"attributes":[{"type":0,"value":"awb_mode_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"type":0,"value":"OFF"},{"type":2,"value":0}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Off"}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"AUTO"}]},{"type":6,"attributes":[{"type":0,"value":"SUNLIGHT"}]},{"type":6,"attributes":[{"type":0,"value":"CLOUDY"}]},{"type":6,"attributes":[{"type":0,"value":"SHADE"}]},{"type":6,"attributes":[{"type":0,"value":"TUNGSTEN"}]},{"type":6,"attributes":[{"type":0,"value":"FLUORESCENT"}]},{"type":6,"attributes":[{"type":0,"value":"INCANDESCENT"}]},{"type":6,"attributes":[{"type":0,"value":"FLASH"}]},{"type":6,"attributes":[{"type":0,"value":"HORIZON"}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"iso_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"iso"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Iso"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"shutter_speed_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"shutter_speed"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Shutter Speed (ms)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ev_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"ev"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Exposure Compensation"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"sharpness_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"sharpness"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Sharpness"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"contrast_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"contrast"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Contrast"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"brightness_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"brightness"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Brightness"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"saturation_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"saturation"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Saturation"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"awb_mode_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"awb_mode"}]},{"type":20,"children":[{"type":6,"attributes":[{"type":0,"value":"awb_mode_t::AUTO"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"AWB Mode"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"quality_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"quality"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Quality"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"type":0,"value":"recording"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Recording"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"RC5T619_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"adc0_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":50}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"ADC0 Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"adc1_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":5}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"ADC1 Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"RC5T619_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Resampler_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"input_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Input Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"output_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Output Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Resampler_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"LPF_Filter_Config"}]}]},{"type":6,"attributes":[{"type":0,"value":"lpf"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"LPF"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Scalar_Generator_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Scalar_Generator_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":6,"attributes":[{"type":0,"value":"value"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Value"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Gimbal_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"commands_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Commands Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Servo_Gimbal_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"euler_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"min_angle"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":-180.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Min Angle (°)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"euler_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_angle"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":180.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max Angle (°)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"pwm_signal_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"min_pwm"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max PWM"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"pwm_signal_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_pwm"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Max PWM"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"x_channel"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"X Channel"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"y_channel"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Y Channel"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"type":0,"value":"z_channel"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Z Channel"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"SRF01_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"SRF01_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"direction_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"direction"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Direction"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":6.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Maximum Distance (m)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"min_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.200000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Minimum Distance (m)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"SRF02_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"SRF02_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"direction_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"direction"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Direction"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"max_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":6.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Maximum Distance (m)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"type":0,"value":"min_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.200000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Minimum Distance (m)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Throttle_To_PWM_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"channel_count_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":128}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"channel_count_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"channel_count"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Channel Count"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Throttle_To_PWM_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Transformer_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Transformer_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"UBLOX_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"UBLOX_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Vec3_Generator_Descriptor"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Descriptor_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Rate (Hz)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Vec3_Generator_Config"}]},{"type":7,"attributes":[{"type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Config_Base"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"math::vec3f"}]}]},{"type":6,"attributes":[{"type":0,"value":"value"}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"Value"}]}]}]}]}]}]}]}]}]})xxx";
  return s_json;
}

namespace math
{

}
namespace silk
{

  void UAV_Config_Base::set_name(std::string const& value)
  {
    m_name = value;
  }
  auto UAV_Config_Base::get_name() const -> std::string const& 
  {
    return m_name;
  }


////////////////////////////////////////////////////////////

  void UAV_Config_Base::set_mass(float const& value)
  {
    m_mass = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto UAV_Config_Base::get_mass() const -> float const& 
  {
    return m_mass;
  }


////////////////////////////////////////////////////////////

  void UAV_Config_Base::set_moment_of_inertia(float const& value)
  {
    m_moment_of_inertia = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto UAV_Config_Base::get_moment_of_inertia() const -> float const& 
  {
    return m_moment_of_inertia;
  }


////////////////////////////////////////////////////////////

    void Multirotor_Config::Motor::set_position(math::vec3<float> const& value)
    {
      m_position = clamp(value, math::vec3<float>(-340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f), math::vec3<float>(340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f));
    }
    auto Multirotor_Config::Motor::get_position() const -> math::vec3<float> const& 
    {
      return m_position;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Config::Motor::set_thrust_vector(math::vec3<float> const& value)
    {
      m_thrust_vector = clamp(value, math::vec3<float>(-340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f), math::vec3<float>(340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f));
    }
    auto Multirotor_Config::Motor::get_thrust_vector() const -> math::vec3<float> const& 
    {
      return m_thrust_vector;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Config::Motor::set_clockwise(bool const& value)
    {
      m_clockwise = value;
    }
    auto Multirotor_Config::Motor::get_clockwise() const -> bool const& 
    {
      return m_clockwise;
    }


////////////////////////////////////////////////////////////

  void Multirotor_Config::set_name(std::string const& value)
  {
    m_name = value;
  }
  auto Multirotor_Config::get_name() const -> std::string const& 
  {
    return m_name;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Config::set_mass(float const& value)
  {
    m_mass = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Multirotor_Config::get_mass() const -> float const& 
  {
    return m_mass;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Config::set_moment_of_inertia(float const& value)
  {
    m_moment_of_inertia = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Multirotor_Config::get_moment_of_inertia() const -> float const& 
  {
    return m_moment_of_inertia;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Config::set_radius(float const& value)
  {
    m_radius = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Multirotor_Config::get_radius() const -> float const& 
  {
    return m_radius;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Config::set_height(float const& value)
  {
    m_height = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Multirotor_Config::get_height() const -> float const& 
  {
    return m_height;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Config::set_motor_z_torque(float const& value)
  {
    m_motor_z_torque = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Multirotor_Config::get_motor_z_torque() const -> float const& 
  {
    return m_motor_z_torque;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Config::set_motor_thrust(float const& value)
  {
    m_motor_thrust = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Multirotor_Config::get_motor_thrust() const -> float const& 
  {
    return m_motor_thrust;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Config::set_motor_acceleration(float const& value)
  {
    m_motor_acceleration = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Multirotor_Config::get_motor_acceleration() const -> float const& 
  {
    return m_motor_acceleration;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Config::set_motor_deceleration(float const& value)
  {
    m_motor_deceleration = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Multirotor_Config::get_motor_deceleration() const -> float const& 
  {
    return m_motor_deceleration;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Config::set_motors(std::vector<::silk::Multirotor_Config::Motor> const& value)
  {
    m_motors = value;
  }
  auto Multirotor_Config::get_motors() const -> std::vector<::silk::Multirotor_Config::Motor> const& 
  {
    return m_motors;
  }


////////////////////////////////////////////////////////////

  void Bus::set_name(std::string const& value)
  {
    m_name = value;
  }
  auto Bus::get_name() const -> std::string const& 
  {
    return m_name;
  }


////////////////////////////////////////////////////////////

  void Bus::set_type(std::string const& value)
  {
    m_type = value;
  }
  auto Bus::get_type() const -> std::string const& 
  {
    return m_type;
  }


////////////////////////////////////////////////////////////

  void Bus::set_descriptor(std::shared_ptr<::silk::Bus_Descriptor_Base> const& value)
  {
    m_descriptor = value;
  }
  auto Bus::get_descriptor() const -> std::shared_ptr<::silk::Bus_Descriptor_Base> const& 
  {
    return m_descriptor;
  }


////////////////////////////////////////////////////////////

  void Node::set_name(std::string const& value)
  {
    m_name = value;
  }
  auto Node::get_name() const -> std::string const& 
  {
    return m_name;
  }


////////////////////////////////////////////////////////////

  void Node::set_type(std::string const& value)
  {
    m_type = value;
  }
  auto Node::get_type() const -> std::string const& 
  {
    return m_type;
  }


////////////////////////////////////////////////////////////

  void Node::set_descriptor(std::shared_ptr<::silk::Node_Descriptor_Base> const& value)
  {
    m_descriptor = value;
  }
  auto Node::get_descriptor() const -> std::shared_ptr<::silk::Node_Descriptor_Base> const& 
  {
    return m_descriptor;
  }


////////////////////////////////////////////////////////////

  void Node::set_config(std::shared_ptr<::silk::Node_Config_Base> const& value)
  {
    m_config = value;
  }
  auto Node::get_config() const -> std::shared_ptr<::silk::Node_Config_Base> const& 
  {
    return m_config;
  }


////////////////////////////////////////////////////////////

  void Node::set_input_paths(std::vector<std::string> const& value)
  {
    m_input_paths = value;
  }
  auto Node::get_input_paths() const -> std::vector<std::string> const& 
  {
    return m_input_paths;
  }


////////////////////////////////////////////////////////////

  void Settings::set_config(std::shared_ptr<::silk::UAV_Config_Base> const& value)
  {
    m_config = value;
  }
  auto Settings::get_config() const -> std::shared_ptr<::silk::UAV_Config_Base> const& 
  {
    return m_config;
  }


////////////////////////////////////////////////////////////

  void Settings::set_buses(std::vector<::silk::Bus> const& value)
  {
    m_buses = value;
  }
  auto Settings::get_buses() const -> std::vector<::silk::Bus> const& 
  {
    return m_buses;
  }


////////////////////////////////////////////////////////////

  void Settings::set_nodes(std::vector<::silk::Node> const& value)
  {
    m_nodes = value;
  }
  auto Settings::get_nodes() const -> std::vector<::silk::Node> const& 
  {
    return m_nodes;
  }


////////////////////////////////////////////////////////////

  void UART_Linux_Descriptor::set_dev(std::string const& value)
  {
    m_dev = value;
  }
  auto UART_Linux_Descriptor::get_dev() const -> std::string const& 
  {
    return m_dev;
  }


////////////////////////////////////////////////////////////

  void UART_Linux_Descriptor::set_baud(uint32_t const& value)
  {
    m_baud = clamp(value, uint32_t(0), uint32_t(4294967295));
  }
  auto UART_Linux_Descriptor::get_baud() const -> uint32_t const& 
  {
    return m_baud;
  }


////////////////////////////////////////////////////////////

  void UART_BBang_Descriptor::set_rx_pin(uint32_t const& value)
  {
    m_rx_pin = clamp(value, uint32_t(0), uint32_t(4294967295));
  }
  auto UART_BBang_Descriptor::get_rx_pin() const -> uint32_t const& 
  {
    return m_rx_pin;
  }


////////////////////////////////////////////////////////////

  void UART_BBang_Descriptor::set_baud(uint32_t const& value)
  {
    m_baud = clamp(value, uint32_t(0), uint32_t(4294967295));
  }
  auto UART_BBang_Descriptor::get_baud() const -> uint32_t const& 
  {
    return m_baud;
  }


////////////////////////////////////////////////////////////

  void UART_BBang_Descriptor::set_invert(bool const& value)
  {
    m_invert = value;
  }
  auto UART_BBang_Descriptor::get_invert() const -> bool const& 
  {
    return m_invert;
  }


////////////////////////////////////////////////////////////

  void I2C_BCM_Descriptor::set_dev(uint32_t const& value)
  {
    m_dev = clamp(value, uint32_t(0), uint32_t(4294967295));
  }
  auto I2C_BCM_Descriptor::get_dev() const -> uint32_t const& 
  {
    return m_dev;
  }


////////////////////////////////////////////////////////////

  void I2C_Linux_Descriptor::set_dev(std::string const& value)
  {
    m_dev = value;
  }
  auto I2C_Linux_Descriptor::get_dev() const -> std::string const& 
  {
    return m_dev;
  }


////////////////////////////////////////////////////////////

  void SPI_BCM_Descriptor::set_dev(uint32_t const& value)
  {
    m_dev = clamp(value, uint32_t(0), uint32_t(4294967295));
  }
  auto SPI_BCM_Descriptor::get_dev() const -> uint32_t const& 
  {
    return m_dev;
  }


////////////////////////////////////////////////////////////

  void SPI_BCM_Descriptor::set_baud(uint32_t const& value)
  {
    m_baud = clamp(value, uint32_t(0), uint32_t(4294967295));
  }
  auto SPI_BCM_Descriptor::get_baud() const -> uint32_t const& 
  {
    return m_baud;
  }


////////////////////////////////////////////////////////////

  void SPI_BCM_Descriptor::set_mode(uint32_t const& value)
  {
    m_mode = clamp(value, uint32_t(0), uint32_t(3));
  }
  auto SPI_BCM_Descriptor::get_mode() const -> uint32_t const& 
  {
    return m_mode;
  }


////////////////////////////////////////////////////////////

  void SPI_Linux_Descriptor::set_dev(std::string const& value)
  {
    m_dev = value;
  }
  auto SPI_Linux_Descriptor::get_dev() const -> std::string const& 
  {
    return m_dev;
  }


////////////////////////////////////////////////////////////

  void SPI_Linux_Descriptor::set_speed(uint32_t const& value)
  {
    m_speed = clamp(value, uint32_t(0), uint32_t(4294967295));
  }
  auto SPI_Linux_Descriptor::get_speed() const -> uint32_t const& 
  {
    return m_speed;
  }


////////////////////////////////////////////////////////////

  void P_Controller_Descriptor::set_kp(float const& value)
  {
    m_kp = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto P_Controller_Descriptor::get_kp() const -> float const& 
  {
    return m_kp;
  }


////////////////////////////////////////////////////////////

  void PI_Controller_Descriptor::set_kp(float const& value)
  {
    m_kp = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto PI_Controller_Descriptor::get_kp() const -> float const& 
  {
    return m_kp;
  }


////////////////////////////////////////////////////////////

  void PI_Controller_Descriptor::set_ki(float const& value)
  {
    m_ki = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto PI_Controller_Descriptor::get_ki() const -> float const& 
  {
    return m_ki;
  }


////////////////////////////////////////////////////////////

  void PI_Controller_Descriptor::set_max_i(float const& value)
  {
    m_max_i = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto PI_Controller_Descriptor::get_max_i() const -> float const& 
  {
    return m_max_i;
  }


////////////////////////////////////////////////////////////

  void PD_Controller_Descriptor::set_kp(float const& value)
  {
    m_kp = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto PD_Controller_Descriptor::get_kp() const -> float const& 
  {
    return m_kp;
  }


////////////////////////////////////////////////////////////

  void PD_Controller_Descriptor::set_kd(float const& value)
  {
    m_kd = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto PD_Controller_Descriptor::get_kd() const -> float const& 
  {
    return m_kd;
  }


////////////////////////////////////////////////////////////

  void PD_Controller_Descriptor::set_d_filter(float const& value)
  {
    m_d_filter = clamp(value, float(0.000000f), float(1.000000f));
  }
  auto PD_Controller_Descriptor::get_d_filter() const -> float const& 
  {
    return m_d_filter;
  }


////////////////////////////////////////////////////////////

  void PID_Controller_Descriptor::set_kp(float const& value)
  {
    m_kp = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto PID_Controller_Descriptor::get_kp() const -> float const& 
  {
    return m_kp;
  }


////////////////////////////////////////////////////////////

  void PID_Controller_Descriptor::set_kd(float const& value)
  {
    m_kd = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto PID_Controller_Descriptor::get_kd() const -> float const& 
  {
    return m_kd;
  }


////////////////////////////////////////////////////////////

  void PID_Controller_Descriptor::set_d_filter(float const& value)
  {
    m_d_filter = clamp(value, float(0.000000f), float(1.000000f));
  }
  auto PID_Controller_Descriptor::get_d_filter() const -> float const& 
  {
    return m_d_filter;
  }


////////////////////////////////////////////////////////////

  void PID_Controller_Descriptor::set_ki(float const& value)
  {
    m_ki = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto PID_Controller_Descriptor::get_ki() const -> float const& 
  {
    return m_ki;
  }


////////////////////////////////////////////////////////////

  void PID_Controller_Descriptor::set_max_i(float const& value)
  {
    m_max_i = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto PID_Controller_Descriptor::get_max_i() const -> float const& 
  {
    return m_max_i;
  }


////////////////////////////////////////////////////////////

  void ADC_Ammeter_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto ADC_Ammeter_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void ADC_Ammeter_Config::set_scale(float const& value)
  {
    m_scale = clamp(value, float(-340282346638528859811704183484516925440.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto ADC_Ammeter_Config::get_scale() const -> float const& 
  {
    return m_scale;
  }


////////////////////////////////////////////////////////////

  void ADC_Ammeter_Config::set_bias(float const& value)
  {
    m_bias = clamp(value, float(-340282346638528859811704183484516925440.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto ADC_Ammeter_Config::get_bias() const -> float const& 
  {
    return m_bias;
  }


////////////////////////////////////////////////////////////

  void ADC_Voltmeter_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto ADC_Voltmeter_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void ADC_Voltmeter_Config::set_scale(float const& value)
  {
    m_scale = clamp(value, float(-340282346638528859811704183484516925440.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto ADC_Voltmeter_Config::get_scale() const -> float const& 
  {
    return m_scale;
  }


////////////////////////////////////////////////////////////

  void ADC_Voltmeter_Config::set_bias(float const& value)
  {
    m_bias = clamp(value, float(-340282346638528859811704183484516925440.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto ADC_Voltmeter_Config::get_bias() const -> float const& 
  {
    return m_bias;
  }


////////////////////////////////////////////////////////////

  void ADS1115_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  auto ADS1115_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }


////////////////////////////////////////////////////////////

  void ADS1115_Descriptor::set_adc0_rate(uint32_t const& value)
  {
    m_adc0_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto ADS1115_Descriptor::get_adc0_rate() const -> uint32_t const& 
  {
    return m_adc0_rate;
  }


////////////////////////////////////////////////////////////

  void ADS1115_Descriptor::set_adc1_rate(uint32_t const& value)
  {
    m_adc1_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto ADS1115_Descriptor::get_adc1_rate() const -> uint32_t const& 
  {
    return m_adc1_rate;
  }


////////////////////////////////////////////////////////////

  void ADS1115_Descriptor::set_adc2_rate(uint32_t const& value)
  {
    m_adc2_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto ADS1115_Descriptor::get_adc2_rate() const -> uint32_t const& 
  {
    return m_adc2_rate;
  }


////////////////////////////////////////////////////////////

  void ADS1115_Descriptor::set_adc3_rate(uint32_t const& value)
  {
    m_adc3_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto ADS1115_Descriptor::get_adc3_rate() const -> uint32_t const& 
  {
    return m_adc3_rate;
  }


////////////////////////////////////////////////////////////

  void AVRADC_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  auto AVRADC_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }


////////////////////////////////////////////////////////////

  void AVRADC_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto AVRADC_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Comp_AHRS_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Comp_AHRS_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Comp_AHRS_Config::set_drift_correction_factor(float const& value)
  {
    m_drift_correction_factor = clamp(value, float(0.000000f), float(1.000000f));
  }
  auto Comp_AHRS_Config::get_drift_correction_factor() const -> float const& 
  {
    return m_drift_correction_factor;
  }


////////////////////////////////////////////////////////////

  void Combiner_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Combiner_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Gravity_Filter_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Gravity_Filter_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void ECEF_KF_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto ECEF_KF_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void ECEF_KF_Filter_Config::set_gps_position_lag(float const& value)
  {
    m_gps_position_lag = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto ECEF_KF_Filter_Config::get_gps_position_lag() const -> float const& 
  {
    return m_gps_position_lag;
  }


////////////////////////////////////////////////////////////

  void ECEF_KF_Filter_Config::set_gps_position_accuracy(float const& value)
  {
    m_gps_position_accuracy = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto ECEF_KF_Filter_Config::get_gps_position_accuracy() const -> float const& 
  {
    return m_gps_position_accuracy;
  }


////////////////////////////////////////////////////////////

  void ECEF_KF_Filter_Config::set_gps_velocity_lag(float const& value)
  {
    m_gps_velocity_lag = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto ECEF_KF_Filter_Config::get_gps_velocity_lag() const -> float const& 
  {
    return m_gps_velocity_lag;
  }


////////////////////////////////////////////////////////////

  void ECEF_KF_Filter_Config::set_gps_velocity_accuracy(float const& value)
  {
    m_gps_velocity_accuracy = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto ECEF_KF_Filter_Config::get_gps_velocity_accuracy() const -> float const& 
  {
    return m_gps_velocity_accuracy;
  }


////////////////////////////////////////////////////////////

  void ECEF_KF_Filter_Config::set_acceleration_lag(float const& value)
  {
    m_acceleration_lag = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto ECEF_KF_Filter_Config::get_acceleration_lag() const -> float const& 
  {
    return m_acceleration_lag;
  }


////////////////////////////////////////////////////////////

  void ECEF_KF_Filter_Config::set_acceleration_accuracy(float const& value)
  {
    m_acceleration_accuracy = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto ECEF_KF_Filter_Config::get_acceleration_accuracy() const -> float const& 
  {
    return m_acceleration_accuracy;
  }


////////////////////////////////////////////////////////////

  void LPF_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto LPF_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void LPF_Filter_Config::set_lpf_poles(uint32_t const& value)
  {
    m_lpf_poles = clamp(value, uint32_t(0), uint32_t(6));
  }
  auto LPF_Filter_Config::get_lpf_poles() const -> uint32_t const& 
  {
    return m_lpf_poles;
  }


////////////////////////////////////////////////////////////

  void LPF_Filter_Config::set_lpf_cutoff_frequency(float const& value)
  {
    m_lpf_cutoff_frequency = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto LPF_Filter_Config::get_lpf_cutoff_frequency() const -> float const& 
  {
    return m_lpf_cutoff_frequency;
  }


////////////////////////////////////////////////////////////

  void MaxSonar_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  auto MaxSonar_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }


////////////////////////////////////////////////////////////

  void MaxSonar_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto MaxSonar_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void MaxSonar_Config::set_direction(math::vec3<float> const& value)
  {
    m_direction = clamp(value, math::vec3<float>(-340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f), math::vec3<float>(340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f));
  }
  auto MaxSonar_Config::get_direction() const -> math::vec3<float> const& 
  {
    return m_direction;
  }


////////////////////////////////////////////////////////////

  void MaxSonar_Config::set_max_distance(float const& value)
  {
    m_max_distance = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto MaxSonar_Config::get_max_distance() const -> float const& 
  {
    return m_max_distance;
  }


////////////////////////////////////////////////////////////

  void MaxSonar_Config::set_min_distance(float const& value)
  {
    m_min_distance = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto MaxSonar_Config::get_min_distance() const -> float const& 
  {
    return m_min_distance;
  }


////////////////////////////////////////////////////////////

  void Motor_Mixer_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Motor_Mixer_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Motor_Mixer_Config::set_armed_thrust(float const& value)
  {
    m_armed_thrust = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Motor_Mixer_Config::get_armed_thrust() const -> float const& 
  {
    return m_armed_thrust;
  }


////////////////////////////////////////////////////////////

  void Acceleration_Calibration_Point::set_temperature(float const& value)
  {
    m_temperature = clamp(value, float(-340282346638528859811704183484516925440.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Acceleration_Calibration_Point::get_temperature() const -> float const& 
  {
    return m_temperature;
  }


////////////////////////////////////////////////////////////

  void Acceleration_Calibration_Point::set_bias(math::vec3<float> const& value)
  {
    m_bias = clamp(value, math::vec3<float>(-340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f), math::vec3<float>(340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f));
  }
  auto Acceleration_Calibration_Point::get_bias() const -> math::vec3<float> const& 
  {
    return m_bias;
  }


////////////////////////////////////////////////////////////

  void Acceleration_Calibration_Point::set_scale(math::vec3<float> const& value)
  {
    m_scale = clamp(value, math::vec3<float>(-340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f), math::vec3<float>(340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f));
  }
  auto Acceleration_Calibration_Point::get_scale() const -> math::vec3<float> const& 
  {
    return m_scale;
  }


////////////////////////////////////////////////////////////

  void Angular_Velocity_Calibration_Point::set_temperature(float const& value)
  {
    m_temperature = clamp(value, float(-340282346638528859811704183484516925440.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Angular_Velocity_Calibration_Point::get_temperature() const -> float const& 
  {
    return m_temperature;
  }


////////////////////////////////////////////////////////////

  void Angular_Velocity_Calibration_Point::set_bias(math::vec3<float> const& value)
  {
    m_bias = clamp(value, math::vec3<float>(-340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f), math::vec3<float>(340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f));
  }
  auto Angular_Velocity_Calibration_Point::get_bias() const -> math::vec3<float> const& 
  {
    return m_bias;
  }


////////////////////////////////////////////////////////////

  void Magnetic_Field_Calibration_Point::set_temperature(float const& value)
  {
    m_temperature = clamp(value, float(-340282346638528859811704183484516925440.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Magnetic_Field_Calibration_Point::get_temperature() const -> float const& 
  {
    return m_temperature;
  }


////////////////////////////////////////////////////////////

  void Magnetic_Field_Calibration_Point::set_bias(math::vec3<float> const& value)
  {
    m_bias = clamp(value, math::vec3<float>(-340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f), math::vec3<float>(340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f));
  }
  auto Magnetic_Field_Calibration_Point::get_bias() const -> math::vec3<float> const& 
  {
    return m_bias;
  }


////////////////////////////////////////////////////////////

  void Magnetic_Field_Calibration_Point::set_scale(math::vec3<float> const& value)
  {
    m_scale = clamp(value, math::vec3<float>(-340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f), math::vec3<float>(340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f));
  }
  auto Magnetic_Field_Calibration_Point::get_scale() const -> math::vec3<float> const& 
  {
    return m_scale;
  }


////////////////////////////////////////////////////////////

  void MPU9250_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  auto MPU9250_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }


////////////////////////////////////////////////////////////

  void MPU9250_Descriptor::set_imu_rate(imu_rate_t const& value)
  {
    m_imu_rate = value;
  }
  auto MPU9250_Descriptor::get_imu_rate() const -> imu_rate_t const& 
  {
    return m_imu_rate;
  }


////////////////////////////////////////////////////////////

  void MPU9250_Descriptor::set_magnetometer_rate(uint32_t const& value)
  {
    m_magnetometer_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto MPU9250_Descriptor::get_magnetometer_rate() const -> uint32_t const& 
  {
    return m_magnetometer_rate;
  }


////////////////////////////////////////////////////////////

  void MPU9250_Descriptor::set_thermometer_rate(uint32_t const& value)
  {
    m_thermometer_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto MPU9250_Descriptor::get_thermometer_rate() const -> uint32_t const& 
  {
    return m_thermometer_rate;
  }


////////////////////////////////////////////////////////////

  void MPU9250_Descriptor::set_acceleration_range(acceleration_range_t const& value)
  {
    m_acceleration_range = value;
  }
  auto MPU9250_Descriptor::get_acceleration_range() const -> acceleration_range_t const& 
  {
    return m_acceleration_range;
  }


////////////////////////////////////////////////////////////

  void MPU9250_Descriptor::set_angular_velocity_range(angular_velocity_range_t const& value)
  {
    m_angular_velocity_range = value;
  }
  auto MPU9250_Descriptor::get_angular_velocity_range() const -> angular_velocity_range_t const& 
  {
    return m_angular_velocity_range;
  }


////////////////////////////////////////////////////////////

    void MPU9250_Config::Calibration::set_acceleration(std::vector<::silk::Acceleration_Calibration_Point> const& value)
    {
      m_acceleration = value;
    }
    auto MPU9250_Config::Calibration::get_acceleration() const -> std::vector<::silk::Acceleration_Calibration_Point> const& 
    {
      return m_acceleration;
    }


////////////////////////////////////////////////////////////

    void MPU9250_Config::Calibration::set_angular_velocity(std::vector<::silk::Angular_Velocity_Calibration_Point> const& value)
    {
      m_angular_velocity = value;
    }
    auto MPU9250_Config::Calibration::get_angular_velocity() const -> std::vector<::silk::Angular_Velocity_Calibration_Point> const& 
    {
      return m_angular_velocity;
    }


////////////////////////////////////////////////////////////

    void MPU9250_Config::Calibration::set_magnetic_field(std::vector<::silk::Magnetic_Field_Calibration_Point> const& value)
    {
      m_magnetic_field = value;
    }
    auto MPU9250_Config::Calibration::get_magnetic_field() const -> std::vector<::silk::Magnetic_Field_Calibration_Point> const& 
    {
      return m_magnetic_field;
    }


////////////////////////////////////////////////////////////

  void MPU9250_Config::set_rotation(math::vec3<float> const& value)
  {
    m_rotation = clamp(value, math::vec3<float>(-180.000000f, -180.000000f, -180.000000f), math::vec3<float>(180.000000f, 180.000000f, 180.000000f));
  }
  auto MPU9250_Config::get_rotation() const -> math::vec3<float> const& 
  {
    return m_rotation;
  }


////////////////////////////////////////////////////////////

  void MPU9250_Config::set_calibration(Calibration const& value)
  {
    m_calibration = value;
  }
  auto MPU9250_Config::get_calibration() const -> Calibration const& 
  {
    return m_calibration;
  }


////////////////////////////////////////////////////////////

  void MS5611_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  auto MS5611_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }


////////////////////////////////////////////////////////////

  void MS5611_Descriptor::set_pressure_rate(uint32_t const& value)
  {
    m_pressure_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto MS5611_Descriptor::get_pressure_rate() const -> uint32_t const& 
  {
    return m_pressure_rate;
  }


////////////////////////////////////////////////////////////

  void MS5611_Descriptor::set_temperature_rate(uint32_t const& value)
  {
    m_temperature_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto MS5611_Descriptor::get_temperature_rate() const -> uint32_t const& 
  {
    return m_temperature_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Brain_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Brain_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Brain_Descriptor::set_commands_rate(uint32_t const& value)
  {
    m_commands_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Brain_Descriptor::get_commands_rate() const -> uint32_t const& 
  {
    return m_commands_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Brain_Descriptor::set_state_rate(uint32_t const& value)
  {
    m_state_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Brain_Descriptor::get_state_rate() const -> uint32_t const& 
  {
    return m_state_rate;
  }


////////////////////////////////////////////////////////////

      void Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs::set_kp(float const& value)
      {
        m_kp = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
      }
      auto Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs::get_kp() const -> float const& 
      {
        return m_kp;
      }


////////////////////////////////////////////////////////////

      void Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs::set_kd(float const& value)
      {
        m_kd = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
      }
      auto Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs::get_kd() const -> float const& 
      {
        return m_kd;
      }


////////////////////////////////////////////////////////////

      void Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs::set_d_filter(float const& value)
      {
        m_d_filter = clamp(value, float(0.000000f), float(1.000000f));
      }
      auto Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs::get_d_filter() const -> float const& 
      {
        return m_d_filter;
      }


////////////////////////////////////////////////////////////

      void Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs::set_ki(float const& value)
      {
        m_ki = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
      }
      auto Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs::get_ki() const -> float const& 
      {
        return m_ki;
      }


////////////////////////////////////////////////////////////

      void Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs::set_max_i(float const& value)
      {
        m_max_i = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
      }
      auto Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs::get_max_i() const -> float const& 
      {
        return m_max_i;
      }


////////////////////////////////////////////////////////////

      void Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs::set_x_pid(::silk::PID_Controller_Descriptor const& value)
      {
        m_x_pid = value;
      }
      auto Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs::get_x_pid() const -> ::silk::PID_Controller_Descriptor const& 
      {
        return m_x_pid;
      }


////////////////////////////////////////////////////////////

      void Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs::set_y_pid(::silk::PID_Controller_Descriptor const& value)
      {
        m_y_pid = value;
      }
      auto Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs::get_y_pid() const -> ::silk::PID_Controller_Descriptor const& 
      {
        return m_y_pid;
      }


////////////////////////////////////////////////////////////

    void Multirotor_Brain_Config::Horizontal_Angle::set_max_speed_deg(float const& value)
    {
      m_max_speed_deg = clamp(value, float(0.000000f), float(500.000000f));
    }
    auto Multirotor_Brain_Config::Horizontal_Angle::get_max_speed_deg() const -> float const& 
    {
      return m_max_speed_deg;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Brain_Config::Horizontal_Angle::set_xy_pids(boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const& value)
    {
      m_xy_pids = value;
    }
    auto Multirotor_Brain_Config::Horizontal_Angle::get_xy_pids() const -> boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const& 
    {
      return m_xy_pids;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Brain_Config::Horizontal_Position::set_max_speed(float const& value)
    {
      m_max_speed = clamp(value, float(0.000000f), float(100.000000f));
    }
    auto Multirotor_Brain_Config::Horizontal_Position::get_max_speed() const -> float const& 
    {
      return m_max_speed;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Brain_Config::Horizontal_Position::set_lpf(::silk::LPF_Filter_Config const& value)
    {
      m_lpf = value;
    }
    auto Multirotor_Brain_Config::Horizontal_Position::get_lpf() const -> ::silk::LPF_Filter_Config const& 
    {
      return m_lpf;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Brain_Config::Horizontal_Position::set_velocity_pi(::silk::PI_Controller_Descriptor const& value)
    {
      m_velocity_pi = value;
    }
    auto Multirotor_Brain_Config::Horizontal_Position::get_velocity_pi() const -> ::silk::PI_Controller_Descriptor const& 
    {
      return m_velocity_pi;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Brain_Config::Horizontal_Position::set_position_p(::silk::P_Controller_Descriptor const& value)
    {
      m_position_p = value;
    }
    auto Multirotor_Brain_Config::Horizontal_Position::get_position_p() const -> ::silk::P_Controller_Descriptor const& 
    {
      return m_position_p;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Brain_Config::Yaw_Angle::set_max_speed_deg(float const& value)
    {
      m_max_speed_deg = clamp(value, float(0.000000f), float(500.000000f));
    }
    auto Multirotor_Brain_Config::Yaw_Angle::get_max_speed_deg() const -> float const& 
    {
      return m_max_speed_deg;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Brain_Config::Yaw_Angle::set_pid(::silk::PID_Controller_Descriptor const& value)
    {
      m_pid = value;
    }
    auto Multirotor_Brain_Config::Yaw_Angle::get_pid() const -> ::silk::PID_Controller_Descriptor const& 
    {
      return m_pid;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Brain_Config::Altitude::set_max_speed(float const& value)
    {
      m_max_speed = clamp(value, float(0.000000f), float(100.000000f));
    }
    auto Multirotor_Brain_Config::Altitude::get_max_speed() const -> float const& 
    {
      return m_max_speed;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Brain_Config::Altitude::set_lpf(::silk::LPF_Filter_Config const& value)
    {
      m_lpf = value;
    }
    auto Multirotor_Brain_Config::Altitude::get_lpf() const -> ::silk::LPF_Filter_Config const& 
    {
      return m_lpf;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Brain_Config::Altitude::set_speed_pi(::silk::PI_Controller_Descriptor const& value)
    {
      m_speed_pi = value;
    }
    auto Multirotor_Brain_Config::Altitude::get_speed_pi() const -> ::silk::PI_Controller_Descriptor const& 
    {
      return m_speed_pi;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Brain_Config::Altitude::set_position_p(::silk::P_Controller_Descriptor const& value)
    {
      m_position_p = value;
    }
    auto Multirotor_Brain_Config::Altitude::get_position_p() const -> ::silk::P_Controller_Descriptor const& 
    {
      return m_position_p;
    }


////////////////////////////////////////////////////////////

  void Multirotor_Brain_Config::set_horizontal_angle(Horizontal_Angle const& value)
  {
    m_horizontal_angle = value;
  }
  auto Multirotor_Brain_Config::get_horizontal_angle() const -> Horizontal_Angle const& 
  {
    return m_horizontal_angle;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Brain_Config::set_horizontal_position(Horizontal_Position const& value)
  {
    m_horizontal_position = value;
  }
  auto Multirotor_Brain_Config::get_horizontal_position() const -> Horizontal_Position const& 
  {
    return m_horizontal_position;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Brain_Config::set_yaw_angle(Yaw_Angle const& value)
  {
    m_yaw_angle = value;
  }
  auto Multirotor_Brain_Config::get_yaw_angle() const -> Yaw_Angle const& 
  {
    return m_yaw_angle;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Brain_Config::set_altitude(Altitude const& value)
  {
    m_altitude = value;
  }
  auto Multirotor_Brain_Config::get_altitude() const -> Altitude const& 
  {
    return m_altitude;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Pilot_Descriptor::set_commands_rate(uint32_t const& value)
  {
    m_commands_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Pilot_Descriptor::get_commands_rate() const -> uint32_t const& 
  {
    return m_commands_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Pilot_Descriptor::set_state_rate(uint32_t const& value)
  {
    m_state_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Pilot_Descriptor::get_state_rate() const -> uint32_t const& 
  {
    return m_state_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Pilot_Descriptor::set_video_rate(uint32_t const& value)
  {
    m_video_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Pilot_Descriptor::get_video_rate() const -> uint32_t const& 
  {
    return m_video_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Descriptor::set_throttle_rate(uint32_t const& value)
  {
    m_throttle_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Simulator_Descriptor::get_throttle_rate() const -> uint32_t const& 
  {
    return m_throttle_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Descriptor::set_acceleration_rate(uint32_t const& value)
  {
    m_acceleration_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Simulator_Descriptor::get_acceleration_rate() const -> uint32_t const& 
  {
    return m_acceleration_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Descriptor::set_angular_velocity_rate(uint32_t const& value)
  {
    m_angular_velocity_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Simulator_Descriptor::get_angular_velocity_rate() const -> uint32_t const& 
  {
    return m_angular_velocity_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Descriptor::set_magnetic_field_rate(uint32_t const& value)
  {
    m_magnetic_field_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Simulator_Descriptor::get_magnetic_field_rate() const -> uint32_t const& 
  {
    return m_magnetic_field_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Descriptor::set_pressure_rate(uint32_t const& value)
  {
    m_pressure_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Simulator_Descriptor::get_pressure_rate() const -> uint32_t const& 
  {
    return m_pressure_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Descriptor::set_temperature_rate(uint32_t const& value)
  {
    m_temperature_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Simulator_Descriptor::get_temperature_rate() const -> uint32_t const& 
  {
    return m_temperature_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Descriptor::set_distance_rate(uint32_t const& value)
  {
    m_distance_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Simulator_Descriptor::get_distance_rate() const -> uint32_t const& 
  {
    return m_distance_rate;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Descriptor::set_gps_rate(uint32_t const& value)
  {
    m_gps_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Simulator_Descriptor::get_gps_rate() const -> uint32_t const& 
  {
    return m_gps_rate;
  }


////////////////////////////////////////////////////////////

    void Multirotor_Simulator_Config::Noise::set_gps_position(float const& value)
    {
      m_gps_position = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Multirotor_Simulator_Config::Noise::get_gps_position() const -> float const& 
    {
      return m_gps_position;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Simulator_Config::Noise::set_gps_velocity(float const& value)
    {
      m_gps_velocity = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Multirotor_Simulator_Config::Noise::get_gps_velocity() const -> float const& 
    {
      return m_gps_velocity;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Simulator_Config::Noise::set_gps_pacc(float const& value)
    {
      m_gps_pacc = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Multirotor_Simulator_Config::Noise::get_gps_pacc() const -> float const& 
    {
      return m_gps_pacc;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Simulator_Config::Noise::set_gps_vacc(float const& value)
    {
      m_gps_vacc = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Multirotor_Simulator_Config::Noise::get_gps_vacc() const -> float const& 
    {
      return m_gps_vacc;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Simulator_Config::Noise::set_acceleration(float const& value)
    {
      m_acceleration = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Multirotor_Simulator_Config::Noise::get_acceleration() const -> float const& 
    {
      return m_acceleration;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Simulator_Config::Noise::set_angular_velocity(float const& value)
    {
      m_angular_velocity = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Multirotor_Simulator_Config::Noise::get_angular_velocity() const -> float const& 
    {
      return m_angular_velocity;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Simulator_Config::Noise::set_magnetic_field(float const& value)
    {
      m_magnetic_field = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Multirotor_Simulator_Config::Noise::get_magnetic_field() const -> float const& 
    {
      return m_magnetic_field;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Simulator_Config::Noise::set_pressure(float const& value)
    {
      m_pressure = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Multirotor_Simulator_Config::Noise::get_pressure() const -> float const& 
    {
      return m_pressure;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Simulator_Config::Noise::set_temperature(float const& value)
    {
      m_temperature = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Multirotor_Simulator_Config::Noise::get_temperature() const -> float const& 
    {
      return m_temperature;
    }


////////////////////////////////////////////////////////////

    void Multirotor_Simulator_Config::Noise::set_ground_distance(float const& value)
    {
      m_ground_distance = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Multirotor_Simulator_Config::Noise::get_ground_distance() const -> float const& 
    {
      return m_ground_distance;
    }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Config::set_simulation_enabled(bool const& value)
  {
    m_simulation_enabled = value;
  }
  auto Multirotor_Simulator_Config::get_simulation_enabled() const -> bool const& 
  {
    return m_simulation_enabled;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Config::set_drag_enabled(bool const& value)
  {
    m_drag_enabled = value;
  }
  auto Multirotor_Simulator_Config::get_drag_enabled() const -> bool const& 
  {
    return m_drag_enabled;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Config::set_ground_enabled(bool const& value)
  {
    m_ground_enabled = value;
  }
  auto Multirotor_Simulator_Config::get_ground_enabled() const -> bool const& 
  {
    return m_ground_enabled;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Config::set_gravity_enabled(bool const& value)
  {
    m_gravity_enabled = value;
  }
  auto Multirotor_Simulator_Config::get_gravity_enabled() const -> bool const& 
  {
    return m_gravity_enabled;
  }


////////////////////////////////////////////////////////////

  void Multirotor_Simulator_Config::set_noise(Noise const& value)
  {
    m_noise = value;
  }
  auto Multirotor_Simulator_Config::get_noise() const -> Noise const& 
  {
    return m_noise;
  }


////////////////////////////////////////////////////////////

  void Oscillator_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Oscillator_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Oscillator_Descriptor::set_component_count(int32_t const& value)
  {
    m_component_count = clamp(value, int32_t(1), int32_t(32));
  }
  auto Oscillator_Descriptor::get_component_count() const -> int32_t const& 
  {
    return m_component_count;
  }


////////////////////////////////////////////////////////////

    void Oscillator_Config::Component::set_frequency(float const& value)
    {
      m_frequency = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Oscillator_Config::Component::get_frequency() const -> float const& 
    {
      return m_frequency;
    }


////////////////////////////////////////////////////////////

    void Oscillator_Config::Component::set_amplitude(float const& value)
    {
      m_amplitude = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Oscillator_Config::Component::get_amplitude() const -> float const& 
    {
      return m_amplitude;
    }


////////////////////////////////////////////////////////////

    void Oscillator_Config::Component::set_square(bool const& value)
    {
      m_square = value;
    }
    auto Oscillator_Config::Component::get_square() const -> bool const& 
    {
      return m_square;
    }


////////////////////////////////////////////////////////////

  void Oscillator_Config::set_amplitude(float const& value)
  {
    m_amplitude = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Oscillator_Config::get_amplitude() const -> float const& 
  {
    return m_amplitude;
  }


////////////////////////////////////////////////////////////

  void Oscillator_Config::set_noise(float const& value)
  {
    m_noise = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Oscillator_Config::get_noise() const -> float const& 
  {
    return m_noise;
  }


////////////////////////////////////////////////////////////

  void Oscillator_Config::set_components(std::vector<::silk::Oscillator_Config::Component> const& value)
  {
    m_components = value;
  }
  auto Oscillator_Config::get_components() const -> std::vector<::silk::Oscillator_Config::Component> const& 
  {
    return m_components;
  }


////////////////////////////////////////////////////////////

  void PCA9685_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  auto PCA9685_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }


////////////////////////////////////////////////////////////

  void PCA9685_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto PCA9685_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void PCA9685_Descriptor::set_address(uint8_t const& value)
  {
    m_address = clamp(value, uint8_t(0), uint8_t(255));
  }
  auto PCA9685_Descriptor::get_address() const -> uint8_t const& 
  {
    return m_address;
  }


////////////////////////////////////////////////////////////

    void PCA9685_Config::Channel::set_servo_signal(bool const& value)
    {
      m_servo_signal = value;
    }
    auto PCA9685_Config::Channel::get_servo_signal() const -> bool const& 
    {
      return m_servo_signal;
    }


////////////////////////////////////////////////////////////

    void PCA9685_Config::Channel::set_min_servo(float const& value)
    {
      m_min_servo = clamp(value, float(1.000000f), float(2.000000f));
    }
    auto PCA9685_Config::Channel::get_min_servo() const -> float const& 
    {
      return m_min_servo;
    }


////////////////////////////////////////////////////////////

    void PCA9685_Config::Channel::set_max_servo(float const& value)
    {
      m_max_servo = clamp(value, float(1.000000f), float(2.000000f));
    }
    auto PCA9685_Config::Channel::get_max_servo() const -> float const& 
    {
      return m_max_servo;
    }


////////////////////////////////////////////////////////////

    void PCA9685_Config::Channel::set_min_pwm(float const& value)
    {
      m_min_pwm = clamp(value, float(0.000000f), float(1.000000f));
    }
    auto PCA9685_Config::Channel::get_min_pwm() const -> float const& 
    {
      return m_min_pwm;
    }


////////////////////////////////////////////////////////////

    void PCA9685_Config::Channel::set_max_pwm(float const& value)
    {
      m_max_pwm = clamp(value, float(0.000000f), float(1.000000f));
    }
    auto PCA9685_Config::Channel::get_max_pwm() const -> float const& 
    {
      return m_max_pwm;
    }


////////////////////////////////////////////////////////////

  void PCA9685_Config::set_channels(std::vector<::silk::PCA9685_Config::Channel> const& value)
  {
    m_channels = value;
  }
  auto PCA9685_Config::get_channels() const -> std::vector<::silk::PCA9685_Config::Channel> const& 
  {
    return m_channels;
  }


////////////////////////////////////////////////////////////

    void PIGPIO_Descriptor::Channel::set_enabled(bool const& value)
    {
      m_enabled = value;
    }
    auto PIGPIO_Descriptor::Channel::get_enabled() const -> bool const& 
    {
      return m_enabled;
    }


////////////////////////////////////////////////////////////

    void PIGPIO_Descriptor::Channel::set_servo_signal(bool const& value)
    {
      m_servo_signal = value;
    }
    auto PIGPIO_Descriptor::Channel::get_servo_signal() const -> bool const& 
    {
      return m_servo_signal;
    }


////////////////////////////////////////////////////////////

    void PIGPIO_Descriptor::Channel::set_rate(uint32_t const& value)
    {
      m_rate = clamp(value, uint32_t(1), uint32_t(10000));
    }
    auto PIGPIO_Descriptor::Channel::get_rate() const -> uint32_t const& 
    {
      return m_rate;
    }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_2(Channel const& value)
  {
    m_gpio_2 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_2() const -> Channel const& 
  {
    return m_gpio_2;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_3(Channel const& value)
  {
    m_gpio_3 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_3() const -> Channel const& 
  {
    return m_gpio_3;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_4(Channel const& value)
  {
    m_gpio_4 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_4() const -> Channel const& 
  {
    return m_gpio_4;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_5(Channel const& value)
  {
    m_gpio_5 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_5() const -> Channel const& 
  {
    return m_gpio_5;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_6(Channel const& value)
  {
    m_gpio_6 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_6() const -> Channel const& 
  {
    return m_gpio_6;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_7(Channel const& value)
  {
    m_gpio_7 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_7() const -> Channel const& 
  {
    return m_gpio_7;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_8(Channel const& value)
  {
    m_gpio_8 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_8() const -> Channel const& 
  {
    return m_gpio_8;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_9(Channel const& value)
  {
    m_gpio_9 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_9() const -> Channel const& 
  {
    return m_gpio_9;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_10(Channel const& value)
  {
    m_gpio_10 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_10() const -> Channel const& 
  {
    return m_gpio_10;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_11(Channel const& value)
  {
    m_gpio_11 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_11() const -> Channel const& 
  {
    return m_gpio_11;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_12(Channel const& value)
  {
    m_gpio_12 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_12() const -> Channel const& 
  {
    return m_gpio_12;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_13(Channel const& value)
  {
    m_gpio_13 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_13() const -> Channel const& 
  {
    return m_gpio_13;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_14(Channel const& value)
  {
    m_gpio_14 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_14() const -> Channel const& 
  {
    return m_gpio_14;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_15(Channel const& value)
  {
    m_gpio_15 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_15() const -> Channel const& 
  {
    return m_gpio_15;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_16(Channel const& value)
  {
    m_gpio_16 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_16() const -> Channel const& 
  {
    return m_gpio_16;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_17(Channel const& value)
  {
    m_gpio_17 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_17() const -> Channel const& 
  {
    return m_gpio_17;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_18(Channel const& value)
  {
    m_gpio_18 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_18() const -> Channel const& 
  {
    return m_gpio_18;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_19(Channel const& value)
  {
    m_gpio_19 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_19() const -> Channel const& 
  {
    return m_gpio_19;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_20(Channel const& value)
  {
    m_gpio_20 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_20() const -> Channel const& 
  {
    return m_gpio_20;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_21(Channel const& value)
  {
    m_gpio_21 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_21() const -> Channel const& 
  {
    return m_gpio_21;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_22(Channel const& value)
  {
    m_gpio_22 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_22() const -> Channel const& 
  {
    return m_gpio_22;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_23(Channel const& value)
  {
    m_gpio_23 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_23() const -> Channel const& 
  {
    return m_gpio_23;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_24(Channel const& value)
  {
    m_gpio_24 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_24() const -> Channel const& 
  {
    return m_gpio_24;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_25(Channel const& value)
  {
    m_gpio_25 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_25() const -> Channel const& 
  {
    return m_gpio_25;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_26(Channel const& value)
  {
    m_gpio_26 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_26() const -> Channel const& 
  {
    return m_gpio_26;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Descriptor::set_gpio_27(Channel const& value)
  {
    m_gpio_27 = value;
  }
  auto PIGPIO_Descriptor::get_gpio_27() const -> Channel const& 
  {
    return m_gpio_27;
  }


////////////////////////////////////////////////////////////

    void PIGPIO_Config::Servo_Channel::set_min(float const& value)
    {
      m_min = clamp(value, float(1.000000f), float(2.000000f));
    }
    auto PIGPIO_Config::Servo_Channel::get_min() const -> float const& 
    {
      return m_min;
    }


////////////////////////////////////////////////////////////

    void PIGPIO_Config::Servo_Channel::set_max(float const& value)
    {
      m_max = clamp(value, float(1.000000f), float(2.000000f));
    }
    auto PIGPIO_Config::Servo_Channel::get_max() const -> float const& 
    {
      return m_max;
    }


////////////////////////////////////////////////////////////

    void PIGPIO_Config::PWM_Channel::set_min(float const& value)
    {
      m_min = clamp(value, float(0.000000f), float(1.000000f));
    }
    auto PIGPIO_Config::PWM_Channel::get_min() const -> float const& 
    {
      return m_min;
    }


////////////////////////////////////////////////////////////

    void PIGPIO_Config::PWM_Channel::set_max(float const& value)
    {
      m_max = clamp(value, float(0.000000f), float(1.000000f));
    }
    auto PIGPIO_Config::PWM_Channel::get_max() const -> float const& 
    {
      return m_max;
    }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_2(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_2 = value;
  }
  auto PIGPIO_Config::get_gpio_2() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_2;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_3(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_3 = value;
  }
  auto PIGPIO_Config::get_gpio_3() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_3;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_4(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_4 = value;
  }
  auto PIGPIO_Config::get_gpio_4() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_4;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_5(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_5 = value;
  }
  auto PIGPIO_Config::get_gpio_5() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_5;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_6(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_6 = value;
  }
  auto PIGPIO_Config::get_gpio_6() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_6;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_7(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_7 = value;
  }
  auto PIGPIO_Config::get_gpio_7() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_7;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_8(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_8 = value;
  }
  auto PIGPIO_Config::get_gpio_8() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_8;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_9(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_9 = value;
  }
  auto PIGPIO_Config::get_gpio_9() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_9;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_10(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_10 = value;
  }
  auto PIGPIO_Config::get_gpio_10() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_10;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_11(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_11 = value;
  }
  auto PIGPIO_Config::get_gpio_11() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_11;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_12(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_12 = value;
  }
  auto PIGPIO_Config::get_gpio_12() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_12;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_13(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_13 = value;
  }
  auto PIGPIO_Config::get_gpio_13() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_13;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_14(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_14 = value;
  }
  auto PIGPIO_Config::get_gpio_14() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_14;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_15(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_15 = value;
  }
  auto PIGPIO_Config::get_gpio_15() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_15;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_16(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_16 = value;
  }
  auto PIGPIO_Config::get_gpio_16() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_16;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_17(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_17 = value;
  }
  auto PIGPIO_Config::get_gpio_17() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_17;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_18(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_18 = value;
  }
  auto PIGPIO_Config::get_gpio_18() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_18;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_19(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_19 = value;
  }
  auto PIGPIO_Config::get_gpio_19() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_19;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_20(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_20 = value;
  }
  auto PIGPIO_Config::get_gpio_20() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_20;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_21(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_21 = value;
  }
  auto PIGPIO_Config::get_gpio_21() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_21;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_22(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_22 = value;
  }
  auto PIGPIO_Config::get_gpio_22() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_22;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_23(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_23 = value;
  }
  auto PIGPIO_Config::get_gpio_23() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_23;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_24(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_24 = value;
  }
  auto PIGPIO_Config::get_gpio_24() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_24;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_25(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_25 = value;
  }
  auto PIGPIO_Config::get_gpio_25() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_25;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_26(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_26 = value;
  }
  auto PIGPIO_Config::get_gpio_26() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_26;
  }


////////////////////////////////////////////////////////////

  void PIGPIO_Config::set_gpio_27(boost::variant<Servo_Channel,PWM_Channel> const& value)
  {
    m_gpio_27 = value;
  }
  auto PIGPIO_Config::get_gpio_27() const -> boost::variant<Servo_Channel,PWM_Channel> const& 
  {
    return m_gpio_27;
  }


////////////////////////////////////////////////////////////

  void Pressure_Velocity_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Pressure_Velocity_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Proximity_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Proximity_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Proximity_Descriptor::set_channel_count(int32_t const& value)
  {
    m_channel_count = clamp(value, int32_t(1), int32_t(128));
  }
  auto Proximity_Descriptor::get_channel_count() const -> int32_t const& 
  {
    return m_channel_count;
  }


////////////////////////////////////////////////////////////

  void Rate_Controller_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Rate_Controller_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

      void Rate_Controller_Config::Feedback::Combined_XY_PIDs::set_kp(float const& value)
      {
        m_kp = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
      }
      auto Rate_Controller_Config::Feedback::Combined_XY_PIDs::get_kp() const -> float const& 
      {
        return m_kp;
      }


////////////////////////////////////////////////////////////

      void Rate_Controller_Config::Feedback::Combined_XY_PIDs::set_kd(float const& value)
      {
        m_kd = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
      }
      auto Rate_Controller_Config::Feedback::Combined_XY_PIDs::get_kd() const -> float const& 
      {
        return m_kd;
      }


////////////////////////////////////////////////////////////

      void Rate_Controller_Config::Feedback::Combined_XY_PIDs::set_d_filter(float const& value)
      {
        m_d_filter = clamp(value, float(0.000000f), float(1.000000f));
      }
      auto Rate_Controller_Config::Feedback::Combined_XY_PIDs::get_d_filter() const -> float const& 
      {
        return m_d_filter;
      }


////////////////////////////////////////////////////////////

      void Rate_Controller_Config::Feedback::Combined_XY_PIDs::set_ki(float const& value)
      {
        m_ki = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
      }
      auto Rate_Controller_Config::Feedback::Combined_XY_PIDs::get_ki() const -> float const& 
      {
        return m_ki;
      }


////////////////////////////////////////////////////////////

      void Rate_Controller_Config::Feedback::Combined_XY_PIDs::set_max_i(float const& value)
      {
        m_max_i = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
      }
      auto Rate_Controller_Config::Feedback::Combined_XY_PIDs::get_max_i() const -> float const& 
      {
        return m_max_i;
      }


////////////////////////////////////////////////////////////

      void Rate_Controller_Config::Feedback::Separate_XY_PIDs::set_x_pid(::silk::PID_Controller_Descriptor const& value)
      {
        m_x_pid = value;
      }
      auto Rate_Controller_Config::Feedback::Separate_XY_PIDs::get_x_pid() const -> ::silk::PID_Controller_Descriptor const& 
      {
        return m_x_pid;
      }


////////////////////////////////////////////////////////////

      void Rate_Controller_Config::Feedback::Separate_XY_PIDs::set_y_pid(::silk::PID_Controller_Descriptor const& value)
      {
        m_y_pid = value;
      }
      auto Rate_Controller_Config::Feedback::Separate_XY_PIDs::get_y_pid() const -> ::silk::PID_Controller_Descriptor const& 
      {
        return m_y_pid;
      }


////////////////////////////////////////////////////////////

    void Rate_Controller_Config::Feedback::set_weight(float const& value)
    {
      m_weight = clamp(value, float(0.000000f), float(1.000000f));
    }
    auto Rate_Controller_Config::Feedback::get_weight() const -> float const& 
    {
      return m_weight;
    }


////////////////////////////////////////////////////////////

    void Rate_Controller_Config::Feedback::set_xy_pids(boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const& value)
    {
      m_xy_pids = value;
    }
    auto Rate_Controller_Config::Feedback::get_xy_pids() const -> boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const& 
    {
      return m_xy_pids;
    }


////////////////////////////////////////////////////////////

    void Rate_Controller_Config::Feedback::set_z_pid(::silk::PID_Controller_Descriptor const& value)
    {
      m_z_pid = value;
    }
    auto Rate_Controller_Config::Feedback::get_z_pid() const -> ::silk::PID_Controller_Descriptor const& 
    {
      return m_z_pid;
    }


////////////////////////////////////////////////////////////

    void Rate_Controller_Config::Feedforward::set_weight(float const& value)
    {
      m_weight = clamp(value, float(0.000000f), float(1.000000f));
    }
    auto Rate_Controller_Config::Feedforward::get_weight() const -> float const& 
    {
      return m_weight;
    }


////////////////////////////////////////////////////////////

    void Rate_Controller_Config::Feedforward::set_max_torque(float const& value)
    {
      m_max_torque = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
    }
    auto Rate_Controller_Config::Feedforward::get_max_torque() const -> float const& 
    {
      return m_max_torque;
    }


////////////////////////////////////////////////////////////

  void Rate_Controller_Config::set_feedback(Feedback const& value)
  {
    m_feedback = value;
  }
  auto Rate_Controller_Config::get_feedback() const -> Feedback const& 
  {
    return m_feedback;
  }


////////////////////////////////////////////////////////////

  void Rate_Controller_Config::set_feedforward(Feedforward const& value)
  {
    m_feedforward = value;
  }
  auto Rate_Controller_Config::get_feedforward() const -> Feedforward const& 
  {
    return m_feedforward;
  }


////////////////////////////////////////////////////////////

    void Raspicam_Descriptor::Quality::set_resolution(math::vec2<int64_t> const& value)
    {
      m_resolution = clamp(value, math::vec2<int64_t>(8, 8), math::vec2<int64_t>(1980, 1080));
    }
    auto Raspicam_Descriptor::Quality::get_resolution() const -> math::vec2<int64_t> const& 
    {
      return m_resolution;
    }


////////////////////////////////////////////////////////////

    void Raspicam_Descriptor::Quality::set_bitrate(int32_t const& value)
    {
      m_bitrate = clamp(value, int32_t(10000), int32_t(32000000));
    }
    auto Raspicam_Descriptor::Quality::get_bitrate() const -> int32_t const& 
    {
      return m_bitrate;
    }


////////////////////////////////////////////////////////////

  void Raspicam_Descriptor::set_fps(int32_t const& value)
  {
    m_fps = clamp(value, int32_t(10), int32_t(60));
  }
  auto Raspicam_Descriptor::get_fps() const -> int32_t const& 
  {
    return m_fps;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Descriptor::set_streaming_low(Quality const& value)
  {
    m_streaming_low = value;
  }
  auto Raspicam_Descriptor::get_streaming_low() const -> Quality const& 
  {
    return m_streaming_low;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Descriptor::set_streaming_high(Quality const& value)
  {
    m_streaming_high = value;
  }
  auto Raspicam_Descriptor::get_streaming_high() const -> Quality const& 
  {
    return m_streaming_high;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Descriptor::set_recording(Quality const& value)
  {
    m_recording = value;
  }
  auto Raspicam_Descriptor::get_recording() const -> Quality const& 
  {
    return m_recording;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Config::set_iso(int32_t const& value)
  {
    m_iso = clamp(value, int32_t(0), int32_t(1600));
  }
  auto Raspicam_Config::get_iso() const -> int32_t const& 
  {
    return m_iso;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Config::set_shutter_speed(float const& value)
  {
    m_shutter_speed = clamp(value, float(0.000000f), float(1000.000000f));
  }
  auto Raspicam_Config::get_shutter_speed() const -> float const& 
  {
    return m_shutter_speed;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Config::set_ev(int32_t const& value)
  {
    m_ev = clamp(value, int32_t(-10), int32_t(10));
  }
  auto Raspicam_Config::get_ev() const -> int32_t const& 
  {
    return m_ev;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Config::set_sharpness(int32_t const& value)
  {
    m_sharpness = clamp(value, int32_t(0), int32_t(100));
  }
  auto Raspicam_Config::get_sharpness() const -> int32_t const& 
  {
    return m_sharpness;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Config::set_contrast(int32_t const& value)
  {
    m_contrast = clamp(value, int32_t(0), int32_t(100));
  }
  auto Raspicam_Config::get_contrast() const -> int32_t const& 
  {
    return m_contrast;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Config::set_brightness(int32_t const& value)
  {
    m_brightness = clamp(value, int32_t(0), int32_t(100));
  }
  auto Raspicam_Config::get_brightness() const -> int32_t const& 
  {
    return m_brightness;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Config::set_saturation(int32_t const& value)
  {
    m_saturation = clamp(value, int32_t(0), int32_t(100));
  }
  auto Raspicam_Config::get_saturation() const -> int32_t const& 
  {
    return m_saturation;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Config::set_awb_mode(awb_mode_t const& value)
  {
    m_awb_mode = value;
  }
  auto Raspicam_Config::get_awb_mode() const -> awb_mode_t const& 
  {
    return m_awb_mode;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Config::set_quality(int32_t const& value)
  {
    m_quality = clamp(value, int32_t(0), int32_t(1));
  }
  auto Raspicam_Config::get_quality() const -> int32_t const& 
  {
    return m_quality;
  }


////////////////////////////////////////////////////////////

  void Raspicam_Config::set_recording(bool const& value)
  {
    m_recording = value;
  }
  auto Raspicam_Config::get_recording() const -> bool const& 
  {
    return m_recording;
  }


////////////////////////////////////////////////////////////

  void RC5T619_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  auto RC5T619_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }


////////////////////////////////////////////////////////////

  void RC5T619_Descriptor::set_adc0_rate(uint32_t const& value)
  {
    m_adc0_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto RC5T619_Descriptor::get_adc0_rate() const -> uint32_t const& 
  {
    return m_adc0_rate;
  }


////////////////////////////////////////////////////////////

  void RC5T619_Descriptor::set_adc1_rate(uint32_t const& value)
  {
    m_adc1_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto RC5T619_Descriptor::get_adc1_rate() const -> uint32_t const& 
  {
    return m_adc1_rate;
  }


////////////////////////////////////////////////////////////

  void Resampler_Descriptor::set_input_rate(uint32_t const& value)
  {
    m_input_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Resampler_Descriptor::get_input_rate() const -> uint32_t const& 
  {
    return m_input_rate;
  }


////////////////////////////////////////////////////////////

  void Resampler_Descriptor::set_output_rate(uint32_t const& value)
  {
    m_output_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Resampler_Descriptor::get_output_rate() const -> uint32_t const& 
  {
    return m_output_rate;
  }


////////////////////////////////////////////////////////////

  void Resampler_Config::set_lpf(::silk::LPF_Filter_Config const& value)
  {
    m_lpf = value;
  }
  auto Resampler_Config::get_lpf() const -> ::silk::LPF_Filter_Config const& 
  {
    return m_lpf;
  }


////////////////////////////////////////////////////////////

  void Scalar_Generator_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Scalar_Generator_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Scalar_Generator_Config::set_value(float const& value)
  {
    m_value = clamp(value, float(-340282346638528859811704183484516925440.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto Scalar_Generator_Config::get_value() const -> float const& 
  {
    return m_value;
  }


////////////////////////////////////////////////////////////

  void Servo_Gimbal_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Servo_Gimbal_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Servo_Gimbal_Descriptor::set_commands_rate(uint32_t const& value)
  {
    m_commands_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Servo_Gimbal_Descriptor::get_commands_rate() const -> uint32_t const& 
  {
    return m_commands_rate;
  }


////////////////////////////////////////////////////////////

    void Servo_Gimbal_Config::Channel::set_min_angle(float const& value)
    {
      m_min_angle = clamp(value, float(-180.000000f), float(180.000000f));
    }
    auto Servo_Gimbal_Config::Channel::get_min_angle() const -> float const& 
    {
      return m_min_angle;
    }


////////////////////////////////////////////////////////////

    void Servo_Gimbal_Config::Channel::set_max_angle(float const& value)
    {
      m_max_angle = clamp(value, float(-180.000000f), float(180.000000f));
    }
    auto Servo_Gimbal_Config::Channel::get_max_angle() const -> float const& 
    {
      return m_max_angle;
    }


////////////////////////////////////////////////////////////

    void Servo_Gimbal_Config::Channel::set_min_pwm(float const& value)
    {
      m_min_pwm = clamp(value, float(0.000000f), float(1.000000f));
    }
    auto Servo_Gimbal_Config::Channel::get_min_pwm() const -> float const& 
    {
      return m_min_pwm;
    }


////////////////////////////////////////////////////////////

    void Servo_Gimbal_Config::Channel::set_max_pwm(float const& value)
    {
      m_max_pwm = clamp(value, float(0.000000f), float(1.000000f));
    }
    auto Servo_Gimbal_Config::Channel::get_max_pwm() const -> float const& 
    {
      return m_max_pwm;
    }


////////////////////////////////////////////////////////////

  void Servo_Gimbal_Config::set_x_channel(Channel const& value)
  {
    m_x_channel = value;
  }
  auto Servo_Gimbal_Config::get_x_channel() const -> Channel const& 
  {
    return m_x_channel;
  }


////////////////////////////////////////////////////////////

  void Servo_Gimbal_Config::set_y_channel(Channel const& value)
  {
    m_y_channel = value;
  }
  auto Servo_Gimbal_Config::get_y_channel() const -> Channel const& 
  {
    return m_y_channel;
  }


////////////////////////////////////////////////////////////

  void Servo_Gimbal_Config::set_z_channel(Channel const& value)
  {
    m_z_channel = value;
  }
  auto Servo_Gimbal_Config::get_z_channel() const -> Channel const& 
  {
    return m_z_channel;
  }


////////////////////////////////////////////////////////////

  void SRF01_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  auto SRF01_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }


////////////////////////////////////////////////////////////

  void SRF01_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto SRF01_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void SRF01_Config::set_direction(math::vec3<float> const& value)
  {
    m_direction = clamp(value, math::vec3<float>(-340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f), math::vec3<float>(340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f));
  }
  auto SRF01_Config::get_direction() const -> math::vec3<float> const& 
  {
    return m_direction;
  }


////////////////////////////////////////////////////////////

  void SRF01_Config::set_max_distance(float const& value)
  {
    m_max_distance = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto SRF01_Config::get_max_distance() const -> float const& 
  {
    return m_max_distance;
  }


////////////////////////////////////////////////////////////

  void SRF01_Config::set_min_distance(float const& value)
  {
    m_min_distance = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto SRF01_Config::get_min_distance() const -> float const& 
  {
    return m_min_distance;
  }


////////////////////////////////////////////////////////////

  void SRF02_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  auto SRF02_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }


////////////////////////////////////////////////////////////

  void SRF02_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto SRF02_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void SRF02_Config::set_direction(math::vec3<float> const& value)
  {
    m_direction = clamp(value, math::vec3<float>(-340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f), math::vec3<float>(340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f));
  }
  auto SRF02_Config::get_direction() const -> math::vec3<float> const& 
  {
    return m_direction;
  }


////////////////////////////////////////////////////////////

  void SRF02_Config::set_max_distance(float const& value)
  {
    m_max_distance = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto SRF02_Config::get_max_distance() const -> float const& 
  {
    return m_max_distance;
  }


////////////////////////////////////////////////////////////

  void SRF02_Config::set_min_distance(float const& value)
  {
    m_min_distance = clamp(value, float(0.000000f), float(340282346638528859811704183484516925440.000000f));
  }
  auto SRF02_Config::get_min_distance() const -> float const& 
  {
    return m_min_distance;
  }


////////////////////////////////////////////////////////////

  void Throttle_To_PWM_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Throttle_To_PWM_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Throttle_To_PWM_Descriptor::set_channel_count(int32_t const& value)
  {
    m_channel_count = clamp(value, int32_t(1), int32_t(128));
  }
  auto Throttle_To_PWM_Descriptor::get_channel_count() const -> int32_t const& 
  {
    return m_channel_count;
  }


////////////////////////////////////////////////////////////

  void Transformer_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Transformer_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void UBLOX_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  auto UBLOX_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }


////////////////////////////////////////////////////////////

  void UBLOX_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto UBLOX_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Vec3_Generator_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  auto Vec3_Generator_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }


////////////////////////////////////////////////////////////

  void Vec3_Generator_Config::set_value(math::vec3<float> const& value)
  {
    m_value = clamp(value, math::vec3<float>(-340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f, -340282346638528859811704183484516925440.000000f), math::vec3<float>(340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f, 340282346638528859811704183484516925440.000000f));
  }
  auto Vec3_Generator_Config::get_value() const -> math::vec3<float> const& 
  {
    return m_value;
  }


////////////////////////////////////////////////////////////

}
ts::Result<void> deserialize(bool& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_bool()) { return ts::Error("Expected bool value when deserializing"); }
  value = sz_value.get_as_bool();
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(bool const& value)
{
  return ts::serialization::Value(value);
}
ts::Result<void> deserialize(std::string& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  value = sz_value.get_as_string();
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(std::string const& value)
{
  return ts::serialization::Value(value);
}
ts::Result<void> deserialize(int64_t& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(int64_t const& value)
{
  return ts::serialization::Value(value);
}
ts::Result<void> deserialize(float& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_real_number()) { return ts::Error("Expected real number value when deserializing"); }
  value = (float)sz_value.get_as_real_number();
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(float const& value)
{
  return ts::serialization::Value(value);
}
ts::Result<void> deserialize(double& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_real_number()) { return ts::Error("Expected real number value when deserializing"); }
  value = sz_value.get_as_real_number();
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(double const& value)
{
  return ts::serialization::Value(value);
}
ts::Result<void> deserialize(math::vec2<float>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(math::vec2<float> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.x);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("x", result.extract_payload());
  }
  {
    auto result = serialize(value.y);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("y", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(math::vec2<double>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(math::vec2<double> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.x);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("x", result.extract_payload());
  }
  {
    auto result = serialize(value.y);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("y", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(math::vec2<int64_t>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(math::vec2<int64_t> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.x);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("x", result.extract_payload());
  }
  {
    auto result = serialize(value.y);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("y", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(math::vec3<float>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(math::vec3<float> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.x);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("x", result.extract_payload());
  }
  {
    auto result = serialize(value.y);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("y", result.extract_payload());
  }
  {
    auto result = serialize(value.z);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("z", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(math::vec3<double>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(math::vec3<double> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.x);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("x", result.extract_payload());
  }
  {
    auto result = serialize(value.y);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("y", result.extract_payload());
  }
  {
    auto result = serialize(value.z);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("z", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(math::vec3<int64_t>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(math::vec3<int64_t> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.x);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("x", result.extract_payload());
  }
  {
    auto result = serialize(value.y);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("y", result.extract_payload());
  }
  {
    auto result = serialize(value.z);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("z", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(math::vec4<float>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(math::vec4<float> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.x);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("x", result.extract_payload());
  }
  {
    auto result = serialize(value.y);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("y", result.extract_payload());
  }
  {
    auto result = serialize(value.z);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("z", result.extract_payload());
  }
  {
    auto result = serialize(value.w);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("w", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(math::vec4<double>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(math::vec4<double> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.x);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("x", result.extract_payload());
  }
  {
    auto result = serialize(value.y);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("y", result.extract_payload());
  }
  {
    auto result = serialize(value.z);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("z", result.extract_payload());
  }
  {
    auto result = serialize(value.w);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("w", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(math::vec4<int64_t>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(math::vec4<int64_t> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.x);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("x", result.extract_payload());
  }
  {
    auto result = serialize(value.y);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("y", result.extract_payload());
  }
  {
    auto result = serialize(value.z);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("z", result.extract_payload());
  }
  {
    auto result = serialize(value.w);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("w", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(int8_t& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(int8_t const& value)
{
  return ts::serialization::Value(value);
}
ts::Result<void> deserialize(uint8_t& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(uint8_t const& value)
{
  return ts::serialization::Value(value);
}
ts::Result<void> deserialize(int16_t& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(int16_t const& value)
{
  return ts::serialization::Value(value);
}
ts::Result<void> deserialize(uint16_t& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(uint16_t const& value)
{
  return ts::serialization::Value(value);
}
ts::Result<void> deserialize(int32_t& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(int32_t const& value)
{
  return ts::serialization::Value(value);
}
ts::Result<void> deserialize(uint32_t& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_integral_number()) { return ts::Error("Expected integral number value when deserializing"); }
  value = sz_value.get_as_integral_number();
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(uint32_t const& value)
{
  return ts::serialization::Value(value);
}
ts::Result<void> deserialize(::silk::UAV_Config_Base& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("mass");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'mass'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_mass())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_mass(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("moment_of_inertia");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'moment_of_inertia'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_moment_of_inertia())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_moment_of_inertia(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::UAV_Config_Base const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_name());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("name", result.extract_payload());
  }
  {
    auto result = serialize(value.get_mass());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("mass", result.extract_payload());
  }
  {
    auto result = serialize(value.get_moment_of_inertia());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("moment_of_inertia", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Config::Motor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("position");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'position'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_position())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_position(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("thrust_vector");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'thrust_vector'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_thrust_vector())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_thrust_vector(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("clockwise");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'clockwise'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_clockwise())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_clockwise(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Config::Motor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_position());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("position", result.extract_payload());
  }
  {
    auto result = serialize(value.get_thrust_vector());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("thrust_vector", result.extract_payload());
  }
  {
    auto result = serialize(value.get_clockwise());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("clockwise", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("mass");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'mass'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_mass())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_mass(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("moment_of_inertia");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'moment_of_inertia'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_moment_of_inertia())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_moment_of_inertia(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("radius");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'radius'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_radius())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_radius(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("height");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'height'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_height())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_height(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("motor_z_torque");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'motor_z_torque'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_motor_z_torque())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_motor_z_torque(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("motor_thrust");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'motor_thrust'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_motor_thrust())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_motor_thrust(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("motor_acceleration");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'motor_acceleration'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_motor_acceleration())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_motor_acceleration(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("motor_deceleration");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'motor_deceleration'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_motor_deceleration())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_motor_deceleration(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("motors");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'motors'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_motors())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_motors(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_name());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("name", result.extract_payload());
  }
  {
    auto result = serialize(value.get_mass());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("mass", result.extract_payload());
  }
  {
    auto result = serialize(value.get_moment_of_inertia());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("moment_of_inertia", result.extract_payload());
  }
  {
    auto result = serialize(value.get_radius());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("radius", result.extract_payload());
  }
  {
    auto result = serialize(value.get_height());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("height", result.extract_payload());
  }
  {
    auto result = serialize(value.get_motor_z_torque());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("motor_z_torque", result.extract_payload());
  }
  {
    auto result = serialize(value.get_motor_thrust());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("motor_thrust", result.extract_payload());
  }
  {
    auto result = serialize(value.get_motor_acceleration());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("motor_acceleration", result.extract_payload());
  }
  {
    auto result = serialize(value.get_motor_deceleration());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("motor_deceleration", result.extract_payload());
  }
  {
    auto result = serialize(value.get_motors());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("motors", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Bus_Descriptor_Base& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Bus_Descriptor_Base const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::Bus& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("type");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'type'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_type())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_type(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("descriptor");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'descriptor'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_descriptor())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_descriptor(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Bus const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_name());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("name", result.extract_payload());
  }
  {
    auto result = serialize(value.get_type());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("type", result.extract_payload());
  }
  {
    auto result = serialize(value.get_descriptor());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("descriptor", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Node_Descriptor_Base& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Node_Descriptor_Base const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::Node_Config_Base& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Node_Config_Base const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::Node& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("type");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'type'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_type())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_type(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("descriptor");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'descriptor'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_descriptor())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_descriptor(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("config");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'config'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_config())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_config(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("input_paths");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'input_paths'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_input_paths())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_input_paths(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Node const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_name());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("name", result.extract_payload());
  }
  {
    auto result = serialize(value.get_type());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("type", result.extract_payload());
  }
  {
    auto result = serialize(value.get_descriptor());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("descriptor", result.extract_payload());
  }
  {
    auto result = serialize(value.get_config());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("config", result.extract_payload());
  }
  {
    auto result = serialize(value.get_input_paths());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("input_paths", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Settings& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("config");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'config'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_config())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_config(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("buses");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'buses'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_buses())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_buses(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("nodes");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'nodes'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_nodes())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_nodes(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Settings const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_config());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("config", result.extract_payload());
  }
  {
    auto result = serialize(value.get_buses());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("buses", result.extract_payload());
  }
  {
    auto result = serialize(value.get_nodes());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("nodes", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::UART_Linux_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("dev");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'dev'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_dev())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_dev(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("baud");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'baud'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_baud())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_baud(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::UART_Linux_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_dev());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("dev", result.extract_payload());
  }
  {
    auto result = serialize(value.get_baud());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("baud", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::UART_BBang_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rx_pin");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rx_pin'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rx_pin())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rx_pin(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("baud");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'baud'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_baud())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_baud(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("invert");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'invert'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_invert())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_invert(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::UART_BBang_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rx_pin());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rx_pin", result.extract_payload());
  }
  {
    auto result = serialize(value.get_baud());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("baud", result.extract_payload());
  }
  {
    auto result = serialize(value.get_invert());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("invert", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::I2C_BCM_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("dev");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'dev'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_dev())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_dev(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::I2C_BCM_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_dev());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("dev", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::I2C_Linux_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("dev");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'dev'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_dev())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_dev(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::I2C_Linux_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_dev());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("dev", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::SPI_BCM_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("dev");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'dev'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_dev())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_dev(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("baud");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'baud'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_baud())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_baud(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("mode");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'mode'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_mode())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_mode(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::SPI_BCM_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_dev());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("dev", result.extract_payload());
  }
  {
    auto result = serialize(value.get_baud());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("baud", result.extract_payload());
  }
  {
    auto result = serialize(value.get_mode());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("mode", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::SPI_Linux_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("dev");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'dev'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_dev())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_dev(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("speed");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'speed'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_speed())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_speed(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::SPI_Linux_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_dev());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("dev", result.extract_payload());
  }
  {
    auto result = serialize(value.get_speed());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("speed", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::P_Controller_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("kp");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'kp'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_kp())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_kp(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::P_Controller_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_kp());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("kp", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::PI_Controller_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("kp");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'kp'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_kp())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_kp(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("ki");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'ki'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_ki())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_ki(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_i");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_i'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_i())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_i(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::PI_Controller_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_kp());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("kp", result.extract_payload());
  }
  {
    auto result = serialize(value.get_ki());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("ki", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max_i());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_i", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::PD_Controller_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("kp");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'kp'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_kp())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_kp(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("kd");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'kd'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_kd())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_kd(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("d_filter");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'd_filter'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_d_filter())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_d_filter(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::PD_Controller_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_kp());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("kp", result.extract_payload());
  }
  {
    auto result = serialize(value.get_kd());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("kd", result.extract_payload());
  }
  {
    auto result = serialize(value.get_d_filter());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("d_filter", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::PID_Controller_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("kp");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'kp'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_kp())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_kp(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("kd");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'kd'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_kd())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_kd(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("d_filter");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'd_filter'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_d_filter())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_d_filter(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("ki");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'ki'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_ki())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_ki(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_i");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_i'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_i())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_i(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::PID_Controller_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_kp());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("kp", result.extract_payload());
  }
  {
    auto result = serialize(value.get_kd());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("kd", result.extract_payload());
  }
  {
    auto result = serialize(value.get_d_filter());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("d_filter", result.extract_payload());
  }
  {
    auto result = serialize(value.get_ki());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("ki", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max_i());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_i", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::ADC_Ammeter_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::ADC_Ammeter_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::ADC_Ammeter_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("scale");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'scale'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_scale())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_scale(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bias");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bias'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bias())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bias(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::ADC_Ammeter_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_scale());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("scale", result.extract_payload());
  }
  {
    auto result = serialize(value.get_bias());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bias", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::ADC_Voltmeter_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::ADC_Voltmeter_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::ADC_Voltmeter_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("scale");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'scale'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_scale())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_scale(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bias");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bias'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bias())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bias(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::ADC_Voltmeter_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_scale());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("scale", result.extract_payload());
  }
  {
    auto result = serialize(value.get_bias());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bias", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::ADS1115_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bus");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bus'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bus(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("adc0_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'adc0_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_adc0_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_adc0_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("adc1_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'adc1_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_adc1_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_adc1_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("adc2_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'adc2_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_adc2_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_adc2_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("adc3_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'adc3_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_adc3_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_adc3_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::ADS1115_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_bus());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bus", result.extract_payload());
  }
  {
    auto result = serialize(value.get_adc0_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("adc0_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_adc1_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("adc1_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_adc2_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("adc2_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_adc3_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("adc3_rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::ADS1115_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::ADS1115_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::AVRADC_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bus");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bus'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bus(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::AVRADC_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_bus());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bus", result.extract_payload());
  }
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::AVRADC_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::AVRADC_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::Comp_AHRS_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Comp_AHRS_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Comp_AHRS_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("drift_correction_factor");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'drift_correction_factor'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_drift_correction_factor())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_drift_correction_factor(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Comp_AHRS_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_drift_correction_factor());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("drift_correction_factor", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Combiner_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Combiner_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Combiner_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Combiner_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::Gravity_Filter_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Gravity_Filter_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Gravity_Filter_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Gravity_Filter_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::ECEF_KF_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::ECEF_KF_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::ECEF_KF_Filter_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gps_position_lag");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gps_position_lag'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gps_position_lag())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gps_position_lag(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gps_position_accuracy");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gps_position_accuracy'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gps_position_accuracy())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gps_position_accuracy(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gps_velocity_lag");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gps_velocity_lag'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gps_velocity_lag())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gps_velocity_lag(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gps_velocity_accuracy");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gps_velocity_accuracy'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gps_velocity_accuracy())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gps_velocity_accuracy(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("acceleration_lag");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'acceleration_lag'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_acceleration_lag())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_acceleration_lag(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("acceleration_accuracy");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'acceleration_accuracy'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_acceleration_accuracy())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_acceleration_accuracy(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::ECEF_KF_Filter_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_gps_position_lag());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gps_position_lag", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gps_position_accuracy());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gps_position_accuracy", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gps_velocity_lag());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gps_velocity_lag", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gps_velocity_accuracy());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gps_velocity_accuracy", result.extract_payload());
  }
  {
    auto result = serialize(value.get_acceleration_lag());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("acceleration_lag", result.extract_payload());
  }
  {
    auto result = serialize(value.get_acceleration_accuracy());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("acceleration_accuracy", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::LPF_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::LPF_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::LPF_Filter_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("lpf_poles");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'lpf_poles'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_lpf_poles())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_lpf_poles(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("lpf_cutoff_frequency");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'lpf_cutoff_frequency'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_lpf_cutoff_frequency())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_lpf_cutoff_frequency(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::LPF_Filter_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_lpf_poles());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("lpf_poles", result.extract_payload());
  }
  {
    auto result = serialize(value.get_lpf_cutoff_frequency());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("lpf_cutoff_frequency", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::MaxSonar_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bus");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bus'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bus(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::MaxSonar_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_bus());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bus", result.extract_payload());
  }
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::MaxSonar_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("direction");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'direction'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_direction())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_direction(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_distance");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_distance'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_distance())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_distance(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("min_distance");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'min_distance'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_min_distance())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_min_distance(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::MaxSonar_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_direction());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("direction", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max_distance());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_distance", result.extract_payload());
  }
  {
    auto result = serialize(value.get_min_distance());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("min_distance", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Motor_Mixer_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Motor_Mixer_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Motor_Mixer_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("armed_thrust");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'armed_thrust'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_armed_thrust())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_armed_thrust(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Motor_Mixer_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_armed_thrust());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("armed_thrust", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Acceleration_Calibration_Point& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("temperature");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'temperature'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_temperature())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_temperature(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bias");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bias'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bias())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bias(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("scale");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'scale'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_scale())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_scale(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Acceleration_Calibration_Point const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_temperature());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("temperature", result.extract_payload());
  }
  {
    auto result = serialize(value.get_bias());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bias", result.extract_payload());
  }
  {
    auto result = serialize(value.get_scale());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("scale", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Angular_Velocity_Calibration_Point& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("temperature");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'temperature'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_temperature())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_temperature(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bias");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bias'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bias())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bias(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Angular_Velocity_Calibration_Point const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_temperature());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("temperature", result.extract_payload());
  }
  {
    auto result = serialize(value.get_bias());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bias", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Magnetic_Field_Calibration_Point& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("temperature");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'temperature'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_temperature())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_temperature(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bias");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bias'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bias())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bias(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("scale");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'scale'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_scale())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_scale(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Magnetic_Field_Calibration_Point const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_temperature());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("temperature", result.extract_payload());
  }
  {
    auto result = serialize(value.get_bias());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bias", result.extract_payload());
  }
  {
    auto result = serialize(value.get_scale());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("scale", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::MPU9250_Descriptor::imu_rate_t& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string or null value when deserializing"); }
  std::string const& str = sz_value.get_as_string();
  if (false) {} //this is here just to have the next items with 'else if'
  else if (str == "_250")
  {
    value = ::silk::MPU9250_Descriptor::imu_rate_t::_250;
  }
  else if (str == "_500")
  {
    value = ::silk::MPU9250_Descriptor::imu_rate_t::_500;
  }
  else if (str == "_1000")
  {
    value = ::silk::MPU9250_Descriptor::imu_rate_t::_1000;
  }
  else { return ts::Error("Cannot find item '" + str + "' when deserializing"); }
}
ts::Result<ts::serialization::Value> serialize(::silk::MPU9250_Descriptor::imu_rate_t const& value)
{
  if (false) {} //this is here just to have the next items with 'else if'
  else if (value == ::silk::MPU9250_Descriptor::imu_rate_t::_250)
  {
    return ts::serialization::Value("_250");
  }
  else if (value == ::silk::MPU9250_Descriptor::imu_rate_t::_500)
  {
    return ts::serialization::Value("_500");
  }
  else if (value == ::silk::MPU9250_Descriptor::imu_rate_t::_1000)
  {
    return ts::serialization::Value("_1000");
  }
  else { return ts::Error("Cannot serialize type"); }
}
ts::Result<void> deserialize(::silk::MPU9250_Descriptor::acceleration_range_t& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string or null value when deserializing"); }
  std::string const& str = sz_value.get_as_string();
  if (false) {} //this is here just to have the next items with 'else if'
  else if (str == "_2")
  {
    value = ::silk::MPU9250_Descriptor::acceleration_range_t::_2;
  }
  else if (str == "_4")
  {
    value = ::silk::MPU9250_Descriptor::acceleration_range_t::_4;
  }
  else if (str == "_8")
  {
    value = ::silk::MPU9250_Descriptor::acceleration_range_t::_8;
  }
  else if (str == "_16")
  {
    value = ::silk::MPU9250_Descriptor::acceleration_range_t::_16;
  }
  else { return ts::Error("Cannot find item '" + str + "' when deserializing"); }
}
ts::Result<ts::serialization::Value> serialize(::silk::MPU9250_Descriptor::acceleration_range_t const& value)
{
  if (false) {} //this is here just to have the next items with 'else if'
  else if (value == ::silk::MPU9250_Descriptor::acceleration_range_t::_2)
  {
    return ts::serialization::Value("_2");
  }
  else if (value == ::silk::MPU9250_Descriptor::acceleration_range_t::_4)
  {
    return ts::serialization::Value("_4");
  }
  else if (value == ::silk::MPU9250_Descriptor::acceleration_range_t::_8)
  {
    return ts::serialization::Value("_8");
  }
  else if (value == ::silk::MPU9250_Descriptor::acceleration_range_t::_16)
  {
    return ts::serialization::Value("_16");
  }
  else { return ts::Error("Cannot serialize type"); }
}
ts::Result<void> deserialize(::silk::MPU9250_Descriptor::angular_velocity_range_t& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string or null value when deserializing"); }
  std::string const& str = sz_value.get_as_string();
  if (false) {} //this is here just to have the next items with 'else if'
  else if (str == "_250")
  {
    value = ::silk::MPU9250_Descriptor::angular_velocity_range_t::_250;
  }
  else if (str == "_500")
  {
    value = ::silk::MPU9250_Descriptor::angular_velocity_range_t::_500;
  }
  else if (str == "_1000")
  {
    value = ::silk::MPU9250_Descriptor::angular_velocity_range_t::_1000;
  }
  else if (str == "_2000")
  {
    value = ::silk::MPU9250_Descriptor::angular_velocity_range_t::_2000;
  }
  else { return ts::Error("Cannot find item '" + str + "' when deserializing"); }
}
ts::Result<ts::serialization::Value> serialize(::silk::MPU9250_Descriptor::angular_velocity_range_t const& value)
{
  if (false) {} //this is here just to have the next items with 'else if'
  else if (value == ::silk::MPU9250_Descriptor::angular_velocity_range_t::_250)
  {
    return ts::serialization::Value("_250");
  }
  else if (value == ::silk::MPU9250_Descriptor::angular_velocity_range_t::_500)
  {
    return ts::serialization::Value("_500");
  }
  else if (value == ::silk::MPU9250_Descriptor::angular_velocity_range_t::_1000)
  {
    return ts::serialization::Value("_1000");
  }
  else if (value == ::silk::MPU9250_Descriptor::angular_velocity_range_t::_2000)
  {
    return ts::serialization::Value("_2000");
  }
  else { return ts::Error("Cannot serialize type"); }
}
ts::Result<void> deserialize(::silk::MPU9250_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bus");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bus'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bus(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("imu_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'imu_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_imu_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_imu_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("magnetometer_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'magnetometer_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_magnetometer_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_magnetometer_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("thermometer_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'thermometer_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_thermometer_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_thermometer_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("acceleration_range");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'acceleration_range'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_acceleration_range())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_acceleration_range(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("angular_velocity_range");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'angular_velocity_range'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_angular_velocity_range())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_angular_velocity_range(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::MPU9250_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_bus());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bus", result.extract_payload());
  }
  {
    auto result = serialize(value.get_imu_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("imu_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_magnetometer_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("magnetometer_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_thermometer_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("thermometer_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_acceleration_range());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("acceleration_range", result.extract_payload());
  }
  {
    auto result = serialize(value.get_angular_velocity_range());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("angular_velocity_range", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::MPU9250_Config::Calibration& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("acceleration");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'acceleration'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_acceleration())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_acceleration(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("angular_velocity");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'angular_velocity'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_angular_velocity())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_angular_velocity(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("magnetic_field");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'magnetic_field'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_magnetic_field())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_magnetic_field(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::MPU9250_Config::Calibration const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_acceleration());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("acceleration", result.extract_payload());
  }
  {
    auto result = serialize(value.get_angular_velocity());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("angular_velocity", result.extract_payload());
  }
  {
    auto result = serialize(value.get_magnetic_field());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("magnetic_field", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::MPU9250_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rotation");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rotation'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rotation())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rotation(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("calibration");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'calibration'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_calibration())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_calibration(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::MPU9250_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rotation());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rotation", result.extract_payload());
  }
  {
    auto result = serialize(value.get_calibration());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("calibration", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::MS5611_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bus");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bus'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bus(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("pressure_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'pressure_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_pressure_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_pressure_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("temperature_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'temperature_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_temperature_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_temperature_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::MS5611_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_bus());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bus", result.extract_payload());
  }
  {
    auto result = serialize(value.get_pressure_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("pressure_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_temperature_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("temperature_rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::MS5611_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::MS5611_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Brain_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("commands_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'commands_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_commands_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_commands_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("state_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'state_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_state_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_state_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_commands_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("commands_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_state_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("state_rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("kp");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'kp'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_kp())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_kp(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("kd");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'kd'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_kd())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_kd(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("d_filter");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'd_filter'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_d_filter())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_d_filter(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("ki");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'ki'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_ki())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_ki(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_i");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_i'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_i())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_i(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_kp());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("kp", result.extract_payload());
  }
  {
    auto result = serialize(value.get_kd());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("kd", result.extract_payload());
  }
  {
    auto result = serialize(value.get_d_filter());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("d_filter", result.extract_payload());
  }
  {
    auto result = serialize(value.get_ki());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("ki", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max_i());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_i", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("x_pid");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'x_pid'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_x_pid())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_x_pid(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("y_pid");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'y_pid'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_y_pid())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_y_pid(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_x_pid());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("x_pid", result.extract_payload());
  }
  {
    auto result = serialize(value.get_y_pid());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("y_pid", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config::Horizontal_Angle& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_speed_deg");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_speed_deg'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_speed_deg())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_speed_deg(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("xy_pids");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'xy_pids'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_xy_pids())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_xy_pids(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config::Horizontal_Angle const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_max_speed_deg());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_speed_deg", result.extract_payload());
  }
  {
    auto result = serialize(value.get_xy_pids());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("xy_pids", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config::Horizontal_Position& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_speed");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_speed'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_speed())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_speed(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("lpf");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'lpf'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_lpf())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_lpf(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("velocity_pi");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'velocity_pi'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_velocity_pi())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_velocity_pi(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("position_p");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'position_p'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_position_p())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_position_p(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config::Horizontal_Position const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_max_speed());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_speed", result.extract_payload());
  }
  {
    auto result = serialize(value.get_lpf());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("lpf", result.extract_payload());
  }
  {
    auto result = serialize(value.get_velocity_pi());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("velocity_pi", result.extract_payload());
  }
  {
    auto result = serialize(value.get_position_p());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("position_p", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config::Yaw_Angle& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_speed_deg");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_speed_deg'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_speed_deg())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_speed_deg(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("pid");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'pid'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_pid())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_pid(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config::Yaw_Angle const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_max_speed_deg());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_speed_deg", result.extract_payload());
  }
  {
    auto result = serialize(value.get_pid());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("pid", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config::Altitude& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_speed");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_speed'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_speed())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_speed(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("lpf");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'lpf'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_lpf())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_lpf(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("speed_pi");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'speed_pi'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_speed_pi())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_speed_pi(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("position_p");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'position_p'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_position_p())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_position_p(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config::Altitude const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_max_speed());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_speed", result.extract_payload());
  }
  {
    auto result = serialize(value.get_lpf());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("lpf", result.extract_payload());
  }
  {
    auto result = serialize(value.get_speed_pi());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("speed_pi", result.extract_payload());
  }
  {
    auto result = serialize(value.get_position_p());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("position_p", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Brain_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("horizontal_angle");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'horizontal_angle'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_horizontal_angle())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_horizontal_angle(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("horizontal_position");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'horizontal_position'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_horizontal_position())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_horizontal_position(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("yaw_angle");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'yaw_angle'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_yaw_angle())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_yaw_angle(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("altitude");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'altitude'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_altitude())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_altitude(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Brain_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_horizontal_angle());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("horizontal_angle", result.extract_payload());
  }
  {
    auto result = serialize(value.get_horizontal_position());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("horizontal_position", result.extract_payload());
  }
  {
    auto result = serialize(value.get_yaw_angle());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("yaw_angle", result.extract_payload());
  }
  {
    auto result = serialize(value.get_altitude());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("altitude", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Pilot_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("commands_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'commands_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_commands_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_commands_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("state_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'state_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_state_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_state_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("video_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'video_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_video_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_video_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Pilot_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_commands_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("commands_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_state_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("state_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_video_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("video_rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Pilot_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Pilot_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Simulator_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("throttle_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'throttle_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_throttle_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_throttle_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("acceleration_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'acceleration_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_acceleration_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_acceleration_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("angular_velocity_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'angular_velocity_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_angular_velocity_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_angular_velocity_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("magnetic_field_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'magnetic_field_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_magnetic_field_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_magnetic_field_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("pressure_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'pressure_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_pressure_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_pressure_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("temperature_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'temperature_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_temperature_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_temperature_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("distance_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'distance_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_distance_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_distance_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gps_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gps_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gps_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gps_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Simulator_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_throttle_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("throttle_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_acceleration_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("acceleration_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_angular_velocity_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("angular_velocity_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_magnetic_field_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("magnetic_field_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_pressure_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("pressure_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_temperature_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("temperature_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_distance_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("distance_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gps_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gps_rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Simulator_Config::Noise& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gps_position");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gps_position'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gps_position())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gps_position(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gps_velocity");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gps_velocity'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gps_velocity())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gps_velocity(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gps_pacc");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gps_pacc'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gps_pacc())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gps_pacc(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gps_vacc");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gps_vacc'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gps_vacc())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gps_vacc(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("acceleration");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'acceleration'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_acceleration())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_acceleration(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("angular_velocity");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'angular_velocity'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_angular_velocity())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_angular_velocity(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("magnetic_field");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'magnetic_field'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_magnetic_field())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_magnetic_field(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("pressure");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'pressure'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_pressure())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_pressure(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("temperature");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'temperature'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_temperature())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_temperature(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("ground_distance");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'ground_distance'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_ground_distance())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_ground_distance(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Simulator_Config::Noise const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_gps_position());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gps_position", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gps_velocity());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gps_velocity", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gps_pacc());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gps_pacc", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gps_vacc());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gps_vacc", result.extract_payload());
  }
  {
    auto result = serialize(value.get_acceleration());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("acceleration", result.extract_payload());
  }
  {
    auto result = serialize(value.get_angular_velocity());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("angular_velocity", result.extract_payload());
  }
  {
    auto result = serialize(value.get_magnetic_field());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("magnetic_field", result.extract_payload());
  }
  {
    auto result = serialize(value.get_pressure());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("pressure", result.extract_payload());
  }
  {
    auto result = serialize(value.get_temperature());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("temperature", result.extract_payload());
  }
  {
    auto result = serialize(value.get_ground_distance());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("ground_distance", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Multirotor_Simulator_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("simulation_enabled");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'simulation_enabled'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_simulation_enabled())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_simulation_enabled(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("drag_enabled");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'drag_enabled'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_drag_enabled())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_drag_enabled(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("ground_enabled");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'ground_enabled'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_ground_enabled())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_ground_enabled(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gravity_enabled");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gravity_enabled'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gravity_enabled())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gravity_enabled(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("noise");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'noise'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_noise())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_noise(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Multirotor_Simulator_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_simulation_enabled());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("simulation_enabled", result.extract_payload());
  }
  {
    auto result = serialize(value.get_drag_enabled());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("drag_enabled", result.extract_payload());
  }
  {
    auto result = serialize(value.get_ground_enabled());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("ground_enabled", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gravity_enabled());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gravity_enabled", result.extract_payload());
  }
  {
    auto result = serialize(value.get_noise());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("noise", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Oscillator_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("component_count");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'component_count'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_component_count())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_component_count(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Oscillator_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_component_count());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("component_count", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Oscillator_Config::Component& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("frequency");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'frequency'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_frequency())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_frequency(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("amplitude");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'amplitude'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_amplitude())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_amplitude(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("square");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'square'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_square())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_square(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Oscillator_Config::Component const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_frequency());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("frequency", result.extract_payload());
  }
  {
    auto result = serialize(value.get_amplitude());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("amplitude", result.extract_payload());
  }
  {
    auto result = serialize(value.get_square());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("square", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Oscillator_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("amplitude");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'amplitude'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_amplitude())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_amplitude(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("noise");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'noise'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_noise())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_noise(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("components");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'components'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_components())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_components(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Oscillator_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_amplitude());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("amplitude", result.extract_payload());
  }
  {
    auto result = serialize(value.get_noise());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("noise", result.extract_payload());
  }
  {
    auto result = serialize(value.get_components());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("components", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::PCA9685_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bus");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bus'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bus(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("address");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'address'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_address())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_address(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::PCA9685_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_bus());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bus", result.extract_payload());
  }
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_address());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("address", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::PCA9685_Config::Channel& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("servo_signal");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'servo_signal'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_servo_signal())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_servo_signal(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("min_servo");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'min_servo'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_min_servo())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_min_servo(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_servo");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_servo'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_servo())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_servo(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("min_pwm");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'min_pwm'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_min_pwm())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_min_pwm(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_pwm");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_pwm'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_pwm())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_pwm(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::PCA9685_Config::Channel const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_servo_signal());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("servo_signal", result.extract_payload());
  }
  {
    auto result = serialize(value.get_min_servo());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("min_servo", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max_servo());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_servo", result.extract_payload());
  }
  {
    auto result = serialize(value.get_min_pwm());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("min_pwm", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max_pwm());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_pwm", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::PCA9685_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("channels");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'channels'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_channels())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_channels(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::PCA9685_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_channels());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("channels", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::PIGPIO_Descriptor::Channel& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("enabled");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'enabled'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_enabled())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_enabled(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("servo_signal");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'servo_signal'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_servo_signal())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_servo_signal(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::PIGPIO_Descriptor::Channel const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_enabled());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("enabled", result.extract_payload());
  }
  {
    auto result = serialize(value.get_servo_signal());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("servo_signal", result.extract_payload());
  }
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::PIGPIO_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_2");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_2'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_2())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_2(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_3");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_3'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_3())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_3(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_4");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_4'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_4())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_4(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_5");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_5'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_5())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_5(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_6");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_6'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_6())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_6(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_7");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_7'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_7())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_7(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_8");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_8'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_8())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_8(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_9");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_9'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_9())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_9(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_10");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_10'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_10())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_10(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_11");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_11'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_11())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_11(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_12");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_12'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_12())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_12(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_13");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_13'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_13())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_13(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_14");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_14'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_14())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_14(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_15");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_15'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_15())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_15(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_16");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_16'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_16())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_16(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_17");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_17'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_17())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_17(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_18");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_18'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_18())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_18(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_19");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_19'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_19())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_19(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_20");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_20'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_20())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_20(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_21");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_21'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_21())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_21(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_22");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_22'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_22())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_22(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_23");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_23'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_23())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_23(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_24");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_24'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_24())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_24(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_25");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_25'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_25())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_25(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_26");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_26'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_26())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_26(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_27");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_27'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_27())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_27(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::PIGPIO_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_gpio_2());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_2", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_3());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_3", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_4());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_4", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_5());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_5", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_6());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_6", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_7());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_7", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_8());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_8", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_9());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_9", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_10());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_10", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_11());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_11", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_12());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_12", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_13());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_13", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_14());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_14", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_15());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_15", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_16());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_16", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_17());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_17", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_18());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_18", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_19());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_19", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_20());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_20", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_21());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_21", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_22());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_22", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_23());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_23", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_24());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_24", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_25());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_25", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_26());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_26", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_27());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_27", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::PIGPIO_Config::Servo_Channel& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("min");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'min'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_min())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_min(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::PIGPIO_Config::Servo_Channel const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_min());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("min", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::PIGPIO_Config::PWM_Channel& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("min");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'min'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_min())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_min(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::PIGPIO_Config::PWM_Channel const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_min());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("min", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::PIGPIO_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_2");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_2'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_2())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_2(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_3");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_3'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_3())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_3(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_4");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_4'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_4())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_4(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_5");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_5'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_5())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_5(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_6");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_6'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_6())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_6(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_7");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_7'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_7())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_7(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_8");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_8'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_8())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_8(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_9");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_9'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_9())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_9(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_10");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_10'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_10())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_10(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_11");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_11'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_11())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_11(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_12");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_12'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_12())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_12(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_13");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_13'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_13())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_13(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_14");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_14'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_14())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_14(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_15");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_15'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_15())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_15(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_16");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_16'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_16())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_16(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_17");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_17'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_17())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_17(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_18");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_18'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_18())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_18(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_19");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_19'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_19())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_19(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_20");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_20'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_20())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_20(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_21");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_21'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_21())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_21(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_22");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_22'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_22())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_22(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_23");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_23'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_23())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_23(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_24");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_24'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_24())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_24(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_25");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_25'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_25())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_25(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_26");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_26'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_26())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_26(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("gpio_27");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'gpio_27'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_gpio_27())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_gpio_27(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::PIGPIO_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_gpio_2());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_2", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_3());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_3", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_4());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_4", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_5());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_5", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_6());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_6", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_7());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_7", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_8());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_8", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_9());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_9", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_10());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_10", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_11());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_11", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_12());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_12", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_13());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_13", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_14());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_14", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_15());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_15", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_16());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_16", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_17());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_17", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_18());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_18", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_19());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_19", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_20());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_20", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_21());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_21", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_22());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_22", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_23());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_23", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_24());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_24", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_25());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_25", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_26());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_26", result.extract_payload());
  }
  {
    auto result = serialize(value.get_gpio_27());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("gpio_27", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Pressure_Velocity_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Pressure_Velocity_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Pressure_Velocity_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Pressure_Velocity_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::Proximity_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("channel_count");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'channel_count'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_channel_count())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_channel_count(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Proximity_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_channel_count());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("channel_count", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Proximity_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Proximity_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::Rate_Controller_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Rate_Controller_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("kp");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'kp'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_kp())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_kp(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("kd");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'kd'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_kd())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_kd(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("d_filter");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'd_filter'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_d_filter())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_d_filter(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("ki");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'ki'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_ki())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_ki(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_i");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_i'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_i())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_i(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_kp());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("kp", result.extract_payload());
  }
  {
    auto result = serialize(value.get_kd());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("kd", result.extract_payload());
  }
  {
    auto result = serialize(value.get_d_filter());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("d_filter", result.extract_payload());
  }
  {
    auto result = serialize(value.get_ki());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("ki", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max_i());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_i", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("x_pid");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'x_pid'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_x_pid())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_x_pid(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("y_pid");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'y_pid'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_y_pid())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_y_pid(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_x_pid());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("x_pid", result.extract_payload());
  }
  {
    auto result = serialize(value.get_y_pid());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("y_pid", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Rate_Controller_Config::Feedback& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("weight");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'weight'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_weight())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_weight(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("xy_pids");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'xy_pids'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_xy_pids())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_xy_pids(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("z_pid");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'z_pid'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_z_pid())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_z_pid(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Rate_Controller_Config::Feedback const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_weight());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("weight", result.extract_payload());
  }
  {
    auto result = serialize(value.get_xy_pids());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("xy_pids", result.extract_payload());
  }
  {
    auto result = serialize(value.get_z_pid());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("z_pid", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Rate_Controller_Config::Feedforward& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("weight");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'weight'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_weight())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_weight(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_torque");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_torque'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_torque())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_torque(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Rate_Controller_Config::Feedforward const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_weight());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("weight", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max_torque());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_torque", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Rate_Controller_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("feedback");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'feedback'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_feedback())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_feedback(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("feedforward");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'feedforward'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_feedforward())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_feedforward(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Rate_Controller_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_feedback());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("feedback", result.extract_payload());
  }
  {
    auto result = serialize(value.get_feedforward());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("feedforward", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Raspicam_Descriptor::Quality& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("resolution");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'resolution'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_resolution())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_resolution(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bitrate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bitrate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bitrate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bitrate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Raspicam_Descriptor::Quality const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_resolution());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("resolution", result.extract_payload());
  }
  {
    auto result = serialize(value.get_bitrate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bitrate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Raspicam_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("fps");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'fps'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_fps())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_fps(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("streaming_low");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'streaming_low'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_streaming_low())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_streaming_low(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("streaming_high");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'streaming_high'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_streaming_high())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_streaming_high(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("recording");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'recording'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_recording())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_recording(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Raspicam_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_fps());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("fps", result.extract_payload());
  }
  {
    auto result = serialize(value.get_streaming_low());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("streaming_low", result.extract_payload());
  }
  {
    auto result = serialize(value.get_streaming_high());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("streaming_high", result.extract_payload());
  }
  {
    auto result = serialize(value.get_recording());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("recording", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Raspicam_Config::awb_mode_t& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string or null value when deserializing"); }
  std::string const& str = sz_value.get_as_string();
  if (false) {} //this is here just to have the next items with 'else if'
  else if (str == "OFF")
  {
    value = ::silk::Raspicam_Config::awb_mode_t::OFF;
  }
  else if (str == "AUTO")
  {
    value = ::silk::Raspicam_Config::awb_mode_t::AUTO;
  }
  else if (str == "SUNLIGHT")
  {
    value = ::silk::Raspicam_Config::awb_mode_t::SUNLIGHT;
  }
  else if (str == "CLOUDY")
  {
    value = ::silk::Raspicam_Config::awb_mode_t::CLOUDY;
  }
  else if (str == "SHADE")
  {
    value = ::silk::Raspicam_Config::awb_mode_t::SHADE;
  }
  else if (str == "TUNGSTEN")
  {
    value = ::silk::Raspicam_Config::awb_mode_t::TUNGSTEN;
  }
  else if (str == "FLUORESCENT")
  {
    value = ::silk::Raspicam_Config::awb_mode_t::FLUORESCENT;
  }
  else if (str == "INCANDESCENT")
  {
    value = ::silk::Raspicam_Config::awb_mode_t::INCANDESCENT;
  }
  else if (str == "FLASH")
  {
    value = ::silk::Raspicam_Config::awb_mode_t::FLASH;
  }
  else if (str == "HORIZON")
  {
    value = ::silk::Raspicam_Config::awb_mode_t::HORIZON;
  }
  else { return ts::Error("Cannot find item '" + str + "' when deserializing"); }
}
ts::Result<ts::serialization::Value> serialize(::silk::Raspicam_Config::awb_mode_t const& value)
{
  if (false) {} //this is here just to have the next items with 'else if'
  else if (value == ::silk::Raspicam_Config::awb_mode_t::OFF)
  {
    return ts::serialization::Value("OFF");
  }
  else if (value == ::silk::Raspicam_Config::awb_mode_t::AUTO)
  {
    return ts::serialization::Value("AUTO");
  }
  else if (value == ::silk::Raspicam_Config::awb_mode_t::SUNLIGHT)
  {
    return ts::serialization::Value("SUNLIGHT");
  }
  else if (value == ::silk::Raspicam_Config::awb_mode_t::CLOUDY)
  {
    return ts::serialization::Value("CLOUDY");
  }
  else if (value == ::silk::Raspicam_Config::awb_mode_t::SHADE)
  {
    return ts::serialization::Value("SHADE");
  }
  else if (value == ::silk::Raspicam_Config::awb_mode_t::TUNGSTEN)
  {
    return ts::serialization::Value("TUNGSTEN");
  }
  else if (value == ::silk::Raspicam_Config::awb_mode_t::FLUORESCENT)
  {
    return ts::serialization::Value("FLUORESCENT");
  }
  else if (value == ::silk::Raspicam_Config::awb_mode_t::INCANDESCENT)
  {
    return ts::serialization::Value("INCANDESCENT");
  }
  else if (value == ::silk::Raspicam_Config::awb_mode_t::FLASH)
  {
    return ts::serialization::Value("FLASH");
  }
  else if (value == ::silk::Raspicam_Config::awb_mode_t::HORIZON)
  {
    return ts::serialization::Value("HORIZON");
  }
  else { return ts::Error("Cannot serialize type"); }
}
ts::Result<void> deserialize(::silk::Raspicam_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("iso");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'iso'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_iso())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_iso(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("shutter_speed");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'shutter_speed'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_shutter_speed())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_shutter_speed(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("ev");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'ev'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_ev())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_ev(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("sharpness");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'sharpness'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_sharpness())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_sharpness(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("contrast");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'contrast'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_contrast())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_contrast(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("brightness");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'brightness'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_brightness())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_brightness(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("saturation");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'saturation'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_saturation())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_saturation(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("awb_mode");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'awb_mode'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_awb_mode())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_awb_mode(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("quality");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'quality'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_quality())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_quality(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("recording");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'recording'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_recording())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_recording(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Raspicam_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_iso());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("iso", result.extract_payload());
  }
  {
    auto result = serialize(value.get_shutter_speed());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("shutter_speed", result.extract_payload());
  }
  {
    auto result = serialize(value.get_ev());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("ev", result.extract_payload());
  }
  {
    auto result = serialize(value.get_sharpness());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("sharpness", result.extract_payload());
  }
  {
    auto result = serialize(value.get_contrast());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("contrast", result.extract_payload());
  }
  {
    auto result = serialize(value.get_brightness());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("brightness", result.extract_payload());
  }
  {
    auto result = serialize(value.get_saturation());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("saturation", result.extract_payload());
  }
  {
    auto result = serialize(value.get_awb_mode());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("awb_mode", result.extract_payload());
  }
  {
    auto result = serialize(value.get_quality());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("quality", result.extract_payload());
  }
  {
    auto result = serialize(value.get_recording());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("recording", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::RC5T619_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bus");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bus'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bus(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("adc0_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'adc0_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_adc0_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_adc0_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("adc1_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'adc1_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_adc1_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_adc1_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::RC5T619_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_bus());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bus", result.extract_payload());
  }
  {
    auto result = serialize(value.get_adc0_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("adc0_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_adc1_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("adc1_rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::RC5T619_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::RC5T619_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::Resampler_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("input_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'input_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_input_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_input_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("output_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'output_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_output_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_output_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Resampler_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_input_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("input_rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_output_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("output_rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Resampler_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("lpf");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'lpf'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_lpf())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_lpf(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Resampler_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_lpf());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("lpf", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Scalar_Generator_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Scalar_Generator_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Scalar_Generator_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("value");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'value'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_value())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_value(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Scalar_Generator_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_value());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Servo_Gimbal_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("commands_rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'commands_rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_commands_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_commands_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Servo_Gimbal_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_commands_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("commands_rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Servo_Gimbal_Config::Channel& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("min_angle");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'min_angle'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_min_angle())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_min_angle(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_angle");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_angle'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_angle())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_angle(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("min_pwm");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'min_pwm'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_min_pwm())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_min_pwm(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_pwm");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_pwm'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_pwm())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_pwm(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Servo_Gimbal_Config::Channel const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_min_angle());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("min_angle", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max_angle());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_angle", result.extract_payload());
  }
  {
    auto result = serialize(value.get_min_pwm());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("min_pwm", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max_pwm());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_pwm", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Servo_Gimbal_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("x_channel");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'x_channel'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_x_channel())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_x_channel(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("y_channel");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'y_channel'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_y_channel())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_y_channel(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("z_channel");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'z_channel'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_z_channel())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_z_channel(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Servo_Gimbal_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_x_channel());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("x_channel", result.extract_payload());
  }
  {
    auto result = serialize(value.get_y_channel());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("y_channel", result.extract_payload());
  }
  {
    auto result = serialize(value.get_z_channel());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("z_channel", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::SRF01_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bus");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bus'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bus(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::SRF01_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_bus());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bus", result.extract_payload());
  }
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::SRF01_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("direction");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'direction'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_direction())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_direction(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_distance");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_distance'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_distance())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_distance(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("min_distance");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'min_distance'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_min_distance())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_min_distance(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::SRF01_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_direction());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("direction", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max_distance());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_distance", result.extract_payload());
  }
  {
    auto result = serialize(value.get_min_distance());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("min_distance", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::SRF02_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bus");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bus'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bus(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::SRF02_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_bus());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bus", result.extract_payload());
  }
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::SRF02_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("direction");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'direction'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_direction())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_direction(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("max_distance");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'max_distance'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_max_distance())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_max_distance(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("min_distance");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'min_distance'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_min_distance())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_min_distance(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::SRF02_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_direction());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("direction", result.extract_payload());
  }
  {
    auto result = serialize(value.get_max_distance());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("max_distance", result.extract_payload());
  }
  {
    auto result = serialize(value.get_min_distance());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("min_distance", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Throttle_To_PWM_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("channel_count");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'channel_count'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_channel_count())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_channel_count(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Throttle_To_PWM_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_channel_count());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("channel_count", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Throttle_To_PWM_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Throttle_To_PWM_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::Transformer_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Transformer_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Transformer_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Transformer_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::UBLOX_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("bus");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'bus'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_bus(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::UBLOX_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_bus());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("bus", result.extract_payload());
  }
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::UBLOX_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::UBLOX_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::Vec3_Generator_Descriptor& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Vec3_Generator_Descriptor const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::Vec3_Generator_Config& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("value");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'value'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_value())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_value(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::Vec3_Generator_Config const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_value());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<::silk::Multirotor_Config::Motor>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Multirotor_Config::Motor> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::ARRAY);
  for (size_t i = 0; i < value.size(); i++)
  {
    auto result = serialize(value[i]);
    if (result != ts::success) { return result; }
    sz_value.add_array_element(result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(std::shared_ptr<::silk::Bus_Descriptor_Base>& value, ts::serialization::Value const& sz_value)
{
  if (sz_value.is_empty())
  {
    value = nullptr;    return ts::success;
  }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) {} //this is here just to have the next items with 'else if'
  else if (path == "::silk::UART_Linux_Descriptor")
  {
    value.reset(new ::silk::UART_Linux_Descriptor());
    return deserialize((::silk::UART_Linux_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::UART_BBang_Descriptor")
  {
    value.reset(new ::silk::UART_BBang_Descriptor());
    return deserialize((::silk::UART_BBang_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::I2C_BCM_Descriptor")
  {
    value.reset(new ::silk::I2C_BCM_Descriptor());
    return deserialize((::silk::I2C_BCM_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::I2C_Linux_Descriptor")
  {
    value.reset(new ::silk::I2C_Linux_Descriptor());
    return deserialize((::silk::I2C_Linux_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::SPI_BCM_Descriptor")
  {
    value.reset(new ::silk::SPI_BCM_Descriptor());
    return deserialize((::silk::SPI_BCM_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::SPI_Linux_Descriptor")
  {
    value.reset(new ::silk::SPI_Linux_Descriptor());
    return deserialize((::silk::SPI_Linux_Descriptor&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
}
ts::Result<ts::serialization::Value> serialize(std::shared_ptr<::silk::Bus_Descriptor_Base> const& value)
{
  if (!value) { return ts::serialization::Value(ts::serialization::Value::Type::EMPTY); }
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  if (false) {} //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(::silk::UART_Linux_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::UART_Linux_Descriptor");
    auto result = serialize((::silk::UART_Linux_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::UART_BBang_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::UART_BBang_Descriptor");
    auto result = serialize((::silk::UART_BBang_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::I2C_BCM_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::I2C_BCM_Descriptor");
    auto result = serialize((::silk::I2C_BCM_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::I2C_Linux_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::I2C_Linux_Descriptor");
    auto result = serialize((::silk::I2C_Linux_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::SPI_BCM_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::SPI_BCM_Descriptor");
    auto result = serialize((::silk::SPI_BCM_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::SPI_Linux_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::SPI_Linux_Descriptor");
    auto result = serialize((::silk::SPI_Linux_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else { return ts::Error("Cannot serialize type"); }
}
ts::Result<void> deserialize(std::shared_ptr<::silk::Node_Descriptor_Base>& value, ts::serialization::Value const& sz_value)
{
  if (sz_value.is_empty())
  {
    value = nullptr;    return ts::success;
  }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) {} //this is here just to have the next items with 'else if'
  else if (path == "::silk::ADC_Ammeter_Descriptor")
  {
    value.reset(new ::silk::ADC_Ammeter_Descriptor());
    return deserialize((::silk::ADC_Ammeter_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::ADC_Voltmeter_Descriptor")
  {
    value.reset(new ::silk::ADC_Voltmeter_Descriptor());
    return deserialize((::silk::ADC_Voltmeter_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::ADS1115_Descriptor")
  {
    value.reset(new ::silk::ADS1115_Descriptor());
    return deserialize((::silk::ADS1115_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::AVRADC_Descriptor")
  {
    value.reset(new ::silk::AVRADC_Descriptor());
    return deserialize((::silk::AVRADC_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Comp_AHRS_Descriptor")
  {
    value.reset(new ::silk::Comp_AHRS_Descriptor());
    return deserialize((::silk::Comp_AHRS_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Combiner_Descriptor")
  {
    value.reset(new ::silk::Combiner_Descriptor());
    return deserialize((::silk::Combiner_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Gravity_Filter_Descriptor")
  {
    value.reset(new ::silk::Gravity_Filter_Descriptor());
    return deserialize((::silk::Gravity_Filter_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::ECEF_KF_Descriptor")
  {
    value.reset(new ::silk::ECEF_KF_Descriptor());
    return deserialize((::silk::ECEF_KF_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::LPF_Descriptor")
  {
    value.reset(new ::silk::LPF_Descriptor());
    return deserialize((::silk::LPF_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::MaxSonar_Descriptor")
  {
    value.reset(new ::silk::MaxSonar_Descriptor());
    return deserialize((::silk::MaxSonar_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Motor_Mixer_Descriptor")
  {
    value.reset(new ::silk::Motor_Mixer_Descriptor());
    return deserialize((::silk::Motor_Mixer_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::MPU9250_Descriptor")
  {
    value.reset(new ::silk::MPU9250_Descriptor());
    return deserialize((::silk::MPU9250_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::MS5611_Descriptor")
  {
    value.reset(new ::silk::MS5611_Descriptor());
    return deserialize((::silk::MS5611_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Multirotor_Brain_Descriptor")
  {
    value.reset(new ::silk::Multirotor_Brain_Descriptor());
    return deserialize((::silk::Multirotor_Brain_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Multirotor_Pilot_Descriptor")
  {
    value.reset(new ::silk::Multirotor_Pilot_Descriptor());
    return deserialize((::silk::Multirotor_Pilot_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Multirotor_Simulator_Descriptor")
  {
    value.reset(new ::silk::Multirotor_Simulator_Descriptor());
    return deserialize((::silk::Multirotor_Simulator_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Oscillator_Descriptor")
  {
    value.reset(new ::silk::Oscillator_Descriptor());
    return deserialize((::silk::Oscillator_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::PCA9685_Descriptor")
  {
    value.reset(new ::silk::PCA9685_Descriptor());
    return deserialize((::silk::PCA9685_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::PIGPIO_Descriptor")
  {
    value.reset(new ::silk::PIGPIO_Descriptor());
    return deserialize((::silk::PIGPIO_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Pressure_Velocity_Descriptor")
  {
    value.reset(new ::silk::Pressure_Velocity_Descriptor());
    return deserialize((::silk::Pressure_Velocity_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Proximity_Descriptor")
  {
    value.reset(new ::silk::Proximity_Descriptor());
    return deserialize((::silk::Proximity_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Rate_Controller_Descriptor")
  {
    value.reset(new ::silk::Rate_Controller_Descriptor());
    return deserialize((::silk::Rate_Controller_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Raspicam_Descriptor")
  {
    value.reset(new ::silk::Raspicam_Descriptor());
    return deserialize((::silk::Raspicam_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::RC5T619_Descriptor")
  {
    value.reset(new ::silk::RC5T619_Descriptor());
    return deserialize((::silk::RC5T619_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Resampler_Descriptor")
  {
    value.reset(new ::silk::Resampler_Descriptor());
    return deserialize((::silk::Resampler_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Scalar_Generator_Descriptor")
  {
    value.reset(new ::silk::Scalar_Generator_Descriptor());
    return deserialize((::silk::Scalar_Generator_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Servo_Gimbal_Descriptor")
  {
    value.reset(new ::silk::Servo_Gimbal_Descriptor());
    return deserialize((::silk::Servo_Gimbal_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::SRF01_Descriptor")
  {
    value.reset(new ::silk::SRF01_Descriptor());
    return deserialize((::silk::SRF01_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::SRF02_Descriptor")
  {
    value.reset(new ::silk::SRF02_Descriptor());
    return deserialize((::silk::SRF02_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Throttle_To_PWM_Descriptor")
  {
    value.reset(new ::silk::Throttle_To_PWM_Descriptor());
    return deserialize((::silk::Throttle_To_PWM_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Transformer_Descriptor")
  {
    value.reset(new ::silk::Transformer_Descriptor());
    return deserialize((::silk::Transformer_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::UBLOX_Descriptor")
  {
    value.reset(new ::silk::UBLOX_Descriptor());
    return deserialize((::silk::UBLOX_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::silk::Vec3_Generator_Descriptor")
  {
    value.reset(new ::silk::Vec3_Generator_Descriptor());
    return deserialize((::silk::Vec3_Generator_Descriptor&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
}
ts::Result<ts::serialization::Value> serialize(std::shared_ptr<::silk::Node_Descriptor_Base> const& value)
{
  if (!value) { return ts::serialization::Value(ts::serialization::Value::Type::EMPTY); }
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  if (false) {} //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(::silk::ADC_Ammeter_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::ADC_Ammeter_Descriptor");
    auto result = serialize((::silk::ADC_Ammeter_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::ADC_Voltmeter_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::ADC_Voltmeter_Descriptor");
    auto result = serialize((::silk::ADC_Voltmeter_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::ADS1115_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::ADS1115_Descriptor");
    auto result = serialize((::silk::ADS1115_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::AVRADC_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::AVRADC_Descriptor");
    auto result = serialize((::silk::AVRADC_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Comp_AHRS_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Comp_AHRS_Descriptor");
    auto result = serialize((::silk::Comp_AHRS_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Combiner_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Combiner_Descriptor");
    auto result = serialize((::silk::Combiner_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Gravity_Filter_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Gravity_Filter_Descriptor");
    auto result = serialize((::silk::Gravity_Filter_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::ECEF_KF_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::ECEF_KF_Descriptor");
    auto result = serialize((::silk::ECEF_KF_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::LPF_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::LPF_Descriptor");
    auto result = serialize((::silk::LPF_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::MaxSonar_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::MaxSonar_Descriptor");
    auto result = serialize((::silk::MaxSonar_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Motor_Mixer_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Motor_Mixer_Descriptor");
    auto result = serialize((::silk::Motor_Mixer_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::MPU9250_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::MPU9250_Descriptor");
    auto result = serialize((::silk::MPU9250_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::MS5611_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::MS5611_Descriptor");
    auto result = serialize((::silk::MS5611_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Multirotor_Brain_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Multirotor_Brain_Descriptor");
    auto result = serialize((::silk::Multirotor_Brain_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Multirotor_Pilot_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Multirotor_Pilot_Descriptor");
    auto result = serialize((::silk::Multirotor_Pilot_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Multirotor_Simulator_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Multirotor_Simulator_Descriptor");
    auto result = serialize((::silk::Multirotor_Simulator_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Oscillator_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Oscillator_Descriptor");
    auto result = serialize((::silk::Oscillator_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::PCA9685_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::PCA9685_Descriptor");
    auto result = serialize((::silk::PCA9685_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::PIGPIO_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::PIGPIO_Descriptor");
    auto result = serialize((::silk::PIGPIO_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Pressure_Velocity_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Pressure_Velocity_Descriptor");
    auto result = serialize((::silk::Pressure_Velocity_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Proximity_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Proximity_Descriptor");
    auto result = serialize((::silk::Proximity_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Rate_Controller_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Rate_Controller_Descriptor");
    auto result = serialize((::silk::Rate_Controller_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Raspicam_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Raspicam_Descriptor");
    auto result = serialize((::silk::Raspicam_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::RC5T619_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::RC5T619_Descriptor");
    auto result = serialize((::silk::RC5T619_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Resampler_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Resampler_Descriptor");
    auto result = serialize((::silk::Resampler_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Scalar_Generator_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Scalar_Generator_Descriptor");
    auto result = serialize((::silk::Scalar_Generator_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Servo_Gimbal_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Servo_Gimbal_Descriptor");
    auto result = serialize((::silk::Servo_Gimbal_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::SRF01_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::SRF01_Descriptor");
    auto result = serialize((::silk::SRF01_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::SRF02_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::SRF02_Descriptor");
    auto result = serialize((::silk::SRF02_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Throttle_To_PWM_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Throttle_To_PWM_Descriptor");
    auto result = serialize((::silk::Throttle_To_PWM_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Transformer_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Transformer_Descriptor");
    auto result = serialize((::silk::Transformer_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::UBLOX_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::UBLOX_Descriptor");
    auto result = serialize((::silk::UBLOX_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Vec3_Generator_Descriptor))
  {
    sz_value.add_object_member("type", "::silk::Vec3_Generator_Descriptor");
    auto result = serialize((::silk::Vec3_Generator_Descriptor&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else { return ts::Error("Cannot serialize type"); }
}
ts::Result<void> deserialize(std::shared_ptr<::silk::Node_Config_Base>& value, ts::serialization::Value const& sz_value)
{
  if (sz_value.is_empty())
  {
    value = nullptr;    return ts::success;
  }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) {} //this is here just to have the next items with 'else if'
  else if (path == "::silk::ADC_Ammeter_Config")
  {
    value.reset(new ::silk::ADC_Ammeter_Config());
    return deserialize((::silk::ADC_Ammeter_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::ADC_Voltmeter_Config")
  {
    value.reset(new ::silk::ADC_Voltmeter_Config());
    return deserialize((::silk::ADC_Voltmeter_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::ADS1115_Config")
  {
    value.reset(new ::silk::ADS1115_Config());
    return deserialize((::silk::ADS1115_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::AVRADC_Config")
  {
    value.reset(new ::silk::AVRADC_Config());
    return deserialize((::silk::AVRADC_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Comp_AHRS_Config")
  {
    value.reset(new ::silk::Comp_AHRS_Config());
    return deserialize((::silk::Comp_AHRS_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Combiner_Config")
  {
    value.reset(new ::silk::Combiner_Config());
    return deserialize((::silk::Combiner_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Gravity_Filter_Config")
  {
    value.reset(new ::silk::Gravity_Filter_Config());
    return deserialize((::silk::Gravity_Filter_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::ECEF_KF_Filter_Config")
  {
    value.reset(new ::silk::ECEF_KF_Filter_Config());
    return deserialize((::silk::ECEF_KF_Filter_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::LPF_Filter_Config")
  {
    value.reset(new ::silk::LPF_Filter_Config());
    return deserialize((::silk::LPF_Filter_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::MaxSonar_Config")
  {
    value.reset(new ::silk::MaxSonar_Config());
    return deserialize((::silk::MaxSonar_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Motor_Mixer_Config")
  {
    value.reset(new ::silk::Motor_Mixer_Config());
    return deserialize((::silk::Motor_Mixer_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::MPU9250_Config")
  {
    value.reset(new ::silk::MPU9250_Config());
    return deserialize((::silk::MPU9250_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::MS5611_Config")
  {
    value.reset(new ::silk::MS5611_Config());
    return deserialize((::silk::MS5611_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Multirotor_Brain_Config")
  {
    value.reset(new ::silk::Multirotor_Brain_Config());
    return deserialize((::silk::Multirotor_Brain_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Multirotor_Pilot_Config")
  {
    value.reset(new ::silk::Multirotor_Pilot_Config());
    return deserialize((::silk::Multirotor_Pilot_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Multirotor_Simulator_Config")
  {
    value.reset(new ::silk::Multirotor_Simulator_Config());
    return deserialize((::silk::Multirotor_Simulator_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Oscillator_Config")
  {
    value.reset(new ::silk::Oscillator_Config());
    return deserialize((::silk::Oscillator_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::PCA9685_Config")
  {
    value.reset(new ::silk::PCA9685_Config());
    return deserialize((::silk::PCA9685_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::PIGPIO_Config")
  {
    value.reset(new ::silk::PIGPIO_Config());
    return deserialize((::silk::PIGPIO_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Pressure_Velocity_Config")
  {
    value.reset(new ::silk::Pressure_Velocity_Config());
    return deserialize((::silk::Pressure_Velocity_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Proximity_Config")
  {
    value.reset(new ::silk::Proximity_Config());
    return deserialize((::silk::Proximity_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Rate_Controller_Config")
  {
    value.reset(new ::silk::Rate_Controller_Config());
    return deserialize((::silk::Rate_Controller_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Raspicam_Config")
  {
    value.reset(new ::silk::Raspicam_Config());
    return deserialize((::silk::Raspicam_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::RC5T619_Config")
  {
    value.reset(new ::silk::RC5T619_Config());
    return deserialize((::silk::RC5T619_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Resampler_Config")
  {
    value.reset(new ::silk::Resampler_Config());
    return deserialize((::silk::Resampler_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Scalar_Generator_Config")
  {
    value.reset(new ::silk::Scalar_Generator_Config());
    return deserialize((::silk::Scalar_Generator_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Servo_Gimbal_Config")
  {
    value.reset(new ::silk::Servo_Gimbal_Config());
    return deserialize((::silk::Servo_Gimbal_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::SRF01_Config")
  {
    value.reset(new ::silk::SRF01_Config());
    return deserialize((::silk::SRF01_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::SRF02_Config")
  {
    value.reset(new ::silk::SRF02_Config());
    return deserialize((::silk::SRF02_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Throttle_To_PWM_Config")
  {
    value.reset(new ::silk::Throttle_To_PWM_Config());
    return deserialize((::silk::Throttle_To_PWM_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Transformer_Config")
  {
    value.reset(new ::silk::Transformer_Config());
    return deserialize((::silk::Transformer_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::UBLOX_Config")
  {
    value.reset(new ::silk::UBLOX_Config());
    return deserialize((::silk::UBLOX_Config&)*value, *value_sz_value);
  }
  else if (path == "::silk::Vec3_Generator_Config")
  {
    value.reset(new ::silk::Vec3_Generator_Config());
    return deserialize((::silk::Vec3_Generator_Config&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
}
ts::Result<ts::serialization::Value> serialize(std::shared_ptr<::silk::Node_Config_Base> const& value)
{
  if (!value) { return ts::serialization::Value(ts::serialization::Value::Type::EMPTY); }
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  if (false) {} //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(::silk::ADC_Ammeter_Config))
  {
    sz_value.add_object_member("type", "::silk::ADC_Ammeter_Config");
    auto result = serialize((::silk::ADC_Ammeter_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::ADC_Voltmeter_Config))
  {
    sz_value.add_object_member("type", "::silk::ADC_Voltmeter_Config");
    auto result = serialize((::silk::ADC_Voltmeter_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::ADS1115_Config))
  {
    sz_value.add_object_member("type", "::silk::ADS1115_Config");
    auto result = serialize((::silk::ADS1115_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::AVRADC_Config))
  {
    sz_value.add_object_member("type", "::silk::AVRADC_Config");
    auto result = serialize((::silk::AVRADC_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Comp_AHRS_Config))
  {
    sz_value.add_object_member("type", "::silk::Comp_AHRS_Config");
    auto result = serialize((::silk::Comp_AHRS_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Combiner_Config))
  {
    sz_value.add_object_member("type", "::silk::Combiner_Config");
    auto result = serialize((::silk::Combiner_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Gravity_Filter_Config))
  {
    sz_value.add_object_member("type", "::silk::Gravity_Filter_Config");
    auto result = serialize((::silk::Gravity_Filter_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::ECEF_KF_Filter_Config))
  {
    sz_value.add_object_member("type", "::silk::ECEF_KF_Filter_Config");
    auto result = serialize((::silk::ECEF_KF_Filter_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::LPF_Filter_Config))
  {
    sz_value.add_object_member("type", "::silk::LPF_Filter_Config");
    auto result = serialize((::silk::LPF_Filter_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::MaxSonar_Config))
  {
    sz_value.add_object_member("type", "::silk::MaxSonar_Config");
    auto result = serialize((::silk::MaxSonar_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Motor_Mixer_Config))
  {
    sz_value.add_object_member("type", "::silk::Motor_Mixer_Config");
    auto result = serialize((::silk::Motor_Mixer_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::MPU9250_Config))
  {
    sz_value.add_object_member("type", "::silk::MPU9250_Config");
    auto result = serialize((::silk::MPU9250_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::MS5611_Config))
  {
    sz_value.add_object_member("type", "::silk::MS5611_Config");
    auto result = serialize((::silk::MS5611_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Multirotor_Brain_Config))
  {
    sz_value.add_object_member("type", "::silk::Multirotor_Brain_Config");
    auto result = serialize((::silk::Multirotor_Brain_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Multirotor_Pilot_Config))
  {
    sz_value.add_object_member("type", "::silk::Multirotor_Pilot_Config");
    auto result = serialize((::silk::Multirotor_Pilot_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Multirotor_Simulator_Config))
  {
    sz_value.add_object_member("type", "::silk::Multirotor_Simulator_Config");
    auto result = serialize((::silk::Multirotor_Simulator_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Oscillator_Config))
  {
    sz_value.add_object_member("type", "::silk::Oscillator_Config");
    auto result = serialize((::silk::Oscillator_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::PCA9685_Config))
  {
    sz_value.add_object_member("type", "::silk::PCA9685_Config");
    auto result = serialize((::silk::PCA9685_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::PIGPIO_Config))
  {
    sz_value.add_object_member("type", "::silk::PIGPIO_Config");
    auto result = serialize((::silk::PIGPIO_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Pressure_Velocity_Config))
  {
    sz_value.add_object_member("type", "::silk::Pressure_Velocity_Config");
    auto result = serialize((::silk::Pressure_Velocity_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Proximity_Config))
  {
    sz_value.add_object_member("type", "::silk::Proximity_Config");
    auto result = serialize((::silk::Proximity_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Rate_Controller_Config))
  {
    sz_value.add_object_member("type", "::silk::Rate_Controller_Config");
    auto result = serialize((::silk::Rate_Controller_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Raspicam_Config))
  {
    sz_value.add_object_member("type", "::silk::Raspicam_Config");
    auto result = serialize((::silk::Raspicam_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::RC5T619_Config))
  {
    sz_value.add_object_member("type", "::silk::RC5T619_Config");
    auto result = serialize((::silk::RC5T619_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Resampler_Config))
  {
    sz_value.add_object_member("type", "::silk::Resampler_Config");
    auto result = serialize((::silk::Resampler_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Scalar_Generator_Config))
  {
    sz_value.add_object_member("type", "::silk::Scalar_Generator_Config");
    auto result = serialize((::silk::Scalar_Generator_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Servo_Gimbal_Config))
  {
    sz_value.add_object_member("type", "::silk::Servo_Gimbal_Config");
    auto result = serialize((::silk::Servo_Gimbal_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::SRF01_Config))
  {
    sz_value.add_object_member("type", "::silk::SRF01_Config");
    auto result = serialize((::silk::SRF01_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::SRF02_Config))
  {
    sz_value.add_object_member("type", "::silk::SRF02_Config");
    auto result = serialize((::silk::SRF02_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Throttle_To_PWM_Config))
  {
    sz_value.add_object_member("type", "::silk::Throttle_To_PWM_Config");
    auto result = serialize((::silk::Throttle_To_PWM_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Transformer_Config))
  {
    sz_value.add_object_member("type", "::silk::Transformer_Config");
    auto result = serialize((::silk::Transformer_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::UBLOX_Config))
  {
    sz_value.add_object_member("type", "::silk::UBLOX_Config");
    auto result = serialize((::silk::UBLOX_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(::silk::Vec3_Generator_Config))
  {
    sz_value.add_object_member("type", "::silk::Vec3_Generator_Config");
    auto result = serialize((::silk::Vec3_Generator_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else { return ts::Error("Cannot serialize type"); }
}
ts::Result<void> deserialize(std::vector<std::string>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<std::string> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::ARRAY);
  for (size_t i = 0; i < value.size(); i++)
  {
    auto result = serialize(value[i]);
    if (result != ts::success) { return result; }
    sz_value.add_array_element(result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(std::shared_ptr<::silk::UAV_Config_Base>& value, ts::serialization::Value const& sz_value)
{
  if (sz_value.is_empty())
  {
    value = nullptr;    return ts::success;
  }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) {} //this is here just to have the next items with 'else if'
  else if (path == "::silk::Multirotor_Config")
  {
    value.reset(new ::silk::Multirotor_Config());
    return deserialize((::silk::Multirotor_Config&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
}
ts::Result<ts::serialization::Value> serialize(std::shared_ptr<::silk::UAV_Config_Base> const& value)
{
  if (!value) { return ts::serialization::Value(ts::serialization::Value::Type::EMPTY); }
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  if (false) {} //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(::silk::Multirotor_Config))
  {
    sz_value.add_object_member("type", "::silk::Multirotor_Config");
    auto result = serialize((::silk::Multirotor_Config&)*value);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else { return ts::Error("Cannot serialize type"); }
}
ts::Result<void> deserialize(std::vector<::silk::Bus>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Bus> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::ARRAY);
  for (size_t i = 0; i < value.size(); i++)
  {
    auto result = serialize(value[i]);
    if (result != ts::success) { return result; }
    sz_value.add_array_element(result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<::silk::Node>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Node> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::ARRAY);
  for (size_t i = 0; i < value.size(); i++)
  {
    auto result = serialize(value[i]);
    if (result != ts::success) { return result; }
    sz_value.add_array_element(result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<::silk::Acceleration_Calibration_Point>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Acceleration_Calibration_Point> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::ARRAY);
  for (size_t i = 0; i < value.size(); i++)
  {
    auto result = serialize(value[i]);
    if (result != ts::success) { return result; }
    sz_value.add_array_element(result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<::silk::Angular_Velocity_Calibration_Point>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Angular_Velocity_Calibration_Point> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::ARRAY);
  for (size_t i = 0; i < value.size(); i++)
  {
    auto result = serialize(value[i]);
    if (result != ts::success) { return result; }
    sz_value.add_array_element(result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<::silk::Magnetic_Field_Calibration_Point>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Magnetic_Field_Calibration_Point> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::ARRAY);
  for (size_t i = 0; i < value.size(); i++)
  {
    auto result = serialize(value[i]);
    if (result != ts::success) { return result; }
    sz_value.add_array_element(result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(boost::variant<silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs,silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs>& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) {} //this is here just to have the next items with 'else if'
  else if (path == "::silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs")
  {
    ::silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs v;
    auto result = deserialize(boost::get<::silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs>(value), *value_sz_value);
    if (result != ts::success) { return result; }
    value = v;
  }
  else if (path == "::silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs")
  {
    ::silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs v;
    auto result = deserialize(boost::get<::silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs>(value), *value_sz_value);
    if (result != ts::success) { return result; }
    value = v;
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
}
ts::Result<ts::serialization::Value> serialize(boost::variant<silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs,silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  if (false) {} //this is here just to have the next items with 'else if'
  else if (auto* v = boost::get<::silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs>(&value))
  {
    sz_value.add_object_member("type", "::silk::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs");
    auto result = serialize(*v);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<::silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs>(&value))
  {
    sz_value.add_object_member("type", "::silk::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs");
    auto result = serialize(*v);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else { return ts::Error("Cannot serialize type"); }
}
ts::Result<void> deserialize(std::vector<::silk::Oscillator_Config::Component>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::Oscillator_Config::Component> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::ARRAY);
  for (size_t i = 0; i < value.size(); i++)
  {
    auto result = serialize(value[i]);
    if (result != ts::success) { return result; }
    sz_value.add_array_element(result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<::silk::PCA9685_Config::Channel>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::PCA9685_Config::Channel> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::ARRAY);
  for (size_t i = 0; i < value.size(); i++)
  {
    auto result = serialize(value[i]);
    if (result != ts::success) { return result; }
    sz_value.add_array_element(result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(boost::variant<silk::PIGPIO_Config::Servo_Channel,silk::PIGPIO_Config::PWM_Channel>& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) {} //this is here just to have the next items with 'else if'
  else if (path == "::silk::PIGPIO_Config::Servo_Channel")
  {
    ::silk::PIGPIO_Config::Servo_Channel v;
    auto result = deserialize(boost::get<::silk::PIGPIO_Config::Servo_Channel>(value), *value_sz_value);
    if (result != ts::success) { return result; }
    value = v;
  }
  else if (path == "::silk::PIGPIO_Config::PWM_Channel")
  {
    ::silk::PIGPIO_Config::PWM_Channel v;
    auto result = deserialize(boost::get<::silk::PIGPIO_Config::PWM_Channel>(value), *value_sz_value);
    if (result != ts::success) { return result; }
    value = v;
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
}
ts::Result<ts::serialization::Value> serialize(boost::variant<silk::PIGPIO_Config::Servo_Channel,silk::PIGPIO_Config::PWM_Channel> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  if (false) {} //this is here just to have the next items with 'else if'
  else if (auto* v = boost::get<::silk::PIGPIO_Config::Servo_Channel>(&value))
  {
    sz_value.add_object_member("type", "::silk::PIGPIO_Config::Servo_Channel");
    auto result = serialize(*v);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<::silk::PIGPIO_Config::PWM_Channel>(&value))
  {
    sz_value.add_object_member("type", "::silk::PIGPIO_Config::PWM_Channel");
    auto result = serialize(*v);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else { return ts::Error("Cannot serialize type"); }
}
ts::Result<void> deserialize(boost::variant<silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs,silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs>& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) {} //this is here just to have the next items with 'else if'
  else if (path == "::silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs")
  {
    ::silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs v;
    auto result = deserialize(boost::get<::silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs>(value), *value_sz_value);
    if (result != ts::success) { return result; }
    value = v;
  }
  else if (path == "::silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs")
  {
    ::silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs v;
    auto result = deserialize(boost::get<::silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs>(value), *value_sz_value);
    if (result != ts::success) { return result; }
    value = v;
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
}
ts::Result<ts::serialization::Value> serialize(boost::variant<silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs,silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  if (false) {} //this is here just to have the next items with 'else if'
  else if (auto* v = boost::get<::silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs>(&value))
  {
    sz_value.add_object_member("type", "::silk::Rate_Controller_Config::Feedback::Combined_XY_PIDs");
    auto result = serialize(*v);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<::silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs>(&value))
  {
    sz_value.add_object_member("type", "::silk::Rate_Controller_Config::Feedback::Separate_XY_PIDs");
    auto result = serialize(*v);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else { return ts::Error("Cannot serialize type"); }
}
