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

namespace comms
{

struct Set_Clock_Req
{
public:

  Set_Clock_Req() noexcept {};
  virtual ~Set_Clock_Req() noexcept {};

  void set_time(int64_t const& value);
  auto get_time() const -> int64_t const&;


private:

  int64_t m_time = {0};
};

struct Set_Clock_Res
{
public:

  Set_Clock_Res() noexcept {};
  virtual ~Set_Clock_Res() noexcept {};

  void set_time(int64_t const& value);
  auto get_time() const -> int64_t const&;


private:

  int64_t m_time = {0};
};

struct Set_UAV_Descriptor_Req
{
public:

  Set_UAV_Descriptor_Req() noexcept {};
  virtual ~Set_UAV_Descriptor_Req() noexcept {};

  void set_data(std::string const& value);
  auto get_data() const -> std::string const&;


private:

  std::string m_data = {};
};

struct Set_UAV_Descriptor_Res
{
public:

  Set_UAV_Descriptor_Res() noexcept {};
  virtual ~Set_UAV_Descriptor_Res() noexcept {};

  void set_data(std::string const& value);
  auto get_data() const -> std::string const&;


private:

  std::string m_data = {};
};

struct Get_UAV_Descriptor_Req
{
public:

  Get_UAV_Descriptor_Req() noexcept {};
  virtual ~Get_UAV_Descriptor_Req() noexcept {};


private:

};

struct Get_UAV_Descriptor_Res
{
public:

  Get_UAV_Descriptor_Res() noexcept {};
  virtual ~Get_UAV_Descriptor_Res() noexcept {};

  void set_data(std::string const& value);
  auto get_data() const -> std::string const&;


private:

  std::string m_data = {};
};

struct Node_Def_Data
{
public:

  struct Input
  {
  public:

    Input() noexcept {};
    virtual ~Input() noexcept {};

    void set_name(std::string const& value);
    auto get_name() const -> std::string const&;

    void set_type(std::string const& value);
    auto get_type() const -> std::string const&;

    void set_rate(uint32_t const& value);
    auto get_rate() const -> uint32_t const&;


  private:

    std::string m_name = {};
    std::string m_type = {};
    uint32_t m_rate = {0};
  };

  struct Output
  {
  public:

    Output() noexcept {};
    virtual ~Output() noexcept {};

    void set_name(std::string const& value);
    auto get_name() const -> std::string const&;

    void set_type(std::string const& value);
    auto get_type() const -> std::string const&;

    void set_rate(uint32_t const& value);
    auto get_rate() const -> uint32_t const&;


  private:

    std::string m_name = {};
    std::string m_type = {};
    uint32_t m_rate = {0};
  };

  Node_Def_Data() noexcept {};
  virtual ~Node_Def_Data() noexcept {};

  void set_name(std::string const& value);
  auto get_name() const -> std::string const&;

  void set_type(std::string const& value);
  auto get_type() const -> std::string const&;

  void set_inputs(std::vector<::silk::comms::Node_Def_Data::Input> const& value);
  auto get_inputs() const -> std::vector<::silk::comms::Node_Def_Data::Input> const&;
  auto get_inputs() -> std::vector<::silk::comms::Node_Def_Data::Input>&;

  void set_outputs(std::vector<::silk::comms::Node_Def_Data::Output> const& value);
  auto get_outputs() const -> std::vector<::silk::comms::Node_Def_Data::Output> const&;
  auto get_outputs() -> std::vector<::silk::comms::Node_Def_Data::Output>&;

  void set_descriptor_data(std::string const& value);
  auto get_descriptor_data() const -> std::string const&;


private:

  std::string m_name = {};
  std::string m_type = {};
  std::vector<::silk::comms::Node_Def_Data::Input> m_inputs = {};
  std::vector<::silk::comms::Node_Def_Data::Output> m_outputs = {};
  std::string m_descriptor_data = {};
};

struct Get_Node_Defs_Req
{
public:

