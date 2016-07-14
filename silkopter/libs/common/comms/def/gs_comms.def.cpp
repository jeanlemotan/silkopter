#include "gs_comms.def.h"
namespace silk
{

namespace gs_comms
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
namespace setup
{

  void IReq::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void IReq::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto IReq::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void IRes::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void IRes::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto IRes::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Error::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Error::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Error::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Error::set_message(std::string const& value)
  {
    m_message = value;
  }
  void Error::set_message(std::string&& value)
  {
    m_message = std::move(value);
  }
  auto Error::get_message() const -> std::string const& 
  {
    return m_message;
  }

////////////////////////////////////////////////////////////
  void Get_AST_Req::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Get_AST_Req::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Get_AST_Req::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Get_AST_Res::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Get_AST_Res::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Get_AST_Res::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Get_AST_Res::set_data(setup::serialized_data_t const& value)
  {
    m_data = value;
  }
  void Get_AST_Res::set_data(setup::serialized_data_t&& value)
  {
    m_data = std::move(value);
  }
  auto Get_AST_Res::get_data() const -> setup::serialized_data_t const& 
  {
    return m_data;
  }

////////////////////////////////////////////////////////////
  void Set_Clock_Req::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Set_Clock_Req::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Set_Clock_Req::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Set_Clock_Req::set_time(setup::time_ms_t const& value)
  {
    m_time = value;
  }
  void Set_Clock_Req::set_time(setup::time_ms_t&& value)
  {
    m_time = std::move(value);
  }
  auto Set_Clock_Req::get_time() const -> setup::time_ms_t const& 
  {
    return m_time;
  }

////////////////////////////////////////////////////////////
  void Set_Clock_Res::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Set_Clock_Res::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Set_Clock_Res::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Set_Clock_Res::set_time(setup::time_ms_t const& value)
  {
    m_time = value;
  }
  void Set_Clock_Res::set_time(setup::time_ms_t&& value)
  {
    m_time = std::move(value);
  }
  auto Set_Clock_Res::get_time() const -> setup::time_ms_t const& 
  {
    return m_time;
  }

////////////////////////////////////////////////////////////
  void Set_UAV_Descriptor_Req::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Set_UAV_Descriptor_Req::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Set_UAV_Descriptor_Req::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Set_UAV_Descriptor_Req::set_data(setup::serialized_data_t const& value)
  {
    m_data = value;
  }
  void Set_UAV_Descriptor_Req::set_data(setup::serialized_data_t&& value)
  {
    m_data = std::move(value);
  }
  auto Set_UAV_Descriptor_Req::get_data() const -> setup::serialized_data_t const& 
  {
    return m_data;
  }

////////////////////////////////////////////////////////////
  void Set_UAV_Descriptor_Res::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Set_UAV_Descriptor_Res::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Set_UAV_Descriptor_Res::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Set_UAV_Descriptor_Res::set_data(setup::serialized_data_t const& value)
  {
    m_data = value;
  }
  void Set_UAV_Descriptor_Res::set_data(setup::serialized_data_t&& value)
  {
    m_data = std::move(value);
  }
  auto Set_UAV_Descriptor_Res::get_data() const -> setup::serialized_data_t const& 
  {
    return m_data;
  }

////////////////////////////////////////////////////////////
  void Get_UAV_Descriptor_Req::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Get_UAV_Descriptor_Req::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Get_UAV_Descriptor_Req::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Get_UAV_Descriptor_Res::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Get_UAV_Descriptor_Res::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Get_UAV_Descriptor_Res::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Get_UAV_Descriptor_Res::set_data(setup::serialized_data_t const& value)
  {
    m_data = value;
  }
  void Get_UAV_Descriptor_Res::set_data(setup::serialized_data_t&& value)
  {
    m_data = std::move(value);
  }
  auto Get_UAV_Descriptor_Res::get_data() const -> setup::serialized_data_t const& 
  {
    return m_data;
  }

////////////////////////////////////////////////////////////
    void Node_Def_Data::Input::set_name(std::string const& value)
    {
      m_name = value;
    }
    void Node_Def_Data::Input::set_name(std::string&& value)
    {
      m_name = std::move(value);
    }
    auto Node_Def_Data::Input::get_name() const -> std::string const& 
    {
      return m_name;
    }

////////////////////////////////////////////////////////////
    void Node_Def_Data::Input::set_space(uint8_t const& value)
    {
      m_space = value;
    }
    void Node_Def_Data::Input::set_space(uint8_t&& value)
    {
      m_space = std::move(value);
    }
    auto Node_Def_Data::Input::get_space() const -> uint8_t const& 
    {
      return m_space;
    }

////////////////////////////////////////////////////////////
    void Node_Def_Data::Input::set_semantic(uint8_t const& value)
    {
      m_semantic = value;
    }
    void Node_Def_Data::Input::set_semantic(uint8_t&& value)
    {
      m_semantic = std::move(value);
    }
    auto Node_Def_Data::Input::get_semantic() const -> uint8_t const& 
    {
      return m_semantic;
    }

////////////////////////////////////////////////////////////
    void Node_Def_Data::Output::set_name(std::string const& value)
    {
      m_name = value;
    }
    void Node_Def_Data::Output::set_name(std::string&& value)
    {
      m_name = std::move(value);
    }
    auto Node_Def_Data::Output::get_name() const -> std::string const& 
    {
      return m_name;
    }

////////////////////////////////////////////////////////////
    void Node_Def_Data::Output::set_space(uint8_t const& value)
    {
      m_space = value;
    }
    void Node_Def_Data::Output::set_space(uint8_t&& value)
    {
      m_space = std::move(value);
    }
    auto Node_Def_Data::Output::get_space() const -> uint8_t const& 
    {
      return m_space;
    }

////////////////////////////////////////////////////////////
    void Node_Def_Data::Output::set_semantic(uint8_t const& value)
    {
      m_semantic = value;
    }
    void Node_Def_Data::Output::set_semantic(uint8_t&& value)
    {
      m_semantic = std::move(value);
    }
    auto Node_Def_Data::Output::get_semantic() const -> uint8_t const& 
    {
      return m_semantic;
    }

////////////////////////////////////////////////////////////
  void Node_Def_Data::set_name(std::string const& value)
  {
    m_name = value;
  }
  void Node_Def_Data::set_name(std::string&& value)
  {
    m_name = std::move(value);
  }
  auto Node_Def_Data::get_name() const -> std::string const& 
  {
    return m_name;
  }

////////////////////////////////////////////////////////////
  void Node_Def_Data::set_type(uint8_t const& value)
  {
    m_type = value;
  }
  void Node_Def_Data::set_type(uint8_t&& value)
  {
    m_type = std::move(value);
  }
  auto Node_Def_Data::get_type() const -> uint8_t const& 
  {
    return m_type;
  }

////////////////////////////////////////////////////////////
  void Node_Def_Data::set_inputs(std::vector<setup::Node_Def_Data::Input> const& value)
  {
    m_inputs = value;
  }
  void Node_Def_Data::set_inputs(std::vector<setup::Node_Def_Data::Input>&& value)
  {
    m_inputs = std::move(value);
  }
  auto Node_Def_Data::get_inputs() const -> std::vector<setup::Node_Def_Data::Input> const& 
  {
    return m_inputs;
  }

