#ifndef DECLARATION_DIALOG_H
#define DECLARATION_DIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include "class/common/scrollcontroller.h"

class CDeclarationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CDeclarationDialog(QWidget *parent = 0);
    
signals:
      void  decButtonClicked();
    
public slots:


private:
    QLabel *m_DecLable;
    QTextEdit *m_DecText;
    QPushButton *m_DecButton;
    QLabel *m_ForDecButton;
    QWidget *m_widget_bg;  

     ScrollController *m_ScrollController;
     QScrollArea *m_scrollarea_1;
};

#endif // DECLARATION_DIALOG_H
