#pragma once

#include "def_lang/IAttribute.h"

namespace ts
{

class Public_Attribute : public virtual IAttribute
{
public:
    Public_Attribute(bool is_public);
    ~Public_Attribute();

    std::string get_name() const override;
    bool is_public() const;

private:
    bool m_is_public = true;
};

}
