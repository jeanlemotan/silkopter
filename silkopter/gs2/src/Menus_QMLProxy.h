#pragma once

#include <QObject>
#include <QUrl>
#include <QQuickView>

class Menus_QMLProxy : public QObject
{
    Q_OBJECT
public:
    explicit Menus_QMLProxy(QObject *parent = 0);

    void init(QQuickView& view);

signals:

public slots:
    Q_INVOKABLE void pop();
    Q_INVOKABLE void push(const QString& qml);

private slots:
    void setView();

private:
    QQuickView* m_view = nullptr;
    bool m_isLocked = false;
    std::vector<QUrl> m_stack;
};
