#pragma once

#include "IInput_Device.h"

namespace silk
{
class IButton;

class IButton_Matrix : public IInput_Device
{
public:
    virtual ~IButton_Matrix() = default;

    virtual size_t get_row_count() const = 0;
    virtual size_t get_column_count() const = 0;
    virtual IButton& get_button(size_t row, size_t column) const = 0;
};

}
