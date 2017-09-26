#include "QMLVideoSurface.h"
#include "Video_Decoder.h"
#include <QResizeEvent>
#include <QTimer>
#include <QThread>

#include <chrono>
#include <thread>
#include <mutex>

#define EGL_SYNC_FENCE_KHR                      0x30F9


static QMLVideoTexture* s_videoTexture = nullptr;
static std::mutex s_videoDataMutex;
static math::vec2u16 s_videoResolution;
static bool s_videoResolutionDirty = false;

QMLVideoTexture::QMLVideoTexture(uint32_t videoTextureId)
    : m_subRect(0, 0, 1, 1)
    , m_textureSize(1280, 720)
    , m_textureId(videoTextureId)
    , m_updated(false)
{
}

QMLVideoTexture::~QMLVideoTexture()
{
    releaseTexture();
}

int QMLVideoTexture::textureId() const
{
    return m_textureId;
}

QSize QMLVideoTexture::textureSize() const
{
    return m_textureSize;
}

void QMLVideoTexture::setTextureSize(const QSize &size)
{
    m_textureSize = size;
}

bool QMLVideoTexture::hasAlphaChannel() const
{
    return false;
}

bool QMLVideoTexture::hasMipmaps() const
{
    return false;
}

QRectF QMLVideoTexture::normalizedTextureSubRect() const
{
    return m_subRect;
}

void QMLVideoTexture::bind()
{
    if (m_textureId != 0)
    {
#if defined RASPBERRY_PI
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_textureId);
#endif
    }
}

bool QMLVideoTexture::updateTexture()
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

void QMLVideoTexture::invalidateTexture()
{
}

void QMLVideoTexture::releaseTexture()
{
    if (m_updated)
    {
        m_updated = false;
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
    QMLVideoMaterial *material = static_cast<QMLVideoMaterial *>(newEffect);

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
            "\n varying highp vec2 v_texcoord;"
            "\n void main(void)"
            "\n {"
            "\n     gl_Position = matrix * position;"
            "\n     v_texcoord = (texcoord * subrect.zw) + subrect.xy;"
            "\n }";
}

const char *VideoMaterialShader::fragmentShader() const
{
    return  "\n #extension GL_OES_EGL_image_external : require"
            "\n uniform samplerExternalOES texture;"
            "\n uniform lowp float opacity;\n"
            "\n varying highp vec2 v_texcoord;"
            "\n void main(void)"
            "\n {"
            "\n     gl_FragColor = opacity * texture2D(texture, v_texcoord.st);"
            "\n }";
}

QMLVideoMaterial::QMLVideoMaterial(QMLVideoTexture *texture)
    : m_texture(texture)
{
}

QSGMaterialShader *QMLVideoMaterial::createShader() const
{
    return new VideoMaterialShader;
}

QSGMaterialType *QMLVideoMaterial::type() const
{
    return &VideoMaterialShader::type;
}

int QMLVideoMaterial::compare(const QSGMaterial *other) const
{
    return m_texture - static_cast<const QMLVideoMaterial *>(other)->m_texture;
}

QMLVideoNode::QMLVideoNode(QMLVideoTexture *texture)
    : m_material(texture)
    , m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
{
    setGeometry(&m_geometry);
    setMaterial(&m_material);
    setFlag(UsePreprocess);
}

QMLVideoNode::~QMLVideoNode()
{
    //m_material.m_texture->deleteLater();
}

void QMLVideoNode::preprocess()
{
    QMLVideoTexture *t = m_material.m_texture;
    if (t && t->updateTexture())
    {
        markDirty(QSGNode::DirtyMaterial);
    }
}

void QMLVideoNode::setBoundingRect(const QRectF &rect, int orientation, bool horizontalMirror, bool verticalMirror)
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
QMLVideoSurface::QMLVideoSurface(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents, true);

//    QTimer* timer = new QTimer;
//    connect(timer, SIGNAL(timeout()), this, SLOT(doUpdate()));
//    timer->setSingleShot(false);
//    timer->start(1);
}

void QMLVideoSurface::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
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
QSGNode* QMLVideoSurface::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QMLVideoNode* node = static_cast<QMLVideoNode*>(oldNode);

    if (!s_videoTexture)
    {
        return nullptr;
    }

    if (!node)
    {
        node = new QMLVideoNode(s_videoTexture);
        m_isGeomertyDirty = true;
    }

    std::lock_guard<std::mutex> lg(s_videoDataMutex);

    if (m_isGeomertyDirty || s_videoResolutionDirty)
    {
        QLOGI("Resolution changed to: {}", s_videoResolution);
        const QRectF br = boundingRect();
        QRectF rect = QRectF(QPointF(0, 0), QSizeF(s_videoResolution.x, s_videoResolution.y).scaled(br.size(), Qt::KeepAspectRatio));
        rect.moveCenter(br.center());

        //align left, top
//        float dx = (br.width() - rect.width()) / 2.f;
//        float dy = (br.height() - rect.height()) / 2.f;
//        rect.translate(-dx, -dy);

        int orientation = 0;//(m_orientation - m_textureOrientation) % 360;
        if (orientation < 0)
            orientation += 360;
        node->setBoundingRect(
                    rect,
                    orientation,
                    false,
                    false);
        node->markDirty(QSGNode::DirtyGeometry);

        QLOGI("Node rect size: {}x{}", rect.width(), rect.height());

        m_isGeomertyDirty = false;
        s_videoResolutionDirty = false;
    }

    node->markDirty(QSGNode::DirtyMaterial);
    update();

    return node;
}

void QMLVideoSurface::init(Video_Decoder& decoder)
{
    if (!s_videoTexture)
    {
        s_videoTexture = new QMLVideoTexture(decoder.get_video_texture_id());
    }
}

void QMLVideoSurface::setResolution(math::vec2u16 const& resolution)
{
    std::lock_guard<std::mutex> lg(s_videoDataMutex);
    if (s_videoResolution != resolution)
    {
        //s_videoResolution = math::vec2u16(1280, 720);
        s_videoResolution = resolution;
        s_videoResolutionDirty = true;
    }
}


