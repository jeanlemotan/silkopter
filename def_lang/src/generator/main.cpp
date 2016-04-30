#if 1

#include "def_lang/Type_System.h"
#include "ast/Builder.h"

#include "def_lang/IStruct_Type.h"
#include "def_lang/IStruct_Value.h"
#include "def_lang/IString_Value.h"
#include "def_lang/IVector_Value.h"
#include "def_lang/All_INumeric_Values.h"
#include "def_lang/Value_Selector.h"
#include "def_lang/impl/Initializer_List.h"
#include "def_lang/Mapper.h"
#include "def_lang/JSON_Serializer.h"

#include <chrono>

int main(int argc, char **argv)
{
    ast::Builder builder;

    auto parse_result = builder.parse("../uav.def");
    if (parse_result != ts::success)
    {
        std::cerr << parse_result.error().what();
        std::cerr.flush();
    }

    std::cout << builder.get_ast_root_node().to_string(0, true) << std::endl;
    std::cout.flush();

    ts::Type_System ts;
    ts.populate_builtin_types();

    auto compile_result = builder.compile(ts);
    if (compile_result != ts::success)
    {
        std::cerr << compile_result.error().what();
        std::cerr.flush();
    }

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

    std::string json = ts::serialization::to_json(serialize_result.payload());
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

#endif
