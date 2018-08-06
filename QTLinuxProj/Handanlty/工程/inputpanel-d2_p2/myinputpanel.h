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

#ifndef MYINPUTPANEL_H
#define MYINPUTPANEL_H

#include <QtGui>
#include <QtCore>

#include "ui_myinputpanelform.h"

#include <QPushButton>

//! [0]

#include "define_screen.h"

class MyInputPanel : public QWidget
{
    Q_OBJECT

public:
    MyInputPanel();

    enum
    {
        kEnumNumber,//数字显示
        kEnumLowercase,//小写
        kEnumUppercase//大写
    };

signals:
    void characterGenerated(unsigned int character);
    void signal_show_input_panel();
protected:
    bool event(QEvent *e);

public slots:

private slots:
    void saveFocusWidget(QWidget *oldFocus, QWidget *newFocus);
    void buttonClicked(QWidget *w);
    void slot_button_test();
    void slot_pushbutton_line3_1();
    void slot_pushbutton_line4_1();
    void slot_pushbutton_close();

    //void slot_pushbutton_line3_1(); //ABC abc
    //void slot_pushbutton_line4_1(); //切换 ABC 123
    //void slot_pushbutton_close();   //关闭键盘

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public:
    void set_model_modal(bool _flag);
private:
    void resizeEvent(QResizeEvent *e);
    void init_new_button(QPushButton *_button,int _x,int _y,int _w,int _h,QString _typeSheet = "");
    void set_inputpanel_show(char _type);

    QPushButton *m_pushbutton_1;
    QPushButton *m_pushbutton_2;
    QPushButton *m_pushbutton_3;
    QPushButton *m_pushbutton_4;
    QPushButton *m_pushbutton_5;
    QPushButton *m_pushbutton_6;
    QPushButton *m_pushbutton_7;
    QPushButton *m_pushbutton_8;
    QPushButton *m_pushbutton_9;
    QPushButton *m_pushbutton_0;
    QPushButton *m_pushbutton_backspace;
    QPushButton *m_pushbutton_close;

    QPushButton *m_pushbutton_line2_1;
    QPushButton *m_pushbutton_line2_2;
    QPushButton *m_pushbutton_line2_3;
    QPushButton *m_pushbutton_line2_4;
    QPushButton *m_pushbutton_line2_5;
    QPushButton *m_pushbutton_line2_6;
    QPushButton *m_pushbutton_line2_7;
    QPushButton *m_pushbutton_line2_8;
    QPushButton *m_pushbutton_line2_9;

    QPushButton *m_pushbutton_line3_1;
    QPushButton *m_pushbutton_line3_2;
    QPushButton *m_pushbutton_line3_3;
    QPushButton *m_pushbutton_line3_4;
    QPushButton *m_pushbutton_line3_5;
    QPushButton *m_pushbutton_line3_6;
    QPushButton *m_pushbutton_line3_7;
    QPushButton *m_pushbutton_line3_8;

    QPushButton *m_pushbutton_line4_1;
    QPushButton *m_pushbutton_line4_2;
    QPushButton *m_pushbutton_line4_3;
    QPushButton *m_pushbutton_line4_4;
    QPushButton *m_pushbutton_line4_5;

    Ui::MyInputPanelForm form;
    QWidget *lastFocusedWidget;
    QSignalMapper signalMapper;

    bool  m_bLeftButtonPress;
    QPoint m_ptPress;
    QPoint m_ptMove;

    bool m_number;  // true:123  false:abc
    bool m_lowercase; //true:adb false:ABC

    CDefineScreen *m_define_screen;

};

//! [0]

#endif // MYINPUTPANEL_H

















