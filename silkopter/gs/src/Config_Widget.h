#pragma once

class Properties_Browser;


class Config_Widget : public QWidget
{
public:
    void init(ts::Type_System& ts, Properties_Browser& browser);

    void refresh();

private:
    ts::Type_System* m_ts = nullptr;
    Properties_Browser* m_browser = nullptr;
};
