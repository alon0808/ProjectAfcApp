#include "basemodeldata.h"


BaseModelData::BaseModelData()
{
    isPressed	= false;
    ischecked	= false;
    isMouseOver = false;
    m_row = -1;
    m_column = -1;
    curFocus		= evtfNone;
    curMouseEvent	= evtmeNone;
    curState		= evtsStop;
    m_mouseRightClickPs = QPoint();
}

BaseModelData::~BaseModelData()
{

}
