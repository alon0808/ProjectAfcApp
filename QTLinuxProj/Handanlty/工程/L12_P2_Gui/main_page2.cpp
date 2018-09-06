#include "Macro_Proj.h"
#include "main_page2.h"

#include "LtyCommonDefine.h"
#include "class/common_interface.h"
#include "class/common/my_messagebox1.h"
#include <QDateTime>
#include <QPainter>
#include "dynamic_text.h"
#include <QDebug>
#include "class/ltystring.h"
#include "libHandanCore.h"
#include "UtilityProc.h"

#include <string.h>


#define STATION_NAME_SHOW_FONT_SIZE     30
#define STATION_NAME_SHOW_LENGTH        160

#define MESSAGE_FONT_SIZE       16
#define MESSAGE_SHOW_LENGTH     395

static CMainPage2 *gMainPageThis = NULL;

CMainPage2 *CMainPage2::getInstance()
{
	return gMainPageThis;
}


showImage::showImage(QWidget *parent) :QWidget(parent)
{
	//CCommonData::getInstance()->m_CPlayPicture.m_image.load(":/img_p2/res/p2/left_right/guanggao.jpg");
}
showImage::~showImage()
{
}

void showImage::set_image(const QImage &_image)
{
	m_image = _image;
}

void showImage::paintEvent(QPaintEvent *e)
{
	QPainter pp(this);
	QRect Temp(0, 0, this->width(), this->height());
	pp.drawImage(Temp, m_image);
}

CMainPage2::CMainPage2(QWidget *parent) :
	QDialog(parent)
{
	gMainPageThis = this;
	this->setFixedSize(640, 480);
	this->setObjectName("CMainPage2");
	this->setStyleSheet("QDialog#CMainPage2{border:0px solid #00ff00;background-color:#000000;}");
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);
	//kEnumUpDown

	m_is_demo = CCommonInterface::p2_is_demo_mainpage();
	//m_is_demo = true;
	set_type(kEnumLeftRight);

	m_label_test_picture = new QLabel(this);
	m_label_test_picture->move(0, 0);
	m_label_test_picture->setFixedSize(640, 479);
	m_label_test_picture->hide();


	m_pushbutton_test = new QPushButton(this);
	m_pushbutton_test->setFixedSize(40, 25);
	m_pushbutton_test->move(164 + 150, (40 - 25) / 2);
	m_pushbutton_test->setFocusPolicy(Qt::NoFocus);
	m_pushbutton_test->setText("测试");
	m_pushbutton_test->setStyleSheet("QPushButton{font:15px;}");
	m_pushbutton_test->hide();

	connect(m_pushbutton_test, SIGNAL(clicked()), SLOT(slot_pushbutton_test()));
	req_tbp_status();
	req_tmp_status();
	req_sd_status();
	req_station_and_line();

}


void CMainPage2::req_station_and_line()
{
	CProtocol::getInstance()->call_json_station_name_set(CProtocol::kEnumGet);
}
void CMainPage2::req_sd_status()
{
	int event = -1;
	// 值的定义参考CCommonData::getInstance()->m_hdd_status获取处
	if (1 == CCommonData::getInstance()->m_hdd_status)
	{
		event = kEnumSdNormalNormal;
	}
	else if (3 == CCommonData::getInstance()->m_hdd_status)
	{
		event = kEnumSdAbnormal1;
	}
	else
	{
		event = kEnumError;
	}
	show_sd_status(event);
}
void CMainPage2::req_tbp_status()
{
	// 为了解决导航画面进来看不到右上角平台状态问题
	int tbpState = -1;
	if (2 == CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_status)
	{
		tbpState = kEnumPlatformConnect;
	}
	else if (3 == CCommonData::getInstance()->m_runStatusInfo.m_CRunStatusMobileNetwork->center_server2_status)
	{
		tbpState = kEnumPlatformNormal;
	}
	else
	{
		tbpState = kEnumPlatformAbnormal1;
	}
	qDebug() << "req_tbp_status tbpState" << tbpState;
	slot_head_widget_notify(kEnumMainPagePlatformStatus, tbpState, 0);
}


void CMainPage2::req_tmp_status()
{

	int tmpState = -1;
	tmpState = CCommonData::getInstance()->m_tmp_status;
	printf("CMainPage2::req_tmp_status()>>>>>>>>>tmpState:%d\n", tmpState);
	slot_head_widget_notify(kEnumMainPageTMPPlatformStatus, tmpState, 0);
}



void CMainPage2::set_type(char _type)
{
	m_type = _type;
	switch (_type)
	{
	case kEnumLeftRight: //界面左右
	{
		m_station_font_size = STATION_NAME_SHOW_FONT_SIZE;
		m_station_show_length = STATION_NAME_SHOW_LENGTH;

		m_timer_qr = new QTimer(this);
		m_timer_qr->setInterval(3000);
		connect(m_timer_qr, SIGNAL(timeout()), this, SLOT(slot_qr_status_timer()));

		main_page_left_right();
	}break;
	case kEnumUpDown:     //界面上下
	{
		m_station_font_size = 24;
		m_station_show_length = 180;
		main_page_up_down();
	}break;
	}

}
#include <QDesktopWidget>
void CMainPage2::main_page_left_right()
{
	//this->setStyleSheet("QDialog#CMainPage2{border-image:url(:/img_p2/res/p2/left_right/bg.png);}");
	if (m_is_demo)
		main_page_demo();
	else
	{
		left_right_init1();
		qint64 current_time = QDateTime::currentDateTime().toMSecsSinceEpoch();
		QDateTime resume_date = QDateTime::fromString("2017-09-01", "yyyy-MM-dd");
		qint64 resume_end = resume_date.toMSecsSinceEpoch();
#ifdef CURRENT_ARM_RUN
#if 0
		if (current_time >= resume_end)
			debug_interface(false);
		else
			debug_interface(true);
#endif
		debug_interface(false);
#else
		debug_interface(false);
#endif
	}

}
void CMainPage2::slot_button_menu_test()
{
	CProtocol::getInstance()->call_json_system_passenger_flow(7, 1, 0);
	qDebug() << "slot_button_menu_test";
}


void CMainPage2::main_page_demo()
{

	m_label_network = NULL;            //2G 3G 4G
	m_pushbutton_signal = NULL;   //4G 信号
	m_label_gps_number = NULL; //GPS个数
	m_pushbutton_gps = NULL;//GPS
	m_pushbutton_platform_status = NULL;//平台连接状态 (调度平台)
	m_pushbutton_sd_status = NULL;
	m_label_time = NULL; //时间显示
	m_label_time1 = NULL;
	m_timer = NULL;
	m_widget_show_picture1 = NULL;
	m_widget_show_voice = NULL;
	m_label_scroll_text = NULL;
	m_widget_qr_status = NULL;
	m_label_line_name = NULL;
	m_label_station1 = NULL;
	m_label_station2 = NULL;
	m_dynamic_text_station_name = NULL;
	m_dynamic_text_message = NULL;

	QWidget *widget_head_line = new QWidget(this);
	widget_head_line->move(0, 50);
	widget_head_line->setFixedSize(640, 1);
	widget_head_line->setStyleSheet("QWidget{border:0px solid #00ff00;background-color:#ffffff;}");

	QLabel *label_title = new QLabel(this);
	QString str_show = tr("欢迎使用公交一卡通系统");
	QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:35px;color:#ffffff;}";
	CCommonInterface::init_label_text(label_title, 0, 85, this->width(), 38, str_show, str_sheet, Qt::AlignCenter, 35);

	m_widget_qr_status = new QWidget(this);
	m_widget_qr_status->setFixedSize(80, 80);
	m_widget_qr_status->move(440 + (200 - 80) / 2, 84 - 8);

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:18px;color:#ffffff;}";
	m_label_qr_text = new QLabel(this);
	CCommonInterface::init_label_text(m_label_qr_text, 440 - 440 / 2, 155, 200, 20, str_show, str_sheet, Qt::AlignCenter, 18);


	QString strCurrentTime = tr("2017-05-30 55:55:55");//QDateTime::currentDateTime().toString("hh:mm:ss");
	int str_length = CCommonInterface::get_string_length(strCurrentTime, 16);

	m_label_time = new QLabel(this);
	m_label_time->setFixedSize(200, 36);
	m_label_time->move(440 - 440 / 2, 95);
	m_label_time->setStyleSheet("QLabel{border:0px;background:transparent;font:36px;color:#ffffff; }");
	m_label_time->setAlignment(Qt::AlignCenter);
	m_label_time->hide();

	m_label_time1 = new QLabel(this);
	m_label_time1->setFixedSize(260, 50);
	m_label_time1->move((this->width() - 260) / 2, 0);
	m_label_time1->setStyleSheet("QLabel{border:0px;background:transparent;font:18px;color:#ffffff; }");
	m_label_time1->setAlignment(Qt::AlignCenter);


	//时间定时器
	m_timer = new QTimer(this);
	m_timer->setInterval(1000);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_1s_timer()));
	m_timer->start();

	m_pushbutton_menu = new QPushButton(this);
	str_sheet = "QPushButton{border:0px solid #ffffff;background:transparent;font:30px;color:#ffffff;}";
	CCommonInterface::init_new_button(m_pushbutton_menu, (this->width() - 260) / 2, 0, 260, 50, str_sheet);
	//bool ret = connect(m_pushbutton_menu,SIGNAL(clicked()),this,SLOT(slot_button_menu_test()));
	//qDebug() << ret;
	connect(m_pushbutton_menu, SIGNAL(clicked()), this, SIGNAL(signal_button_menu()));
	//slot_button_menu_test

	//m_widget_show_picture1->hide();


	//m_widget_bg->hide();

