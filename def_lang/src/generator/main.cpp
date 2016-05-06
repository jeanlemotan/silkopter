#include "def_lang/Type_System.h"
#include "def_lang/ast/Builder.h"

#include "def_lang/IMember_Def.h"
#include "def_lang/IVector_Type.h"
#include "def_lang/IVariant_Type.h"
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


struct Context
{
    Context(std::string& o_h_file, std::string& o_cpp_file, ts::IDeclaration_Scope const& parent_scope)
        : h_file(o_h_file)
        , cpp_file(o_cpp_file)
        , parent_scope(parent_scope)
    {}
    std::string h_filename;
    std::string& h_file;
    std::string& cpp_file;
    ts::IDeclaration_Scope const& parent_scope;
    std::string ident_str;
};


static ts::Result<void> generate_code(Context& context, ts::ast::Node const& ast_root_node, ts::Type_System const& ts);

static ts::Symbol_Path s_namespace;


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
        ("namespace", po::value<std::string>(), "The namespace where to put it all")
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

    std::string h_filename = out_filename + ".h";
    std::ofstream h_stream(h_filename);
    if (!h_stream.is_open())
    {
        std::cerr << "Cannot open out file '" + h_filename + "'";
        return 1;
    }
    std::string cpp_filename = out_filename + ".cpp";
    std::ofstream cpp_stream(cpp_filename);
    if (!cpp_stream.is_open())
    {
        std::cerr << "Cannot open out file '" + cpp_filename + "'";
        return 1;
    }

    bool show_ast = vm.count("ast") != 0;
    bool nice_json = vm.count("nice") != 0;
    s_namespace = vm.count("namespace") ? ts::Symbol_Path(vm["namespace"].as<std::string>()) : ts::Symbol_Path();

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
    Context context(h_file, cpp_file, ts);
    context.h_filename = h_filename;
    auto result = generate_code(context, builder.get_ast_root_node(), ts);
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

static ts::Result<void> generate_ast_code(Context& context, std::string const& ast_json)
{
    context.h_file += context.ident_str + "// Returns the ast json from which a ast root node can be serialized\n";
    context.h_file += context.ident_str + "std::string const& get_ast_json();\n";
    context.h_file += context.ident_str + "// Returns the root ast node from which a typesystem can be generated\n";
    context.h_file += context.ident_str + "ts::Result<ts::ast::Node> get_ast_root_node();\n\n";

    context.cpp_file += context.ident_str + "std::string const& get_ast_json()\n";
    context.cpp_file += context.ident_str + "{\n";
    context.cpp_file += context.ident_str + "  static const std::string s_json = R\"xxx(";
    context.cpp_file += context.ident_str + ast_json;
    context.cpp_file += context.ident_str + ")xxx\";\n";
    context.cpp_file += context.ident_str + "  return s_json;\n";
    context.cpp_file += context.ident_str + "}\n\n";
    context.cpp_file += context.ident_str + "ts::Result<ts::ast::Node> get_ast_root_node()\n";
    context.cpp_file += context.ident_str + "{\n";
    context.cpp_file += context.ident_str + "  ts::Result<ts::serialization::Value> result = ts::serialization::from_json(get_ast_json());\n";
    context.cpp_file += context.ident_str + "  if (result != ts::success)\n";
    context.cpp_file += context.ident_str + "  {\n";
    context.cpp_file += context.ident_str + "    return result.error();\n";
    context.cpp_file += context.ident_str + "  }\n";
    context.cpp_file += context.ident_str + "  ts::ast::Node root_node;\n";
    context.cpp_file += context.ident_str + "  auto deserialize_result = root_node.deserialize(result.payload());\n";
    context.cpp_file += context.ident_str + "  if (deserialize_result != ts::success)\n";
    context.cpp_file += context.ident_str + "  {\n";
    context.cpp_file += context.ident_str + "    return deserialize_result.error();\n";
    context.cpp_file += context.ident_str + "  }\n";
    context.cpp_file += context.ident_str + "  return root_node;\n";
    context.cpp_file += context.ident_str + "}\n\n";

    return ts::success;
}

static ts::Result<void> generate_declaration_scope_code(Context& context, ts::IDeclaration_Scope const& ds);
static ts::Symbol_Path get_type_relative_scope_path(ts::IDeclaration_Scope const& scope, ts::IType const& type);


static ts::Symbol_Path get_symbol_path(ts::Symbol_Path const& path)
{
    return path;
}

