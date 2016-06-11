#include "comms.def.h"
template <typename T>
T clamp(T v, T min, T max)
{
  return std::min(std::max(v, min), max);
}
std::string const& get_ast_json()
{
  static const std::string s_json = R"xxx({"type":1,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"int8_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":-128}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":127}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"int8_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"uint8_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":255}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"uint8_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"int16_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":-32768}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":32767}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"int16_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"uint16_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":65535}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"uint16_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"int32_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":-2147483648}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":2147483647}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"int32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"max"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":4294967295}]}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"native_type"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":0,"value":"uint32_t"}]}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"int64_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int"}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"ufloat"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"float"}]}]},{"type":21,"children":[{"type":6,"attributes":[{"type":0,"value":"min"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":3,"value":0.000000}]}]}]}]},{"type":10,"children":[{"type":6,"attributes":[{"type":0,"value":"silk"}]},{"type":11,"children":[{"type":10,"children":[{"type":6,"attributes":[{"type":0,"value":"comms"}]},{"type":11,"children":[{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"time_ms_t"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"int64_t"}]}]}]},{"type":12,"children":[{"type":6,"attributes":[{"type":0,"value":"serialized_data"}]},{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Set_Clock_Req"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"time_ms_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"time"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Set_Clock_Res"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"time_ms_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"time"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Set_UAV_Descriptor_Req"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"serialized_data"}]}]},{"type":6,"attributes":[{"type":0,"value":"data"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Set_UAV_Descriptor_Res"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"serialized_data"}]}]},{"type":6,"attributes":[{"type":0,"value":"data"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Get_UAV_Descriptor_Req"}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Get_UAV_Descriptor_Res"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"serialized_data"}]}]},{"type":6,"attributes":[{"type":0,"value":"data"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Def_Data"}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Input"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"type"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Output"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"type"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"type"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Input"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"inputs"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Output"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"outputs"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"serialized_data"}]}]},{"type":6,"attributes":[{"type":0,"value":"descriptor_data"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Get_Node_Defs_Req"}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Get_Node_Defs_Res"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Def_Data"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"node_def_datas"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Data"}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Input"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"type"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"stream_path"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Output"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"type"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"uint32_t"}]}]},{"type":6,"attributes":[{"type":0,"value":"rate"}]},{"type":20,"children":[{"type":4,"attributes":[{"type":2,"value":0}]}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"type"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Input"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"inputs"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Output"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"outputs"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"serialized_data"}]}]},{"type":6,"attributes":[{"type":0,"value":"descriptor_data"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"serialized_data"}]}]},{"type":6,"attributes":[{"type":0,"value":"config_data"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Get_Nodes_Req"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"name"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Get_Nodes_Res"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"vector"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Data"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"node_datas"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Set_Node_Input_Stream_Path_Req"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"node_name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"input_name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"stream_path"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Set_Node_Input_Stream_Path_Res"}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Error"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"message"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Data"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Error"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"result"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Add_Node_Req"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"def_name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"name"}]}]},{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"serialized_data"}]}]},{"type":6,"attributes":[{"type":0,"value":"descriptor_data"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Add_Node_Res"}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Error"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"message"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"variant"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Node_Data"}]}]}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Error"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"result"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Remove_Node_Req"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"name"}]}]}]}]},{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Remove_Node_Res"}]},{"type":14,"children":[{"type":13,"children":[{"type":6,"attributes":[{"type":0,"value":"Error"}]},{"type":14,"children":[{"type":18,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"string"}]}]},{"type":6,"attributes":[{"type":0,"value":"message"}]}]}]}]},{"type":18,"children":[{"type":3,"children":[{"type":8,"children":[{"type":6,"attributes":[{"type":0,"value":"optional"}]},{"type":9,"children":[{"type":3,"children":[{"type":6,"attributes":[{"type":0,"value":"Error"}]}]}]}]}]},{"type":6,"attributes":[{"type":0,"value":"error"}]}]}]}]}]}]}]}]}]})xxx";
  return s_json;
}