#ifndef CURRENT_ARM_RUN
	m_pushbutton_exit = new QPushButton(this);
	str_sheet = "";
	// CCommonInterface ::init_new_button(m_pushbutton_exit,164 + str_length + 30,0,40,25,str_sheet);
	m_pushbutton_exit->setFixedSize(40, 25);
	m_pushbutton_exit->move(164 + str_length + 30 - 200, (40 - 25) / 2);
	m_pushbutton_exit->setText("退出");
	connect(m_pushbutton_exit, SIGNAL(clicked()), this, SIGNAL(signal_button_exit()));
	m_pushbutton_exit->setStyleSheet("QPushButton{font:15px;}");
#endif


	m_label_scroll_text = new QLabel(this);
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:16px;color:#72a4ff;}";
	str_show = "";
	CCommonInterface::init_label_text(m_label_scroll_text, 36, this->height() - 45 + 9, MESSAGE_SHOW_LENGTH, 20, str_show, str_sheet, Qt::AlignLeft, 16);



	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:24px;color:#ffffff;}";
	m_label_line_name = new QLabel(this);
	CCommonInterface::init_label_text(m_label_line_name, 30, 195, 80, 30, str_show, str_sheet, Qt::AlignLeft, 24);

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:18px;color:#ffffff;}";
	m_label_direction = new QLabel(this);
	CCommonInterface::init_label_text(m_label_direction, 30, 195 + 30, 80, 25, str_show, str_sheet, Qt::AlignLeft, 18);



	str_show = tr("本站:");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:16px;color:#ffffff;}";
	m_label_station1 = new QLabel(this);
	CCommonInterface::init_label_text(m_label_station1, 440 - 440 / 2, 359, 100, 20, str_show, str_sheet, Qt::AlignLeft, 16);
	m_label_station1->hide();

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:55px;color:#ffffff;}";
	m_label_station2 = new QLabel(this);
	CCommonInterface::init_label_text(m_label_station2, (this->width() - 400) / 2, 370, 400, 60, str_show, str_sheet, Qt::AlignLeft, 55);

	m_dynamic_text_station_name = new CDynamicText(this);
	connect(m_dynamic_text_station_name, SIGNAL(signal_dynamic_text_event(CDynTextParam)), this, SLOT(slot_dynamic_text_event(CDynTextParam)));

	m_dynamic_text_station_name1 = new CDynamicText(this);
	connect(m_dynamic_text_station_name1, SIGNAL(signal_dynamic_text_event(CDynTextParam)), this, SLOT(slot_dynamic_text_event(CDynTextParam)));


	m_dynamic_text_message = new CDynamicText(this);
	connect(m_dynamic_text_message, SIGNAL(signal_dynamic_text_event(CDynTextParam)), this, SLOT(slot_dynamic_text_event(CDynTextParam)));


	slot_1s_timer();


	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:20px;color:#ffffff;}";
	m_label_charge_type = new QLabel(this);
	CCommonInterface::init_label_text(m_label_charge_type, this->width() - 100, 0, 100, 50, str_show, str_sheet, Qt::AlignCenter, 20);


	str_show = tr("票价");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:20px;color:#ffffff;}";
	m_label_ticket = new QLabel(this);
	CCommonInterface::init_label_text(m_label_ticket, 440 - 440 / 2, 307, 200, 20, str_show, str_sheet, Qt::AlignCenter, 20);

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:28px;color:#ffffff;}";
	m_label_ticket1 = new QLabel(this);
	CCommonInterface::init_label_text(m_label_ticket1, 440 - 440 / 2, 341 - 3, 200, 32, str_show, str_sheet, Qt::AlignCenter, 28);

	str_show = tr("余额");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:20px;color:#ffffff;}";
	m_label_remaining_sum = new QLabel(this);
	CCommonInterface::init_label_text(m_label_remaining_sum, 440 - 440 / 2, 394, 200, 22, str_show, str_sheet, Qt::AlignCenter, 20);

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:28px;color:#ffffff;}";
	m_label_remaining_sum1 = new QLabel(this);
	CCommonInterface::init_label_text(m_label_remaining_sum1, 440 - 440 / 2, 426, 200, 32, str_show, str_sheet, Qt::AlignCenter, 28);

	str_show = tr("到站");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:20px;color:#ffffff;}";
	m_label_arrive = new QLabel(this);
	CCommonInterface::init_label_text(m_label_arrive, 440 - 440 / 2, 394, 200, 22, str_show, str_sheet, Qt::AlignCenter, 20);

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #ffffff;background:transparent;font:28px;color:#ffffff;}";
	m_label_arrive1 = new QLabel(this);
	CCommonInterface::init_label_text(m_label_arrive1, 440 - 440 / 2, 426 - 3, 200, 32, str_show, str_sheet, Qt::AlignCenter, 28);


	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:28px;color:#33cc00;}";
	m_label_printf_test = new QLabel(this);
	CCommonInterface::init_label_text(m_label_printf_test, (this->width() - 375) / 2, 180, 375, 120, str_show, str_sheet, Qt::AlignCenter, 28);
	m_label_printf_test->hide();
}

void CMainPage2::main_page_up_down()
{
	m_label_network = NULL;            //2G 3G 4G
	m_pushbutton_signal = NULL;   //4G 信号
	m_label_gps_number = NULL; //GPS个数
	m_pushbutton_gps = NULL;//GPS
	m_pushbutton_platform_status = NULL;//平台连接状态 (调度平台)
	m_pushbutton_sd_status = NULL;
	m_label_time = NULL; //时间显示
	m_label_time1 = NULL;
	m_timer = NULL;
	m_widget_show_picture1 = NULL;
	m_widget_show_voice = NULL;
	m_label_scroll_text = NULL;
	m_widget_qr_status = NULL;
	m_label_line_name = NULL;
	m_label_station1 = NULL;
	m_label_station2 = NULL;
	m_dynamic_text_station_name = NULL;
	m_dynamic_text_message = NULL;

	m_widget_bottom = new QWidget(this);
	m_widget_bottom->setFixedSize(640, 126);
	m_widget_bottom->move(0, 354);
	m_widget_bottom->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/up_down/bg.png);}");

	QString str_show = tr("本站:");
	QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:16px;color:#8df6ff;}";
	m_label_station1 = new QLabel(this);
	CCommonInterface::init_label_text(m_label_station1, 20, 398, 100, 20, str_show, str_sheet, Qt::AlignLeft, 16);

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:24px;color:#8df6ff;}";
	m_label_station2 = new QLabel(this);
	CCommonInterface::init_label_text(m_label_station2, 20, 421, m_station_show_length, 28, str_show, str_sheet, Qt::AlignLeft, 24);

	m_dynamic_text_station_name = new CDynamicText(this);
	connect(m_dynamic_text_station_name, SIGNAL(signal_dynamic_text_event(CDynTextParam)), this, SLOT(slot_dynamic_text_event(CDynTextParam)));



	m_widget_show_picture1 = new showImage(this);
	m_widget_show_picture1->setFixedSize(640, 354);
	m_widget_show_picture1->move(0, 0);

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:30px;color:#ffffff;}";
	m_label_line_name = new QLabel(this);
	CCommonInterface::init_label_text(m_label_line_name, 253, 134, 300, 40, str_show, str_sheet, Qt::AlignCenter, 30);

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:16px;color:#7ebaff;}";
	m_label_direction = new QLabel(this);
	CCommonInterface::init_label_text(m_label_direction, 293, 440 - 5, 134, 20, str_show, str_sheet, Qt::AlignLeft, 16);

	QString strCurrentTime = tr("55:55:55");//QDateTime::currentDateTime().toString("hh:mm:ss");
	int str_length = CCommonInterface::get_string_length(strCurrentTime, 18);

	m_label_time = new QLabel(this);
	m_label_time->setFixedSize(140, 36);
	m_label_time->move(479, 98);
	m_label_time->setStyleSheet("QLabel{border:0px;background:transparent;font:34px;color:#ffffff; }");
	m_label_time->setAlignment(Qt::AlignLeft);

	createComponent();

	//时间定时器
	m_timer = new QTimer(this);
	m_timer->setInterval(1000);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_1s_timer()));
	m_timer->start();

	m_pushbutton_menu = new QPushButton(this);
	str_sheet = "QPushButton{border:0px solid #ffffff;background:transparent;font:30px;color:#ffffff;}";
	CCommonInterface::init_new_button(m_pushbutton_menu, 555, 0, str_length + 10, 100, str_sheet);
	connect(m_pushbutton_menu, SIGNAL(clicked()), this, SIGNAL(signal_button_menu()));
