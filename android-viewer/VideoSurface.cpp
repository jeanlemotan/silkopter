//#include <private/qdeclarativevideooutput_backend_p.h>
//#include <private/qdeclarativevideooutput_p.h>

#include "VideoSurface.h"
//#include <QGuiApplication>
//#include <QMediaObject>
//#include <QMediaService>
//#include <QMutex>
#include <QResizeEvent>
//#include <QQuickWindow>
#include <QTimer>

#include <jni.h>
#include <android/log.h>

//#include <qpa/qplatformnativeinterface.h>
//#include <private/qgstreamerelementcontrol_p.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
//#include <gst/interfaces/nemovideotexture.h>
//#include <gst/video/gstvideometa.h>

#include <QThread>

#define EGL_SYNC_FENCE_KHR                      0x30F9





/*----------------------------------------------------------------------
|    definitions
+---------------------------------------------------------------------*/
#define ATTACH_TO_CURRENT_THREAD_THEN_RET(ret_statement)     \
    JNIEnv* env;                                              \
    if (s_javaVM->AttachCurrentThread(&env, NULL) < 0)  \
{                                                               \
    qCritical() << "Failed to attach to current thread."; \
    ret_statement;                                        \
    }

#define DETACH_FROM_CURRENT_THREAD_THEN_RET(ret_statement)       \
    if (!s_javaVM->DetachCurrentThread() < 0)                   \
{                                                           \
    qCritical() << "Failed to deattach from current thread."; \
    ret_statement;                                            \
    }

static JavaVM* s_javaVM;
static JNIEnv* s_env;
static jmethodID mid_loadVideoTexture;
static jmethodID mid_updateTexture;
static jclass class_TextureHelper;
static jclass class_SurfaceTexture;


/*----------------------------------------------------------------------
|    load_custom_java_classes
+---------------------------------------------------------------------*/
int load_custom_java_classes(JNIEnv* env)
{
    const char* classNameTexHelper =   "org/silkopter/TextureHelper";
    const char* classNameSurfTexture = "org/silkopter/MySurfaceTexture";

    // TODO: remember to free.
    jclass local_class_java_delegate = env->FindClass(classNameTexHelper);
    jclass local_class_surf_texture  = env->FindClass(classNameSurfTexture);
    if (!local_class_java_delegate)
    {
        __android_log_print(ANDROID_LOG_FATAL, "Qt", "Unable to find class %s.", classNameTexHelper);
        return JNI_FALSE;
    }
    if (!local_class_surf_texture)
    {
        __android_log_print(ANDROID_LOG_FATAL, "Qt", "Unable to find class %s.", classNameSurfTexture);
        return JNI_FALSE;
    }

    class_TextureHelper  = (jclass)env->NewGlobalRef(local_class_java_delegate);
    class_SurfaceTexture = (jclass)env->NewGlobalRef(local_class_surf_texture);
    env->DeleteLocalRef(local_class_java_delegate);
    env->DeleteLocalRef(local_class_surf_texture);

    mid_loadVideoTexture = env->GetStaticMethodID(class_TextureHelper,
                                                  "loadVideoTexture",
                                                  "(I)Landroid/graphics/SurfaceTexture;");
    mid_updateTexture    = env->GetMethodID(class_SurfaceTexture,
                                            "updateTexImage",
                                            "()V");
    if (!mid_updateTexture)
    {
        exit(-1);
    }

    return JNI_TRUE;
}

/*----------------------------------------------------------------------
|    JNI_OnLoad
+---------------------------------------------------------------------*/
Q_DECL_EXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    qDebug("JNI_OnLoad invoked!");
    s_javaVM = vm;

    if (s_javaVM->GetEnv((void**)&s_env, JNI_VERSION_1_4) != JNI_OK)
    {
        __android_log_print(ANDROID_LOG_FATAL, "Qt", "GetEnv failed.");
        return JNI_ERR;
    }

    // lcarlon: load the classes for the Qt application here.
    if (!load_custom_java_classes(s_env))
    {
        __android_log_print(ANDROID_LOG_FATAL, "Qt", "Couldn't register user defined classes.");
        return JNI_ERR;
    }

    return JNI_VERSION_1_4;
}




VideoTexture::VideoTexture()
    : m_subRect(0, 0, 1, 1)
    , m_textureSize(1280, 720)
    , m_textureId(0)
    , m_updated(false)
{
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_textureId);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

VideoTexture::~VideoTexture()
{
    releaseTexture();
    if (m_textureId)
    {
        glDeleteTextures(1, &m_textureId);
    }
}

int VideoTexture::textureId() const
{
    return m_textureId;
}

QSize VideoTexture::textureSize() const
{
    return m_textureSize;
}

void VideoTexture::setTextureSize(const QSize &size)
{
    m_textureSize = size;
}

bool VideoTexture::hasAlphaChannel() const
{
    return false;
}

bool VideoTexture::hasMipmaps() const
{
    return false;
}

QRectF VideoTexture::normalizedTextureSubRect() const
{
    return m_subRect;
}

