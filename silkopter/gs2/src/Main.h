#pragma once

#include <QQuickView>
#include <QQuickItem>

class Main : public QObject
{
    Q_OBJECT
public:
    explicit Main(QObject *parent = 0);

signals:

private slots:
    void showSplash();
    void showMM();

private:
    QQuickView m_view;
};
