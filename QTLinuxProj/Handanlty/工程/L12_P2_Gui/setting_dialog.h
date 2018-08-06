#ifndef SETTING_DIALOG_H
#define SETTING_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

#include "common_data.h"

class CSettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CSettingDialog(QWidget *parent = 0);
    
signals:
   void settingButtonClicked();


public slots:
   void slot_settingButtonClicked();

private:

   QLabel *m_SettingLable;
   QLabel *m_CityLable;
   QLabel *m_CompLable;
   QLabel *m_LineLable;
   QLabel *m_CarNumLable;
   QPushButton *m_SettingButton;
   QLineEdit *m_CityLine;
   QLineEdit *m_CompLine;
   QLineEdit *m_LineLine;
   QLineEdit * m_CarLIne;
   QWidget *m_widget_bg;

   QLabel *m_SNLable;
   QLabel *m_SNNumLable;

};

#endif // SETTING_DIALOG_H
