#pragma once

#include "HAL.h"
#include "Comms.h"
#include "ui_Float_Stream_Viewer.h"

template<class Stream>
class Float_Stream_Viewer : public QWidget
{
public:
    Float_Stream_Viewer(std::string const& unit, std::shared_ptr<Stream> stream, QWidget *parent)
        : QWidget(parent)
        , m_stream(stream)
        , m_unit(unit)
    {
        m_ui.setupUi(this);

        m_ui.plot->addGraph();
        m_ui.plot->graph(0)->setPen(QPen(Qt::blue));
        m_ui.plot->yAxis->setLabel(unit.c_str());

        m_connection = stream->samples_available_signal.connect([this](Stream& stream)
        {
            process_samples(stream);
        });
    }

    ~Float_Stream_Viewer()
    {
    }

private:
    Ui::Float_Stream_Viewer m_ui;
    silk::node::stream::Stream_wptr m_stream;
    std::string m_unit;
    q::util::Scoped_Connection m_connection;
    double m_time = 0;

    void process_samples(Stream& stream)
    {
        for (auto const& s: stream.samples)
        {
            m_ui.plot->graph(0)->addData(m_time, s.value);
            m_time += std::chrono::duration<double>(s.dt).count();
        }
    }
};
