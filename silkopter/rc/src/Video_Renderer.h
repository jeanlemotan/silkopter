#pragma once

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>


class Video_Renderer;

class Video_Renderer_OpenGL : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    Video_Renderer_OpenGL(Video_Renderer* video_renderer);

    ~Video_Renderer_OpenGL();

    void setWindow(QQuickWindow *window);

public slots:
    void paint();

private:
    Video_Renderer* m_video_renderer = nullptr;
    QQuickWindow *m_window = nullptr;

    bool m_is_initialized = false;

    uint32_t m_vertex_shader = 0;
    uint32_t m_fragment_shader = 0;
    uint32_t m_shader_program = 0;
    int32_t m_position_location = 0;
    int32_t m_texture_location = 0;
};

////////////////////////////////////////////////////////////////////////


class Video_Renderer : public QQuickItem
{
    Q_OBJECT

public:
    Video_Renderer();
    ~Video_Renderer();

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    Video_Renderer_OpenGL* m_renderer = nullptr;
};


