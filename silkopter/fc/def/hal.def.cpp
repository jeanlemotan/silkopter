#include "hal.def.h"
namespace silk
{

namespace hal
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
std::string const& get_ast_json()
{
  static const std::string s_json = R"xxx({"type":1,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int8_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":-128}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":127}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"int8_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":255}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int16_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":-32768}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":32767}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"int16_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint16_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":65535}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint16_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int32_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":-2147483648}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":2147483647}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"int32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":4294967295}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"muf_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"direction_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vec3f"}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"euler_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":-180.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":180.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"euler3_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vec3f"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":-180.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":-180.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":-180.000000}]}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":180.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":180.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":180.000000}]}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"servo_signal_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":2.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"pwm_signal_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IUAV_Descriptor"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"UAV Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"name"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Name"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Comms"}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"channel_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":11}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"channel_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"channel"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"tx_power_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":20.500000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"tx_power_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"tx_power"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":20.500000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"TX Power"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Quality"}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"mtu_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":128}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1360}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"mtu_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"mtu"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1360}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"MTU"}]}]}]}]},{"type":15,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"B_1M_CCK"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11b 1Mbps CCK"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"B_2M_CCK"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11b 2Mbps CCK"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"B_2M_CCK_SHORT_PREAMBLE"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11b 2Mbps Short Preamble CCK"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"B_5_5M_CCK"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11b 5.5Mbps CCK"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"B_5_5M_CCK_SHORT_PREAMBLE"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11b 5.5Mbps Short Preamble CCK"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"B_11M_CCK"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11b 11Mbps CCK"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"B_11M_CCK_SHORT_PREAMBLE"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11b 11Mbps Short Preamble CCK"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"G_6M_ODFM"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11g 6Mbps ODFM"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"G_9M_ODFM"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11g 9Mbps ODFM"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"G_12M_ODFM"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11g 12Mbps ODFM"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"G_18M_ODFM"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11g 18Mbps ODFM"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"G_24M_ODFM"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11g 24Mbps ODFM"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"G_36M_ODFM"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11g 36Mbps ODFM"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"G_48M_ODFM"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11g 48Mbps ODFM"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"G_54M_ODFM"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11g 56Mbps ODFM"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t::B_11M_CCK"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"fec_k_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":16}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"fec_n_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":32}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"fec_k_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"fec_k"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":12}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"FEC K"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"fec_n_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"fec_n"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":20}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"FEC N"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Quality"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"low"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Low Quality Streaming"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Quality"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"high"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"High Quality Streaming"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Comms"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"comms"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Comms"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"mass_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.010000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":100.000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"mass_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"mass"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Mass"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Kg"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"moment_of_inertia"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Moment Of Inertia"}]}]}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Poly_IUAV_Descriptor"}]},{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IUAV_Descriptor"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IBus_Descriptor"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bus Descriptor"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Poly_IBus_Descriptor"}]},{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IBus_Descriptor"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Node Descriptor"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Poly_INode_Descriptor"}]},{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Node Config"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Poly_INode_Config"}]},{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Settings"}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Bus_Data"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"type"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"attributes":[{"name":"const","type":1,"value":true}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IBus_Descriptor"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"descriptor"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Node_Data"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"type"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"attributes":[{"name":"const","type":1,"value":true}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"descriptor"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"attributes":[{"name":"const","type":1,"value":true}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"config"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"input_paths"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"attributes":[{"name":"const","type":1,"value":true}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IUAV_Descriptor"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"uav_descriptor"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Bus_Data"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"buses"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Node_Data"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"nodes"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IUAV_Descriptor"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Multirotor Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"height_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.010000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":100.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"radius_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.010000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":100.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"torque_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.010000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1000.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"thrust_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.010000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1000.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.100000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"radius_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"radius"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Radius"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"height_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"height"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Height"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"torque_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"motor_z_torque"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Motor Z Torque"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Nm"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"thrust_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"motor_thrust"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Motor Thrust"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"N"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"motor_acceleration"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":10.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Motor acceleration"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"N/s"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"motor_deceleration"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":10.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Motor Deceleration"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"N/s"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Tri_Multirotor_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Descriptor"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Tri Multirotor Descriptor"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Quad_Multirotor_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Descriptor"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Quad Multirotor Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"plus_configuration"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"+ Configuration"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Hexa_Multirotor_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Descriptor"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hexa Multirotor Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"plus_configuration"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"+ Configuration"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Hexatri_Multirotor_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Descriptor"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hexatri Multirotor Descriptor"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Octo_Multirotor_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Descriptor"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Octo Multirotor Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"plus_configuration"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"+ Configuration"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Octaquad_Multirotor_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Descriptor"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Octoquad Multirotor Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"plus_configuration"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"+ Configuration"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Custom_Multirotor_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Descriptor"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Custom Multirotor Descriptor"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Motor"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vec3f"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"position"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"direction_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"thrust_vector"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"clockwise"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Motor"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"motors"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"UART_Linux_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IBus_Descriptor"}]}]}]},{"type":14,"children":[{"type":15,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"baud_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"_9600"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"9600"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_19200"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"19200"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_38400"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"38400"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_57600"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"57600"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_115200"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"115200"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_230400"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"230400"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"dev"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"/dev/ttyAMA0"}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"baud_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"baud"}]},{"type":20,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"baud_t::_115200"}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"UART_BB_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IBus_Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rx_pin"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"baud"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":115200}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"invert"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"I2C_BCM_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IBus_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"dev_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"baud_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"dev_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"dev"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"baud_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"baud"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":400000}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"I2C_Linux_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IBus_Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"dev"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"/dev/i2c0"}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"SPI_BCM_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IBus_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"dev_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"mode_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":3}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"dev_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"dev"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"baud"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1000000}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"mode_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"mode"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"speed"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1000000}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"SPI_Linux_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IBus_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"dev"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"/dev/ttyAMA0"}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"speed"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1000000}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"P_Controller_Descriptor"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"kp"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"kP"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PI_Controller_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"P_Controller_Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"ki"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"kI"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_i"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max I"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PD_Controller_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"P_Controller_Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"kd"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"kD"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"muf_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"d_filter"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"D Filter"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PID_Controller_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PD_Controller_Descriptor"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"ki"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"kI"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_i"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max I"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ADC_Ammeter_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ADC_Ammeter_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"scale"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Scale"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bias"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bias"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ADC_Voltmeter_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ADC_Voltmeter_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"scale"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Scale"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bias"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bias"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ADS1115_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":400}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":127}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"hex"}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":72},{"name":"hex","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"I2C Address"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ADC"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"is_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":200}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ADC"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"adc0"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ADC0"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ADC"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"adc1"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ADC1"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ADC"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"adc2"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ADC2"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ADC"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"adc3"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ADC3"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ADS1115_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"AVRADC_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":500}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":127}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"hex"}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":23},{"name":"hex","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"I2C Address"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":200}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"AVRADC_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Comp_AHRS_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":200}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Comp_AHRS_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"muf_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"drift_correction_factor"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.300000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Drift Correction Factor"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Combiner_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":200}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Combiner_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Gravity_Filter_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Gravity_Filter_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"KF_ECEF_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"KF_ECEF_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gps_position_lag"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPS Position Lag"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"s"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gps_position_accuracy"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPS Position Accuracy"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m CEP"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gps_velocity_lag"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPS Velocity Lag"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"s"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gps_velocity_accuracy"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.200000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPS Velocity Accuracy"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m/s"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_lag"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Acceleration Lag"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"s"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_accuracy"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Acceleration Accuracy"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"(m/s^2)"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ENU_Frame_System_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ENU_Frame_System_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"LPF_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"LPF_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poles_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":6}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poles_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"poles"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"LPF Poles"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"cutoff_frequency"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"LPF Cutoff Frequency"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"MaxSonar_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":15}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"MaxSonar_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"direction_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"direction"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Direction"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":10.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Maximum Distance"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"min_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.200000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Minimum Distance"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Motor_Mixer_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Motor_Mixer_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"armed_thrust"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Armed Thrust"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"N"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Quad_Multirotor_Motor_Mixer_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Quad_Multirotor_Motor_Mixer_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"muf_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"armed_min_throttle"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.100000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Armed Min Throttle"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Acceleration_Calibration_Point"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"temperature"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Temperature"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"°C"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vec3f"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bias"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bias"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vec3f"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"scale"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Scale"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Angular_Velocity_Calibration_Point"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"temperature"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Temperature"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"°C"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vec3f"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bias"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bias"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Magnetic_Field_Calibration_Point"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"temperature"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Temperature"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"°C"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vec3f"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bias"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bias"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vec3f"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"scale"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Scale"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"MPU9250_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":127}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"hex"}]}]}]},{"type":15,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"imu_rate_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"_250"},{"name":"integral_value","type":2,"value":250}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"250 Hz"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_500"},{"name":"integral_value","type":2,"value":500}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"500 Hz"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_1000"},{"name":"integral_value","type":2,"value":1000}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"1000 Hz"}]}]}]}]}]}]},{"type":15,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_range_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"_2"},{"name":"integral_value","type":2,"value":2}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"2 G"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_4"},{"name":"integral_value","type":2,"value":4}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"4 G"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_8"},{"name":"integral_value","type":2,"value":8}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"8 G"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_16"},{"name":"integral_value","type":2,"value":16}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"16 G"}]}]}]}]}]}]},{"type":15,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity_range_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"_250"},{"name":"integral_value","type":2,"value":250}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"250 DPS"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_500"},{"name":"integral_value","type":2,"value":500}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"500 DPS"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_1000"},{"name":"integral_value","type":2,"value":1000}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"1000 DPS"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_2000"},{"name":"integral_value","type":2,"value":2000}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"2000 DPS"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":105},{"name":"hex","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"I2C Address"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"imu_rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"imu_rate"}]},{"type":20,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"imu_rate_t::_1000"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"IMU Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"magnetometer_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Magnetometer Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"thermometer_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Thermometer Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_range_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_range"}]},{"type":20,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_range_t::_8"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Acceleration Range"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity_range_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity_range"}]},{"type":20,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity_range_t::_500"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Angular Velocity Range"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"MPU9250_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Calibration"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Acceleration_Calibration_Point"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Acceleration"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Angular_Velocity_Calibration_Point"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Angular Velocity"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Magnetic_Field_Calibration_Point"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"magnetic_field"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Magnetic Field"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"euler3_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rotation"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rotation"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"°"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Calibration"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"calibration"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Calibration"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"MS5611_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":127}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"hex"}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":119},{"name":"hex","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"I2C Address"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"pressure_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Pressure Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"temperature_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Temperature Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"MS5611_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"UltimateSensorFusion_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_divisor_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":127}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"hex"}]}]}]},{"type":15,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"imu_rate_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"_250"},{"name":"integral_value","type":2,"value":250}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"250 Hz"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_400"},{"name":"integral_value","type":2,"value":400}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"400 Hz"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_500"},{"name":"integral_value","type":2,"value":500}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"500 Hz"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_1000"},{"name":"integral_value","type":2,"value":1000}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"1000 Hz"}]}]}]}]}]}]},{"type":15,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_range_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"_2"},{"name":"integral_value","type":2,"value":2}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"2 G"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_4"},{"name":"integral_value","type":2,"value":4}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"4 G"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_8"},{"name":"integral_value","type":2,"value":8}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"8 G"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_16"},{"name":"integral_value","type":2,"value":16}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"16 G"}]}]}]}]}]}]},{"type":15,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity_range_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"_250"},{"name":"integral_value","type":2,"value":250}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"250 DPS"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_500"},{"name":"integral_value","type":2,"value":500}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"500 DPS"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_1000"},{"name":"integral_value","type":2,"value":1000}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"1000 DPS"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_2000"},{"name":"integral_value","type":2,"value":2000}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"2000 DPS"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":40},{"name":"hex","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"I2C Address"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"imu_rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"imu_rate"}]},{"type":20,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"imu_rate_t::_1000"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"IMU Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"magnetometer_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Magnetometer Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"barometer_thermometer_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Barometer/Thermometer Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_range_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_range"}]},{"type":20,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_range_t::_8"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Acceleration Range"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity_range_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity_range"}]},{"type":20,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity_range_t::_500"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Angular Velocity Range"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_divisor_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"frame_rate_divisor"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Frame Processing Rate Divisor"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_divisor_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"imu_output_rate_divisor"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"IMU Output Rate Divisor"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_output_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Acceleration Output Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity_output_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Angular Velocity Output Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"magnetic_field_output_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Magnetic Field Output Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"frame_output_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Frame Output Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"pressure_output_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Pressure Output Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"temperature_output_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Temperature Output Enabled"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"UltimateSensorFusion_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"euler3_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rotation"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rotation"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"°"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Brain_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"commands_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Commands Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"state_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"State Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Brain_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Horizontal"}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_deg_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":10.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":3000.000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_deg_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_rate_deg"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":180.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"°/s"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Combined_Angle_PIDs"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Combined"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Separate_Angle_PIDs"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Separate"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"x_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"X"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"y_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Y"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Combined_Angle_PIDs"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Separate_Angle_PIDs"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"angle_pids"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Angle PIDs"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max_angle_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vec2f"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":-90.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":-90.000000}]}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":90.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":90.000000}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max_angle_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_angle_deg"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":60.000000}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":60.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max Angle"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"°"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":10.000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_speed"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max Speed"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m/s"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"LPF_Config"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"position_lpf"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Position LPF"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PI_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"velocity_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Velocity PID"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"P_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"position_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Position PID"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Yaw"}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_deg_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":10.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":3000.000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_deg_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_rate_deg"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":180.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"°/s"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"angle_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Angle PID"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Vertical"}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":10.000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_speed"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max Speed"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m/s"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"LPF_Config"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"altitude_lpf"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Altitude LPF"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PI_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"speed_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Speed PID"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"P_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"altitude_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Altitude PID"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_allowed_position_variation"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":10.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max Allowed Position Variation"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_thrust"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max Thrust"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"N"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"min_thrust"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Min Thrust"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"N"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Vertical"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"vertical"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Vertical"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Horizontal"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"horizontal"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Horizontal"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Yaw"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"yaw"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Yaw"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Pilot_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"commands_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Commands Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"state_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"State Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"video_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Video Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Pilot_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Simulator_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"throttle_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Throttle Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"state_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"State Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Acceleration Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Angular Velocity Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"magnetic_field_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Magnetic Field Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"pressure_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Pressure Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"temperature_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Temperature Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"distance_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":12}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Distance Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gps_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":5}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPS Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Multirotor_Simulator_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Noise"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gps_position"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPS Position"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gps_velocity"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.200000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPS Velocity"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gps_pacc"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.500000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPS PACC"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gps_vacc"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.100000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPS VACC"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.010000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Acceleration"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.010000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Angular Velocity"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"magnetic_field"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.100000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Magnetic Field"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"pressure"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.100000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Pressure"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"temperature"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.100000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Temperature"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"ground_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.010000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Ground Distance"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"simulation_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Simulation Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"drag_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Drag Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"ground_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Ground Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gravity_enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Gravity Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Noise"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"noise"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Noise"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Calibration"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Acceleration_Calibration_Point"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"acceleration"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Acceleration"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Angular_Velocity_Calibration_Point"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"angular_velocity"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Angular Velocity"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Magnetic_Field_Calibration_Point"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"magnetic_field"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Magnetic Field"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Calibration"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"calibration"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Calibration"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Oscillator_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"component_count_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":32}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"component_count_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"component_count"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Components"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Oscillator_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Component"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"frequency"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Frequency"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"amplitude"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Amplitude"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"square"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Square"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"amplitude"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Amplitude"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"noise"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Noise"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Component"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"components"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Components"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PCA9685_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":8000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":127}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint8_t"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"hex"}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"i2c_address_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"address"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":64},{"name":"hex","type":1,"value":true}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Address"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"servo_signal"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Servo Signal"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"channels"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Channels"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PCA9685_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Servo_Channel"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"servo_signal_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Min"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ms"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"servo_signal_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ms"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PWM_Channel"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"pwm_signal_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Min"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"pwm_signal_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"channels"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Channels"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PIGPIO_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":8000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"enabled"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Enabled"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"servo_signal"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Servo Signal"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":400}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_2"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 2"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_3"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 3"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_4"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 4"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_5"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 5"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_6"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 6"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_7"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 7"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_8"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 8"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_9"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 9"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_10"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 10"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_11"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 11"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_12"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 12"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_13"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 13"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_14"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 14"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_15"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 15"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_16"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 16"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_17"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 17"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_18"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 18"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_19"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 19"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_20"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 20"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_21"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 21"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_22"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 22"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_23"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 23"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_24"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 24"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_25"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 25"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_26"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 26"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_27"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 27"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PIGPIO_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Servo_Channel"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"servo_signal_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Min"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ms"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"servo_signal_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":2.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ms"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PWM_Channel"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"pwm_signal_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Min"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"pwm_signal_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_2"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 2"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_3"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 3"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_4"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 4"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_5"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 5"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_6"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 6"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_7"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 7"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_8"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 8"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_9"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 9"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_10"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 10"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_11"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 11"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_12"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 12"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_13"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 13"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_14"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 14"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_15"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 15"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_16"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 16"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_17"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 17"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_18"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 18"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_19"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 19"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_20"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 20"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_21"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 21"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_22"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 22"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_23"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 23"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_24"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 24"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_25"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 25"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_26"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 26"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"poly"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"IChannel"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_27"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"GPIO 27"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Pressure_Velocity_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Pressure_Velocity_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Proximity_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"channel_count_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":128}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"channel_count_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"channel_count"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Channel Count"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Proximity_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Rate_Controller_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Rate_Controller_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Feedback"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"muf_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"weight"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Weight"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Combined_XY_PIDs"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Combined"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Separate_XY_PIDs"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Separate"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"x_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"X"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"y_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Y"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Combined_XY_PIDs"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Separate_XY_PIDs"}]}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"xy_pids"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"XY PIDs"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"PID_Controller_Descriptor"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"z_pid"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Z PID"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Feedforward"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"muf_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"weight"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Weight"}]}]}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"torque_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.010000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"torque_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_torque"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max Torque"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"N"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Feedback"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"feedback"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Feedback"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Feedforward"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"feedforward"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Feedforward"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Raspicam_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Quality"}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"resolution_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vec2i"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":8}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":8}]}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":19,"children":[{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1980}]}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1080}]}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bitrate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":32000000}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"resolution_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"resolution"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Resolution"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bitrate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bitrate"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bitrate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"bps"}]}]}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"fps_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":120}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"fps_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"fps"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"FPS"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"iframe_interval_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"iframe_interval_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"iframe_interval"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":30}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"IFrame Interval"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"commands_rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":30}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"commands_rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"commands_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Commands Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Quality"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"streaming_low"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Streaming Low"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Quality"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"streaming_high"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Streaming High"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Quality"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"recording"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Recording"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Raspicam_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"iso_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1600}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"shutter_speed_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1000.000000}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ev_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":-10}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"sharpness_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"contrast_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"brightness_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"saturation_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":100}]}]}]}]},{"type":15,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"awb_mode_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"OFF"},{"name":"integral_value","type":2,"value":0}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Off"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"AUTO"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Auto"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"SUNLIGHT"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Sunlight"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"CLOUDY"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Cloudy"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"SHADE"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Shade"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"TUNGSTEN"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Tungsten"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"FLUORESCENT"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Flourescent"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"INCANDESCENT"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Incandescent"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"FLASH"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Flash"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"HORIZON"}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Horizon"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"iso_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"iso"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Iso"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"shutter_speed_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"shutter_speed"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Shutter Speed"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ms"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ev_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"ev"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":0}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Exposure Compensation"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"sharpness_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"sharpness"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":50}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Sharpness"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"contrast_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"contrast"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":50}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Contrast"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"brightness_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"brightness"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":50}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Brightness"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"saturation_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"saturation"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":50}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Saturation"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"awb_mode_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"awb_mode"}]},{"type":20,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"awb_mode_t::AUTO"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"AWB Mode"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"hflip"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Horizontal Flip"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"vflip"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Vertical Flip"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"RC5T619_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":200}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"adc0_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":50}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ADC0 Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"adc1_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":5}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ADC1 Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"RC5T619_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Resampler_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"input_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Input Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"output_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Output Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Resampler_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"LPF_Config"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"lpf"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"LPF"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Scalar_Generator_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Scalar_Generator_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"float"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"value"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Value"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Servo_Gimbal_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"commands_rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Commands Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Servo_Gimbal_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"euler_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"min_angle"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":-180.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Min Angle"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"°"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"euler_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_angle"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":180.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max Angle"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"°"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"pwm_signal_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"min_pwm"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max PWM"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"pwm_signal_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_pwm"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max PWM"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"x_channel"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"X Channel"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"y_channel"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Y Channel"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Channel"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"z_channel"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Z Channel"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"SRF01_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"SRF01_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"direction_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"direction"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Direction"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":6.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Maximum Distance"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"min_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.200000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Minimum Distance"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"SRF02_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"SRF02_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"direction_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"direction"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Direction"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":6.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Maximum Distance"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"min_distance"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.200000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Minimum Distance"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"m"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Throttle_To_PWM_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"channel_count_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"int32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":128}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"channel_count_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"channel_count"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Channel Count"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Throttle_To_PWM_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Transformer_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Transformer_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"UBLOX_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"string"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"bus"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Bus"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"UBLOX_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Vec3_Generator_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"Vec3_Generator_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"vec3f"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"value"}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Value"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"CPPM_Receiver_Descriptor"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Descriptor"}]}]}]},{"type":14,"children":[{"type":15,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_t"}]},{"type":16,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"_2"},{"name":"integral_value","type":2,"value":2}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"2"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_3"},{"name":"integral_value","type":2,"value":3}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"3"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_4"},{"name":"integral_value","type":2,"value":4}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"4"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_5"},{"name":"integral_value","type":2,"value":5}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"5"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_6"},{"name":"integral_value","type":2,"value":6}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"6"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_7"},{"name":"integral_value","type":2,"value":7}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"7"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_8"},{"name":"integral_value","type":2,"value":8}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"8"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_9"},{"name":"integral_value","type":2,"value":9}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"9"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_10"},{"name":"integral_value","type":2,"value":10}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"10"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_11"},{"name":"integral_value","type":2,"value":11}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"11"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_12"},{"name":"integral_value","type":2,"value":12}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"12"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_13"},{"name":"integral_value","type":2,"value":13}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"13"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_14"},{"name":"integral_value","type":2,"value":14}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"14"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_15"},{"name":"integral_value","type":2,"value":15}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"15"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_16"},{"name":"integral_value","type":2,"value":16}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"16"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_17"},{"name":"integral_value","type":2,"value":17}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"17"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_18"},{"name":"integral_value","type":2,"value":18}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"18"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_19"},{"name":"integral_value","type":2,"value":19}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"19"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_20"},{"name":"integral_value","type":2,"value":20}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"20"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_21"},{"name":"integral_value","type":2,"value":21}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"21"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_22"},{"name":"integral_value","type":2,"value":22}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"22"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_23"},{"name":"integral_value","type":2,"value":23}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"23"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_24"},{"name":"integral_value","type":2,"value":24}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"24"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_25"},{"name":"integral_value","type":2,"value":25}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"25"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_26"},{"name":"integral_value","type":2,"value":26}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"26"}]}]}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"_27"},{"name":"integral_value","type":2,"value":27}],"children":[{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"27"}]}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio"}]},{"type":20,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"gpio_t::_26"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"CPPM GPIO"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"channel_count_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":16}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"channel_count_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"channel_count"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":8}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Channels"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":1}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":10000}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"uint32_t"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"rate_t"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":2,"value":50}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Rate"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Hz"}]}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"CPPM_Receiver_Config"}]},{"type":7,"attributes":[{"name":"visibility","type":0,"value":"public"}],"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"INode_Config"}]}]}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"bool"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"inverted"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":1,"value":false}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Inverted Signal"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"frame_length"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":27.000000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Frame Length"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ms"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"gap_pulse_length"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.400000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Gap Pulse Length"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ms"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"min_pulse_length"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":0.600000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Min Pulse Length"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ms"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ufloat"}]}]},{"type":6,"attributes":[{"name":"value","type":0,"value":"max_pulse_length"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":3,"value":1.600000}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_name"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"Max Pulse Length"}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"name":"value","type":0,"value":"ui_suffix"}]},{"type":20,"children":[{"type":4,"attributes":[{"name":"value","type":0,"value":"ms"}]}]}]}]}]}]}]})xxx";
  return s_json;
}

      void IUAV_Descriptor::Comms::Quality::set_mtu(mtu_t const& value)
      {
        m_mtu = clamp(value, mtu_t(128), mtu_t(1360));
      }
      void IUAV_Descriptor::Comms::Quality::set_mtu(mtu_t&& value)
      {
        m_mtu = clamp(std::move(value), mtu_t(128), mtu_t(1360));
      }
      auto IUAV_Descriptor::Comms::Quality::get_mtu() const -> mtu_t const& 
      {
        return m_mtu;
      }

////////////////////////////////////////////////////////////
      void IUAV_Descriptor::Comms::Quality::set_rate(rate_t const& value)
      {
        m_rate = value;
      }
      void IUAV_Descriptor::Comms::Quality::set_rate(rate_t&& value)
      {
        m_rate = std::move(value);
      }
      auto IUAV_Descriptor::Comms::Quality::get_rate() const -> rate_t const& 
      {
        return m_rate;
      }

////////////////////////////////////////////////////////////
      void IUAV_Descriptor::Comms::Quality::set_fec_k(fec_k_t const& value)
      {
        m_fec_k = clamp(value, fec_k_t(1), fec_k_t(16));
      }
      void IUAV_Descriptor::Comms::Quality::set_fec_k(fec_k_t&& value)
      {
        m_fec_k = clamp(std::move(value), fec_k_t(1), fec_k_t(16));
      }
      auto IUAV_Descriptor::Comms::Quality::get_fec_k() const -> fec_k_t const& 
      {
        return m_fec_k;
      }

////////////////////////////////////////////////////////////
      void IUAV_Descriptor::Comms::Quality::set_fec_n(fec_n_t const& value)
      {
        m_fec_n = clamp(value, fec_n_t(1), fec_n_t(32));
      }
      void IUAV_Descriptor::Comms::Quality::set_fec_n(fec_n_t&& value)
      {
        m_fec_n = clamp(std::move(value), fec_n_t(1), fec_n_t(32));
      }
      auto IUAV_Descriptor::Comms::Quality::get_fec_n() const -> fec_n_t const& 
      {
        return m_fec_n;
      }

////////////////////////////////////////////////////////////
    void IUAV_Descriptor::Comms::set_channel(channel_t const& value)
    {
      m_channel = clamp(value, channel_t(1), channel_t(11));
    }
    void IUAV_Descriptor::Comms::set_channel(channel_t&& value)
    {
      m_channel = clamp(std::move(value), channel_t(1), channel_t(11));
    }
    auto IUAV_Descriptor::Comms::get_channel() const -> channel_t const& 
    {
      return m_channel;
    }

////////////////////////////////////////////////////////////
    void IUAV_Descriptor::Comms::set_tx_power(tx_power_t const& value)
    {
      m_tx_power = clamp(value, tx_power_t(0), tx_power_t(20.500000f));
    }
    void IUAV_Descriptor::Comms::set_tx_power(tx_power_t&& value)
    {
      m_tx_power = clamp(std::move(value), tx_power_t(0), tx_power_t(20.500000f));
    }
    auto IUAV_Descriptor::Comms::get_tx_power() const -> tx_power_t const& 
    {
      return m_tx_power;
    }

////////////////////////////////////////////////////////////
    void IUAV_Descriptor::Comms::set_low(Quality const& value)
    {
      m_low = value;
    }
    void IUAV_Descriptor::Comms::set_low(Quality&& value)
    {
      m_low = std::move(value);
    }
    auto IUAV_Descriptor::Comms::get_low() const -> Quality const& 
    {
      return m_low;
    }

    auto IUAV_Descriptor::Comms::get_low() -> Quality& 
    {
      return m_low;
    }

////////////////////////////////////////////////////////////
    void IUAV_Descriptor::Comms::set_high(Quality const& value)
    {
      m_high = value;
    }
    void IUAV_Descriptor::Comms::set_high(Quality&& value)
    {
      m_high = std::move(value);
    }
    auto IUAV_Descriptor::Comms::get_high() const -> Quality const& 
    {
      return m_high;
    }

    auto IUAV_Descriptor::Comms::get_high() -> Quality& 
    {
      return m_high;
    }

////////////////////////////////////////////////////////////
  void IUAV_Descriptor::set_name(std::string const& value)
  {
    m_name = value;
  }
  void IUAV_Descriptor::set_name(std::string&& value)
  {
    m_name = std::move(value);
  }
  auto IUAV_Descriptor::get_name() const -> std::string const& 
  {
    return m_name;
  }

////////////////////////////////////////////////////////////
  void IUAV_Descriptor::set_comms(Comms const& value)
  {
    m_comms = value;
  }
  void IUAV_Descriptor::set_comms(Comms&& value)
  {
    m_comms = std::move(value);
  }
  auto IUAV_Descriptor::get_comms() const -> Comms const& 
  {
    return m_comms;
  }

  auto IUAV_Descriptor::get_comms() -> Comms& 
  {
    return m_comms;
  }

////////////////////////////////////////////////////////////
  void IUAV_Descriptor::set_mass(mass_t const& value)
  {
    m_mass = clamp(value, mass_t(0.010000f), mass_t(100.000000f));
  }
  void IUAV_Descriptor::set_mass(mass_t&& value)
  {
    m_mass = clamp(std::move(value), mass_t(0.010000f), mass_t(100.000000f));
  }
  auto IUAV_Descriptor::get_mass() const -> mass_t const& 
  {
    return m_mass;
  }

////////////////////////////////////////////////////////////
  void IUAV_Descriptor::set_moment_of_inertia(ufloat const& value)
  {
    m_moment_of_inertia = max(value, ufloat(0));
  }
  void IUAV_Descriptor::set_moment_of_inertia(ufloat&& value)
  {
    m_moment_of_inertia = max(std::move(value), ufloat(0));
  }
  auto IUAV_Descriptor::get_moment_of_inertia() const -> ufloat const& 
  {
    return m_moment_of_inertia;
  }

////////////////////////////////////////////////////////////
    void Settings::Bus_Data::set_name(std::string const& value)
    {
      m_name = value;
    }
    void Settings::Bus_Data::set_name(std::string&& value)
    {
      m_name = std::move(value);
    }
    auto Settings::Bus_Data::get_name() const -> std::string const& 
    {
      return m_name;
    }

////////////////////////////////////////////////////////////
    void Settings::Bus_Data::set_type(std::string const& value)
    {
      m_type = value;
    }
    void Settings::Bus_Data::set_type(std::string&& value)
    {
      m_type = std::move(value);
    }
    auto Settings::Bus_Data::get_type() const -> std::string const& 
    {
      return m_type;
    }

////////////////////////////////////////////////////////////
    void Settings::Bus_Data::set_descriptor(Poly<const IBus_Descriptor> const& value)
    {
      m_descriptor = value;
    }
    void Settings::Bus_Data::set_descriptor(Poly<const IBus_Descriptor>&& value)
    {
      m_descriptor = std::move(value);
    }
    auto Settings::Bus_Data::get_descriptor() const -> Poly<const IBus_Descriptor> const& 
    {
      return m_descriptor;
    }

    auto Settings::Bus_Data::get_descriptor() -> Poly<const IBus_Descriptor>& 
    {
      return m_descriptor;
    }

////////////////////////////////////////////////////////////
    void Settings::Node_Data::set_name(std::string const& value)
    {
      m_name = value;
    }
    void Settings::Node_Data::set_name(std::string&& value)
    {
      m_name = std::move(value);
    }
    auto Settings::Node_Data::get_name() const -> std::string const& 
    {
      return m_name;
    }

////////////////////////////////////////////////////////////
    void Settings::Node_Data::set_type(std::string const& value)
    {
      m_type = value;
    }
    void Settings::Node_Data::set_type(std::string&& value)
    {
      m_type = std::move(value);
    }
    auto Settings::Node_Data::get_type() const -> std::string const& 
    {
      return m_type;
    }

////////////////////////////////////////////////////////////
    void Settings::Node_Data::set_descriptor(Poly<const INode_Descriptor> const& value)
    {
      m_descriptor = value;
    }
    void Settings::Node_Data::set_descriptor(Poly<const INode_Descriptor>&& value)
    {
      m_descriptor = std::move(value);
    }
    auto Settings::Node_Data::get_descriptor() const -> Poly<const INode_Descriptor> const& 
    {
      return m_descriptor;
    }

    auto Settings::Node_Data::get_descriptor() -> Poly<const INode_Descriptor>& 
    {
      return m_descriptor;
    }

////////////////////////////////////////////////////////////
    void Settings::Node_Data::set_config(Poly<const INode_Config> const& value)
    {
      m_config = value;
    }
    void Settings::Node_Data::set_config(Poly<const INode_Config>&& value)
    {
      m_config = std::move(value);
    }
    auto Settings::Node_Data::get_config() const -> Poly<const INode_Config> const& 
    {
      return m_config;
    }

    auto Settings::Node_Data::get_config() -> Poly<const INode_Config>& 
    {
      return m_config;
    }

////////////////////////////////////////////////////////////
    void Settings::Node_Data::set_input_paths(std::vector<std::string> const& value)
    {
      m_input_paths = value;
    }
    void Settings::Node_Data::set_input_paths(std::vector<std::string>&& value)
    {
      m_input_paths = std::move(value);
    }
    auto Settings::Node_Data::get_input_paths() const -> std::vector<std::string> const& 
    {
      return m_input_paths;
    }

    auto Settings::Node_Data::get_input_paths() -> std::vector<std::string>& 
    {
      return m_input_paths;
    }

////////////////////////////////////////////////////////////
  void Settings::set_uav_descriptor(Poly<const IUAV_Descriptor> const& value)
  {
    m_uav_descriptor = value;
  }
  void Settings::set_uav_descriptor(Poly<const IUAV_Descriptor>&& value)
  {
    m_uav_descriptor = std::move(value);
  }
  auto Settings::get_uav_descriptor() const -> Poly<const IUAV_Descriptor> const& 
  {
    return m_uav_descriptor;
  }

  auto Settings::get_uav_descriptor() -> Poly<const IUAV_Descriptor>& 
  {
    return m_uav_descriptor;
  }

////////////////////////////////////////////////////////////
  void Settings::set_buses(std::vector<Settings::Bus_Data> const& value)
  {
    m_buses = value;
  }
  void Settings::set_buses(std::vector<Settings::Bus_Data>&& value)
  {
    m_buses = std::move(value);
  }
  auto Settings::get_buses() const -> std::vector<Settings::Bus_Data> const& 
  {
    return m_buses;
  }

  auto Settings::get_buses() -> std::vector<Settings::Bus_Data>& 
  {
    return m_buses;
  }

