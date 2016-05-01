#include "def_lang/Type_System.h"
#include "def_lang/ast/Builder.h"

#include "def_lang/IMember_Def.h"
#include "def_lang/IVector_Type.h"
#include "def_lang/IPoly_Type.h"
#include "def_lang/IEnum_Type.h"
#include "def_lang/IEnum_Value.h"
#include "def_lang/IEnum_Item.h"
#include "def_lang/IStruct_Type.h"
#include "def_lang/IStruct_Value.h"
#include "def_lang/IString_Value.h"
#include "def_lang/IVector_Value.h"
#include "def_lang/impl/Namespace.h"
#include "def_lang/IDeclaration_Scope.h"
#include "def_lang/All_INumeric_Values.h"
#include "def_lang/Value_Selector.h"
#include "def_lang/impl/Initializer_List.h"
#include "def_lang/Mapper.h"
#include "def_lang/JSON_Serializer.h"

#include <chrono>

#include <boost/program_options.hpp>

static ts::Result<void> generate_code(std::string& h_file, std::string& cpp_file, ts::ast::Node const& ast_root_node, ts::Type_System const& ts);


int main(int argc, char **argv)
{
    namespace po = boost::program_options;

    std::string def_filename;
    std::string out_filename;

    po::options_description desc("Options");
    desc.add_options()
        ("help", "This help message")
        ("ast", "Print the AST")
        ("nice", "Format the AST JSON nicely")
        ("def", po::value<std::string>(&def_filename), "Definition file")
        ("out", po::value<std::string>(&out_filename), "Output file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }

    if (def_filename.empty())
    {
        std::cout << "Filename is required!\n" << desc << "\n";
        return 1;
    }

    if (out_filename.empty())
    {
        out_filename = def_filename;
    }

    std::string filename = out_filename + ".h";
    std::ofstream h_stream(filename);
    if (!h_stream.is_open())
    {
        std::cerr << "Cannot open out file '" + filename + "'";
        return 1;
    }
    filename = out_filename + ".cpp";
    std::ofstream cpp_stream(filename);
    if (!cpp_stream.is_open())
    {
        std::cerr << "Cannot open out file '" + filename + "'";
        return 1;
    }

    bool show_ast = vm.count("ast") != 0;
    bool nice_json = vm.count("nice") != 0;

    ts::ast::Builder builder;

    auto parse_result = builder.parse_file(def_filename);
    if (parse_result != ts::success)
    {
        std::cerr << parse_result.error().what();
        std::cerr.flush();
        return 1;
    }

    if (show_ast)
    {
        std::cout << builder.get_ast_root_node().to_string(0, true) << std::endl;
        std::cout.flush();
    }

    ts::Type_System ts;
    ts.populate_builtin_types();

    auto compile_result = builder.compile(ts);
    if (compile_result != ts::success)
    {
        std::cerr << compile_result.error().what();
        std::cerr.flush();
        return 1;
    }

    std::string h_file, cpp_file;
    auto result = generate_code(h_file, cpp_file, builder.get_ast_root_node(), ts);
    if (result != ts::success)
    {
        std::cerr << result.error().what();
        std::cerr.flush();
    }

    h_stream << h_file;
    cpp_stream << cpp_file;

    return 0;

    std::shared_ptr<ts::IStruct_Type> type = ts.find_specialized_symbol_by_path<ts::IStruct_Type>("silk::Rate_Controller_Config");
    TS_ASSERT(type);

    std::shared_ptr<ts::IStruct_Value> value = type->create_specialized_value();
    auto construct_result = value->construct();
    TS_ASSERT(construct_result == ts::success);

//    {
//        std::shared_ptr<ts::IString_Value> name = value->select_specialized<ts::IString_Value>("name");
//        auto result = name->set_value("silkopter");
//        TS_ASSERT(result == ts::success);

//        std::shared_ptr<ts::IVector_Value> motors = value->select_specialized<ts::IVector_Value>("motors");
//        for (size_t i = 0; i < 10; i++)
//        {
//            auto result = motors->insert_default_value(motors->get_value_count());
//            TS_ASSERT(result == ts::success);
//        }

////        std::shared_ptr<ts::IVec3f_Value> motor = value->select_specialized<ts::IVec3f_Value>("motors[0].position");
//    }

//    {
//        std::string name;
//        float mass;

//        auto result = ts::mapper::get(*value, "name", name);
//        TS_ASSERT(result == ts::success);
//        result = ts::mapper::get(*value, "mass", mass);
//        TS_ASSERT(result == ts::success);
//    }

    auto serialize_result = value->serialize();
    TS_ASSERT(serialize_result == ts::success);

    std::string json = ts::serialization::to_json(serialize_result.payload(), true);
    std::cout << json << "\n";
    std::cout.flush();

    auto deserialize_result = value->deserialize(serialize_result.payload());
    TS_ASSERT(deserialize_result == ts::success);


    {
        std::ifstream fs("../test.json");
        std::string json((std::istreambuf_iterator<char>(fs)),
                         std::istreambuf_iterator<char>());

        auto start = std::chrono::system_clock::now();
        ts::Result<ts::serialization::Value> result = ts::serialization::from_json(json);
        if (result != ts::success)
        {
            std::cerr << result.error().what();
            return 1;
        }

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
        std::cout << duration << "\n";

        ts::serialization::Value parsed = result.extract_payload();
        std::cout.flush();
    }

    return 0;
}

static ts::Result<void> generate_ast_code(std::string& o_h_file, std::string& o_cpp_file, std::string const& ident_str, std::string const& ast_json)
{
    o_h_file += ident_str + "// Returns the ast json from which a ast root node can be serialized\n";
    o_h_file += ident_str + "std::string const& get_ast_json();\n";
    o_h_file += ident_str + "// Returns the root ast node from which a typesystem can be generated\n";
    o_h_file += ident_str + "ts::Result<ts::ast::Node> get_ast_root_node();\n\n";

    o_cpp_file += ident_str + "std::string const& get_ast_json()\n";
    o_cpp_file += ident_str + "{\n";
    o_cpp_file += ident_str + "  static constexpr std::string s_json = R\"xxx(";
    o_cpp_file += ident_str + ast_json;
    o_cpp_file += ident_str + ")xxx\"\n";
    o_cpp_file += ident_str + "  return s_json;\n";
    o_cpp_file += ident_str + "}\n\n";
    o_cpp_file += ident_str + "ts::Result<ts::ast::Node> get_ast_root_node()\n";
    o_cpp_file += ident_str + "{\n";
    o_cpp_file += ident_str + "  ts::Result<ts::serialization::Value> result = ts::serialization::from_json(get_ast_json());\n";
    o_cpp_file += ident_str + "  if (result != ts::success)\n";
    o_cpp_file += ident_str + "  {\n";
    o_cpp_file += ident_str + "    return result.error();\n";
    o_cpp_file += ident_str + "  }\n";
    o_cpp_file += ident_str + "  ts::ast::Node root_node;\n";
    o_cpp_file += ident_str + "  auto deserialize_result = root_node.deserialize(result.payload());\n";
    o_cpp_file += ident_str + "  if (deserialize_result != ts::success)\n";
    o_cpp_file += ident_str + "  {\n";
    o_cpp_file += ident_str + "    return deserialize_result.error();\n";
    o_cpp_file += ident_str + "  }\n";
    o_cpp_file += ident_str + "  return root_node;\n";
    o_cpp_file += ident_str + "}\n\n";

    return ts::success;
}

static ts::Result<void> generate_declaration_scope_code(std::string& o_h_file, std::string& o_cpp_file, std::string const& ident_str, ts::IDeclaration_Scope const& ds);


static ts::Symbol_Path get_symbol_path(ts::Symbol_Path const& path)
{
    return path;
}

static ts::Symbol_Path get_native_type(ts::IType const& type)
{
//    if (ts::IVector_Type const* vector_type = dynamic_cast<ts::IVector_Type const*>(&type))
//    {
//        return "std::vector<" + get_native_type_str(*vector_type->get_inner_type()) + ">";
//    }
//    if (ts::IPoly_Type const* poly_type = dynamic_cast<ts::IPoly_Type const*>(&type))
//    {
//        return get_native_type_str(*poly_type->get_inner_type()) + " const*";
//    }
    return get_symbol_path(type.get_native_type());
}

static std::string get_value_str(ts::IValue const& value)
{
    if (ts::IBool_Value const* v = dynamic_cast<ts::IBool_Value const*>(&value))
    {
        return v->get_value() ? "true" : "false";
    }
    if (ts::IInt_Value const* v = dynamic_cast<ts::IInt_Value const*>(&value))
    {
        return std::to_string(v->get_value());
    }
    if (ts::IFloat_Value const* v = dynamic_cast<ts::IFloat_Value const*>(&value))
    {
        return std::to_string(v->get_value()) + "f";
    }
    if (ts::IDouble_Value const* v = dynamic_cast<ts::IDouble_Value const*>(&value))
    {
        return std::to_string(v->get_value());
    }
    if (ts::IVec2f_Value const* v = dynamic_cast<ts::IVec2f_Value const*>(&value))
    {
        auto vec = v->get_value();
        return std::to_string(vec.x) + "f, " + std::to_string(vec.y) + "f";
    }
    if (ts::IVec2d_Value const* v = dynamic_cast<ts::IVec2d_Value const*>(&value))
    {
        auto vec = v->get_value();
        return std::to_string(vec.x) + ", " + std::to_string(vec.y);
    }
    if (ts::IVec2i_Value const* v = dynamic_cast<ts::IVec2i_Value const*>(&value))
    {
        auto vec = v->get_value();
        return std::to_string(vec.x) + ", " + std::to_string(vec.y);
    }
    if (ts::IVec3f_Value const* v = dynamic_cast<ts::IVec3f_Value const*>(&value))
    {
        auto vec = v->get_value();
        return std::to_string(vec.x) + "f, " + std::to_string(vec.y) + "f, " + std::to_string(vec.z) + "f";
    }
    if (ts::IVec3d_Value const* v = dynamic_cast<ts::IVec3d_Value const*>(&value))
    {
        auto vec = v->get_value();
        return std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z);
    }
    if (ts::IVec3i_Value const* v = dynamic_cast<ts::IVec3i_Value const*>(&value))
    {
        auto vec = v->get_value();
        return std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z);
    }
    if (ts::IVec4f_Value const* v = dynamic_cast<ts::IVec4f_Value const*>(&value))
    {
        auto vec = v->get_value();
        return std::to_string(vec.x) + "f, " + std::to_string(vec.y) + "f, " + std::to_string(vec.z) + "f, " + std::to_string(vec.w) + "f";
    }
    if (ts::IVec4d_Value const* v = dynamic_cast<ts::IVec4d_Value const*>(&value))
    {
        auto vec = v->get_value();
        return std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ", " + std::to_string(vec.w);
    }
    if (ts::IVec4i_Value const* v = dynamic_cast<ts::IVec4i_Value const*>(&value))
    {
        auto vec = v->get_value();
        return std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ", " + std::to_string(vec.w);
    }
    if (ts::IString_Value const* v = dynamic_cast<ts::IString_Value const*>(&value))
    {
        std::string const& str = v->get_value();
        return str.empty() ? str : "\"" + v->get_value() + "\"";
    }
    if (ts::IEnum_Value const* v = dynamic_cast<ts::IEnum_Value const*>(&value))
    {
        return get_symbol_path(v->get_value()->get_symbol_path()).to_string();
    }
    return "";
}