void VideoTexture::bind()
{
    if (m_textureId) {
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_textureId);
    }
}

bool VideoTexture::updateTexture()
{
//    static const PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES
//            = reinterpret_cast<PFNGLEGLIMAGETARGETTEXTURE2DOESPROC>(eglGetProcAddress("glEGLImageTargetTexture2DOES"));

//    if (m_updated) {
//        return true;
//    }

//    NemoGstVideoTexture *sink = NEMO_GST_VIDEO_TEXTURE(m_sink);

//    if (!nemo_gst_video_texture_acquire_frame(sink)) {
//        return false;
//    }

//    int left = 0;
//    int top = 0;
//    int right = 0;
//    int bottom = 0;
//    if (GstMeta *meta = nemo_gst_video_texture_get_frame_meta (sink,
//                                                               GST_VIDEO_CROP_META_API_TYPE)) {
//        GstVideoCropMeta *crop = (GstVideoCropMeta *) meta;
//        left = crop->x;
//        top = crop->y;
//        right = crop->width + left;
//        bottom = crop->height + top;
//    }

//    // All the calculations below are based on ideas from Android GLConsumer.
//    // The difference between us and Android is we rely on texture coordinates
//    // while Android relies on a matxrix for cropping
//    qreal x = 0.0, y = 0.0, width = 1.0, height = 1.0;

//    // This value is taken from Android GLConsumer
//    qreal shrinkAmount = 1.0;
//    if (right - left < m_textureSize.width()) {
//        x = (left + shrinkAmount) / m_textureSize.width();
//        width = ((right - left) - (2.0f * shrinkAmount)) / m_textureSize.width();
//    }

//    if (bottom - top < m_textureSize.height()) {
//        y = (top + shrinkAmount) / m_textureSize.height();
//        height = (bottom - top - (2.0 * shrinkAmount)) / m_textureSize.height();
//    }

//    m_subRect = QRectF(x, y, width, height);

//    EGLImageKHR image;
//    if (!nemo_gst_video_texture_bind_frame(sink, &image)) {
//        if (m_textureId) {
//            glDeleteTextures(1, &m_textureId);
//            m_textureId = 0;
//        }
//        nemo_gst_video_texture_release_frame(sink, NULL);
//        return false;
//    } else {
//        if (!m_textureId) {
//            glGenTextures(1, &m_textureId);
//            glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_textureId);
//            glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        } else {
//            glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_textureId);
//        }
//        glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, image);
//        m_updated = true;
//        return true;
//    }
    return true;
}

void VideoTexture::invalidateTexture()
{
    if (m_textureId) {
        glDeleteTextures(1, &m_textureId);
        m_textureId = 0;
    }
}

void VideoTexture::releaseTexture()
{
//    static const PFNEGLCREATESYNCKHRPROC eglCreateSyncKHR
//            = reinterpret_cast<PFNEGLCREATESYNCKHRPROC>(eglGetProcAddress("eglCreateSyncKHR"));

    if (m_updated) {
        m_updated = false;

        if (m_textureId) {
            glDeleteTextures(1, &m_textureId);
            m_textureId = 0;
        }
    }
}

class VideoMaterialShader : public QSGMaterialShader
{
public:
    static QSGMaterialType type;

    void updateState(const RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect);
    char const *const *attributeNames() const;

protected:
    void initialize();

    const char *vertexShader() const;
    const char *fragmentShader() const;

private:
    int id_matrix;
    int id_subrect;
    int id_opacity;
    int id_texture;
};

void VideoMaterialShader::updateState(const RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect)
{
    VideoMaterial *material = static_cast<VideoMaterial *>(newEffect);

    if (state.isMatrixDirty()) {
        program()->setUniformValue(id_matrix, state.combinedMatrix());
    }

    if (state.isOpacityDirty()) {
        program()->setUniformValue(id_opacity, state.opacity());
    }

    if (!oldEffect) {
        program()->setUniformValue(id_texture, 0);
    }

    const QRectF subRect = material->m_texture->normalizedTextureSubRect();
    program()->setUniformValue(
                id_subrect, QVector4D(subRect.x(), subRect.y(), subRect.width(), subRect.height()));

    glActiveTexture(GL_TEXTURE0);
    material->m_texture->bind();
}

char const *const *VideoMaterialShader::attributeNames() const
{
    static char const *const attr[] = { "position", "texcoord", 0 };
    return attr;
}

void VideoMaterialShader::initialize()
{
    id_matrix = program()->uniformLocation("matrix");
    id_subrect = program()->uniformLocation("subrect");
    id_opacity = program()->uniformLocation("opacity");
    id_texture = program()->uniformLocation("texture");
}

QSGMaterialType VideoMaterialShader::type;

const char *VideoMaterialShader::vertexShader() const
{
    return  "\n#version 140"
            "\n uniform highp mat4 matrix;"
            "\n uniform highp vec4 subrect;"
            "\n attribute highp vec4 position;"
            "\n attribute highp vec2 texcoord;"
            "\n varying highp vec2 frag_tx;"
            "\n void main(void)"
            "\n {"
            "\n     gl_Position = matrix * position;"
            "\n     frag_tx = (texcoord * subrect.zw) + subrect.xy;"
            "\n }";
}

