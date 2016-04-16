#include "IBool_Type.h"
#include "ep/Type_Template_EP.h"

namespace ts
{

class IBool_Value;
class Bool_Value;
class Bool_Type;

struct Bool_Type_Traits : public IBool_Type::traits
{
    typedef Bool_Value value_implementation;
    typedef Bool_Type type_implementation;
};

class Bool_Type final : public Type_Template_EP<Bool_Type_Traits>
{
public:
    Bool_Type(std::string const& name);

protected:
    auto validate_attribute(IAttribute const& attribute) -> Result<void> override;
};


}