static ts::Result<void> generate_member_def_declaration_code(std::string& o_h_file, std::string& o_cpp_file, std::string const& ident_str, ts::IMember_Def const& member_def)
{
    std::string native_type_str = get_native_type(*member_def.get_type()).to_string();

    o_h_file += ident_str +
            native_type_str +
            " " +
            member_def.get_name() +
            " = " +
            native_type_str +
            "(" +
            get_value_str(*member_def.get_default_value()) +
            ");\n";
    return ts::success;
}

static ts::Result<void> generate_struct_type_code(std::string& o_h_file, std::string& o_cpp_file, std::string const& ident_str, ts::IStruct_Type const& struct_type)
{
    std::string struct_name = get_native_type(struct_type).back();
    o_h_file += ident_str + "struct " + struct_name;
    if (struct_type.get_base_struct())
    {
        o_h_file += " : public " + get_native_type(*struct_type.get_base_struct()).to_string();
    }
    o_h_file += "\n";
    o_h_file += ident_str +  "{\n";
    o_h_file += ident_str + "public:\n";

    o_h_file += ident_str + "  virtual ~" + struct_name + "() = default;\n\n";

    auto result = generate_declaration_scope_code(o_h_file, o_cpp_file, ident_str + "  ", struct_type);
    if (result != ts::success)
    {
        return result;
    }

    o_h_file += ident_str + "private:\n";

    for (size_t i = 0; i < struct_type.get_member_def_count(); i++)
    {
        auto result = generate_member_def_declaration_code(o_h_file, o_cpp_file, ident_str + "  ", *struct_type.get_member_def(i));
        if (result != ts::success)
        {
            return result;
        }
    }

    o_h_file += ident_str + "};\n\n";

    return ts::success;
}

