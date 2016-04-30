#include "def_lang/impl/Literal_Initializer.h"
#include "def_lang/ts_assert.h"

namespace ts
{

Literal_Initializer::Literal_Initializer(std::shared_ptr<const ILiteral> literal)
    : m_literal(literal)
{
    TS_ASSERT(m_literal);
}

std::shared_ptr<const ILiteral> Literal_Initializer::get_literal() const
{
    return m_literal;
}

}
