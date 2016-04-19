#include "Type_System.h"
#include "ast/Builder.h"

#include "IStruct_Type.h"
#include "IStruct_Value.h"
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

    std::shared_ptr<ts::IStruct_Type> type = ts.find_specialized_symbol_by_name<ts::IStruct_Type>("Multirotor_Config");
    TS_ASSERT(type);

    std::shared_ptr<ts::IStruct_Value> value = type->create_specialized_value();

    value->select(ts::Value_Selector("name"));

    return 0;
}
