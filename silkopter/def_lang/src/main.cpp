#include "Type_System.h"
#include "ast/Builder.h"


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

    return 0;
}
