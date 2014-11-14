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

#include "inputhandler_p.h"
#include "inputmanagers_p.h"
#include "keyboardeventfilter_p.h"
#include "assignkeyboardfocusjob_p.h"
#include "keyeventdispatcherjob_p.h"
#include <QWindow>

QT_BEGIN_NAMESPACE

namespace Qt3D {

namespace Input {

InputHandler::InputHandler()
    : m_keyboardControllerManager(new KeyboardControllerManager())
    , m_keyboardInputManager(new KeyboardInputManager())
    , m_window(Q_NULLPTR)
    , m_keyboardEventFilter(new KeyboardEventFilter())
{
    m_keyboardEventFilter->setInputHandler(this);
}

// Called in MainThread
void InputHandler::setWindow(QWindow *window)
{
    if (window != m_window) {
        if (m_window)
            m_window->removeEventFilter(m_keyboardEventFilter);
        clearPendingKeyEvents();
        m_window = window;
        if (m_window)
            m_window->installEventFilter(m_keyboardEventFilter);
    }
}

// Called by the keyboardEventFilter in the main thread
void InputHandler::appendKeyEvent(const QKeyEvent &event)
{
    QMutexLocker lock(&m_mutex);
    m_pendingEvents.append(event);
}

// Called by QInputASpect::jobsToExecute (aspectThread)
QList<QKeyEvent> InputHandler::pendingKeyEvents()
{
    QMutexLocker lock(&m_mutex);
    QList<QKeyEvent> pendingEvents = m_pendingEvents;
    m_pendingEvents.clear();
    return pendingEvents;
}

// Called by QInputASpect::jobsToExecute (aspectThread)
void InputHandler::clearPendingKeyEvents()
{
    QMutexLocker lock(&m_mutex);
    m_pendingEvents.clear();
}

void InputHandler::appendKeyboardController(HKeyboardController controller)
{
    m_activeKeyboardControllers.append(controller);
}

void InputHandler::removeKeyboardController(HKeyboardController controller)
{
    m_activeKeyboardControllers.removeAll(controller);
}

// Return a vector of jobs to be performed for keyboard events
// Handles all dependencies between jobs
QVector<QAspectJobPtr> InputHandler::keyboardJobs()
{
    // One job for Keyboard focus change event per Keyboard Controller
    QVector<QAspectJobPtr> jobs;
    const QList<QKeyEvent> events = pendingKeyEvents();

    Q_FOREACH (const HKeyboardController cHandle, m_activeKeyboardControllers) {
        KeyboardController *controller = m_keyboardControllerManager->data(cHandle);
        if (controller) {
            QAspectJobPtr focusChangeJob;
            if (controller->lastKeyboardInputRequester() != controller->currentFocusItem()) {
                AssignKeyboardFocusJob *job = new AssignKeyboardFocusJob(controller->peerUuid());
                job->setInputHandler(this);
                focusChangeJob.reset(job);
                jobs.append(focusChangeJob);
                // One job for Keyboard events (depends on the focus change job if there was one)
            }
            // Event dispacthing job
            if (!events.isEmpty()) {
                KeyEventDispatcherJob *job = new KeyEventDispatcherJob(controller->currentFocusItem(), events);
                job->setInputHandler(this);
                if (focusChangeJob)
                    job->addDependency(focusChangeJob);
                jobs.append(QAspectJobPtr(job));
            }
        }
    }
    return jobs;
}

} // Input

} // Qt3D

QT_END_NAMESPACE