#ifndef CURRENT_ARM_RUN
	m_pushbutton_exit = new QPushButton(this);
	str_sheet = "";
	// CCommonInterface ::init_new_button(m_pushbutton_exit,164 + str_length + 30,0,40,25,str_sheet);
	m_pushbutton_exit->setFixedSize(40, 25);
	m_pushbutton_exit->move(555 - 50, 10 + (40 - 25) / 2);
	m_pushbutton_exit->setText("退出");
	connect(m_pushbutton_exit, SIGNAL(clicked()), this, SIGNAL(signal_button_exit()));
	m_pushbutton_exit->setStyleSheet("QPushButton{font:15px;}");


#endif
}

void CMainPage2::left_right_init1()
{
#if 1


	m_widget_bg = new QWidget(this);
	m_widget_bg->setFixedSize(200, 479);
	m_widget_bg->move(640 - 200, 0);
	//m_widget_bg->move(0,0);
	m_widget_bg->setObjectName("m_widget_bg");
	m_widget_bg->setStyleSheet("QWidget#m_widget_bg{border:0px;background:url(:/img_p2/res/p2/left_right/bg.png);}");
	//m_widget_bg->setStyleSheet("QWidget{border:0px;background-image:url(:/img_p2/res/p2/left_right/bg.png);}");
	//m_widget_bg->hide();
#if 1
	m_widget_show_picture1 = new showImage(this);
	m_widget_show_picture1->setFixedSize(440, 479);
	m_widget_show_picture1->move(0, 0);
	//m_widget_show_picture1->hide();
#endif
#endif
	m_label_network = new QLabel(this);
	m_label_network->setFixedSize(22, 15);
	m_label_network->move(455 - 5, 2);
	m_label_network->setStyleSheet("QLabel{border:0px;background:transparent;font:13px;color:#ffffff;}");

	//信号
	m_pushbutton_signal = new QPushButton(this);
	CCommonInterface::init_new_button(m_pushbutton_signal, 446, (40 - 18) / 2, 14, 18);

	//GPS
	m_label_gps_number = new QLabel(this);
	m_label_gps_number->setFixedSize(22, 15);
	m_label_gps_number->move(501, 2);
	m_label_gps_number->setStyleSheet("QLabel{border:0px;background:transparent;font:13px;color:#ffffff;}");

	m_pushbutton_gps = new QPushButton(this);
	CCommonInterface::init_new_button(m_pushbutton_gps, 480, (40 - 18) / 2, 18, 18);




	m_pushbutton_sd_status = new QPushButton(this);
	CCommonInterface::init_new_button(m_pushbutton_sd_status, 528, (40 - 16) / 2, 18, 16);


	m_label_tbp = new QLabel(this);
	m_label_tbp->setText(tr("B"));
	m_label_tbp->setFixedSize(20, 16);
	m_label_tbp->move(568, (40 - 16) / 2 - 1);
	m_label_tbp->setAlignment(Qt::AlignHCenter);
	m_label_tbp->setStyleSheet("QLabel{border:0px;background:transparent;font:9px;color:#ffffff;}");

	//TBP平台连接状态
	m_pushbutton_platform_status = new QPushButton(this);
	CCommonInterface::init_new_button(m_pushbutton_platform_status, 569, (40 - 16) / 2, 22, 16);

	m_label_tmp = new QLabel(this);
	m_label_tmp->setText(tr("M"));
	m_label_tmp->setFixedSize(20, 16);
	m_label_tmp->move(609, (40 - 16) / 2 - 1);
	m_label_tmp->setAlignment(Qt::AlignHCenter);
	m_label_tmp->setStyleSheet("QLabel{border:0px;background:transparent;font:9px;color:#ffffff;}");
	//TMP平台连接状态
	m_pushbutton_platform_status1 = new QPushButton(this);
	CCommonInterface::init_new_button(m_pushbutton_platform_status1, 609, (40 - 16) / 2, 22, 16);


#if 0
	//#ifndef CURRENT_ARM_RUN
	QString str_sheet = "QPushButton{border:0px;background:transparent;font:30px;color:#ffffff;}";

#ifndef CURRENT_USE_LIB
	//菜单和退出调换位置
	m_pushbutton_menu = new QPushButton(this);
	CCommonInterface::init_new_button(m_pushbutton_menu, 238 + 5 + 36 + 10, 18, 50, 42, str_sheet);
	m_pushbutton_menu->setText("退出");
	connect(m_pushbutton_menu, SIGNAL(clicked()), this, SIGNAL(signal_pushbutton_quit()));
#endif
#endif

	m_widget_qr_status = new QWidget(this);
	m_widget_qr_status->setFixedSize(110, 110);
	m_widget_qr_status->move(440 + (200 - 110) / 2 + 1, 63 - 8);


	QString str_show = tr("");
	QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:18px;color:#ffffff;}";
	m_label_qr_text = new QLabel(this);
	CCommonInterface::init_label_text(m_label_qr_text, 440, 172 - 5 - 2/*155*/, 200, 20, str_show, str_sheet, Qt::AlignCenter, 18);



	QString strCurrentTime = tr("2017-05-30 55:55:55");//QDateTime::currentDateTime().toString("hh:mm:ss");
	int str_length = CCommonInterface::get_string_length(strCurrentTime, 16);

	m_label_time = new QLabel(this);
	m_label_time->setFixedSize(200, 36);
	m_label_time->move(440, 95);
	m_label_time->setStyleSheet("QLabel{border:0px;background:transparent;font:36px;color:#ffffff; }");
	m_label_time->setAlignment(Qt::AlignCenter);

	m_label_time1 = new QLabel(this);
	m_label_time1->setFixedSize(200, 22);
	m_label_time1->move(440, 140);
	m_label_time1->setStyleSheet("QLabel{border:0px;background:transparent;font:18px;color:#ffffff; }");
	m_label_time1->setAlignment(Qt::AlignCenter);

	createComponent();
	//m_textBrown_slzr->show();

	//时间定时器
	m_timer = new QTimer(this);
	m_timer->setInterval(1000);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_1s_timer()));
	m_timer->start();

	m_timer_init = new QTimer(this);
	m_timer_init->setInterval(20000);
	connect(m_timer_init, SIGNAL(timeout()), this, SLOT(slot_init_time_out()));

	//m_widget_show_picture1->hide();


	//m_widget_bg->hide();

#ifndef CURRENT_ARM_RUN
	m_pushbutton_exit = new QPushButton(this);
	str_sheet = "";
	// CCommonInterface ::init_new_button(m_pushbutton_exit,164 + str_length + 30,0,40,25,str_sheet);
	m_pushbutton_exit->setFixedSize(40, 25);
	m_pushbutton_exit->move(164 + str_length + 30, (40 - 25) / 2);
	m_pushbutton_exit->setText("退出");
	connect(m_pushbutton_exit, SIGNAL(clicked()), this, SIGNAL(signal_button_exit()));
	m_pushbutton_exit->setStyleSheet("QPushButton{font:15px;}");