static ts::Result<void> generate_enum_type_code(std::string& o_h_file, std::string& o_cpp_file, std::string const& ident_str, ts::IEnum_Type const& enum_type)
{
    std::string enum_name = get_native_type(enum_type).back();
    o_h_file += ident_str + "enum class " + enum_name + "\n";
    o_h_file += ident_str + "{\n";

    for (size_t i = 0; i < enum_type.get_item_count(); i++)
    {
        std::shared_ptr<const ts::IEnum_Item> item = enum_type.get_item(i);
        o_h_file += ident_str + "  " + item->get_name() + " = " + std::to_string(item->get_integral_value()) + ",\n";
    }

    o_h_file += ident_str + "};\n\n";

    return ts::success;
}

static ts::Result<void> generate_namespace_code(std::string& o_h_file, std::string& o_cpp_file, std::string const& ident_str, ts::Namespace const& ns)
{
    o_h_file += ident_str + "namespace " + ns.get_name() + "\n";
    o_h_file += ident_str + "{\n\n";
    o_cpp_file += ident_str + "namespace " + ns.get_name() + "\n";
    o_cpp_file += ident_str + "{\n\n";

    auto result = generate_declaration_scope_code(o_h_file, o_cpp_file, ident_str, ns);
    if (result != ts::success)
    {
        return result;
    }

    o_h_file += ident_str + "}\n";
    o_cpp_file += ident_str + "}\n";

    return ts::success;
}