namespace math
{

}
namespace silk
{

namespace comms
{

  void Set_Clock_Req::set_time(int64_t const& value)
  {
    m_time = clamp(value, int64_t(-9223372036854775808), int64_t(9223372036854775807));
  }
  auto Set_Clock_Req::get_time() const -> int64_t const& 
  {
    return m_time;
  }


////////////////////////////////////////////////////////////

  void Set_Clock_Res::set_time(int64_t const& value)
  {
    m_time = clamp(value, int64_t(-9223372036854775808), int64_t(9223372036854775807));
  }
  auto Set_Clock_Res::get_time() const -> int64_t const& 
  {
    return m_time;
  }


////////////////////////////////////////////////////////////

  void Set_UAV_Descriptor_Req::set_data(std::string const& value)
  {
    m_data = value;
  }
  auto Set_UAV_Descriptor_Req::get_data() const -> std::string const& 
  {
    return m_data;
  }


////////////////////////////////////////////////////////////

  void Set_UAV_Descriptor_Res::set_data(std::string const& value)
  {
    m_data = value;
  }
  auto Set_UAV_Descriptor_Res::get_data() const -> std::string const& 
  {
    return m_data;
  }


////////////////////////////////////////////////////////////

  void Get_UAV_Descriptor_Res::set_data(std::string const& value)
  {
    m_data = value;
  }
  auto Get_UAV_Descriptor_Res::get_data() const -> std::string const& 
  {
    return m_data;
  }


////////////////////////////////////////////////////////////

    void Node_Def_Data::Input::set_name(std::string const& value)
    {
      m_name = value;
    }
    auto Node_Def_Data::Input::get_name() const -> std::string const& 
    {
      return m_name;
    }


////////////////////////////////////////////////////////////

    void Node_Def_Data::Input::set_type(std::string const& value)
    {
      m_type = value;
    }
    auto Node_Def_Data::Input::get_type() const -> std::string const& 
    {
      return m_type;
    }


////////////////////////////////////////////////////////////

    void Node_Def_Data::Input::set_rate(uint32_t const& value)
    {
      m_rate = clamp(value, uint32_t(0), uint32_t(4294967295));
    }
    auto Node_Def_Data::Input::get_rate() const -> uint32_t const& 
    {
      return m_rate;
    }


////////////////////////////////////////////////////////////

    void Node_Def_Data::Output::set_name(std::string const& value)
    {
      m_name = value;
    }
    auto Node_Def_Data::Output::get_name() const -> std::string const& 
    {
      return m_name;
    }


////////////////////////////////////////////////////////////

    void Node_Def_Data::Output::set_type(std::string const& value)
    {
      m_type = value;
    }
    auto Node_Def_Data::Output::get_type() const -> std::string const& 
    {
      return m_type;
    }


////////////////////////////////////////////////////////////

    void Node_Def_Data::Output::set_rate(uint32_t const& value)
    {
      m_rate = clamp(value, uint32_t(0), uint32_t(4294967295));
    }
    auto Node_Def_Data::Output::get_rate() const -> uint32_t const& 
    {
      return m_rate;
    }


////////////////////////////////////////////////////////////

  void Node_Def_Data::set_name(std::string const& value)
  {
    m_name = value;
  }
  auto Node_Def_Data::get_name() const -> std::string const& 
  {
    return m_name;
  }


////////////////////////////////////////////////////////////

  void Node_Def_Data::set_type(std::string const& value)
  {
    m_type = value;
  }
  auto Node_Def_Data::get_type() const -> std::string const& 
  {
    return m_type;
  }


////////////////////////////////////////////////////////////

  void Node_Def_Data::set_inputs(std::vector<::silk::comms::Node_Def_Data::Input> const& value)
  {
    m_inputs = value;
  }
  auto Node_Def_Data::get_inputs() const -> std::vector<::silk::comms::Node_Def_Data::Input> const& 
  {
    return m_inputs;
  }