#endif



	m_widget_show_voice = new QWidget(this);
	m_widget_show_voice->setFixedSize(20, 16);
	m_widget_show_voice->move(10, this->height() - 45 + 12);
	m_widget_show_voice->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/icon_news.png);}");
	m_widget_show_voice->hide();

	m_label_scroll_text = new QLabel(this);
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:16px;color:#72a4ff;}";
	str_show = "";
	CCommonInterface::init_label_text(m_label_scroll_text, 36, this->height() - 45 + 9, MESSAGE_SHOW_LENGTH, 20, str_show, str_sheet, Qt::AlignLeft, 16);



	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:24px;color:#ffffff;}";
	m_label_line_name = new QLabel(this);
	CCommonInterface::init_label_text(m_label_line_name, 470 - 30 + 20, 334, 120, 30, str_show, str_sheet, Qt::AlignCenter);

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:18px;color:#ffffff;}";
	m_label_direction = new QLabel(this);
	//CCommonInterface::init_label_text(m_label_direction,553,338 + 1,80,25,str_show,str_sheet,Qt::AlignLeft,18);
	CCommonInterface::init_label_text(m_label_direction, 580, 341 + 5, 12, 12, str_show, str_sheet, Qt::AlignLeft, 18);


	str_show = tr("本站:");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:16px;color:#ffffff;}";
	m_label_station1 = new QLabel(this);
	CCommonInterface::init_label_text(m_label_station1, 440, 359, 100, 20, str_show, str_sheet, Qt::AlignLeft, 16);
	m_label_station1->hide();

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:30px;color:#ffffff;}";
	m_label_station2 = new QLabel(this);
	CCommonInterface::init_label_text(m_label_station2, 440, 381, 200, 33, str_show, str_sheet, Qt::AlignLeft, 30);

	m_dynamic_text_station_name = new CDynamicText(this);
	connect(m_dynamic_text_station_name, SIGNAL(signal_dynamic_text_event(CDynTextParam)), this, SLOT(slot_dynamic_text_event(CDynTextParam)));

	m_dynamic_text_station_name1 = new CDynamicText(this);
	connect(m_dynamic_text_station_name1, SIGNAL(signal_dynamic_text_event(CDynTextParam)), this, SLOT(slot_dynamic_text_event(CDynTextParam)));


	m_dynamic_text_message = new CDynamicText(this);
	connect(m_dynamic_text_message, SIGNAL(signal_dynamic_text_event(CDynTextParam)), this, SLOT(slot_dynamic_text_event(CDynTextParam)));


	slot_1s_timer();

	//set_message_content("中国智能公交系统整体方案提供商及服务商");

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:20px;color:#ffffff;}";
	m_label_charge_type = new QLabel(this);
	CCommonInterface::init_label_text(m_label_charge_type, 467, 243, 200, 25, str_show, str_sheet, Qt::AlignLeft, 20);

	/*
		QLabel *m_label_ticket; //票价
		QLabel *m_label_ticket1; //金额
		QLabel *m_label_remaining_sum;  //余额
		QLabel *m_label_remaining_sum1; //余额1

		QLabel *m_label_arrive;//到站
		QLabel *m_label_arrive1;//到站
	*/
	str_show = tr("票价");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:20px;color:#ffffff;}";
	m_label_ticket = new QLabel(this);
	CCommonInterface::init_label_text(m_label_ticket, 440, 307, 200, 20, str_show, str_sheet, Qt::AlignCenter, 20);

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:28px;color:#ffffff;}";
	m_label_ticket1 = new QLabel(this);
	CCommonInterface::init_label_text(m_label_ticket1, 440, 341 - 3, 200, 32, str_show, str_sheet, Qt::AlignCenter, 28);

	str_show = tr("余额");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:20px;color:#ffffff;}";
	m_label_remaining_sum = new QLabel(this);
	CCommonInterface::init_label_text(m_label_remaining_sum, 440, 394, 200, 22, str_show, str_sheet, Qt::AlignCenter, 20);

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:28px;color:#ffffff;}";
	m_label_remaining_sum1 = new QLabel(this);
	CCommonInterface::init_label_text(m_label_remaining_sum1, 440, 426, 200, 32, str_show, str_sheet, Qt::AlignCenter, 28);

	str_show = tr("到站");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:20px;color:#ffffff;}";
	m_label_arrive = new QLabel(this);
	CCommonInterface::init_label_text(m_label_arrive, 440, 394, 200, 22, str_show, str_sheet, Qt::AlignCenter, 20);

	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #ffffff;background:transparent;font:28px;color:#ffffff;}";
	m_label_arrive1 = new QLabel(this);
	CCommonInterface::init_label_text(m_label_arrive1, 440, 426 - 3, 200, 32, str_show, str_sheet, Qt::AlignCenter, 28);


	str_show = tr("");
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:17px;color:#FFFFFF;}";
	m_label_printf_test = new QLabel(this);
	CCommonInterface::init_label_text(m_label_printf_test, 440, 133, 200, 70, str_show, str_sheet, Qt::AlignHCenter | Qt::AlignTop, 17);

	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:22px;color:#4f89de;}";
	m_label_message_type = new QLabel(this);
	CCommonInterface::init_label_text(m_label_message_type, 440, 87, 200, 25, str_show, str_sheet, Qt::AlignCenter, 22);

	m_widget_message_bg = new QWidget(this);
	m_widget_message_bg->setFixedSize(132, 1);
	m_widget_message_bg->move(474, 119);
	m_widget_message_bg->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/line_blue.png);}");

	m_pushbutton_menu = new QPushButton(this);
	str_sheet = "QPushButton{border:0px solid #ffffff;background:transparent;font:30px;color:#ffffff;}";
	CCommonInterface::init_new_button(m_pushbutton_menu, 445, 200/*460,50*/, 100/*str_length + 10*/, 100, str_sheet);
	connect(m_pushbutton_menu, SIGNAL(clicked()), this, SIGNAL(signal_button_menu()));
	//connect(m_pushbutton_menu,SIGNAL(clicked()),this,SLOT(slot_button_menu_test()));

	//str_show = "20178";
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:18px;color:#ffffff;}";
	m_m_label_vehicle_number = new QLabel(this);
	CCommonInterface::init_label_text(m_m_label_vehicle_number, 440 + 100, 230 - 5, 100 - 10, 20, str_show, str_sheet, Qt::AlignRight, 18);

	str_show = "";
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:18px;color:#bdcbe5;}";
	m_m_label_station_status = new QLabel(this);
	CCommonInterface::init_label_text(m_m_label_station_status, 440, 422, 200, 20, str_show, str_sheet, Qt::AlignCenter, 18);

	m_m_widget_debug_bg = new QWidget(this);
	m_m_widget_debug_bg->setFixedSize(640, 480);
	m_m_widget_debug_bg->move(0, 0);
	m_m_widget_debug_bg->setStyleSheet("QWidget {border:0px;background:#000000;}");

	m_widget_init_bg = new QWidget(this);
	m_widget_init_bg->setFixedSize(640, 480);
	m_widget_init_bg->move(0, 0);
	m_widget_init_bg->setStyleSheet("QWidget {border:0px;background:#000000;}");
	m_widget_init_bg->hide();

	str_show = "设备连接中...";
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:60px;color:#ffffff;}";
	m_label_debug = new QLabel(this);
	CCommonInterface::init_label_text(m_label_debug, 185, 192 - 7, 390, 70, str_show, str_sheet, Qt::AlignLeft, 60);

	str_show = "设备正在从服务器更新配置...";
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:40px;color:#ffffff;}";
	m_label_init = new QLabel(this);
	CCommonInterface::init_label_text(m_label_init, 100, 192 - 7, 550, 70, str_show, str_sheet, Qt::AlignLeft, 60);
	m_label_init->hide();

	str_show = "请 您 到 前 门 扫 码 或 刷 卡";
	str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:20px;color:#ffffff;}";
	m_label_debug_tip = new QLabel(this);
	CCommonInterface::init_label_text(m_label_debug_tip, 197 - 4, 429 - 5, 265, 30, str_show, str_sheet, Qt::AlignLeft, 20);

	//QWidget *m_widget_line1;
	//QWidget *m_widget_line2;
	m_widget_line1 = new QWidget(this);
	m_widget_line1->setFixedSize(20, 1);
	m_widget_line1->move(157, 439);
	m_widget_line1->setStyleSheet("QWidget {border:0px;background:#ffffff;}");

	m_widget_line2 = new QWidget(this);
	m_widget_line2->setFixedSize(20, 1);
	m_widget_line2->move(464, 439);
	m_widget_line2->setStyleSheet("QWidget {border:0px;background:#ffffff;}");

	m_widget_icon = new QWidget(this);
	m_widget_icon->setFixedSize(80, 80);
	m_widget_icon->move(94, 180 + 10);
	m_widget_icon->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/icon_prompt_x1.png);}");

	m_widget_init_icon = new QWidget(this);
	m_widget_init_icon->setFixedSize(70, 70);
	m_widget_init_icon->move(20, 180);
	m_widget_init_icon->setStyleSheet("QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/icon_prompt_x1.png);}");
	m_widget_init_icon->hide();

	if (1 == CCommonData::getInstance()->m_naviFinish)
	{
		// 确保仅从导航界面启动时显示
		CCommonData::getInstance()->m_naviFinish = 0;
		m_timer_init->start();
		show_init_tips(true);
	}
	else
	{
		show_init_tips(false);
	}
}


