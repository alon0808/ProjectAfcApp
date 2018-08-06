#ifndef BASEMODELDATA_H
#define BASEMODELDATA_H


#include "common_data.h"

class BaseModelData
{
public:
    BaseModelData();
    ~BaseModelData();

public:
    bool isPressed;
    bool ischecked;
    bool isMouseOver;
    int m_row;
    int m_column;
    eViewTableFocus curFocus;
    eViewTableMouseEvent curMouseEvent;
    eViewTableState curState;
    QPoint m_mouseRightClickPs;

private:

};
#endif // BASEMODELDATA_H