  auto Node_Def_Data::get_inputs() -> std::vector<::silk::comms::Node_Def_Data::Input>& 
  {
    return m_inputs;
  }


////////////////////////////////////////////////////////////

  void Node_Def_Data::set_outputs(std::vector<::silk::comms::Node_Def_Data::Output> const& value)
  {
    m_outputs = value;
  }
  auto Node_Def_Data::get_outputs() const -> std::vector<::silk::comms::Node_Def_Data::Output> const& 
  {
    return m_outputs;
  }

  auto Node_Def_Data::get_outputs() -> std::vector<::silk::comms::Node_Def_Data::Output>& 
  {
    return m_outputs;
  }


////////////////////////////////////////////////////////////

  void Node_Def_Data::set_descriptor_data(std::string const& value)
  {
    m_descriptor_data = value;
  }
  auto Node_Def_Data::get_descriptor_data() const -> std::string const& 
  {
    return m_descriptor_data;
  }


////////////////////////////////////////////////////////////

  void Get_Node_Defs_Res::set_node_def_datas(std::vector<::silk::comms::Node_Def_Data> const& value)
  {
    m_node_def_datas = value;
  }
  auto Get_Node_Defs_Res::get_node_def_datas() const -> std::vector<::silk::comms::Node_Def_Data> const& 
  {
    return m_node_def_datas;
  }

  auto Get_Node_Defs_Res::get_node_def_datas() -> std::vector<::silk::comms::Node_Def_Data>& 
  {
    return m_node_def_datas;
  }


////////////////////////////////////////////////////////////

    void Node_Data::Input::set_name(std::string const& value)
    {
      m_name = value;
    }
    auto Node_Data::Input::get_name() const -> std::string const& 
    {
      return m_name;
    }


////////////////////////////////////////////////////////////

    void Node_Data::Input::set_type(std::string const& value)
    {
      m_type = value;
    }
    auto Node_Data::Input::get_type() const -> std::string const& 
    {
      return m_type;
    }


////////////////////////////////////////////////////////////

    void Node_Data::Input::set_rate(uint32_t const& value)
    {
      m_rate = clamp(value, uint32_t(0), uint32_t(4294967295));
    }
    auto Node_Data::Input::get_rate() const -> uint32_t const& 
    {
      return m_rate;
    }


////////////////////////////////////////////////////////////

    void Node_Data::Input::set_stream_path(std::string const& value)
    {
      m_stream_path = value;
    }
    auto Node_Data::Input::get_stream_path() const -> std::string const& 
    {
      return m_stream_path;
    }


////////////////////////////////////////////////////////////

    void Node_Data::Output::set_name(std::string const& value)
    {
      m_name = value;
    }
    auto Node_Data::Output::get_name() const -> std::string const& 
    {
      return m_name;
    }


////////////////////////////////////////////////////////////

    void Node_Data::Output::set_type(std::string const& value)
    {
      m_type = value;
    }
    auto Node_Data::Output::get_type() const -> std::string const& 
    {
      return m_type;
    }


////////////////////////////////////////////////////////////

    void Node_Data::Output::set_rate(uint32_t const& value)
    {
      m_rate = clamp(value, uint32_t(0), uint32_t(4294967295));
    }
    auto Node_Data::Output::get_rate() const -> uint32_t const& 
    {
      return m_rate;
    }


////////////////////////////////////////////////////////////

  void Node_Data::set_name(std::string const& value)
  {
    m_name = value;
  }
  auto Node_Data::get_name() const -> std::string const& 
  {
    return m_name;
  }


////////////////////////////////////////////////////////////

  void Node_Data::set_type(std::string const& value)
  {
    m_type = value;
  }
  auto Node_Data::get_type() const -> std::string const& 
  {
    return m_type;
  }


////////////////////////////////////////////////////////////

