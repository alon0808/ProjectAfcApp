#ifndef COMBOBOXITEM_H
#define COMBOBOXITEM_H

#include <QWidget>
#include <QLabel>

class ComboboxItem : public QWidget
{
    Q_OBJECT
public:
    explicit ComboboxItem(QWidget *parent = 0);
    
signals:
    
public slots:
private:
    void set_text(const QString &_text);

    QLabel *m_label;
};

#endif // COMBOBOXITEM_H
