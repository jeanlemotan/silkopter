#pragma once

#include <fstream>
#include "def_lang/Result.h"
#include "def_lang/ast/Node.h"
#include "def_lang/Type_System.h"
#include "def_lang/Source_Location.h"

namespace ts
{
namespace ast
{
class Lexer;
}
}

namespace yy
{
class parser;
}

namespace ts
{
namespace ast
{

class Builder final
{
    friend class yy::parser;
    friend class ast::Lexer;
public:
    Builder();
    ~Builder();

    ts::Result<void> parse_file(std::string const& filename);
    ts::Result<void> parse_string(std::string const& def);

    Node& get_ast_root_node();
    Node const& get_ast_root_node() const;

    ts::Result<void> compile(ts::Type_System& ts);

    Lexer& get_lexer();

protected:
    void report_error(ts::Error const& error);
    std::string get_filename() const;

    ts::Source_Location get_location() const;

    bool start_file(std::string const& filename);
    bool end_file();

private:
    bool start_stream(std::string const& filename, std::shared_ptr<std::istream>&& stream);

    Node m_root_node;
    std::shared_ptr<Lexer> m_lexer;
    std::shared_ptr<yy::parser> m_parser;

    struct Import
    {
        std::string filename;
        std::shared_ptr<std::istream> stream;
    };

    std::vector<Import> m_imports;

    ts::Result<void> m_parse_result;
};

}
}