  void Node_Data::set_inputs(std::vector<::silk::comms::Node_Data::Input> const& value)
  {
    m_inputs = value;
  }
  auto Node_Data::get_inputs() const -> std::vector<::silk::comms::Node_Data::Input> const& 
  {
    return m_inputs;
  }

  auto Node_Data::get_inputs() -> std::vector<::silk::comms::Node_Data::Input>& 
  {
    return m_inputs;
  }


////////////////////////////////////////////////////////////

  void Node_Data::set_outputs(std::vector<::silk::comms::Node_Data::Output> const& value)
  {
    m_outputs = value;
  }
  auto Node_Data::get_outputs() const -> std::vector<::silk::comms::Node_Data::Output> const& 
  {
    return m_outputs;
  }

  auto Node_Data::get_outputs() -> std::vector<::silk::comms::Node_Data::Output>& 
  {
    return m_outputs;
  }


////////////////////////////////////////////////////////////

  void Node_Data::set_descriptor_data(std::string const& value)
  {
    m_descriptor_data = value;
  }
  auto Node_Data::get_descriptor_data() const -> std::string const& 
  {
    return m_descriptor_data;
  }


////////////////////////////////////////////////////////////

  void Node_Data::set_config_data(std::string const& value)
  {
    m_config_data = value;
  }
  auto Node_Data::get_config_data() const -> std::string const& 
  {
    return m_config_data;
  }


////////////////////////////////////////////////////////////

  void Get_Nodes_Req::set_name(std::string const& value)
  {
    m_name = value;
  }
  auto Get_Nodes_Req::get_name() const -> std::string const& 
  {
    return m_name;
  }


////////////////////////////////////////////////////////////

  void Get_Nodes_Res::set_node_datas(std::vector<::silk::comms::Node_Data> const& value)
  {
    m_node_datas = value;
  }
  auto Get_Nodes_Res::get_node_datas() const -> std::vector<::silk::comms::Node_Data> const& 
  {
    return m_node_datas;
  }

  auto Get_Nodes_Res::get_node_datas() -> std::vector<::silk::comms::Node_Data>& 
  {
    return m_node_datas;
  }


////////////////////////////////////////////////////////////

  void Set_Node_Input_Stream_Path_Req::set_node_name(std::string const& value)
  {
    m_node_name = value;
  }
  auto Set_Node_Input_Stream_Path_Req::get_node_name() const -> std::string const& 
  {
    return m_node_name;
  }


////////////////////////////////////////////////////////////

  void Set_Node_Input_Stream_Path_Req::set_input_name(std::string const& value)
  {
    m_input_name = value;
  }
  auto Set_Node_Input_Stream_Path_Req::get_input_name() const -> std::string const& 
  {
    return m_input_name;
  }


////////////////////////////////////////////////////////////

  void Set_Node_Input_Stream_Path_Req::set_stream_path(std::string const& value)
  {
    m_stream_path = value;
  }
  auto Set_Node_Input_Stream_Path_Req::get_stream_path() const -> std::string const& 
  {
    return m_stream_path;
  }


////////////////////////////////////////////////////////////

    void Set_Node_Input_Stream_Path_Res::Error::set_message(std::string const& value)
    {
      m_message = value;
    }
    auto Set_Node_Input_Stream_Path_Res::Error::get_message() const -> std::string const& 
    {
      return m_message;
    }


////////////////////////////////////////////////////////////

  void Set_Node_Input_Stream_Path_Res::set_result(boost::variant<::silk::comms::Node_Data,Error> const& value)
  {
    m_result = value;
  }
  auto Set_Node_Input_Stream_Path_Res::get_result() const -> boost::variant<::silk::comms::Node_Data,Error> const& 
  {
    return m_result;
  }

  auto Set_Node_Input_Stream_Path_Res::get_result() -> boost::variant<::silk::comms::Node_Data,Error>& 
  {
    return m_result;
  }


////////////////////////////////////////////////////////////

  void Add_Node_Req::set_def_name(std::string const& value)
  {
    m_def_name = value;
  }
  auto Add_Node_Req::get_def_name() const -> std::string const& 
  {
    return m_def_name;
  }


////////////////////////////////////////////////////////////

