#pragma once

#include <fstream>
#include "Result.h"
#include "Node.h"
#include "Type_System.h"
#include "Source_Location.h"

namespace ast
{
class Lexer;
}

namespace yy
{
class parser;
}

namespace ast
{

class Builder final
{
    friend class yy::parser;
    friend class ast::Lexer;
public:
    Builder();
    ~Builder();

    auto parse(std::string const& filename) -> ts::Result<void>;

    auto get_ast_root_node() -> Node&;
    auto get_ast_root_node() const -> Node const&;

    auto compile(ts::Type_System& ts) -> ts::Result<void>;

    auto get_lexer() -> Lexer&;

protected:
    auto get_filename() const -> std::string;

    auto get_location() const -> ts::Source_Location;

    auto start_file(std::string const& filename) -> ts::Result<void>;
    auto end_file() -> bool;

private:
    Node m_root_node;
    std::unique_ptr<Lexer> m_lexer;
    std::unique_ptr<yy::parser> m_parser;

    struct Import
    {
        std::string filename;
        std::unique_ptr<std::ifstream> stream;
    };

    std::vector<Import> m_imports;
};

}
