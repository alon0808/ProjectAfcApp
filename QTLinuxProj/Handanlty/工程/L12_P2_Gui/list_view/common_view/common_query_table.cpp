#include "common_query_table.h"


#include <QtGui>
#include <QDebug>

#include "common_query_list.h"

#include <QPalette>

#include "class/common/define_screen.h"


CCommonQueryTable::CCommonQueryTable(QObject * parent) : QStyledItemDelegate(parent)
{
    //m_tableview = (MyTableView*)parent;
    m_common_List = (CCommonQueryList *)parent;
    m_tableview = m_common_List->pFileManageTable();
}

void CCommonQueryTable::paint(QPainter * painter, const QStyleOptionViewItem & opt, const QModelIndex & index ) const
{
    painter->save();

    CCommonQueryMode data_type = index.data().value<CCommonQueryMode>();
    int colindex = index.column();
    bool ischeck = data_type.ischecked;

    CPresetEvent *pPresetEvent = NULL;
    CImportantMessage *pImportantMessage = NULL;
    CVehiclePlan *pVehiclePlan = NULL;
    CCallBook *pCallBook = NULL;
    CSimulation *pSimulation = NULL;
    CNetworkStatus *pNetworkStatus = NULL;
    CDateSelect *pDateSelect = NULL;
    CQueryResult *pQueryResult = NULL;
    CSoftwareUpgrade *pSoftwareUpgrade = NULL;
    CWifiSearch *pWifiSearch = NULL;

    if(CCommonQueryList::kEnumPresetEventList == m_common_List->GetListType())
    {
        pPresetEvent =(CPresetEvent *)data_type.data;
    }
    else if(CCommonQueryList::kEnumImportantMessageList == m_common_List->GetListType()
            || CCommonQueryList::kEnumNormalMessageList == m_common_List->GetListType())
    {
        //pPresetEvent =(CPresetEvent *)data_type.data;
        pImportantMessage = (CImportantMessage *)data_type.data;
    }
    else if(CCommonQueryList::kEnumVehiclePlanList == m_common_List->GetListType())
    {
        pVehiclePlan =(CVehiclePlan *)data_type.data;
    }
    else if(CCommonQueryList::kEnumCallList == m_common_List->GetListType())
    {
        pCallBook = (CCallBook *)data_type.data;
    }
    else if(CCommonQueryList::kEnumSimulation == m_common_List->GetListType())
    {
        pSimulation = (CSimulation *)data_type.data;
    }
    else if(CCommonQueryList::kEnumNetworkStatus == m_common_List->GetListType())
    {
        pNetworkStatus = (CNetworkStatus *)data_type.data;
    }
    else if(CCommonQueryList::kEnumDateSelect == m_common_List->GetListType())
    {
        pDateSelect = (CDateSelect *)data_type.data;
    }
    else if(CCommonQueryList::kEnumQueryResult == m_common_List->GetListType())
    {
        pQueryResult = (CQueryResult *)data_type.data;
    }
    else if(CCommonQueryList::kEnumUpgrade == m_common_List->GetListType())
    {
        pSoftwareUpgrade = (CSoftwareUpgrade *)data_type.data;
    }
    else if(CCommonQueryList::kEnumWifiSearch == m_common_List->GetListType())
    {
        pWifiSearch = (CWifiSearch *)data_type.data;
    }

    QColor font_color;
    if(data_type.ischecked)
        font_color = "#03111f";
    else
        font_color = "#94C6FF";

    if(CCommonQueryList::kEnumDateSelect != m_common_List->GetListType())
    {
        CommonDelegateControl::drawBackground(painter, opt, index, data_type.isMouseOver, data_type.ischecked, false);
        painter->setPen(QColor("#24508F"));
        painter->drawLine(opt.rect.x(),opt.rect.y() + opt.rect.height() - 1,opt.rect.x() + opt.rect.width(),opt.rect.y() + opt.rect.height() - 1);
    }


    QString show_content = tr("");

    if(CCommonQueryList::kEnumDateSelect == m_common_List->GetListType())
    {
        if(pDateSelect->m_list.size() - 1 >= colindex)
        {
            if(pDateSelect->m_list.at(colindex).m_current_month)
            {


                if(pDateSelect->m_list.at(colindex).m_normal)
                {
                    font_color = "#0f1d31";
                    painter->fillRect(opt.rect, "#80e3ff");
                }
                else
                {
                    font_color = "#94c6ff";
                    painter->fillRect(opt.rect, ALTERNATING_ROW_COLOR_WHITE);
                }

                if(pDateSelect->m_list.at(colindex).m_police)
                {
                    QRect rect = opt.rect;
                    rect.setX(opt.rect.x());
                    rect.setY(opt.rect.y() + opt.rect.height() - 8);
                    rect.setWidth(opt.rect.width());
                    rect.setHeight(8);
                    painter->fillRect(rect, "#ff2e2e");
                }
                bool is_select = false;
#if 1
                if(pDateSelect->m_list.at(colindex).m_normal || pDateSelect->m_list.at(colindex).m_police)
                    is_select = true;
#else
                if(pDateSelect->m_list.at(colindex).m_current_month)
                    is_select = true;
#endif
                if(is_select && index.row() == data_type.m_row && index.column() == data_type.m_column)
                {
                    QRect rect1 = opt.rect;
                    QRect rect2 = opt.rect;
                    QRect rect3 = opt.rect;
                    QRect rect4 = opt.rect;

                    rect1.setHeight(4);
                    rect2.setWidth(4);
                    rect3.setX(opt.rect.x() + opt.rect.width() - 4);
                    rect4.setY(opt.rect.y() + opt.rect.height() - 4);
                    painter->fillRect(rect1, "#ffa200");
                    painter->fillRect(rect2, "#ffa200");
                    painter->fillRect(rect3, "#ffa200");
                    painter->fillRect(rect4, "#ffa200");
                }


            }
            else
            {
                font_color = "#4f6688";
                painter->fillRect(opt.rect, ALTERNATING_ROW_COLOR_WHITE);
            }
        }

    }
    int font_size = 0;

    if(CCommonQueryList::kEnumDateSelect == m_common_List->GetListType())
        font_size = FONT_SIZE_16;
    else if(CCommonQueryList::kEnumQueryResult == m_common_List->GetListType())
        font_size = 18;
    else
        font_size = CMyDefineScreen::getInstance()->get_change_factor_y(26);


    if(colindex == 0) //第一列
    {
        Qt::Alignment align_mode = Qt::AlignCenter;
        QRect rect = opt.rect;
        if(pPresetEvent)
            show_content = QString::number(index.row() + 1/*pPresetEvent->m_number*/);
        else if(pImportantMessage)
            show_content = QString::number(index.row() + 1/*pImportantMessage->m_number*/);
        else if(pVehiclePlan)
            show_content = QString::number(index.row() + 1/*pVehiclePlan->m_number*/);
        else if(pCallBook)
            show_content = pCallBook->m_name;
        else if(pSimulation)
            show_content = QString::number(pSimulation->m_number);
        else if(pNetworkStatus)
            show_content = QString::number(pNetworkStatus->m_number);
        else if(pDateSelect)
        {
            if(pDateSelect->m_list.size() - 1 >= colindex)
            {
                show_content = QString::number(pDateSelect->m_list.at(colindex).m_number);
            }
        }
        else if(pSoftwareUpgrade)
            show_content = QString::number(index.row() + 1/*pSoftwareUpgrade->m_number*/);
        else if(pWifiSearch)
        {
            align_mode = Qt::AlignVCenter | Qt::AlignLeft;
            show_content = pWifiSearch->m_name;
            rect.setX(rect.x() + 30);
        }

        if(pQueryResult)
        {
            QRect CheckBoxRect = CommonDelegateControl::createCheckBoxRect(opt, 20);
            CommonDelegateControl::drawCheckBox(painter, CheckBoxRect, ischeck, ":/img/res/main_page/menu/system_manage/selected.png", ":/img/res/main_page/menu/system_manage/marquee.png", Qt::AlignLeft);
        }
        else
        {
            CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), show_content, FONT_FAMILY, rect, font_size, font_color, true, align_mode);
        }
    }
    else if(colindex==1)//第二列
    {
        Qt::Alignment align_mode = Qt::AlignCenter;
        QRect rect = opt.rect;
        if(pPresetEvent)
        {
            align_mode = Qt::AlignLeft | Qt::AlignVCenter;
            show_content = pPresetEvent->m_content;
            button_paint(painter,data_type,opt,index,kEnumMessagePre);
        }
        else if(pImportantMessage)
        {
            align_mode = Qt::AlignLeft | Qt::AlignVCenter;

            if(!pImportantMessage->m_calculate)
                m_common_List->HandleData(pImportantMessage);

            show_content = /*pImportantMessage->m_recv_time + " " +*/ pImportantMessage->m_show_content;
            if(pImportantMessage->m_more)
                button_paint(painter,data_type,opt,index);
        }
        else if(pVehiclePlan)
        {
            show_content = pVehiclePlan->m_recv_time;
            align_mode = Qt::AlignLeft | Qt::AlignVCenter;
        }
        else if(pCallBook)
            show_content = pCallBook->m_number;
         else if(pSimulation)
            show_content = pSimulation->m_passageway;
        else if(pNetworkStatus)
            show_content = pNetworkStatus->m_passageway;
        else if(pDateSelect)
        {
            if(pDateSelect->m_list.size() - 1 >= colindex)
            {
                show_content = QString::number(pDateSelect->m_list.at(colindex).m_number);
            }
        }
        else if(pQueryResult)
            show_content = pQueryResult->m_date;
        else if(pSoftwareUpgrade)
        {
            show_content = pSoftwareUpgrade->m_file_name;
            align_mode = Qt::AlignLeft | Qt::AlignVCenter;
        }
        else if(pWifiSearch)
        {
            show_content = pWifiSearch->m_signal;
            QString img_path = ":/img/res/main_page/menu/system_manage/network/icon_web_keyt.png";
            bool bRet = false;
            align_mode = Qt::AlignLeft | Qt::AlignVCenter;
            if(pWifiSearch->m_lock)
            {
                CommonDelegateControl::drawItemPixmap(painter, opt, bRet, 5 + 13, img_path, "", 21, 31, Qt::AlignLeft);
                rect.setX(rect.x() + 5 + 34 + 10);
            }
            else
            {
                img_path = ":/img/res/main_page/menu/system_manage/network/icon_web_connected.png";
                CommonDelegateControl::drawItemPixmap(painter, opt, bRet, 5, img_path, "", 34, 34, Qt::AlignLeft);
                rect.setX(rect.x() + 5 + 34 + 10);
            }

        }
        CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), show_content, FONT_FAMILY, rect, font_size, font_color, true, align_mode);
        if(pVehiclePlan)
        {
            rect.setX(opt.rect.x() + opt.rect.width() - 200);
            show_content = pVehiclePlan->m_attribute;
            font_color = "#398BDD";
            CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), show_content, FONT_FAMILY, rect, font_size, font_color, true, align_mode);
        }
    }
    else if(colindex==2)//第三列
    {
        if(pPresetEvent)
            show_content = QString::number(pPresetEvent->m_event_code);
        else if(pVehiclePlan)
            show_content = pVehiclePlan->m_start_time;
        else if(pSimulation)
            show_content = pSimulation->m_av1;
        else if(pNetworkStatus)
            show_content = pNetworkStatus->m_ip;
        else if(pDateSelect)
        {
            if(pDateSelect->m_list.size() - 1 >= colindex)
            {
                show_content = QString::number(pDateSelect->m_list.at(colindex).m_number);
            }
        }
        else if(pQueryResult)
            show_content = QString::number(pQueryResult->m_time);

        CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), show_content, FONT_FAMILY, opt.rect, font_size, font_color, true, Qt::AlignCenter);
    }
    else if(colindex==3)//第四列
    {
        Qt::Alignment align = Qt::AlignCenter;
        QRect rect = opt.rect;
        if(pPresetEvent)
            show_content = pPresetEvent->m_shortcut_key;
        else if(pVehiclePlan)
            show_content = pVehiclePlan->m_attribute;
        else if(pSimulation)
            show_content = pSimulation->m_av2;
        else if(pNetworkStatus)
            show_content = pNetworkStatus->m_mac;
        else if(pDateSelect)
        {
            if(pDateSelect->m_list.size() - 1 >= colindex)
            {
                show_content = QString::number(pDateSelect->m_list.at(colindex).m_number);
            }
        }
        else if(pQueryResult)
        {
            show_content = pQueryResult->m_passageway;
            align = Qt::AlignLeft | Qt::AlignVCenter;
            rect.setX(rect.x() + 10);
        }

        CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), show_content, FONT_FAMILY, rect, font_size, font_color, true, align);
    }

    else if(colindex==4)//第五列
    {
        if(pSimulation)
            show_content = pSimulation->m_attribute;
        else if(pNetworkStatus)
            show_content = pNetworkStatus->m_attribute;
        else if(pDateSelect)
        {
            if(pDateSelect->m_list.size() - 1 >= colindex)
            {
                show_content = QString::number(pDateSelect->m_list.at(colindex).m_number);
            }
        }

        CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), show_content, FONT_FAMILY, opt.rect, font_size, font_color, true, Qt::AlignCenter);
    }
    else if(colindex==5)//第六列
    {
        if(pDateSelect)
        {
            if(pDateSelect->m_list.size() - 1 >= colindex)
            {
                show_content = QString::number(pDateSelect->m_list.at(colindex).m_number);
            }
        }
        CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), show_content, FONT_FAMILY, opt.rect, font_size, font_color, true, Qt::AlignCenter);
    }
    else if(colindex==6)//第七列
    {
        if(pDateSelect)
        {
            if(pDateSelect->m_list.size() - 1 >= colindex)
            {
                show_content = QString::number(pDateSelect->m_list.at(colindex).m_number);
            }
        }
        CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), show_content, FONT_FAMILY, opt.rect, font_size, font_color, true, Qt::AlignCenter);
    }


    painter->restore();

}

