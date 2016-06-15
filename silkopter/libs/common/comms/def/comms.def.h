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

namespace comms
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
typedef int8_t int8_t;
typedef uint8_t uint8_t;
typedef int16_t int16_t;
typedef uint16_t uint16_t;
typedef int32_t int32_t;
typedef uint32_t uint32_t;
typedef int64_t int64_t;
typedef float ufloat;
namespace setup
{

typedef int64_t time_ms_t;
typedef std::string serialized_data_t;
struct Error
{
public:
  virtual ~Error() = default;
  void set_message(std::string const& value);
  void set_message(std::string&& value);
  auto get_message() const -> std::string const&;

private:
  std::string m_message;
};

struct Set_Clock_Req
{
public:
  virtual ~Set_Clock_Req() = default;
  void set_time(setup::time_ms_t const& value);
  void set_time(setup::time_ms_t&& value);
  auto get_time() const -> setup::time_ms_t const&;

private:
  setup::time_ms_t m_time = {0};
};

struct Set_Clock_Res
{
public:
  virtual ~Set_Clock_Res() = default;
  void set_time(setup::time_ms_t const& value);
  void set_time(setup::time_ms_t&& value);
  auto get_time() const -> setup::time_ms_t const&;

  void set_error(boost::optional<setup::Error> const& value);
  void set_error(boost::optional<setup::Error>&& value);
  auto get_error() const -> boost::optional<setup::Error> const&;
  auto get_error() -> boost::optional<setup::Error>&;

private:
  setup::time_ms_t m_time = {0};
  boost::optional<setup::Error> m_error;
};

struct Set_UAV_Descriptor_Req
{
public:
  virtual ~Set_UAV_Descriptor_Req() = default;
  void set_data(setup::serialized_data_t const& value);
  void set_data(setup::serialized_data_t&& value);
  auto get_data() const -> setup::serialized_data_t const&;

private:
  setup::serialized_data_t m_data;
};

struct Set_UAV_Descriptor_Res
{
public:
  virtual ~Set_UAV_Descriptor_Res() = default;
  void set_data(setup::serialized_data_t const& value);
  void set_data(setup::serialized_data_t&& value);
  auto get_data() const -> setup::serialized_data_t const&;

private:
  setup::serialized_data_t m_data;
};

struct Get_UAV_Descriptor_Req
{
public:
  virtual ~Get_UAV_Descriptor_Req() = default;
private:
};

struct Get_UAV_Descriptor_Res
{
public:
  virtual ~Get_UAV_Descriptor_Res() = default;
  void set_data(setup::serialized_data_t const& value);
  void set_data(setup::serialized_data_t&& value);
  auto get_data() const -> setup::serialized_data_t const&;

private:
  setup::serialized_data_t m_data;
};

struct Node_Def_Data
{
public:
  struct Input
  {
  public:
    virtual ~Input() = default;
    void set_name(std::string const& value);
    void set_name(std::string&& value);
    auto get_name() const -> std::string const&;

    void set_space(uint8_t const& value);
    void set_space(uint8_t&& value);
    auto get_space() const -> uint8_t const&;

    void set_semantic(uint8_t const& value);
    void set_semantic(uint8_t&& value);
    auto get_semantic() const -> uint8_t const&;

    void set_rate(uint32_t const& value);
    void set_rate(uint32_t&& value);
    auto get_rate() const -> uint32_t const&;

  private:
    std::string m_name;
    uint8_t m_space = {0};
    uint8_t m_semantic = {0};
    uint32_t m_rate = {0};
  };

  struct Output
  {
  public:
    virtual ~Output() = default;
    void set_name(std::string const& value);
    void set_name(std::string&& value);
    auto get_name() const -> std::string const&;

    void set_space(uint8_t const& value);
    void set_space(uint8_t&& value);
    auto get_space() const -> uint8_t const&;

    void set_semantic(uint8_t const& value);
    void set_semantic(uint8_t&& value);
    auto get_semantic() const -> uint8_t const&;

    void set_rate(uint32_t const& value);
    void set_rate(uint32_t&& value);
    auto get_rate() const -> uint32_t const&;

