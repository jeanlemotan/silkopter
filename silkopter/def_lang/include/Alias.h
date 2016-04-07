#pragma once

#include "types/IType.h"

namespace ts
{

class Alias final : virtual public IType
{
public:
    Alias(std::shared_ptr<const IType> type);

    //ISymbol
    auto get_name() const -> std::string const& override;

    //IAttribute_Container
    void add_attribute(std::unique_ptr<const IAttribute>&& att) override;

    auto get_attribute_count() const -> size_t override;

    auto get_attribute(size_t idx) const -> std::shared_ptr<const IAttribute> override;
    auto get_attribute(size_t idx) -> std::shared_ptr<IAttribute> override;

    auto find_attribute_idx_by_name(std::string const& name) const -> boost::optional<size_t> override;

    auto find_attribute_by_name(std::string const& name) const -> std::shared_ptr<const IAttribute> override;
    auto find_attribute_by_name(std::string const& name) -> std::shared_ptr<IAttribute> override;


    //IType
    auto get_default_value() const -> std::shared_ptr<const IValue> override;
    auto create_value() const -> std::unique_ptr<IValue> override;

};

}
