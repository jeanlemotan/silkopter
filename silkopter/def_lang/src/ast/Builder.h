#pragma once

#include <fstream>
#include "Node.h"
#include "Type_System.h"
#include "Source_Location.h"

namespace ast
{
class Lexer;
}

namespace ast
{

class Builder final
{
public:
    Builder();
    ~Builder();

    auto start_file(std::string const& filename) -> bool;
    auto end_file() -> bool;

    auto get_filename() const -> std::string;

    auto get_lexer() -> Lexer&;

    auto get_root_node() -> Node&;
    auto get_root_node() const -> Node const&;

    auto get_location() const -> ts::Source_Location;

    auto populate_type_system(ts::Type_System& ts) -> bool;

private:
    Node m_root_node;
    std::unique_ptr<Lexer> m_lexer;

    struct Import
    {
        std::string filename;
        std::unique_ptr<std::ifstream> stream;
    };

    std::vector<Import> m_imports;
};

}
