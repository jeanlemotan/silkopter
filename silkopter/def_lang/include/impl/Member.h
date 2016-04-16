#pragma once

#include <memory>
#include "IMember.h"

namespace ts
{

class IValue;

class Member final : public IMember
{
public:
    Member(IMember_Def const& member_def);
    ~Member();

    auto get_member_def() const -> IMember_Def const& override;

    auto get_value() const -> IValue const& override;
    auto get_value() -> IValue& override;

private:
    IMember_Def const& m_member_def;
    std::unique_ptr<IValue> m_value;
};

}
