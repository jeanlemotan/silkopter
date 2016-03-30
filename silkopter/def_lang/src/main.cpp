#include <iostream>

#include "Type_System.h"


extern int yyparse();

int main(int argc, char **argv)
{
    yyparse();
    return 0;
}