////////////////////////////////////////////////////////////
  void Settings::set_nodes(std::vector<Settings::Node_Data> const& value)
  {
    m_nodes = value;
  }
  void Settings::set_nodes(std::vector<Settings::Node_Data>&& value)
  {
    m_nodes = std::move(value);
  }
  auto Settings::get_nodes() const -> std::vector<Settings::Node_Data> const& 
  {
    return m_nodes;
  }

  auto Settings::get_nodes() -> std::vector<Settings::Node_Data>& 
  {
    return m_nodes;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Descriptor::set_radius(radius_t const& value)
  {
    m_radius = clamp(value, radius_t(0.010000f), radius_t(100.000000f));
  }
  void Multirotor_Descriptor::set_radius(radius_t&& value)
  {
    m_radius = clamp(std::move(value), radius_t(0.010000f), radius_t(100.000000f));
  }
  auto Multirotor_Descriptor::get_radius() const -> radius_t const& 
  {
    return m_radius;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Descriptor::set_height(height_t const& value)
  {
    m_height = clamp(value, height_t(0.010000f), height_t(100.000000f));
  }
  void Multirotor_Descriptor::set_height(height_t&& value)
  {
    m_height = clamp(std::move(value), height_t(0.010000f), height_t(100.000000f));
  }
  auto Multirotor_Descriptor::get_height() const -> height_t const& 
  {
    return m_height;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Descriptor::set_motor_z_torque(torque_t const& value)
  {
    m_motor_z_torque = clamp(value, torque_t(0.010000f), torque_t(1000.000000f));
  }
  void Multirotor_Descriptor::set_motor_z_torque(torque_t&& value)
  {
    m_motor_z_torque = clamp(std::move(value), torque_t(0.010000f), torque_t(1000.000000f));
  }
  auto Multirotor_Descriptor::get_motor_z_torque() const -> torque_t const& 
  {
    return m_motor_z_torque;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Descriptor::set_motor_thrust(thrust_t const& value)
  {
    m_motor_thrust = clamp(value, thrust_t(0.010000f), thrust_t(1000.000000f));
  }
  void Multirotor_Descriptor::set_motor_thrust(thrust_t&& value)
  {
    m_motor_thrust = clamp(std::move(value), thrust_t(0.010000f), thrust_t(1000.000000f));
  }
  auto Multirotor_Descriptor::get_motor_thrust() const -> thrust_t const& 
  {
    return m_motor_thrust;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Descriptor::set_motor_acceleration(acceleration_t const& value)
  {
    m_motor_acceleration = max(value, acceleration_t(0.100000f));
  }
  void Multirotor_Descriptor::set_motor_acceleration(acceleration_t&& value)
  {
    m_motor_acceleration = max(std::move(value), acceleration_t(0.100000f));
  }
  auto Multirotor_Descriptor::get_motor_acceleration() const -> acceleration_t const& 
  {
    return m_motor_acceleration;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Descriptor::set_motor_deceleration(acceleration_t const& value)
  {
    m_motor_deceleration = max(value, acceleration_t(0.100000f));
  }
  void Multirotor_Descriptor::set_motor_deceleration(acceleration_t&& value)
  {
    m_motor_deceleration = max(std::move(value), acceleration_t(0.100000f));
  }
  auto Multirotor_Descriptor::get_motor_deceleration() const -> acceleration_t const& 
  {
    return m_motor_deceleration;
  }

////////////////////////////////////////////////////////////
  void Quad_Multirotor_Descriptor::set_plus_configuration(bool const& value)
  {
    m_plus_configuration = value;
  }
  void Quad_Multirotor_Descriptor::set_plus_configuration(bool&& value)
  {
    m_plus_configuration = std::move(value);
  }
  auto Quad_Multirotor_Descriptor::get_plus_configuration() const -> bool const& 
  {
    return m_plus_configuration;
  }

////////////////////////////////////////////////////////////
  void Hexa_Multirotor_Descriptor::set_plus_configuration(bool const& value)
  {
    m_plus_configuration = value;
  }
  void Hexa_Multirotor_Descriptor::set_plus_configuration(bool&& value)
  {
    m_plus_configuration = std::move(value);
  }
  auto Hexa_Multirotor_Descriptor::get_plus_configuration() const -> bool const& 
  {
    return m_plus_configuration;
  }

////////////////////////////////////////////////////////////
  void Octo_Multirotor_Descriptor::set_plus_configuration(bool const& value)
  {
    m_plus_configuration = value;
  }
  void Octo_Multirotor_Descriptor::set_plus_configuration(bool&& value)
  {
    m_plus_configuration = std::move(value);
  }
  auto Octo_Multirotor_Descriptor::get_plus_configuration() const -> bool const& 
  {
    return m_plus_configuration;
  }

////////////////////////////////////////////////////////////
  void Octaquad_Multirotor_Descriptor::set_plus_configuration(bool const& value)
  {
    m_plus_configuration = value;
  }
  void Octaquad_Multirotor_Descriptor::set_plus_configuration(bool&& value)
  {
    m_plus_configuration = std::move(value);
  }
  auto Octaquad_Multirotor_Descriptor::get_plus_configuration() const -> bool const& 
  {
    return m_plus_configuration;
  }

////////////////////////////////////////////////////////////
    void Custom_Multirotor_Descriptor::Motor::set_position(vec3f const& value)
    {
      m_position = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
    }
    void Custom_Multirotor_Descriptor::Motor::set_position(vec3f&& value)
    {
      m_position = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
    }
    auto Custom_Multirotor_Descriptor::Motor::get_position() const -> vec3f const& 
    {
      return m_position;
    }

////////////////////////////////////////////////////////////
    void Custom_Multirotor_Descriptor::Motor::set_thrust_vector(direction_t const& value)
    {
      m_thrust_vector = clamp(value, direction_t(-FLT_MAX, -FLT_MAX, -FLT_MAX), direction_t(FLT_MAX, FLT_MAX, FLT_MAX));
    }
    void Custom_Multirotor_Descriptor::Motor::set_thrust_vector(direction_t&& value)
    {
      m_thrust_vector = clamp(std::move(value), direction_t(-FLT_MAX, -FLT_MAX, -FLT_MAX), direction_t(FLT_MAX, FLT_MAX, FLT_MAX));
    }
    auto Custom_Multirotor_Descriptor::Motor::get_thrust_vector() const -> direction_t const& 
    {
      return m_thrust_vector;
    }

////////////////////////////////////////////////////////////
    void Custom_Multirotor_Descriptor::Motor::set_clockwise(bool const& value)
    {
      m_clockwise = value;
    }
    void Custom_Multirotor_Descriptor::Motor::set_clockwise(bool&& value)
    {
      m_clockwise = std::move(value);
    }
    auto Custom_Multirotor_Descriptor::Motor::get_clockwise() const -> bool const& 
    {
      return m_clockwise;
    }

////////////////////////////////////////////////////////////
  void Custom_Multirotor_Descriptor::set_motors(std::vector<Custom_Multirotor_Descriptor::Motor> const& value)
  {
    m_motors = value;
  }
  void Custom_Multirotor_Descriptor::set_motors(std::vector<Custom_Multirotor_Descriptor::Motor>&& value)
  {
    m_motors = std::move(value);
  }
  auto Custom_Multirotor_Descriptor::get_motors() const -> std::vector<Custom_Multirotor_Descriptor::Motor> const& 
  {
    return m_motors;
  }

  auto Custom_Multirotor_Descriptor::get_motors() -> std::vector<Custom_Multirotor_Descriptor::Motor>& 
  {
    return m_motors;
  }

////////////////////////////////////////////////////////////
  void UART_Linux_Descriptor::set_dev(std::string const& value)
  {
    m_dev = value;
  }
  void UART_Linux_Descriptor::set_dev(std::string&& value)
  {
    m_dev = std::move(value);
  }
  auto UART_Linux_Descriptor::get_dev() const -> std::string const& 
  {
    return m_dev;
  }

////////////////////////////////////////////////////////////
  void UART_Linux_Descriptor::set_baud(baud_t const& value)
  {
    m_baud = value;
  }
  void UART_Linux_Descriptor::set_baud(baud_t&& value)
  {
    m_baud = std::move(value);
  }
  auto UART_Linux_Descriptor::get_baud() const -> baud_t const& 
  {
    return m_baud;
  }

////////////////////////////////////////////////////////////
  void UART_BB_Descriptor::set_rx_pin(uint32_t const& value)
  {
    m_rx_pin = value;
  }
  void UART_BB_Descriptor::set_rx_pin(uint32_t&& value)
  {
    m_rx_pin = std::move(value);
  }
  auto UART_BB_Descriptor::get_rx_pin() const -> uint32_t const& 
  {
    return m_rx_pin;
  }

////////////////////////////////////////////////////////////
  void UART_BB_Descriptor::set_baud(uint32_t const& value)
  {
    m_baud = value;
  }
  void UART_BB_Descriptor::set_baud(uint32_t&& value)
  {
    m_baud = std::move(value);
  }
  auto UART_BB_Descriptor::get_baud() const -> uint32_t const& 
  {
    return m_baud;
  }

////////////////////////////////////////////////////////////
  void UART_BB_Descriptor::set_invert(bool const& value)
  {
    m_invert = value;
  }
  void UART_BB_Descriptor::set_invert(bool&& value)
  {
    m_invert = std::move(value);
  }
  auto UART_BB_Descriptor::get_invert() const -> bool const& 
  {
    return m_invert;
  }

////////////////////////////////////////////////////////////
  void I2C_BCM_Descriptor::set_dev(dev_t const& value)
  {
    m_dev = clamp(value, dev_t(0), dev_t(1));
  }
  void I2C_BCM_Descriptor::set_dev(dev_t&& value)
  {
    m_dev = clamp(std::move(value), dev_t(0), dev_t(1));
  }
  auto I2C_BCM_Descriptor::get_dev() const -> dev_t const& 
  {
    return m_dev;
  }

////////////////////////////////////////////////////////////
  void I2C_BCM_Descriptor::set_baud(baud_t const& value)
  {
    m_baud = clamp(value, baud_t(10000), baud_t(1000000));
  }
  void I2C_BCM_Descriptor::set_baud(baud_t&& value)
  {
    m_baud = clamp(std::move(value), baud_t(10000), baud_t(1000000));
  }
  auto I2C_BCM_Descriptor::get_baud() const -> baud_t const& 
  {
    return m_baud;
  }

////////////////////////////////////////////////////////////
  void I2C_Linux_Descriptor::set_dev(std::string const& value)
  {
    m_dev = value;
  }
  void I2C_Linux_Descriptor::set_dev(std::string&& value)
  {
    m_dev = std::move(value);
  }
  auto I2C_Linux_Descriptor::get_dev() const -> std::string const& 
  {
    return m_dev;
  }

////////////////////////////////////////////////////////////
  void SPI_BCM_Descriptor::set_dev(dev_t const& value)
  {
    m_dev = clamp(value, dev_t(0), dev_t(1));
  }
  void SPI_BCM_Descriptor::set_dev(dev_t&& value)
  {
    m_dev = clamp(std::move(value), dev_t(0), dev_t(1));
  }
  auto SPI_BCM_Descriptor::get_dev() const -> dev_t const& 
  {
    return m_dev;
  }

////////////////////////////////////////////////////////////
  void SPI_BCM_Descriptor::set_baud(uint32_t const& value)
  {
    m_baud = value;
  }
  void SPI_BCM_Descriptor::set_baud(uint32_t&& value)
  {
    m_baud = std::move(value);
  }
  auto SPI_BCM_Descriptor::get_baud() const -> uint32_t const& 
  {
    return m_baud;
  }

////////////////////////////////////////////////////////////
  void SPI_BCM_Descriptor::set_mode(mode_t const& value)
  {
    m_mode = clamp(value, mode_t(0), mode_t(3));
  }
  void SPI_BCM_Descriptor::set_mode(mode_t&& value)
  {
    m_mode = clamp(std::move(value), mode_t(0), mode_t(3));
  }
  auto SPI_BCM_Descriptor::get_mode() const -> mode_t const& 
  {
    return m_mode;
  }

////////////////////////////////////////////////////////////
  void SPI_BCM_Descriptor::set_speed(speed_t const& value)
  {
    m_speed = clamp(value, speed_t(1000), speed_t(4294967295LL));
  }
  void SPI_BCM_Descriptor::set_speed(speed_t&& value)
  {
    m_speed = clamp(std::move(value), speed_t(1000), speed_t(4294967295LL));
  }
  auto SPI_BCM_Descriptor::get_speed() const -> speed_t const& 
  {
    return m_speed;
  }

////////////////////////////////////////////////////////////
  void SPI_Linux_Descriptor::set_dev(std::string const& value)
  {
    m_dev = value;
  }
  void SPI_Linux_Descriptor::set_dev(std::string&& value)
  {
    m_dev = std::move(value);
  }
  auto SPI_Linux_Descriptor::get_dev() const -> std::string const& 
  {
    return m_dev;
  }

////////////////////////////////////////////////////////////
  void SPI_Linux_Descriptor::set_speed(speed_t const& value)
  {
    m_speed = clamp(value, speed_t(1000), speed_t(4294967295LL));
  }
  void SPI_Linux_Descriptor::set_speed(speed_t&& value)
  {
    m_speed = clamp(std::move(value), speed_t(1000), speed_t(4294967295LL));
  }
  auto SPI_Linux_Descriptor::get_speed() const -> speed_t const& 
  {
    return m_speed;
  }

////////////////////////////////////////////////////////////
  void P_Controller_Descriptor::set_kp(ufloat const& value)
  {
    m_kp = max(value, ufloat(0));
  }
  void P_Controller_Descriptor::set_kp(ufloat&& value)
  {
    m_kp = max(std::move(value), ufloat(0));
  }
  auto P_Controller_Descriptor::get_kp() const -> ufloat const& 
  {
    return m_kp;
  }

////////////////////////////////////////////////////////////
  void PI_Controller_Descriptor::set_ki(ufloat const& value)
  {
    m_ki = max(value, ufloat(0));
  }
  void PI_Controller_Descriptor::set_ki(ufloat&& value)
  {
    m_ki = max(std::move(value), ufloat(0));
  }
  auto PI_Controller_Descriptor::get_ki() const -> ufloat const& 
  {
    return m_ki;
  }

////////////////////////////////////////////////////////////
  void PI_Controller_Descriptor::set_max_i(ufloat const& value)
  {
    m_max_i = max(value, ufloat(0));
  }
  void PI_Controller_Descriptor::set_max_i(ufloat&& value)
  {
    m_max_i = max(std::move(value), ufloat(0));
  }
  auto PI_Controller_Descriptor::get_max_i() const -> ufloat const& 
  {
    return m_max_i;
  }

////////////////////////////////////////////////////////////
  void PD_Controller_Descriptor::set_kd(ufloat const& value)
  {
    m_kd = max(value, ufloat(0));
  }
  void PD_Controller_Descriptor::set_kd(ufloat&& value)
  {
    m_kd = max(std::move(value), ufloat(0));
  }
  auto PD_Controller_Descriptor::get_kd() const -> ufloat const& 
  {
    return m_kd;
  }

////////////////////////////////////////////////////////////
  void PD_Controller_Descriptor::set_d_filter(muf_t const& value)
  {
    m_d_filter = clamp(value, muf_t(0), muf_t(1.000000f));
  }
  void PD_Controller_Descriptor::set_d_filter(muf_t&& value)
  {
    m_d_filter = clamp(std::move(value), muf_t(0), muf_t(1.000000f));
  }
  auto PD_Controller_Descriptor::get_d_filter() const -> muf_t const& 
  {
    return m_d_filter;
  }

////////////////////////////////////////////////////////////
  void PID_Controller_Descriptor::set_ki(ufloat const& value)
  {
    m_ki = max(value, ufloat(0));
  }
  void PID_Controller_Descriptor::set_ki(ufloat&& value)
  {
    m_ki = max(std::move(value), ufloat(0));
  }
  auto PID_Controller_Descriptor::get_ki() const -> ufloat const& 
  {
    return m_ki;
  }

////////////////////////////////////////////////////////////
  void PID_Controller_Descriptor::set_max_i(ufloat const& value)
  {
    m_max_i = max(value, ufloat(0));
  }
  void PID_Controller_Descriptor::set_max_i(ufloat&& value)
  {
    m_max_i = max(std::move(value), ufloat(0));
  }
  auto PID_Controller_Descriptor::get_max_i() const -> ufloat const& 
  {
    return m_max_i;
  }

////////////////////////////////////////////////////////////
  void ADC_Ammeter_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void ADC_Ammeter_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto ADC_Ammeter_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void ADC_Ammeter_Config::set_scale(float const& value)
  {
    m_scale = value;
  }
  void ADC_Ammeter_Config::set_scale(float&& value)
  {
    m_scale = std::move(value);
  }
  auto ADC_Ammeter_Config::get_scale() const -> float const& 
  {
    return m_scale;
  }

////////////////////////////////////////////////////////////
  void ADC_Ammeter_Config::set_bias(float const& value)
  {
    m_bias = value;
  }
  void ADC_Ammeter_Config::set_bias(float&& value)
  {
    m_bias = std::move(value);
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
  void ADC_Voltmeter_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto ADC_Voltmeter_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void ADC_Voltmeter_Config::set_scale(float const& value)
  {
    m_scale = value;
  }
  void ADC_Voltmeter_Config::set_scale(float&& value)
  {
    m_scale = std::move(value);
  }
  auto ADC_Voltmeter_Config::get_scale() const -> float const& 
  {
    return m_scale;
  }

////////////////////////////////////////////////////////////
  void ADC_Voltmeter_Config::set_bias(float const& value)
  {
    m_bias = value;
  }
  void ADC_Voltmeter_Config::set_bias(float&& value)
  {
    m_bias = std::move(value);
  }
  auto ADC_Voltmeter_Config::get_bias() const -> float const& 
  {
    return m_bias;
  }

////////////////////////////////////////////////////////////
    void ADS1115_Descriptor::ADC::set_is_enabled(bool const& value)
    {
      m_is_enabled = value;
    }
    void ADS1115_Descriptor::ADC::set_is_enabled(bool&& value)
    {
      m_is_enabled = std::move(value);
    }
    auto ADS1115_Descriptor::ADC::get_is_enabled() const -> bool const& 
    {
      return m_is_enabled;
    }

////////////////////////////////////////////////////////////
    void ADS1115_Descriptor::ADC::set_rate(uint32_t const& value)
    {
      m_rate = clamp(value, uint32_t(1), uint32_t(400));
    }
    void ADS1115_Descriptor::ADC::set_rate(uint32_t&& value)
    {
      m_rate = clamp(std::move(value), uint32_t(1), uint32_t(400));
    }
    auto ADS1115_Descriptor::ADC::get_rate() const -> uint32_t const& 
    {
      return m_rate;
    }

////////////////////////////////////////////////////////////
  void ADS1115_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  void ADS1115_Descriptor::set_bus(std::string&& value)
  {
    m_bus = std::move(value);
  }
  auto ADS1115_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }

////////////////////////////////////////////////////////////
  void ADS1115_Descriptor::set_i2c_address(uint8_t const& value)
  {
    m_i2c_address = min(value, uint8_t(127));
  }
  void ADS1115_Descriptor::set_i2c_address(uint8_t&& value)
  {
    m_i2c_address = min(std::move(value), uint8_t(127));
  }
  auto ADS1115_Descriptor::get_i2c_address() const -> uint8_t const& 
  {
    return m_i2c_address;
  }

////////////////////////////////////////////////////////////
  void ADS1115_Descriptor::set_adc0(ADC const& value)
  {
    m_adc0 = value;
  }
  void ADS1115_Descriptor::set_adc0(ADC&& value)
  {
    m_adc0 = std::move(value);
  }
  auto ADS1115_Descriptor::get_adc0() const -> ADC const& 
  {
    return m_adc0;
  }

  auto ADS1115_Descriptor::get_adc0() -> ADC& 
  {
    return m_adc0;
  }

////////////////////////////////////////////////////////////
  void ADS1115_Descriptor::set_adc1(ADC const& value)
  {
    m_adc1 = value;
  }
  void ADS1115_Descriptor::set_adc1(ADC&& value)
  {
    m_adc1 = std::move(value);
  }
  auto ADS1115_Descriptor::get_adc1() const -> ADC const& 
  {
    return m_adc1;
  }

  auto ADS1115_Descriptor::get_adc1() -> ADC& 
  {
    return m_adc1;
  }

////////////////////////////////////////////////////////////
  void ADS1115_Descriptor::set_adc2(ADC const& value)
  {
    m_adc2 = value;
  }
  void ADS1115_Descriptor::set_adc2(ADC&& value)
  {
    m_adc2 = std::move(value);
  }
  auto ADS1115_Descriptor::get_adc2() const -> ADC const& 
  {
    return m_adc2;
  }

  auto ADS1115_Descriptor::get_adc2() -> ADC& 
  {
    return m_adc2;
  }

////////////////////////////////////////////////////////////
  void ADS1115_Descriptor::set_adc3(ADC const& value)
  {
    m_adc3 = value;
  }
  void ADS1115_Descriptor::set_adc3(ADC&& value)
  {
    m_adc3 = std::move(value);
  }
  auto ADS1115_Descriptor::get_adc3() const -> ADC const& 
  {
    return m_adc3;
  }

  auto ADS1115_Descriptor::get_adc3() -> ADC& 
  {
    return m_adc3;
  }

////////////////////////////////////////////////////////////
  void AVRADC_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  void AVRADC_Descriptor::set_bus(std::string&& value)
  {
    m_bus = std::move(value);
  }
  auto AVRADC_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }

////////////////////////////////////////////////////////////
  void AVRADC_Descriptor::set_i2c_address(uint8_t const& value)
  {
    m_i2c_address = min(value, uint8_t(127));
  }
  void AVRADC_Descriptor::set_i2c_address(uint8_t&& value)
  {
    m_i2c_address = min(std::move(value), uint8_t(127));
  }
  auto AVRADC_Descriptor::get_i2c_address() const -> uint8_t const& 
  {
    return m_i2c_address;
  }

////////////////////////////////////////////////////////////
  void AVRADC_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(500));
  }
  void AVRADC_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(500));
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
  void Comp_AHRS_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Comp_AHRS_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void Comp_AHRS_Config::set_drift_correction_factor(muf_t const& value)
  {
    m_drift_correction_factor = clamp(value, muf_t(0), muf_t(1.000000f));
  }
  void Comp_AHRS_Config::set_drift_correction_factor(muf_t&& value)
  {
    m_drift_correction_factor = clamp(std::move(value), muf_t(0), muf_t(1.000000f));
  }
  auto Comp_AHRS_Config::get_drift_correction_factor() const -> muf_t const& 
  {
    return m_drift_correction_factor;
  }

////////////////////////////////////////////////////////////
  void Combiner_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Combiner_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Gravity_Filter_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Gravity_Filter_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void KF_ECEF_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void KF_ECEF_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto KF_ECEF_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void KF_ECEF_Config::set_gps_position_lag(ufloat const& value)
  {
    m_gps_position_lag = max(value, ufloat(0));
  }
  void KF_ECEF_Config::set_gps_position_lag(ufloat&& value)
  {
    m_gps_position_lag = max(std::move(value), ufloat(0));
  }
  auto KF_ECEF_Config::get_gps_position_lag() const -> ufloat const& 
  {
    return m_gps_position_lag;
  }

////////////////////////////////////////////////////////////
  void KF_ECEF_Config::set_gps_position_accuracy(ufloat const& value)
  {
    m_gps_position_accuracy = max(value, ufloat(0));
  }
  void KF_ECEF_Config::set_gps_position_accuracy(ufloat&& value)
  {
    m_gps_position_accuracy = max(std::move(value), ufloat(0));
  }
  auto KF_ECEF_Config::get_gps_position_accuracy() const -> ufloat const& 
  {
    return m_gps_position_accuracy;
  }

////////////////////////////////////////////////////////////
  void KF_ECEF_Config::set_gps_velocity_lag(ufloat const& value)
  {
    m_gps_velocity_lag = max(value, ufloat(0));
  }
  void KF_ECEF_Config::set_gps_velocity_lag(ufloat&& value)
  {
    m_gps_velocity_lag = max(std::move(value), ufloat(0));
  }
  auto KF_ECEF_Config::get_gps_velocity_lag() const -> ufloat const& 
  {
    return m_gps_velocity_lag;
  }

////////////////////////////////////////////////////////////
  void KF_ECEF_Config::set_gps_velocity_accuracy(ufloat const& value)
  {
    m_gps_velocity_accuracy = max(value, ufloat(0));
  }
  void KF_ECEF_Config::set_gps_velocity_accuracy(ufloat&& value)
  {
    m_gps_velocity_accuracy = max(std::move(value), ufloat(0));
  }
  auto KF_ECEF_Config::get_gps_velocity_accuracy() const -> ufloat const& 
  {
    return m_gps_velocity_accuracy;
  }

////////////////////////////////////////////////////////////
  void KF_ECEF_Config::set_acceleration_lag(ufloat const& value)
  {
    m_acceleration_lag = max(value, ufloat(0));
  }
  void KF_ECEF_Config::set_acceleration_lag(ufloat&& value)
  {
    m_acceleration_lag = max(std::move(value), ufloat(0));
  }
  auto KF_ECEF_Config::get_acceleration_lag() const -> ufloat const& 
  {
    return m_acceleration_lag;
  }

////////////////////////////////////////////////////////////
  void KF_ECEF_Config::set_acceleration_accuracy(ufloat const& value)
  {
    m_acceleration_accuracy = max(value, ufloat(0));
  }
  void KF_ECEF_Config::set_acceleration_accuracy(ufloat&& value)
  {
    m_acceleration_accuracy = max(std::move(value), ufloat(0));
  }
  auto KF_ECEF_Config::get_acceleration_accuracy() const -> ufloat const& 
  {
    return m_acceleration_accuracy;
  }

////////////////////////////////////////////////////////////
  void ENU_Frame_System_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void ENU_Frame_System_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto ENU_Frame_System_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void LPF_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void LPF_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto LPF_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void LPF_Config::set_poles(poles_t const& value)
  {
    m_poles = clamp(value, poles_t(1), poles_t(6));
  }
  void LPF_Config::set_poles(poles_t&& value)
  {
    m_poles = clamp(std::move(value), poles_t(1), poles_t(6));
  }
  auto LPF_Config::get_poles() const -> poles_t const& 
  {
    return m_poles;
  }

////////////////////////////////////////////////////////////
  void LPF_Config::set_cutoff_frequency(ufloat const& value)
  {
    m_cutoff_frequency = max(value, ufloat(0));
  }
  void LPF_Config::set_cutoff_frequency(ufloat&& value)
  {
    m_cutoff_frequency = max(std::move(value), ufloat(0));
  }
  auto LPF_Config::get_cutoff_frequency() const -> ufloat const& 
  {
    return m_cutoff_frequency;
  }

////////////////////////////////////////////////////////////
  void MaxSonar_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  void MaxSonar_Descriptor::set_bus(std::string&& value)
  {
    m_bus = std::move(value);
  }
  auto MaxSonar_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }

////////////////////////////////////////////////////////////
  void MaxSonar_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(15));
  }
  void MaxSonar_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(15));
  }
  auto MaxSonar_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void MaxSonar_Config::set_direction(direction_t const& value)
  {
    m_direction = clamp(value, direction_t(-FLT_MAX, -FLT_MAX, -FLT_MAX), direction_t(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void MaxSonar_Config::set_direction(direction_t&& value)
  {
    m_direction = clamp(std::move(value), direction_t(-FLT_MAX, -FLT_MAX, -FLT_MAX), direction_t(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto MaxSonar_Config::get_direction() const -> direction_t const& 
  {
    return m_direction;
  }

////////////////////////////////////////////////////////////
  void MaxSonar_Config::set_max_distance(ufloat const& value)
  {
    m_max_distance = max(value, ufloat(0));
  }
  void MaxSonar_Config::set_max_distance(ufloat&& value)
  {
    m_max_distance = max(std::move(value), ufloat(0));
  }
  auto MaxSonar_Config::get_max_distance() const -> ufloat const& 
  {
    return m_max_distance;
  }

////////////////////////////////////////////////////////////
  void MaxSonar_Config::set_min_distance(ufloat const& value)
  {
    m_min_distance = max(value, ufloat(0));
  }
  void MaxSonar_Config::set_min_distance(ufloat&& value)
  {
    m_min_distance = max(std::move(value), ufloat(0));
  }
  auto MaxSonar_Config::get_min_distance() const -> ufloat const& 
  {
    return m_min_distance;
  }

////////////////////////////////////////////////////////////
  void Motor_Mixer_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Motor_Mixer_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Motor_Mixer_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void Motor_Mixer_Config::set_armed_thrust(ufloat const& value)
  {
    m_armed_thrust = max(value, ufloat(0));
  }
  void Motor_Mixer_Config::set_armed_thrust(ufloat&& value)
  {
    m_armed_thrust = max(std::move(value), ufloat(0));
  }
  auto Motor_Mixer_Config::get_armed_thrust() const -> ufloat const& 
  {
    return m_armed_thrust;
  }

////////////////////////////////////////////////////////////
  void Quad_Multirotor_Motor_Mixer_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Quad_Multirotor_Motor_Mixer_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Quad_Multirotor_Motor_Mixer_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void Quad_Multirotor_Motor_Mixer_Config::set_armed_min_throttle(muf_t const& value)
  {
    m_armed_min_throttle = clamp(value, muf_t(0), muf_t(1.000000f));
  }
  void Quad_Multirotor_Motor_Mixer_Config::set_armed_min_throttle(muf_t&& value)
  {
    m_armed_min_throttle = clamp(std::move(value), muf_t(0), muf_t(1.000000f));
  }
  auto Quad_Multirotor_Motor_Mixer_Config::get_armed_min_throttle() const -> muf_t const& 
  {
    return m_armed_min_throttle;
  }

////////////////////////////////////////////////////////////
  void Acceleration_Calibration_Point::set_temperature(float const& value)
  {
    m_temperature = value;
  }
  void Acceleration_Calibration_Point::set_temperature(float&& value)
  {
    m_temperature = std::move(value);
  }
  auto Acceleration_Calibration_Point::get_temperature() const -> float const& 
  {
    return m_temperature;
  }

////////////////////////////////////////////////////////////
  void Acceleration_Calibration_Point::set_bias(vec3f const& value)
  {
    m_bias = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Acceleration_Calibration_Point::set_bias(vec3f&& value)
  {
    m_bias = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Acceleration_Calibration_Point::get_bias() const -> vec3f const& 
  {
    return m_bias;
  }

////////////////////////////////////////////////////////////
  void Acceleration_Calibration_Point::set_scale(vec3f const& value)
  {
    m_scale = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Acceleration_Calibration_Point::set_scale(vec3f&& value)
  {
    m_scale = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Acceleration_Calibration_Point::get_scale() const -> vec3f const& 
  {
    return m_scale;
  }

////////////////////////////////////////////////////////////
  void Angular_Velocity_Calibration_Point::set_temperature(float const& value)
  {
    m_temperature = value;
  }
  void Angular_Velocity_Calibration_Point::set_temperature(float&& value)
  {
    m_temperature = std::move(value);
  }
  auto Angular_Velocity_Calibration_Point::get_temperature() const -> float const& 
  {
    return m_temperature;
  }

////////////////////////////////////////////////////////////
  void Angular_Velocity_Calibration_Point::set_bias(vec3f const& value)
  {
    m_bias = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Angular_Velocity_Calibration_Point::set_bias(vec3f&& value)
  {
    m_bias = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Angular_Velocity_Calibration_Point::get_bias() const -> vec3f const& 
  {
    return m_bias;
  }

////////////////////////////////////////////////////////////
  void Magnetic_Field_Calibration_Point::set_temperature(float const& value)
  {
    m_temperature = value;
  }
  void Magnetic_Field_Calibration_Point::set_temperature(float&& value)
  {
    m_temperature = std::move(value);
  }
  auto Magnetic_Field_Calibration_Point::get_temperature() const -> float const& 
  {
    return m_temperature;
  }

////////////////////////////////////////////////////////////
  void Magnetic_Field_Calibration_Point::set_bias(vec3f const& value)
  {
    m_bias = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Magnetic_Field_Calibration_Point::set_bias(vec3f&& value)
  {
    m_bias = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Magnetic_Field_Calibration_Point::get_bias() const -> vec3f const& 
  {
    return m_bias;
  }

////////////////////////////////////////////////////////////
  void Magnetic_Field_Calibration_Point::set_scale(vec3f const& value)
  {
    m_scale = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Magnetic_Field_Calibration_Point::set_scale(vec3f&& value)
  {
    m_scale = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Magnetic_Field_Calibration_Point::get_scale() const -> vec3f const& 
  {
    return m_scale;
  }

////////////////////////////////////////////////////////////
  void MPU9250_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  void MPU9250_Descriptor::set_bus(std::string&& value)
  {
    m_bus = std::move(value);
  }
  auto MPU9250_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }

////////////////////////////////////////////////////////////
  void MPU9250_Descriptor::set_i2c_address(uint8_t const& value)
  {
    m_i2c_address = min(value, uint8_t(127));
  }
  void MPU9250_Descriptor::set_i2c_address(uint8_t&& value)
  {
    m_i2c_address = min(std::move(value), uint8_t(127));
  }
  auto MPU9250_Descriptor::get_i2c_address() const -> uint8_t const& 
  {
    return m_i2c_address;
  }

////////////////////////////////////////////////////////////
  void MPU9250_Descriptor::set_imu_rate(imu_rate_t const& value)
  {
    m_imu_rate = value;
  }
  void MPU9250_Descriptor::set_imu_rate(imu_rate_t&& value)
  {
    m_imu_rate = std::move(value);
  }
  auto MPU9250_Descriptor::get_imu_rate() const -> imu_rate_t const& 
  {
    return m_imu_rate;
  }

////////////////////////////////////////////////////////////
  void MPU9250_Descriptor::set_magnetometer_rate(uint32_t const& value)
  {
    m_magnetometer_rate = clamp(value, uint32_t(1), uint32_t(100));
  }
  void MPU9250_Descriptor::set_magnetometer_rate(uint32_t&& value)
  {
    m_magnetometer_rate = clamp(std::move(value), uint32_t(1), uint32_t(100));
  }
  auto MPU9250_Descriptor::get_magnetometer_rate() const -> uint32_t const& 
  {
    return m_magnetometer_rate;
  }

////////////////////////////////////////////////////////////
  void MPU9250_Descriptor::set_thermometer_rate(uint32_t const& value)
  {
    m_thermometer_rate = clamp(value, uint32_t(1), uint32_t(100));
  }
  void MPU9250_Descriptor::set_thermometer_rate(uint32_t&& value)
  {
    m_thermometer_rate = clamp(std::move(value), uint32_t(1), uint32_t(100));
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
  void MPU9250_Descriptor::set_acceleration_range(acceleration_range_t&& value)
  {
    m_acceleration_range = std::move(value);
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
  void MPU9250_Descriptor::set_angular_velocity_range(angular_velocity_range_t&& value)
  {
    m_angular_velocity_range = std::move(value);
  }
  auto MPU9250_Descriptor::get_angular_velocity_range() const -> angular_velocity_range_t const& 
  {
    return m_angular_velocity_range;
  }

////////////////////////////////////////////////////////////
    void MPU9250_Config::Calibration::set_acceleration(std::vector<Acceleration_Calibration_Point> const& value)
    {
      m_acceleration = value;
    }
    void MPU9250_Config::Calibration::set_acceleration(std::vector<Acceleration_Calibration_Point>&& value)
    {
      m_acceleration = std::move(value);
    }
    auto MPU9250_Config::Calibration::get_acceleration() const -> std::vector<Acceleration_Calibration_Point> const& 
    {
      return m_acceleration;
    }

    auto MPU9250_Config::Calibration::get_acceleration() -> std::vector<Acceleration_Calibration_Point>& 
    {
      return m_acceleration;
    }

////////////////////////////////////////////////////////////
    void MPU9250_Config::Calibration::set_angular_velocity(std::vector<Angular_Velocity_Calibration_Point> const& value)
    {
      m_angular_velocity = value;
    }
    void MPU9250_Config::Calibration::set_angular_velocity(std::vector<Angular_Velocity_Calibration_Point>&& value)
    {
      m_angular_velocity = std::move(value);
    }
    auto MPU9250_Config::Calibration::get_angular_velocity() const -> std::vector<Angular_Velocity_Calibration_Point> const& 
    {
      return m_angular_velocity;
    }

    auto MPU9250_Config::Calibration::get_angular_velocity() -> std::vector<Angular_Velocity_Calibration_Point>& 
    {
      return m_angular_velocity;
    }

////////////////////////////////////////////////////////////
    void MPU9250_Config::Calibration::set_magnetic_field(std::vector<Magnetic_Field_Calibration_Point> const& value)
    {
      m_magnetic_field = value;
    }
    void MPU9250_Config::Calibration::set_magnetic_field(std::vector<Magnetic_Field_Calibration_Point>&& value)
    {
      m_magnetic_field = std::move(value);
    }
    auto MPU9250_Config::Calibration::get_magnetic_field() const -> std::vector<Magnetic_Field_Calibration_Point> const& 
    {
      return m_magnetic_field;
    }

    auto MPU9250_Config::Calibration::get_magnetic_field() -> std::vector<Magnetic_Field_Calibration_Point>& 
    {
      return m_magnetic_field;
    }

////////////////////////////////////////////////////////////
  void MPU9250_Config::set_rotation(euler3_t const& value)
  {
    m_rotation = clamp(value, euler3_t(-180.000000f, -180.000000f, -180.000000f), euler3_t(180.000000f, 180.000000f, 180.000000f));
  }
  void MPU9250_Config::set_rotation(euler3_t&& value)
  {
    m_rotation = clamp(std::move(value), euler3_t(-180.000000f, -180.000000f, -180.000000f), euler3_t(180.000000f, 180.000000f, 180.000000f));
  }
  auto MPU9250_Config::get_rotation() const -> euler3_t const& 
  {
    return m_rotation;
  }

////////////////////////////////////////////////////////////
  void MPU9250_Config::set_calibration(Calibration const& value)
  {
    m_calibration = value;
  }
  void MPU9250_Config::set_calibration(Calibration&& value)
  {
    m_calibration = std::move(value);
  }
  auto MPU9250_Config::get_calibration() const -> Calibration const& 
  {
    return m_calibration;
  }

  auto MPU9250_Config::get_calibration() -> Calibration& 
  {
    return m_calibration;
  }

////////////////////////////////////////////////////////////
  void MS5611_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  void MS5611_Descriptor::set_bus(std::string&& value)
  {
    m_bus = std::move(value);
  }
  auto MS5611_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }

////////////////////////////////////////////////////////////
  void MS5611_Descriptor::set_i2c_address(uint8_t const& value)
  {
    m_i2c_address = min(value, uint8_t(127));
  }
  void MS5611_Descriptor::set_i2c_address(uint8_t&& value)
  {
    m_i2c_address = min(std::move(value), uint8_t(127));
  }
  auto MS5611_Descriptor::get_i2c_address() const -> uint8_t const& 
  {
    return m_i2c_address;
  }

////////////////////////////////////////////////////////////
  void MS5611_Descriptor::set_pressure_rate(uint32_t const& value)
  {
    m_pressure_rate = clamp(value, uint32_t(1), uint32_t(100));
  }
  void MS5611_Descriptor::set_pressure_rate(uint32_t&& value)
  {
    m_pressure_rate = clamp(std::move(value), uint32_t(1), uint32_t(100));
  }
  auto MS5611_Descriptor::get_pressure_rate() const -> uint32_t const& 
  {
    return m_pressure_rate;
  }

////////////////////////////////////////////////////////////
  void MS5611_Descriptor::set_temperature_rate(uint32_t const& value)
  {
    m_temperature_rate = clamp(value, uint32_t(1), uint32_t(100));
  }
  void MS5611_Descriptor::set_temperature_rate(uint32_t&& value)
  {
    m_temperature_rate = clamp(std::move(value), uint32_t(1), uint32_t(100));
  }
  auto MS5611_Descriptor::get_temperature_rate() const -> uint32_t const& 
  {
    return m_temperature_rate;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  void UltimateSensorFusion_Descriptor::set_bus(std::string&& value)
  {
    m_bus = std::move(value);
  }
  auto UltimateSensorFusion_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_i2c_address(uint8_t const& value)
  {
    m_i2c_address = min(value, uint8_t(127));
  }
  void UltimateSensorFusion_Descriptor::set_i2c_address(uint8_t&& value)
  {
    m_i2c_address = min(std::move(value), uint8_t(127));
  }
  auto UltimateSensorFusion_Descriptor::get_i2c_address() const -> uint8_t const& 
  {
    return m_i2c_address;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_imu_rate(imu_rate_t const& value)
  {
    m_imu_rate = value;
  }
  void UltimateSensorFusion_Descriptor::set_imu_rate(imu_rate_t&& value)
  {
    m_imu_rate = std::move(value);
  }
  auto UltimateSensorFusion_Descriptor::get_imu_rate() const -> imu_rate_t const& 
  {
    return m_imu_rate;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_magnetometer_rate(uint32_t const& value)
  {
    m_magnetometer_rate = clamp(value, uint32_t(1), uint32_t(100));
  }
  void UltimateSensorFusion_Descriptor::set_magnetometer_rate(uint32_t&& value)
  {
    m_magnetometer_rate = clamp(std::move(value), uint32_t(1), uint32_t(100));
  }
  auto UltimateSensorFusion_Descriptor::get_magnetometer_rate() const -> uint32_t const& 
  {
    return m_magnetometer_rate;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_barometer_thermometer_rate(uint32_t const& value)
  {
    m_barometer_thermometer_rate = clamp(value, uint32_t(1), uint32_t(100));
  }
  void UltimateSensorFusion_Descriptor::set_barometer_thermometer_rate(uint32_t&& value)
  {
    m_barometer_thermometer_rate = clamp(std::move(value), uint32_t(1), uint32_t(100));
  }
  auto UltimateSensorFusion_Descriptor::get_barometer_thermometer_rate() const -> uint32_t const& 
  {
    return m_barometer_thermometer_rate;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_acceleration_range(acceleration_range_t const& value)
  {
    m_acceleration_range = value;
  }
  void UltimateSensorFusion_Descriptor::set_acceleration_range(acceleration_range_t&& value)
  {
    m_acceleration_range = std::move(value);
  }
  auto UltimateSensorFusion_Descriptor::get_acceleration_range() const -> acceleration_range_t const& 
  {
    return m_acceleration_range;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_angular_velocity_range(angular_velocity_range_t const& value)
  {
    m_angular_velocity_range = value;
  }
  void UltimateSensorFusion_Descriptor::set_angular_velocity_range(angular_velocity_range_t&& value)
  {
    m_angular_velocity_range = std::move(value);
  }
  auto UltimateSensorFusion_Descriptor::get_angular_velocity_range() const -> angular_velocity_range_t const& 
  {
    return m_angular_velocity_range;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_frame_rate_divisor(uint32_t const& value)
  {
    m_frame_rate_divisor = clamp(value, uint32_t(1), uint32_t(10));
  }
  void UltimateSensorFusion_Descriptor::set_frame_rate_divisor(uint32_t&& value)
  {
    m_frame_rate_divisor = clamp(std::move(value), uint32_t(1), uint32_t(10));
  }
  auto UltimateSensorFusion_Descriptor::get_frame_rate_divisor() const -> uint32_t const& 
  {
    return m_frame_rate_divisor;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_imu_output_rate_divisor(uint32_t const& value)
  {
    m_imu_output_rate_divisor = clamp(value, uint32_t(1), uint32_t(10));
  }
  void UltimateSensorFusion_Descriptor::set_imu_output_rate_divisor(uint32_t&& value)
  {
    m_imu_output_rate_divisor = clamp(std::move(value), uint32_t(1), uint32_t(10));
  }
  auto UltimateSensorFusion_Descriptor::get_imu_output_rate_divisor() const -> uint32_t const& 
  {
    return m_imu_output_rate_divisor;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_acceleration_output_enabled(bool const& value)
  {
    m_acceleration_output_enabled = value;
  }
  void UltimateSensorFusion_Descriptor::set_acceleration_output_enabled(bool&& value)
  {
    m_acceleration_output_enabled = std::move(value);
  }
  auto UltimateSensorFusion_Descriptor::get_acceleration_output_enabled() const -> bool const& 
  {
    return m_acceleration_output_enabled;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_angular_velocity_output_enabled(bool const& value)
  {
    m_angular_velocity_output_enabled = value;
  }
  void UltimateSensorFusion_Descriptor::set_angular_velocity_output_enabled(bool&& value)
  {
    m_angular_velocity_output_enabled = std::move(value);
  }
  auto UltimateSensorFusion_Descriptor::get_angular_velocity_output_enabled() const -> bool const& 
  {
    return m_angular_velocity_output_enabled;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_magnetic_field_output_enabled(bool const& value)
  {
    m_magnetic_field_output_enabled = value;
  }
  void UltimateSensorFusion_Descriptor::set_magnetic_field_output_enabled(bool&& value)
  {
    m_magnetic_field_output_enabled = std::move(value);
  }
  auto UltimateSensorFusion_Descriptor::get_magnetic_field_output_enabled() const -> bool const& 
  {
    return m_magnetic_field_output_enabled;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_frame_output_enabled(bool const& value)
  {
    m_frame_output_enabled = value;
  }
  void UltimateSensorFusion_Descriptor::set_frame_output_enabled(bool&& value)
  {
    m_frame_output_enabled = std::move(value);
  }
  auto UltimateSensorFusion_Descriptor::get_frame_output_enabled() const -> bool const& 
  {
    return m_frame_output_enabled;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_pressure_output_enabled(bool const& value)
  {
    m_pressure_output_enabled = value;
  }
  void UltimateSensorFusion_Descriptor::set_pressure_output_enabled(bool&& value)
  {
    m_pressure_output_enabled = std::move(value);
  }
  auto UltimateSensorFusion_Descriptor::get_pressure_output_enabled() const -> bool const& 
  {
    return m_pressure_output_enabled;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Descriptor::set_temperature_output_enabled(bool const& value)
  {
    m_temperature_output_enabled = value;
  }
  void UltimateSensorFusion_Descriptor::set_temperature_output_enabled(bool&& value)
  {
    m_temperature_output_enabled = std::move(value);
  }
  auto UltimateSensorFusion_Descriptor::get_temperature_output_enabled() const -> bool const& 
  {
    return m_temperature_output_enabled;
  }

////////////////////////////////////////////////////////////
  void UltimateSensorFusion_Config::set_rotation(euler3_t const& value)
  {
    m_rotation = clamp(value, euler3_t(-180.000000f, -180.000000f, -180.000000f), euler3_t(180.000000f, 180.000000f, 180.000000f));
  }
  void UltimateSensorFusion_Config::set_rotation(euler3_t&& value)
  {
    m_rotation = clamp(std::move(value), euler3_t(-180.000000f, -180.000000f, -180.000000f), euler3_t(180.000000f, 180.000000f, 180.000000f));
  }
  auto UltimateSensorFusion_Config::get_rotation() const -> euler3_t const& 
  {
    return m_rotation;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Brain_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Multirotor_Brain_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Multirotor_Brain_Descriptor::set_commands_rate(uint32_t&& value)
  {
    m_commands_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Multirotor_Brain_Descriptor::set_state_rate(uint32_t&& value)
  {
    m_state_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Brain_Descriptor::get_state_rate() const -> uint32_t const& 
  {
    return m_state_rate;
  }

////////////////////////////////////////////////////////////
      void Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs::set_x_pid(PID_Controller_Descriptor const& value)
      {
        m_x_pid = value;
      }
      void Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs::set_x_pid(PID_Controller_Descriptor&& value)
      {
        m_x_pid = std::move(value);
      }
      auto Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs::get_x_pid() const -> PID_Controller_Descriptor const& 
      {
        return m_x_pid;
      }

      auto Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs::get_x_pid() -> PID_Controller_Descriptor& 
      {
        return m_x_pid;
      }

////////////////////////////////////////////////////////////
      void Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs::set_y_pid(PID_Controller_Descriptor const& value)
      {
        m_y_pid = value;
      }
      void Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs::set_y_pid(PID_Controller_Descriptor&& value)
      {
        m_y_pid = std::move(value);
      }
      auto Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs::get_y_pid() const -> PID_Controller_Descriptor const& 
      {
        return m_y_pid;
      }

      auto Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs::get_y_pid() -> PID_Controller_Descriptor& 
      {
        return m_y_pid;
      }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Horizontal::set_max_rate_deg(speed_deg_t const& value)
    {
      m_max_rate_deg = clamp(value, speed_deg_t(10.000000f), speed_deg_t(3000.000000f));
    }
    void Multirotor_Brain_Config::Horizontal::set_max_rate_deg(speed_deg_t&& value)
    {
      m_max_rate_deg = clamp(std::move(value), speed_deg_t(10.000000f), speed_deg_t(3000.000000f));
    }
    auto Multirotor_Brain_Config::Horizontal::get_max_rate_deg() const -> speed_deg_t const& 
    {
      return m_max_rate_deg;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Horizontal::set_angle_pids(boost::variant<Combined_Angle_PIDs,Separate_Angle_PIDs> const& value)
    {
      m_angle_pids = value;
    }
    void Multirotor_Brain_Config::Horizontal::set_angle_pids(boost::variant<Combined_Angle_PIDs,Separate_Angle_PIDs>&& value)
    {
      m_angle_pids = std::move(value);
    }
    auto Multirotor_Brain_Config::Horizontal::get_angle_pids() const -> boost::variant<Combined_Angle_PIDs,Separate_Angle_PIDs> const& 
    {
      return m_angle_pids;
    }

    auto Multirotor_Brain_Config::Horizontal::get_angle_pids() -> boost::variant<Combined_Angle_PIDs,Separate_Angle_PIDs>& 
    {
      return m_angle_pids;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Horizontal::set_max_angle_deg(max_angle_t const& value)
    {
      m_max_angle_deg = clamp(value, max_angle_t(-90.000000f, -90.000000f), max_angle_t(90.000000f, 90.000000f));
    }
    void Multirotor_Brain_Config::Horizontal::set_max_angle_deg(max_angle_t&& value)
    {
      m_max_angle_deg = clamp(std::move(value), max_angle_t(-90.000000f, -90.000000f), max_angle_t(90.000000f, 90.000000f));
    }
    auto Multirotor_Brain_Config::Horizontal::get_max_angle_deg() const -> max_angle_t const& 
    {
      return m_max_angle_deg;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Horizontal::set_max_speed(speed_t const& value)
    {
      m_max_speed = clamp(value, speed_t(0), speed_t(10.000000f));
    }
    void Multirotor_Brain_Config::Horizontal::set_max_speed(speed_t&& value)
    {
      m_max_speed = clamp(std::move(value), speed_t(0), speed_t(10.000000f));
    }
    auto Multirotor_Brain_Config::Horizontal::get_max_speed() const -> speed_t const& 
    {
      return m_max_speed;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Horizontal::set_position_lpf(LPF_Config const& value)
    {
      m_position_lpf = value;
    }
    void Multirotor_Brain_Config::Horizontal::set_position_lpf(LPF_Config&& value)
    {
      m_position_lpf = std::move(value);
    }
    auto Multirotor_Brain_Config::Horizontal::get_position_lpf() const -> LPF_Config const& 
    {
      return m_position_lpf;
    }

    auto Multirotor_Brain_Config::Horizontal::get_position_lpf() -> LPF_Config& 
    {
      return m_position_lpf;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Horizontal::set_velocity_pid(PI_Controller_Descriptor const& value)
    {
      m_velocity_pid = value;
    }
    void Multirotor_Brain_Config::Horizontal::set_velocity_pid(PI_Controller_Descriptor&& value)
    {
      m_velocity_pid = std::move(value);
    }
    auto Multirotor_Brain_Config::Horizontal::get_velocity_pid() const -> PI_Controller_Descriptor const& 
    {
      return m_velocity_pid;
    }

    auto Multirotor_Brain_Config::Horizontal::get_velocity_pid() -> PI_Controller_Descriptor& 
    {
      return m_velocity_pid;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Horizontal::set_position_pid(P_Controller_Descriptor const& value)
    {
      m_position_pid = value;
    }
    void Multirotor_Brain_Config::Horizontal::set_position_pid(P_Controller_Descriptor&& value)
    {
      m_position_pid = std::move(value);
    }
    auto Multirotor_Brain_Config::Horizontal::get_position_pid() const -> P_Controller_Descriptor const& 
    {
      return m_position_pid;
    }

    auto Multirotor_Brain_Config::Horizontal::get_position_pid() -> P_Controller_Descriptor& 
    {
      return m_position_pid;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Yaw::set_max_rate_deg(speed_deg_t const& value)
    {
      m_max_rate_deg = clamp(value, speed_deg_t(10.000000f), speed_deg_t(3000.000000f));
    }
    void Multirotor_Brain_Config::Yaw::set_max_rate_deg(speed_deg_t&& value)
    {
      m_max_rate_deg = clamp(std::move(value), speed_deg_t(10.000000f), speed_deg_t(3000.000000f));
    }
    auto Multirotor_Brain_Config::Yaw::get_max_rate_deg() const -> speed_deg_t const& 
    {
      return m_max_rate_deg;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Yaw::set_angle_pid(PID_Controller_Descriptor const& value)
    {
      m_angle_pid = value;
    }
    void Multirotor_Brain_Config::Yaw::set_angle_pid(PID_Controller_Descriptor&& value)
    {
      m_angle_pid = std::move(value);
    }
    auto Multirotor_Brain_Config::Yaw::get_angle_pid() const -> PID_Controller_Descriptor const& 
    {
      return m_angle_pid;
    }

    auto Multirotor_Brain_Config::Yaw::get_angle_pid() -> PID_Controller_Descriptor& 
    {
      return m_angle_pid;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Vertical::set_max_speed(speed_t const& value)
    {
      m_max_speed = clamp(value, speed_t(0), speed_t(10.000000f));
    }
    void Multirotor_Brain_Config::Vertical::set_max_speed(speed_t&& value)
    {
      m_max_speed = clamp(std::move(value), speed_t(0), speed_t(10.000000f));
    }
    auto Multirotor_Brain_Config::Vertical::get_max_speed() const -> speed_t const& 
    {
      return m_max_speed;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Vertical::set_altitude_lpf(LPF_Config const& value)
    {
      m_altitude_lpf = value;
    }
    void Multirotor_Brain_Config::Vertical::set_altitude_lpf(LPF_Config&& value)
    {
      m_altitude_lpf = std::move(value);
    }
    auto Multirotor_Brain_Config::Vertical::get_altitude_lpf() const -> LPF_Config const& 
    {
      return m_altitude_lpf;
    }

    auto Multirotor_Brain_Config::Vertical::get_altitude_lpf() -> LPF_Config& 
    {
      return m_altitude_lpf;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Vertical::set_speed_pid(PI_Controller_Descriptor const& value)
    {
      m_speed_pid = value;
    }
    void Multirotor_Brain_Config::Vertical::set_speed_pid(PI_Controller_Descriptor&& value)
    {
      m_speed_pid = std::move(value);
    }
    auto Multirotor_Brain_Config::Vertical::get_speed_pid() const -> PI_Controller_Descriptor const& 
    {
      return m_speed_pid;
    }

    auto Multirotor_Brain_Config::Vertical::get_speed_pid() -> PI_Controller_Descriptor& 
    {
      return m_speed_pid;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Brain_Config::Vertical::set_altitude_pid(P_Controller_Descriptor const& value)
    {
      m_altitude_pid = value;
    }
    void Multirotor_Brain_Config::Vertical::set_altitude_pid(P_Controller_Descriptor&& value)
    {
      m_altitude_pid = std::move(value);
    }
    auto Multirotor_Brain_Config::Vertical::get_altitude_pid() const -> P_Controller_Descriptor const& 
    {
      return m_altitude_pid;
    }

    auto Multirotor_Brain_Config::Vertical::get_altitude_pid() -> P_Controller_Descriptor& 
    {
      return m_altitude_pid;
    }

////////////////////////////////////////////////////////////
  void Multirotor_Brain_Config::set_max_allowed_position_variation(ufloat const& value)
  {
    m_max_allowed_position_variation = max(value, ufloat(0));
  }
  void Multirotor_Brain_Config::set_max_allowed_position_variation(ufloat&& value)
  {
    m_max_allowed_position_variation = max(std::move(value), ufloat(0));
  }
  auto Multirotor_Brain_Config::get_max_allowed_position_variation() const -> ufloat const& 
  {
    return m_max_allowed_position_variation;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Brain_Config::set_max_thrust(ufloat const& value)
  {
    m_max_thrust = max(value, ufloat(0));
  }
  void Multirotor_Brain_Config::set_max_thrust(ufloat&& value)
  {
    m_max_thrust = max(std::move(value), ufloat(0));
  }
  auto Multirotor_Brain_Config::get_max_thrust() const -> ufloat const& 
  {
    return m_max_thrust;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Brain_Config::set_min_thrust(ufloat const& value)
  {
    m_min_thrust = max(value, ufloat(0));
  }
  void Multirotor_Brain_Config::set_min_thrust(ufloat&& value)
  {
    m_min_thrust = max(std::move(value), ufloat(0));
  }
  auto Multirotor_Brain_Config::get_min_thrust() const -> ufloat const& 
  {
    return m_min_thrust;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Brain_Config::set_vertical(Vertical const& value)
  {
    m_vertical = value;
  }
  void Multirotor_Brain_Config::set_vertical(Vertical&& value)
  {
    m_vertical = std::move(value);
  }
  auto Multirotor_Brain_Config::get_vertical() const -> Vertical const& 
  {
    return m_vertical;
  }

  auto Multirotor_Brain_Config::get_vertical() -> Vertical& 
  {
    return m_vertical;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Brain_Config::set_horizontal(Horizontal const& value)
  {
    m_horizontal = value;
  }
  void Multirotor_Brain_Config::set_horizontal(Horizontal&& value)
  {
    m_horizontal = std::move(value);
  }
  auto Multirotor_Brain_Config::get_horizontal() const -> Horizontal const& 
  {
    return m_horizontal;
  }

  auto Multirotor_Brain_Config::get_horizontal() -> Horizontal& 
  {
    return m_horizontal;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Brain_Config::set_yaw(Yaw const& value)
  {
    m_yaw = value;
  }
  void Multirotor_Brain_Config::set_yaw(Yaw&& value)
  {
    m_yaw = std::move(value);
  }
  auto Multirotor_Brain_Config::get_yaw() const -> Yaw const& 
  {
    return m_yaw;
  }

  auto Multirotor_Brain_Config::get_yaw() -> Yaw& 
  {
    return m_yaw;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Pilot_Descriptor::set_commands_rate(uint32_t const& value)
  {
    m_commands_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Multirotor_Pilot_Descriptor::set_commands_rate(uint32_t&& value)
  {
    m_commands_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Multirotor_Pilot_Descriptor::set_state_rate(uint32_t&& value)
  {
    m_state_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Multirotor_Pilot_Descriptor::set_video_rate(uint32_t&& value)
  {
    m_video_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Multirotor_Simulator_Descriptor::set_throttle_rate(uint32_t&& value)
  {
    m_throttle_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Simulator_Descriptor::get_throttle_rate() const -> uint32_t const& 
  {
    return m_throttle_rate;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulator_Descriptor::set_state_rate(uint32_t const& value)
  {
    m_state_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Multirotor_Simulator_Descriptor::set_state_rate(uint32_t&& value)
  {
    m_state_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Simulator_Descriptor::get_state_rate() const -> uint32_t const& 
  {
    return m_state_rate;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulator_Descriptor::set_acceleration_rate(uint32_t const& value)
  {
    m_acceleration_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Multirotor_Simulator_Descriptor::set_acceleration_rate(uint32_t&& value)
  {
    m_acceleration_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Multirotor_Simulator_Descriptor::set_angular_velocity_rate(uint32_t&& value)
  {
    m_angular_velocity_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Multirotor_Simulator_Descriptor::set_magnetic_field_rate(uint32_t&& value)
  {
    m_magnetic_field_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Multirotor_Simulator_Descriptor::set_pressure_rate(uint32_t&& value)
  {
    m_pressure_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Multirotor_Simulator_Descriptor::set_temperature_rate(uint32_t&& value)
  {
    m_temperature_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Multirotor_Simulator_Descriptor::set_distance_rate(uint32_t&& value)
  {
    m_distance_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Multirotor_Simulator_Descriptor::set_gps_rate(uint32_t&& value)
  {
    m_gps_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Multirotor_Simulator_Descriptor::get_gps_rate() const -> uint32_t const& 
  {
    return m_gps_rate;
  }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Noise::set_gps_position(ufloat const& value)
    {
      m_gps_position = max(value, ufloat(0));
    }
    void Multirotor_Simulator_Config::Noise::set_gps_position(ufloat&& value)
    {
      m_gps_position = max(std::move(value), ufloat(0));
    }
    auto Multirotor_Simulator_Config::Noise::get_gps_position() const -> ufloat const& 
    {
      return m_gps_position;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Noise::set_gps_velocity(ufloat const& value)
    {
      m_gps_velocity = max(value, ufloat(0));
    }
    void Multirotor_Simulator_Config::Noise::set_gps_velocity(ufloat&& value)
    {
      m_gps_velocity = max(std::move(value), ufloat(0));
    }
    auto Multirotor_Simulator_Config::Noise::get_gps_velocity() const -> ufloat const& 
    {
      return m_gps_velocity;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Noise::set_gps_pacc(ufloat const& value)
    {
      m_gps_pacc = max(value, ufloat(0));
    }
    void Multirotor_Simulator_Config::Noise::set_gps_pacc(ufloat&& value)
    {
      m_gps_pacc = max(std::move(value), ufloat(0));
    }
    auto Multirotor_Simulator_Config::Noise::get_gps_pacc() const -> ufloat const& 
    {
      return m_gps_pacc;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Noise::set_gps_vacc(ufloat const& value)
    {
      m_gps_vacc = max(value, ufloat(0));
    }
    void Multirotor_Simulator_Config::Noise::set_gps_vacc(ufloat&& value)
    {
      m_gps_vacc = max(std::move(value), ufloat(0));
    }
    auto Multirotor_Simulator_Config::Noise::get_gps_vacc() const -> ufloat const& 
    {
      return m_gps_vacc;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Noise::set_acceleration(ufloat const& value)
    {
      m_acceleration = max(value, ufloat(0));
    }
    void Multirotor_Simulator_Config::Noise::set_acceleration(ufloat&& value)
    {
      m_acceleration = max(std::move(value), ufloat(0));
    }
    auto Multirotor_Simulator_Config::Noise::get_acceleration() const -> ufloat const& 
    {
      return m_acceleration;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Noise::set_angular_velocity(ufloat const& value)
    {
      m_angular_velocity = max(value, ufloat(0));
    }
    void Multirotor_Simulator_Config::Noise::set_angular_velocity(ufloat&& value)
    {
      m_angular_velocity = max(std::move(value), ufloat(0));
    }
    auto Multirotor_Simulator_Config::Noise::get_angular_velocity() const -> ufloat const& 
    {
      return m_angular_velocity;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Noise::set_magnetic_field(ufloat const& value)
    {
      m_magnetic_field = max(value, ufloat(0));
    }
    void Multirotor_Simulator_Config::Noise::set_magnetic_field(ufloat&& value)
    {
      m_magnetic_field = max(std::move(value), ufloat(0));
    }
    auto Multirotor_Simulator_Config::Noise::get_magnetic_field() const -> ufloat const& 
    {
      return m_magnetic_field;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Noise::set_pressure(ufloat const& value)
    {
      m_pressure = max(value, ufloat(0));
    }
    void Multirotor_Simulator_Config::Noise::set_pressure(ufloat&& value)
    {
      m_pressure = max(std::move(value), ufloat(0));
    }
    auto Multirotor_Simulator_Config::Noise::get_pressure() const -> ufloat const& 
    {
      return m_pressure;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Noise::set_temperature(ufloat const& value)
    {
      m_temperature = max(value, ufloat(0));
    }
    void Multirotor_Simulator_Config::Noise::set_temperature(ufloat&& value)
    {
      m_temperature = max(std::move(value), ufloat(0));
    }
    auto Multirotor_Simulator_Config::Noise::get_temperature() const -> ufloat const& 
    {
      return m_temperature;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Noise::set_ground_distance(ufloat const& value)
    {
      m_ground_distance = max(value, ufloat(0));
    }
    void Multirotor_Simulator_Config::Noise::set_ground_distance(ufloat&& value)
    {
      m_ground_distance = max(std::move(value), ufloat(0));
    }
    auto Multirotor_Simulator_Config::Noise::get_ground_distance() const -> ufloat const& 
    {
      return m_ground_distance;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Calibration::set_acceleration(std::vector<Acceleration_Calibration_Point> const& value)
    {
      m_acceleration = value;
    }
    void Multirotor_Simulator_Config::Calibration::set_acceleration(std::vector<Acceleration_Calibration_Point>&& value)
    {
      m_acceleration = std::move(value);
    }
    auto Multirotor_Simulator_Config::Calibration::get_acceleration() const -> std::vector<Acceleration_Calibration_Point> const& 
    {
      return m_acceleration;
    }

    auto Multirotor_Simulator_Config::Calibration::get_acceleration() -> std::vector<Acceleration_Calibration_Point>& 
    {
      return m_acceleration;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Calibration::set_angular_velocity(std::vector<Angular_Velocity_Calibration_Point> const& value)
    {
      m_angular_velocity = value;
    }
    void Multirotor_Simulator_Config::Calibration::set_angular_velocity(std::vector<Angular_Velocity_Calibration_Point>&& value)
    {
      m_angular_velocity = std::move(value);
    }
    auto Multirotor_Simulator_Config::Calibration::get_angular_velocity() const -> std::vector<Angular_Velocity_Calibration_Point> const& 
    {
      return m_angular_velocity;
    }

    auto Multirotor_Simulator_Config::Calibration::get_angular_velocity() -> std::vector<Angular_Velocity_Calibration_Point>& 
    {
      return m_angular_velocity;
    }

////////////////////////////////////////////////////////////
    void Multirotor_Simulator_Config::Calibration::set_magnetic_field(std::vector<Magnetic_Field_Calibration_Point> const& value)
    {
      m_magnetic_field = value;
    }
    void Multirotor_Simulator_Config::Calibration::set_magnetic_field(std::vector<Magnetic_Field_Calibration_Point>&& value)
    {
      m_magnetic_field = std::move(value);
    }
    auto Multirotor_Simulator_Config::Calibration::get_magnetic_field() const -> std::vector<Magnetic_Field_Calibration_Point> const& 
    {
      return m_magnetic_field;
    }

    auto Multirotor_Simulator_Config::Calibration::get_magnetic_field() -> std::vector<Magnetic_Field_Calibration_Point>& 
    {
      return m_magnetic_field;
    }

////////////////////////////////////////////////////////////
  void Multirotor_Simulator_Config::set_simulation_enabled(bool const& value)
  {
    m_simulation_enabled = value;
  }
  void Multirotor_Simulator_Config::set_simulation_enabled(bool&& value)
  {
    m_simulation_enabled = std::move(value);
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
  void Multirotor_Simulator_Config::set_drag_enabled(bool&& value)
  {
    m_drag_enabled = std::move(value);
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
  void Multirotor_Simulator_Config::set_ground_enabled(bool&& value)
  {
    m_ground_enabled = std::move(value);
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
  void Multirotor_Simulator_Config::set_gravity_enabled(bool&& value)
  {
    m_gravity_enabled = std::move(value);
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
  void Multirotor_Simulator_Config::set_noise(Noise&& value)
  {
    m_noise = std::move(value);
  }
  auto Multirotor_Simulator_Config::get_noise() const -> Noise const& 
  {
    return m_noise;
  }

  auto Multirotor_Simulator_Config::get_noise() -> Noise& 
  {
    return m_noise;
  }

////////////////////////////////////////////////////////////
  void Multirotor_Simulator_Config::set_calibration(Calibration const& value)
  {
    m_calibration = value;
  }
  void Multirotor_Simulator_Config::set_calibration(Calibration&& value)
  {
    m_calibration = std::move(value);
  }
  auto Multirotor_Simulator_Config::get_calibration() const -> Calibration const& 
  {
    return m_calibration;
  }

  auto Multirotor_Simulator_Config::get_calibration() -> Calibration& 
  {
    return m_calibration;
  }

////////////////////////////////////////////////////////////
  void Oscillator_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Oscillator_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Oscillator_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void Oscillator_Descriptor::set_component_count(component_count_t const& value)
  {
    m_component_count = clamp(value, component_count_t(1), component_count_t(32));
  }
  void Oscillator_Descriptor::set_component_count(component_count_t&& value)
  {
    m_component_count = clamp(std::move(value), component_count_t(1), component_count_t(32));
  }
  auto Oscillator_Descriptor::get_component_count() const -> component_count_t const& 
  {
    return m_component_count;
  }

////////////////////////////////////////////////////////////
    void Oscillator_Config::Component::set_frequency(ufloat const& value)
    {
      m_frequency = max(value, ufloat(0));
    }
    void Oscillator_Config::Component::set_frequency(ufloat&& value)
    {
      m_frequency = max(std::move(value), ufloat(0));
    }
    auto Oscillator_Config::Component::get_frequency() const -> ufloat const& 
    {
      return m_frequency;
    }

////////////////////////////////////////////////////////////
    void Oscillator_Config::Component::set_amplitude(ufloat const& value)
    {
      m_amplitude = max(value, ufloat(0));
    }
    void Oscillator_Config::Component::set_amplitude(ufloat&& value)
    {
      m_amplitude = max(std::move(value), ufloat(0));
    }
    auto Oscillator_Config::Component::get_amplitude() const -> ufloat const& 
    {
      return m_amplitude;
    }

////////////////////////////////////////////////////////////
    void Oscillator_Config::Component::set_square(bool const& value)
    {
      m_square = value;
    }
    void Oscillator_Config::Component::set_square(bool&& value)
    {
      m_square = std::move(value);
    }
    auto Oscillator_Config::Component::get_square() const -> bool const& 
    {
      return m_square;
    }

////////////////////////////////////////////////////////////
  void Oscillator_Config::set_amplitude(ufloat const& value)
  {
    m_amplitude = max(value, ufloat(0));
  }
  void Oscillator_Config::set_amplitude(ufloat&& value)
  {
    m_amplitude = max(std::move(value), ufloat(0));
  }
  auto Oscillator_Config::get_amplitude() const -> ufloat const& 
  {
    return m_amplitude;
  }

////////////////////////////////////////////////////////////
  void Oscillator_Config::set_noise(ufloat const& value)
  {
    m_noise = max(value, ufloat(0));
  }
  void Oscillator_Config::set_noise(ufloat&& value)
  {
    m_noise = max(std::move(value), ufloat(0));
  }
  auto Oscillator_Config::get_noise() const -> ufloat const& 
  {
    return m_noise;
  }

////////////////////////////////////////////////////////////
  void Oscillator_Config::set_components(std::vector<Oscillator_Config::Component> const& value)
  {
    m_components = value;
  }
  void Oscillator_Config::set_components(std::vector<Oscillator_Config::Component>&& value)
  {
    m_components = std::move(value);
  }
  auto Oscillator_Config::get_components() const -> std::vector<Oscillator_Config::Component> const& 
  {
    return m_components;
  }

  auto Oscillator_Config::get_components() -> std::vector<Oscillator_Config::Component>& 
  {
    return m_components;
  }

////////////////////////////////////////////////////////////
    void PCA9685_Descriptor::Channel::set_enabled(bool const& value)
    {
      m_enabled = value;
    }
    void PCA9685_Descriptor::Channel::set_enabled(bool&& value)
    {
      m_enabled = std::move(value);
    }
    auto PCA9685_Descriptor::Channel::get_enabled() const -> bool const& 
    {
      return m_enabled;
    }

////////////////////////////////////////////////////////////
    void PCA9685_Descriptor::Channel::set_servo_signal(bool const& value)
    {
      m_servo_signal = value;
    }
    void PCA9685_Descriptor::Channel::set_servo_signal(bool&& value)
    {
      m_servo_signal = std::move(value);
    }
    auto PCA9685_Descriptor::Channel::get_servo_signal() const -> bool const& 
    {
      return m_servo_signal;
    }

////////////////////////////////////////////////////////////
  void PCA9685_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  void PCA9685_Descriptor::set_bus(std::string&& value)
  {
    m_bus = std::move(value);
  }
  auto PCA9685_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }

////////////////////////////////////////////////////////////
  void PCA9685_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(8000));
  }
  void PCA9685_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(8000));
  }
  auto PCA9685_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void PCA9685_Descriptor::set_address(uint8_t const& value)
  {
    m_address = min(value, uint8_t(127));
  }
  void PCA9685_Descriptor::set_address(uint8_t&& value)
  {
    m_address = min(std::move(value), uint8_t(127));
  }
  auto PCA9685_Descriptor::get_address() const -> uint8_t const& 
  {
    return m_address;
  }

////////////////////////////////////////////////////////////
  void PCA9685_Descriptor::set_channels(std::vector<PCA9685_Descriptor::Channel> const& value)
  {
    m_channels = value;
  }
  void PCA9685_Descriptor::set_channels(std::vector<PCA9685_Descriptor::Channel>&& value)
  {
    m_channels = std::move(value);
  }
  auto PCA9685_Descriptor::get_channels() const -> std::vector<PCA9685_Descriptor::Channel> const& 
  {
    return m_channels;
  }

  auto PCA9685_Descriptor::get_channels() -> std::vector<PCA9685_Descriptor::Channel>& 
  {
    return m_channels;
  }

////////////////////////////////////////////////////////////
    void PCA9685_Config::Servo_Channel::set_min(servo_signal_t const& value)
    {
      m_min = clamp(value, servo_signal_t(1.000000f), servo_signal_t(2.000000f));
    }
    void PCA9685_Config::Servo_Channel::set_min(servo_signal_t&& value)
    {
      m_min = clamp(std::move(value), servo_signal_t(1.000000f), servo_signal_t(2.000000f));
    }
    auto PCA9685_Config::Servo_Channel::get_min() const -> servo_signal_t const& 
    {
      return m_min;
    }

////////////////////////////////////////////////////////////
    void PCA9685_Config::Servo_Channel::set_max(servo_signal_t const& value)
    {
      m_max = clamp(value, servo_signal_t(1.000000f), servo_signal_t(2.000000f));
    }
    void PCA9685_Config::Servo_Channel::set_max(servo_signal_t&& value)
    {
      m_max = clamp(std::move(value), servo_signal_t(1.000000f), servo_signal_t(2.000000f));
    }
    auto PCA9685_Config::Servo_Channel::get_max() const -> servo_signal_t const& 
    {
      return m_max;
    }

////////////////////////////////////////////////////////////
    void PCA9685_Config::PWM_Channel::set_min(pwm_signal_t const& value)
    {
      m_min = clamp(value, pwm_signal_t(0), pwm_signal_t(1.000000f));
    }
    void PCA9685_Config::PWM_Channel::set_min(pwm_signal_t&& value)
    {
      m_min = clamp(std::move(value), pwm_signal_t(0), pwm_signal_t(1.000000f));
    }
    auto PCA9685_Config::PWM_Channel::get_min() const -> pwm_signal_t const& 
    {
      return m_min;
    }

////////////////////////////////////////////////////////////
    void PCA9685_Config::PWM_Channel::set_max(pwm_signal_t const& value)
    {
      m_max = clamp(value, pwm_signal_t(0), pwm_signal_t(1.000000f));
    }
    void PCA9685_Config::PWM_Channel::set_max(pwm_signal_t&& value)
    {
      m_max = clamp(std::move(value), pwm_signal_t(0), pwm_signal_t(1.000000f));
    }
    auto PCA9685_Config::PWM_Channel::get_max() const -> pwm_signal_t const& 
    {
      return m_max;
    }

////////////////////////////////////////////////////////////
  void PCA9685_Config::set_channels(std::vector<Poly<PCA9685_Config::IChannel>> const& value)
  {
    m_channels = value;
  }
  void PCA9685_Config::set_channels(std::vector<Poly<PCA9685_Config::IChannel>>&& value)
  {
    m_channels = std::move(value);
  }
  auto PCA9685_Config::get_channels() const -> std::vector<Poly<PCA9685_Config::IChannel>> const& 
  {
    return m_channels;
  }

  auto PCA9685_Config::get_channels() -> std::vector<Poly<PCA9685_Config::IChannel>>& 
  {
    return m_channels;
  }

////////////////////////////////////////////////////////////
    void PIGPIO_Descriptor::Channel::set_enabled(bool const& value)
    {
      m_enabled = value;
    }
    void PIGPIO_Descriptor::Channel::set_enabled(bool&& value)
    {
      m_enabled = std::move(value);
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
    void PIGPIO_Descriptor::Channel::set_servo_signal(bool&& value)
    {
      m_servo_signal = std::move(value);
    }
    auto PIGPIO_Descriptor::Channel::get_servo_signal() const -> bool const& 
    {
      return m_servo_signal;
    }

////////////////////////////////////////////////////////////
    void PIGPIO_Descriptor::Channel::set_rate(uint32_t const& value)
    {
      m_rate = clamp(value, uint32_t(1), uint32_t(8000));
    }
    void PIGPIO_Descriptor::Channel::set_rate(uint32_t&& value)
    {
      m_rate = clamp(std::move(value), uint32_t(1), uint32_t(8000));
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
  void PIGPIO_Descriptor::set_gpio_2(Channel&& value)
  {
    m_gpio_2 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_2() const -> Channel const& 
  {
    return m_gpio_2;
  }

  auto PIGPIO_Descriptor::get_gpio_2() -> Channel& 
  {
    return m_gpio_2;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_3(Channel const& value)
  {
    m_gpio_3 = value;
  }
  void PIGPIO_Descriptor::set_gpio_3(Channel&& value)
  {
    m_gpio_3 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_3() const -> Channel const& 
  {
    return m_gpio_3;
  }

  auto PIGPIO_Descriptor::get_gpio_3() -> Channel& 
  {
    return m_gpio_3;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_4(Channel const& value)
  {
    m_gpio_4 = value;
  }
  void PIGPIO_Descriptor::set_gpio_4(Channel&& value)
  {
    m_gpio_4 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_4() const -> Channel const& 
  {
    return m_gpio_4;
  }

  auto PIGPIO_Descriptor::get_gpio_4() -> Channel& 
  {
    return m_gpio_4;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_5(Channel const& value)
  {
    m_gpio_5 = value;
  }
  void PIGPIO_Descriptor::set_gpio_5(Channel&& value)
  {
    m_gpio_5 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_5() const -> Channel const& 
  {
    return m_gpio_5;
  }

  auto PIGPIO_Descriptor::get_gpio_5() -> Channel& 
  {
    return m_gpio_5;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_6(Channel const& value)
  {
    m_gpio_6 = value;
  }
  void PIGPIO_Descriptor::set_gpio_6(Channel&& value)
  {
    m_gpio_6 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_6() const -> Channel const& 
  {
    return m_gpio_6;
  }

  auto PIGPIO_Descriptor::get_gpio_6() -> Channel& 
  {
    return m_gpio_6;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_7(Channel const& value)
  {
    m_gpio_7 = value;
  }
  void PIGPIO_Descriptor::set_gpio_7(Channel&& value)
  {
    m_gpio_7 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_7() const -> Channel const& 
  {
    return m_gpio_7;
  }

  auto PIGPIO_Descriptor::get_gpio_7() -> Channel& 
  {
    return m_gpio_7;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_8(Channel const& value)
  {
    m_gpio_8 = value;
  }
  void PIGPIO_Descriptor::set_gpio_8(Channel&& value)
  {
    m_gpio_8 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_8() const -> Channel const& 
  {
    return m_gpio_8;
  }

  auto PIGPIO_Descriptor::get_gpio_8() -> Channel& 
  {
    return m_gpio_8;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_9(Channel const& value)
  {
    m_gpio_9 = value;
  }
  void PIGPIO_Descriptor::set_gpio_9(Channel&& value)
  {
    m_gpio_9 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_9() const -> Channel const& 
  {
    return m_gpio_9;
  }

  auto PIGPIO_Descriptor::get_gpio_9() -> Channel& 
  {
    return m_gpio_9;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_10(Channel const& value)
  {
    m_gpio_10 = value;
  }
  void PIGPIO_Descriptor::set_gpio_10(Channel&& value)
  {
    m_gpio_10 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_10() const -> Channel const& 
  {
    return m_gpio_10;
  }

  auto PIGPIO_Descriptor::get_gpio_10() -> Channel& 
  {
    return m_gpio_10;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_11(Channel const& value)
  {
    m_gpio_11 = value;
  }
  void PIGPIO_Descriptor::set_gpio_11(Channel&& value)
  {
    m_gpio_11 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_11() const -> Channel const& 
  {
    return m_gpio_11;
  }

  auto PIGPIO_Descriptor::get_gpio_11() -> Channel& 
  {
    return m_gpio_11;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_12(Channel const& value)
  {
    m_gpio_12 = value;
  }
  void PIGPIO_Descriptor::set_gpio_12(Channel&& value)
  {
    m_gpio_12 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_12() const -> Channel const& 
  {
    return m_gpio_12;
  }

  auto PIGPIO_Descriptor::get_gpio_12() -> Channel& 
  {
    return m_gpio_12;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_13(Channel const& value)
  {
    m_gpio_13 = value;
  }
  void PIGPIO_Descriptor::set_gpio_13(Channel&& value)
  {
    m_gpio_13 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_13() const -> Channel const& 
  {
    return m_gpio_13;
  }

  auto PIGPIO_Descriptor::get_gpio_13() -> Channel& 
  {
    return m_gpio_13;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_14(Channel const& value)
  {
    m_gpio_14 = value;
  }
  void PIGPIO_Descriptor::set_gpio_14(Channel&& value)
  {
    m_gpio_14 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_14() const -> Channel const& 
  {
    return m_gpio_14;
  }

  auto PIGPIO_Descriptor::get_gpio_14() -> Channel& 
  {
    return m_gpio_14;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_15(Channel const& value)
  {
    m_gpio_15 = value;
  }
  void PIGPIO_Descriptor::set_gpio_15(Channel&& value)
  {
    m_gpio_15 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_15() const -> Channel const& 
  {
    return m_gpio_15;
  }

  auto PIGPIO_Descriptor::get_gpio_15() -> Channel& 
  {
    return m_gpio_15;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_16(Channel const& value)
  {
    m_gpio_16 = value;
  }
  void PIGPIO_Descriptor::set_gpio_16(Channel&& value)
  {
    m_gpio_16 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_16() const -> Channel const& 
  {
    return m_gpio_16;
  }

  auto PIGPIO_Descriptor::get_gpio_16() -> Channel& 
  {
    return m_gpio_16;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_17(Channel const& value)
  {
    m_gpio_17 = value;
  }
  void PIGPIO_Descriptor::set_gpio_17(Channel&& value)
  {
    m_gpio_17 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_17() const -> Channel const& 
  {
    return m_gpio_17;
  }

  auto PIGPIO_Descriptor::get_gpio_17() -> Channel& 
  {
    return m_gpio_17;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_18(Channel const& value)
  {
    m_gpio_18 = value;
  }
  void PIGPIO_Descriptor::set_gpio_18(Channel&& value)
  {
    m_gpio_18 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_18() const -> Channel const& 
  {
    return m_gpio_18;
  }

  auto PIGPIO_Descriptor::get_gpio_18() -> Channel& 
  {
    return m_gpio_18;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_19(Channel const& value)
  {
    m_gpio_19 = value;
  }
  void PIGPIO_Descriptor::set_gpio_19(Channel&& value)
  {
    m_gpio_19 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_19() const -> Channel const& 
  {
    return m_gpio_19;
  }

  auto PIGPIO_Descriptor::get_gpio_19() -> Channel& 
  {
    return m_gpio_19;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_20(Channel const& value)
  {
    m_gpio_20 = value;
  }
  void PIGPIO_Descriptor::set_gpio_20(Channel&& value)
  {
    m_gpio_20 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_20() const -> Channel const& 
  {
    return m_gpio_20;
  }

  auto PIGPIO_Descriptor::get_gpio_20() -> Channel& 
  {
    return m_gpio_20;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_21(Channel const& value)
  {
    m_gpio_21 = value;
  }
  void PIGPIO_Descriptor::set_gpio_21(Channel&& value)
  {
    m_gpio_21 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_21() const -> Channel const& 
  {
    return m_gpio_21;
  }

  auto PIGPIO_Descriptor::get_gpio_21() -> Channel& 
  {
    return m_gpio_21;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_22(Channel const& value)
  {
    m_gpio_22 = value;
  }
  void PIGPIO_Descriptor::set_gpio_22(Channel&& value)
  {
    m_gpio_22 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_22() const -> Channel const& 
  {
    return m_gpio_22;
  }

  auto PIGPIO_Descriptor::get_gpio_22() -> Channel& 
  {
    return m_gpio_22;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_23(Channel const& value)
  {
    m_gpio_23 = value;
  }
  void PIGPIO_Descriptor::set_gpio_23(Channel&& value)
  {
    m_gpio_23 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_23() const -> Channel const& 
  {
    return m_gpio_23;
  }

  auto PIGPIO_Descriptor::get_gpio_23() -> Channel& 
  {
    return m_gpio_23;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_24(Channel const& value)
  {
    m_gpio_24 = value;
  }
  void PIGPIO_Descriptor::set_gpio_24(Channel&& value)
  {
    m_gpio_24 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_24() const -> Channel const& 
  {
    return m_gpio_24;
  }

  auto PIGPIO_Descriptor::get_gpio_24() -> Channel& 
  {
    return m_gpio_24;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_25(Channel const& value)
  {
    m_gpio_25 = value;
  }
  void PIGPIO_Descriptor::set_gpio_25(Channel&& value)
  {
    m_gpio_25 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_25() const -> Channel const& 
  {
    return m_gpio_25;
  }

  auto PIGPIO_Descriptor::get_gpio_25() -> Channel& 
  {
    return m_gpio_25;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_26(Channel const& value)
  {
    m_gpio_26 = value;
  }
  void PIGPIO_Descriptor::set_gpio_26(Channel&& value)
  {
    m_gpio_26 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_26() const -> Channel const& 
  {
    return m_gpio_26;
  }

  auto PIGPIO_Descriptor::get_gpio_26() -> Channel& 
  {
    return m_gpio_26;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Descriptor::set_gpio_27(Channel const& value)
  {
    m_gpio_27 = value;
  }
  void PIGPIO_Descriptor::set_gpio_27(Channel&& value)
  {
    m_gpio_27 = std::move(value);
  }
  auto PIGPIO_Descriptor::get_gpio_27() const -> Channel const& 
  {
    return m_gpio_27;
  }

  auto PIGPIO_Descriptor::get_gpio_27() -> Channel& 
  {
    return m_gpio_27;
  }

////////////////////////////////////////////////////////////
    void PIGPIO_Config::Servo_Channel::set_min(servo_signal_t const& value)
    {
      m_min = clamp(value, servo_signal_t(1.000000f), servo_signal_t(2.000000f));
    }
    void PIGPIO_Config::Servo_Channel::set_min(servo_signal_t&& value)
    {
      m_min = clamp(std::move(value), servo_signal_t(1.000000f), servo_signal_t(2.000000f));
    }
    auto PIGPIO_Config::Servo_Channel::get_min() const -> servo_signal_t const& 
    {
      return m_min;
    }

////////////////////////////////////////////////////////////
    void PIGPIO_Config::Servo_Channel::set_max(servo_signal_t const& value)
    {
      m_max = clamp(value, servo_signal_t(1.000000f), servo_signal_t(2.000000f));
    }
    void PIGPIO_Config::Servo_Channel::set_max(servo_signal_t&& value)
    {
      m_max = clamp(std::move(value), servo_signal_t(1.000000f), servo_signal_t(2.000000f));
    }
    auto PIGPIO_Config::Servo_Channel::get_max() const -> servo_signal_t const& 
    {
      return m_max;
    }

////////////////////////////////////////////////////////////
    void PIGPIO_Config::PWM_Channel::set_min(pwm_signal_t const& value)
    {
      m_min = clamp(value, pwm_signal_t(0), pwm_signal_t(1.000000f));
    }
    void PIGPIO_Config::PWM_Channel::set_min(pwm_signal_t&& value)
    {
      m_min = clamp(std::move(value), pwm_signal_t(0), pwm_signal_t(1.000000f));
    }
    auto PIGPIO_Config::PWM_Channel::get_min() const -> pwm_signal_t const& 
    {
      return m_min;
    }

////////////////////////////////////////////////////////////
    void PIGPIO_Config::PWM_Channel::set_max(pwm_signal_t const& value)
    {
      m_max = clamp(value, pwm_signal_t(0), pwm_signal_t(1.000000f));
    }
    void PIGPIO_Config::PWM_Channel::set_max(pwm_signal_t&& value)
    {
      m_max = clamp(std::move(value), pwm_signal_t(0), pwm_signal_t(1.000000f));
    }
    auto PIGPIO_Config::PWM_Channel::get_max() const -> pwm_signal_t const& 
    {
      return m_max;
    }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_2(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_2 = value;
  }
  void PIGPIO_Config::set_gpio_2(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_2 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_2() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_2;
  }

  auto PIGPIO_Config::get_gpio_2() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_2;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_3(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_3 = value;
  }
  void PIGPIO_Config::set_gpio_3(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_3 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_3() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_3;
  }

  auto PIGPIO_Config::get_gpio_3() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_3;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_4(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_4 = value;
  }
  void PIGPIO_Config::set_gpio_4(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_4 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_4() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_4;
  }

  auto PIGPIO_Config::get_gpio_4() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_4;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_5(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_5 = value;
  }
  void PIGPIO_Config::set_gpio_5(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_5 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_5() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_5;
  }

  auto PIGPIO_Config::get_gpio_5() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_5;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_6(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_6 = value;
  }
  void PIGPIO_Config::set_gpio_6(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_6 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_6() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_6;
  }

  auto PIGPIO_Config::get_gpio_6() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_6;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_7(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_7 = value;
  }
  void PIGPIO_Config::set_gpio_7(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_7 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_7() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_7;
  }

  auto PIGPIO_Config::get_gpio_7() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_7;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_8(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_8 = value;
  }
  void PIGPIO_Config::set_gpio_8(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_8 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_8() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_8;
  }

  auto PIGPIO_Config::get_gpio_8() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_8;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_9(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_9 = value;
  }
  void PIGPIO_Config::set_gpio_9(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_9 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_9() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_9;
  }

  auto PIGPIO_Config::get_gpio_9() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_9;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_10(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_10 = value;
  }
  void PIGPIO_Config::set_gpio_10(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_10 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_10() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_10;
  }

  auto PIGPIO_Config::get_gpio_10() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_10;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_11(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_11 = value;
  }
  void PIGPIO_Config::set_gpio_11(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_11 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_11() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_11;
  }

  auto PIGPIO_Config::get_gpio_11() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_11;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_12(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_12 = value;
  }
  void PIGPIO_Config::set_gpio_12(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_12 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_12() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_12;
  }

  auto PIGPIO_Config::get_gpio_12() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_12;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_13(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_13 = value;
  }
  void PIGPIO_Config::set_gpio_13(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_13 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_13() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_13;
  }

  auto PIGPIO_Config::get_gpio_13() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_13;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_14(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_14 = value;
  }
  void PIGPIO_Config::set_gpio_14(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_14 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_14() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_14;
  }

  auto PIGPIO_Config::get_gpio_14() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_14;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_15(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_15 = value;
  }
  void PIGPIO_Config::set_gpio_15(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_15 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_15() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_15;
  }

  auto PIGPIO_Config::get_gpio_15() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_15;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_16(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_16 = value;
  }
  void PIGPIO_Config::set_gpio_16(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_16 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_16() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_16;
  }

  auto PIGPIO_Config::get_gpio_16() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_16;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_17(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_17 = value;
  }
  void PIGPIO_Config::set_gpio_17(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_17 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_17() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_17;
  }

  auto PIGPIO_Config::get_gpio_17() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_17;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_18(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_18 = value;
  }
  void PIGPIO_Config::set_gpio_18(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_18 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_18() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_18;
  }

  auto PIGPIO_Config::get_gpio_18() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_18;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_19(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_19 = value;
  }
  void PIGPIO_Config::set_gpio_19(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_19 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_19() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_19;
  }

  auto PIGPIO_Config::get_gpio_19() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_19;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_20(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_20 = value;
  }
  void PIGPIO_Config::set_gpio_20(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_20 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_20() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_20;
  }

  auto PIGPIO_Config::get_gpio_20() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_20;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_21(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_21 = value;
  }
  void PIGPIO_Config::set_gpio_21(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_21 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_21() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_21;
  }

  auto PIGPIO_Config::get_gpio_21() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_21;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_22(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_22 = value;
  }
  void PIGPIO_Config::set_gpio_22(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_22 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_22() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_22;
  }

  auto PIGPIO_Config::get_gpio_22() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_22;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_23(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_23 = value;
  }
  void PIGPIO_Config::set_gpio_23(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_23 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_23() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_23;
  }

  auto PIGPIO_Config::get_gpio_23() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_23;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_24(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_24 = value;
  }
  void PIGPIO_Config::set_gpio_24(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_24 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_24() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_24;
  }

  auto PIGPIO_Config::get_gpio_24() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_24;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_25(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_25 = value;
  }
  void PIGPIO_Config::set_gpio_25(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_25 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_25() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_25;
  }

  auto PIGPIO_Config::get_gpio_25() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_25;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_26(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_26 = value;
  }
  void PIGPIO_Config::set_gpio_26(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_26 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_26() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_26;
  }

  auto PIGPIO_Config::get_gpio_26() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_26;
  }

////////////////////////////////////////////////////////////
  void PIGPIO_Config::set_gpio_27(Poly<PIGPIO_Config::IChannel> const& value)
  {
    m_gpio_27 = value;
  }
  void PIGPIO_Config::set_gpio_27(Poly<PIGPIO_Config::IChannel>&& value)
  {
    m_gpio_27 = std::move(value);
  }
  auto PIGPIO_Config::get_gpio_27() const -> Poly<PIGPIO_Config::IChannel> const& 
  {
    return m_gpio_27;
  }

  auto PIGPIO_Config::get_gpio_27() -> Poly<PIGPIO_Config::IChannel>& 
  {
    return m_gpio_27;
  }

////////////////////////////////////////////////////////////
  void Pressure_Velocity_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Pressure_Velocity_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Proximity_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Proximity_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void Proximity_Descriptor::set_channel_count(channel_count_t const& value)
  {
    m_channel_count = clamp(value, channel_count_t(1), channel_count_t(128));
  }
  void Proximity_Descriptor::set_channel_count(channel_count_t&& value)
  {
    m_channel_count = clamp(std::move(value), channel_count_t(1), channel_count_t(128));
  }
  auto Proximity_Descriptor::get_channel_count() const -> channel_count_t const& 
  {
    return m_channel_count;
  }

////////////////////////////////////////////////////////////
  void Rate_Controller_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Rate_Controller_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Rate_Controller_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
      void Rate_Controller_Config::Feedback::Separate_XY_PIDs::set_x_pid(PID_Controller_Descriptor const& value)
      {
        m_x_pid = value;
      }
      void Rate_Controller_Config::Feedback::Separate_XY_PIDs::set_x_pid(PID_Controller_Descriptor&& value)
      {
        m_x_pid = std::move(value);
      }
      auto Rate_Controller_Config::Feedback::Separate_XY_PIDs::get_x_pid() const -> PID_Controller_Descriptor const& 
      {
        return m_x_pid;
      }

      auto Rate_Controller_Config::Feedback::Separate_XY_PIDs::get_x_pid() -> PID_Controller_Descriptor& 
      {
        return m_x_pid;
      }

////////////////////////////////////////////////////////////
      void Rate_Controller_Config::Feedback::Separate_XY_PIDs::set_y_pid(PID_Controller_Descriptor const& value)
      {
        m_y_pid = value;
      }
      void Rate_Controller_Config::Feedback::Separate_XY_PIDs::set_y_pid(PID_Controller_Descriptor&& value)
      {
        m_y_pid = std::move(value);
      }
      auto Rate_Controller_Config::Feedback::Separate_XY_PIDs::get_y_pid() const -> PID_Controller_Descriptor const& 
      {
        return m_y_pid;
      }

      auto Rate_Controller_Config::Feedback::Separate_XY_PIDs::get_y_pid() -> PID_Controller_Descriptor& 
      {
        return m_y_pid;
      }

////////////////////////////////////////////////////////////
    void Rate_Controller_Config::Feedback::set_weight(muf_t const& value)
    {
      m_weight = clamp(value, muf_t(0), muf_t(1.000000f));
    }
    void Rate_Controller_Config::Feedback::set_weight(muf_t&& value)
    {
      m_weight = clamp(std::move(value), muf_t(0), muf_t(1.000000f));
    }
    auto Rate_Controller_Config::Feedback::get_weight() const -> muf_t const& 
    {
      return m_weight;
    }

////////////////////////////////////////////////////////////
    void Rate_Controller_Config::Feedback::set_xy_pids(boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const& value)
    {
      m_xy_pids = value;
    }
    void Rate_Controller_Config::Feedback::set_xy_pids(boost::variant<Combined_XY_PIDs,Separate_XY_PIDs>&& value)
    {
      m_xy_pids = std::move(value);
    }
    auto Rate_Controller_Config::Feedback::get_xy_pids() const -> boost::variant<Combined_XY_PIDs,Separate_XY_PIDs> const& 
    {
      return m_xy_pids;
    }

    auto Rate_Controller_Config::Feedback::get_xy_pids() -> boost::variant<Combined_XY_PIDs,Separate_XY_PIDs>& 
    {
      return m_xy_pids;
    }

////////////////////////////////////////////////////////////
    void Rate_Controller_Config::Feedback::set_z_pid(PID_Controller_Descriptor const& value)
    {
      m_z_pid = value;
    }
    void Rate_Controller_Config::Feedback::set_z_pid(PID_Controller_Descriptor&& value)
    {
      m_z_pid = std::move(value);
    }
    auto Rate_Controller_Config::Feedback::get_z_pid() const -> PID_Controller_Descriptor const& 
    {
      return m_z_pid;
    }

    auto Rate_Controller_Config::Feedback::get_z_pid() -> PID_Controller_Descriptor& 
    {
      return m_z_pid;
    }

////////////////////////////////////////////////////////////
    void Rate_Controller_Config::Feedforward::set_weight(muf_t const& value)
    {
      m_weight = clamp(value, muf_t(0), muf_t(1.000000f));
    }
    void Rate_Controller_Config::Feedforward::set_weight(muf_t&& value)
    {
      m_weight = clamp(std::move(value), muf_t(0), muf_t(1.000000f));
    }
    auto Rate_Controller_Config::Feedforward::get_weight() const -> muf_t const& 
    {
      return m_weight;
    }

////////////////////////////////////////////////////////////
  void Rate_Controller_Config::set_max_torque(torque_t const& value)
  {
    m_max_torque = max(value, torque_t(0.010000f));
  }
  void Rate_Controller_Config::set_max_torque(torque_t&& value)
  {
    m_max_torque = max(std::move(value), torque_t(0.010000f));
  }
  auto Rate_Controller_Config::get_max_torque() const -> torque_t const& 
  {
    return m_max_torque;
  }

////////////////////////////////////////////////////////////
  void Rate_Controller_Config::set_feedback(Feedback const& value)
  {
    m_feedback = value;
  }
  void Rate_Controller_Config::set_feedback(Feedback&& value)
  {
    m_feedback = std::move(value);
  }
  auto Rate_Controller_Config::get_feedback() const -> Feedback const& 
  {
    return m_feedback;
  }

  auto Rate_Controller_Config::get_feedback() -> Feedback& 
  {
    return m_feedback;
  }

////////////////////////////////////////////////////////////
  void Rate_Controller_Config::set_feedforward(Feedforward const& value)
  {
    m_feedforward = value;
  }
  void Rate_Controller_Config::set_feedforward(Feedforward&& value)
  {
    m_feedforward = std::move(value);
  }
  auto Rate_Controller_Config::get_feedforward() const -> Feedforward const& 
  {
    return m_feedforward;
  }

  auto Rate_Controller_Config::get_feedforward() -> Feedforward& 
  {
    return m_feedforward;
  }

////////////////////////////////////////////////////////////
    void Raspicam_Descriptor::Quality::set_resolution(resolution_t const& value)
    {
      m_resolution = clamp(value, resolution_t(8, 8), resolution_t(1980, 1080));
    }
    void Raspicam_Descriptor::Quality::set_resolution(resolution_t&& value)
    {
      m_resolution = clamp(std::move(value), resolution_t(8, 8), resolution_t(1980, 1080));
    }
    auto Raspicam_Descriptor::Quality::get_resolution() const -> resolution_t const& 
    {
      return m_resolution;
    }

////////////////////////////////////////////////////////////
    void Raspicam_Descriptor::Quality::set_bitrate(bitrate_t const& value)
    {
      m_bitrate = clamp(value, bitrate_t(10000), bitrate_t(32000000));
    }
    void Raspicam_Descriptor::Quality::set_bitrate(bitrate_t&& value)
    {
      m_bitrate = clamp(std::move(value), bitrate_t(10000), bitrate_t(32000000));
    }
    auto Raspicam_Descriptor::Quality::get_bitrate() const -> bitrate_t const& 
    {
      return m_bitrate;
    }

////////////////////////////////////////////////////////////
  void Raspicam_Descriptor::set_fps(fps_t const& value)
  {
    m_fps = clamp(value, fps_t(10), fps_t(120));
  }
  void Raspicam_Descriptor::set_fps(fps_t&& value)
  {
    m_fps = clamp(std::move(value), fps_t(10), fps_t(120));
  }
  auto Raspicam_Descriptor::get_fps() const -> fps_t const& 
  {
    return m_fps;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Descriptor::set_iframe_interval(iframe_interval_t const& value)
  {
    m_iframe_interval = clamp(value, iframe_interval_t(1), iframe_interval_t(4294967295LL));
  }
  void Raspicam_Descriptor::set_iframe_interval(iframe_interval_t&& value)
  {
    m_iframe_interval = clamp(std::move(value), iframe_interval_t(1), iframe_interval_t(4294967295LL));
  }
  auto Raspicam_Descriptor::get_iframe_interval() const -> iframe_interval_t const& 
  {
    return m_iframe_interval;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Descriptor::set_commands_rate(uint32_t const& value)
  {
    m_commands_rate = clamp(value, uint32_t(1), uint32_t(30));
  }
  void Raspicam_Descriptor::set_commands_rate(uint32_t&& value)
  {
    m_commands_rate = clamp(std::move(value), uint32_t(1), uint32_t(30));
  }
  auto Raspicam_Descriptor::get_commands_rate() const -> uint32_t const& 
  {
    return m_commands_rate;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Descriptor::set_streaming_low(Quality const& value)
  {
    m_streaming_low = value;
  }
  void Raspicam_Descriptor::set_streaming_low(Quality&& value)
  {
    m_streaming_low = std::move(value);
  }
  auto Raspicam_Descriptor::get_streaming_low() const -> Quality const& 
  {
    return m_streaming_low;
  }

  auto Raspicam_Descriptor::get_streaming_low() -> Quality& 
  {
    return m_streaming_low;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Descriptor::set_streaming_high(Quality const& value)
  {
    m_streaming_high = value;
  }
  void Raspicam_Descriptor::set_streaming_high(Quality&& value)
  {
    m_streaming_high = std::move(value);
  }
  auto Raspicam_Descriptor::get_streaming_high() const -> Quality const& 
  {
    return m_streaming_high;
  }

  auto Raspicam_Descriptor::get_streaming_high() -> Quality& 
  {
    return m_streaming_high;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Descriptor::set_recording(Quality const& value)
  {
    m_recording = value;
  }
  void Raspicam_Descriptor::set_recording(Quality&& value)
  {
    m_recording = std::move(value);
  }
  auto Raspicam_Descriptor::get_recording() const -> Quality const& 
  {
    return m_recording;
  }

  auto Raspicam_Descriptor::get_recording() -> Quality& 
  {
    return m_recording;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Config::set_iso(iso_t const& value)
  {
    m_iso = clamp(value, iso_t(0), iso_t(1600));
  }
  void Raspicam_Config::set_iso(iso_t&& value)
  {
    m_iso = clamp(std::move(value), iso_t(0), iso_t(1600));
  }
  auto Raspicam_Config::get_iso() const -> iso_t const& 
  {
    return m_iso;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Config::set_shutter_speed(shutter_speed_t const& value)
  {
    m_shutter_speed = clamp(value, shutter_speed_t(0), shutter_speed_t(1000.000000f));
  }
  void Raspicam_Config::set_shutter_speed(shutter_speed_t&& value)
  {
    m_shutter_speed = clamp(std::move(value), shutter_speed_t(0), shutter_speed_t(1000.000000f));
  }
  auto Raspicam_Config::get_shutter_speed() const -> shutter_speed_t const& 
  {
    return m_shutter_speed;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Config::set_ev(ev_t const& value)
  {
    m_ev = clamp(value, ev_t(-10), ev_t(10));
  }
  void Raspicam_Config::set_ev(ev_t&& value)
  {
    m_ev = clamp(std::move(value), ev_t(-10), ev_t(10));
  }
  auto Raspicam_Config::get_ev() const -> ev_t const& 
  {
    return m_ev;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Config::set_sharpness(sharpness_t const& value)
  {
    m_sharpness = clamp(value, sharpness_t(0), sharpness_t(100));
  }
  void Raspicam_Config::set_sharpness(sharpness_t&& value)
  {
    m_sharpness = clamp(std::move(value), sharpness_t(0), sharpness_t(100));
  }
  auto Raspicam_Config::get_sharpness() const -> sharpness_t const& 
  {
    return m_sharpness;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Config::set_contrast(contrast_t const& value)
  {
    m_contrast = clamp(value, contrast_t(0), contrast_t(100));
  }
  void Raspicam_Config::set_contrast(contrast_t&& value)
  {
    m_contrast = clamp(std::move(value), contrast_t(0), contrast_t(100));
  }
  auto Raspicam_Config::get_contrast() const -> contrast_t const& 
  {
    return m_contrast;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Config::set_brightness(brightness_t const& value)
  {
    m_brightness = clamp(value, brightness_t(0), brightness_t(100));
  }
  void Raspicam_Config::set_brightness(brightness_t&& value)
  {
    m_brightness = clamp(std::move(value), brightness_t(0), brightness_t(100));
  }
  auto Raspicam_Config::get_brightness() const -> brightness_t const& 
  {
    return m_brightness;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Config::set_saturation(saturation_t const& value)
  {
    m_saturation = clamp(value, saturation_t(0), saturation_t(100));
  }
  void Raspicam_Config::set_saturation(saturation_t&& value)
  {
    m_saturation = clamp(std::move(value), saturation_t(0), saturation_t(100));
  }
  auto Raspicam_Config::get_saturation() const -> saturation_t const& 
  {
    return m_saturation;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Config::set_awb_mode(awb_mode_t const& value)
  {
    m_awb_mode = value;
  }
  void Raspicam_Config::set_awb_mode(awb_mode_t&& value)
  {
    m_awb_mode = std::move(value);
  }
  auto Raspicam_Config::get_awb_mode() const -> awb_mode_t const& 
  {
    return m_awb_mode;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Config::set_hflip(bool const& value)
  {
    m_hflip = value;
  }
  void Raspicam_Config::set_hflip(bool&& value)
  {
    m_hflip = std::move(value);
  }
  auto Raspicam_Config::get_hflip() const -> bool const& 
  {
    return m_hflip;
  }

////////////////////////////////////////////////////////////
  void Raspicam_Config::set_vflip(bool const& value)
  {
    m_vflip = value;
  }
  void Raspicam_Config::set_vflip(bool&& value)
  {
    m_vflip = std::move(value);
  }
  auto Raspicam_Config::get_vflip() const -> bool const& 
  {
    return m_vflip;
  }

////////////////////////////////////////////////////////////
  void RC5T619_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  void RC5T619_Descriptor::set_bus(std::string&& value)
  {
    m_bus = std::move(value);
  }
  auto RC5T619_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }

////////////////////////////////////////////////////////////
  void RC5T619_Descriptor::set_adc0_rate(uint32_t const& value)
  {
    m_adc0_rate = clamp(value, uint32_t(1), uint32_t(200));
  }
  void RC5T619_Descriptor::set_adc0_rate(uint32_t&& value)
  {
    m_adc0_rate = clamp(std::move(value), uint32_t(1), uint32_t(200));
  }
  auto RC5T619_Descriptor::get_adc0_rate() const -> uint32_t const& 
  {
    return m_adc0_rate;
  }

////////////////////////////////////////////////////////////
  void RC5T619_Descriptor::set_adc1_rate(uint32_t const& value)
  {
    m_adc1_rate = clamp(value, uint32_t(1), uint32_t(200));
  }
  void RC5T619_Descriptor::set_adc1_rate(uint32_t&& value)
  {
    m_adc1_rate = clamp(std::move(value), uint32_t(1), uint32_t(200));
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
  void Resampler_Descriptor::set_input_rate(uint32_t&& value)
  {
    m_input_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Resampler_Descriptor::set_output_rate(uint32_t&& value)
  {
    m_output_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Resampler_Descriptor::get_output_rate() const -> uint32_t const& 
  {
    return m_output_rate;
  }

////////////////////////////////////////////////////////////
  void Resampler_Config::set_lpf(LPF_Config const& value)
  {
    m_lpf = value;
  }
  void Resampler_Config::set_lpf(LPF_Config&& value)
  {
    m_lpf = std::move(value);
  }
  auto Resampler_Config::get_lpf() const -> LPF_Config const& 
  {
    return m_lpf;
  }

  auto Resampler_Config::get_lpf() -> LPF_Config& 
  {
    return m_lpf;
  }

////////////////////////////////////////////////////////////
  void Scalar_Generator_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Scalar_Generator_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Scalar_Generator_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void Scalar_Generator_Config::set_value(float const& value)
  {
    m_value = value;
  }
  void Scalar_Generator_Config::set_value(float&& value)
  {
    m_value = std::move(value);
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
  void Servo_Gimbal_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void Servo_Gimbal_Descriptor::set_commands_rate(uint32_t&& value)
  {
    m_commands_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Servo_Gimbal_Descriptor::get_commands_rate() const -> uint32_t const& 
  {
    return m_commands_rate;
  }

////////////////////////////////////////////////////////////
    void Servo_Gimbal_Config::Channel::set_min_angle(euler_t const& value)
    {
      m_min_angle = clamp(value, euler_t(-180.000000f), euler_t(180.000000f));
    }
    void Servo_Gimbal_Config::Channel::set_min_angle(euler_t&& value)
    {
      m_min_angle = clamp(std::move(value), euler_t(-180.000000f), euler_t(180.000000f));
    }
    auto Servo_Gimbal_Config::Channel::get_min_angle() const -> euler_t const& 
    {
      return m_min_angle;
    }

////////////////////////////////////////////////////////////
    void Servo_Gimbal_Config::Channel::set_max_angle(euler_t const& value)
    {
      m_max_angle = clamp(value, euler_t(-180.000000f), euler_t(180.000000f));
    }
    void Servo_Gimbal_Config::Channel::set_max_angle(euler_t&& value)
    {
      m_max_angle = clamp(std::move(value), euler_t(-180.000000f), euler_t(180.000000f));
    }
    auto Servo_Gimbal_Config::Channel::get_max_angle() const -> euler_t const& 
    {
      return m_max_angle;
    }

////////////////////////////////////////////////////////////
    void Servo_Gimbal_Config::Channel::set_min_pwm(pwm_signal_t const& value)
    {
      m_min_pwm = clamp(value, pwm_signal_t(0), pwm_signal_t(1.000000f));
    }
    void Servo_Gimbal_Config::Channel::set_min_pwm(pwm_signal_t&& value)
    {
      m_min_pwm = clamp(std::move(value), pwm_signal_t(0), pwm_signal_t(1.000000f));
    }
    auto Servo_Gimbal_Config::Channel::get_min_pwm() const -> pwm_signal_t const& 
    {
      return m_min_pwm;
    }

////////////////////////////////////////////////////////////
    void Servo_Gimbal_Config::Channel::set_max_pwm(pwm_signal_t const& value)
    {
      m_max_pwm = clamp(value, pwm_signal_t(0), pwm_signal_t(1.000000f));
    }
    void Servo_Gimbal_Config::Channel::set_max_pwm(pwm_signal_t&& value)
    {
      m_max_pwm = clamp(std::move(value), pwm_signal_t(0), pwm_signal_t(1.000000f));
    }
    auto Servo_Gimbal_Config::Channel::get_max_pwm() const -> pwm_signal_t const& 
    {
      return m_max_pwm;
    }

////////////////////////////////////////////////////////////
  void Servo_Gimbal_Config::set_x_channel(Channel const& value)
  {
    m_x_channel = value;
  }
  void Servo_Gimbal_Config::set_x_channel(Channel&& value)
  {
    m_x_channel = std::move(value);
  }
  auto Servo_Gimbal_Config::get_x_channel() const -> Channel const& 
  {
    return m_x_channel;
  }

  auto Servo_Gimbal_Config::get_x_channel() -> Channel& 
  {
    return m_x_channel;
  }

////////////////////////////////////////////////////////////
  void Servo_Gimbal_Config::set_y_channel(Channel const& value)
  {
    m_y_channel = value;
  }
  void Servo_Gimbal_Config::set_y_channel(Channel&& value)
  {
    m_y_channel = std::move(value);
  }
  auto Servo_Gimbal_Config::get_y_channel() const -> Channel const& 
  {
    return m_y_channel;
  }

  auto Servo_Gimbal_Config::get_y_channel() -> Channel& 
  {
    return m_y_channel;
  }

////////////////////////////////////////////////////////////
  void Servo_Gimbal_Config::set_z_channel(Channel const& value)
  {
    m_z_channel = value;
  }
  void Servo_Gimbal_Config::set_z_channel(Channel&& value)
  {
    m_z_channel = std::move(value);
  }
  auto Servo_Gimbal_Config::get_z_channel() const -> Channel const& 
  {
    return m_z_channel;
  }

  auto Servo_Gimbal_Config::get_z_channel() -> Channel& 
  {
    return m_z_channel;
  }

////////////////////////////////////////////////////////////
  void SRF01_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  void SRF01_Descriptor::set_bus(std::string&& value)
  {
    m_bus = std::move(value);
  }
  auto SRF01_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }

////////////////////////////////////////////////////////////
  void SRF01_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10));
  }
  void SRF01_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10));
  }
  auto SRF01_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void SRF01_Config::set_direction(direction_t const& value)
  {
    m_direction = clamp(value, direction_t(-FLT_MAX, -FLT_MAX, -FLT_MAX), direction_t(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void SRF01_Config::set_direction(direction_t&& value)
  {
    m_direction = clamp(std::move(value), direction_t(-FLT_MAX, -FLT_MAX, -FLT_MAX), direction_t(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto SRF01_Config::get_direction() const -> direction_t const& 
  {
    return m_direction;
  }

////////////////////////////////////////////////////////////
  void SRF01_Config::set_max_distance(ufloat const& value)
  {
    m_max_distance = max(value, ufloat(0));
  }
  void SRF01_Config::set_max_distance(ufloat&& value)
  {
    m_max_distance = max(std::move(value), ufloat(0));
  }
  auto SRF01_Config::get_max_distance() const -> ufloat const& 
  {
    return m_max_distance;
  }

////////////////////////////////////////////////////////////
  void SRF01_Config::set_min_distance(ufloat const& value)
  {
    m_min_distance = max(value, ufloat(0));
  }
  void SRF01_Config::set_min_distance(ufloat&& value)
  {
    m_min_distance = max(std::move(value), ufloat(0));
  }
  auto SRF01_Config::get_min_distance() const -> ufloat const& 
  {
    return m_min_distance;
  }

////////////////////////////////////////////////////////////
  void SRF02_Descriptor::set_bus(std::string const& value)
  {
    m_bus = value;
  }
  void SRF02_Descriptor::set_bus(std::string&& value)
  {
    m_bus = std::move(value);
  }
  auto SRF02_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }

////////////////////////////////////////////////////////////
  void SRF02_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10));
  }
  void SRF02_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10));
  }
  auto SRF02_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void SRF02_Config::set_direction(direction_t const& value)
  {
    m_direction = clamp(value, direction_t(-FLT_MAX, -FLT_MAX, -FLT_MAX), direction_t(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void SRF02_Config::set_direction(direction_t&& value)
  {
    m_direction = clamp(std::move(value), direction_t(-FLT_MAX, -FLT_MAX, -FLT_MAX), direction_t(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto SRF02_Config::get_direction() const -> direction_t const& 
  {
    return m_direction;
  }

////////////////////////////////////////////////////////////
  void SRF02_Config::set_max_distance(ufloat const& value)
  {
    m_max_distance = max(value, ufloat(0));
  }
  void SRF02_Config::set_max_distance(ufloat&& value)
  {
    m_max_distance = max(std::move(value), ufloat(0));
  }
  auto SRF02_Config::get_max_distance() const -> ufloat const& 
  {
    return m_max_distance;
  }

////////////////////////////////////////////////////////////
  void SRF02_Config::set_min_distance(ufloat const& value)
  {
    m_min_distance = max(value, ufloat(0));
  }
  void SRF02_Config::set_min_distance(ufloat&& value)
  {
    m_min_distance = max(std::move(value), ufloat(0));
  }
  auto SRF02_Config::get_min_distance() const -> ufloat const& 
  {
    return m_min_distance;
  }

////////////////////////////////////////////////////////////
  void Throttle_To_PWM_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Throttle_To_PWM_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Throttle_To_PWM_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void Throttle_To_PWM_Descriptor::set_channel_count(channel_count_t const& value)
  {
    m_channel_count = clamp(value, channel_count_t(1), channel_count_t(128));
  }
  void Throttle_To_PWM_Descriptor::set_channel_count(channel_count_t&& value)
  {
    m_channel_count = clamp(std::move(value), channel_count_t(1), channel_count_t(128));
  }
  auto Throttle_To_PWM_Descriptor::get_channel_count() const -> channel_count_t const& 
  {
    return m_channel_count;
  }

////////////////////////////////////////////////////////////
  void Transformer_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void Transformer_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
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
  void UBLOX_Descriptor::set_bus(std::string&& value)
  {
    m_bus = std::move(value);
  }
  auto UBLOX_Descriptor::get_bus() const -> std::string const& 
  {
    return m_bus;
  }

////////////////////////////////////////////////////////////
  void UBLOX_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10));
  }
  void UBLOX_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10));
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
  void Vec3_Generator_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto Vec3_Generator_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void Vec3_Generator_Config::set_value(vec3f const& value)
  {
    m_value = clamp(value, vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  void Vec3_Generator_Config::set_value(vec3f&& value)
  {
    m_value = clamp(std::move(value), vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX), vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  }
  auto Vec3_Generator_Config::get_value() const -> vec3f const& 
  {
    return m_value;
  }

////////////////////////////////////////////////////////////
  void CPPM_Receiver_Descriptor::set_gpio(gpio_t const& value)
  {
    m_gpio = value;
  }
  void CPPM_Receiver_Descriptor::set_gpio(gpio_t&& value)
  {
    m_gpio = std::move(value);
  }
  auto CPPM_Receiver_Descriptor::get_gpio() const -> gpio_t const& 
  {
    return m_gpio;
  }

////////////////////////////////////////////////////////////
  void CPPM_Receiver_Descriptor::set_channel_count(uint32_t const& value)
  {
    m_channel_count = clamp(value, uint32_t(1), uint32_t(16));
  }
  void CPPM_Receiver_Descriptor::set_channel_count(uint32_t&& value)
  {
    m_channel_count = clamp(std::move(value), uint32_t(1), uint32_t(16));
  }
  auto CPPM_Receiver_Descriptor::get_channel_count() const -> uint32_t const& 
  {
    return m_channel_count;
  }

////////////////////////////////////////////////////////////
  void CPPM_Receiver_Descriptor::set_rate(uint32_t const& value)
  {
    m_rate = clamp(value, uint32_t(1), uint32_t(10000));
  }
  void CPPM_Receiver_Descriptor::set_rate(uint32_t&& value)
  {
    m_rate = clamp(std::move(value), uint32_t(1), uint32_t(10000));
  }
  auto CPPM_Receiver_Descriptor::get_rate() const -> uint32_t const& 
  {
    return m_rate;
  }

////////////////////////////////////////////////////////////
  void CPPM_Receiver_Config::set_inverted(bool const& value)
  {
    m_inverted = value;
  }
  void CPPM_Receiver_Config::set_inverted(bool&& value)
  {
    m_inverted = std::move(value);
  }
  auto CPPM_Receiver_Config::get_inverted() const -> bool const& 
  {
    return m_inverted;
  }

////////////////////////////////////////////////////////////
  void CPPM_Receiver_Config::set_frame_length(ufloat const& value)
  {
    m_frame_length = max(value, ufloat(0));
  }
  void CPPM_Receiver_Config::set_frame_length(ufloat&& value)
  {
    m_frame_length = max(std::move(value), ufloat(0));
  }
  auto CPPM_Receiver_Config::get_frame_length() const -> ufloat const& 
  {
    return m_frame_length;
  }

////////////////////////////////////////////////////////////
  void CPPM_Receiver_Config::set_gap_pulse_length(ufloat const& value)
  {
    m_gap_pulse_length = max(value, ufloat(0));
  }
  void CPPM_Receiver_Config::set_gap_pulse_length(ufloat&& value)
  {
    m_gap_pulse_length = max(std::move(value), ufloat(0));
  }
  auto CPPM_Receiver_Config::get_gap_pulse_length() const -> ufloat const& 
  {
    return m_gap_pulse_length;
  }

////////////////////////////////////////////////////////////
  void CPPM_Receiver_Config::set_min_pulse_length(ufloat const& value)
  {
    m_min_pulse_length = max(value, ufloat(0));
  }
  void CPPM_Receiver_Config::set_min_pulse_length(ufloat&& value)
  {
    m_min_pulse_length = max(std::move(value), ufloat(0));
  }
  auto CPPM_Receiver_Config::get_min_pulse_length() const -> ufloat const& 
  {
    return m_min_pulse_length;
  }

////////////////////////////////////////////////////////////
  void CPPM_Receiver_Config::set_max_pulse_length(ufloat const& value)
  {
    m_max_pulse_length = max(value, ufloat(0));
  }
  void CPPM_Receiver_Config::set_max_pulse_length(ufloat&& value)
  {
    m_max_pulse_length = max(std::move(value), ufloat(0));
  }
  auto CPPM_Receiver_Config::get_max_pulse_length() const -> ufloat const& 
  {
    return m_max_pulse_length;
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
ts::Result<void> deserialize(IUAV_Descriptor::Comms::Quality::rate_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  std::string const& key = sz_value.get_as_string();
  typedef IUAV_Descriptor::Comms::Quality::rate_t _etype;
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
ts::sz::Value serialize(IUAV_Descriptor::Comms::Quality::rate_t const& value)
{
  typedef IUAV_Descriptor::Comms::Quality::rate_t _etype;
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
ts::Result<void> deserialize(IUAV_Descriptor::Comms::Quality& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "mtu")
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
    else     if (sz_value.get_object_member_name(i) == "fec_k")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_fec_k())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_fec_k(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "fec_n")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_fec_n())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_fec_n(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(IUAV_Descriptor::Comms::Quality const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("mtu", serialize(value.get_mtu()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  sz_value.add_object_member("fec_k", serialize(value.get_fec_k()));
  sz_value.add_object_member("fec_n", serialize(value.get_fec_n()));
  return sz_value;
}
ts::Result<void> deserialize(IUAV_Descriptor::Comms& value, ts::sz::Value const& sz_value)
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
    else     if (sz_value.get_object_member_name(i) == "low")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_low())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_low(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "high")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_high())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_high(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(IUAV_Descriptor::Comms const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("channel", serialize(value.get_channel()));
  sz_value.add_object_member("tx_power", serialize(value.get_tx_power()));
  sz_value.add_object_member("low", serialize(value.get_low()));
  sz_value.add_object_member("high", serialize(value.get_high()));
  return sz_value;
}
ts::Result<void> deserialize(IUAV_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "name")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_name(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "comms")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_comms())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_comms(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "mass")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_mass())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_mass(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "moment_of_inertia")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_moment_of_inertia())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_moment_of_inertia(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(IUAV_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("comms", serialize(value.get_comms()));
  sz_value.add_object_member("mass", serialize(value.get_mass()));
  sz_value.add_object_member("moment_of_inertia", serialize(value.get_moment_of_inertia()));
  return sz_value;
}
ts::Result<void> deserialize(Poly<IUAV_Descriptor>& value, ts::sz::Value const& sz_value)
{
  if (sz_value.is_empty()) { value = Poly<IUAV_Descriptor>(); return ts::success; }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::Multirotor_Descriptor")
  {
    value = Poly<IUAV_Descriptor>(new Multirotor_Descriptor());
    return deserialize((Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Tri_Multirotor_Descriptor")
  {
    value = Poly<IUAV_Descriptor>(new Tri_Multirotor_Descriptor());
    return deserialize((Tri_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Quad_Multirotor_Descriptor")
  {
    value = Poly<IUAV_Descriptor>(new Quad_Multirotor_Descriptor());
    return deserialize((Quad_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Hexa_Multirotor_Descriptor")
  {
    value = Poly<IUAV_Descriptor>(new Hexa_Multirotor_Descriptor());
    return deserialize((Hexa_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Hexatri_Multirotor_Descriptor")
  {
    value = Poly<IUAV_Descriptor>(new Hexatri_Multirotor_Descriptor());
    return deserialize((Hexatri_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Octo_Multirotor_Descriptor")
  {
    value = Poly<IUAV_Descriptor>(new Octo_Multirotor_Descriptor());
    return deserialize((Octo_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Octaquad_Multirotor_Descriptor")
  {
    value = Poly<IUAV_Descriptor>(new Octaquad_Multirotor_Descriptor());
    return deserialize((Octaquad_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Custom_Multirotor_Descriptor")
  {
    value = Poly<IUAV_Descriptor>(new Custom_Multirotor_Descriptor());
    return deserialize((Custom_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Poly<IUAV_Descriptor> const& value)
{
  if (!value) { return ts::sz::Value(); }
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Tri_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Tri_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Tri_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Quad_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Quad_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Quad_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Hexa_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Hexa_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Hexa_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Hexatri_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Hexatri_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Hexatri_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Octo_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Octo_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Octo_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Octaquad_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Octaquad_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Octaquad_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Custom_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Custom_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Custom_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(IBus_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(IBus_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Poly<IBus_Descriptor>& value, ts::sz::Value const& sz_value)
{
  if (sz_value.is_empty()) { value = Poly<IBus_Descriptor>(); return ts::success; }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::UART_Linux_Descriptor")
  {
    value = Poly<IBus_Descriptor>(new UART_Linux_Descriptor());
    return deserialize((UART_Linux_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::UART_BB_Descriptor")
  {
    value = Poly<IBus_Descriptor>(new UART_BB_Descriptor());
    return deserialize((UART_BB_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::I2C_BCM_Descriptor")
  {
    value = Poly<IBus_Descriptor>(new I2C_BCM_Descriptor());
    return deserialize((I2C_BCM_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::I2C_Linux_Descriptor")
  {
    value = Poly<IBus_Descriptor>(new I2C_Linux_Descriptor());
    return deserialize((I2C_Linux_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::SPI_BCM_Descriptor")
  {
    value = Poly<IBus_Descriptor>(new SPI_BCM_Descriptor());
    return deserialize((SPI_BCM_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::SPI_Linux_Descriptor")
  {
    value = Poly<IBus_Descriptor>(new SPI_Linux_Descriptor());
    return deserialize((SPI_Linux_Descriptor&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Poly<IBus_Descriptor> const& value)
{
  if (!value) { return ts::sz::Value(); }
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(UART_Linux_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::UART_Linux_Descriptor");
    sz_value.add_object_member("value", serialize((UART_Linux_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(UART_BB_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::UART_BB_Descriptor");
    sz_value.add_object_member("value", serialize((UART_BB_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(I2C_BCM_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::I2C_BCM_Descriptor");
    sz_value.add_object_member("value", serialize((I2C_BCM_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(I2C_Linux_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::I2C_Linux_Descriptor");
    sz_value.add_object_member("value", serialize((I2C_Linux_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(SPI_BCM_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::SPI_BCM_Descriptor");
    sz_value.add_object_member("value", serialize((SPI_BCM_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(SPI_Linux_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::SPI_Linux_Descriptor");
    sz_value.add_object_member("value", serialize((SPI_Linux_Descriptor&)*value));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(INode_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(INode_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Poly<INode_Descriptor>& value, ts::sz::Value const& sz_value)
{
  if (sz_value.is_empty()) { value = Poly<INode_Descriptor>(); return ts::success; }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::ADC_Ammeter_Descriptor")
  {
    value = Poly<INode_Descriptor>(new ADC_Ammeter_Descriptor());
    return deserialize((ADC_Ammeter_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::ADC_Voltmeter_Descriptor")
  {
    value = Poly<INode_Descriptor>(new ADC_Voltmeter_Descriptor());
    return deserialize((ADC_Voltmeter_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::ADS1115_Descriptor")
  {
    value = Poly<INode_Descriptor>(new ADS1115_Descriptor());
    return deserialize((ADS1115_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::AVRADC_Descriptor")
  {
    value = Poly<INode_Descriptor>(new AVRADC_Descriptor());
    return deserialize((AVRADC_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Comp_AHRS_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Comp_AHRS_Descriptor());
    return deserialize((Comp_AHRS_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Combiner_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Combiner_Descriptor());
    return deserialize((Combiner_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Gravity_Filter_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Gravity_Filter_Descriptor());
    return deserialize((Gravity_Filter_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::KF_ECEF_Descriptor")
  {
    value = Poly<INode_Descriptor>(new KF_ECEF_Descriptor());
    return deserialize((KF_ECEF_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::ENU_Frame_System_Descriptor")
  {
    value = Poly<INode_Descriptor>(new ENU_Frame_System_Descriptor());
    return deserialize((ENU_Frame_System_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::LPF_Descriptor")
  {
    value = Poly<INode_Descriptor>(new LPF_Descriptor());
    return deserialize((LPF_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::MaxSonar_Descriptor")
  {
    value = Poly<INode_Descriptor>(new MaxSonar_Descriptor());
    return deserialize((MaxSonar_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Motor_Mixer_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Motor_Mixer_Descriptor());
    return deserialize((Motor_Mixer_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Quad_Multirotor_Motor_Mixer_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Quad_Multirotor_Motor_Mixer_Descriptor());
    return deserialize((Quad_Multirotor_Motor_Mixer_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::MPU9250_Descriptor")
  {
    value = Poly<INode_Descriptor>(new MPU9250_Descriptor());
    return deserialize((MPU9250_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::MS5611_Descriptor")
  {
    value = Poly<INode_Descriptor>(new MS5611_Descriptor());
    return deserialize((MS5611_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::UltimateSensorFusion_Descriptor")
  {
    value = Poly<INode_Descriptor>(new UltimateSensorFusion_Descriptor());
    return deserialize((UltimateSensorFusion_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Brain_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Multirotor_Brain_Descriptor());
    return deserialize((Multirotor_Brain_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Pilot_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Multirotor_Pilot_Descriptor());
    return deserialize((Multirotor_Pilot_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Simulator_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Multirotor_Simulator_Descriptor());
    return deserialize((Multirotor_Simulator_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Oscillator_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Oscillator_Descriptor());
    return deserialize((Oscillator_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::PCA9685_Descriptor")
  {
    value = Poly<INode_Descriptor>(new PCA9685_Descriptor());
    return deserialize((PCA9685_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::PIGPIO_Descriptor")
  {
    value = Poly<INode_Descriptor>(new PIGPIO_Descriptor());
    return deserialize((PIGPIO_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Pressure_Velocity_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Pressure_Velocity_Descriptor());
    return deserialize((Pressure_Velocity_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Proximity_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Proximity_Descriptor());
    return deserialize((Proximity_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Rate_Controller_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Rate_Controller_Descriptor());
    return deserialize((Rate_Controller_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Raspicam_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Raspicam_Descriptor());
    return deserialize((Raspicam_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::RC5T619_Descriptor")
  {
    value = Poly<INode_Descriptor>(new RC5T619_Descriptor());
    return deserialize((RC5T619_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Resampler_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Resampler_Descriptor());
    return deserialize((Resampler_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Scalar_Generator_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Scalar_Generator_Descriptor());
    return deserialize((Scalar_Generator_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Servo_Gimbal_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Servo_Gimbal_Descriptor());
    return deserialize((Servo_Gimbal_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::SRF01_Descriptor")
  {
    value = Poly<INode_Descriptor>(new SRF01_Descriptor());
    return deserialize((SRF01_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::SRF02_Descriptor")
  {
    value = Poly<INode_Descriptor>(new SRF02_Descriptor());
    return deserialize((SRF02_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Throttle_To_PWM_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Throttle_To_PWM_Descriptor());
    return deserialize((Throttle_To_PWM_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Transformer_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Transformer_Descriptor());
    return deserialize((Transformer_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::UBLOX_Descriptor")
  {
    value = Poly<INode_Descriptor>(new UBLOX_Descriptor());
    return deserialize((UBLOX_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Vec3_Generator_Descriptor")
  {
    value = Poly<INode_Descriptor>(new Vec3_Generator_Descriptor());
    return deserialize((Vec3_Generator_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::CPPM_Receiver_Descriptor")
  {
    value = Poly<INode_Descriptor>(new CPPM_Receiver_Descriptor());
    return deserialize((CPPM_Receiver_Descriptor&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Poly<INode_Descriptor> const& value)
{
  if (!value) { return ts::sz::Value(); }
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(ADC_Ammeter_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ADC_Ammeter_Descriptor");
    sz_value.add_object_member("value", serialize((ADC_Ammeter_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(ADC_Voltmeter_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ADC_Voltmeter_Descriptor");
    sz_value.add_object_member("value", serialize((ADC_Voltmeter_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(ADS1115_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ADS1115_Descriptor");
    sz_value.add_object_member("value", serialize((ADS1115_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(AVRADC_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::AVRADC_Descriptor");
    sz_value.add_object_member("value", serialize((AVRADC_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Comp_AHRS_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Comp_AHRS_Descriptor");
    sz_value.add_object_member("value", serialize((Comp_AHRS_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Combiner_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Combiner_Descriptor");
    sz_value.add_object_member("value", serialize((Combiner_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Gravity_Filter_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Gravity_Filter_Descriptor");
    sz_value.add_object_member("value", serialize((Gravity_Filter_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(KF_ECEF_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::KF_ECEF_Descriptor");
    sz_value.add_object_member("value", serialize((KF_ECEF_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(ENU_Frame_System_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ENU_Frame_System_Descriptor");
    sz_value.add_object_member("value", serialize((ENU_Frame_System_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(LPF_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::LPF_Descriptor");
    sz_value.add_object_member("value", serialize((LPF_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(MaxSonar_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::MaxSonar_Descriptor");
    sz_value.add_object_member("value", serialize((MaxSonar_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Motor_Mixer_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Motor_Mixer_Descriptor");
    sz_value.add_object_member("value", serialize((Motor_Mixer_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Quad_Multirotor_Motor_Mixer_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Quad_Multirotor_Motor_Mixer_Descriptor");
    sz_value.add_object_member("value", serialize((Quad_Multirotor_Motor_Mixer_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(MPU9250_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::MPU9250_Descriptor");
    sz_value.add_object_member("value", serialize((MPU9250_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(MS5611_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::MS5611_Descriptor");
    sz_value.add_object_member("value", serialize((MS5611_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(UltimateSensorFusion_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::UltimateSensorFusion_Descriptor");
    sz_value.add_object_member("value", serialize((UltimateSensorFusion_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Brain_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Brain_Descriptor");
    sz_value.add_object_member("value", serialize((Multirotor_Brain_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Pilot_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Pilot_Descriptor");
    sz_value.add_object_member("value", serialize((Multirotor_Pilot_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Simulator_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Simulator_Descriptor");
    sz_value.add_object_member("value", serialize((Multirotor_Simulator_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Oscillator_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Oscillator_Descriptor");
    sz_value.add_object_member("value", serialize((Oscillator_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(PCA9685_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::PCA9685_Descriptor");
    sz_value.add_object_member("value", serialize((PCA9685_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(PIGPIO_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::PIGPIO_Descriptor");
    sz_value.add_object_member("value", serialize((PIGPIO_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Pressure_Velocity_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Pressure_Velocity_Descriptor");
    sz_value.add_object_member("value", serialize((Pressure_Velocity_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Proximity_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Proximity_Descriptor");
    sz_value.add_object_member("value", serialize((Proximity_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Rate_Controller_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Rate_Controller_Descriptor");
    sz_value.add_object_member("value", serialize((Rate_Controller_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Raspicam_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Raspicam_Descriptor");
    sz_value.add_object_member("value", serialize((Raspicam_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(RC5T619_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::RC5T619_Descriptor");
    sz_value.add_object_member("value", serialize((RC5T619_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Resampler_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Resampler_Descriptor");
    sz_value.add_object_member("value", serialize((Resampler_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Scalar_Generator_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Scalar_Generator_Descriptor");
    sz_value.add_object_member("value", serialize((Scalar_Generator_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Servo_Gimbal_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Servo_Gimbal_Descriptor");
    sz_value.add_object_member("value", serialize((Servo_Gimbal_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(SRF01_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::SRF01_Descriptor");
    sz_value.add_object_member("value", serialize((SRF01_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(SRF02_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::SRF02_Descriptor");
    sz_value.add_object_member("value", serialize((SRF02_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Throttle_To_PWM_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Throttle_To_PWM_Descriptor");
    sz_value.add_object_member("value", serialize((Throttle_To_PWM_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Transformer_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Transformer_Descriptor");
    sz_value.add_object_member("value", serialize((Transformer_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(UBLOX_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::UBLOX_Descriptor");
    sz_value.add_object_member("value", serialize((UBLOX_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Vec3_Generator_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Vec3_Generator_Descriptor");
    sz_value.add_object_member("value", serialize((Vec3_Generator_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(CPPM_Receiver_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::CPPM_Receiver_Descriptor");
    sz_value.add_object_member("value", serialize((CPPM_Receiver_Descriptor&)*value));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(INode_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(INode_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Poly<INode_Config>& value, ts::sz::Value const& sz_value)
{
  if (sz_value.is_empty()) { value = Poly<INode_Config>(); return ts::success; }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::ADC_Ammeter_Config")
  {
    value = Poly<INode_Config>(new ADC_Ammeter_Config());
    return deserialize((ADC_Ammeter_Config&)*value, *value_sz_value);
  }
  else if (path == "::ADC_Voltmeter_Config")
  {
    value = Poly<INode_Config>(new ADC_Voltmeter_Config());
    return deserialize((ADC_Voltmeter_Config&)*value, *value_sz_value);
  }
  else if (path == "::ADS1115_Config")
  {
    value = Poly<INode_Config>(new ADS1115_Config());
    return deserialize((ADS1115_Config&)*value, *value_sz_value);
  }
  else if (path == "::AVRADC_Config")
  {
    value = Poly<INode_Config>(new AVRADC_Config());
    return deserialize((AVRADC_Config&)*value, *value_sz_value);
  }
  else if (path == "::Comp_AHRS_Config")
  {
    value = Poly<INode_Config>(new Comp_AHRS_Config());
    return deserialize((Comp_AHRS_Config&)*value, *value_sz_value);
  }
  else if (path == "::Combiner_Config")
  {
    value = Poly<INode_Config>(new Combiner_Config());
    return deserialize((Combiner_Config&)*value, *value_sz_value);
  }
  else if (path == "::Gravity_Filter_Config")
  {
    value = Poly<INode_Config>(new Gravity_Filter_Config());
    return deserialize((Gravity_Filter_Config&)*value, *value_sz_value);
  }
  else if (path == "::KF_ECEF_Config")
  {
    value = Poly<INode_Config>(new KF_ECEF_Config());
    return deserialize((KF_ECEF_Config&)*value, *value_sz_value);
  }
  else if (path == "::ENU_Frame_System_Config")
  {
    value = Poly<INode_Config>(new ENU_Frame_System_Config());
    return deserialize((ENU_Frame_System_Config&)*value, *value_sz_value);
  }
  else if (path == "::LPF_Config")
  {
    value = Poly<INode_Config>(new LPF_Config());
    return deserialize((LPF_Config&)*value, *value_sz_value);
  }
  else if (path == "::MaxSonar_Config")
  {
    value = Poly<INode_Config>(new MaxSonar_Config());
    return deserialize((MaxSonar_Config&)*value, *value_sz_value);
  }
  else if (path == "::Motor_Mixer_Config")
  {
    value = Poly<INode_Config>(new Motor_Mixer_Config());
    return deserialize((Motor_Mixer_Config&)*value, *value_sz_value);
  }
  else if (path == "::Quad_Multirotor_Motor_Mixer_Config")
  {
    value = Poly<INode_Config>(new Quad_Multirotor_Motor_Mixer_Config());
    return deserialize((Quad_Multirotor_Motor_Mixer_Config&)*value, *value_sz_value);
  }
  else if (path == "::MPU9250_Config")
  {
    value = Poly<INode_Config>(new MPU9250_Config());
    return deserialize((MPU9250_Config&)*value, *value_sz_value);
  }
  else if (path == "::MS5611_Config")
  {
    value = Poly<INode_Config>(new MS5611_Config());
    return deserialize((MS5611_Config&)*value, *value_sz_value);
  }
  else if (path == "::UltimateSensorFusion_Config")
  {
    value = Poly<INode_Config>(new UltimateSensorFusion_Config());
    return deserialize((UltimateSensorFusion_Config&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Brain_Config")
  {
    value = Poly<INode_Config>(new Multirotor_Brain_Config());
    return deserialize((Multirotor_Brain_Config&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Pilot_Config")
  {
    value = Poly<INode_Config>(new Multirotor_Pilot_Config());
    return deserialize((Multirotor_Pilot_Config&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Simulator_Config")
  {
    value = Poly<INode_Config>(new Multirotor_Simulator_Config());
    return deserialize((Multirotor_Simulator_Config&)*value, *value_sz_value);
  }
  else if (path == "::Oscillator_Config")
  {
    value = Poly<INode_Config>(new Oscillator_Config());
    return deserialize((Oscillator_Config&)*value, *value_sz_value);
  }
  else if (path == "::PCA9685_Config")
  {
    value = Poly<INode_Config>(new PCA9685_Config());
    return deserialize((PCA9685_Config&)*value, *value_sz_value);
  }
  else if (path == "::PIGPIO_Config")
  {
    value = Poly<INode_Config>(new PIGPIO_Config());
    return deserialize((PIGPIO_Config&)*value, *value_sz_value);
  }
  else if (path == "::Pressure_Velocity_Config")
  {
    value = Poly<INode_Config>(new Pressure_Velocity_Config());
    return deserialize((Pressure_Velocity_Config&)*value, *value_sz_value);
  }
  else if (path == "::Proximity_Config")
  {
    value = Poly<INode_Config>(new Proximity_Config());
    return deserialize((Proximity_Config&)*value, *value_sz_value);
  }
  else if (path == "::Rate_Controller_Config")
  {
    value = Poly<INode_Config>(new Rate_Controller_Config());
    return deserialize((Rate_Controller_Config&)*value, *value_sz_value);
  }
  else if (path == "::Raspicam_Config")
  {
    value = Poly<INode_Config>(new Raspicam_Config());
    return deserialize((Raspicam_Config&)*value, *value_sz_value);
  }
  else if (path == "::RC5T619_Config")
  {
    value = Poly<INode_Config>(new RC5T619_Config());
    return deserialize((RC5T619_Config&)*value, *value_sz_value);
  }
  else if (path == "::Resampler_Config")
  {
    value = Poly<INode_Config>(new Resampler_Config());
    return deserialize((Resampler_Config&)*value, *value_sz_value);
  }
  else if (path == "::Scalar_Generator_Config")
  {
    value = Poly<INode_Config>(new Scalar_Generator_Config());
    return deserialize((Scalar_Generator_Config&)*value, *value_sz_value);
  }
  else if (path == "::Servo_Gimbal_Config")
  {
    value = Poly<INode_Config>(new Servo_Gimbal_Config());
    return deserialize((Servo_Gimbal_Config&)*value, *value_sz_value);
  }
  else if (path == "::SRF01_Config")
  {
    value = Poly<INode_Config>(new SRF01_Config());
    return deserialize((SRF01_Config&)*value, *value_sz_value);
  }
  else if (path == "::SRF02_Config")
  {
    value = Poly<INode_Config>(new SRF02_Config());
    return deserialize((SRF02_Config&)*value, *value_sz_value);
  }
  else if (path == "::Throttle_To_PWM_Config")
  {
    value = Poly<INode_Config>(new Throttle_To_PWM_Config());
    return deserialize((Throttle_To_PWM_Config&)*value, *value_sz_value);
  }
  else if (path == "::Transformer_Config")
  {
    value = Poly<INode_Config>(new Transformer_Config());
    return deserialize((Transformer_Config&)*value, *value_sz_value);
  }
  else if (path == "::UBLOX_Config")
  {
    value = Poly<INode_Config>(new UBLOX_Config());
    return deserialize((UBLOX_Config&)*value, *value_sz_value);
  }
  else if (path == "::Vec3_Generator_Config")
  {
    value = Poly<INode_Config>(new Vec3_Generator_Config());
    return deserialize((Vec3_Generator_Config&)*value, *value_sz_value);
  }
  else if (path == "::CPPM_Receiver_Config")
  {
    value = Poly<INode_Config>(new CPPM_Receiver_Config());
    return deserialize((CPPM_Receiver_Config&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Poly<INode_Config> const& value)
{
  if (!value) { return ts::sz::Value(); }
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(ADC_Ammeter_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ADC_Ammeter_Config");
    sz_value.add_object_member("value", serialize((ADC_Ammeter_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(ADC_Voltmeter_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ADC_Voltmeter_Config");
    sz_value.add_object_member("value", serialize((ADC_Voltmeter_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(ADS1115_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ADS1115_Config");
    sz_value.add_object_member("value", serialize((ADS1115_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(AVRADC_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::AVRADC_Config");
    sz_value.add_object_member("value", serialize((AVRADC_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Comp_AHRS_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Comp_AHRS_Config");
    sz_value.add_object_member("value", serialize((Comp_AHRS_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Combiner_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Combiner_Config");
    sz_value.add_object_member("value", serialize((Combiner_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Gravity_Filter_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Gravity_Filter_Config");
    sz_value.add_object_member("value", serialize((Gravity_Filter_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(KF_ECEF_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::KF_ECEF_Config");
    sz_value.add_object_member("value", serialize((KF_ECEF_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(ENU_Frame_System_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ENU_Frame_System_Config");
    sz_value.add_object_member("value", serialize((ENU_Frame_System_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(LPF_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::LPF_Config");
    sz_value.add_object_member("value", serialize((LPF_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(MaxSonar_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::MaxSonar_Config");
    sz_value.add_object_member("value", serialize((MaxSonar_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Motor_Mixer_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Motor_Mixer_Config");
    sz_value.add_object_member("value", serialize((Motor_Mixer_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Quad_Multirotor_Motor_Mixer_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Quad_Multirotor_Motor_Mixer_Config");
    sz_value.add_object_member("value", serialize((Quad_Multirotor_Motor_Mixer_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(MPU9250_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::MPU9250_Config");
    sz_value.add_object_member("value", serialize((MPU9250_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(MS5611_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::MS5611_Config");
    sz_value.add_object_member("value", serialize((MS5611_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(UltimateSensorFusion_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::UltimateSensorFusion_Config");
    sz_value.add_object_member("value", serialize((UltimateSensorFusion_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Brain_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Brain_Config");
    sz_value.add_object_member("value", serialize((Multirotor_Brain_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Pilot_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Pilot_Config");
    sz_value.add_object_member("value", serialize((Multirotor_Pilot_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Simulator_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Simulator_Config");
    sz_value.add_object_member("value", serialize((Multirotor_Simulator_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Oscillator_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Oscillator_Config");
    sz_value.add_object_member("value", serialize((Oscillator_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(PCA9685_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::PCA9685_Config");
    sz_value.add_object_member("value", serialize((PCA9685_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(PIGPIO_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::PIGPIO_Config");
    sz_value.add_object_member("value", serialize((PIGPIO_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Pressure_Velocity_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Pressure_Velocity_Config");
    sz_value.add_object_member("value", serialize((Pressure_Velocity_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Proximity_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Proximity_Config");
    sz_value.add_object_member("value", serialize((Proximity_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Rate_Controller_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Rate_Controller_Config");
    sz_value.add_object_member("value", serialize((Rate_Controller_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Raspicam_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Raspicam_Config");
    sz_value.add_object_member("value", serialize((Raspicam_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(RC5T619_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::RC5T619_Config");
    sz_value.add_object_member("value", serialize((RC5T619_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Resampler_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Resampler_Config");
    sz_value.add_object_member("value", serialize((Resampler_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Scalar_Generator_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Scalar_Generator_Config");
    sz_value.add_object_member("value", serialize((Scalar_Generator_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Servo_Gimbal_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Servo_Gimbal_Config");
    sz_value.add_object_member("value", serialize((Servo_Gimbal_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(SRF01_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::SRF01_Config");
    sz_value.add_object_member("value", serialize((SRF01_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(SRF02_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::SRF02_Config");
    sz_value.add_object_member("value", serialize((SRF02_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Throttle_To_PWM_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Throttle_To_PWM_Config");
    sz_value.add_object_member("value", serialize((Throttle_To_PWM_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Transformer_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Transformer_Config");
    sz_value.add_object_member("value", serialize((Transformer_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(UBLOX_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::UBLOX_Config");
    sz_value.add_object_member("value", serialize((UBLOX_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Vec3_Generator_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Vec3_Generator_Config");
    sz_value.add_object_member("value", serialize((Vec3_Generator_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(CPPM_Receiver_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::CPPM_Receiver_Config");
    sz_value.add_object_member("value", serialize((CPPM_Receiver_Config&)*value));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(Settings::Bus_Data& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "name")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_name(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "type")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_type())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_type(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "descriptor")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_descriptor())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_descriptor(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Settings::Bus_Data const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("type", serialize(value.get_type()));
  sz_value.add_object_member("descriptor", serialize(value.get_descriptor()));
  return sz_value;
}
ts::Result<void> deserialize(Settings::Node_Data& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "name")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_name(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "type")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_type())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_type(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "descriptor")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_descriptor())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_descriptor(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "config")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_config())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_config(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "input_paths")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_input_paths())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_input_paths(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Settings::Node_Data const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(5);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("type", serialize(value.get_type()));
  sz_value.add_object_member("descriptor", serialize(value.get_descriptor()));
  sz_value.add_object_member("config", serialize(value.get_config()));
  sz_value.add_object_member("input_paths", serialize(value.get_input_paths()));
  return sz_value;
}
ts::Result<void> deserialize(Settings& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "uav_descriptor")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_uav_descriptor())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_uav_descriptor(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "buses")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_buses())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_buses(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "nodes")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_nodes())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_nodes(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Settings const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("uav_descriptor", serialize(value.get_uav_descriptor()));
  sz_value.add_object_member("buses", serialize(value.get_buses()));
  sz_value.add_object_member("nodes", serialize(value.get_nodes()));
  return sz_value;
}
ts::Result<void> deserialize(Poly<const IBus_Descriptor>& value, ts::sz::Value const& sz_value)
{
  if (sz_value.is_empty()) { value = Poly<const IBus_Descriptor>(); return ts::success; }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::UART_Linux_Descriptor")
  {
    value = Poly<const IBus_Descriptor>(new UART_Linux_Descriptor());
    return deserialize((UART_Linux_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::UART_BB_Descriptor")
  {
    value = Poly<const IBus_Descriptor>(new UART_BB_Descriptor());
    return deserialize((UART_BB_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::I2C_BCM_Descriptor")
  {
    value = Poly<const IBus_Descriptor>(new I2C_BCM_Descriptor());
    return deserialize((I2C_BCM_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::I2C_Linux_Descriptor")
  {
    value = Poly<const IBus_Descriptor>(new I2C_Linux_Descriptor());
    return deserialize((I2C_Linux_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::SPI_BCM_Descriptor")
  {
    value = Poly<const IBus_Descriptor>(new SPI_BCM_Descriptor());
    return deserialize((SPI_BCM_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::SPI_Linux_Descriptor")
  {
    value = Poly<const IBus_Descriptor>(new SPI_Linux_Descriptor());
    return deserialize((SPI_Linux_Descriptor&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Poly<const IBus_Descriptor> const& value)
{
  if (!value) { return ts::sz::Value(); }
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(UART_Linux_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::UART_Linux_Descriptor");
    sz_value.add_object_member("value", serialize((UART_Linux_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(UART_BB_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::UART_BB_Descriptor");
    sz_value.add_object_member("value", serialize((UART_BB_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(I2C_BCM_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::I2C_BCM_Descriptor");
    sz_value.add_object_member("value", serialize((I2C_BCM_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(I2C_Linux_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::I2C_Linux_Descriptor");
    sz_value.add_object_member("value", serialize((I2C_Linux_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(SPI_BCM_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::SPI_BCM_Descriptor");
    sz_value.add_object_member("value", serialize((SPI_BCM_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(SPI_Linux_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::SPI_Linux_Descriptor");
    sz_value.add_object_member("value", serialize((SPI_Linux_Descriptor&)*value));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(Poly<const INode_Descriptor>& value, ts::sz::Value const& sz_value)
{
  if (sz_value.is_empty()) { value = Poly<const INode_Descriptor>(); return ts::success; }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::ADC_Ammeter_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new ADC_Ammeter_Descriptor());
    return deserialize((ADC_Ammeter_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::ADC_Voltmeter_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new ADC_Voltmeter_Descriptor());
    return deserialize((ADC_Voltmeter_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::ADS1115_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new ADS1115_Descriptor());
    return deserialize((ADS1115_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::AVRADC_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new AVRADC_Descriptor());
    return deserialize((AVRADC_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Comp_AHRS_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Comp_AHRS_Descriptor());
    return deserialize((Comp_AHRS_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Combiner_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Combiner_Descriptor());
    return deserialize((Combiner_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Gravity_Filter_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Gravity_Filter_Descriptor());
    return deserialize((Gravity_Filter_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::KF_ECEF_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new KF_ECEF_Descriptor());
    return deserialize((KF_ECEF_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::ENU_Frame_System_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new ENU_Frame_System_Descriptor());
    return deserialize((ENU_Frame_System_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::LPF_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new LPF_Descriptor());
    return deserialize((LPF_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::MaxSonar_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new MaxSonar_Descriptor());
    return deserialize((MaxSonar_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Motor_Mixer_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Motor_Mixer_Descriptor());
    return deserialize((Motor_Mixer_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Quad_Multirotor_Motor_Mixer_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Quad_Multirotor_Motor_Mixer_Descriptor());
    return deserialize((Quad_Multirotor_Motor_Mixer_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::MPU9250_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new MPU9250_Descriptor());
    return deserialize((MPU9250_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::MS5611_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new MS5611_Descriptor());
    return deserialize((MS5611_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::UltimateSensorFusion_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new UltimateSensorFusion_Descriptor());
    return deserialize((UltimateSensorFusion_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Brain_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Multirotor_Brain_Descriptor());
    return deserialize((Multirotor_Brain_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Pilot_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Multirotor_Pilot_Descriptor());
    return deserialize((Multirotor_Pilot_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Simulator_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Multirotor_Simulator_Descriptor());
    return deserialize((Multirotor_Simulator_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Oscillator_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Oscillator_Descriptor());
    return deserialize((Oscillator_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::PCA9685_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new PCA9685_Descriptor());
    return deserialize((PCA9685_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::PIGPIO_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new PIGPIO_Descriptor());
    return deserialize((PIGPIO_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Pressure_Velocity_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Pressure_Velocity_Descriptor());
    return deserialize((Pressure_Velocity_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Proximity_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Proximity_Descriptor());
    return deserialize((Proximity_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Rate_Controller_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Rate_Controller_Descriptor());
    return deserialize((Rate_Controller_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Raspicam_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Raspicam_Descriptor());
    return deserialize((Raspicam_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::RC5T619_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new RC5T619_Descriptor());
    return deserialize((RC5T619_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Resampler_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Resampler_Descriptor());
    return deserialize((Resampler_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Scalar_Generator_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Scalar_Generator_Descriptor());
    return deserialize((Scalar_Generator_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Servo_Gimbal_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Servo_Gimbal_Descriptor());
    return deserialize((Servo_Gimbal_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::SRF01_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new SRF01_Descriptor());
    return deserialize((SRF01_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::SRF02_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new SRF02_Descriptor());
    return deserialize((SRF02_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Throttle_To_PWM_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Throttle_To_PWM_Descriptor());
    return deserialize((Throttle_To_PWM_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Transformer_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Transformer_Descriptor());
    return deserialize((Transformer_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::UBLOX_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new UBLOX_Descriptor());
    return deserialize((UBLOX_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Vec3_Generator_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new Vec3_Generator_Descriptor());
    return deserialize((Vec3_Generator_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::CPPM_Receiver_Descriptor")
  {
    value = Poly<const INode_Descriptor>(new CPPM_Receiver_Descriptor());
    return deserialize((CPPM_Receiver_Descriptor&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Poly<const INode_Descriptor> const& value)
{
  if (!value) { return ts::sz::Value(); }
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(ADC_Ammeter_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ADC_Ammeter_Descriptor");
    sz_value.add_object_member("value", serialize((ADC_Ammeter_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(ADC_Voltmeter_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ADC_Voltmeter_Descriptor");
    sz_value.add_object_member("value", serialize((ADC_Voltmeter_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(ADS1115_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ADS1115_Descriptor");
    sz_value.add_object_member("value", serialize((ADS1115_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(AVRADC_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::AVRADC_Descriptor");
    sz_value.add_object_member("value", serialize((AVRADC_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Comp_AHRS_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Comp_AHRS_Descriptor");
    sz_value.add_object_member("value", serialize((Comp_AHRS_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Combiner_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Combiner_Descriptor");
    sz_value.add_object_member("value", serialize((Combiner_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Gravity_Filter_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Gravity_Filter_Descriptor");
    sz_value.add_object_member("value", serialize((Gravity_Filter_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(KF_ECEF_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::KF_ECEF_Descriptor");
    sz_value.add_object_member("value", serialize((KF_ECEF_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(ENU_Frame_System_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ENU_Frame_System_Descriptor");
    sz_value.add_object_member("value", serialize((ENU_Frame_System_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(LPF_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::LPF_Descriptor");
    sz_value.add_object_member("value", serialize((LPF_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(MaxSonar_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::MaxSonar_Descriptor");
    sz_value.add_object_member("value", serialize((MaxSonar_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Motor_Mixer_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Motor_Mixer_Descriptor");
    sz_value.add_object_member("value", serialize((Motor_Mixer_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Quad_Multirotor_Motor_Mixer_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Quad_Multirotor_Motor_Mixer_Descriptor");
    sz_value.add_object_member("value", serialize((Quad_Multirotor_Motor_Mixer_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(MPU9250_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::MPU9250_Descriptor");
    sz_value.add_object_member("value", serialize((MPU9250_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(MS5611_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::MS5611_Descriptor");
    sz_value.add_object_member("value", serialize((MS5611_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(UltimateSensorFusion_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::UltimateSensorFusion_Descriptor");
    sz_value.add_object_member("value", serialize((UltimateSensorFusion_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Brain_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Brain_Descriptor");
    sz_value.add_object_member("value", serialize((Multirotor_Brain_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Pilot_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Pilot_Descriptor");
    sz_value.add_object_member("value", serialize((Multirotor_Pilot_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Simulator_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Simulator_Descriptor");
    sz_value.add_object_member("value", serialize((Multirotor_Simulator_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Oscillator_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Oscillator_Descriptor");
    sz_value.add_object_member("value", serialize((Oscillator_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(PCA9685_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::PCA9685_Descriptor");
    sz_value.add_object_member("value", serialize((PCA9685_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(PIGPIO_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::PIGPIO_Descriptor");
    sz_value.add_object_member("value", serialize((PIGPIO_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Pressure_Velocity_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Pressure_Velocity_Descriptor");
    sz_value.add_object_member("value", serialize((Pressure_Velocity_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Proximity_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Proximity_Descriptor");
    sz_value.add_object_member("value", serialize((Proximity_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Rate_Controller_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Rate_Controller_Descriptor");
    sz_value.add_object_member("value", serialize((Rate_Controller_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Raspicam_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Raspicam_Descriptor");
    sz_value.add_object_member("value", serialize((Raspicam_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(RC5T619_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::RC5T619_Descriptor");
    sz_value.add_object_member("value", serialize((RC5T619_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Resampler_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Resampler_Descriptor");
    sz_value.add_object_member("value", serialize((Resampler_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Scalar_Generator_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Scalar_Generator_Descriptor");
    sz_value.add_object_member("value", serialize((Scalar_Generator_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Servo_Gimbal_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Servo_Gimbal_Descriptor");
    sz_value.add_object_member("value", serialize((Servo_Gimbal_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(SRF01_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::SRF01_Descriptor");
    sz_value.add_object_member("value", serialize((SRF01_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(SRF02_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::SRF02_Descriptor");
    sz_value.add_object_member("value", serialize((SRF02_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Throttle_To_PWM_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Throttle_To_PWM_Descriptor");
    sz_value.add_object_member("value", serialize((Throttle_To_PWM_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Transformer_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Transformer_Descriptor");
    sz_value.add_object_member("value", serialize((Transformer_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(UBLOX_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::UBLOX_Descriptor");
    sz_value.add_object_member("value", serialize((UBLOX_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Vec3_Generator_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Vec3_Generator_Descriptor");
    sz_value.add_object_member("value", serialize((Vec3_Generator_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(CPPM_Receiver_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::CPPM_Receiver_Descriptor");
    sz_value.add_object_member("value", serialize((CPPM_Receiver_Descriptor&)*value));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(Poly<const INode_Config>& value, ts::sz::Value const& sz_value)
{
  if (sz_value.is_empty()) { value = Poly<const INode_Config>(); return ts::success; }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::ADC_Ammeter_Config")
  {
    value = Poly<const INode_Config>(new ADC_Ammeter_Config());
    return deserialize((ADC_Ammeter_Config&)*value, *value_sz_value);
  }
  else if (path == "::ADC_Voltmeter_Config")
  {
    value = Poly<const INode_Config>(new ADC_Voltmeter_Config());
    return deserialize((ADC_Voltmeter_Config&)*value, *value_sz_value);
  }
  else if (path == "::ADS1115_Config")
  {
    value = Poly<const INode_Config>(new ADS1115_Config());
    return deserialize((ADS1115_Config&)*value, *value_sz_value);
  }
  else if (path == "::AVRADC_Config")
  {
    value = Poly<const INode_Config>(new AVRADC_Config());
    return deserialize((AVRADC_Config&)*value, *value_sz_value);
  }
  else if (path == "::Comp_AHRS_Config")
  {
    value = Poly<const INode_Config>(new Comp_AHRS_Config());
    return deserialize((Comp_AHRS_Config&)*value, *value_sz_value);
  }
  else if (path == "::Combiner_Config")
  {
    value = Poly<const INode_Config>(new Combiner_Config());
    return deserialize((Combiner_Config&)*value, *value_sz_value);
  }
  else if (path == "::Gravity_Filter_Config")
  {
    value = Poly<const INode_Config>(new Gravity_Filter_Config());
    return deserialize((Gravity_Filter_Config&)*value, *value_sz_value);
  }
  else if (path == "::KF_ECEF_Config")
  {
    value = Poly<const INode_Config>(new KF_ECEF_Config());
    return deserialize((KF_ECEF_Config&)*value, *value_sz_value);
  }
  else if (path == "::ENU_Frame_System_Config")
  {
    value = Poly<const INode_Config>(new ENU_Frame_System_Config());
    return deserialize((ENU_Frame_System_Config&)*value, *value_sz_value);
  }
  else if (path == "::LPF_Config")
  {
    value = Poly<const INode_Config>(new LPF_Config());
    return deserialize((LPF_Config&)*value, *value_sz_value);
  }
  else if (path == "::MaxSonar_Config")
  {
    value = Poly<const INode_Config>(new MaxSonar_Config());
    return deserialize((MaxSonar_Config&)*value, *value_sz_value);
  }
  else if (path == "::Motor_Mixer_Config")
  {
    value = Poly<const INode_Config>(new Motor_Mixer_Config());
    return deserialize((Motor_Mixer_Config&)*value, *value_sz_value);
  }
  else if (path == "::Quad_Multirotor_Motor_Mixer_Config")
  {
    value = Poly<const INode_Config>(new Quad_Multirotor_Motor_Mixer_Config());
    return deserialize((Quad_Multirotor_Motor_Mixer_Config&)*value, *value_sz_value);
  }
  else if (path == "::MPU9250_Config")
  {
    value = Poly<const INode_Config>(new MPU9250_Config());
    return deserialize((MPU9250_Config&)*value, *value_sz_value);
  }
  else if (path == "::MS5611_Config")
  {
    value = Poly<const INode_Config>(new MS5611_Config());
    return deserialize((MS5611_Config&)*value, *value_sz_value);
  }
  else if (path == "::UltimateSensorFusion_Config")
  {
    value = Poly<const INode_Config>(new UltimateSensorFusion_Config());
    return deserialize((UltimateSensorFusion_Config&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Brain_Config")
  {
    value = Poly<const INode_Config>(new Multirotor_Brain_Config());
    return deserialize((Multirotor_Brain_Config&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Pilot_Config")
  {
    value = Poly<const INode_Config>(new Multirotor_Pilot_Config());
    return deserialize((Multirotor_Pilot_Config&)*value, *value_sz_value);
  }
  else if (path == "::Multirotor_Simulator_Config")
  {
    value = Poly<const INode_Config>(new Multirotor_Simulator_Config());
    return deserialize((Multirotor_Simulator_Config&)*value, *value_sz_value);
  }
  else if (path == "::Oscillator_Config")
  {
    value = Poly<const INode_Config>(new Oscillator_Config());
    return deserialize((Oscillator_Config&)*value, *value_sz_value);
  }
  else if (path == "::PCA9685_Config")
  {
    value = Poly<const INode_Config>(new PCA9685_Config());
    return deserialize((PCA9685_Config&)*value, *value_sz_value);
  }
  else if (path == "::PIGPIO_Config")
  {
    value = Poly<const INode_Config>(new PIGPIO_Config());
    return deserialize((PIGPIO_Config&)*value, *value_sz_value);
  }
  else if (path == "::Pressure_Velocity_Config")
  {
    value = Poly<const INode_Config>(new Pressure_Velocity_Config());
    return deserialize((Pressure_Velocity_Config&)*value, *value_sz_value);
  }
  else if (path == "::Proximity_Config")
  {
    value = Poly<const INode_Config>(new Proximity_Config());
    return deserialize((Proximity_Config&)*value, *value_sz_value);
  }
  else if (path == "::Rate_Controller_Config")
  {
    value = Poly<const INode_Config>(new Rate_Controller_Config());
    return deserialize((Rate_Controller_Config&)*value, *value_sz_value);
  }
  else if (path == "::Raspicam_Config")
  {
    value = Poly<const INode_Config>(new Raspicam_Config());
    return deserialize((Raspicam_Config&)*value, *value_sz_value);
  }
  else if (path == "::RC5T619_Config")
  {
    value = Poly<const INode_Config>(new RC5T619_Config());
    return deserialize((RC5T619_Config&)*value, *value_sz_value);
  }
  else if (path == "::Resampler_Config")
  {
    value = Poly<const INode_Config>(new Resampler_Config());
    return deserialize((Resampler_Config&)*value, *value_sz_value);
  }
  else if (path == "::Scalar_Generator_Config")
  {
    value = Poly<const INode_Config>(new Scalar_Generator_Config());
    return deserialize((Scalar_Generator_Config&)*value, *value_sz_value);
  }
  else if (path == "::Servo_Gimbal_Config")
  {
    value = Poly<const INode_Config>(new Servo_Gimbal_Config());
    return deserialize((Servo_Gimbal_Config&)*value, *value_sz_value);
  }
  else if (path == "::SRF01_Config")
  {
    value = Poly<const INode_Config>(new SRF01_Config());
    return deserialize((SRF01_Config&)*value, *value_sz_value);
  }
  else if (path == "::SRF02_Config")
  {
    value = Poly<const INode_Config>(new SRF02_Config());
    return deserialize((SRF02_Config&)*value, *value_sz_value);
  }
  else if (path == "::Throttle_To_PWM_Config")
  {
    value = Poly<const INode_Config>(new Throttle_To_PWM_Config());
    return deserialize((Throttle_To_PWM_Config&)*value, *value_sz_value);
  }
  else if (path == "::Transformer_Config")
  {
    value = Poly<const INode_Config>(new Transformer_Config());
    return deserialize((Transformer_Config&)*value, *value_sz_value);
  }
  else if (path == "::UBLOX_Config")
  {
    value = Poly<const INode_Config>(new UBLOX_Config());
    return deserialize((UBLOX_Config&)*value, *value_sz_value);
  }
  else if (path == "::Vec3_Generator_Config")
  {
    value = Poly<const INode_Config>(new Vec3_Generator_Config());
    return deserialize((Vec3_Generator_Config&)*value, *value_sz_value);
  }
  else if (path == "::CPPM_Receiver_Config")
  {
    value = Poly<const INode_Config>(new CPPM_Receiver_Config());
    return deserialize((CPPM_Receiver_Config&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Poly<const INode_Config> const& value)
{
  if (!value) { return ts::sz::Value(); }
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(ADC_Ammeter_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ADC_Ammeter_Config");
    sz_value.add_object_member("value", serialize((ADC_Ammeter_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(ADC_Voltmeter_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ADC_Voltmeter_Config");
    sz_value.add_object_member("value", serialize((ADC_Voltmeter_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(ADS1115_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ADS1115_Config");
    sz_value.add_object_member("value", serialize((ADS1115_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(AVRADC_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::AVRADC_Config");
    sz_value.add_object_member("value", serialize((AVRADC_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Comp_AHRS_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Comp_AHRS_Config");
    sz_value.add_object_member("value", serialize((Comp_AHRS_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Combiner_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Combiner_Config");
    sz_value.add_object_member("value", serialize((Combiner_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Gravity_Filter_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Gravity_Filter_Config");
    sz_value.add_object_member("value", serialize((Gravity_Filter_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(KF_ECEF_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::KF_ECEF_Config");
    sz_value.add_object_member("value", serialize((KF_ECEF_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(ENU_Frame_System_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::ENU_Frame_System_Config");
    sz_value.add_object_member("value", serialize((ENU_Frame_System_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(LPF_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::LPF_Config");
    sz_value.add_object_member("value", serialize((LPF_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(MaxSonar_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::MaxSonar_Config");
    sz_value.add_object_member("value", serialize((MaxSonar_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Motor_Mixer_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Motor_Mixer_Config");
    sz_value.add_object_member("value", serialize((Motor_Mixer_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Quad_Multirotor_Motor_Mixer_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Quad_Multirotor_Motor_Mixer_Config");
    sz_value.add_object_member("value", serialize((Quad_Multirotor_Motor_Mixer_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(MPU9250_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::MPU9250_Config");
    sz_value.add_object_member("value", serialize((MPU9250_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(MS5611_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::MS5611_Config");
    sz_value.add_object_member("value", serialize((MS5611_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(UltimateSensorFusion_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::UltimateSensorFusion_Config");
    sz_value.add_object_member("value", serialize((UltimateSensorFusion_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Brain_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Brain_Config");
    sz_value.add_object_member("value", serialize((Multirotor_Brain_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Pilot_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Pilot_Config");
    sz_value.add_object_member("value", serialize((Multirotor_Pilot_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Multirotor_Simulator_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Simulator_Config");
    sz_value.add_object_member("value", serialize((Multirotor_Simulator_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Oscillator_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Oscillator_Config");
    sz_value.add_object_member("value", serialize((Oscillator_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(PCA9685_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::PCA9685_Config");
    sz_value.add_object_member("value", serialize((PCA9685_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(PIGPIO_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::PIGPIO_Config");
    sz_value.add_object_member("value", serialize((PIGPIO_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Pressure_Velocity_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Pressure_Velocity_Config");
    sz_value.add_object_member("value", serialize((Pressure_Velocity_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Proximity_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Proximity_Config");
    sz_value.add_object_member("value", serialize((Proximity_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Rate_Controller_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Rate_Controller_Config");
    sz_value.add_object_member("value", serialize((Rate_Controller_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Raspicam_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Raspicam_Config");
    sz_value.add_object_member("value", serialize((Raspicam_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(RC5T619_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::RC5T619_Config");
    sz_value.add_object_member("value", serialize((RC5T619_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Resampler_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Resampler_Config");
    sz_value.add_object_member("value", serialize((Resampler_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Scalar_Generator_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Scalar_Generator_Config");
    sz_value.add_object_member("value", serialize((Scalar_Generator_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Servo_Gimbal_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Servo_Gimbal_Config");
    sz_value.add_object_member("value", serialize((Servo_Gimbal_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(SRF01_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::SRF01_Config");
    sz_value.add_object_member("value", serialize((SRF01_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(SRF02_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::SRF02_Config");
    sz_value.add_object_member("value", serialize((SRF02_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Throttle_To_PWM_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Throttle_To_PWM_Config");
    sz_value.add_object_member("value", serialize((Throttle_To_PWM_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Transformer_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Transformer_Config");
    sz_value.add_object_member("value", serialize((Transformer_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(UBLOX_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::UBLOX_Config");
    sz_value.add_object_member("value", serialize((UBLOX_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Vec3_Generator_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Vec3_Generator_Config");
    sz_value.add_object_member("value", serialize((Vec3_Generator_Config&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(CPPM_Receiver_Config))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::CPPM_Receiver_Config");
    sz_value.add_object_member("value", serialize((CPPM_Receiver_Config&)*value));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
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
ts::Result<void> deserialize(Poly<const IUAV_Descriptor>& value, ts::sz::Value const& sz_value)
{
  if (sz_value.is_empty()) { value = Poly<const IUAV_Descriptor>(); return ts::success; }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::Multirotor_Descriptor")
  {
    value = Poly<const IUAV_Descriptor>(new Multirotor_Descriptor());
    return deserialize((Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Tri_Multirotor_Descriptor")
  {
    value = Poly<const IUAV_Descriptor>(new Tri_Multirotor_Descriptor());
    return deserialize((Tri_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Quad_Multirotor_Descriptor")
  {
    value = Poly<const IUAV_Descriptor>(new Quad_Multirotor_Descriptor());
    return deserialize((Quad_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Hexa_Multirotor_Descriptor")
  {
    value = Poly<const IUAV_Descriptor>(new Hexa_Multirotor_Descriptor());
    return deserialize((Hexa_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Hexatri_Multirotor_Descriptor")
  {
    value = Poly<const IUAV_Descriptor>(new Hexatri_Multirotor_Descriptor());
    return deserialize((Hexatri_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Octo_Multirotor_Descriptor")
  {
    value = Poly<const IUAV_Descriptor>(new Octo_Multirotor_Descriptor());
    return deserialize((Octo_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Octaquad_Multirotor_Descriptor")
  {
    value = Poly<const IUAV_Descriptor>(new Octaquad_Multirotor_Descriptor());
    return deserialize((Octaquad_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else if (path == "::Custom_Multirotor_Descriptor")
  {
    value = Poly<const IUAV_Descriptor>(new Custom_Multirotor_Descriptor());
    return deserialize((Custom_Multirotor_Descriptor&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Poly<const IUAV_Descriptor> const& value)
{
  if (!value) { return ts::sz::Value(); }
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Tri_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Tri_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Tri_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Quad_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Quad_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Quad_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Hexa_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Hexa_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Hexa_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Hexatri_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Hexatri_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Hexatri_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Octo_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Octo_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Octo_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Octaquad_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Octaquad_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Octaquad_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(Custom_Multirotor_Descriptor))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Custom_Multirotor_Descriptor");
    sz_value.add_object_member("value", serialize((Custom_Multirotor_Descriptor&)*value));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(std::vector<Settings::Bus_Data>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<Settings::Bus_Data> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<Settings::Node_Data>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<Settings::Node_Data> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "name")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_name(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "comms")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_comms())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_comms(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "mass")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_mass())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_mass(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "moment_of_inertia")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_moment_of_inertia())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_moment_of_inertia(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "radius")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_radius())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_radius(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "height")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_height())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_height(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_z_torque")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_z_torque())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_z_torque(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_thrust")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_thrust())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_thrust(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_acceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_acceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_acceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_deceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_deceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_deceleration(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(10);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("comms", serialize(value.get_comms()));
  sz_value.add_object_member("mass", serialize(value.get_mass()));
  sz_value.add_object_member("moment_of_inertia", serialize(value.get_moment_of_inertia()));
  sz_value.add_object_member("radius", serialize(value.get_radius()));
  sz_value.add_object_member("height", serialize(value.get_height()));
  sz_value.add_object_member("motor_z_torque", serialize(value.get_motor_z_torque()));
  sz_value.add_object_member("motor_thrust", serialize(value.get_motor_thrust()));
  sz_value.add_object_member("motor_acceleration", serialize(value.get_motor_acceleration()));
  sz_value.add_object_member("motor_deceleration", serialize(value.get_motor_deceleration()));
  return sz_value;
}
ts::Result<void> deserialize(Tri_Multirotor_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "name")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_name(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "comms")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_comms())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_comms(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "mass")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_mass())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_mass(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "moment_of_inertia")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_moment_of_inertia())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_moment_of_inertia(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "radius")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_radius())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_radius(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "height")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_height())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_height(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_z_torque")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_z_torque())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_z_torque(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_thrust")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_thrust())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_thrust(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_acceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_acceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_acceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_deceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_deceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_deceleration(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Tri_Multirotor_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(10);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("comms", serialize(value.get_comms()));
  sz_value.add_object_member("mass", serialize(value.get_mass()));
  sz_value.add_object_member("moment_of_inertia", serialize(value.get_moment_of_inertia()));
  sz_value.add_object_member("radius", serialize(value.get_radius()));
  sz_value.add_object_member("height", serialize(value.get_height()));
  sz_value.add_object_member("motor_z_torque", serialize(value.get_motor_z_torque()));
  sz_value.add_object_member("motor_thrust", serialize(value.get_motor_thrust()));
  sz_value.add_object_member("motor_acceleration", serialize(value.get_motor_acceleration()));
  sz_value.add_object_member("motor_deceleration", serialize(value.get_motor_deceleration()));
  return sz_value;
}
ts::Result<void> deserialize(Quad_Multirotor_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "name")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_name(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "comms")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_comms())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_comms(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "mass")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_mass())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_mass(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "moment_of_inertia")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_moment_of_inertia())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_moment_of_inertia(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "radius")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_radius())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_radius(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "height")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_height())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_height(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_z_torque")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_z_torque())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_z_torque(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_thrust")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_thrust())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_thrust(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_acceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_acceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_acceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_deceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_deceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_deceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "plus_configuration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_plus_configuration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_plus_configuration(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Quad_Multirotor_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(11);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("comms", serialize(value.get_comms()));
  sz_value.add_object_member("mass", serialize(value.get_mass()));
  sz_value.add_object_member("moment_of_inertia", serialize(value.get_moment_of_inertia()));
  sz_value.add_object_member("radius", serialize(value.get_radius()));
  sz_value.add_object_member("height", serialize(value.get_height()));
  sz_value.add_object_member("motor_z_torque", serialize(value.get_motor_z_torque()));
  sz_value.add_object_member("motor_thrust", serialize(value.get_motor_thrust()));
  sz_value.add_object_member("motor_acceleration", serialize(value.get_motor_acceleration()));
  sz_value.add_object_member("motor_deceleration", serialize(value.get_motor_deceleration()));
  sz_value.add_object_member("plus_configuration", serialize(value.get_plus_configuration()));
  return sz_value;
}
ts::Result<void> deserialize(Hexa_Multirotor_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "name")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_name(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "comms")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_comms())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_comms(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "mass")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_mass())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_mass(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "moment_of_inertia")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_moment_of_inertia())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_moment_of_inertia(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "radius")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_radius())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_radius(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "height")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_height())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_height(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_z_torque")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_z_torque())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_z_torque(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_thrust")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_thrust())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_thrust(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_acceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_acceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_acceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_deceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_deceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_deceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "plus_configuration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_plus_configuration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_plus_configuration(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Hexa_Multirotor_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(11);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("comms", serialize(value.get_comms()));
  sz_value.add_object_member("mass", serialize(value.get_mass()));
  sz_value.add_object_member("moment_of_inertia", serialize(value.get_moment_of_inertia()));
  sz_value.add_object_member("radius", serialize(value.get_radius()));
  sz_value.add_object_member("height", serialize(value.get_height()));
  sz_value.add_object_member("motor_z_torque", serialize(value.get_motor_z_torque()));
  sz_value.add_object_member("motor_thrust", serialize(value.get_motor_thrust()));
  sz_value.add_object_member("motor_acceleration", serialize(value.get_motor_acceleration()));
  sz_value.add_object_member("motor_deceleration", serialize(value.get_motor_deceleration()));
  sz_value.add_object_member("plus_configuration", serialize(value.get_plus_configuration()));
  return sz_value;
}
ts::Result<void> deserialize(Hexatri_Multirotor_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "name")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_name(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "comms")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_comms())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_comms(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "mass")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_mass())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_mass(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "moment_of_inertia")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_moment_of_inertia())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_moment_of_inertia(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "radius")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_radius())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_radius(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "height")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_height())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_height(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_z_torque")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_z_torque())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_z_torque(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_thrust")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_thrust())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_thrust(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_acceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_acceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_acceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_deceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_deceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_deceleration(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Hexatri_Multirotor_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(10);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("comms", serialize(value.get_comms()));
  sz_value.add_object_member("mass", serialize(value.get_mass()));
  sz_value.add_object_member("moment_of_inertia", serialize(value.get_moment_of_inertia()));
  sz_value.add_object_member("radius", serialize(value.get_radius()));
  sz_value.add_object_member("height", serialize(value.get_height()));
  sz_value.add_object_member("motor_z_torque", serialize(value.get_motor_z_torque()));
  sz_value.add_object_member("motor_thrust", serialize(value.get_motor_thrust()));
  sz_value.add_object_member("motor_acceleration", serialize(value.get_motor_acceleration()));
  sz_value.add_object_member("motor_deceleration", serialize(value.get_motor_deceleration()));
  return sz_value;
}
ts::Result<void> deserialize(Octo_Multirotor_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "name")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_name(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "comms")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_comms())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_comms(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "mass")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_mass())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_mass(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "moment_of_inertia")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_moment_of_inertia())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_moment_of_inertia(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "radius")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_radius())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_radius(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "height")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_height())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_height(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_z_torque")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_z_torque())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_z_torque(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_thrust")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_thrust())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_thrust(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_acceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_acceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_acceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_deceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_deceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_deceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "plus_configuration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_plus_configuration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_plus_configuration(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Octo_Multirotor_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(11);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("comms", serialize(value.get_comms()));
  sz_value.add_object_member("mass", serialize(value.get_mass()));
  sz_value.add_object_member("moment_of_inertia", serialize(value.get_moment_of_inertia()));
  sz_value.add_object_member("radius", serialize(value.get_radius()));
  sz_value.add_object_member("height", serialize(value.get_height()));
  sz_value.add_object_member("motor_z_torque", serialize(value.get_motor_z_torque()));
  sz_value.add_object_member("motor_thrust", serialize(value.get_motor_thrust()));
  sz_value.add_object_member("motor_acceleration", serialize(value.get_motor_acceleration()));
  sz_value.add_object_member("motor_deceleration", serialize(value.get_motor_deceleration()));
  sz_value.add_object_member("plus_configuration", serialize(value.get_plus_configuration()));
  return sz_value;
}
ts::Result<void> deserialize(Octaquad_Multirotor_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "name")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_name(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "comms")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_comms())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_comms(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "mass")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_mass())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_mass(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "moment_of_inertia")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_moment_of_inertia())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_moment_of_inertia(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "radius")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_radius())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_radius(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "height")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_height())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_height(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_z_torque")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_z_torque())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_z_torque(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_thrust")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_thrust())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_thrust(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_acceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_acceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_acceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_deceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_deceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_deceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "plus_configuration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_plus_configuration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_plus_configuration(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Octaquad_Multirotor_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(11);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("comms", serialize(value.get_comms()));
  sz_value.add_object_member("mass", serialize(value.get_mass()));
  sz_value.add_object_member("moment_of_inertia", serialize(value.get_moment_of_inertia()));
  sz_value.add_object_member("radius", serialize(value.get_radius()));
  sz_value.add_object_member("height", serialize(value.get_height()));
  sz_value.add_object_member("motor_z_torque", serialize(value.get_motor_z_torque()));
  sz_value.add_object_member("motor_thrust", serialize(value.get_motor_thrust()));
  sz_value.add_object_member("motor_acceleration", serialize(value.get_motor_acceleration()));
  sz_value.add_object_member("motor_deceleration", serialize(value.get_motor_deceleration()));
  sz_value.add_object_member("plus_configuration", serialize(value.get_plus_configuration()));
  return sz_value;
}
ts::Result<void> deserialize(Custom_Multirotor_Descriptor::Motor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "position")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_position())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_position(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "thrust_vector")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_thrust_vector())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_thrust_vector(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "clockwise")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_clockwise())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_clockwise(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Custom_Multirotor_Descriptor::Motor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("position", serialize(value.get_position()));
  sz_value.add_object_member("thrust_vector", serialize(value.get_thrust_vector()));
  sz_value.add_object_member("clockwise", serialize(value.get_clockwise()));
  return sz_value;
}
ts::Result<void> deserialize(Custom_Multirotor_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "name")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_name(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "comms")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_comms())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_comms(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "mass")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_mass())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_mass(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "moment_of_inertia")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_moment_of_inertia())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_moment_of_inertia(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "radius")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_radius())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_radius(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "height")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_height())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_height(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_z_torque")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_z_torque())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_z_torque(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_thrust")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_thrust())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_thrust(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_acceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_acceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_acceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motor_deceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motor_deceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motor_deceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "motors")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_motors())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_motors(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Custom_Multirotor_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(11);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("comms", serialize(value.get_comms()));
  sz_value.add_object_member("mass", serialize(value.get_mass()));
  sz_value.add_object_member("moment_of_inertia", serialize(value.get_moment_of_inertia()));
  sz_value.add_object_member("radius", serialize(value.get_radius()));
  sz_value.add_object_member("height", serialize(value.get_height()));
  sz_value.add_object_member("motor_z_torque", serialize(value.get_motor_z_torque()));
  sz_value.add_object_member("motor_thrust", serialize(value.get_motor_thrust()));
  sz_value.add_object_member("motor_acceleration", serialize(value.get_motor_acceleration()));
  sz_value.add_object_member("motor_deceleration", serialize(value.get_motor_deceleration()));
  sz_value.add_object_member("motors", serialize(value.get_motors()));
  return sz_value;
}
ts::Result<void> deserialize(std::vector<Custom_Multirotor_Descriptor::Motor>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<Custom_Multirotor_Descriptor::Motor> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(UART_Linux_Descriptor::baud_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  std::string const& key = sz_value.get_as_string();
  typedef UART_Linux_Descriptor::baud_t _etype;
  static std::map<std::string, _etype> s_map = {
    { "_9600", _etype::_9600 },
    { "_19200", _etype::_19200 },
    { "_38400", _etype::_38400 },
    { "_57600", _etype::_57600 },
    { "_115200", _etype::_115200 },
    { "_230400", _etype::_230400 },
  };
  auto it = s_map.find(key);
  if (it == s_map.end()) { return ts::Error("Cannot find item " + key + " when deserializing"); }
  value = it->second;
  return ts::success;
}
ts::sz::Value serialize(UART_Linux_Descriptor::baud_t const& value)
{
  typedef UART_Linux_Descriptor::baud_t _etype;
  static std::map<_etype, std::string> s_map = {
    { _etype::_9600, "_9600" },
    { _etype::_19200, "_19200" },
    { _etype::_38400, "_38400" },
    { _etype::_57600, "_57600" },
    { _etype::_115200, "_115200" },
    { _etype::_230400, "_230400" },
  };
  auto it = s_map.find(value);
  if (it == s_map.end()) { TS_ASSERT(false); return ts::sz::Value(); }
  return ts::sz::Value(it->second);
}
ts::Result<void> deserialize(UART_Linux_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "dev")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_dev())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_dev(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "baud")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_baud())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_baud(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(UART_Linux_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("dev", serialize(value.get_dev()));
  sz_value.add_object_member("baud", serialize(value.get_baud()));
  return sz_value;
}
ts::Result<void> deserialize(UART_BB_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rx_pin")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rx_pin())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rx_pin(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "baud")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_baud())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_baud(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "invert")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_invert())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_invert(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(UART_BB_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("rx_pin", serialize(value.get_rx_pin()));
  sz_value.add_object_member("baud", serialize(value.get_baud()));
  sz_value.add_object_member("invert", serialize(value.get_invert()));
  return sz_value;
}
ts::Result<void> deserialize(I2C_BCM_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "dev")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_dev())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_dev(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "baud")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_baud())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_baud(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(I2C_BCM_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("dev", serialize(value.get_dev()));
  sz_value.add_object_member("baud", serialize(value.get_baud()));
  return sz_value;
}
ts::Result<void> deserialize(I2C_Linux_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "dev")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_dev())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_dev(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(I2C_Linux_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("dev", serialize(value.get_dev()));
  return sz_value;
}
ts::Result<void> deserialize(SPI_BCM_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "dev")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_dev())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_dev(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "baud")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_baud())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_baud(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "mode")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_mode())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_mode(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "speed")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_speed())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_speed(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(SPI_BCM_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("dev", serialize(value.get_dev()));
  sz_value.add_object_member("baud", serialize(value.get_baud()));
  sz_value.add_object_member("mode", serialize(value.get_mode()));
  sz_value.add_object_member("speed", serialize(value.get_speed()));
  return sz_value;
}
ts::Result<void> deserialize(SPI_Linux_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "dev")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_dev())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_dev(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "speed")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_speed())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_speed(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(SPI_Linux_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("dev", serialize(value.get_dev()));
  sz_value.add_object_member("speed", serialize(value.get_speed()));
  return sz_value;
}
ts::Result<void> deserialize(P_Controller_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "kp")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_kp())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_kp(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(P_Controller_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("kp", serialize(value.get_kp()));
  return sz_value;
}
ts::Result<void> deserialize(PI_Controller_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "kp")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_kp())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_kp(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "ki")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_ki())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_ki(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max_i")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_i())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_i(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PI_Controller_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("kp", serialize(value.get_kp()));
  sz_value.add_object_member("ki", serialize(value.get_ki()));
  sz_value.add_object_member("max_i", serialize(value.get_max_i()));
  return sz_value;
}
ts::Result<void> deserialize(PD_Controller_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "kp")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_kp())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_kp(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "kd")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_kd())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_kd(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "d_filter")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_d_filter())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_d_filter(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PD_Controller_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("kp", serialize(value.get_kp()));
  sz_value.add_object_member("kd", serialize(value.get_kd()));
  sz_value.add_object_member("d_filter", serialize(value.get_d_filter()));
  return sz_value;
}
ts::Result<void> deserialize(PID_Controller_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "kp")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_kp())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_kp(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "kd")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_kd())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_kd(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "d_filter")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_d_filter())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_d_filter(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "ki")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_ki())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_ki(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max_i")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_i())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_i(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PID_Controller_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(5);
  sz_value.add_object_member("kp", serialize(value.get_kp()));
  sz_value.add_object_member("kd", serialize(value.get_kd()));
  sz_value.add_object_member("d_filter", serialize(value.get_d_filter()));
  sz_value.add_object_member("ki", serialize(value.get_ki()));
  sz_value.add_object_member("max_i", serialize(value.get_max_i()));
  return sz_value;
}
ts::Result<void> deserialize(ADC_Ammeter_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(ADC_Ammeter_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(ADC_Ammeter_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "scale")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_scale())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_scale(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "bias")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bias())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bias(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(ADC_Ammeter_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("scale", serialize(value.get_scale()));
  sz_value.add_object_member("bias", serialize(value.get_bias()));
  return sz_value;
}
ts::Result<void> deserialize(ADC_Voltmeter_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(ADC_Voltmeter_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(ADC_Voltmeter_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "scale")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_scale())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_scale(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "bias")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bias())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bias(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(ADC_Voltmeter_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("scale", serialize(value.get_scale()));
  sz_value.add_object_member("bias", serialize(value.get_bias()));
  return sz_value;
}
ts::Result<void> deserialize(ADS1115_Descriptor::ADC& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "is_enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_is_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_is_enabled(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(ADS1115_Descriptor::ADC const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("is_enabled", serialize(value.get_is_enabled()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(ADS1115_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "bus")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bus(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "i2c_address")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_i2c_address())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_i2c_address(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "adc0")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_adc0())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_adc0(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "adc1")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_adc1())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_adc1(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "adc2")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_adc2())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_adc2(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "adc3")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_adc3())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_adc3(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(ADS1115_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(6);
  sz_value.add_object_member("bus", serialize(value.get_bus()));
  sz_value.add_object_member("i2c_address", serialize(value.get_i2c_address()));
  sz_value.add_object_member("adc0", serialize(value.get_adc0()));
  sz_value.add_object_member("adc1", serialize(value.get_adc1()));
  sz_value.add_object_member("adc2", serialize(value.get_adc2()));
  sz_value.add_object_member("adc3", serialize(value.get_adc3()));
  return sz_value;
}
ts::Result<void> deserialize(ADS1115_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(ADS1115_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(AVRADC_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "bus")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bus(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "i2c_address")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_i2c_address())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_i2c_address(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(AVRADC_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("bus", serialize(value.get_bus()));
  sz_value.add_object_member("i2c_address", serialize(value.get_i2c_address()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(AVRADC_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(AVRADC_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Comp_AHRS_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Comp_AHRS_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Comp_AHRS_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "drift_correction_factor")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_drift_correction_factor())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_drift_correction_factor(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Comp_AHRS_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("drift_correction_factor", serialize(value.get_drift_correction_factor()));
  return sz_value;
}
ts::Result<void> deserialize(Combiner_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Combiner_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Combiner_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(Combiner_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Gravity_Filter_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Gravity_Filter_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Gravity_Filter_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(Gravity_Filter_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(KF_ECEF_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(KF_ECEF_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(KF_ECEF_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "gps_position_lag")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gps_position_lag())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gps_position_lag(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gps_position_accuracy")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gps_position_accuracy())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gps_position_accuracy(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gps_velocity_lag")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gps_velocity_lag())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gps_velocity_lag(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gps_velocity_accuracy")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gps_velocity_accuracy())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gps_velocity_accuracy(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "acceleration_lag")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_acceleration_lag())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_acceleration_lag(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "acceleration_accuracy")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_acceleration_accuracy())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_acceleration_accuracy(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(KF_ECEF_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(6);
  sz_value.add_object_member("gps_position_lag", serialize(value.get_gps_position_lag()));
  sz_value.add_object_member("gps_position_accuracy", serialize(value.get_gps_position_accuracy()));
  sz_value.add_object_member("gps_velocity_lag", serialize(value.get_gps_velocity_lag()));
  sz_value.add_object_member("gps_velocity_accuracy", serialize(value.get_gps_velocity_accuracy()));
  sz_value.add_object_member("acceleration_lag", serialize(value.get_acceleration_lag()));
  sz_value.add_object_member("acceleration_accuracy", serialize(value.get_acceleration_accuracy()));
  return sz_value;
}
ts::Result<void> deserialize(ENU_Frame_System_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(ENU_Frame_System_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(ENU_Frame_System_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(ENU_Frame_System_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(LPF_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(LPF_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(LPF_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "poles")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_poles())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_poles(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "cutoff_frequency")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_cutoff_frequency())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_cutoff_frequency(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(LPF_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("poles", serialize(value.get_poles()));
  sz_value.add_object_member("cutoff_frequency", serialize(value.get_cutoff_frequency()));
  return sz_value;
}
ts::Result<void> deserialize(MaxSonar_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "bus")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bus(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(MaxSonar_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("bus", serialize(value.get_bus()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(MaxSonar_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "direction")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_direction())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_direction(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max_distance")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_distance())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_distance(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "min_distance")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_min_distance())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_min_distance(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(MaxSonar_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("direction", serialize(value.get_direction()));
  sz_value.add_object_member("max_distance", serialize(value.get_max_distance()));
  sz_value.add_object_member("min_distance", serialize(value.get_min_distance()));
  return sz_value;
}
ts::Result<void> deserialize(Motor_Mixer_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Motor_Mixer_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Motor_Mixer_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "armed_thrust")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_armed_thrust())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_armed_thrust(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Motor_Mixer_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("armed_thrust", serialize(value.get_armed_thrust()));
  return sz_value;
}
ts::Result<void> deserialize(Quad_Multirotor_Motor_Mixer_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Quad_Multirotor_Motor_Mixer_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Quad_Multirotor_Motor_Mixer_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "armed_min_throttle")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_armed_min_throttle())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_armed_min_throttle(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Quad_Multirotor_Motor_Mixer_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("armed_min_throttle", serialize(value.get_armed_min_throttle()));
  return sz_value;
}
ts::Result<void> deserialize(Acceleration_Calibration_Point& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "temperature")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_temperature())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_temperature(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "bias")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bias())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bias(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "scale")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_scale())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_scale(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Acceleration_Calibration_Point const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("temperature", serialize(value.get_temperature()));
  sz_value.add_object_member("bias", serialize(value.get_bias()));
  sz_value.add_object_member("scale", serialize(value.get_scale()));
  return sz_value;
}
ts::Result<void> deserialize(Angular_Velocity_Calibration_Point& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "temperature")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_temperature())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_temperature(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "bias")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bias())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bias(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Angular_Velocity_Calibration_Point const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("temperature", serialize(value.get_temperature()));
  sz_value.add_object_member("bias", serialize(value.get_bias()));
  return sz_value;
}
ts::Result<void> deserialize(Magnetic_Field_Calibration_Point& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "temperature")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_temperature())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_temperature(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "bias")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bias())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bias(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "scale")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_scale())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_scale(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Magnetic_Field_Calibration_Point const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("temperature", serialize(value.get_temperature()));
  sz_value.add_object_member("bias", serialize(value.get_bias()));
  sz_value.add_object_member("scale", serialize(value.get_scale()));
  return sz_value;
}
ts::Result<void> deserialize(MPU9250_Descriptor::imu_rate_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  std::string const& key = sz_value.get_as_string();
  typedef MPU9250_Descriptor::imu_rate_t _etype;
  static std::map<std::string, _etype> s_map = {
    { "_250", _etype::_250 },
    { "_500", _etype::_500 },
    { "_1000", _etype::_1000 },
  };
  auto it = s_map.find(key);
  if (it == s_map.end()) { return ts::Error("Cannot find item " + key + " when deserializing"); }
  value = it->second;
  return ts::success;
}
ts::sz::Value serialize(MPU9250_Descriptor::imu_rate_t const& value)
{
  typedef MPU9250_Descriptor::imu_rate_t _etype;
  static std::map<_etype, std::string> s_map = {
    { _etype::_250, "_250" },
    { _etype::_500, "_500" },
    { _etype::_1000, "_1000" },
  };
  auto it = s_map.find(value);
  if (it == s_map.end()) { TS_ASSERT(false); return ts::sz::Value(); }
  return ts::sz::Value(it->second);
}
ts::Result<void> deserialize(MPU9250_Descriptor::acceleration_range_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  std::string const& key = sz_value.get_as_string();
  typedef MPU9250_Descriptor::acceleration_range_t _etype;
  static std::map<std::string, _etype> s_map = {
    { "_2", _etype::_2 },
    { "_4", _etype::_4 },
    { "_8", _etype::_8 },
    { "_16", _etype::_16 },
  };
  auto it = s_map.find(key);
  if (it == s_map.end()) { return ts::Error("Cannot find item " + key + " when deserializing"); }
  value = it->second;
  return ts::success;
}
ts::sz::Value serialize(MPU9250_Descriptor::acceleration_range_t const& value)
{
  typedef MPU9250_Descriptor::acceleration_range_t _etype;
  static std::map<_etype, std::string> s_map = {
    { _etype::_2, "_2" },
    { _etype::_4, "_4" },
    { _etype::_8, "_8" },
    { _etype::_16, "_16" },
  };
  auto it = s_map.find(value);
  if (it == s_map.end()) { TS_ASSERT(false); return ts::sz::Value(); }
  return ts::sz::Value(it->second);
}
ts::Result<void> deserialize(MPU9250_Descriptor::angular_velocity_range_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  std::string const& key = sz_value.get_as_string();
  typedef MPU9250_Descriptor::angular_velocity_range_t _etype;
  static std::map<std::string, _etype> s_map = {
    { "_250", _etype::_250 },
    { "_500", _etype::_500 },
    { "_1000", _etype::_1000 },
    { "_2000", _etype::_2000 },
  };
  auto it = s_map.find(key);
  if (it == s_map.end()) { return ts::Error("Cannot find item " + key + " when deserializing"); }
  value = it->second;
  return ts::success;
}
ts::sz::Value serialize(MPU9250_Descriptor::angular_velocity_range_t const& value)
{
  typedef MPU9250_Descriptor::angular_velocity_range_t _etype;
  static std::map<_etype, std::string> s_map = {
    { _etype::_250, "_250" },
    { _etype::_500, "_500" },
    { _etype::_1000, "_1000" },
    { _etype::_2000, "_2000" },
  };
  auto it = s_map.find(value);
  if (it == s_map.end()) { TS_ASSERT(false); return ts::sz::Value(); }
  return ts::sz::Value(it->second);
}
ts::Result<void> deserialize(MPU9250_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "bus")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bus(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "i2c_address")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_i2c_address())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_i2c_address(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "imu_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_imu_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_imu_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "magnetometer_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_magnetometer_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_magnetometer_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "thermometer_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_thermometer_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_thermometer_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "acceleration_range")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_acceleration_range())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_acceleration_range(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "angular_velocity_range")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_angular_velocity_range())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_angular_velocity_range(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(MPU9250_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(7);
  sz_value.add_object_member("bus", serialize(value.get_bus()));
  sz_value.add_object_member("i2c_address", serialize(value.get_i2c_address()));
  sz_value.add_object_member("imu_rate", serialize(value.get_imu_rate()));
  sz_value.add_object_member("magnetometer_rate", serialize(value.get_magnetometer_rate()));
  sz_value.add_object_member("thermometer_rate", serialize(value.get_thermometer_rate()));
  sz_value.add_object_member("acceleration_range", serialize(value.get_acceleration_range()));
  sz_value.add_object_member("angular_velocity_range", serialize(value.get_angular_velocity_range()));
  return sz_value;
}
ts::Result<void> deserialize(MPU9250_Config::Calibration& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "acceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_acceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_acceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "angular_velocity")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_angular_velocity())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_angular_velocity(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "magnetic_field")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_magnetic_field())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_magnetic_field(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(MPU9250_Config::Calibration const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("acceleration", serialize(value.get_acceleration()));
  sz_value.add_object_member("angular_velocity", serialize(value.get_angular_velocity()));
  sz_value.add_object_member("magnetic_field", serialize(value.get_magnetic_field()));
  return sz_value;
}
ts::Result<void> deserialize(MPU9250_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rotation")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rotation())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rotation(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "calibration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_calibration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_calibration(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(MPU9250_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("rotation", serialize(value.get_rotation()));
  sz_value.add_object_member("calibration", serialize(value.get_calibration()));
  return sz_value;
}
ts::Result<void> deserialize(std::vector<Acceleration_Calibration_Point>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<Acceleration_Calibration_Point> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<Angular_Velocity_Calibration_Point>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<Angular_Velocity_Calibration_Point> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<Magnetic_Field_Calibration_Point>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<Magnetic_Field_Calibration_Point> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(MS5611_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "bus")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bus(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "i2c_address")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_i2c_address())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_i2c_address(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "pressure_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_pressure_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_pressure_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "temperature_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_temperature_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_temperature_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(MS5611_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("bus", serialize(value.get_bus()));
  sz_value.add_object_member("i2c_address", serialize(value.get_i2c_address()));
  sz_value.add_object_member("pressure_rate", serialize(value.get_pressure_rate()));
  sz_value.add_object_member("temperature_rate", serialize(value.get_temperature_rate()));
  return sz_value;
}
ts::Result<void> deserialize(MS5611_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(MS5611_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(UltimateSensorFusion_Descriptor::imu_rate_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  std::string const& key = sz_value.get_as_string();
  typedef UltimateSensorFusion_Descriptor::imu_rate_t _etype;
  static std::map<std::string, _etype> s_map = {
    { "_250", _etype::_250 },
    { "_400", _etype::_400 },
    { "_500", _etype::_500 },
    { "_1000", _etype::_1000 },
  };
  auto it = s_map.find(key);
  if (it == s_map.end()) { return ts::Error("Cannot find item " + key + " when deserializing"); }
  value = it->second;
  return ts::success;
}
ts::sz::Value serialize(UltimateSensorFusion_Descriptor::imu_rate_t const& value)
{
  typedef UltimateSensorFusion_Descriptor::imu_rate_t _etype;
  static std::map<_etype, std::string> s_map = {
    { _etype::_250, "_250" },
    { _etype::_400, "_400" },
    { _etype::_500, "_500" },
    { _etype::_1000, "_1000" },
  };
  auto it = s_map.find(value);
  if (it == s_map.end()) { TS_ASSERT(false); return ts::sz::Value(); }
  return ts::sz::Value(it->second);
}
ts::Result<void> deserialize(UltimateSensorFusion_Descriptor::acceleration_range_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  std::string const& key = sz_value.get_as_string();
  typedef UltimateSensorFusion_Descriptor::acceleration_range_t _etype;
  static std::map<std::string, _etype> s_map = {
    { "_2", _etype::_2 },
    { "_4", _etype::_4 },
    { "_8", _etype::_8 },
    { "_16", _etype::_16 },
  };
  auto it = s_map.find(key);
  if (it == s_map.end()) { return ts::Error("Cannot find item " + key + " when deserializing"); }
  value = it->second;
  return ts::success;
}
ts::sz::Value serialize(UltimateSensorFusion_Descriptor::acceleration_range_t const& value)
{
  typedef UltimateSensorFusion_Descriptor::acceleration_range_t _etype;
  static std::map<_etype, std::string> s_map = {
    { _etype::_2, "_2" },
    { _etype::_4, "_4" },
    { _etype::_8, "_8" },
    { _etype::_16, "_16" },
  };
  auto it = s_map.find(value);
  if (it == s_map.end()) { TS_ASSERT(false); return ts::sz::Value(); }
  return ts::sz::Value(it->second);
}
ts::Result<void> deserialize(UltimateSensorFusion_Descriptor::angular_velocity_range_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  std::string const& key = sz_value.get_as_string();
  typedef UltimateSensorFusion_Descriptor::angular_velocity_range_t _etype;
  static std::map<std::string, _etype> s_map = {
    { "_250", _etype::_250 },
    { "_500", _etype::_500 },
    { "_1000", _etype::_1000 },
    { "_2000", _etype::_2000 },
  };
  auto it = s_map.find(key);
  if (it == s_map.end()) { return ts::Error("Cannot find item " + key + " when deserializing"); }
  value = it->second;
  return ts::success;
}
ts::sz::Value serialize(UltimateSensorFusion_Descriptor::angular_velocity_range_t const& value)
{
  typedef UltimateSensorFusion_Descriptor::angular_velocity_range_t _etype;
  static std::map<_etype, std::string> s_map = {
    { _etype::_250, "_250" },
    { _etype::_500, "_500" },
    { _etype::_1000, "_1000" },
    { _etype::_2000, "_2000" },
  };
  auto it = s_map.find(value);
  if (it == s_map.end()) { TS_ASSERT(false); return ts::sz::Value(); }
  return ts::sz::Value(it->second);
}
ts::Result<void> deserialize(UltimateSensorFusion_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "bus")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bus(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "i2c_address")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_i2c_address())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_i2c_address(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "imu_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_imu_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_imu_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "magnetometer_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_magnetometer_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_magnetometer_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "barometer_thermometer_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_barometer_thermometer_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_barometer_thermometer_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "acceleration_range")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_acceleration_range())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_acceleration_range(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "angular_velocity_range")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_angular_velocity_range())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_angular_velocity_range(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "frame_rate_divisor")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_frame_rate_divisor())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_frame_rate_divisor(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "imu_output_rate_divisor")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_imu_output_rate_divisor())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_imu_output_rate_divisor(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "acceleration_output_enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_acceleration_output_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_acceleration_output_enabled(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "angular_velocity_output_enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_angular_velocity_output_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_angular_velocity_output_enabled(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "magnetic_field_output_enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_magnetic_field_output_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_magnetic_field_output_enabled(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "frame_output_enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_frame_output_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_frame_output_enabled(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "pressure_output_enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_pressure_output_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_pressure_output_enabled(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "temperature_output_enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_temperature_output_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_temperature_output_enabled(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(UltimateSensorFusion_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(15);
  sz_value.add_object_member("bus", serialize(value.get_bus()));
  sz_value.add_object_member("i2c_address", serialize(value.get_i2c_address()));
  sz_value.add_object_member("imu_rate", serialize(value.get_imu_rate()));
  sz_value.add_object_member("magnetometer_rate", serialize(value.get_magnetometer_rate()));
  sz_value.add_object_member("barometer_thermometer_rate", serialize(value.get_barometer_thermometer_rate()));
  sz_value.add_object_member("acceleration_range", serialize(value.get_acceleration_range()));
  sz_value.add_object_member("angular_velocity_range", serialize(value.get_angular_velocity_range()));
  sz_value.add_object_member("frame_rate_divisor", serialize(value.get_frame_rate_divisor()));
  sz_value.add_object_member("imu_output_rate_divisor", serialize(value.get_imu_output_rate_divisor()));
  sz_value.add_object_member("acceleration_output_enabled", serialize(value.get_acceleration_output_enabled()));
  sz_value.add_object_member("angular_velocity_output_enabled", serialize(value.get_angular_velocity_output_enabled()));
  sz_value.add_object_member("magnetic_field_output_enabled", serialize(value.get_magnetic_field_output_enabled()));
  sz_value.add_object_member("frame_output_enabled", serialize(value.get_frame_output_enabled()));
  sz_value.add_object_member("pressure_output_enabled", serialize(value.get_pressure_output_enabled()));
  sz_value.add_object_member("temperature_output_enabled", serialize(value.get_temperature_output_enabled()));
  return sz_value;
}
ts::Result<void> deserialize(UltimateSensorFusion_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rotation")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rotation())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rotation(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(UltimateSensorFusion_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rotation", serialize(value.get_rotation()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Brain_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "commands_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_commands_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_commands_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "state_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_state_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_state_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Brain_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  sz_value.add_object_member("commands_rate", serialize(value.get_commands_rate()));
  sz_value.add_object_member("state_rate", serialize(value.get_state_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "x_pid")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_x_pid())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_x_pid(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "y_pid")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_y_pid())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_y_pid(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("x_pid", serialize(value.get_x_pid()));
  sz_value.add_object_member("y_pid", serialize(value.get_y_pid()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Brain_Config::Horizontal& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "max_rate_deg")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_rate_deg())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_rate_deg(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "angle_pids")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_angle_pids())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_angle_pids(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max_angle_deg")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_angle_deg())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_angle_deg(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max_speed")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_speed())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_speed(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "position_lpf")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_position_lpf())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_position_lpf(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "velocity_pid")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_velocity_pid())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_velocity_pid(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "position_pid")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_position_pid())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_position_pid(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Brain_Config::Horizontal const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(7);
  sz_value.add_object_member("max_rate_deg", serialize(value.get_max_rate_deg()));
  sz_value.add_object_member("angle_pids", serialize(value.get_angle_pids()));
  sz_value.add_object_member("max_angle_deg", serialize(value.get_max_angle_deg()));
  sz_value.add_object_member("max_speed", serialize(value.get_max_speed()));
  sz_value.add_object_member("position_lpf", serialize(value.get_position_lpf()));
  sz_value.add_object_member("velocity_pid", serialize(value.get_velocity_pid()));
  sz_value.add_object_member("position_pid", serialize(value.get_position_pid()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Brain_Config::Yaw& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "max_rate_deg")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_rate_deg())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_rate_deg(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "angle_pid")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_angle_pid())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_angle_pid(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Brain_Config::Yaw const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("max_rate_deg", serialize(value.get_max_rate_deg()));
  sz_value.add_object_member("angle_pid", serialize(value.get_angle_pid()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Brain_Config::Vertical& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "max_speed")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_speed())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_speed(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "altitude_lpf")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_altitude_lpf())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_altitude_lpf(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "speed_pid")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_speed_pid())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_speed_pid(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "altitude_pid")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_altitude_pid())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_altitude_pid(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Brain_Config::Vertical const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("max_speed", serialize(value.get_max_speed()));
  sz_value.add_object_member("altitude_lpf", serialize(value.get_altitude_lpf()));
  sz_value.add_object_member("speed_pid", serialize(value.get_speed_pid()));
  sz_value.add_object_member("altitude_pid", serialize(value.get_altitude_pid()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Brain_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "max_allowed_position_variation")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_allowed_position_variation())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_allowed_position_variation(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max_thrust")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_thrust())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_thrust(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "min_thrust")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_min_thrust())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_min_thrust(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "vertical")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_vertical())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_vertical(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "horizontal")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_horizontal())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_horizontal(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "yaw")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_yaw())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_yaw(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Brain_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(6);
  sz_value.add_object_member("max_allowed_position_variation", serialize(value.get_max_allowed_position_variation()));
  sz_value.add_object_member("max_thrust", serialize(value.get_max_thrust()));
  sz_value.add_object_member("min_thrust", serialize(value.get_min_thrust()));
  sz_value.add_object_member("vertical", serialize(value.get_vertical()));
  sz_value.add_object_member("horizontal", serialize(value.get_horizontal()));
  sz_value.add_object_member("yaw", serialize(value.get_yaw()));
  return sz_value;
}
ts::Result<void> deserialize(boost::variant<Multirotor_Brain_Config::Horizontal::Combined_Angle_PIDs,Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs>& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::Multirotor_Brain_Config::Horizontal::Combined_Angle_PIDs")
  {
    value = Multirotor_Brain_Config::Horizontal::Combined_Angle_PIDs();
    auto result = deserialize(boost::get<Multirotor_Brain_Config::Horizontal::Combined_Angle_PIDs>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs")
  {
    value = Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs();
    auto result = deserialize(boost::get<Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(boost::variant<Multirotor_Brain_Config::Horizontal::Combined_Angle_PIDs,Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (auto* v = boost::get<Multirotor_Brain_Config::Horizontal::Combined_Angle_PIDs>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Brain_Config::Horizontal::Combined_Angle_PIDs");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(Multirotor_Pilot_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "commands_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_commands_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_commands_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "state_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_state_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_state_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "video_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_video_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_video_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Pilot_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("commands_rate", serialize(value.get_commands_rate()));
  sz_value.add_object_member("state_rate", serialize(value.get_state_rate()));
  sz_value.add_object_member("video_rate", serialize(value.get_video_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Pilot_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Pilot_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulator_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "throttle_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_throttle_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_throttle_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "state_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_state_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_state_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "acceleration_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_acceleration_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_acceleration_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "angular_velocity_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_angular_velocity_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_angular_velocity_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "magnetic_field_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_magnetic_field_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_magnetic_field_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "pressure_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_pressure_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_pressure_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "temperature_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_temperature_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_temperature_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "distance_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_distance_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_distance_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gps_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gps_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gps_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulator_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(9);
  sz_value.add_object_member("throttle_rate", serialize(value.get_throttle_rate()));
  sz_value.add_object_member("state_rate", serialize(value.get_state_rate()));
  sz_value.add_object_member("acceleration_rate", serialize(value.get_acceleration_rate()));
  sz_value.add_object_member("angular_velocity_rate", serialize(value.get_angular_velocity_rate()));
  sz_value.add_object_member("magnetic_field_rate", serialize(value.get_magnetic_field_rate()));
  sz_value.add_object_member("pressure_rate", serialize(value.get_pressure_rate()));
  sz_value.add_object_member("temperature_rate", serialize(value.get_temperature_rate()));
  sz_value.add_object_member("distance_rate", serialize(value.get_distance_rate()));
  sz_value.add_object_member("gps_rate", serialize(value.get_gps_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulator_Config::Noise& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "gps_position")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gps_position())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gps_position(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gps_velocity")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gps_velocity())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gps_velocity(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gps_pacc")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gps_pacc())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gps_pacc(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gps_vacc")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gps_vacc())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gps_vacc(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "acceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_acceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_acceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "angular_velocity")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_angular_velocity())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_angular_velocity(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "magnetic_field")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_magnetic_field())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_magnetic_field(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "pressure")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_pressure())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_pressure(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "temperature")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_temperature())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_temperature(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "ground_distance")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_ground_distance())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_ground_distance(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulator_Config::Noise const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(10);
  sz_value.add_object_member("gps_position", serialize(value.get_gps_position()));
  sz_value.add_object_member("gps_velocity", serialize(value.get_gps_velocity()));
  sz_value.add_object_member("gps_pacc", serialize(value.get_gps_pacc()));
  sz_value.add_object_member("gps_vacc", serialize(value.get_gps_vacc()));
  sz_value.add_object_member("acceleration", serialize(value.get_acceleration()));
  sz_value.add_object_member("angular_velocity", serialize(value.get_angular_velocity()));
  sz_value.add_object_member("magnetic_field", serialize(value.get_magnetic_field()));
  sz_value.add_object_member("pressure", serialize(value.get_pressure()));
  sz_value.add_object_member("temperature", serialize(value.get_temperature()));
  sz_value.add_object_member("ground_distance", serialize(value.get_ground_distance()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulator_Config::Calibration& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "acceleration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_acceleration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_acceleration(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "angular_velocity")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_angular_velocity())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_angular_velocity(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "magnetic_field")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_magnetic_field())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_magnetic_field(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulator_Config::Calibration const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("acceleration", serialize(value.get_acceleration()));
  sz_value.add_object_member("angular_velocity", serialize(value.get_angular_velocity()));
  sz_value.add_object_member("magnetic_field", serialize(value.get_magnetic_field()));
  return sz_value;
}
ts::Result<void> deserialize(Multirotor_Simulator_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "simulation_enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_simulation_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_simulation_enabled(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "drag_enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_drag_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_drag_enabled(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "ground_enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_ground_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_ground_enabled(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gravity_enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gravity_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gravity_enabled(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "noise")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_noise())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_noise(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "calibration")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_calibration())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_calibration(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Multirotor_Simulator_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(6);
  sz_value.add_object_member("simulation_enabled", serialize(value.get_simulation_enabled()));
  sz_value.add_object_member("drag_enabled", serialize(value.get_drag_enabled()));
  sz_value.add_object_member("ground_enabled", serialize(value.get_ground_enabled()));
  sz_value.add_object_member("gravity_enabled", serialize(value.get_gravity_enabled()));
  sz_value.add_object_member("noise", serialize(value.get_noise()));
  sz_value.add_object_member("calibration", serialize(value.get_calibration()));
  return sz_value;
}
ts::Result<void> deserialize(Oscillator_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "component_count")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_component_count())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_component_count(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Oscillator_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  sz_value.add_object_member("component_count", serialize(value.get_component_count()));
  return sz_value;
}
ts::Result<void> deserialize(Oscillator_Config::Component& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "frequency")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_frequency())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_frequency(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "amplitude")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_amplitude())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_amplitude(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "square")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_square())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_square(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Oscillator_Config::Component const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("frequency", serialize(value.get_frequency()));
  sz_value.add_object_member("amplitude", serialize(value.get_amplitude()));
  sz_value.add_object_member("square", serialize(value.get_square()));
  return sz_value;
}
ts::Result<void> deserialize(Oscillator_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "amplitude")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_amplitude())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_amplitude(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "noise")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_noise())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_noise(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "components")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_components())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_components(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Oscillator_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("amplitude", serialize(value.get_amplitude()));
  sz_value.add_object_member("noise", serialize(value.get_noise()));
  sz_value.add_object_member("components", serialize(value.get_components()));
  return sz_value;
}
ts::Result<void> deserialize(std::vector<Oscillator_Config::Component>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<Oscillator_Config::Component> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(PCA9685_Descriptor::Channel& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_enabled(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "servo_signal")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_servo_signal())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_servo_signal(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PCA9685_Descriptor::Channel const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("enabled", serialize(value.get_enabled()));
  sz_value.add_object_member("servo_signal", serialize(value.get_servo_signal()));
  return sz_value;
}
ts::Result<void> deserialize(PCA9685_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "bus")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bus(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "address")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_address())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_address(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "channels")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_channels())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_channels(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PCA9685_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("bus", serialize(value.get_bus()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  sz_value.add_object_member("address", serialize(value.get_address()));
  sz_value.add_object_member("channels", serialize(value.get_channels()));
  return sz_value;
}
ts::Result<void> deserialize(std::vector<PCA9685_Descriptor::Channel>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<PCA9685_Descriptor::Channel> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(PCA9685_Config::IChannel& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(PCA9685_Config::IChannel const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(PCA9685_Config::Servo_Channel& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "min")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_min())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_min(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PCA9685_Config::Servo_Channel const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("min", serialize(value.get_min()));
  sz_value.add_object_member("max", serialize(value.get_max()));
  return sz_value;
}
ts::Result<void> deserialize(PCA9685_Config::PWM_Channel& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "min")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_min())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_min(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PCA9685_Config::PWM_Channel const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("min", serialize(value.get_min()));
  sz_value.add_object_member("max", serialize(value.get_max()));
  return sz_value;
}
ts::Result<void> deserialize(PCA9685_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "channels")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_channels())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_channels(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PCA9685_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("channels", serialize(value.get_channels()));
  return sz_value;
}
ts::Result<void> deserialize(Poly<PCA9685_Config::IChannel>& value, ts::sz::Value const& sz_value)
{
  if (sz_value.is_empty()) { value = Poly<PCA9685_Config::IChannel>(); return ts::success; }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::PCA9685_Config::Servo_Channel")
  {
    value = Poly<PCA9685_Config::IChannel>(new PCA9685_Config::Servo_Channel());
    return deserialize((PCA9685_Config::Servo_Channel&)*value, *value_sz_value);
  }
  else if (path == "::PCA9685_Config::PWM_Channel")
  {
    value = Poly<PCA9685_Config::IChannel>(new PCA9685_Config::PWM_Channel());
    return deserialize((PCA9685_Config::PWM_Channel&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Poly<PCA9685_Config::IChannel> const& value)
{
  if (!value) { return ts::sz::Value(); }
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(PCA9685_Config::Servo_Channel))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::PCA9685_Config::Servo_Channel");
    sz_value.add_object_member("value", serialize((PCA9685_Config::Servo_Channel&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(PCA9685_Config::PWM_Channel))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::PCA9685_Config::PWM_Channel");
    sz_value.add_object_member("value", serialize((PCA9685_Config::PWM_Channel&)*value));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(std::vector<Poly<PCA9685_Config::IChannel>>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<Poly<PCA9685_Config::IChannel>> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(PIGPIO_Descriptor::Channel& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "enabled")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_enabled())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_enabled(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "servo_signal")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_servo_signal())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_servo_signal(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PIGPIO_Descriptor::Channel const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("enabled", serialize(value.get_enabled()));
  sz_value.add_object_member("servo_signal", serialize(value.get_servo_signal()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(PIGPIO_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "gpio_2")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_2())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_2(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_3")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_3())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_3(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_4")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_4())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_4(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_5")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_5())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_5(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_6")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_6())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_6(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_7")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_7())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_7(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_8")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_8())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_8(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_9")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_9())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_9(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_10")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_10())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_10(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_11")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_11())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_11(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_12")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_12())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_12(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_13")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_13())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_13(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_14")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_14())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_14(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_15")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_15())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_15(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_16")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_16())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_16(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_17")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_17())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_17(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_18")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_18())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_18(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_19")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_19())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_19(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_20")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_20())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_20(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_21")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_21())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_21(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_22")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_22())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_22(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_23")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_23())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_23(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_24")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_24())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_24(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_25")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_25())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_25(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_26")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_26())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_26(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_27")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_27())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_27(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PIGPIO_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(26);
  sz_value.add_object_member("gpio_2", serialize(value.get_gpio_2()));
  sz_value.add_object_member("gpio_3", serialize(value.get_gpio_3()));
  sz_value.add_object_member("gpio_4", serialize(value.get_gpio_4()));
  sz_value.add_object_member("gpio_5", serialize(value.get_gpio_5()));
  sz_value.add_object_member("gpio_6", serialize(value.get_gpio_6()));
  sz_value.add_object_member("gpio_7", serialize(value.get_gpio_7()));
  sz_value.add_object_member("gpio_8", serialize(value.get_gpio_8()));
  sz_value.add_object_member("gpio_9", serialize(value.get_gpio_9()));
  sz_value.add_object_member("gpio_10", serialize(value.get_gpio_10()));
  sz_value.add_object_member("gpio_11", serialize(value.get_gpio_11()));
  sz_value.add_object_member("gpio_12", serialize(value.get_gpio_12()));
  sz_value.add_object_member("gpio_13", serialize(value.get_gpio_13()));
  sz_value.add_object_member("gpio_14", serialize(value.get_gpio_14()));
  sz_value.add_object_member("gpio_15", serialize(value.get_gpio_15()));
  sz_value.add_object_member("gpio_16", serialize(value.get_gpio_16()));
  sz_value.add_object_member("gpio_17", serialize(value.get_gpio_17()));
  sz_value.add_object_member("gpio_18", serialize(value.get_gpio_18()));
  sz_value.add_object_member("gpio_19", serialize(value.get_gpio_19()));
  sz_value.add_object_member("gpio_20", serialize(value.get_gpio_20()));
  sz_value.add_object_member("gpio_21", serialize(value.get_gpio_21()));
  sz_value.add_object_member("gpio_22", serialize(value.get_gpio_22()));
  sz_value.add_object_member("gpio_23", serialize(value.get_gpio_23()));
  sz_value.add_object_member("gpio_24", serialize(value.get_gpio_24()));
  sz_value.add_object_member("gpio_25", serialize(value.get_gpio_25()));
  sz_value.add_object_member("gpio_26", serialize(value.get_gpio_26()));
  sz_value.add_object_member("gpio_27", serialize(value.get_gpio_27()));
  return sz_value;
}
ts::Result<void> deserialize(PIGPIO_Config::IChannel& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(PIGPIO_Config::IChannel const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(PIGPIO_Config::Servo_Channel& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "min")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_min())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_min(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PIGPIO_Config::Servo_Channel const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("min", serialize(value.get_min()));
  sz_value.add_object_member("max", serialize(value.get_max()));
  return sz_value;
}
ts::Result<void> deserialize(PIGPIO_Config::PWM_Channel& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "min")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_min())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_min(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PIGPIO_Config::PWM_Channel const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("min", serialize(value.get_min()));
  sz_value.add_object_member("max", serialize(value.get_max()));
  return sz_value;
}
ts::Result<void> deserialize(PIGPIO_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "gpio_2")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_2())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_2(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_3")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_3())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_3(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_4")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_4())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_4(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_5")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_5())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_5(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_6")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_6())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_6(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_7")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_7())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_7(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_8")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_8())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_8(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_9")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_9())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_9(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_10")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_10())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_10(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_11")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_11())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_11(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_12")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_12())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_12(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_13")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_13())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_13(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_14")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_14())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_14(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_15")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_15())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_15(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_16")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_16())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_16(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_17")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_17())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_17(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_18")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_18())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_18(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_19")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_19())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_19(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_20")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_20())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_20(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_21")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_21())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_21(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_22")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_22())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_22(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_23")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_23())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_23(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_24")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_24())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_24(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_25")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_25())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_25(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_26")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_26())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_26(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gpio_27")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio_27())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio_27(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(PIGPIO_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(26);
  sz_value.add_object_member("gpio_2", serialize(value.get_gpio_2()));
  sz_value.add_object_member("gpio_3", serialize(value.get_gpio_3()));
  sz_value.add_object_member("gpio_4", serialize(value.get_gpio_4()));
  sz_value.add_object_member("gpio_5", serialize(value.get_gpio_5()));
  sz_value.add_object_member("gpio_6", serialize(value.get_gpio_6()));
  sz_value.add_object_member("gpio_7", serialize(value.get_gpio_7()));
  sz_value.add_object_member("gpio_8", serialize(value.get_gpio_8()));
  sz_value.add_object_member("gpio_9", serialize(value.get_gpio_9()));
  sz_value.add_object_member("gpio_10", serialize(value.get_gpio_10()));
  sz_value.add_object_member("gpio_11", serialize(value.get_gpio_11()));
  sz_value.add_object_member("gpio_12", serialize(value.get_gpio_12()));
  sz_value.add_object_member("gpio_13", serialize(value.get_gpio_13()));
  sz_value.add_object_member("gpio_14", serialize(value.get_gpio_14()));
  sz_value.add_object_member("gpio_15", serialize(value.get_gpio_15()));
  sz_value.add_object_member("gpio_16", serialize(value.get_gpio_16()));
  sz_value.add_object_member("gpio_17", serialize(value.get_gpio_17()));
  sz_value.add_object_member("gpio_18", serialize(value.get_gpio_18()));
  sz_value.add_object_member("gpio_19", serialize(value.get_gpio_19()));
  sz_value.add_object_member("gpio_20", serialize(value.get_gpio_20()));
  sz_value.add_object_member("gpio_21", serialize(value.get_gpio_21()));
  sz_value.add_object_member("gpio_22", serialize(value.get_gpio_22()));
  sz_value.add_object_member("gpio_23", serialize(value.get_gpio_23()));
  sz_value.add_object_member("gpio_24", serialize(value.get_gpio_24()));
  sz_value.add_object_member("gpio_25", serialize(value.get_gpio_25()));
  sz_value.add_object_member("gpio_26", serialize(value.get_gpio_26()));
  sz_value.add_object_member("gpio_27", serialize(value.get_gpio_27()));
  return sz_value;
}
ts::Result<void> deserialize(Poly<PIGPIO_Config::IChannel>& value, ts::sz::Value const& sz_value)
{
  if (sz_value.is_empty()) { value = Poly<PIGPIO_Config::IChannel>(); return ts::success; }
  if (!sz_value.is_object()) { return ts::Error("Expected object or null value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::PIGPIO_Config::Servo_Channel")
  {
    value = Poly<PIGPIO_Config::IChannel>(new PIGPIO_Config::Servo_Channel());
    return deserialize((PIGPIO_Config::Servo_Channel&)*value, *value_sz_value);
  }
  else if (path == "::PIGPIO_Config::PWM_Channel")
  {
    value = Poly<PIGPIO_Config::IChannel>(new PIGPIO_Config::PWM_Channel());
    return deserialize((PIGPIO_Config::PWM_Channel&)*value, *value_sz_value);
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(Poly<PIGPIO_Config::IChannel> const& value)
{
  if (!value) { return ts::sz::Value(); }
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (typeid(*value) == typeid(PIGPIO_Config::Servo_Channel))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::PIGPIO_Config::Servo_Channel");
    sz_value.add_object_member("value", serialize((PIGPIO_Config::Servo_Channel&)*value));
    return std::move(sz_value);
  }
  else if (typeid(*value) == typeid(PIGPIO_Config::PWM_Channel))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::PIGPIO_Config::PWM_Channel");
    sz_value.add_object_member("value", serialize((PIGPIO_Config::PWM_Channel&)*value));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(Pressure_Velocity_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Pressure_Velocity_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Pressure_Velocity_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(Pressure_Velocity_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Proximity_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "channel_count")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_channel_count())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_channel_count(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Proximity_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  sz_value.add_object_member("channel_count", serialize(value.get_channel_count()));
  return sz_value;
}
ts::Result<void> deserialize(Proximity_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(Proximity_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Rate_Controller_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Rate_Controller_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Rate_Controller_Config::Feedback::Separate_XY_PIDs& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "x_pid")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_x_pid())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_x_pid(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "y_pid")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_y_pid())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_y_pid(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Rate_Controller_Config::Feedback::Separate_XY_PIDs const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("x_pid", serialize(value.get_x_pid()));
  sz_value.add_object_member("y_pid", serialize(value.get_y_pid()));
  return sz_value;
}
ts::Result<void> deserialize(Rate_Controller_Config::Feedback& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "weight")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_weight())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_weight(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "xy_pids")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_xy_pids())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_xy_pids(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "z_pid")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_z_pid())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_z_pid(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Rate_Controller_Config::Feedback const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("weight", serialize(value.get_weight()));
  sz_value.add_object_member("xy_pids", serialize(value.get_xy_pids()));
  sz_value.add_object_member("z_pid", serialize(value.get_z_pid()));
  return sz_value;
}
ts::Result<void> deserialize(Rate_Controller_Config::Feedforward& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "weight")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_weight())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_weight(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Rate_Controller_Config::Feedforward const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("weight", serialize(value.get_weight()));
  return sz_value;
}
ts::Result<void> deserialize(Rate_Controller_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "max_torque")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_torque())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_torque(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "feedback")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_feedback())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_feedback(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "feedforward")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_feedforward())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_feedforward(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Rate_Controller_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("max_torque", serialize(value.get_max_torque()));
  sz_value.add_object_member("feedback", serialize(value.get_feedback()));
  sz_value.add_object_member("feedforward", serialize(value.get_feedforward()));
  return sz_value;
}
ts::Result<void> deserialize(boost::variant<Rate_Controller_Config::Feedback::Combined_XY_PIDs,Rate_Controller_Config::Feedback::Separate_XY_PIDs>& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::Rate_Controller_Config::Feedback::Combined_XY_PIDs")
  {
    value = Rate_Controller_Config::Feedback::Combined_XY_PIDs();
    auto result = deserialize(boost::get<Rate_Controller_Config::Feedback::Combined_XY_PIDs>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::Rate_Controller_Config::Feedback::Separate_XY_PIDs")
  {
    value = Rate_Controller_Config::Feedback::Separate_XY_PIDs();
    auto result = deserialize(boost::get<Rate_Controller_Config::Feedback::Separate_XY_PIDs>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(boost::variant<Rate_Controller_Config::Feedback::Combined_XY_PIDs,Rate_Controller_Config::Feedback::Separate_XY_PIDs> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (auto* v = boost::get<Rate_Controller_Config::Feedback::Combined_XY_PIDs>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Rate_Controller_Config::Feedback::Combined_XY_PIDs");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<Rate_Controller_Config::Feedback::Separate_XY_PIDs>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::Rate_Controller_Config::Feedback::Separate_XY_PIDs");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(Raspicam_Descriptor::Quality& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "resolution")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_resolution())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_resolution(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "bitrate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bitrate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bitrate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Raspicam_Descriptor::Quality const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("resolution", serialize(value.get_resolution()));
  sz_value.add_object_member("bitrate", serialize(value.get_bitrate()));
  return sz_value;
}
ts::Result<void> deserialize(Raspicam_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "fps")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_fps())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_fps(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "iframe_interval")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_iframe_interval())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_iframe_interval(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "commands_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_commands_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_commands_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "streaming_low")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_streaming_low())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_streaming_low(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "streaming_high")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_streaming_high())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_streaming_high(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "recording")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_recording())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_recording(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Raspicam_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(6);
  sz_value.add_object_member("fps", serialize(value.get_fps()));
  sz_value.add_object_member("iframe_interval", serialize(value.get_iframe_interval()));
  sz_value.add_object_member("commands_rate", serialize(value.get_commands_rate()));
  sz_value.add_object_member("streaming_low", serialize(value.get_streaming_low()));
  sz_value.add_object_member("streaming_high", serialize(value.get_streaming_high()));
  sz_value.add_object_member("recording", serialize(value.get_recording()));
  return sz_value;
}
ts::Result<void> deserialize(Raspicam_Config::awb_mode_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  std::string const& key = sz_value.get_as_string();
  typedef Raspicam_Config::awb_mode_t _etype;
  static std::map<std::string, _etype> s_map = {
    { "OFF", _etype::OFF },
    { "AUTO", _etype::AUTO },
    { "SUNLIGHT", _etype::SUNLIGHT },
    { "CLOUDY", _etype::CLOUDY },
    { "SHADE", _etype::SHADE },
    { "TUNGSTEN", _etype::TUNGSTEN },
    { "FLUORESCENT", _etype::FLUORESCENT },
    { "INCANDESCENT", _etype::INCANDESCENT },
    { "FLASH", _etype::FLASH },
    { "HORIZON", _etype::HORIZON },
  };
  auto it = s_map.find(key);
  if (it == s_map.end()) { return ts::Error("Cannot find item " + key + " when deserializing"); }
  value = it->second;
  return ts::success;
}
ts::sz::Value serialize(Raspicam_Config::awb_mode_t const& value)
{
  typedef Raspicam_Config::awb_mode_t _etype;
  static std::map<_etype, std::string> s_map = {
    { _etype::OFF, "OFF" },
    { _etype::AUTO, "AUTO" },
    { _etype::SUNLIGHT, "SUNLIGHT" },
    { _etype::CLOUDY, "CLOUDY" },
    { _etype::SHADE, "SHADE" },
    { _etype::TUNGSTEN, "TUNGSTEN" },
    { _etype::FLUORESCENT, "FLUORESCENT" },
    { _etype::INCANDESCENT, "INCANDESCENT" },
    { _etype::FLASH, "FLASH" },
    { _etype::HORIZON, "HORIZON" },
  };
  auto it = s_map.find(value);
  if (it == s_map.end()) { TS_ASSERT(false); return ts::sz::Value(); }
  return ts::sz::Value(it->second);
}
ts::Result<void> deserialize(Raspicam_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "iso")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_iso())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_iso(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "shutter_speed")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_shutter_speed())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_shutter_speed(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "ev")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_ev())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_ev(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "sharpness")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_sharpness())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_sharpness(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "contrast")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_contrast())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_contrast(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "brightness")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_brightness())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_brightness(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "saturation")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_saturation())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_saturation(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "awb_mode")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_awb_mode())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_awb_mode(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "hflip")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_hflip())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_hflip(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "vflip")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_vflip())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_vflip(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Raspicam_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(10);
  sz_value.add_object_member("iso", serialize(value.get_iso()));
  sz_value.add_object_member("shutter_speed", serialize(value.get_shutter_speed()));
  sz_value.add_object_member("ev", serialize(value.get_ev()));
  sz_value.add_object_member("sharpness", serialize(value.get_sharpness()));
  sz_value.add_object_member("contrast", serialize(value.get_contrast()));
  sz_value.add_object_member("brightness", serialize(value.get_brightness()));
  sz_value.add_object_member("saturation", serialize(value.get_saturation()));
  sz_value.add_object_member("awb_mode", serialize(value.get_awb_mode()));
  sz_value.add_object_member("hflip", serialize(value.get_hflip()));
  sz_value.add_object_member("vflip", serialize(value.get_vflip()));
  return sz_value;
}
ts::Result<void> deserialize(RC5T619_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "bus")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bus(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "adc0_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_adc0_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_adc0_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "adc1_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_adc1_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_adc1_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(RC5T619_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("bus", serialize(value.get_bus()));
  sz_value.add_object_member("adc0_rate", serialize(value.get_adc0_rate()));
  sz_value.add_object_member("adc1_rate", serialize(value.get_adc1_rate()));
  return sz_value;
}
ts::Result<void> deserialize(RC5T619_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(RC5T619_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Resampler_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "input_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_input_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_input_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "output_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_output_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_output_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Resampler_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("input_rate", serialize(value.get_input_rate()));
  sz_value.add_object_member("output_rate", serialize(value.get_output_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Resampler_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "lpf")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_lpf())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_lpf(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Resampler_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("lpf", serialize(value.get_lpf()));
  return sz_value;
}
ts::Result<void> deserialize(Scalar_Generator_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Scalar_Generator_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Scalar_Generator_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "value")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_value())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_value(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Scalar_Generator_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("value", serialize(value.get_value()));
  return sz_value;
}
ts::Result<void> deserialize(Servo_Gimbal_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "commands_rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_commands_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_commands_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Servo_Gimbal_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  sz_value.add_object_member("commands_rate", serialize(value.get_commands_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Servo_Gimbal_Config::Channel& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "min_angle")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_min_angle())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_min_angle(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max_angle")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_angle())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_angle(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "min_pwm")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_min_pwm())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_min_pwm(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max_pwm")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_pwm())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_pwm(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Servo_Gimbal_Config::Channel const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("min_angle", serialize(value.get_min_angle()));
  sz_value.add_object_member("max_angle", serialize(value.get_max_angle()));
  sz_value.add_object_member("min_pwm", serialize(value.get_min_pwm()));
  sz_value.add_object_member("max_pwm", serialize(value.get_max_pwm()));
  return sz_value;
}
ts::Result<void> deserialize(Servo_Gimbal_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "x_channel")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_x_channel())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_x_channel(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "y_channel")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_y_channel())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_y_channel(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "z_channel")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_z_channel())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_z_channel(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Servo_Gimbal_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("x_channel", serialize(value.get_x_channel()));
  sz_value.add_object_member("y_channel", serialize(value.get_y_channel()));
  sz_value.add_object_member("z_channel", serialize(value.get_z_channel()));
  return sz_value;
}
ts::Result<void> deserialize(SRF01_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "bus")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bus(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(SRF01_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("bus", serialize(value.get_bus()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(SRF01_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "direction")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_direction())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_direction(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max_distance")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_distance())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_distance(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "min_distance")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_min_distance())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_min_distance(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(SRF01_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("direction", serialize(value.get_direction()));
  sz_value.add_object_member("max_distance", serialize(value.get_max_distance()));
  sz_value.add_object_member("min_distance", serialize(value.get_min_distance()));
  return sz_value;
}
ts::Result<void> deserialize(SRF02_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "bus")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bus(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(SRF02_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("bus", serialize(value.get_bus()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(SRF02_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "direction")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_direction())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_direction(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max_distance")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_distance())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_distance(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "min_distance")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_min_distance())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_min_distance(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(SRF02_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("direction", serialize(value.get_direction()));
  sz_value.add_object_member("max_distance", serialize(value.get_max_distance()));
  sz_value.add_object_member("min_distance", serialize(value.get_min_distance()));
  return sz_value;
}
ts::Result<void> deserialize(Throttle_To_PWM_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "channel_count")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_channel_count())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_channel_count(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Throttle_To_PWM_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  sz_value.add_object_member("channel_count", serialize(value.get_channel_count()));
  return sz_value;
}
ts::Result<void> deserialize(Throttle_To_PWM_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(Throttle_To_PWM_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Transformer_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Transformer_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Transformer_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(Transformer_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(UBLOX_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "bus")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_bus())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_bus(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(UBLOX_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("bus", serialize(value.get_bus()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(UBLOX_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
  }
  return ts::success;
}
ts::sz::Value serialize(UBLOX_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(0);
  return sz_value;
}
ts::Result<void> deserialize(Vec3_Generator_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Vec3_Generator_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(Vec3_Generator_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "value")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_value())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_value(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(Vec3_Generator_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("value", serialize(value.get_value()));
  return sz_value;
}
ts::Result<void> deserialize(CPPM_Receiver_Descriptor::gpio_t& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_string()) { return ts::Error("Expected string value when deserializing"); }
  std::string const& key = sz_value.get_as_string();
  typedef CPPM_Receiver_Descriptor::gpio_t _etype;
  static std::map<std::string, _etype> s_map = {
    { "_2", _etype::_2 },
    { "_3", _etype::_3 },
    { "_4", _etype::_4 },
    { "_5", _etype::_5 },
    { "_6", _etype::_6 },
    { "_7", _etype::_7 },
    { "_8", _etype::_8 },
    { "_9", _etype::_9 },
    { "_10", _etype::_10 },
    { "_11", _etype::_11 },
    { "_12", _etype::_12 },
    { "_13", _etype::_13 },
    { "_14", _etype::_14 },
    { "_15", _etype::_15 },
    { "_16", _etype::_16 },
    { "_17", _etype::_17 },
    { "_18", _etype::_18 },
    { "_19", _etype::_19 },
    { "_20", _etype::_20 },
    { "_21", _etype::_21 },
    { "_22", _etype::_22 },
    { "_23", _etype::_23 },
    { "_24", _etype::_24 },
    { "_25", _etype::_25 },
    { "_26", _etype::_26 },
    { "_27", _etype::_27 },
  };
  auto it = s_map.find(key);
  if (it == s_map.end()) { return ts::Error("Cannot find item " + key + " when deserializing"); }
  value = it->second;
  return ts::success;
}
ts::sz::Value serialize(CPPM_Receiver_Descriptor::gpio_t const& value)
{
  typedef CPPM_Receiver_Descriptor::gpio_t _etype;
  static std::map<_etype, std::string> s_map = {
    { _etype::_2, "_2" },
    { _etype::_3, "_3" },
    { _etype::_4, "_4" },
    { _etype::_5, "_5" },
    { _etype::_6, "_6" },
    { _etype::_7, "_7" },
    { _etype::_8, "_8" },
    { _etype::_9, "_9" },
    { _etype::_10, "_10" },
    { _etype::_11, "_11" },
    { _etype::_12, "_12" },
    { _etype::_13, "_13" },
    { _etype::_14, "_14" },
    { _etype::_15, "_15" },
    { _etype::_16, "_16" },
    { _etype::_17, "_17" },
    { _etype::_18, "_18" },
    { _etype::_19, "_19" },
    { _etype::_20, "_20" },
    { _etype::_21, "_21" },
    { _etype::_22, "_22" },
    { _etype::_23, "_23" },
    { _etype::_24, "_24" },
    { _etype::_25, "_25" },
    { _etype::_26, "_26" },
    { _etype::_27, "_27" },
  };
  auto it = s_map.find(value);
  if (it == s_map.end()) { TS_ASSERT(false); return ts::sz::Value(); }
  return ts::sz::Value(it->second);
}
ts::Result<void> deserialize(CPPM_Receiver_Descriptor& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "gpio")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gpio())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gpio(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "channel_count")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_channel_count())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_channel_count(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "rate")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_rate(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(CPPM_Receiver_Descriptor const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("gpio", serialize(value.get_gpio()));
  sz_value.add_object_member("channel_count", serialize(value.get_channel_count()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(CPPM_Receiver_Config& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  for (size_t i = 0; i < sz_value.get_object_member_count(); i++)
  {
    ts::sz::Value const& member_sz_value = sz_value.get_object_member_value(i);
    if (sz_value.get_object_member_name(i) == "inverted")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_inverted())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_inverted(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "frame_length")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_frame_length())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_frame_length(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "gap_pulse_length")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_gap_pulse_length())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_gap_pulse_length(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "min_pulse_length")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_min_pulse_length())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_min_pulse_length(std::move(v));
    }
    else     if (sz_value.get_object_member_name(i) == "max_pulse_length")
    {
      std::remove_cv<std::remove_reference<decltype(value.get_max_pulse_length())>::type>::type v;
      auto result = deserialize(v, member_sz_value);
      if (result != ts::success) { return result; }
      value.set_max_pulse_length(std::move(v));
    }
  }
  return ts::success;
}
ts::sz::Value serialize(CPPM_Receiver_Config const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(5);
  sz_value.add_object_member("inverted", serialize(value.get_inverted()));
  sz_value.add_object_member("frame_length", serialize(value.get_frame_length()));
  sz_value.add_object_member("gap_pulse_length", serialize(value.get_gap_pulse_length()));
  sz_value.add_object_member("min_pulse_length", serialize(value.get_min_pulse_length()));
  sz_value.add_object_member("max_pulse_length", serialize(value.get_max_pulse_length()));
  return sz_value;
}
}
}
