#include "Video_Renderer.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

#include "Video_Decoder.h"

#define CHECK_GL_ERRORS

#if defined(CHECK_GL_ERRORS)
#define GLCHK \
do { \
    GLenum err = GL_NO_ERROR; \
   while ((err = glGetError()) != GL_NO_ERROR) \
   { \
      QLOGE("GL error {} in file {} line {}", err, __FILE__,__LINE__); \
   } \
} \
while(0)
#else
#define GLCHK
#endif /* CHECK_GL_ERRORS */


extern Video_Decoder s_video_decoder;


Video_Renderer_OpenGL::Video_Renderer_OpenGL(Video_Renderer* video_renderer)
    : m_video_renderer(video_renderer)
{
}

Video_Renderer_OpenGL::~Video_Renderer_OpenGL()
{
}

void Video_Renderer_OpenGL::setWindow(QQuickWindow *window)
{
    m_window = window;
}

void Video_Renderer_OpenGL::paint()
{
    if (!m_is_initialized)
    {
        m_is_initialized = true;

        char const* vertex_source =  "attribute highp vec4 vertices;\n"
                                     "varying highp vec2 coords;\n"
                                     "void main() {\n"
                                     "    gl_Position = vertices;\n"
                                     "    coords = vertices.xy * 0.5 + vec2(0.5);\n"
                                     "    coords.x = 1.0 - coords.x;\n"
                                     "}";

        m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_vertex_shader, 1, &vertex_source, nullptr);
        glCompileShader(m_vertex_shader);
        int compiled[1];
        glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, compiled);
        if (compiled[0] == 0)
        {
            GLchar info[1024];
            GLsizei size = 0;
            glGetShaderInfoLog(m_vertex_shader, 1024, &size, info);
            info[size] = 0;
            QLOGE("Could not compile shader: {}", info);
            glDeleteShader(m_vertex_shader);
            m_vertex_shader = 0;
        }
        GLCHK;

        char const* fragment_source =   "#extension GL_OES_EGL_image_external : require\n"
                                        "uniform samplerExternalOES texture;\n"
                                        "varying highp vec2 coords;\n"
                                        "void main() {\n"
                                        "    lowp vec4 c = texture2D(texture, coords);\n"
                                        "    gl_FragColor = c;\n"
                                        "}";

        m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_fragment_shader, 1, &fragment_source, nullptr);
        glCompileShader(m_fragment_shader);
        glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, compiled);
        if (compiled[0] == 0)
        {
            GLchar info[1024];
            GLsizei size = 0;
            glGetShaderInfoLog(m_fragment_shader, 1024, &size, info);
            info[size] = 0;
            QLOGE("Could not compile shader: {}", info);
            glDeleteShader(m_fragment_shader);
            m_fragment_shader = 0;
        }
        GLCHK;

        m_shader_program = glCreateProgram();
        QASSERT(m_shader_program != 0);
        glAttachShader(m_shader_program, m_vertex_shader);
        glAttachShader(m_shader_program, m_fragment_shader);
        glLinkProgram(m_shader_program);
        int linkStatus[1];
        glGetProgramiv(m_shader_program, GL_LINK_STATUS, linkStatus);
        if (linkStatus[0] != GL_TRUE)
        {
            GLchar info[1024];
            GLsizei size = 0;
            glGetProgramInfoLog(m_shader_program, 1024, &size, info);
            info[size] = 0;
            QLOGE("Could not link program: {}", info);
            glDeleteProgram(m_shader_program);
            m_shader_program = 0;
        }

        m_position_location = glGetAttribLocation(m_shader_program, "vertices");
        QASSERT(m_position_location != -1);
        m_texture_location = glGetUniformLocation(m_shader_program, "texture");
        QASSERT(m_texture_location != -1);
    }

    float values[] =
    {
        -1, -1,
        1, -1,
        -1, 1,
        1, 1
    };

    int x = m_video_renderer->x();
    int y = m_window->height() - (m_video_renderer->y() + m_video_renderer->height());
    int width = m_video_renderer->width();
    int height = m_video_renderer->height();

    glViewport(x, y, width, height);

    glDisable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(m_shader_program);

    uint32_t video_texture = s_video_decoder.get_video_texture_id();
    if (video_texture > 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, video_texture);
        GLCHK;
    }

    glVertexAttribPointer(m_position_location, 2, GL_FLOAT, false, 8, values);
    glEnableVertexAttribArray(m_position_location);

    glUniform1i(m_texture_location, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    GLCHK;

    glDisableVertexAttribArray(m_position_location);
    //glFinish();

    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    m_window->resetOpenGLState();
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

Video_Renderer::Video_Renderer()
{
    connect(this, &QQuickItem::windowChanged, this, &Video_Renderer::handleWindowChanged);
}

Video_Renderer::~Video_Renderer()
{
    delete m_renderer;
}

void Video_Renderer::handleWindowChanged(QQuickWindow* win)
{
    if (win)
    {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &Video_Renderer::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &Video_Renderer::cleanup, Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void Video_Renderer::cleanup()
{
    delete m_renderer;
    m_renderer = 0;
}

void Video_Renderer::sync()
{
    if (!m_renderer)
    {
        m_renderer = new Video_Renderer_OpenGL(this);
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &Video_Renderer_OpenGL::paint, Qt::DirectConnection);
    }

    m_renderer->setWindow(window());
}
