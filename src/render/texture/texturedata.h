/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QT3DRENDER_TEXTUREDATA_H
#define QT3DRENDER_TEXTUREDATA_H

#include <QOpenGLTexture>
#include <QImage>
#include <QSharedPointer>
#include <Qt3DRender/qt3drender_global.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QT3DRENDERSHARED_EXPORT TexImageData
{
public:
    TexImageData();
    ~TexImageData();

    void cleanup();

    bool isCompressed() const
    { return m_isCompressed; }

    inline int width() const { return m_width; }
    inline int height() const { return m_height; }
    inline int depth() const { return m_depth; }
    inline QOpenGLTexture::TextureFormat format() const { return m_format; }

    void setImage(const QImage &);

    void setData(const QByteArray &data,
                 QOpenGLTexture::PixelFormat fmt,
                 QOpenGLTexture::PixelType ptype);

    bool setCompressedFile(const QString &source);

    QByteArray data() const
    { return m_data; }

    QOpenGLTexture::PixelFormat pixelFormat() const
    { return m_pixelFormat; }

    QOpenGLTexture::PixelType pixelType() const
    { return m_pixelType; }

private:
    int m_width, m_height, m_depth;
    QOpenGLTexture::PixelFormat m_pixelFormat;
    QOpenGLTexture::PixelType m_pixelType;

    bool m_isCompressed;
    QByteArray m_data;
    QOpenGLTexture::TextureFormat m_format;
};

typedef QSharedPointer<TexImageData> TexImageDataPtr;

} // namespace Qt3DRender


QT_END_NAMESPACE

#endif // QT3DRENDER_TEXTUREDATA_H