const char *VideoMaterialShader::fragmentShader() const
{
    return  "\n#version 140"
            "\n #extension GL_OES_EGL_image_external : require"
            "\n uniform samplerExternalOES texture;"
            "\n uniform lowp float opacity;\n"
            "\n varying highp vec2 frag_tx;"
            "\n void main(void)"
            "\n {"
            "\n     gl_FragColor = opacity * texture2D(texture, frag_tx.st);"
            "\n     gl_FragColor.g = 1.0;"
            "\n }";
}

VideoMaterial::VideoMaterial(VideoTexture *texture)
    : m_texture(texture)
{
}

QSGMaterialShader *VideoMaterial::createShader() const
{
    return new VideoMaterialShader;
}

QSGMaterialType *VideoMaterial::type() const
{
    return &VideoMaterialShader::type;
}

int VideoMaterial::compare(const QSGMaterial *other) const
{
    return m_texture - static_cast<const VideoMaterial *>(other)->m_texture;
}

VideoNode::VideoNode(VideoTexture *texture)
    : m_material(texture)
    , m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
{
    setGeometry(&m_geometry);
    setMaterial(&m_material);
    setFlag(UsePreprocess);
}

VideoNode::~VideoNode()
{
    m_material.m_texture->deleteLater();
}

void VideoNode::preprocess()
{
    VideoTexture *t = m_material.m_texture;
    if (t && t->updateTexture())
    {
        markDirty(QSGNode::DirtyMaterial);
    }
}

void VideoNode::setBoundingRect(
        const QRectF &rect, int orientation, bool horizontalMirror, bool verticalMirror)
{
    // Texture vertices clock wise from top left: or tl, tr, br, lf
    // Vertex order is tl, bl, tr, br. So unrotated the texture indexes are [0, 3, 1, 2] and
    // by shifting the array left and wrapping we rotate the image in 90 degree increments.
    const float tx[] = { 0, 1, 1, 0 };
    const float ty[] = { 0, 0, 1, 1 };

    // Texture coordinates are 0, or 1 so flip by subtracting the cooridinate from 1 and
    // taking the absolute value. 1 - 0 = 1, 1 - 1 = 0.  The absolute of 0 take the coordinate
    // gives back the original value. 0 - 0 = 0, 0 - 1 = -1.
    const float hm = horizontalMirror ? 1 : 0;
    const float vm = verticalMirror ? 1 : 0;

    const int offset = orientation / 90;
    QSGGeometry::TexturedPoint2D vertices[] = {
        { (float)rect.left() , (float)rect.top()   , qAbs(hm - tx[(0 + offset) % 4]), qAbs(vm - ty[(0 + offset) % 4]) },
        { (float)rect.left() , (float)rect.bottom(), qAbs(hm - tx[(3 + offset) % 4]), qAbs(vm - ty[(3 + offset) % 4]) },
        { (float)rect.right(), (float)rect.top()   , qAbs(hm - tx[(1 + offset) % 4]), qAbs(vm - ty[(1 + offset) % 4]) },
        { (float)rect.right(), (float)rect.bottom(), qAbs(hm - tx[(2 + offset) % 4]), qAbs(vm - ty[(2 + offset) % 4]) }
    };

    memcpy(m_geometry.vertexDataAsTexturedPoint2D(), vertices, sizeof(vertices));
}

/*----------------------------------------------------------------------
|    VideoSurface::VideoSurface
+---------------------------------------------------------------------*/
VideoSurface::VideoSurface(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents, true);

    QTimer* timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(doUpdate()));
    timer->setSingleShot(false);
    timer->start(40);
}

/*----------------------------------------------------------------------
|    VideoSurface::paint
+---------------------------------------------------------------------*/
QSGNode *VideoSurface::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    VideoNode* node = static_cast<VideoNode*>(oldNode);

    if (!node)
    {
        VideoTexture* texture = new VideoTexture();
        node = new VideoNode(texture);

        ATTACH_TO_CURRENT_THREAD_THEN_RET((void)0);
        jobject surfTexture = env->CallStaticObjectMethod(class_TextureHelper, mid_loadVideoTexture, texture->textureId());
        if (!surfTexture)
        {
            qWarning("Failed to instantiate SurfaceTexture.");
        }
        mSurfaceTexture = env->NewGlobalRef(surfTexture);
        DETACH_FROM_CURRENT_THREAD_THEN_RET((void)0);
    }

    if (mSurfaceTexture)
    {
        ATTACH_TO_CURRENT_THREAD_THEN_RET(return node);
        env->CallVoidMethod(mSurfaceTexture, mid_updateTexture);
        DETACH_FROM_CURRENT_THREAD_THEN_RET(return node);
    }

    node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);

    return node;
}

/*----------------------------------------------------------------------
|    VideoSurface::doUpdate
+---------------------------------------------------------------------*/
void VideoSurface::doUpdate()
{
    update();
}