  private:
    std::string m_name;
    uint8_t m_space = {0};
    uint8_t m_semantic = {0};
    uint32_t m_rate = {0};
  };

  virtual ~Node_Def_Data() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

  void set_type(uint8_t const& value);
  void set_type(uint8_t&& value);
  auto get_type() const -> uint8_t const&;

  void set_inputs(std::vector<setup::Node_Def_Data::Input> const& value);
  void set_inputs(std::vector<setup::Node_Def_Data::Input>&& value);
  auto get_inputs() const -> std::vector<setup::Node_Def_Data::Input> const&;
  auto get_inputs() -> std::vector<setup::Node_Def_Data::Input>&;

  void set_outputs(std::vector<setup::Node_Def_Data::Output> const& value);
  void set_outputs(std::vector<setup::Node_Def_Data::Output>&& value);
  auto get_outputs() const -> std::vector<setup::Node_Def_Data::Output> const&;
  auto get_outputs() -> std::vector<setup::Node_Def_Data::Output>&;

  void set_descriptor_data(setup::serialized_data_t const& value);
  void set_descriptor_data(setup::serialized_data_t&& value);
  auto get_descriptor_data() const -> setup::serialized_data_t const&;

private:
  std::string m_name;
  uint8_t m_type = {0};
  std::vector<setup::Node_Def_Data::Input> m_inputs;
  std::vector<setup::Node_Def_Data::Output> m_outputs;
  setup::serialized_data_t m_descriptor_data;
};

struct Get_Node_Defs_Req
{
public:
  virtual ~Get_Node_Defs_Req() = default;
private:
};

struct Get_Node_Defs_Res
{
public:
  virtual ~Get_Node_Defs_Res() = default;
  void set_node_def_datas(std::vector<setup::Node_Def_Data> const& value);
  void set_node_def_datas(std::vector<setup::Node_Def_Data>&& value);
  auto get_node_def_datas() const -> std::vector<setup::Node_Def_Data> const&;
  auto get_node_def_datas() -> std::vector<setup::Node_Def_Data>&;

private:
  std::vector<setup::Node_Def_Data> m_node_def_datas;
};

struct Node_Data
{
public:
  struct Input
  {
  public:
    virtual ~Input() = default;
    void set_name(std::string const& value);
    void set_name(std::string&& value);
    auto get_name() const -> std::string const&;

    void set_space(uint8_t const& value);
    void set_space(uint8_t&& value);
    auto get_space() const -> uint8_t const&;

    void set_semantic(uint8_t const& value);
    void set_semantic(uint8_t&& value);
    auto get_semantic() const -> uint8_t const&;

    void set_rate(uint32_t const& value);
    void set_rate(uint32_t&& value);
    auto get_rate() const -> uint32_t const&;

    void set_stream_path(std::string const& value);
    void set_stream_path(std::string&& value);
    auto get_stream_path() const -> std::string const&;

  private:
    std::string m_name;
    uint8_t m_space = {0};
    uint8_t m_semantic = {0};
    uint32_t m_rate = {0};
    std::string m_stream_path;
  };

  struct Output
  {
  public:
    virtual ~Output() = default;
    void set_name(std::string const& value);
    void set_name(std::string&& value);
    auto get_name() const -> std::string const&;

    void set_space(uint8_t const& value);
    void set_space(uint8_t&& value);
    auto get_space() const -> uint8_t const&;

    void set_semantic(uint8_t const& value);
    void set_semantic(uint8_t&& value);
    auto get_semantic() const -> uint8_t const&;

    void set_rate(uint32_t const& value);
    void set_rate(uint32_t&& value);
    auto get_rate() const -> uint32_t const&;

  private:
    std::string m_name;
    uint8_t m_space = {0};
    uint8_t m_semantic = {0};
    uint32_t m_rate = {0};
  };

  virtual ~Node_Data() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

  void set_type(uint8_t const& value);
  void set_type(uint8_t&& value);
  auto get_type() const -> uint8_t const&;

  void set_inputs(std::vector<setup::Node_Data::Input> const& value);
  void set_inputs(std::vector<setup::Node_Data::Input>&& value);
  auto get_inputs() const -> std::vector<setup::Node_Data::Input> const&;
  auto get_inputs() -> std::vector<setup::Node_Data::Input>&;

