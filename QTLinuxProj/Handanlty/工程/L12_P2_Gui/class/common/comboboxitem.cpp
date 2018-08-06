#include "comboboxitem.h"

ComboboxItem::ComboboxItem(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(100,33);
#if 0
    m_img = new QLabel(this);
  QPixmap pic(QStringLiteral(":/misc/preference"));
  m_img->setPixmap(pic);
  m_img->setFixedSize(pic.size());
  m_label = new QLabel(this);

  m_layout = new QHBoxLayout(this);
  m_layout->addWidget(m_label);
  m_layout->addStretch();
  m_layout->addWidget(m_img);

  m_layout->setSpacing(5);
  m_layout->setContentsMargins(5, 5, 5, 5);

  setLayout(m_layout);
#endif

    m_label = new QLabel(this);
    m_label->move(0,0);
    m_label->setFixedSize(100,33);

}

void ComboboxItem::set_text(const QString &_text)
{
    m_label->setText(_text);
}