  Get_Node_Defs_Req() noexcept {};
  virtual ~Get_Node_Defs_Req() noexcept {};


private:

};

struct Get_Node_Defs_Res
{
public:

  Get_Node_Defs_Res() noexcept {};
  virtual ~Get_Node_Defs_Res() noexcept {};

  void set_node_def_datas(std::vector<::silk::comms::Node_Def_Data> const& value);
  auto get_node_def_datas() const -> std::vector<::silk::comms::Node_Def_Data> const&;
  auto get_node_def_datas() -> std::vector<::silk::comms::Node_Def_Data>&;


private:

  std::vector<::silk::comms::Node_Def_Data> m_node_def_datas = {};
};

struct Node_Data
{
public:

  struct Input
  {
  public:

    Input() noexcept {};
    virtual ~Input() noexcept {};

    void set_name(std::string const& value);
    auto get_name() const -> std::string const&;

    void set_type(std::string const& value);
    auto get_type() const -> std::string const&;

    void set_rate(uint32_t const& value);
    auto get_rate() const -> uint32_t const&;

    void set_stream_path(std::string const& value);
    auto get_stream_path() const -> std::string const&;


  private:

    std::string m_name = {};
    std::string m_type = {};
    uint32_t m_rate = {0};
    std::string m_stream_path = {};
  };

  struct Output
  {
  public:

    Output() noexcept {};
    virtual ~Output() noexcept {};

    void set_name(std::string const& value);
    auto get_name() const -> std::string const&;

    void set_type(std::string const& value);
    auto get_type() const -> std::string const&;

    void set_rate(uint32_t const& value);
    auto get_rate() const -> uint32_t const&;


  private:

    std::string m_name = {};
    std::string m_type = {};
    uint32_t m_rate = {0};
  };

  Node_Data() noexcept {};
  virtual ~Node_Data() noexcept {};

  void set_name(std::string const& value);
  auto get_name() const -> std::string const&;

  void set_type(std::string const& value);
  auto get_type() const -> std::string const&;

  void set_inputs(std::vector<::silk::comms::Node_Data::Input> const& value);
  auto get_inputs() const -> std::vector<::silk::comms::Node_Data::Input> const&;
  auto get_inputs() -> std::vector<::silk::comms::Node_Data::Input>&;

  void set_outputs(std::vector<::silk::comms::Node_Data::Output> const& value);
  auto get_outputs() const -> std::vector<::silk::comms::Node_Data::Output> const&;
  auto get_outputs() -> std::vector<::silk::comms::Node_Data::Output>&;

  void set_descriptor_data(std::string const& value);
  auto get_descriptor_data() const -> std::string const&;

  void set_config_data(std::string const& value);
  auto get_config_data() const -> std::string const&;


private:

  std::string m_name = {};
  std::string m_type = {};
  std::vector<::silk::comms::Node_Data::Input> m_inputs = {};
  std::vector<::silk::comms::Node_Data::Output> m_outputs = {};
  std::string m_descriptor_data = {};
  std::string m_config_data = {};
};

struct Get_Nodes_Req
{
public:

  Get_Nodes_Req() noexcept {};
  virtual ~Get_Nodes_Req() noexcept {};

  void set_name(std::string const& value);
  auto get_name() const -> std::string const&;


private:

  std::string m_name = {};
};

struct Get_Nodes_Res
{
public:

  Get_Nodes_Res() noexcept {};
  virtual ~Get_Nodes_Res() noexcept {};

  void set_node_datas(std::vector<::silk::comms::Node_Data> const& value);
  auto get_node_datas() const -> std::vector<::silk::comms::Node_Data> const&;
  auto get_node_datas() -> std::vector<::silk::comms::Node_Data>&;


private:

  std::vector<::silk::comms::Node_Data> m_node_datas = {};
};

struct Set_Node_Input_Stream_Path_Req
{
public:

  Set_Node_Input_Stream_Path_Req() noexcept {};
  virtual ~Set_Node_Input_Stream_Path_Req() noexcept {};