  auto Node_Def_Data::get_inputs() -> std::vector<setup::Node_Def_Data::Input>& 
  {
    return m_inputs;
  }

////////////////////////////////////////////////////////////
  void Node_Def_Data::set_outputs(std::vector<setup::Node_Def_Data::Output> const& value)
  {
    m_outputs = value;
  }
  void Node_Def_Data::set_outputs(std::vector<setup::Node_Def_Data::Output>&& value)
  {
    m_outputs = std::move(value);
  }
  auto Node_Def_Data::get_outputs() const -> std::vector<setup::Node_Def_Data::Output> const& 
  {
    return m_outputs;
  }

  auto Node_Def_Data::get_outputs() -> std::vector<setup::Node_Def_Data::Output>& 
  {
    return m_outputs;
  }

////////////////////////////////////////////////////////////
  void Node_Def_Data::set_descriptor_data(setup::serialized_data_t const& value)
  {
    m_descriptor_data = value;
  }
  void Node_Def_Data::set_descriptor_data(setup::serialized_data_t&& value)
  {
    m_descriptor_data = std::move(value);
  }
  auto Node_Def_Data::get_descriptor_data() const -> setup::serialized_data_t const& 
  {
    return m_descriptor_data;
  }

////////////////////////////////////////////////////////////
  void Get_Node_Defs_Req::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Get_Node_Defs_Req::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Get_Node_Defs_Req::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Get_Node_Defs_Res::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Get_Node_Defs_Res::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Get_Node_Defs_Res::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Get_Node_Defs_Res::set_node_def_datas(std::vector<setup::Node_Def_Data> const& value)
  {
    m_node_def_datas = value;
  }
  void Get_Node_Defs_Res::set_node_def_datas(std::vector<setup::Node_Def_Data>&& value)
  {
    m_node_def_datas = std::move(value);
  }
  auto Get_Node_Defs_Res::get_node_def_datas() const -> std::vector<setup::Node_Def_Data> const& 
  {
    return m_node_def_datas;
  }

