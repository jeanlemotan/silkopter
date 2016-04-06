#include <iostream>



#include "Type_System.h"
#include "ast/Builder.h"

//ast::Builder g_parser;

//extern int yyparse();

#include "yy_parser.hpp"

//extern YY_BUFFER_STATE yy_scan_string (yyconst char *yy_str  );

int main(int argc, char **argv)
{
    ast::Builder builder;

    yy::parser x(builder);

    builder.start_file("test.def");

    x.parse();

    std::cout << builder.get_root_node().to_string(0, true) << std::endl;
    std::cout.flush();

    ts::Type_System ts;
    ts.populate_builtin_types();
    builder.populate_type_system(ts);

    return 0;
}