void CMainPage2::show_init_tips(bool _flag)
{
	qDebug() << "CMainPage2::show_init_tips(bool _flag) _flag:\n" << _flag;
	if (_flag)
	{
		m_widget_init_bg->show();
		m_label_init->show();
		m_widget_init_icon->show();
	}
	else
	{
		m_widget_init_bg->hide();
		m_label_init->hide();
		m_widget_init_icon->hide();
	}
}

void CMainPage2::slot_head_widget_notify(int _msg, int _event, void *_param)
{
	QString str_sheet = "";


	switch (_msg)
	{
		////////////////////////////////////////////////////////////////////头部显示 start
	case kEnumMainPageSignal1://SIM卡　2G 3G 4G
	{
		if (NULL == m_label_network)
			break;
		if (kEnumNetwork2G == _event)
			m_label_network->setText(tr("2G"));
		else if (kEnumNetwork3G == _event)
			m_label_network->setText(tr("3G"));
		else if (kEnumNetwork4G == _event)
			m_label_network->setText(tr("4G"));
		else
			m_label_network->setText(tr(""));
	}break;
	case kEnumMainPageSignal2: //SIM卡信号强弱
	{
		if (NULL == m_pushbutton_signal)
			break;

		if (kEnumNetworkSignal1 == _event)
		{
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
				.arg(":/img_p2/res/p2/left_right/signal_1.png");
			CCommonInterface::modify_button_sheet(m_pushbutton_signal, str_sheet);
		}
		else if (kEnumNetworkSignal2 == _event)
		{
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
				.arg(":/img_p2/res/p2/left_right/signal_2.png");
			CCommonInterface::modify_button_sheet(m_pushbutton_signal, str_sheet);
		}
		else if (kEnumNetworkSignal3 == _event)
		{
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
				.arg(":/img_p2/res/p2/left_right/signal_3.png");
			CCommonInterface::modify_button_sheet(m_pushbutton_signal, str_sheet);
		}
		else if (kEnumNetworkSignal4 == _event)
		{
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
				.arg(":/img_p2/res/p2/left_right/signal_4.png");
			CCommonInterface::modify_button_sheet(m_pushbutton_signal, str_sheet);
		}
		else if (kEnumNetworkSignalError == _event)
		{
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
				.arg(":/img_p2/res/p2/left_right/signal_5.png");
			CCommonInterface::modify_button_sheet(m_pushbutton_signal, str_sheet);
		}
		else
		{
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
				.arg(":/img_p2/res/p2/left_right/signal.png");

			CCommonInterface::modify_button_sheet(m_pushbutton_signal, str_sheet);
		}
	}break;
	case kEnumMainPageGpsNumber://GPS个数
	{
		if (NULL == m_label_gps_number)
			break;

		if (_event >= 0)
			m_label_gps_number->setText(QString::number(_event));
		else
			m_label_gps_number->setText("");

	}break;
	case kEnumMainPageGpsStatus://GPS状态
	{
		if (NULL == m_pushbutton_gps)
			break;

		if (kEnumGpsNormal == _event)
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/positioning.png");
		else //if(kEnumGpsAbnormal == _event)
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/positioning_2.png");

		CCommonInterface::modify_button_sheet(m_pushbutton_gps, str_sheet);

	}break;
	case kEnumMainPagePlatformStatus://平台连接状态
	{
		if (NULL == m_pushbutton_platform_status)
			break;
		m_label_tbp->move(568, (40 - 16) / 2 - 1);

		if (kEnumPlatformNormal == _event)
		{
			m_label_tbp->move(570, (40 - 16) / 2 - 1);
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/screen_status_1.png");
		}

		else if (kEnumPlatformAbnormal1 == _event)
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/screen_status_2.png");
		else if (kEnumPlatformAbnormal2 == _event)
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/screen_status_3.png");
		else if (kEnumPlatformConnect == _event)
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/screen_status_4.png");
		else
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/screen_status_2.png");

		CCommonInterface::modify_button_sheet(m_pushbutton_platform_status, str_sheet);

	}break;
	case kEnumMainPageTMPPlatformStatus: //TMP平台连接状态
	{
		if (NULL == m_pushbutton_platform_status1)
			break;
		m_label_tmp->move(609, (40 - 16) / 2 - 1);
		// 连接成功
		if (kEnumTMP_SUCCESS == _event)
		{
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/screen_status_1.png");
			// TMP图标大小不一致，“M”文字调整居中
			m_label_tmp->move(610, (40 - 16) / 2 - 1);
		}
		else if (kEnumTMP_FAILURE == _event)
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/screen_status_2.png");
		// 连接中
		else if (kEnumTMP_DOING == _event)
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/screen_status_4.png");
		else
			str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/screen_status_2.png");

		CCommonInterface::modify_button_sheet(m_pushbutton_platform_status1, str_sheet);

	}break;
	case kEnumMainPageSdStatus:
	{
		show_sd_status(_event);
	}break;
	case kEnumMainPageAdvert:          //（广告图片）左右显示
	{
		if (NULL == m_widget_show_picture1)
			break;

		CEventData *pData = (CEventData *)_param;
		if (pData)
		{
			m_widget_show_picture1->setStyleSheet(QString("QWidget {border:0px;border-image:url(%1); }")
				.arg(pData->m_str_data));
		}
	}break;
	case kEnumMainPageVerificationRrStatus:
	{
		printf("kEnumMainPageVerificationRrStatus   _event:%d ", _event);
		QString str_show_text = "";

		if (kEnumLeftRight == m_type)
		{
			if (NULL == m_widget_qr_status)
				break;
			bool start_timer = false;
			QString str_sheet = "QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/icon_failure.png);}";
			int timer_plays = 3;

			if (m_is_demo)
			{
				m_widget_qr_status->hide();
				m_label_qr_text->hide();
			}
			else
			{
				m_widget_qr_status->show();
				m_label_qr_text->show();
			}

			if (kEnumQrStatusNormal == _event)
			{
				//str_sheet = "QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/msg_1.png);}";
				m_label_charge_type->setText(tr("一票制"));

				m_label_time->show();
				m_label_time1->show();
				m_label_charge_type->show();
				m_label_line_name->show();
				m_label_direction->show();
				m_label_station2->show();
				m_m_label_station_status->show();

				m_label_ticket->hide(); //票价
				m_label_ticket1->hide(); //金额
				m_label_remaining_sum->hide();  //余额
				m_label_remaining_sum1->hide(); //余额1
				m_label_arrive->hide();//到站
				m_label_arrive1->hide();//到站
				m_label_printf_test->hide();
				m_widget_qr_status->hide();
				m_label_qr_text->hide();
				m_label_message_type->hide();//刷卡消息类型
				m_widget_message_bg->hide();
				if (e_Dev_Mode_Slave_Mac == CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE)
				{
					m_m_label_station_status->hide();
					m_label_direction->hide();
					m_label_station2->hide();
				}
				if (e_Charge_Mode_One_Ticket == CCommonData::getInstance()->m_CSystemPosSetup.CHARGMODE)
				{
					slot_head_widget_notify(kEnumMainPageChargeType, e_Charge_Mode_One_Ticket, 0);
				}
				else if (e_Charge_Mode_Block_Charge == CCommonData::getInstance()->m_CSystemPosSetup.CHARGMODE)
				{
					slot_head_widget_notify(kEnumMainPageChargeType, e_Charge_Mode_Block_Charge, 0);
				}
				else
				{
				}
			}
			else if (kEnumQrStatusSuccess == _event)
			{
				start_timer = true;
				str_sheet = "QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/icon_success.png);}";

				if ("元" == m_label_ticket1->text())
				{
					qDebug() << "slot_head_widget_notify：:kEnumQrStatusSuccess monery　is  zero , get on bus for section Charge\n";
				}
				else
				{
					qDebug() << "slot_head_widget_notify：:kEnumQrStatusSuccess monery is not zero , not get on bus for section Charge\n";
					show_failed_interface();

					//m_label_charge_type->hide();
					m_label_line_name->hide();
					m_label_direction->hide();
					m_label_station2->hide();
					m_m_label_station_status->hide();

					m_label_ticket->show(); //票价
					m_label_ticket1->show(); //金额
					m_label_remaining_sum->hide();  //余额
					m_label_remaining_sum1->hide(); //余额1
					m_label_arrive->show();//到站
					m_label_arrive1->show();//到站
				}
				m_label_time->hide();
				m_label_time1->hide();
				str_show_text = tr("扫码成功");
				CEventData *pData = (CEventData *)_param;
				if (pData)
				{
					if (pData->m_i_data > 0)
						timer_plays = pData->m_i_data;
				}
			}
			else if (kEnumQrStatusFail == _event)
			{
				start_timer = true;
				str_sheet = "QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/icon_failure.png);}";

				show_failed_interface();
				str_show_text = tr("验码失败");
				CEventData *pData = (CEventData *)_param;
				if (pData)
				{
					if (pData->m_i_data > 0)
						timer_plays = pData->m_i_data;
				}

			}
			else if (kEnumQrStatusabNormal == _event)
			{
				start_timer = true;
				str_sheet = "QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/icon_prompt.png);}";

				show_failed_interface();
				str_show_text = tr("请重新扫码");

				CEventData *pData = (CEventData *)_param;
				if (pData)
				{
					if (pData->m_i_data > 0)
						timer_plays = pData->m_i_data;
				}
			}
			else if (kEnumQrStatusInvalid == _event)  /////new
			{
				start_timer = true;
				str_sheet = "QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/icon_failure.png);}";

				show_failed_interface();
				str_show_text = tr("二维码无效");

				CEventData *pData = (CEventData *)_param;
				if (pData)
				{
					if (pData->m_i_data > 0)
						timer_plays = pData->m_i_data;
				}

			}
			//刷卡
			else if (kEnumCardStatusSuccess == _event)
			{
				start_timer = true;
				str_sheet = "QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/icon_success.png);}";

				if ("元" == m_label_ticket1->text())
				{
					qDebug() << "slot_head_widget_notify：:kEnumQrStatusSuccess monery　is  zero , get on bus for section Charge\n";
				}
				else
				{
					qDebug() << "slot_head_widget_notify：:kEnumQrStatusSuccess monery is not zero , not get on bus for section Charge\n";
					show_failed_interface();

					//m_label_charge_type->hide();
					m_label_line_name->hide();
					m_label_direction->hide();
					m_label_station2->hide();
					m_m_label_station_status->hide();

					m_label_ticket->show(); //票价
					m_label_ticket1->show(); //金额
					m_label_remaining_sum->hide();  //余额
					m_label_remaining_sum1->hide(); //余额1
					m_label_arrive->show();//到站
					m_label_arrive1->show();//到站
				}
				m_label_time->hide();
				m_label_time1->hide();
				str_show_text = tr("刷卡成功");
				CEventData *pData = (CEventData *)_param;
				if (pData)
				{
					if (pData->m_i_data > 0)
						timer_plays = pData->m_i_data;
					QString str_content = "";

					if (0x00 == pData->m_param1)
						str_content = tr("司机卡");
					else if (0x01 == pData->m_param1)
						str_content = tr("普通卡");
					else if (0x02 == pData->m_param1)
						str_content = tr("学生卡");
					else if (0x03 == pData->m_param1)
						str_content = tr("老人卡");
					else if (0x04 == pData->m_param1)
						str_content = tr("测试卡");
					else if (0x05 == pData->m_param1)
						str_content = tr("军人卡");
					else if (0x06 == pData->m_param1)
						str_content = tr("优惠卡");
					else if (0x07 == pData->m_param1)
						str_content = tr("职工卡");

					CEventData event_data;
					event_data.m_str_data = str_content;
					slot_head_widget_notify(kEnumMainPageChargeType, e_Charge_Mode_Other, &event_data);

				}

			}
			else if (kEnumCardStatusFail == _event)
			{
				start_timer = true;
				str_sheet = "QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/icon_failure.png);}";

				show_failed_interface();
				str_show_text = tr("刷卡失败");

				CEventData *pData = (CEventData *)_param;
				if (pData)
				{
					if (pData->m_i_data > 0)
						timer_plays = pData->m_i_data;

					if (m_is_demo)
					{
						m_label_printf_test->show();
						m_label_printf_test->setText(pData->m_str_data);
					}
				}
			}
			else if (kEnumCardStatusAbnormal == _event)
			{
				start_timer = true;
				str_sheet = "QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/icon_prompt.png);}";

				show_failed_interface();
				str_show_text = tr("请重新刷卡");
				CEventData *pData = (CEventData *)_param;
				if (pData)
				{
					if (pData->m_i_data > 0)
						timer_plays = pData->m_i_data;
					if (m_is_demo)
					{
						m_label_printf_test->show();
						m_label_printf_test->setText(pData->m_str_data);
					}

				}

			}
			else if (kEnumCardStatusFlow == _event)
			{
				start_timer = true;
				str_sheet = "QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/icon_failure.png);}";

				show_failed_interface();
				str_show_text = tr("余额不足");
				if (m_is_demo)
				{
					m_label_printf_test->show();
					m_label_printf_test->setText(str_show_text);
				}
				CEventData *pData = (CEventData *)_param;
				if (pData)
				{
					if (pData->m_i_data > 0)
						timer_plays = pData->m_i_data;
				}
			}
			else if (kEnumCardStatusMessage == _event)
			{
				CEventData *pData = (CEventData *)_param;
				if (pData)
				{
					if (pData->m_i_data > 0)
						timer_plays = pData->m_i_data;

					if (m_is_demo)
					{
						start_timer = true;
						m_label_printf_test->show();
						m_label_printf_test->setText(pData->m_str_data);
					}
					else
					{
						start_timer = true;
						show_failed_interface();
						m_widget_qr_status->hide();
						m_label_qr_text->hide();
						m_label_printf_test->show();
						m_label_printf_test->setText(pData->m_str_data);

						m_label_message_type->show();
						m_widget_message_bg->show();

						QString text_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:22px;color:#%1;}";
						QString bg_sheet = "QWidget {border:0px;border-image:url(:/img_p2/res/p2/left_right/%1);}";

						if (1 != pData->m_param1)
						{
							m_label_message_type->setText(tr("提示信息"));
							m_label_message_type->setStyleSheet(text_sheet.arg("4f89de"));
							m_widget_message_bg->setStyleSheet(bg_sheet.arg("line_blue.png"));
						}
						else
						{
							m_label_message_type->setText(tr("交易失败"));
							m_label_message_type->setStyleSheet(text_sheet.arg("ff4546"));
							m_widget_message_bg->setStyleSheet(bg_sheet.arg("line_rred.png"));
						}



					}

				}
			}

			m_widget_qr_status->setStyleSheet(str_sheet);
			m_label_qr_text->setText(str_show_text);
			update();
			//start_timer = false;
			if (start_timer)
			{
				if (m_timer_qr->isActive())
					m_timer_qr->stop();

				m_timer_qr->setInterval(timer_plays * 1000);
				m_timer_qr->start();
			}

		}//end
		else
		{
			bool start_timer = false;

			CMyMessageBox1::getInstance()->move(210, 100);

			if (kEnumQrStatusSuccess == _event)
			{
				start_timer = true;
				CMyMessageBox1::getInstance()->set_type(CMyMessageBox1::kEnumSuccess);
			}
			else if (kEnumQrStatusFail == _event)
			{
				start_timer = true;
				CMyMessageBox1::getInstance()->set_type(CMyMessageBox1::kEnumFail);
			}
			else if (kEnumQrStatusabNormal == _event)
			{
				start_timer = true;
				CMyMessageBox1::getInstance()->set_type(CMyMessageBox1::kEnumAbnormal);
			}
			if (start_timer)
			{
				CMyMessageBox1::getInstance()->set_timer();
				CMyMessageBox1::getInstance()->exec();
			}
		}



	}break;
	case kEnumMainPageLine:    //线路
	{
		if (NULL == m_label_line_name)
			break;

		QString str_up = tr("(下行)");
		CEventData *pData = (CEventData *)_param;
		if (pData)
		{
			if (1 == pData->m_i_data)
			{
				//str_up = tr("(上行)");
				m_label_direction->setPixmap(QPixmap(":/img_p2/res/p2/left_right/icon_arrow_up.png"));
			}
			else
			{
				m_label_direction->setPixmap(QPixmap(":/img_p2/res/p2/left_right/icon_arrow_down.png"));
			}
			if (kEnumLeftRight == m_type)
			{

				m_label_line_name->setText(pData->m_str_data);
				//m_label_direction->setText(str_up);
			}
			else
			{
				m_label_line_name->setText(pData->m_str_data);
				m_label_direction->setText(str_up);
			}
		}

	}break;
	case kEnumMainPageUpdateStationName:
	{
		if (NULL == m_dynamic_text_station_name)
			break;


		CEventData *pData = (CEventData *)_param;

		if (pData)
		{
			CDynTextParam param_station_name;

			qDebug() << "station_first   :" << CCommonData::getInstance()->m_sd_info.m_first_station;
			qDebug() << "station_last    :" << CCommonData::getInstance()->m_sd_info.m_last_station;
			qDebug() << "station_current :" << pData->m_str_data;

			if (pData->m_str_data == CCommonData::getInstance()->m_sd_info.m_first_station)
			{
				qDebug() << QString::fromLocal8Bit("station_type : 起始站");
				slot_head_widget_notify(kEnumMainPageStationStatus, kEnumStationStart, 0);
			}
			else if (pData->m_str_data == CCommonData::getInstance()->m_sd_info.m_last_station)
			{
				qDebug() << QString::fromLocal8Bit("station_type : 终点站");
				slot_head_widget_notify(kEnumMainPageStationStatus, kEnumStationEnd, 0);
			}
			else
			{
				qDebug() << QString::fromLocal8Bit("station_type : 本站");
			}


			if (m_is_demo)
			{
				param_station_name.m_control_this = m_label_station2;
				param_station_name.m_control_type = CDynamicText::kEnumLabel;
				param_station_name.m_control_id = kEnumPushbuttonStationName;
				param_station_name.m_control_source_text = pData->m_str_data;
				param_station_name.m_control_font_size = 55;
				param_station_name.m_control_show_length = 400;
			}
			else
			{
				param_station_name.m_control_this = m_label_station2;
				param_station_name.m_control_type = CDynamicText::kEnumLabel;
				param_station_name.m_control_id = kEnumPushbuttonStationName;
				param_station_name.m_control_source_text = pData->m_str_data;
				param_station_name.m_control_font_size = m_station_font_size;
				param_station_name.m_control_show_length = m_station_show_length;
			}


			m_dynamic_text_station_name->set_text(param_station_name);
		}
	}break;
	case kEnumMainPageUpdateStationName1:
	{
		if (NULL == m_dynamic_text_station_name1)
			break;

		CEventData *pData = (CEventData *)_param;
		if (pData)
		{
			CDynTextParam param_station_name;
			param_station_name.m_control_this = m_label_arrive1;
			param_station_name.m_control_type = CDynamicText::kEnumLabel;
			param_station_name.m_control_id = kEnumPushbuttonStationName1;
			param_station_name.m_control_source_text = pData->m_str_data;
			param_station_name.m_control_font_size = 28;
			param_station_name.m_control_show_length = m_station_show_length;

			m_dynamic_text_station_name1->set_text(param_station_name);
		}
	}break;
	case kEnumMainPageCurrentStationName:
	{
		if (NULL == m_label_station2)
			break;

		CEventData *pData = (CEventData *)_param;
		if (pData)
		{
			m_label_station2->setText(pData->m_str_data);
		}
	}break;
	case kEnumMainPageCurrentStationName1:
	{
		if (NULL == m_label_arrive1)
			break;

		CEventData *pData = (CEventData *)_param;
		if (pData)
		{
			//qDebug() << m_label_arrive1->pos();
			//qDebug() << m_label_arrive1->rect();
			m_label_arrive1->setText(pData->m_str_data);
		}
	}break;
	case kEnumMainPageCurrentMessage:
	{
		if (NULL == m_label_scroll_text)
			break;

		CEventData *pData = (CEventData *)_param;
		if (pData)
		{
			m_label_scroll_text->setText(pData->m_str_data);
		}
	}break;
	case kEnumMainPageChargeType:
	{
		if (e_Charge_Mode_One_Ticket == _event)
		{
			m_label_charge_type->setText(tr("一票制"));
			if (!m_is_demo)
			{
				m_label_charge_type->move(467 - 2, 243 - 3 - 2);
				m_label_charge_type->setFixedSize(200, 25);
			}

		}
		else if (e_Charge_Mode_Block_Charge == _event)
		{
			if (!m_is_demo)
			{
				m_label_charge_type->move(477, 230 - 3);
				m_label_charge_type->setFixedSize(200, 60);
				m_label_charge_type->setText(tr("分段\n收费"));
			}
			else
			{
				m_label_charge_type->setText(tr("分段收费"));
			}
		}
		else if (e_Charge_Mode_Other == _event)
		{
			CEventData *pData = (CEventData *)_param;
			if (pData)
			{
				m_label_charge_type->setText(pData->m_str_data);
				if (!m_is_demo)
				{
					m_label_charge_type->move(467 - 2, 243 - 3 - 2);
					m_label_charge_type->setFixedSize(200, 25);
				}
			}

		}

	}break;
	case kEnumMainPageAmount:        //金额
	{

		CEventData *pData = (CEventData *)_param;
		if (pData)
		{
			m_label_ticket1->setText(pData->m_str_data);
		}

		printf("kEnumMainPageAmount   pData->m_str_data:%d ", pData->m_str_data.toUtf8().data());

	}break;
	case kEnumMainPageBalance:       //余额
	{
		CEventData *pData = (CEventData *)_param;
		if (pData)
		{
			m_label_remaining_sum1->setText(pData->m_str_data);
		}

	}break;
#if 0
	case kEnumMainPageArriveStation:  //到站
	{
		CEventData *pData = (CEventData *)_param;
		if (pData)
		{
			m_label_arrive1->setText(pData->m_str_data);
		}
	}break;
#endif
	case kEnumMainPageStationStatus:
	{
		QString str_status = "本站";
#if 0
		if (kEnumStationIn == _event)
			str_status = CLtyString::getInstance()->m_station_in;
		else if (kEnumStationOut == _event)
			str_status = CLtyString::getInstance()->m_station_out;
		else
#endif
			if (kEnumStationStart == _event)
				str_status = "起点站";
			else if (kEnumStationEnd == _event)
				str_status = "终点站";

		m_m_label_station_status->setText(str_status);

	}break;
	case kEnumMainPageVehicleModel:
	{
		CEventData *pData = (CEventData *)_param;
		if (0 == _event)
		{
			if (pData)
			{
				m_m_label_vehicle_number->setText(pData->m_str_data);
			}
		}
	}break;
	case kEnumMainPageFromStatus:
	{
		CEventData *pData = (CEventData *)_param;

		if (1 == _event)
			debug_interface(false);
		else
			debug_interface(true);

	}break;
	case kEnumMainPageFromMachine:
	{
		m_m_label_station_status->hide();
		m_label_direction->hide();
		m_label_station2->hide();

	}break;
	////////////////////////////////////////////////////////////////////头部显示 end
	}
}

