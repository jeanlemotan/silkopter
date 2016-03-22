#pragma once

#include <QObject>

class OS_QML_Proxy : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(float brightness READ getBrightness WRITE setBrightness NOTIFY brightnessChanged)

    explicit OS_QML_Proxy(QObject *parent = 0);

    float getBrightness() const;
    void setBrightness(float v);

    Q_INVOKABLE void poweroffSystem();

signals:
    void brightnessChanged(float);

public slots:

private:
    float m_brightness = 1.f;
};

