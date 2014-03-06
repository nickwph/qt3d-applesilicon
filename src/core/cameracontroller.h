/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <QObject>
#include "qt3dcore_global.h"

#include <QPoint>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;


namespace Qt3D
{

class Camera;
class Entity;

class CameraController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( float linearSpeed READ linearSpeed WRITE setLinearSpeed NOTIFY linearSpeedChanged )

    Q_PROPERTY( float orbitRate READ orbitRate WRITE setOrbitRate NOTIFY orbitRateChanged )
    Q_PROPERTY( float lookRate READ lookRate WRITE setLookRate NOTIFY lookRateChanged )

    Q_PROPERTY( bool multisampleEnabled READ isMultisampleEnabled() NOTIFY multisampleEnabledChanged )
public:
    CameraController();

    void setCamera( Camera* cam );
    void setLinearSpeed( float speed );
    float linearSpeed() const;

    float orbitRate() const;
    void setOrbitRate( float rate );

    float lookRate() const;
    void setLookRate( float rate );

    void mousePressEvent( QMouseEvent* aEvent );
    void mouseReleaseEvent( QMouseEvent* aEvent );
    void mouseMoveEvent( QMouseEvent* aEvent );

    bool keyPressEvent( QKeyEvent* aEvent );
    bool keyReleaseEvent( QKeyEvent* aEvent );

    bool handleScroll( QWheelEvent* aWheel );

    void update( double t );

    bool isMultisampleEnabled() const;
public slots:
    void toggleMSAA();



signals:
    void linearSpeedChanged();
    void orbitRateChanged();
    void lookRateChanged();

    void multisampleEnabledChanged();
private:

    Camera* m_camera;
    Entity* m_cameraEntity;

    float m_vx;
    float m_vy;
    float m_vz;
    bool m_viewCenterFixed;
    float m_panAngle;
    float m_tiltAngle;

    bool m_leftButtonPressed;
    QPoint m_prevPos;
    QPoint m_pos;
    bool m_orbitMode;

    float m_linearSpeed;
    float m_time;
    float m_orbitRate;
    float m_lookRate;

    bool m_translateFast;
    bool m_multisampleEnabled;
};

} // of namespace Qt3D

#endif // of CAMERA_CONTROLLER_H