  void set_node_name(std::string const& value);
  auto get_node_name() const -> std::string const&;

  void set_input_name(std::string const& value);
  auto get_input_name() const -> std::string const&;

  void set_stream_path(std::string const& value);
  auto get_stream_path() const -> std::string const&;


private:

  std::string m_node_name = {};
  std::string m_input_name = {};
  std::string m_stream_path = {};
};

struct Set_Node_Input_Stream_Path_Res
{
public:

  struct Error
  {
  public:

    Error() noexcept {};
    virtual ~Error() noexcept {};

    void set_message(std::string const& value);
    auto get_message() const -> std::string const&;


  private:

    std::string m_message = {};
  };

  Set_Node_Input_Stream_Path_Res() noexcept {};
  virtual ~Set_Node_Input_Stream_Path_Res() noexcept {};

  void set_result(boost::variant<::silk::comms::Node_Data,Error> const& value);
  auto get_result() const -> boost::variant<::silk::comms::Node_Data,Error> const&;
  auto get_result() -> boost::variant<::silk::comms::Node_Data,Error>&;


private:

  boost::variant<::silk::comms::Node_Data,Error> m_result = {};
};

struct Add_Node_Req
{
public:

  Add_Node_Req() noexcept {};
  virtual ~Add_Node_Req() noexcept {};

  void set_def_name(std::string const& value);
  auto get_def_name() const -> std::string const&;

  void set_name(std::string const& value);
  auto get_name() const -> std::string const&;

  void set_descriptor_data(std::string const& value);
  auto get_descriptor_data() const -> std::string const&;


private:

  std::string m_def_name = {};
  std::string m_name = {};
  std::string m_descriptor_data = {};
};

struct Add_Node_Res
{
public:

  struct Error
  {
  public:

    Error() noexcept {};
    virtual ~Error() noexcept {};

    void set_message(std::string const& value);
    auto get_message() const -> std::string const&;


  private:

    std::string m_message = {};
  };

  Add_Node_Res() noexcept {};
  virtual ~Add_Node_Res() noexcept {};

  void set_result(boost::variant<::silk::comms::Node_Data,Error> const& value);
  auto get_result() const -> boost::variant<::silk::comms::Node_Data,Error> const&;
  auto get_result() -> boost::variant<::silk::comms::Node_Data,Error>&;


private:

  boost::variant<::silk::comms::Node_Data,Error> m_result = {};
};

struct Remove_Node_Req
{
public:

  Remove_Node_Req() noexcept {};
  virtual ~Remove_Node_Req() noexcept {};

  void set_name(std::string const& value);
  auto get_name() const -> std::string const&;


private:

  std::string m_name = {};
};

struct Remove_Node_Res
{
public:

  struct Error
  {
  public:

    Error() noexcept {};
    virtual ~Error() noexcept {};

    void set_message(std::string const& value);
    auto get_message() const -> std::string const&;


  private:

    std::string m_message = {};
  };

  Remove_Node_Res() noexcept {};
  virtual ~Remove_Node_Res() noexcept {};

  void set_error(boost::optional<Error> const& value);
  auto get_error() const -> boost::optional<Error> const&;
  auto get_error() -> boost::optional<Error>&;


private:

