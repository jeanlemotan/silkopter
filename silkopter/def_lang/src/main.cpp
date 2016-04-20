#include "Type_System.h"
#include "ast/Builder.h"

#include "IStruct_Type.h"
#include "IStruct_Value.h"
#include "IString_Value.h"
#include "Value_Selector.h"

int main(int argc, char **argv)
{
    ast::Builder builder;

    auto parse_result = builder.parse("test.def");
    if (parse_result != ts::success)
    {
        std::cerr << parse_result.error().what();
    }

    std::cout << builder.get_ast_root_node().to_string(0, true) << std::endl;
    std::cout.flush();

    ts::Type_System ts;
    ts.populate_builtin_types();

    auto compile_result = builder.compile(ts);
    if (compile_result != ts::success)
    {
        std::cerr << compile_result.error().what();
    }

    std::shared_ptr<ts::IStruct_Type> type = ts.find_specialized_symbol_by_path<ts::IStruct_Type>(ts::Symbol_Path("silk::Multirotor_Config"));
    TS_ASSERT(type);

    std::shared_ptr<ts::IStruct_Value> value = type->create_specialized_value();

    std::shared_ptr<ts::IString_Value> name = value->select_specialized<ts::IString_Value>(ts::Value_Selector("name"));

    for (volatile size_t i = 0; i < 1000; i++)
    {
        size_t idx = i % 10;
        ts::Value_Selector selector("motors");
        selector.push_back(ts::Value_Selector::Element(idx));
        std::shared_ptr<ts::IStruct_Value> motor = value->select_specialized<ts::IStruct_Value>(std::move(selector));
    }

    return 0;
}