static ts::Symbol_Path get_native_type(ts::IDeclaration_Scope const& scope, ts::IType const& type)
{
    if (ts::IVariant_Type const* v = dynamic_cast<ts::IVariant_Type const*>(&type))
    {
        std::string str = "boost::variant<";
        for (size_t i = 0; i < v->get_inner_type_count(); i++)
        {
            std::shared_ptr<const ts::IType> const& inner_type = v->get_inner_type(i);
            str += scope.get_symbol_path().get_path_to(get_native_type(scope, *inner_type)).to_string() + ",";
        }
        str.pop_back();
        str += ">";
        return ts::Symbol_Path(str);
    }
    else if (ts::IVector_Type const* v = dynamic_cast<ts::IVector_Type const*>(&type))
    {
        return ts::Symbol_Path("std::vector<" + get_native_type(scope, *v->get_inner_type()).to_string() + ">");
    }
    else if (ts::IPoly_Type const* v = dynamic_cast<ts::IPoly_Type const*>(&type))
    {
        return ts::Symbol_Path("std::shared_ptr<" + get_native_type(scope, *v->get_inner_type()).to_string() + ">");
    }

    return get_symbol_path(type.get_native_type());
}

static ts::Symbol_Path get_type_relative_scope_path(ts::IDeclaration_Scope const& scope, ts::IType const& type)
{
    ts::Symbol_Path scope_path = get_symbol_path(scope.get_symbol_path());
    ts::Symbol_Path type_path = get_symbol_path(get_native_type(scope, type));

    return scope_path.get_path_to(type_path);
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

static void generate_member_def_declaration_code(Context& context, ts::IMember_Def const& member_def)
{
    std::string native_type_str = get_type_relative_scope_path(context.parent_scope, *member_def.get_type()).to_string();

    context.h_file += context.ident_str +
            native_type_str +
            " m_" +
            member_def.get_name() +
            " = {" +
            get_value_str(*member_def.get_default_value()) +
            "};\n";
}

static void generate_member_def_getter_code(Context& context, ts::IStruct_Type const& struct_type, ts::IMember_Def const& member_def)
{
    ts::IDeclaration_Scope const* ds = &struct_type;
    while (dynamic_cast<const ts::IStruct_Type*>(ds))
    {
        ds = ds->get_parent_scope();
    }
    TS_ASSERT(ds);

    std::string struct_type_str = get_type_relative_scope_path(*ds, struct_type).to_string();
    std::string native_type_str = get_type_relative_scope_path(context.parent_scope, *member_def.get_type()).to_string();

    context.h_file += context.ident_str + "auto get_" + member_def.get_name() + "() const -> " + native_type_str + " const&;\n";

    context.cpp_file += context.ident_str + "auto " + struct_type_str + "::get_" + member_def.get_name() + "() const -> " + native_type_str + " const& \n" +
            context.ident_str + "{\n" +
            context.ident_str + "  return m_" + member_def.get_name() + ";\n" +
            context.ident_str + "}\n\n";
}

static void generate_member_def_setter_code(Context& context, ts::IStruct_Type const& struct_type, ts::IMember_Def const& member_def)
{
    ts::IDeclaration_Scope const* ds = &struct_type;
    while (dynamic_cast<const ts::IStruct_Type*>(ds))
    {
        ds = ds->get_parent_scope();
    }
    TS_ASSERT(ds);

    std::string struct_type_str = get_type_relative_scope_path(*ds, struct_type).to_string();
    std::string native_type_str = get_type_relative_scope_path(context.parent_scope, *member_def.get_type()).to_string();

    context.h_file += context.ident_str + "void set_" + member_def.get_name() + "(" + native_type_str + " const& value);\n";

    context.cpp_file += context.ident_str + "void " + struct_type_str + "::set_" + member_def.get_name() + "(" + native_type_str + " const& value)\n" +
            context.ident_str + "{\n" +
            context.ident_str + "  m_" + member_def.get_name() + " = value;\n" +
            context.ident_str + "}\n";
}

static ts::Result<void> generate_struct_type_code(Context& context, ts::IStruct_Type const& struct_type)
{
    std::string struct_name = get_native_type(context.parent_scope, struct_type).back();
    context.h_file += context.ident_str + "struct " + struct_name;
    if (struct_type.get_base_struct())
    {
        context.h_file += " : public " + get_native_type(context.parent_scope, *struct_type.get_base_struct()).to_string();
    }
    context.h_file += "\n";
    context.h_file += context.ident_str +  "{\n\n";
    context.h_file += context.ident_str + "public:\n";

    Context c(context.h_file, context.cpp_file, struct_type);
    c.ident_str = context.ident_str + "  ";
    auto result = generate_declaration_scope_code(c, struct_type);
    if (result != ts::success)
    {
        return result;
    }

    context.h_file += c.ident_str + struct_name + "() noexcept = default;\n";
    context.h_file += c.ident_str + "virtual ~" + struct_name + "() noexcept = default;\n\n";

    for (size_t i = 0; i < struct_type.get_member_def_count(); i++)
    {
        if (std::shared_ptr<const ts::IStruct_Type> base = struct_type.get_base_struct())
        {
            if (base->find_member_def_by_name(struct_type.get_member_def(i)->get_name()))
            {
                continue;
            }
        }

        generate_member_def_getter_code(c, struct_type, *struct_type.get_member_def(i));
        generate_member_def_setter_code(c, struct_type, *struct_type.get_member_def(i));
        context.h_file += "\n";
        context.cpp_file += "\n////////////////////////////////////////////////////////////\n\n";
    }

    context.h_file += "\n\n";
    context.h_file += context.ident_str + "private:\n";

    for (size_t i = 0; i < struct_type.get_member_def_count(); i++)
    {
        if (std::shared_ptr<const ts::IStruct_Type> base = struct_type.get_base_struct())
        {
            if (base->find_member_def_by_name(struct_type.get_member_def(i)->get_name()))
            {
                continue;
            }
        }

        generate_member_def_declaration_code(c, *struct_type.get_member_def(i));
    }

    context.h_file += context.ident_str + "};\n\n";

    return ts::success;
}

static ts::Result<void> generate_enum_type_code(Context& context, ts::IEnum_Type const& enum_type)
{
    std::string enum_name = get_native_type(context.parent_scope, enum_type).back();
    context.h_file += context.ident_str + "enum class " + enum_name + "\n";
    context.h_file += context.ident_str + "{\n";

    for (size_t i = 0; i < enum_type.get_item_count(); i++)
    {
        std::shared_ptr<const ts::IEnum_Item> item = enum_type.get_item(i);
        context.h_file += context.ident_str + "  " + item->get_name() + " = " + std::to_string(item->get_integral_value()) + ",\n";
    }

    context.h_file += context.ident_str + "};\n\n";

    return ts::success;
}

static ts::Result<void> generate_namespace_code(Context& context, ts::Namespace const& ns)
{
    context.h_file += context.ident_str + "namespace " + ns.get_name() + "\n";
    context.h_file += context.ident_str + "{\n\n";
    context.cpp_file += context.ident_str + "namespace " + ns.get_name() + "\n";
    context.cpp_file += context.ident_str + "{\n\n";

    auto result = generate_declaration_scope_code(context, ns);
    if (result != ts::success)
    {
        return result;
    }

    context.h_file += context.ident_str + "}\n";
    context.cpp_file += context.ident_str + "}\n";

    return ts::success;
}

static ts::Result<void> generate_symbol_code(Context& context, ts::ISymbol const& symbol)
{
    if (ts::IStruct_Type const* struct_type = dynamic_cast<ts::IStruct_Type const*>(&symbol))
    {
        auto result = generate_struct_type_code(context, *struct_type);
        if (result != ts::success)
        {
            return result;
        }
    }
    else if (ts::IEnum_Type const* enum_type = dynamic_cast<ts::IEnum_Type const*>(&symbol))
    {
        auto result = generate_enum_type_code(context, *enum_type);
        if (result != ts::success)
        {
            return result;
        }
    }
    else if (ts::Namespace const* ns = dynamic_cast<ts::Namespace const*>(&symbol))
    {
        auto result = generate_namespace_code(context, *ns);
        if (result != ts::success)
        {
            return result;
        }
    }

    return ts::success;
}

static ts::Result<void> generate_declaration_scope_code(Context& context, ts::IDeclaration_Scope const& ds)
{
    for (size_t i = 0; i < ds.get_symbol_count(); i++)
    {
        auto result = generate_symbol_code(context, *ds.get_symbol(i));
        if (result != ts::success)
        {
            return result;
        }
    }

    return ts::success;
}

static ts::Result<void> generate_code(Context& context, ts::ast::Node const& ast_root_node, ts::Type_System const& ts)
{
    auto serialize_result = ast_root_node.serialize();
    if (serialize_result != ts::success)
    {
        return serialize_result.error();
    }

    context.h_file += "#pragma once\n\n";
    context.h_file += "#include <stdint.h>\n";
    context.h_file += "#include <string>\n";
    context.h_file += "#include <vector>\n";
    context.h_file += "#include <memory>\n";
    context.h_file += "#include <qmath.h>\n";
    context.h_file += "#include <def_lang/ast/Node.h>\n";
    context.h_file += "#include <def_lang/Result.h>\n";
    context.h_file += "#include <def_lang/Serialization.h>\n";
    context.h_file += "#include <def_lang/JSON_Serializer.h>\n";
    context.h_file += "#include <boost/variant.hpp>\n";

    context.cpp_file += "#include \"" + context.h_filename + "\"\n";

    if (!s_namespace.empty())
    {
        for (size_t i = 0; i < s_namespace.get_count(); i++)
        {
            context.h_file += "namespace " + s_namespace.get(i) + "\n"
                        "{\n\n";

            context.cpp_file += "namespace " + s_namespace.get(i) + "\n"
                          "{\n\n";
        }
    }

    std::string ast_json = ts::serialization::to_json(serialize_result.payload(), false);

    auto result = generate_ast_code(context, ast_json);
    if (result != ts::success)
    {
        return result;
    }

    result = generate_declaration_scope_code(context, ts);
    if (result != ts::success)
    {
        return result;
    }

    if (!s_namespace.empty())
    {
        context.h_file += "}\n";
        context.cpp_file += "}\n";
    }

    return ts::success;
}