  void set_outputs(std::vector<setup::Node_Data::Output> const& value);
  void set_outputs(std::vector<setup::Node_Data::Output>&& value);
  auto get_outputs() const -> std::vector<setup::Node_Data::Output> const&;
  auto get_outputs() -> std::vector<setup::Node_Data::Output>&;

  void set_descriptor_data(setup::serialized_data_t const& value);
  void set_descriptor_data(setup::serialized_data_t&& value);
  auto get_descriptor_data() const -> setup::serialized_data_t const&;

  void set_config_data(setup::serialized_data_t const& value);
  void set_config_data(setup::serialized_data_t&& value);
  auto get_config_data() const -> setup::serialized_data_t const&;

private:
  std::string m_name;
  uint8_t m_type = {0};
  std::vector<setup::Node_Data::Input> m_inputs;
  std::vector<setup::Node_Data::Output> m_outputs;
  setup::serialized_data_t m_descriptor_data;
  setup::serialized_data_t m_config_data;
};

struct Get_Nodes_Req
{
public:
  virtual ~Get_Nodes_Req() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

private:
  std::string m_name;
};

struct Get_Nodes_Res
{
public:
  virtual ~Get_Nodes_Res() = default;
  void set_node_datas(std::vector<setup::Node_Data> const& value);
  void set_node_datas(std::vector<setup::Node_Data>&& value);
  auto get_node_datas() const -> std::vector<setup::Node_Data> const&;
  auto get_node_datas() -> std::vector<setup::Node_Data>&;

private:
  std::vector<setup::Node_Data> m_node_datas;
};

struct Set_Node_Input_Stream_Path_Req
{
public:
  virtual ~Set_Node_Input_Stream_Path_Req() = default;
  void set_node_name(std::string const& value);
  void set_node_name(std::string&& value);
  auto get_node_name() const -> std::string const&;

  void set_input_name(std::string const& value);
  void set_input_name(std::string&& value);
  auto get_input_name() const -> std::string const&;

  void set_stream_path(std::string const& value);
  void set_stream_path(std::string&& value);
  auto get_stream_path() const -> std::string const&;

private:
  std::string m_node_name;
  std::string m_input_name;
  std::string m_stream_path;
};

struct Set_Node_Input_Stream_Path_Res
{
public:
  virtual ~Set_Node_Input_Stream_Path_Res() = default;
  void set_node_data(setup::Node_Data const& value);
  void set_node_data(setup::Node_Data&& value);
  auto get_node_data() const -> setup::Node_Data const&;
  auto get_node_data() -> setup::Node_Data&;

private:
  setup::Node_Data m_node_data;
};

struct Add_Node_Req
{
public:
  virtual ~Add_Node_Req() = default;
  void set_def_name(std::string const& value);
  void set_def_name(std::string&& value);
  auto get_def_name() const -> std::string const&;

  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

