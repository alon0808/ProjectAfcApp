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

#include "myinputpanel.h"

//! [0]

#define BUTTON_KEY_WIDTH    90
#define BUTTON_KEY_HEIGHT   65

MyInputPanel::MyInputPanel()
    : QWidget(0, Qt::Tool | Qt::WindowStaysOnTopHint),
      lastFocusedWidget(0)
{
    //form.setupUi(this);
    this->setObjectName("MyInputPanel");
    this->setMouseTracking(true);
#ifndef APPLICATION_TYPE_P2
    setAttribute(Qt::WA_ShowModal,true);
#endif
    //setStyleSheet("QWidget{border:1px solid #ffffff;}");
    this->setStyleSheet("QWidget#MyInputPanel{border:1px solid #ffffff;background-color:#0F1D31;}");
    this->setFixedSize(1024,333);

    m_define_screen = new CDefineScreen();

    QWidget *widget_line1 = new QWidget(this);
    QWidget *widget_line2 = new QWidget(this);

    widget_line1->setFixedSize(1024,1);
    widget_line1->move(0,0);
    widget_line1->setStyleSheet("QWidget{background-color:#275ba7;}");

    widget_line2->setFixedSize(1024,1);
    widget_line2->move(0,this->height() - 1);
    widget_line2->setStyleSheet("QWidget{background-color:#275ba7;}");


        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(saveFocusWidget(QWidget*,QWidget*)));

    QString strSheet =  "QPushButton {\
                                      border:1px solid #3ea2ff;\
                                      background:#0F1D31;\
                                      font:30px;\
                                      border-radius: 6px;\
                                      color:#80e3ff;\
                                      } \
                                      QPushButton:pressed {background:#275ba7;} \
                                      QPushButton:hover:!pressed {background:#275ba7;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}";
    m_pushbutton_1 = new QPushButton(this);
    init_new_button(m_pushbutton_1,17,19,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_1->setText(tr("1"));

    m_pushbutton_2 = new QPushButton(this);
    init_new_button(m_pushbutton_2,117,19,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_2->setText(tr("2"));

    m_pushbutton_3 = new QPushButton(this);
    init_new_button(m_pushbutton_3,217,19,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_3->setText(tr("3"));

    m_pushbutton_4 = new QPushButton(this);
    init_new_button(m_pushbutton_4,317,19,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_4->setText(tr("4"));

    m_pushbutton_5 = new QPushButton(this);
    init_new_button(m_pushbutton_5,417,19,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_5->setText(tr("5"));

    m_pushbutton_6 = new QPushButton(this);
    init_new_button(m_pushbutton_6,517,19,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_6->setText(tr("6"));

    m_pushbutton_7 = new QPushButton(this);
    init_new_button(m_pushbutton_7,617,19,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_7->setText(tr("7"));

    m_pushbutton_8 = new QPushButton(this);
    init_new_button(m_pushbutton_8,717,19,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_8->setText(tr("8"));

    m_pushbutton_9 = new QPushButton(this);
    init_new_button(m_pushbutton_9,817,19,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_9->setText(tr("9"));

    m_pushbutton_0 = new QPushButton(this);
    init_new_button(m_pushbutton_0,917,19,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_0->setText(tr("0"));



    m_pushbutton_close = new QPushButton(this);
    init_new_button(m_pushbutton_close,874,249,130,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_close->setText(tr("Esc"));

//第二行
     m_pushbutton_line2_1 = new QPushButton(this);
    init_new_button(m_pushbutton_line2_1,67,96,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line2_1->setText(tr("-"));

    m_pushbutton_line2_2 = new QPushButton(this);
    init_new_button(m_pushbutton_line2_2,167,96,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line2_2->setText(tr("/"));

    m_pushbutton_line2_3 = new QPushButton(this);
    init_new_button(m_pushbutton_line2_3,267,96,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line2_3->setText(tr(":"));

    m_pushbutton_line2_4 = new QPushButton(this);
    init_new_button(m_pushbutton_line2_4,367,96,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line2_4->setText(tr(";"));

    m_pushbutton_line2_5 = new QPushButton(this);
    init_new_button(m_pushbutton_line2_5,467,96,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line2_5->setText(tr("("));

    m_pushbutton_line2_6 = new QPushButton(this);
    init_new_button(m_pushbutton_line2_6,567,96,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line2_6->setText(tr(")"));

    m_pushbutton_line2_7 = new QPushButton(this);
    init_new_button(m_pushbutton_line2_7,667,96,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line2_7->setText(tr("$"));

    m_pushbutton_line2_8 = new QPushButton(this);
    init_new_button(m_pushbutton_line2_8,767,96,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line2_8->setText(tr("@"));

    m_pushbutton_line2_9 = new QPushButton(this);
    init_new_button(m_pushbutton_line2_9,867,96,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line2_9->setText(tr("%"));
//第三行
    m_pushbutton_line3_1 = new QPushButton(this);
    init_new_button(m_pushbutton_line3_1,17,173,130,BUTTON_KEY_HEIGHT,strSheet);
   // m_pushbutton_line3_1->setText(tr("-"));
   m_pushbutton_line3_1->setIcon(QIcon(":/img/res/capital.png"));
   m_pushbutton_line3_1->setIconSize(QSize(m_define_screen->get_change_factor_x(30),m_define_screen->get_change_factor_y(33)));

    m_pushbutton_line3_2 = new QPushButton(this);
    init_new_button(m_pushbutton_line3_2,167,173,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line3_2->setText(tr("#"));

    m_pushbutton_line3_3 = new QPushButton(this);
    init_new_button(m_pushbutton_line3_3,267,173,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line3_3->setText(tr("。"));

    m_pushbutton_line3_4 = new QPushButton(this);
    init_new_button(m_pushbutton_line3_4,367,173,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line3_4->setText(tr(","));

    m_pushbutton_line3_5 = new QPushButton(this);
    init_new_button(m_pushbutton_line3_5,467,173,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line3_5->setText(tr(","));

    m_pushbutton_line3_6 = new QPushButton(this);
    init_new_button(m_pushbutton_line3_6,567,173,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line3_6->setText(tr("?"));

    m_pushbutton_line3_7 = new QPushButton(this);
    init_new_button(m_pushbutton_line3_7,667,173,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line3_7->setText(tr("!"));

    m_pushbutton_line3_8 = new QPushButton(this);
    init_new_button(m_pushbutton_line3_8,767,173,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line3_8->setText(tr("."));

    m_pushbutton_backspace = new QPushButton(this);
    init_new_button(m_pushbutton_backspace,874,173,130,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_backspace->setIcon(QIcon(":/img/res/delete.png"));
    m_pushbutton_backspace->setIconSize(QSize(m_define_screen->get_change_factor_x(35),m_define_screen->get_change_factor_y(33)));

//第四行

    m_pushbutton_line4_1 = new QPushButton(this);
    init_new_button(m_pushbutton_line4_1,17,249,130,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line4_1->setText(tr("ABC"));

    m_pushbutton_line4_2 = new QPushButton(this);
    init_new_button(m_pushbutton_line4_2,167,249,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line4_2->setIcon(QIcon(":/img/res/back.png"));
    m_pushbutton_line4_2->setIconSize(QSize(m_define_screen->get_change_factor_x(14),m_define_screen->get_change_factor_y(33)));

    m_pushbutton_line4_3 = new QPushButton(this);
    init_new_button(m_pushbutton_line4_3,267,249,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line4_3->setIcon(QIcon(":/img/res/front.png"));
    m_pushbutton_line4_3->setIconSize(QSize(m_define_screen->get_change_factor_x(14),m_define_screen->get_change_factor_x(33)));

    m_pushbutton_line4_4 = new QPushButton(this);
    init_new_button(m_pushbutton_line4_4,367,249,390,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line4_4->setText(tr("空格"));

    m_pushbutton_line4_5 = new QPushButton(this);
    init_new_button(m_pushbutton_line4_5,767,249,BUTTON_KEY_WIDTH,BUTTON_KEY_HEIGHT,strSheet);
    m_pushbutton_line4_5->setIcon(QIcon(":/img/res/enter.png"));
    m_pushbutton_line4_5->setIconSize(QSize(m_define_screen->get_change_factor_x(36),m_define_screen->get_change_factor_y(33)));


    signalMapper.setMapping(m_pushbutton_1, m_pushbutton_1);
    signalMapper.setMapping(m_pushbutton_2, m_pushbutton_2);
    signalMapper.setMapping(m_pushbutton_3, m_pushbutton_3);
    signalMapper.setMapping(m_pushbutton_4, m_pushbutton_4);
    signalMapper.setMapping(m_pushbutton_5, m_pushbutton_5);
    signalMapper.setMapping(m_pushbutton_6, m_pushbutton_6);
    signalMapper.setMapping(m_pushbutton_7, m_pushbutton_7);
    signalMapper.setMapping(m_pushbutton_8, m_pushbutton_8);
    signalMapper.setMapping(m_pushbutton_9, m_pushbutton_9);
    signalMapper.setMapping(m_pushbutton_0, m_pushbutton_0);
    signalMapper.setMapping(m_pushbutton_backspace, m_pushbutton_backspace);
//第二行
    signalMapper.setMapping(m_pushbutton_line2_1, m_pushbutton_line2_1);
    signalMapper.setMapping(m_pushbutton_line2_2, m_pushbutton_line2_2);
    signalMapper.setMapping(m_pushbutton_line2_3, m_pushbutton_line2_3);
    signalMapper.setMapping(m_pushbutton_line2_4, m_pushbutton_line2_4);
    signalMapper.setMapping(m_pushbutton_line2_5, m_pushbutton_line2_5);
    signalMapper.setMapping(m_pushbutton_line2_6, m_pushbutton_line2_6);
    signalMapper.setMapping(m_pushbutton_line2_7, m_pushbutton_line2_7);
    signalMapper.setMapping(m_pushbutton_line2_8, m_pushbutton_line2_8);
    signalMapper.setMapping(m_pushbutton_line2_9, m_pushbutton_line2_9);
//第三行
    signalMapper.setMapping(m_pushbutton_line3_1, m_pushbutton_line3_1);
    signalMapper.setMapping(m_pushbutton_line3_2, m_pushbutton_line3_2);
    signalMapper.setMapping(m_pushbutton_line3_3, m_pushbutton_line3_3);
    signalMapper.setMapping(m_pushbutton_line3_4, m_pushbutton_line3_4);
    signalMapper.setMapping(m_pushbutton_line3_5, m_pushbutton_line3_5);
    signalMapper.setMapping(m_pushbutton_line3_6, m_pushbutton_line3_6);
    signalMapper.setMapping(m_pushbutton_line3_7, m_pushbutton_line3_7);
    signalMapper.setMapping(m_pushbutton_line3_8, m_pushbutton_line3_8);
//第四行
    signalMapper.setMapping(m_pushbutton_line4_1, m_pushbutton_line4_1);
    signalMapper.setMapping(m_pushbutton_line4_2, m_pushbutton_line4_2);
    signalMapper.setMapping(m_pushbutton_line4_3, m_pushbutton_line4_3);
    signalMapper.setMapping(m_pushbutton_line4_4, m_pushbutton_line4_4);
    signalMapper.setMapping(m_pushbutton_line4_5, m_pushbutton_line4_5);

    signalMapper.setMapping(m_pushbutton_close, m_pushbutton_close);

    connect(m_pushbutton_1, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_2, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_3, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_4, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_5, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_6, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_7, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_8, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_9, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_0, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_backspace, SIGNAL(clicked()),&signalMapper, SLOT(map()));
//第二行
    connect(m_pushbutton_line2_1, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line2_2, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line2_3, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line2_4, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line2_5, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line2_6, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line2_7, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line2_8, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line2_9, SIGNAL(clicked()),&signalMapper, SLOT(map()));
//第三行
    connect(m_pushbutton_line3_1, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line3_2, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line3_3, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line3_4, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line3_5, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line3_6, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line3_7, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line3_8, SIGNAL(clicked()),&signalMapper, SLOT(map()));

//第四行
    connect(m_pushbutton_line4_1, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line4_2, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line4_3, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line4_4, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(m_pushbutton_line4_5, SIGNAL(clicked()),&signalMapper, SLOT(map()));

    connect(m_pushbutton_close, SIGNAL(clicked()),&signalMapper, SLOT(map()));

    connect(&signalMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(buttonClicked(QWidget*)));


   // bool ret = connect(m_pushbutton_close,SIGNAL(clicked()),this,SLOT(slot_pushbutton_close()));

/*
Object::connect: No such slot MyInputPanel::slot_pushbutton_close()
Object::connect:  (receiver name: 'MyInputPanel')

*/

   // connect(m_pushbutton_line3_1,SIGNAL(clicked()),SLOT(slot_pushbutton_line3_1()));
  //  connect(m_pushbutton_line4_1,SIGNAL(clicked()),SLOT(slot_pushbutton_line4_1()));
 //  bool ret = connect(m_pushbutton_close,SIGNAL(clicked()),SLOT(slot_pushbutton_close()));
 //   qDebug() << "MyInputPanel() m_pushbutton_close " << ret;

    m_bLeftButtonPress = false;
    m_number = true;
    m_lowercase = true;


    m_define_screen->add_parent(this);
    m_define_screen->reset_widget();
    //slot_pushbutton_line4_1();
}

void MyInputPanel::resizeEvent(QResizeEvent *e)
{
    //m_define_screen->reset_widget();
}

void MyInputPanel::slot_pushbutton_line3_1() //ABC abc
{
    if(!m_number)
    {
        if(m_lowercase)
        {
            m_lowercase = false;
            set_inputpanel_show(kEnumUppercase);
        }
        else
        {
            m_lowercase = true;
            set_inputpanel_show(kEnumLowercase);
        }
    }
}

void MyInputPanel::slot_pushbutton_line4_1() //切换 ABC 123
{
    if(m_number)
    {
        m_number = false;
        m_lowercase = true;
        set_inputpanel_show(kEnumLowercase);
        m_pushbutton_line4_1->setText(tr("123"));

    }
    else
    {
        m_number = true;
        set_inputpanel_show(kEnumNumber);
        m_pushbutton_line4_1->setText(tr("ABC"));

    }
}

void MyInputPanel::slot_pushbutton_close()   //关闭键盘
{
    qDebug() << "MyInputPanel() slot_pushbutton_close() ";
    this->hide();
}


void MyInputPanel::slot_button_test()
{
    //qDebug() << "slot_button_test";
    emit characterGenerated(Qt::Key_Backspace);
}
//! [0]



bool MyInputPanel::event(QEvent *e)
{
    switch (e->type()) {
//! [1]
    case QEvent::WindowActivate:
        //qDebug() << "MyInputPanel::event  QEvent::WindowActivate";
        if (lastFocusedWidget)
            lastFocusedWidget->activateWindow();
        break;
//! [1]
    default:
        break;
    }

    return QWidget::event(e);
}

//! [2]

void MyInputPanel::saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus)
{
    if (newFocus != 0 && !this->isAncestorOf(newFocus)) {
        //qDebug() << "MyInputPanel::saveFocusWidget";
        lastFocusedWidget = newFocus;
        if (lastFocusedWidget->inherits("QLineEdit")
            || lastFocusedWidget->inherits("QTextEdit"))
        {
            //qDebug() << "MyInputPanel::saveFocusWidget signal_show_input_panel";
            emit signal_show_input_panel();
        }
    }
}

/*
    m_number = true;  // true:123  false:abc
    m_lowercase = true; //true:adb false:ABC
*/



void MyInputPanel::buttonClicked(QWidget *w)
{
    unsigned int chr = 0;//qvariant_cast<unsigned int>(w->property("buttonValue"));
 //第一行
 /*
             m_pushbutton_1->setText(tr("q"));
            m_pushbutton_2->setText(tr("w"));
            m_pushbutton_3->setText(tr("e"));
            m_pushbutton_4->setText(tr("r"));
            m_pushbutton_5->setText(tr("t"));
            m_pushbutton_6->setText(tr("y"));
            m_pushbutton_7->setText(tr("u"));
            m_pushbutton_8->setText(tr("i"));
            m_pushbutton_9->setText(tr("o"));
            m_pushbutton_0->setText(tr("p"));
 */
    if(m_pushbutton_1 == w)
    {
        if(m_number)
        {
            chr = Qt::Key_1;
        }
        else
        {
            if(m_lowercase)
                chr = 'q';
            else
                chr = 'Q';
        }
    }
    else if(m_pushbutton_2 == w)
    {
        if(m_number)
        {
            chr = Qt::Key_2;
        }
        else
        {
            if(m_lowercase)
                chr = 'w';
            else
                chr = 'W';
        }
    }
    else if(m_pushbutton_3 == w)
    {
        if(m_number)
        {
            chr = Qt::Key_3;
        }
        else
        {
            if(m_lowercase)
                chr = 'e';
            else
                chr = 'E';
        }
    }
    else if(m_pushbutton_4 == w)
    {
        if(m_number)
        {
            chr = Qt::Key_4;
        }
        else
        {
            if(m_lowercase)
                chr = 'r';
            else
                chr = 'R';
        }
    }
    else if(m_pushbutton_5 == w)
    {
        if(m_number)
        {
            chr = Qt::Key_5;
        }
        else
        {
            if(m_lowercase)
                chr = 't';
            else
                chr = 'T';
        }
    }
    else if(m_pushbutton_6 == w)
    {
        if(m_number)
        {
            chr = Qt::Key_6;
        }
        else
        {
            if(m_lowercase)
                chr = 'y';
            else
                chr = 'Y';
        }
    }
    else if(m_pushbutton_7 == w)
    {
        if(m_number)
        {
            chr = Qt::Key_7;
        }
        else
        {
            if(m_lowercase)
                chr = 'u';
            else
                chr = 'U';
        }
    }
    else if(m_pushbutton_8 == w)
    {
        if(m_number)
        {
            chr = Qt::Key_8;
        }
        else
        {
            if(m_lowercase)
                chr = 'i';
            else
                chr = 'I';
        }
    }
    else if(m_pushbutton_9 == w)
    {
        if(m_number)
        {
            chr = Qt::Key_9;
        }
        else
        {
            if(m_lowercase)
                chr = 'o';
            else
                chr = 'O';
        }
    }
    else if(m_pushbutton_0 == w)
    {
        if(m_number)
        {
            chr = Qt::Key_0;
        }
        else
        {
            if(m_lowercase)
                chr = 'p';
            else
                chr = 'P';
        }
    }
 //第二行
/*
            m_pushbutton_line2_1->setText(tr("-"));
            m_pushbutton_line2_2->setText(tr("/"));
            m_pushbutton_line2_3->setText(tr(":"));
            m_pushbutton_line2_4->setText(tr(";"));
            m_pushbutton_line2_5->setText(tr("("));
            m_pushbutton_line2_6->setText(tr(")"));
            m_pushbutton_line2_7->setText(tr("$"));
            m_pushbutton_line2_8->setText(tr("@"));
            m_pushbutton_line2_9->setText(tr("%"));
*/
    else if(m_pushbutton_line2_1 == w)
    {
        if(m_number)
        {
            chr = '-';
        }
        else
        {
            if(m_lowercase)
                chr = 'a';
            else
                chr = 'A';
        }
    }
    else if(m_pushbutton_line2_2 == w)
    {
        if(m_number)
        {
            chr = '/';
        }
        else
        {
            if(m_lowercase)
                chr = 's';
            else
                chr = 'S';
        }
    }
    else if(m_pushbutton_line2_3 == w)
    {
        if(m_number)
        {
            chr = ':';
        }
        else
        {
            if(m_lowercase)
                chr = 'd';
            else
                chr = 'D';
        }
    }
    else if(m_pushbutton_line2_4 == w)
    {
        if(m_number)
        {
            chr = ';';
        }
        else
        {
            if(m_lowercase)
                chr = 'f';
            else
                chr = 'F';
        }
    }
    else if(m_pushbutton_line2_5 == w)
    {
        if(m_number)
        {
            chr = '(';
        }
        else
        {
            if(m_lowercase)
                chr = 'g';
            else
                chr = 'G';
        }
    }
    else if(m_pushbutton_line2_6 == w)
    {
        if(m_number)
        {
            chr = ')';
        }
        else
        {
            if(m_lowercase)
                chr = 'h';
            else
                chr = 'H';
        }
    }
    else if(m_pushbutton_line2_7 == w)
    {
        if(m_number)
        {
            chr = '$';
        }
        else
        {
            if(m_lowercase)
                chr = 'j';
            else
                chr = 'J';
        }
    }
    else if(m_pushbutton_line2_8 == w)
    {
        if(m_number)
        {
            chr = '@';
        }
        else
        {
            if(m_lowercase)
                chr = 'k';
            else
                chr = 'K';
        }
    }
    else if(m_pushbutton_line2_9 == w)
    {
        if(m_number)
        {
            chr = '%';
        }
        else
        {
            if(m_lowercase)
                chr = 'l';
            else
                chr = 'L';
        }
    }
 //第三行
 /*
             m_pushbutton_line3_2->setText(tr("#"));
            m_pushbutton_line3_3->setText(tr("。"));
            m_pushbutton_line3_4->setText(tr(","));
            m_pushbutton_line3_5->setText(tr(","));
            m_pushbutton_line3_6->setText(tr("?"));
            m_pushbutton_line3_7->setText(tr("!"));
            m_pushbutton_line3_8->setText(tr("."));
 */
    else if(m_pushbutton_line3_2 == w)
    {
        if(m_number)
        {
            chr = '#';
        }
        else
        {
            if(m_lowercase)
                chr = 'z';
            else
                chr = 'Z';
        }
    }
    else if(m_pushbutton_line3_3 == w)
    {
        if(m_number)
        {
            chr = 0x2c;
        }
        else
        {
            if(m_lowercase)
                chr = 'x';
            else
                chr = 'X';
        }
    }
    else if(m_pushbutton_line3_4 == w)
    {
        if(m_number)
        {
            chr = 0x2c;
        }
        else
        {
            if(m_lowercase)
                chr = 'c';
            else
                chr = 'C';
        }
    }
    else if(m_pushbutton_line3_5 == w)
    {
        if(m_number)
        {
            chr = 0x60;
        }
        else
        {
            if(m_lowercase)
                chr = 'v';
            else
                chr = 'V';
        }
    }
    else if(m_pushbutton_line3_6 == w)
    {
        if(m_number)
        {
            chr = '?';
        }
        else
        {
            if(m_lowercase)
                chr = 'b';
            else
                chr = 'B';
        }
    }
    else if(m_pushbutton_line3_7 == w)
     {
        if(m_number)
        {
            chr = '!';
        }
        else
        {
            if(m_lowercase)
                chr = 'n';
            else
                chr = 'N';
        }
    }
    else if(m_pushbutton_line3_8 == w)
     {
        if(m_number)
        {
            chr = '.';
        }
        else
        {
            if(m_lowercase)
                chr = 'm';
            else
                chr = 'M';
        }
    }
//第四行 m_pushbutton_line4_2
    else if(m_pushbutton_line4_2 == w)  //向前
    {
        chr = Qt::Key_Left;
    }
    else if(m_pushbutton_line4_3 == w) //向后
    {
        chr = Qt::Key_Right;
    }
    else if(m_pushbutton_line4_4 == w) //空格
    {
        chr = Qt::Key_Space;
    }
    else if(m_pushbutton_line4_5 == w) //回车
    {
        chr = Qt::Key_Enter;
    }

    else if(m_pushbutton_backspace == w)
        chr = Qt::Key_Backspace;
    else if(m_pushbutton_line3_1 == w)
        slot_pushbutton_line3_1();
    else if(m_pushbutton_line4_1 == w)
        slot_pushbutton_line4_1();
    else if(m_pushbutton_close == w)
        slot_pushbutton_close();


        //qDebug() << "buttonClicked" << chr;
    if(m_pushbutton_line3_1 != w &&
      m_pushbutton_line4_1 != w &&
      m_pushbutton_close != w)
        emit characterGenerated(chr);


}

void MyInputPanel::set_inputpanel_show(char _type)
{
    switch(_type)
    {
        case kEnumNumber://数字显示
        {
            m_pushbutton_1->setText(tr("1"));
            m_pushbutton_2->setText(tr("2"));
            m_pushbutton_3->setText(tr("3"));
            m_pushbutton_4->setText(tr("4"));
            m_pushbutton_5->setText(tr("5"));
            m_pushbutton_6->setText(tr("6"));
            m_pushbutton_7->setText(tr("7"));
            m_pushbutton_8->setText(tr("8"));
            m_pushbutton_9->setText(tr("9"));
            m_pushbutton_0->setText(tr("0"));
        //第二行
            m_pushbutton_line2_1->setText(tr("-"));
            m_pushbutton_line2_2->setText(tr("/"));
            m_pushbutton_line2_3->setText(tr(":"));
            m_pushbutton_line2_4->setText(tr(";"));
            m_pushbutton_line2_5->setText(tr("("));
            m_pushbutton_line2_6->setText(tr(")"));
            m_pushbutton_line2_7->setText(tr("$"));
            m_pushbutton_line2_8->setText(tr("@"));
            m_pushbutton_line2_9->setText(tr("%"));
        //第三行
            m_pushbutton_line3_2->setText(tr("#"));
            m_pushbutton_line3_3->setText(tr("。"));
            m_pushbutton_line3_4->setText(tr(","));
            m_pushbutton_line3_5->setText(tr(","));
            m_pushbutton_line3_6->setText(tr("?"));
            m_pushbutton_line3_7->setText(tr("!"));
            m_pushbutton_line3_8->setText(tr("."));
        //第四行


        }break;
        case kEnumLowercase://小写
        {
            m_pushbutton_1->setText(tr("q"));
            m_pushbutton_2->setText(tr("w"));
            m_pushbutton_3->setText(tr("e"));
            m_pushbutton_4->setText(tr("r"));
            m_pushbutton_5->setText(tr("t"));
            m_pushbutton_6->setText(tr("y"));
            m_pushbutton_7->setText(tr("u"));
            m_pushbutton_8->setText(tr("i"));
            m_pushbutton_9->setText(tr("o"));
            m_pushbutton_0->setText(tr("p"));
        //第二行
            m_pushbutton_line2_1->setText(tr("a"));
            m_pushbutton_line2_2->setText(tr("s"));
            m_pushbutton_line2_3->setText(tr("d"));
            m_pushbutton_line2_4->setText(tr("f"));
            m_pushbutton_line2_5->setText(tr("g"));
            m_pushbutton_line2_6->setText(tr("h"));
            m_pushbutton_line2_7->setText(tr("j"));
            m_pushbutton_line2_8->setText(tr("k"));
            m_pushbutton_line2_9->setText(tr("l"));
        //第三行
            m_pushbutton_line3_2->setText(tr("z"));
            m_pushbutton_line3_3->setText(tr("x"));
            m_pushbutton_line3_4->setText(tr("c"));
            m_pushbutton_line3_5->setText(tr("v"));
            m_pushbutton_line3_6->setText(tr("b"));
            m_pushbutton_line3_7->setText(tr("n"));
            m_pushbutton_line3_8->setText(tr("m"));
        }break;
        case kEnumUppercase://大写
        {
            m_pushbutton_1->setText(tr("Q"));
            m_pushbutton_2->setText(tr("W"));
            m_pushbutton_3->setText(tr("E"));
            m_pushbutton_4->setText(tr("R"));
            m_pushbutton_5->setText(tr("T"));
            m_pushbutton_6->setText(tr("Y"));
            m_pushbutton_7->setText(tr("U"));
            m_pushbutton_8->setText(tr("I"));
            m_pushbutton_9->setText(tr("O"));
            m_pushbutton_0->setText(tr("P"));
        //第二行
            m_pushbutton_line2_1->setText(tr("A"));
            m_pushbutton_line2_2->setText(tr("S"));
            m_pushbutton_line2_3->setText(tr("D"));
            m_pushbutton_line2_4->setText(tr("F"));
            m_pushbutton_line2_5->setText(tr("G"));
            m_pushbutton_line2_6->setText(tr("H"));
            m_pushbutton_line2_7->setText(tr("J"));
            m_pushbutton_line2_8->setText(tr("K"));
            m_pushbutton_line2_9->setText(tr("L"));
        //第三行
            m_pushbutton_line3_2->setText(tr("Z"));
            m_pushbutton_line3_3->setText(tr("X"));
            m_pushbutton_line3_4->setText(tr("C"));
            m_pushbutton_line3_5->setText(tr("V"));
            m_pushbutton_line3_6->setText(tr("B"));
            m_pushbutton_line3_7->setText(tr("N"));
            m_pushbutton_line3_8->setText(tr("M"));
        }break;
    }
}


//鼠标按下事件
void MyInputPanel::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << "SyszuxPinyin::mousePressEvent";
    if (event->button() == Qt::LeftButton)
    {
        m_ptPress = event->globalPos();
        m_bLeftButtonPress = true;
        //qDebug() << "SyszuxPinyin::mousePressEvent m_bLeftButtonPress = true;";
    }else
    {
        m_bLeftButtonPress = false;
       // qDebug() << "SyszuxPinyin::mousePressEvent m_bLeftButtonPress = false;";
    }
    event->ignore();
}

//鼠标移动事件
void MyInputPanel::mouseMoveEvent(QMouseEvent *event)
{
   // qDebug() << "SyszuxPinyin::mouseMoveEvent " << m_bLeftButtonPress;
    if(m_bLeftButtonPress)
    {
       // qDebug() << "SyszuxPinyin::mouseMoveEvent m_bLeftButtonPress = true;";
        m_ptMove = event->globalPos();
        //移动主窗口
        QPoint point=m_ptMove-m_ptPress;
       // qDebug()  << QString().sprintf("this->pos(%d,%d) m_ptMove(%d,%d) m_ptPress(%d,%d) point(%d,%d)",
       // this->pos().x(),this->pos().y(),m_ptMove.x(),m_ptMove.y(),m_ptPress.x(),m_ptPress.y(),point.x(),point.y());
       // this->move(this->pos()+point);
        //重新设置m_ptPress;
        m_ptPress = m_ptMove;
    }
    event->ignore();
}
//鼠标释放事件
void MyInputPanel::mouseReleaseEvent(QMouseEvent *event)
{
   // qDebug() << "SyszuxPinyin::mouseReleaseEvent";
    if (event->button() == Qt::LeftButton)
    {
        m_bLeftButtonPress = false;
    }
    event->ignore();
}

void MyInputPanel::init_new_button(QPushButton *_button,int _x,int _y,int _w,int _h,QString _typeSheet)
{
    _button->setFixedSize(_w,_h);
    _button->move(_x,_y);
    if(!_typeSheet.isEmpty())
        _button->setStyleSheet(_typeSheet);

    //_button->setFocusPolicy(Qt::NoFocus);
    //_button->setFlat(true);
}





void MyInputPanel::set_model_modal(bool _flag)
{
    if(_flag)
        setAttribute(Qt::WA_ShowModal,true);
    else
        setAttribute(Qt::WA_ShowModal,false);
}