void CCommonQueryTable::button_paint(QPainter * painter, CCommonQueryMode viewtablemodel,const QStyleOptionViewItem & opt, const QModelIndex & index,char _type) const
{

    QRect rect = QRect(opt.rect.x() + 400 + 30,opt.rect.y() + 1l,100,opt.rect.height() - 2);

    CCommonQueryMode data = index.data().value<CCommonQueryMode>();
    //if(data.m_UpdateData)
      //  data.m_UpdateData->_updateBtnRect = QRect(m_tableview->mapToGlobal(UpdateRect.topLeft()), QSize(UpdateRect.width(), UpdateRect.height()));


    QFont font;
    font.setPixelSize(26);
    painter->setFont(font);

    QColor MoreFillColor;
    QColor PenColor = QColor("#94C5FF");
    MoreFillColor = QColor("#1D304E");

    QStyleOptionViewItem status =opt;
    status.displayAlignment =Qt::AlignLeft;


    switch(viewtablemodel.curFocus)
    {
        case evtfMore:
        case evtfSend:
        {
            switch(viewtablemodel.curMouseEvent)
            {
            case evtmeNone:
            {
                PenColor = QColor("#94C5FF");
                MoreFillColor = QColor("#1D304E");
            }break;
            case evtmeUp:
            {
                PenColor = QColor("#94C5FF");
                MoreFillColor = QColor("#1D304E");
            }break;
            case evtmePress:
            {    PenColor = QColor("#04121F");
                 MoreFillColor = QColor("#3EA3FF");
            }break;
            case evtmeMove:
                break;
            }
            if (opt.state & QStyle::State_MouseOver)	//ok	//鼠标划过
            {
                PenColor = QColor("#04121F");
                MoreFillColor = QColor("#3EA3FF");
            }



        }break;

    }
    painter->setPen(PenColor);
    painter->fillRect(rect, MoreFillColor);
    QString strBtnText = "";
    if(kEnumMessageMore == _type)
        strBtnText = tr("更多");
    else
        strBtnText = tr("发送");
    int iBtnWidth = QFontMetrics(font).width(strBtnText);
    int iBtnHeight = QFontMetrics(font).height();
    QRect UpdateTextRect =QRect(opt.rect.x() + 250 + (100 - iBtnWidth)/2,opt.rect.y() + (opt.rect.height() - iBtnHeight)/2,iBtnWidth,iBtnHeight);
    UpdateTextRect.moveCenter(rect.center());
    QApplication::style()->drawItemText(painter,UpdateTextRect,0,QApplication::palette(),true, strBtnText);


}