  boost::optional<Error> m_error = {};
};

}
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
ts::Result<void> deserialize(::silk::comms::Set_Clock_Req& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_Clock_Req const& value);
ts::Result<void> deserialize(::silk::comms::Set_Clock_Res& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_Clock_Res const& value);
ts::Result<void> deserialize(::silk::comms::Set_UAV_Descriptor_Req& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_UAV_Descriptor_Req const& value);
ts::Result<void> deserialize(::silk::comms::Set_UAV_Descriptor_Res& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_UAV_Descriptor_Res const& value);
ts::Result<void> deserialize(::silk::comms::Get_UAV_Descriptor_Req& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Get_UAV_Descriptor_Req const& value);
ts::Result<void> deserialize(::silk::comms::Get_UAV_Descriptor_Res& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Get_UAV_Descriptor_Res const& value);
ts::Result<void> deserialize(::silk::comms::Node_Def_Data::Input& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Node_Def_Data::Input const& value);
ts::Result<void> deserialize(::silk::comms::Node_Def_Data::Output& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Node_Def_Data::Output const& value);
ts::Result<void> deserialize(::silk::comms::Node_Def_Data& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Node_Def_Data const& value);
ts::Result<void> deserialize(::silk::comms::Get_Node_Defs_Req& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Get_Node_Defs_Req const& value);
ts::Result<void> deserialize(::silk::comms::Get_Node_Defs_Res& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Get_Node_Defs_Res const& value);
ts::Result<void> deserialize(::silk::comms::Node_Data::Input& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Node_Data::Input const& value);
ts::Result<void> deserialize(::silk::comms::Node_Data::Output& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Node_Data::Output const& value);
ts::Result<void> deserialize(::silk::comms::Node_Data& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Node_Data const& value);
ts::Result<void> deserialize(::silk::comms::Get_Nodes_Req& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Get_Nodes_Req const& value);
ts::Result<void> deserialize(::silk::comms::Get_Nodes_Res& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Get_Nodes_Res const& value);
ts::Result<void> deserialize(::silk::comms::Set_Node_Input_Stream_Path_Req& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_Node_Input_Stream_Path_Req const& value);
ts::Result<void> deserialize(::silk::comms::Set_Node_Input_Stream_Path_Res::Error& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_Node_Input_Stream_Path_Res::Error const& value);
ts::Result<void> deserialize(::silk::comms::Set_Node_Input_Stream_Path_Res& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Set_Node_Input_Stream_Path_Res const& value);
ts::Result<void> deserialize(::silk::comms::Add_Node_Req& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Add_Node_Req const& value);
ts::Result<void> deserialize(::silk::comms::Add_Node_Res::Error& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Add_Node_Res::Error const& value);
ts::Result<void> deserialize(::silk::comms::Add_Node_Res& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Add_Node_Res const& value);
ts::Result<void> deserialize(::silk::comms::Remove_Node_Req& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Remove_Node_Req const& value);
ts::Result<void> deserialize(::silk::comms::Remove_Node_Res::Error& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Remove_Node_Res::Error const& value);
ts::Result<void> deserialize(::silk::comms::Remove_Node_Res& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(::silk::comms::Remove_Node_Res const& value);
ts::Result<void> deserialize(std::vector<::silk::comms::Node_Def_Data::Input>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::comms::Node_Def_Data::Input> const& value);
ts::Result<void> deserialize(std::vector<::silk::comms::Node_Def_Data::Output>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::comms::Node_Def_Data::Output> const& value);
ts::Result<void> deserialize(std::vector<::silk::comms::Node_Def_Data>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::comms::Node_Def_Data> const& value);
ts::Result<void> deserialize(std::vector<::silk::comms::Node_Data::Input>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::comms::Node_Data::Input> const& value);
ts::Result<void> deserialize(std::vector<::silk::comms::Node_Data::Output>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::comms::Node_Data::Output> const& value);
ts::Result<void> deserialize(std::vector<::silk::comms::Node_Data>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(std::vector<::silk::comms::Node_Data> const& value);
ts::Result<void> deserialize(boost::variant<silk::comms::Node_Data,silk::comms::Set_Node_Input_Stream_Path_Res::Error>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(boost::variant<silk::comms::Node_Data,silk::comms::Set_Node_Input_Stream_Path_Res::Error> const& value);
ts::Result<void> deserialize(boost::variant<silk::comms::Node_Data,silk::comms::Add_Node_Res::Error>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(boost::variant<silk::comms::Node_Data,silk::comms::Add_Node_Res::Error> const& value);
ts::Result<void> deserialize(boost::optional<silk::comms::Remove_Node_Res::Error>& value, ts::serialization::Value const& sz_value);
ts::Result<ts::serialization::Value> serialize(boost::optional<silk::comms::Remove_Node_Res::Error> const& value);
