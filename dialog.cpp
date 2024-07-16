#include "qboxlayout.h"
#include "qdialog.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "QMessageBox"
class Information : public QDialog
{
public:
    // QString title;
    // QString info;
    Information(QWidget *parent = nullptr, QString title="" ,QString infomation="") : QDialog(parent)
    {
        setWindowTitle(title);
        QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *info = new QLabel(infomation, this);
        QPushButton *checkButton = new QPushButton("确定", this);
        layout->addWidget(info);
        layout->addWidget(checkButton);
        connect(checkButton, &QPushButton::released, this, &QDialog::accept);
        setLayout(layout);
    }
    virtual ~Information(){}
};