bool CCommonQueryTable::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,const QModelIndex &index)
{
    if (NULL == event || NULL == model) return false;

    QMouseEvent *mouse_event = static_cast<QMouseEvent*>(event);
    QRect btnOnekeyRect = QRect(option.rect.x(),option.rect.y()-12+option.rect.height()/2,69,24);
    QRect btnDelRect = QRect(option.rect.x()+92,option.rect.y()-8+option.rect.height()/2,16,16);

    CCommonQueryMode data = index.data().value<CCommonQueryMode>();
    bool bStartTimer = false;
    bool isUpdateCheck = false;

    CImportantMessage *pImportantMessage = NULL;
    char list_type = m_common_List->GetListType();
    if(CCommonQueryList::kEnumNormalMessageList == list_type)
    {
        pImportantMessage = (CImportantMessage *)data.data;
    }

    // 按键事件
    QPoint mouseRightClickPs;
    eViewTableMouseEvent viewmouseevent = evtmeNone;
    int viewmouseeventTmp = evtmeNone;
    eViewTableFocus viewfocus = evtfNone;
    bool isOver = false;

    CommonDelegateControl::editorEvent1(event, model, option, index, data, isUpdateCheck, viewmouseeventTmp, bStartTimer, true, mouseRightClickPs, true,list_type);

    //CommonDelegateControl::editorEvent2(event, model, option, index, data, isUpdateCheck, viewmouseeventTmp, bStartTimer, true, 10,40);
    if(CCommonQueryList::kEnumNormalMessageList == m_common_List->GetListType()
       && 1 == index.column()
       && pImportantMessage
       && pImportantMessage->m_more)
     {
            QRect button_more = QRect(option.rect.x() + 400 + 30,option.rect.y(),100,option.rect.height());
            if (button_more.contains(mouse_event->pos()))
            {
                viewfocus = evtfMore;
            }
     }
     else  if(CCommonQueryList::kEnumPresetEventList == m_common_List->GetListType() && 1 == index.column())
     {
         QRect button_send = QRect(option.rect.x() + 400 + 30,option.rect.y(),100,option.rect.height());
            if (button_send.contains(mouse_event->pos()))
            {
                viewfocus = evtfSend;
            }
     }
     else
     {
        int clicked_width = 1;
        int clicked_height = 1;
        if(CCommonQueryList::kEnumQueryResult == list_type)
        {
            clicked_width = option.rect.width();
            clicked_height = option.rect.height();
        }

        CommonDelegateControl::editorEvent2(event, model, option, index, data, isUpdateCheck, viewmouseeventTmp, bStartTimer, true, 0,clicked_width,clicked_height);
     }

    viewmouseevent = static_cast<eViewTableMouseEvent>(viewmouseeventTmp);
    if (viewmouseevent == evtmeMove || data.isPressed)
    {
        isOver = true;
    }
    if(data.ischecked)
    {
        isOver = false;
    }

    if(evtmeUp == viewmouseevent)
    {
         if(CCommonQueryList::kEnumPresetEventList == list_type ||
            CCommonQueryList::kEnumNormalMessageList == list_type ||
            CCommonQueryList::kEnumVehiclePlanList == list_type ||
            CCommonQueryList::kEnumUpgrade == list_type)
            ((CommonTableModel*)model)->setOneRowSelect(index.row(),true);
    }




    m_tableview->setMouseOver(isOver, index);

    return ((CCommonQueryData*)model)->setData(index, data.ischecked, data.isPressed, viewfocus, viewmouseevent,bStartTimer, isUpdateCheck, mouseRightClickPs);
}
