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

#ifdef Q_ANDROID
#   include <jni.h>
#   include <android/log.h>
#   include <QAndroidJniObject>
#   include <EGL/egl.h>
#   include <EGL/eglext.h>
#endif

#include <QThread>
//#include <qpa/qplatformnativeinterface.h>

#include <chrono>
#include <thread>
#include <mutex>

#define EGL_SYNC_FENCE_KHR                      0x30F9

static const size_t NALU_MAXLEN = 1024 * 1024;



/*----------------------------------------------------------------------
|    definitions
+---------------------------------------------------------------------*/

#ifdef Q_ANDROID

#define ATTACH_TO_CURRENT_THREAD_THEN_RET(ret_statement)    \
    JNIEnv* env;                                            \
    if (s_javaVM->AttachCurrentThread(&env, NULL) < 0)      \
    {                                                       \
        qCritical() << "Failed to attach to current thread.";   \
        ret_statement;                                          \
    }

#define DETACH_FROM_CURRENT_THREAD_THEN_RET(ret_statement)  \
    if (!s_javaVM->DetachCurrentThread() < 0)               \
    {                                                       \
        qCritical() << "Failed to deattach from current thread.";   \
        ret_statement;                                              \
    }

static JavaVM* s_javaVM = nullptr;
static JNIEnv* s_env = nullptr;
static jmethodID s_midSetupDecoder = nullptr;
static jmethodID s_midFeedDecoder = nullptr;
static jmethodID s_midUpdateTexture = nullptr;
static jclass s_classVideoDecoder = nullptr;
static jclass s_classSurfaceTexture = nullptr;
static jobject s_surfaceTexture = nullptr;

/*----------------------------------------------------------------------
|    load_custom_java_classes
+---------------------------------------------------------------------*/
int load_custom_java_classes(JNIEnv* env)
{
    const char* classNameDecoder =   "org/silkopter/VideoDecoder";
    const char* classNameSurfTexture = "org/silkopter/MySurfaceTexture";

    // TODO: remember to free.
    jclass cls = env->FindClass(classNameDecoder);
    if (!cls)
    {
        __android_log_print(ANDROID_LOG_FATAL, "Qt", "Unable to find class %s.", classNameDecoder);
        return JNI_FALSE;
    }
    s_classVideoDecoder  = (jclass)env->NewGlobalRef(cls);
    env->DeleteLocalRef(cls);

    cls = env->FindClass(classNameSurfTexture);
    if (!cls)
    {
        __android_log_print(ANDROID_LOG_FATAL, "Qt", "Unable to find class %s.", classNameSurfTexture);
        return JNI_FALSE;
    }

    s_classSurfaceTexture = (jclass)env->NewGlobalRef(cls);
    env->DeleteLocalRef(cls);

    s_midSetupDecoder = env->GetStaticMethodID(s_classVideoDecoder,
                                                  "setupDecoder",
                                                  "(I)Landroid/graphics/SurfaceTexture;");
    s_midFeedDecoder = env->GetStaticMethodID(s_classVideoDecoder,
                                                  "feedDecoder",
                                                  "([BI)V");
    s_midUpdateTexture    = env->GetMethodID(s_classSurfaceTexture,
                                            "updateTexImage",
                                            "()V");
    if (!s_midUpdateTexture)
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

    {
        QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
        QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");
        QAndroidJniObject decorView = window.callObjectMethod("getDecorView", "()Landroid/view/View;");

        int flagFullscreen = QAndroidJniObject::getStaticField<int>("android/view/View", "SYSTEM_UI_FLAG_FULLSCREEN");
        int flagHideNavigation = QAndroidJniObject::getStaticField<int>("android/view/View", "SYSTEM_UI_FLAG_HIDE_NAVIGATION");
        int flagImmersiveSticky = QAndroidJniObject::getStaticField<int>("android/view/View", "SYSTEM_UI_FLAG_IMMERSIVE_STICKY");

        int flag = flagFullscreen | flagHideNavigation | flagImmersiveSticky;

        //hiding the status bar
        decorView.callMethod<void>("setSystemUiVisibility", "(I)V", flag);

        //keeping the screen on
        const int FLAG_KEEP_SCREEN_ON = 128;
        window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
    }

    return JNI_VERSION_1_4;
}

#endif //Q_ANDROID


static VideoTexture* s_videoTexture = nullptr;
static std::mutex s_videoDataMutex;
static std::vector<uint8_t> s_videoData;
static math::vec2u16 s_videoResolution;
static bool s_videoResolutionDirty = false;

static const uint8_t naluSeparator[4] = { 0, 0, 0, 1 };


VideoTexture::VideoTexture()
    : m_subRect(0, 0, 1, 1)
    , m_textureSize(1280, 720)
    , m_textureId(0)
    , m_updated(false)
{
#ifdef Q_ANDROID
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_textureId);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
}