  void Add_Node_Req::set_name(std::string const& value)
  {
    m_name = value;
  }
  auto Add_Node_Req::get_name() const -> std::string const& 
  {
    return m_name;
  }


////////////////////////////////////////////////////////////

  void Add_Node_Req::set_descriptor_data(std::string const& value)
  {
    m_descriptor_data = value;
  }
  auto Add_Node_Req::get_descriptor_data() const -> std::string const& 
  {
    return m_descriptor_data;
  }


////////////////////////////////////////////////////////////

    void Add_Node_Res::Error::set_message(std::string const& value)
    {
      m_message = value;
    }
    auto Add_Node_Res::Error::get_message() const -> std::string const& 
    {
      return m_message;
    }


////////////////////////////////////////////////////////////

  void Add_Node_Res::set_result(boost::variant<::silk::comms::Node_Data,Error> const& value)
  {
    m_result = value;
  }
  auto Add_Node_Res::get_result() const -> boost::variant<::silk::comms::Node_Data,Error> const& 
  {
    return m_result;
  }

  auto Add_Node_Res::get_result() -> boost::variant<::silk::comms::Node_Data,Error>& 
  {
    return m_result;
  }


////////////////////////////////////////////////////////////

  void Remove_Node_Req::set_name(std::string const& value)
  {
    m_name = value;
  }
  auto Remove_Node_Req::get_name() const -> std::string const& 
  {
    return m_name;
  }


////////////////////////////////////////////////////////////

    void Remove_Node_Res::Error::set_message(std::string const& value)
    {
      m_message = value;
    }
    auto Remove_Node_Res::Error::get_message() const -> std::string const& 
    {
      return m_message;
    }


////////////////////////////////////////////////////////////

  void Remove_Node_Res::set_error(boost::optional<Error> const& value)
  {
    m_error = value;
  }
  auto Remove_Node_Res::get_error() const -> boost::optional<Error> const& 
  {
    return m_error;
  }