void CMainPage2::show_sd_status(int _event)
{
	qDebug() << "CMainPage2::show_sd_status(int _event) _event:" << _event;
	if (NULL == m_pushbutton_sd_status)
	{
		return;
	}
	QString str_sheet = "";
	if (kEnumSdNormalNormal == _event)
		str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/SD_card.png");
	else if (kEnumError == _event)
		str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/SD_card_3.png");
	else
		str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img_p2/res/p2/left_right/SD_card_2.png");

	CCommonInterface::modify_button_sheet(m_pushbutton_sd_status, str_sheet);
}

QString CMainPage2::get_week_string(int _week)
{
	QString ret_value = "";

	switch (_week)
	{
	case 1:
		ret_value = tr("一");
		break;
	case 2:
		ret_value = tr("二");
		break;
	case 3:
		ret_value = tr("三");
		break;
	case 4:
		ret_value = tr("四");
		break;
	case 5:
		ret_value = tr("五");
		break;
	case 6:
		ret_value = tr("六");
		break;
	case 7:
		ret_value = tr("日");
		break;
	}
	return ret_value;
}

void CMainPage2::slot_init_time_out()
{
	m_timer_init->stop();
	show_init_tips(false);
}

void CMainPage2::slot_1s_timer()
{
	QString strCurrentTime = QDateTime::currentDateTime().toString("hh:mm:ss");//yyyy-MM-dd
	QString strCurrentTime1 = "";

	if (m_is_demo)
	{
		m_label_time1->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
	}
	else
	{
		m_label_time->setText(strCurrentTime);
		QDate date;
		int current_week = 0;
		date.setDate(QDateTime::currentDateTime().date().year(),
			QDateTime::currentDateTime().date().month(),
			QDateTime::currentDateTime().date().day());
		current_week = date.dayOfWeek();

		strCurrentTime1 = tr("星期") + get_week_string(current_week)
			+ tr(" | ") + QDateTime::currentDateTime().toString("yyyy/MM/dd");
		m_label_time1->setText(strCurrentTime1);
	}

	if (m_seconds > 2 || m_seconds < 0) {
		stUIData *uiData = GetStatusData();
		QString msgText = "邯郸公交 ";
		char buffer[500];
		int pos = 0;

		if (uiData->message[0] != '\0') {
			msgText = "<br/><font size=\"10\" color=\"red\">";
			msgText += uiData->message;
			msgText += "</font>";
			uiData->message[0] = '\0';
		}
		else {
			if (uiData->isGJOk) {
				msgText += "G";
			}
			else {
				msgText += "N";
			}
			if (uiData->isGpsOk) {
				msgText += "R";
			}
			else {
				msgText += "L";
			}
			sprintf(buffer, " v%X.%02X", (uiData->version >> 8) & 0x00FF, uiData->version & 0x00FF);
			msgText += (buffer);
			msgText += ("<br/>");
			// new line
			sprintf(buffer, "<font size=\"10\" color=\"red\">&nbsp;&nbsp;&nbsp;&nbsp;%d.%02d元   </font>", uiData->basePrice / 100, uiData->basePrice % 100);
			msgText += (buffer);
			msgText += ("<br/>");
			// 第二行
			pos = 0;
			BytesToChars(uiData->lineId, 2, buffer + pos, 50);
			pos += 4;
			buffer[pos] = '-';
			++pos;
			BytesToChars(uiData->lineId + 2, 1, buffer + pos, 50);
			pos += 2;
			msgText += (buffer);
			msgText += "路 ";
			msgText += uiData->devId;
			msgText += ("<br/>");
			// the third line
			sprintf(buffer, "IC:%d", uiData->uploadRec);
			msgText += buffer;

			sprintf(buffer, " %02X-%02X", uiData->task, uiData->linkStatus);
			msgText += buffer;
		}

		m_textBrown_slzr->setHtml(msgText);
		m_seconds = 0;
	}
	else {
		++m_seconds;
	}
}