VideoTexture::~VideoTexture()
{
    releaseTexture();
    if (m_textureId)
    {
#ifdef Q_ANDROID
        glDeleteTextures(1, &m_textureId);
#endif
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
    if (m_textureId != 0)
    {
#ifdef Q_ANDROID
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_textureId);
#endif
    }
}

bool VideoTexture::updateTexture()
{
//    static const PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES
//            = reinterpret_cast<PFNGLEGLIMAGETARGETTEXTURE2DOESPROC>(eglGetProcAddress("glEGLImageTargetTexture2DOES"));

//    if (m_updated)
//    {
//        return true;
//    }

//    NemoGstVideoTexture *sink = NEMO_GST_VIDEO_TEXTURE(m_sink);

//    if (!nemo_gst_video_texture_acquire_frame(sink))
//    {
//        return false;
//    }

//    int left = 0;
//    int top = 0;
//    int right = 0;
//    int bottom = 0;
//    if (GstMeta *meta = nemo_gst_video_texture_get_frame_meta (sink, GST_VIDEO_CROP_META_API_TYPE))
//    {
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
//    if (right - left < m_textureSize.width())
//    {
//        x = (left + shrinkAmount) / m_textureSize.width();
//        width = ((right - left) - (2.0f * shrinkAmount)) / m_textureSize.width();
//    }

//    if (bottom - top < m_textureSize.height())
//    {
//        y = (top + shrinkAmount) / m_textureSize.height();
//        height = (bottom - top - (2.0 * shrinkAmount)) / m_textureSize.height();
//    }

//    m_subRect = QRectF(x, y, width, height);

//    EGLImageKHR image;
//    if (!nemo_gst_video_texture_bind_frame(sink, &image))
//    {
//        if (m_textureId)
//        {
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
    if (m_textureId != 0)
    {
#ifdef Q_ANDROID
        glDeleteTextures(1, &m_textureId);
#endif
        m_textureId = 0;
    }
}

