#pragma once

#include <QWidget>


class ITab_Widget : public QWidget
{
public:
    virtual ~ITab_Widget() = default;

    virtual void set_active(bool active) = 0;
};