void CMainPage2::slot_dynamic_text_event(const CDynTextParam &_param)
{
	QString str_sheet = "";
	switch (_param.m_control_id)
	{
	case kEnumPushbuttonStationName:
	{
		switch (_param.m_action)
		{
		case CDynamicText::kEnumAlignmentLeft: //文字左对齐
#if 0
		case CDynamicText::kEnumAlignmentRight://文字居中对齐
		{
			m_label_station2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		}break;
#endif
		case CDynamicText::kEnumAlignmentRight://文字居中对齐
		{
			m_label_station2->setAlignment(Qt::AlignCenter);
		}break;

		case CDynamicText::kEnumUpdateText:    //更新文本显示
		{
			CEventData eventData;
			eventData.m_str_data = _param.m_control_show_text;
			slot_head_widget_notify(kEnumMainPageCurrentStationName, 0, &eventData);         //当前站名

		}break;
		}
	}break;
	case kEnumPushbuttonStationName1:
	{
		switch (_param.m_action)
		{
		case CDynamicText::kEnumAlignmentLeft: //文字左对齐
		case CDynamicText::kEnumAlignmentRight://文字居中对齐
		{
			m_label_arrive1->setAlignment(Qt::AlignCenter);
		}break;

		case CDynamicText::kEnumUpdateText:    //更新文本显示
		{
			CEventData eventData;
			eventData.m_str_data = _param.m_control_show_text;
			slot_head_widget_notify(kEnumMainPageCurrentStationName1, 0, &eventData);         //当前站名

		}break;
		}
	}break;
	case kEnumLabelMessageShow:
	{

		switch (_param.m_action)
		{
		case CDynamicText::kEnumAlignmentLeft: //文字左对齐
		case CDynamicText::kEnumAlignmentRight://文字居中对齐
		{
			m_label_scroll_text->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		}break;
#if 0
		case CDynamicText::kEnumAlignmentRight://文字居中对齐
		{
			m_label_scroll_text->setAlignment(Qt::AlignCenter);
		}break;
#endif
		case CDynamicText::kEnumUpdateText:    //更新文本显示
		{
			CEventData eventData;
			eventData.m_str_data = _param.m_control_show_text;
			slot_head_widget_notify(kEnumMainPageCurrentMessage, 0, &eventData);

		}break;
		}

	}break;
	}
}