void VideoTexture::releaseTexture()
{
    if (m_updated)
    {
        m_updated = false;
        if (m_textureId != 0)
        {
#ifdef Q_ANDROID
            glDeleteTextures(1, &m_textureId);
#endif
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
    int m_id_matrix = 0;
    int m_id_subrect = 0;
    int m_id_opacity = 0;
    int m_id_texture = 0;
};

void VideoMaterialShader::updateState(const RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect)
{
    VideoMaterial *material = static_cast<VideoMaterial *>(newEffect);

    if (state.isMatrixDirty())
    {
        program()->setUniformValue(m_id_matrix, state.combinedMatrix());
    }

    if (state.isOpacityDirty())
    {
        program()->setUniformValue(m_id_opacity, state.opacity());
    }

    if (!oldEffect)
    {
        program()->setUniformValue(m_id_texture, 0);
    }

    const QRectF subRect = material->m_texture->normalizedTextureSubRect();
    program()->setUniformValue(m_id_subrect, QVector4D(subRect.x(), subRect.y(), subRect.width(), subRect.height()));

#ifdef Q_ANDROID
    glActiveTexture(GL_TEXTURE0);
#endif
    material->m_texture->bind();
}

char const *const *VideoMaterialShader::attributeNames() const
{
    static char const *const attr[] = { "position", "texcoord", 0 };
    return attr;
}

void VideoMaterialShader::initialize()
{
    m_id_matrix = program()->uniformLocation("matrix");
    m_id_subrect = program()->uniformLocation("subrect");
    m_id_opacity = program()->uniformLocation("opacity");
    m_id_texture = program()->uniformLocation("texture");
}

QSGMaterialType VideoMaterialShader::type;

const char *VideoMaterialShader::vertexShader() const
{
    return  "\n uniform highp mat4 matrix;"
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
    return  "\n #extension GL_OES_EGL_image_external : require"
            "\n uniform samplerExternalOES texture;"
            "\n uniform lowp float opacity;\n"
            "\n varying highp vec2 frag_tx;"
            "\n void main(void)"
            "\n {"
            "\n     gl_FragColor = opacity * texture2D(texture, frag_tx.st);"
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
    //m_material.m_texture->deleteLater();
}

void VideoNode::preprocess()
{
    VideoTexture *t = m_material.m_texture;
    if (t && t->updateTexture())
    {
        markDirty(QSGNode::DirtyMaterial);
    }
}

void VideoNode::setBoundingRect(const QRectF &rect, int orientation, bool horizontalMirror, bool verticalMirror)
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
    QSGGeometry::TexturedPoint2D vertices[] =
    {
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

//    QTimer* timer = new QTimer;
//    connect(timer, SIGNAL(timeout()), this, SLOT(doUpdate()));
//    timer->setSingleShot(false);
//    timer->start(1);
}

void VideoSurface::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    if (newGeometry != oldGeometry)
    {
        m_isGeomertyDirty = true;
    }
}

/*----------------------------------------------------------------------
|    VideoSurface::paint
+---------------------------------------------------------------------*/
QSGNode* VideoSurface::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    VideoNode* node = static_cast<VideoNode*>(oldNode);

//    static std::chrono::high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
//    int dt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - tp).count();
//    tp = std::chrono::high_resolution_clock::now();
//    __android_log_print(ANDROID_LOG_INFO, "Qt", "dt %dms", dt);

#ifdef Q_ANDROID
    if (!m_env)
    {
        if (s_javaVM->AttachCurrentThread(&m_env, NULL) < 0)
        {
            qCritical() << "Failed to attach to current thread.";
            return nullptr;
        }
    }
#endif

    if (!node)
    {
        if (!s_videoTexture)
        {
            s_videoTexture = new VideoTexture();

#ifdef Q_ANDROID
            jobject surfTexture = m_env->CallStaticObjectMethod(s_classVideoDecoder, s_midSetupDecoder, s_videoTexture->textureId());
            if (!surfTexture)
            {
                qWarning("Failed to instantiate SurfaceTexture.");
            }
            s_surfaceTexture = m_env->NewGlobalRef(surfTexture);
#endif
        }

        node = new VideoNode(s_videoTexture);
        m_isGeomertyDirty = true;
    }

    {
        static int frames = 0;
        frames++;
        static std::chrono::high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
        int dt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - tp).count();
        if (dt > 1000)
        {
            QLOGI("{} frames, {} FPS", frames, frames * 1000 / dt);
            tp = std::chrono::high_resolution_clock::now();
            frames = 0;
        }
    }

    std::lock_guard<std::mutex> lg(s_videoDataMutex);

    if (m_isGeomertyDirty || s_videoResolutionDirty)
    {
        const QRectF br = boundingRect();
        QRectF rect = QRectF(QPointF(0, 0), QSizeF(s_videoResolution.x, s_videoResolution.y).scaled(br.size(), Qt::KeepAspectRatio));
        rect.moveCenter(br.center());
        int orientation = 0;//(m_orientation - m_textureOrientation) % 360;
        if (orientation < 0)
            orientation += 360;
        node->setBoundingRect(
                    rect,
                    orientation,
                    false,
                    false);
        node->markDirty(QSGNode::DirtyGeometry);

        m_isGeomertyDirty = false;
        s_videoResolutionDirty = false;
    }

    //parse NALU packets
    {
        constexpr size_t naluSeparatorSize = sizeof(naluSeparator);
        while (s_videoData.size() > naluSeparatorSize)
        {
            uint8_t const* src = s_videoData.data();
            uint8_t const* p = reinterpret_cast<uint8_t const*>(memmem(src + naluSeparatorSize, s_videoData.size() - naluSeparatorSize, naluSeparator, naluSeparatorSize));
            if (p)
            {
                //nalupacket found
                const size_t naluSize = p - src;
                //__android_log_print(ANDROID_LOG_INFO, "Skptr", "NALU @ %d, %d left", (int)naluSize, (int)(s_videoData.size() - naluSize));

#ifdef Q_ANDROID
                jbyteArray frameData = m_env->NewByteArray(naluSize);

                m_env->SetByteArrayRegion(frameData, 0, static_cast<int>(naluSize), reinterpret_cast<jbyte const*>(src));
                m_env->CallStaticVoidMethod(s_classVideoDecoder, s_midFeedDecoder, frameData, (int)naluSize);

                m_env->DeleteLocalRef(frameData);
#endif

                s_videoData.erase(s_videoData.begin(), s_videoData.begin() + naluSize);
            }
            else
            {
                //__android_log_print(ANDROID_LOG_INFO, "Skptr", "NALU not found");
                break;
            }
        }
    }

#ifdef Q_ANDROID
    if (s_surfaceTexture)
    {
        m_env->CallVoidMethod(s_surfaceTexture, s_midUpdateTexture);
    }
#endif

    node->markDirty(QSGNode::DirtyMaterial);
    update();

    return node;
}

void VideoSurface::addVideoData(void const* data, size_t size, math::vec2u16 const& resolution)
{
    Q_ASSERT(data && size > 0);
    if (!data || size == 0)
    {
        return;
    }

    std::lock_guard<std::mutex> lg(s_videoDataMutex);

    size_t offset = s_videoData.size();
    s_videoData.resize(offset + size);
    memcpy(s_videoData.data() + offset, data, size);

    if (s_videoResolution != resolution)
    {
        s_videoResolution = math::vec2u16(1280, 720);//resolution;
        s_videoResolutionDirty = true;
    }
}


