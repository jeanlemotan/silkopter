#! /bin/sh

flex++ -o yy_lexer.cpp tokens.l    
bison -d -o yy_parser.cpp parser.y