  void set_descriptor_data(setup::serialized_data_t const& value);
  void set_descriptor_data(setup::serialized_data_t&& value);
  auto get_descriptor_data() const -> setup::serialized_data_t const&;

private:
  std::string m_def_name;
  std::string m_name;
  setup::serialized_data_t m_descriptor_data;
};

struct Add_Node_Res
{
public:
  virtual ~Add_Node_Res() = default;
  void set_node_data(setup::Node_Data const& value);
  void set_node_data(setup::Node_Data&& value);
  auto get_node_data() const -> setup::Node_Data const&;
  auto get_node_data() -> setup::Node_Data&;

private:
  setup::Node_Data m_node_data;
};

struct Remove_Node_Req
{
public:
  virtual ~Remove_Node_Req() = default;
  void set_name(std::string const& value);
  void set_name(std::string&& value);
  auto get_name() const -> std::string const&;

private:
  std::string m_name;
};

struct Remove_Node_Res
{
public:
  virtual ~Remove_Node_Res() = default;
private:
};

typedef boost::variant<setup::Set_Clock_Req,setup::Set_UAV_Descriptor_Req,setup::Get_UAV_Descriptor_Req,setup::Get_Node_Defs_Req,setup::Remove_Node_Req,setup::Add_Node_Req,setup::Get_Nodes_Req,setup::Set_Node_Input_Stream_Path_Req> Brain_Req;
typedef boost::variant<setup::Set_Clock_Res,setup::Set_UAV_Descriptor_Res,setup::Get_UAV_Descriptor_Res,setup::Get_Node_Defs_Res,setup::Remove_Node_Res,setup::Add_Node_Res,setup::Get_Nodes_Res,setup::Set_Node_Input_Stream_Path_Res,setup::Error> Brain_Res;
}
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
ts::Result<void> deserialize(setup::Error& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Error const& value);
ts::Result<void> deserialize(setup::Set_Clock_Req& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Set_Clock_Req const& value);
ts::Result<void> deserialize(setup::Set_Clock_Res& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Set_Clock_Res const& value);
ts::Result<void> deserialize(setup::Set_UAV_Descriptor_Req& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Set_UAV_Descriptor_Req const& value);
ts::Result<void> deserialize(setup::Set_UAV_Descriptor_Res& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Set_UAV_Descriptor_Res const& value);
ts::Result<void> deserialize(setup::Get_UAV_Descriptor_Req& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Get_UAV_Descriptor_Req const& value);
ts::Result<void> deserialize(setup::Get_UAV_Descriptor_Res& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Get_UAV_Descriptor_Res const& value);
ts::Result<void> deserialize(setup::Node_Def_Data::Input& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Node_Def_Data::Input const& value);
ts::Result<void> deserialize(setup::Node_Def_Data::Output& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Node_Def_Data::Output const& value);
ts::Result<void> deserialize(setup::Node_Def_Data& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Node_Def_Data const& value);
ts::Result<void> deserialize(setup::Get_Node_Defs_Req& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Get_Node_Defs_Req const& value);
ts::Result<void> deserialize(setup::Get_Node_Defs_Res& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Get_Node_Defs_Res const& value);
ts::Result<void> deserialize(setup::Node_Data::Input& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Node_Data::Input const& value);
ts::Result<void> deserialize(setup::Node_Data::Output& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Node_Data::Output const& value);
ts::Result<void> deserialize(setup::Node_Data& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Node_Data const& value);
ts::Result<void> deserialize(setup::Get_Nodes_Req& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Get_Nodes_Req const& value);
ts::Result<void> deserialize(setup::Get_Nodes_Res& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Get_Nodes_Res const& value);
ts::Result<void> deserialize(setup::Set_Node_Input_Stream_Path_Req& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Set_Node_Input_Stream_Path_Req const& value);
ts::Result<void> deserialize(setup::Set_Node_Input_Stream_Path_Res& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Set_Node_Input_Stream_Path_Res const& value);
ts::Result<void> deserialize(setup::Add_Node_Req& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Add_Node_Req const& value);
ts::Result<void> deserialize(setup::Add_Node_Res& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Add_Node_Res const& value);
ts::Result<void> deserialize(setup::Remove_Node_Req& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Remove_Node_Req const& value);
ts::Result<void> deserialize(setup::Remove_Node_Res& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Remove_Node_Res const& value);
ts::Result<void> deserialize(setup::Brain_Req& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Brain_Req const& value);
ts::Result<void> deserialize(setup::Brain_Res& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(setup::Brain_Res const& value);
ts::Result<void> deserialize(boost::optional<setup::Error>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(boost::optional<setup::Error> const& value);
ts::Result<void> deserialize(std::vector<setup::Node_Def_Data::Input>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<setup::Node_Def_Data::Input> const& value);
ts::Result<void> deserialize(std::vector<setup::Node_Def_Data::Output>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<setup::Node_Def_Data::Output> const& value);
ts::Result<void> deserialize(std::vector<setup::Node_Def_Data>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<setup::Node_Def_Data> const& value);
ts::Result<void> deserialize(std::vector<setup::Node_Data::Input>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<setup::Node_Data::Input> const& value);
ts::Result<void> deserialize(std::vector<setup::Node_Data::Output>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<setup::Node_Data::Output> const& value);
ts::Result<void> deserialize(std::vector<setup::Node_Data>& value, ts::sz::Value const& sz_value);
ts::sz::Value serialize(std::vector<setup::Node_Data> const& value);
}
}
