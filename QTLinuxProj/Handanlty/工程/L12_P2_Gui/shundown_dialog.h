#ifndef SHUNDOWN_DIALOG_H
#define SHUNDOWN_DIALOG_H

#include <QDialog>
#include <QLabel>

#include "class/common/define_screen.h"

class CShundownDialog : public QDialog
{
public:
    explicit CShundownDialog(QWidget *parent = 0);


private:
    void resizeEvent(QResizeEvent *e);
    QLabel *m_label_show;

    CMyDefineScreen *m_define_screen;

};

#endif // SHUNDOWN_DIALOG_H