static ts::Result<void> generate_symbol_code(std::string& o_h_file, std::string& o_cpp_file, std::string const& ident_str, ts::ISymbol const& symbol)
{
    if (ts::IStruct_Type const* struct_type = dynamic_cast<ts::IStruct_Type const*>(&symbol))
    {
        auto result = generate_struct_type_code(o_h_file, o_cpp_file, ident_str, *struct_type);
        if (result != ts::success)
        {
            return result;
        }
    }
    else if (ts::IEnum_Type const* enum_type = dynamic_cast<ts::IEnum_Type const*>(&symbol))
    {
        auto result = generate_enum_type_code(o_h_file, o_cpp_file, ident_str, *enum_type);
        if (result != ts::success)
        {
            return result;
        }
    }
    else if (ts::Namespace const* ns = dynamic_cast<ts::Namespace const*>(&symbol))
    {
        auto result = generate_namespace_code(o_h_file, o_cpp_file, ident_str, *ns);
        if (result != ts::success)
        {
            return result;
        }
    }

    return ts::success;
}

static ts::Result<void> generate_declaration_scope_code(std::string& o_h_file, std::string& o_cpp_file, std::string const& ident_str, ts::IDeclaration_Scope const& ds)
{
    for (size_t i = 0; i < ds.get_symbol_count(); i++)
    {
        auto result = generate_symbol_code(o_h_file, o_cpp_file, ident_str, *ds.get_symbol(i));
        if (result != ts::success)
        {
            return result;
        }
    }

    return ts::success;
}

static ts::Result<void> generate_code(std::string& o_h_file, std::string& o_cpp_file, ts::ast::Node const& ast_root_node, ts::Type_System const& ts)
{
    auto serialize_result = ast_root_node.serialize();
    if (serialize_result != ts::success)
    {
        return serialize_result.error();
    }

    o_h_file += "namespace gen\n"
                "{\n\n";

    o_cpp_file += "namespace gen\n"
                  "{\n\n";

    std::string ast_json = ts::serialization::to_json(serialize_result.payload(), false);

    std::string ident_str;
    auto result = generate_ast_code(o_h_file, o_cpp_file, ident_str, ast_json);
    if (result != ts::success)
    {
        return result;
    }

    result = generate_declaration_scope_code(o_h_file, o_cpp_file, ident_str, ts);
    if (result != ts::success)
    {
        return result;
    }

    o_h_file += "}\n";
    o_cpp_file += "}\n";

    return ts::success;
}
