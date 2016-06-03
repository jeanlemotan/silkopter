#include "def_lang/IString_Type.h"
#include "def_lang/ep/Type_Template_EP.h"

namespace ts
{

class IString_Value;
class String_Value;
class String_Type;

struct String_Type_Traits : IString_Type::traits
{
    typedef String_Value value_implementation;
    typedef String_Type type_implementation;
};

class String_Type final : public Type_Template_EP<String_Type_Traits>
{
public:
    String_Type(std::string const& name);
    String_Type(String_Type const& other, std::string const& name);

    std::string generate_serialization_code() const;

protected:
    Result<void> validate_attribute_impl(IAttribute const& attribute) override;
};


}

