#include "OS.h"
#include <QSettings>
#include <QProcess>

OS::OS(QObject *parent)
    : QObject(parent)
{
    QSettings settings;

    {
        bool ok = false;
        float brightness = settings.value("screen/brightness").toFloat(&ok);
        m_brightness = 0.f; //to force a refresh
        setBrightness(ok ? brightness : 1.f);
    }
}

void OS::poweroffSystem()
{
}

float OS::getBrightness() const
{
    return m_brightness;
}

void OS::setBrightness(float v)
{
    v = std::min(std::max(v, 0.1f), 1.f);
    if (m_brightness != v)
    {
        m_brightness = v;

        emit brightnessChanged(m_brightness);
    }
}
