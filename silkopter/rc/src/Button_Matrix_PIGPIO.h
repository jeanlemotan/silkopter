#pragma once

#include "IButton_Matrix.h"
#include <vector>
#include <memory>

namespace silk
{
class IButton;

class Button_Matrix_PIGPIO : public IButton_Matrix
{
public:
    Button_Matrix_PIGPIO();
    ~Button_Matrix_PIGPIO();

    ts::Result<void> init(std::vector<uint8_t> const& row_gpios, std::vector<uint8_t> const& column_gpios);

    size_t get_row_count() const override;
    size_t get_column_count() const override;
    IButton& get_button(size_t row, size_t column) const override;

    void process();

private:
    std::vector<uint8_t> m_row_gpios;
    std::vector<uint8_t> m_column_gpios;

    size_t get_button_index(size_t row, size_t column) const;

    struct Matrix_Button;
    std::vector<std::unique_ptr<Matrix_Button>> m_buttons;
};

}