  auto Get_Node_Defs_Res::get_node_def_datas() -> std::vector<setup::Node_Def_Data>& 
  {
    return m_node_def_datas;
  }

////////////////////////////////////////////////////////////
    void Node_Data::Input::set_name(std::string const& value)
    {
      m_name = value;
    }
    void Node_Data::Input::set_name(std::string&& value)
    {
      m_name = std::move(value);
    }
    auto Node_Data::Input::get_name() const -> std::string const& 
    {
      return m_name;
    }

////////////////////////////////////////////////////////////
    void Node_Data::Input::set_space(uint8_t const& value)
    {
      m_space = value;
    }
    void Node_Data::Input::set_space(uint8_t&& value)
    {
      m_space = std::move(value);
    }
    auto Node_Data::Input::get_space() const -> uint8_t const& 
    {
      return m_space;
    }

////////////////////////////////////////////////////////////
    void Node_Data::Input::set_semantic(uint8_t const& value)
    {
      m_semantic = value;
    }
    void Node_Data::Input::set_semantic(uint8_t&& value)
    {
      m_semantic = std::move(value);
    }
    auto Node_Data::Input::get_semantic() const -> uint8_t const& 
    {
      return m_semantic;
    }

////////////////////////////////////////////////////////////
    void Node_Data::Input::set_rate(uint32_t const& value)
    {
      m_rate = value;
    }
    void Node_Data::Input::set_rate(uint32_t&& value)
    {
      m_rate = std::move(value);
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
    void Node_Data::Input::set_stream_path(std::string&& value)
    {
      m_stream_path = std::move(value);
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
    void Node_Data::Output::set_name(std::string&& value)
    {
      m_name = std::move(value);
    }
    auto Node_Data::Output::get_name() const -> std::string const& 
    {
      return m_name;
    }

////////////////////////////////////////////////////////////
    void Node_Data::Output::set_space(uint8_t const& value)
    {
      m_space = value;
    }
    void Node_Data::Output::set_space(uint8_t&& value)
    {
      m_space = std::move(value);
    }
    auto Node_Data::Output::get_space() const -> uint8_t const& 
    {
      return m_space;
    }

////////////////////////////////////////////////////////////
    void Node_Data::Output::set_semantic(uint8_t const& value)
    {
      m_semantic = value;
    }
    void Node_Data::Output::set_semantic(uint8_t&& value)
    {
      m_semantic = std::move(value);
    }
    auto Node_Data::Output::get_semantic() const -> uint8_t const& 
    {
      return m_semantic;
    }

////////////////////////////////////////////////////////////
    void Node_Data::Output::set_rate(uint32_t const& value)
    {
      m_rate = value;
    }
    void Node_Data::Output::set_rate(uint32_t&& value)
    {
      m_rate = std::move(value);
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
  void Node_Data::set_name(std::string&& value)
  {
    m_name = std::move(value);
  }
  auto Node_Data::get_name() const -> std::string const& 
  {
    return m_name;
  }

////////////////////////////////////////////////////////////
  void Node_Data::set_type(uint8_t const& value)
  {
    m_type = value;
  }
  void Node_Data::set_type(uint8_t&& value)
  {
    m_type = std::move(value);
  }
  auto Node_Data::get_type() const -> uint8_t const& 
  {
    return m_type;
  }

////////////////////////////////////////////////////////////
  void Node_Data::set_inputs(std::vector<setup::Node_Data::Input> const& value)
  {
    m_inputs = value;
  }
  void Node_Data::set_inputs(std::vector<setup::Node_Data::Input>&& value)
  {
    m_inputs = std::move(value);
  }
  auto Node_Data::get_inputs() const -> std::vector<setup::Node_Data::Input> const& 
  {
    return m_inputs;
  }

  auto Node_Data::get_inputs() -> std::vector<setup::Node_Data::Input>& 
  {
    return m_inputs;
  }

////////////////////////////////////////////////////////////
  void Node_Data::set_outputs(std::vector<setup::Node_Data::Output> const& value)
  {
    m_outputs = value;
  }
  void Node_Data::set_outputs(std::vector<setup::Node_Data::Output>&& value)
  {
    m_outputs = std::move(value);
  }
  auto Node_Data::get_outputs() const -> std::vector<setup::Node_Data::Output> const& 
  {
    return m_outputs;
  }

  auto Node_Data::get_outputs() -> std::vector<setup::Node_Data::Output>& 
  {
    return m_outputs;
  }

////////////////////////////////////////////////////////////
  void Node_Data::set_descriptor_data(setup::serialized_data_t const& value)
  {
    m_descriptor_data = value;
  }
  void Node_Data::set_descriptor_data(setup::serialized_data_t&& value)
  {
    m_descriptor_data = std::move(value);
  }
  auto Node_Data::get_descriptor_data() const -> setup::serialized_data_t const& 
  {
    return m_descriptor_data;
  }

////////////////////////////////////////////////////////////
  void Node_Data::set_config_data(setup::serialized_data_t const& value)
  {
    m_config_data = value;
  }
  void Node_Data::set_config_data(setup::serialized_data_t&& value)
  {
    m_config_data = std::move(value);
  }
  auto Node_Data::get_config_data() const -> setup::serialized_data_t const& 
  {
    return m_config_data;
  }

////////////////////////////////////////////////////////////
  void Get_Nodes_Req::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Get_Nodes_Req::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Get_Nodes_Req::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Get_Nodes_Req::set_name(std::string const& value)
  {
    m_name = value;
  }
  void Get_Nodes_Req::set_name(std::string&& value)
  {
    m_name = std::move(value);
  }
  auto Get_Nodes_Req::get_name() const -> std::string const& 
  {
    return m_name;
  }

////////////////////////////////////////////////////////////
  void Get_Nodes_Res::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Get_Nodes_Res::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Get_Nodes_Res::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Get_Nodes_Res::set_node_datas(std::vector<setup::Node_Data> const& value)
  {
    m_node_datas = value;
  }
  void Get_Nodes_Res::set_node_datas(std::vector<setup::Node_Data>&& value)
  {
    m_node_datas = std::move(value);
  }
  auto Get_Nodes_Res::get_node_datas() const -> std::vector<setup::Node_Data> const& 
  {
    return m_node_datas;
  }

  auto Get_Nodes_Res::get_node_datas() -> std::vector<setup::Node_Data>& 
  {
    return m_node_datas;
  }

////////////////////////////////////////////////////////////
  void Set_Node_Input_Stream_Path_Req::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Set_Node_Input_Stream_Path_Req::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Set_Node_Input_Stream_Path_Req::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Set_Node_Input_Stream_Path_Req::set_node_name(std::string const& value)
  {
    m_node_name = value;
  }
  void Set_Node_Input_Stream_Path_Req::set_node_name(std::string&& value)
  {
    m_node_name = std::move(value);
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
  void Set_Node_Input_Stream_Path_Req::set_input_name(std::string&& value)
  {
    m_input_name = std::move(value);
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
  void Set_Node_Input_Stream_Path_Req::set_stream_path(std::string&& value)
  {
    m_stream_path = std::move(value);
  }
  auto Set_Node_Input_Stream_Path_Req::get_stream_path() const -> std::string const& 
  {
    return m_stream_path;
  }

////////////////////////////////////////////////////////////
  void Set_Node_Input_Stream_Path_Res::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Set_Node_Input_Stream_Path_Res::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Set_Node_Input_Stream_Path_Res::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Set_Node_Input_Stream_Path_Res::set_node_data(setup::Node_Data const& value)
  {
    m_node_data = value;
  }
  void Set_Node_Input_Stream_Path_Res::set_node_data(setup::Node_Data&& value)
  {
    m_node_data = std::move(value);
  }
  auto Set_Node_Input_Stream_Path_Res::get_node_data() const -> setup::Node_Data const& 
  {
    return m_node_data;
  }

  auto Set_Node_Input_Stream_Path_Res::get_node_data() -> setup::Node_Data& 
  {
    return m_node_data;
  }

////////////////////////////////////////////////////////////
  void Set_Stream_Telemetry_Enabled_Req::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Set_Stream_Telemetry_Enabled_Req::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Set_Stream_Telemetry_Enabled_Req::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Set_Stream_Telemetry_Enabled_Req::set_stream_path(std::string const& value)
  {
    m_stream_path = value;
  }
  void Set_Stream_Telemetry_Enabled_Req::set_stream_path(std::string&& value)
  {
    m_stream_path = std::move(value);
  }
  auto Set_Stream_Telemetry_Enabled_Req::get_stream_path() const -> std::string const& 
  {
    return m_stream_path;
  }

////////////////////////////////////////////////////////////
  void Set_Stream_Telemetry_Enabled_Req::set_enabled(bool const& value)
  {
    m_enabled = value;
  }
  void Set_Stream_Telemetry_Enabled_Req::set_enabled(bool&& value)
  {
    m_enabled = std::move(value);
  }
  auto Set_Stream_Telemetry_Enabled_Req::get_enabled() const -> bool const& 
  {
    return m_enabled;
  }

////////////////////////////////////////////////////////////
  void Set_Stream_Telemetry_Enabled_Res::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Set_Stream_Telemetry_Enabled_Res::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Set_Stream_Telemetry_Enabled_Res::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Add_Node_Req::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Add_Node_Req::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Add_Node_Req::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Add_Node_Req::set_def_name(std::string const& value)
  {
    m_def_name = value;
  }
  void Add_Node_Req::set_def_name(std::string&& value)
  {
    m_def_name = std::move(value);
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
  void Add_Node_Req::set_name(std::string&& value)
  {
    m_name = std::move(value);
  }
  auto Add_Node_Req::get_name() const -> std::string const& 
  {
    return m_name;
  }

////////////////////////////////////////////////////////////
  void Add_Node_Req::set_descriptor_data(setup::serialized_data_t const& value)
  {
    m_descriptor_data = value;
  }
  void Add_Node_Req::set_descriptor_data(setup::serialized_data_t&& value)
  {
    m_descriptor_data = std::move(value);
  }
  auto Add_Node_Req::get_descriptor_data() const -> setup::serialized_data_t const& 
  {
    return m_descriptor_data;
  }

////////////////////////////////////////////////////////////
  void Add_Node_Res::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Add_Node_Res::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Add_Node_Res::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Add_Node_Res::set_node_data(setup::Node_Data const& value)
  {
    m_node_data = value;
  }
  void Add_Node_Res::set_node_data(setup::Node_Data&& value)
  {
    m_node_data = std::move(value);
  }
  auto Add_Node_Res::get_node_data() const -> setup::Node_Data const& 
  {
    return m_node_data;
  }

  auto Add_Node_Res::get_node_data() -> setup::Node_Data& 
  {
    return m_node_data;
  }

////////////////////////////////////////////////////////////
  void Set_Node_Config_Req::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Set_Node_Config_Req::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Set_Node_Config_Req::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Set_Node_Config_Req::set_name(std::string const& value)
  {
    m_name = value;
  }
  void Set_Node_Config_Req::set_name(std::string&& value)
  {
    m_name = std::move(value);
  }
  auto Set_Node_Config_Req::get_name() const -> std::string const& 
  {
    return m_name;
  }

////////////////////////////////////////////////////////////
  void Set_Node_Config_Req::set_config_data(setup::serialized_data_t const& value)
  {
    m_config_data = value;
  }
  void Set_Node_Config_Req::set_config_data(setup::serialized_data_t&& value)
  {
    m_config_data = std::move(value);
  }
  auto Set_Node_Config_Req::get_config_data() const -> setup::serialized_data_t const& 
  {
    return m_config_data;
  }

////////////////////////////////////////////////////////////
  void Set_Node_Config_Res::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Set_Node_Config_Res::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Set_Node_Config_Res::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Set_Node_Config_Res::set_node_data(setup::Node_Data const& value)
  {
    m_node_data = value;
  }
  void Set_Node_Config_Res::set_node_data(setup::Node_Data&& value)
  {
    m_node_data = std::move(value);
  }
  auto Set_Node_Config_Res::get_node_data() const -> setup::Node_Data const& 
  {
    return m_node_data;
  }

  auto Set_Node_Config_Res::get_node_data() -> setup::Node_Data& 
  {
    return m_node_data;
  }

////////////////////////////////////////////////////////////
  void Remove_Node_Req::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Remove_Node_Req::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Remove_Node_Req::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
  void Remove_Node_Req::set_name(std::string const& value)
  {
    m_name = value;
  }
  void Remove_Node_Req::set_name(std::string&& value)
  {
    m_name = std::move(value);
  }
  auto Remove_Node_Req::get_name() const -> std::string const& 
  {
    return m_name;
  }

////////////////////////////////////////////////////////////
  void Remove_Node_Res::set_req_id(uint32_t const& value)
  {
    m_req_id = value;
  }
  void Remove_Node_Res::set_req_id(uint32_t&& value)
  {
    m_req_id = std::move(value);
  }
  auto Remove_Node_Res::get_req_id() const -> uint32_t const& 
  {
    return m_req_id;
  }

////////////////////////////////////////////////////////////
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
ts::Result<void> deserialize(setup::IReq& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::IReq const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  return sz_value;
}
ts::Result<void> deserialize(setup::IRes& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::IRes const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Error& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("message");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'message'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_message())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_message(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Error const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("message", serialize(value.get_message()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Get_AST_Req& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Get_AST_Req const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Get_AST_Res& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_data(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Get_AST_Res const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("data", serialize(value.get_data()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Set_Clock_Req& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("time");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'time'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_time())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_time(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Set_Clock_Req const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("time", serialize(value.get_time()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Set_Clock_Res& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("time");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'time'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_time())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_time(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Set_Clock_Res const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("time", serialize(value.get_time()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Set_UAV_Descriptor_Req& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_data(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Set_UAV_Descriptor_Req const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("data", serialize(value.get_data()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Set_UAV_Descriptor_Res& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_data(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Set_UAV_Descriptor_Res const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("data", serialize(value.get_data()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Get_UAV_Descriptor_Req& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Get_UAV_Descriptor_Req const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Get_UAV_Descriptor_Res& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_data(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Get_UAV_Descriptor_Res const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("data", serialize(value.get_data()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Node_Def_Data::Input& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("space");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'space'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_space())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_space(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("semantic");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'semantic'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_semantic())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_semantic(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Node_Def_Data::Input const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("space", serialize(value.get_space()));
  sz_value.add_object_member("semantic", serialize(value.get_semantic()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Node_Def_Data::Output& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("space");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'space'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_space())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_space(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("semantic");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'semantic'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_semantic())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_semantic(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Node_Def_Data::Output const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("space", serialize(value.get_space()));
  sz_value.add_object_member("semantic", serialize(value.get_semantic()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Node_Def_Data& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("type");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'type'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_type())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_type(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("inputs");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'inputs'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_inputs())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_inputs(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("outputs");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'outputs'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_outputs())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_outputs(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("descriptor_data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'descriptor_data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_descriptor_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_descriptor_data(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Node_Def_Data const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(5);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("type", serialize(value.get_type()));
  sz_value.add_object_member("inputs", serialize(value.get_inputs()));
  sz_value.add_object_member("outputs", serialize(value.get_outputs()));
  sz_value.add_object_member("descriptor_data", serialize(value.get_descriptor_data()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Get_Node_Defs_Req& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Get_Node_Defs_Req const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Get_Node_Defs_Res& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("node_def_datas");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'node_def_datas'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_node_def_datas())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_node_def_datas(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Get_Node_Defs_Res const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("node_def_datas", serialize(value.get_node_def_datas()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Node_Data::Input& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("space");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'space'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_space())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_space(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("semantic");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'semantic'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_semantic())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_semantic(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("stream_path");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'stream_path'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_stream_path())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_stream_path(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Node_Data::Input const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(5);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("space", serialize(value.get_space()));
  sz_value.add_object_member("semantic", serialize(value.get_semantic()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  sz_value.add_object_member("stream_path", serialize(value.get_stream_path()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Node_Data::Output& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("space");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'space'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_space())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_space(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("semantic");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'semantic'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_semantic())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_semantic(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("rate");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'rate'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_rate())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_rate(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Node_Data::Output const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("space", serialize(value.get_space()));
  sz_value.add_object_member("semantic", serialize(value.get_semantic()));
  sz_value.add_object_member("rate", serialize(value.get_rate()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Node_Data& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("type");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'type'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_type())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_type(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("inputs");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'inputs'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_inputs())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_inputs(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("outputs");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'outputs'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_outputs())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_outputs(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("descriptor_data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'descriptor_data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_descriptor_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_descriptor_data(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("config_data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'config_data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_config_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_config_data(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Node_Data const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(6);
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("type", serialize(value.get_type()));
  sz_value.add_object_member("inputs", serialize(value.get_inputs()));
  sz_value.add_object_member("outputs", serialize(value.get_outputs()));
  sz_value.add_object_member("descriptor_data", serialize(value.get_descriptor_data()));
  sz_value.add_object_member("config_data", serialize(value.get_config_data()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Get_Nodes_Req& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Get_Nodes_Req const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("name", serialize(value.get_name()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Get_Nodes_Res& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("node_datas");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'node_datas'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_node_datas())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_node_datas(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Get_Nodes_Res const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("node_datas", serialize(value.get_node_datas()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Set_Node_Input_Stream_Path_Req& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("node_name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'node_name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_node_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_node_name(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("input_name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'input_name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_input_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_input_name(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("stream_path");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'stream_path'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_stream_path())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_stream_path(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Set_Node_Input_Stream_Path_Req const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("node_name", serialize(value.get_node_name()));
  sz_value.add_object_member("input_name", serialize(value.get_input_name()));
  sz_value.add_object_member("stream_path", serialize(value.get_stream_path()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Set_Node_Input_Stream_Path_Res& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("node_data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'node_data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_node_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_node_data(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Set_Node_Input_Stream_Path_Res const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("node_data", serialize(value.get_node_data()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Set_Stream_Telemetry_Enabled_Req& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("stream_path");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'stream_path'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_stream_path())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_stream_path(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("enabled");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'enabled'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_enabled())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_enabled(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Set_Stream_Telemetry_Enabled_Req const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("stream_path", serialize(value.get_stream_path()));
  sz_value.add_object_member("enabled", serialize(value.get_enabled()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Set_Stream_Telemetry_Enabled_Res& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Set_Stream_Telemetry_Enabled_Res const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Add_Node_Req& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("def_name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'def_name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_def_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_def_name(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("descriptor_data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'descriptor_data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_descriptor_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_descriptor_data(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Add_Node_Req const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(4);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("def_name", serialize(value.get_def_name()));
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("descriptor_data", serialize(value.get_descriptor_data()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Add_Node_Res& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("node_data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'node_data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_node_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_node_data(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Add_Node_Res const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("node_data", serialize(value.get_node_data()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Set_Node_Config_Req& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("config_data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'config_data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_config_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_config_data(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Set_Node_Config_Req const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(3);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("name", serialize(value.get_name()));
  sz_value.add_object_member("config_data", serialize(value.get_config_data()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Set_Node_Config_Res& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("node_data");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'node_data'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_node_data())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_node_data(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Set_Node_Config_Res const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("node_data", serialize(value.get_node_data()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Remove_Node_Req& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("name");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'name'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_name())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_name(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Remove_Node_Req const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(2);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  sz_value.add_object_member("name", serialize(value.get_name()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Remove_Node_Res& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  {
    auto const* member_sz_value = sz_value.find_object_member_by_name("req_id");
    if (!member_sz_value) { return ts::Error("Cannot find member value 'req_id'"); }
    std::remove_cv<std::remove_reference<decltype(value.get_req_id())>::type>::type v;
    auto result = deserialize(v, *member_sz_value);
    if (result != ts::success) { return result; }
    value.set_req_id(std::move(v));
  }
  return ts::success;
}
ts::sz::Value serialize(setup::Remove_Node_Res const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  sz_value.reserve_object_members(1);
  sz_value.add_object_member("req_id", serialize(value.get_req_id()));
  return sz_value;
}
ts::Result<void> deserialize(setup::Brain_Req& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::setup::Get_AST_Req")
  {
    value = setup::Get_AST_Req();
    auto result = deserialize(boost::get<setup::Get_AST_Req>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Set_Clock_Req")
  {
    value = setup::Set_Clock_Req();
    auto result = deserialize(boost::get<setup::Set_Clock_Req>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Set_UAV_Descriptor_Req")
  {
    value = setup::Set_UAV_Descriptor_Req();
    auto result = deserialize(boost::get<setup::Set_UAV_Descriptor_Req>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Get_UAV_Descriptor_Req")
  {
    value = setup::Get_UAV_Descriptor_Req();
    auto result = deserialize(boost::get<setup::Get_UAV_Descriptor_Req>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Get_Node_Defs_Req")
  {
    value = setup::Get_Node_Defs_Req();
    auto result = deserialize(boost::get<setup::Get_Node_Defs_Req>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Remove_Node_Req")
  {
    value = setup::Remove_Node_Req();
    auto result = deserialize(boost::get<setup::Remove_Node_Req>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Add_Node_Req")
  {
    value = setup::Add_Node_Req();
    auto result = deserialize(boost::get<setup::Add_Node_Req>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Get_Nodes_Req")
  {
    value = setup::Get_Nodes_Req();
    auto result = deserialize(boost::get<setup::Get_Nodes_Req>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Set_Node_Input_Stream_Path_Req")
  {
    value = setup::Set_Node_Input_Stream_Path_Req();
    auto result = deserialize(boost::get<setup::Set_Node_Input_Stream_Path_Req>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Set_Stream_Telemetry_Enabled_Req")
  {
    value = setup::Set_Stream_Telemetry_Enabled_Req();
    auto result = deserialize(boost::get<setup::Set_Stream_Telemetry_Enabled_Req>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Set_Node_Config_Req")
  {
    value = setup::Set_Node_Config_Req();
    auto result = deserialize(boost::get<setup::Set_Node_Config_Req>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(setup::Brain_Req const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (auto* v = boost::get<setup::Get_AST_Req>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Get_AST_Req");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Set_Clock_Req>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Set_Clock_Req");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Set_UAV_Descriptor_Req>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Set_UAV_Descriptor_Req");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Get_UAV_Descriptor_Req>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Get_UAV_Descriptor_Req");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Get_Node_Defs_Req>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Get_Node_Defs_Req");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Remove_Node_Req>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Remove_Node_Req");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Add_Node_Req>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Add_Node_Req");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Get_Nodes_Req>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Get_Nodes_Req");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Set_Node_Input_Stream_Path_Req>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Set_Node_Input_Stream_Path_Req");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Set_Stream_Telemetry_Enabled_Req>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Set_Stream_Telemetry_Enabled_Req");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Set_Node_Config_Req>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Set_Node_Config_Req");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(setup::Brain_Res& value, ts::sz::Value const& sz_value)
{
  if (!sz_value.is_object()) { return ts::Error("Expected object value when deserializing"); }
  auto const* type_sz_value = sz_value.find_object_member_by_name("type");
  if (!type_sz_value || !type_sz_value->is_string()) { return ts::Error("Expected 'type' string value when deserializing"); }
  auto const* value_sz_value = sz_value.find_object_member_by_name("value");
  if (!value_sz_value) { return ts::Error("Expected 'value' when deserializing"); }
  std::string const& path = type_sz_value->get_as_string();
  if (false) { return ts::Error(""); } //this is here just to have the next items with 'else if'
  else if (path == "::setup::Get_AST_Res")
  {
    value = setup::Get_AST_Res();
    auto result = deserialize(boost::get<setup::Get_AST_Res>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Set_Clock_Res")
  {
    value = setup::Set_Clock_Res();
    auto result = deserialize(boost::get<setup::Set_Clock_Res>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Set_UAV_Descriptor_Res")
  {
    value = setup::Set_UAV_Descriptor_Res();
    auto result = deserialize(boost::get<setup::Set_UAV_Descriptor_Res>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Get_UAV_Descriptor_Res")
  {
    value = setup::Get_UAV_Descriptor_Res();
    auto result = deserialize(boost::get<setup::Get_UAV_Descriptor_Res>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Get_Node_Defs_Res")
  {
    value = setup::Get_Node_Defs_Res();
    auto result = deserialize(boost::get<setup::Get_Node_Defs_Res>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Remove_Node_Res")
  {
    value = setup::Remove_Node_Res();
    auto result = deserialize(boost::get<setup::Remove_Node_Res>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Add_Node_Res")
  {
    value = setup::Add_Node_Res();
    auto result = deserialize(boost::get<setup::Add_Node_Res>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Get_Nodes_Res")
  {
    value = setup::Get_Nodes_Res();
    auto result = deserialize(boost::get<setup::Get_Nodes_Res>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Set_Node_Input_Stream_Path_Res")
  {
    value = setup::Set_Node_Input_Stream_Path_Res();
    auto result = deserialize(boost::get<setup::Set_Node_Input_Stream_Path_Res>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Set_Stream_Telemetry_Enabled_Res")
  {
    value = setup::Set_Stream_Telemetry_Enabled_Res();
    auto result = deserialize(boost::get<setup::Set_Stream_Telemetry_Enabled_Res>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Set_Node_Config_Res")
  {
    value = setup::Set_Node_Config_Res();
    auto result = deserialize(boost::get<setup::Set_Node_Config_Res>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else if (path == "::setup::Error")
  {
    value = setup::Error();
    auto result = deserialize(boost::get<setup::Error>(value), *value_sz_value);
    if (result != ts::success) { return result; }
  }
  else { return ts::Error("Cannot find type '" + path + "' when deserializing"); }
  return ts::success;
}
ts::sz::Value serialize(setup::Brain_Res const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::OBJECT);
  if (false) { return ts::sz::Value(); } //this is here just to have the next items with 'else if'
  else if (auto* v = boost::get<setup::Get_AST_Res>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Get_AST_Res");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Set_Clock_Res>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Set_Clock_Res");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Set_UAV_Descriptor_Res>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Set_UAV_Descriptor_Res");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Get_UAV_Descriptor_Res>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Get_UAV_Descriptor_Res");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Get_Node_Defs_Res>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Get_Node_Defs_Res");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Remove_Node_Res>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Remove_Node_Res");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Add_Node_Res>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Add_Node_Res");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Get_Nodes_Res>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Get_Nodes_Res");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Set_Node_Input_Stream_Path_Res>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Set_Node_Input_Stream_Path_Res");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Set_Stream_Telemetry_Enabled_Res>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Set_Stream_Telemetry_Enabled_Res");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Set_Node_Config_Res>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Set_Node_Config_Res");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else if (auto* v = boost::get<setup::Error>(&value))
  {
    sz_value.reserve_object_members(2);
    sz_value.add_object_member("type", "::setup::Error");
    sz_value.add_object_member("value", serialize(*v));
    return std::move(sz_value);
  }
  else { TS_ASSERT(false); return ts::sz::Value(); }
}
ts::Result<void> deserialize(std::vector<setup::Node_Def_Data::Input>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<setup::Node_Def_Data::Input> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<setup::Node_Def_Data::Output>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<setup::Node_Def_Data::Output> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<setup::Node_Def_Data>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<setup::Node_Def_Data> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<setup::Node_Data::Input>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<setup::Node_Data::Input> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<setup::Node_Data::Output>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<setup::Node_Data::Output> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
ts::Result<void> deserialize(std::vector<setup::Node_Data>& value, ts::sz::Value const& sz_value)
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
ts::sz::Value serialize(std::vector<setup::Node_Data> const& value)
{
  ts::sz::Value sz_value(ts::sz::Value::Type::ARRAY);
  sz_value.reserve_array_members(value.size());
  for (size_t i = 0; i < value.size(); i++)
  {
    sz_value.add_array_element(serialize(value[i]));
  }
  return sz_value;
}
}
}
