#pragma once

#include <QQuickItem>
#include <QSGMaterial>
#include <QSGTexture>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <vector>
#include <functional>

#include "QBase.h"
#include "qmath.h"

class Video_Decoder;

class QMLVideoTexture : public QSGDynamicTexture
{
    Q_OBJECT
public:
    QMLVideoTexture(uint32_t videoTextureId);
    ~QMLVideoTexture();

    bool isActive() const;

    int textureId() const override;
    QSize textureSize() const override;
    void setTextureSize(const QSize &size);
    bool hasAlphaChannel() const override;
    bool hasMipmaps() const override;

    QRectF normalizedTextureSubRect() const;

    void bind();
    bool updateTexture();

    void invalidateTexture();

public slots:
    void releaseTexture();

private:
    QRectF m_subRect;
    QSize m_textureSize;
    GLuint m_textureId;
    bool m_updated;
};

class QMLVideoMaterial : public QSGMaterial
{
public:
    QMLVideoMaterial(QMLVideoTexture* texture);

    QSGMaterialShader* createShader() const;
    QSGMaterialType* type() const;
    int compare(const QSGMaterial* other) const;

    void setTexture(QMLVideoTexture* texture);

private:
    friend class VideoMaterialShader;
    friend class QMLVideoNode;

    QMLVideoTexture* m_texture;
};

class QMLVideoNode : public QSGGeometryNode
{
public:
    QMLVideoNode(QMLVideoTexture* texture);
    ~QMLVideoNode();

    void setBoundingRect(const QRectF &rect, int orientation, bool horizontalMirror, bool verticalMirror);
    void preprocess();

private:
    QMLVideoMaterial m_material;
    QSGGeometry m_geometry;
};

class QMLVideoSurface : public QQuickItem
{
    Q_OBJECT
public:
    explicit QMLVideoSurface(QQuickItem* parent = 0);

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* paintNodeData) override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    static void init(Video_Decoder& decoder);
    static void setResolution(math::vec2u16 const& resolution);

private:
    bool m_isGeomertyDirty = true;
};