  auto Remove_Node_Res::get_error() -> boost::optional<Error>& 
  {
    return m_error;
  }


////////////////////////////////////////////////////////////

}
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
ts::Result<void> deserialize(::silk::comms::Set_Clock_Req& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("time");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'time'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_time())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_time(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_Clock_Req const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_time());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("time", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Set_Clock_Res& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("time");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'time'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_time())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_time(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_Clock_Res const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_time());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("time", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Set_UAV_Descriptor_Req& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_data(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_UAV_Descriptor_Req const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_data());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("data", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Set_UAV_Descriptor_Res& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_data(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_UAV_Descriptor_Res const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_data());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("data", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Get_UAV_Descriptor_Req& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Get_UAV_Descriptor_Req const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Get_UAV_Descriptor_Res& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_data(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Get_UAV_Descriptor_Res const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_data());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("data", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Node_Def_Data::Input& value, ts::serialization::Value const& sz_value)
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Node_Def_Data::Input const& value)
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
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Node_Def_Data::Output& value, ts::serialization::Value const& sz_value)
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Node_Def_Data::Output const& value)
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
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Node_Def_Data& value, ts::serialization::Value const& sz_value)
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("inputs");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'inputs'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_inputs())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_inputs(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("outputs");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'outputs'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_outputs())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_outputs(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("descriptor_data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'descriptor_data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_descriptor_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_descriptor_data(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Node_Def_Data const& value)
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
    auto result = serialize(value.get_inputs());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("inputs", result.extract_payload());
  }
  {
    auto result = serialize(value.get_outputs());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("outputs", result.extract_payload());
  }
  {
    auto result = serialize(value.get_descriptor_data());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("descriptor_data", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Get_Node_Defs_Req& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Get_Node_Defs_Req const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Get_Node_Defs_Res& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("node_def_datas");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'node_def_datas'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_node_def_datas())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_node_def_datas(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Get_Node_Defs_Res const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_node_def_datas());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("node_def_datas", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Node_Data::Input& value, ts::serialization::Value const& sz_value)
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("stream_path");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'stream_path'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_stream_path())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_stream_path(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Node_Data::Input const& value)
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
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  {
    auto result = serialize(value.get_stream_path());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("stream_path", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Node_Data::Output& value, ts::serialization::Value const& sz_value)
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Node_Data::Output const& value)
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
    auto result = serialize(value.get_rate());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("rate", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Node_Data& value, ts::serialization::Value const& sz_value)
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
    auto const* member_sz_value = sz_value.find_object_member_by_name("inputs");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'inputs'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_inputs())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_inputs(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("outputs");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'outputs'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_outputs())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_outputs(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("descriptor_data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'descriptor_data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_descriptor_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_descriptor_data(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("config_data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'config_data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_config_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_config_data(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Node_Data const& value)
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
    auto result = serialize(value.get_inputs());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("inputs", result.extract_payload());
  }
  {
    auto result = serialize(value.get_outputs());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("outputs", result.extract_payload());
  }
  {
    auto result = serialize(value.get_descriptor_data());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("descriptor_data", result.extract_payload());
  }
  {
    auto result = serialize(value.get_config_data());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("config_data", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Get_Nodes_Req& value, ts::serialization::Value const& sz_value)
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
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Get_Nodes_Req const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_name());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("name", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Get_Nodes_Res& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("node_datas");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'node_datas'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_node_datas())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_node_datas(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Get_Nodes_Res const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_node_datas());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("node_datas", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Set_Node_Input_Stream_Path_Req& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("node_name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'node_name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_node_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_node_name(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("input_name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'input_name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_input_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_input_name(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("stream_path");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'stream_path'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_stream_path())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_stream_path(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_Node_Input_Stream_Path_Req const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_node_name());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("node_name", result.extract_payload());
  }
  {
    auto result = serialize(value.get_input_name());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("input_name", result.extract_payload());
  }
  {
    auto result = serialize(value.get_stream_path());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("stream_path", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Set_Node_Input_Stream_Path_Res::Error& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("message");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'message'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_message())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_message(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_Node_Input_Stream_Path_Res::Error const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_message());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("message", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Set_Node_Input_Stream_Path_Res& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("result");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'result'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_result())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_result(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_Node_Input_Stream_Path_Res const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_result());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("result", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Add_Node_Req& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("def_name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'def_name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_def_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_def_name(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(v);
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("descriptor_data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'descriptor_data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_descriptor_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_descriptor_data(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Add_Node_Req const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_def_name());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("def_name", result.extract_payload());
  }
  {
    auto result = serialize(value.get_name());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("name", result.extract_payload());
  }
  {
    auto result = serialize(value.get_descriptor_data());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("descriptor_data", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Add_Node_Res::Error& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("message");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'message'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_message())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_message(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Add_Node_Res::Error const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_message());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("message", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Add_Node_Res& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("result");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'result'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_result())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_result(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Add_Node_Res const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_result());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("result", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Remove_Node_Req& value, ts::serialization::Value const& sz_value)
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
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Remove_Node_Req const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_name());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("name", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Remove_Node_Res::Error& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("message");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'message'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_message())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_message(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Remove_Node_Res::Error const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_message());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("message", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(::silk::comms::Remove_Node_Res& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("error");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'error'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_error())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_error(v);
  }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(::silk::comms::Remove_Node_Res const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  {
    auto result = serialize(value.get_error());
    if (result != ts::success) { return result; }
    sz_value.add_object_member("error", result.extract_payload());
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<::silk::comms::Node_Def_Data::Input>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::comms::Node_Def_Data::Input> const& value)
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
ts::Result<void> deserialize(std::vector<::silk::comms::Node_Def_Data::Output>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::comms::Node_Def_Data::Output> const& value)
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
ts::Result<void> deserialize(std::vector<::silk::comms::Node_Def_Data>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::comms::Node_Def_Data> const& value)
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
ts::Result<void> deserialize(std::vector<::silk::comms::Node_Data::Input>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::comms::Node_Data::Input> const& value)
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
ts::Result<void> deserialize(std::vector<::silk::comms::Node_Data::Output>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::comms::Node_Data::Output> const& value)
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
ts::Result<void> deserialize(std::vector<::silk::comms::Node_Data>& value, ts::serialization::Value const& sz_value)
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
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::comms::Node_Data> const& value)
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
ts::Result<void> deserialize(boost::variant<silk::comms::Node_Data,silk::comms::Set_Node_Input_Stream_Path_Res::Error>& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::silk::comms::Node_Data")
  {
    ::silk::comms::Node_Data v;
    auto result = deserialize(boost::get<::silk::comms::Node_Data>(value), *value_sz_value);
    if (result != ts::success) { return result; }
    value = v;
  }
  else if (path == "::silk::comms::Set_Node_Input_Stream_Path_Res::Error")
  {
    ::silk::comms::Set_Node_Input_Stream_Path_Res::Error v;
    auto result = deserialize(boost::get<::silk::comms::Set_Node_Input_Stream_Path_Res::Error>(value), *value_sz_value);
    if (result != ts::success) { return result; }
    value = v;
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(boost::variant<silk::comms::Node_Data,silk::comms::Set_Node_Input_Stream_Path_Res::Error> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (auto* v = boost::get<::silk::comms::Node_Data>(&value))
  {
    sz_value.add_object_member("type", "::silk::comms::Node_Data");
    auto result = serialize(*v);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<::silk::comms::Set_Node_Input_Stream_Path_Res::Error>(&value))
  {
    sz_value.add_object_member("type", "::silk::comms::Set_Node_Input_Stream_Path_Res::Error");
    auto result = serialize(*v);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else { return ts::Error("Cannot serialize type"); }
}
ts::Result<void> deserialize(boost::variant<silk::comms::Node_Data,silk::comms::Add_Node_Res::Error>& value, ts::serialization::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::silk::comms::Node_Data")
  {
    ::silk::comms::Node_Data v;
    auto result = deserialize(boost::get<::silk::comms::Node_Data>(value), *value_sz_value);
    if (result != ts::success) { return result; }
    value = v;
  }
  else if (path == "::silk::comms::Add_Node_Res::Error")
  {
    ::silk::comms::Add_Node_Res::Error v;
    auto result = deserialize(boost::get<::silk::comms::Add_Node_Res::Error>(value), *value_sz_value);
    if (result != ts::success) { return result; }
    value = v;
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::Result<ts::serialization::Value> serialize(boost::variant<silk::comms::Node_Data,silk::comms::Add_Node_Res::Error> const& value)
{
  ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (auto* v = boost::get<::silk::comms::Node_Data>(&value))
  {
    sz_value.add_object_member("type", "::silk::comms::Node_Data");
    auto result = serialize(*v);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<::silk::comms::Add_Node_Res::Error>(&value))
  {
    sz_value.add_object_member("type", "::silk::comms::Add_Node_Res::Error");
    auto result = serialize(*v);
    if (result != ts::success) { return result; }
    sz_value.add_object_member("value", result.extract_payload());
    return std::move(sz_value);
  }
  else { return ts::Error("Cannot serialize type"); }
}
ts::Result<void> deserialize(boost::optional<silk::comms::Remove_Node_Res::Error>& value, ts::serialization::Value const& sz_value)
{
  if (sz_value.is_empty()) { value = boost::none; return ts::success; }
  value = ::silk::comms::Remove_Node_Res::Error();
  return deserialize(*value, sz_value);
}
ts::Result<ts::serialization::Value> serialize(boost::optional<silk::comms::Remove_Node_Res::Error> const& value)
{
  if (!value) { return ts::serialization::Value sz_value(ts::serialization::Value::Type::EMPTY); }
  return serialize(*value);
}
