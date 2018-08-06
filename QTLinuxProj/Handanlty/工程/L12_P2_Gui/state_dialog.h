#ifndef STATE_DIALOG_H
#define STATE_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QScrollArea>
#include "class/common/scrollcontroller.h"

#include "common_data.h"


class CStateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CStateDialog(QWidget *parent = 0);
    
signals:
      void statePreButtonClicked();
      void stateFinishButtonClicked();

public slots:

    void slot_stateFinishButtonClicked();
    void slot_network_status(int type, const CRunStatusInfo &_runStatusInfo);
    void slot_tmp_status(int type, int state, int errorType);
    void slot_g3_network_status(int type, int state);

private:
      QLabel *m_StateLable;
      QPushButton *m_StatePreButton;
      QPushButton *m_StateFinishButton;
      QTextEdit *m_StateLine;
      bool m_bNetEquitState;
      QWidget *m_widget_bg;
      ScrollController *m_ScrollController;
      QScrollArea *m_scrollarea_1;
    
};

#endif // STATE_DIALOG_H
