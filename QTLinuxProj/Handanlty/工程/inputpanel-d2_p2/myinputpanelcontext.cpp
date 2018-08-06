/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore>

#include "myinputpanelcontext.h"

//! [0]

MyInputPanelContext::MyInputPanelContext()
{
    inputPanel = new MyInputPanel;
    //inputPanel->resize(261,233);
    connect(inputPanel, SIGNAL(characterGenerated(unsigned int)), SLOT(sendCharacter(unsigned int)));
    connect(inputPanel,SIGNAL(signal_show_input_panel()),this,SLOT(slot_show_input_panel()));
}

//! [0]

QWidget *MyInputPanelContext::current_window()
{
    return inputPanel;
}

MyInputPanelContext::~MyInputPanelContext()
{
    delete inputPanel;
}

//! [1]

bool MyInputPanelContext::filterEvent(const QEvent* event)
{
    if (event->type() == QEvent::RequestSoftwareInputPanel) {
        //qDebug() << "MyInputPanelContext::filterEvent() RequestSoftwareInputPanel";
        updatePosition();
        inputPanel->show();
        return true;
    } else if (event->type() == QEvent::CloseSoftwareInputPanel) {
        //qDebug() << "MyInputPanelContext::filterEvent() CloseSoftwareInputPanel";
        inputPanel->hide();
        return true;
    }
    return false;
}

//! [1]

void MyInputPanelContext::slot_show_input_panel()
{
     updatePosition();
     inputPanel->show();
}


QString MyInputPanelContext::identifierName()
{
    return "MyInputPanelContext";
}

void MyInputPanelContext::reset()
{
}

bool MyInputPanelContext::isComposing() const
{
    return false;
}

QString MyInputPanelContext::language()
{
    return "en_US";
}

//! [2]

void MyInputPanelContext::sendCharacter(unsigned int character)
{
    QPointer<QWidget> w = focusWidget();

    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, character, Qt::NoModifier, QString(character));
    QApplication::sendEvent(w, &keyPress);

    if (!w)
        return;

    QKeyEvent keyRelease(QEvent::KeyRelease, character, Qt::NoModifier, QString());
    QApplication::sendEvent(w, &keyRelease);
}

//! [2]

//! [3]

void MyInputPanelContext::updatePosition()
{
    QWidget *widget = focusWidget();
    if (!widget)
        return;

    QRect widgetRect = widget->rect();
    //QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom() + 2);
    QPoint panelPos = QPoint(widgetRect.left() + widgetRect.width()/2 - inputPanel->width()/2, widgetRect.bottom());
    QPoint panel_move_pos;
    panel_move_pos = widget->mapToGlobal(panelPos);

    int point_x = widgetRect.left() + widgetRect.width()/2 - inputPanel->width()/2;
    int point_y = widgetRect.bottom() + 10;//widgetRect.top() - 10 - inputPanel->height();

#if 0
    if(panel_move_pos.y() + inputPanel->height() >= 768)
    {
        //qDebug() << "updatePosition() 1";
        point_y = widgetRect.top() - 10 - inputPanel->height();
        panelPos = QPoint(point_x, point_y);
    }
    if(panel_move_pos.x() + inputPanel->width() > 1024)
    {
        //qDebug() << "updatePosition() 2";
        point_x = widgetRect.right() - inputPanel->width();
        panelPos = QPoint(point_x, point_y);
    }

    if(panel_move_pos.x() < 0)
    {
        //qDebug() << "updatePosition() 3";
        point_x = widgetRect.left();

        panelPos = QPoint(point_x, point_y);
    }


    panel_move_pos = widget->mapToGlobal(panelPos);
#else

#if defined(APPLICATION_TYPE_D2)
    int screen_height = 600;
#elif defined(APPLICATION_TYPE_P2)
    int screen_height = 480;
#else

#endif

    if(panel_move_pos.y() + inputPanel->height() >= screen_height)
    {
        panel_move_pos.setX(0);
        panel_move_pos.setY(0);
    }
    else
    {
        panel_move_pos.setX(0);
        panel_move_pos.setY(screen_height - inputPanel->height());
    }

#endif

    inputPanel->move(panel_move_pos);
}

void MyInputPanelContext::set_model_modal(bool _flag)
{
    inputPanel->set_model_modal(_flag);
}







//! [3]
