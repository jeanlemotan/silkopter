#pragma once


class IHUD_Widget : public QWidget, q::util::Noncopyable
{
public:
    virtual ~IHUD_Widget() {}

    virtual void process() = 0;

};
