#pragma once

#include <memory>
#include "IMember.h"

namespace ts
{

class IValue;

class Member final : public IMember
{
public:
    Member(std::shared_ptr<IMember_Def const> member_def);
    ~Member();

    std::shared_ptr<IMember_Def const> get_member_def() const override;

    std::shared_ptr<const IValue> get_value() const override;
    std::shared_ptr<IValue> get_value() override;

private:
    std::shared_ptr<IMember_Def const> m_member_def;
    std::shared_ptr<IValue> m_value;
};

}