void CMainPage2::set_message_content(const QString &_text)
{
	CDynTextParam param_station_name;
	param_station_name.m_control_this = m_label_scroll_text;
	param_station_name.m_control_type = CDynamicText::kEnumLabel;
	param_station_name.m_control_id = kEnumLabelMessageShow;
	param_station_name.m_control_source_text = _text;
	param_station_name.m_control_font_size = MESSAGE_FONT_SIZE;
	param_station_name.m_control_show_length = MESSAGE_SHOW_LENGTH;

	m_dynamic_text_message->set_text(param_station_name);
}

void CMainPage2::slot_qr_status_timer()
{

	qDebug() << "slot_qr_status_timer";
	m_timer_qr->stop();

	slot_head_widget_notify(kEnumMainPageVerificationRrStatus, kEnumQrStatusNormal, 0);

}
#include <QDebug>
void CMainPage2::slot_main_page2_picture(const CPlayPicture & _data)
{
	//CEventData eventData;
	//qDebug() << "CMainPage2::slot_main_page2_picture";
	//eventData.m_str_data = _data.m_path;
	//slot_head_widget_notify(kEnumMainPageAdvert,0,&eventData);
	if (m_widget_show_picture1)
	{
		m_widget_show_picture1->set_image(_data.m_image);
		m_widget_show_picture1->repaint();
	}

}

void CMainPage2::slot_pushbutton_test()
{
	static int index = 0;

	if (!m_label_test_picture->isVisible())
	{
		m_label_test_picture->show();
		m_label_test_picture->setPixmap(QPixmap("/opt/test_picture/test1.jpg"));
		//m_label_test_picture->setStyleSheet(QString("QLabel{border:0px;border-image:url(%1);}").arg("/opt/test_picture/test1.jpg"));
		index = 1;
	}
	else if (1 == index)
	{
		m_label_test_picture->setPixmap(QPixmap("/opt/test_picture/test2.jpg"));
		//m_label_test_picture->setStyleSheet(QString("QLabel{border:0px;border-image:url(%1);}").arg("/opt/test_picture/test2.jpg"));
		index++;
	}
	else if (2 == index)
	{
		m_label_test_picture->setPixmap(QPixmap("/opt/test_picture/test3.jpg"));
		index++;
	}
	else if (3 == index)
	{
		m_label_test_picture->setPixmap(QPixmap("/opt/test_picture/test4.jpg"));
		index++;
	}
	else if (4 == index)
	{
		m_label_test_picture->setPixmap(QPixmap("/opt/test_picture/test5.jpg"));
		index++;
	}
	else
	{
		m_label_test_picture->hide();
		index++;
	}
}

void CMainPage2::show_failed_interface()
{
	m_label_time->hide();
	if (m_is_demo)
		m_label_time1->show();
	else
		m_label_time1->hide();
	//m_label_charge_type->hide();
	m_label_line_name->show();
	m_label_direction->show();
	m_label_station2->show();

	m_label_ticket->hide(); //票价
	m_label_ticket1->hide(); //金额
	m_label_remaining_sum->hide();  //余额
	m_label_remaining_sum1->hide(); //余额1
	m_label_arrive->hide();//到站
	m_label_arrive1->hide();//到站
	m_label_printf_test->hide();
	m_label_message_type->hide();//刷卡消息类型
	m_widget_message_bg->hide();
	//m_m_label_station_status->hide();


	if (e_Dev_Mode_Slave_Mac == CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE)
	{
		m_m_label_station_status->hide();
		m_label_direction->hide();
		m_label_station2->hide();
	}

}

void CMainPage2::debug_interface(bool _flag)
{
	m_current_is_debug = _flag;
	if (_flag)
	{
		m_m_widget_debug_bg->show();
		m_label_debug->show();

		if (e_Dev_Mode_Slave_Mac == CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE)
		{
			m_widget_line1->show();
			m_widget_line2->show();
			m_label_debug_tip->show();
		}
		else
		{
			m_widget_line1->hide();
			m_widget_line2->hide();
			m_label_debug_tip->hide();
		}

		m_widget_icon->show();
	}
	else
	{
		m_m_widget_debug_bg->hide();
		m_label_debug->hide();
		m_label_debug_tip->hide();

		m_widget_line1->hide();
		m_widget_line2->hide();

		m_widget_icon->hide();
	}

}

bool CMainPage2::get_debug_interface()
{
	return m_current_is_debug;
}

void CMainPage2::createComponent() {

	m_textBrown_slzr = new QTextBrowser(this);
	m_textBrown_slzr->setFixedSize(230, 120);
	m_textBrown_slzr->move(200, 350);
	m_textBrown_slzr->setStyleSheet("QTextBrowser{border:0px;background:transparent;font:15px;color:#ffffff; }");
	m_textBrown_slzr->setAlignment(Qt::AlignLeft);
	m_textBrown_slzr->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置垂直滚动条不可见
	m_textBrown_slzr->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置水平滚动条不可见
	m_textBrown_slzr->setText("邯郸公交2018");



}








