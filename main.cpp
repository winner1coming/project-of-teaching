#include "uidesign.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //添加一个数据库
    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC");
    //设置数据库
    //db.setHostName("192.168.147.122");
    db.setHostName("localhost");
    //设置端口,mysql的端口默认是3306
    db.setPort(3306);
    //设置数据库的用户名
    db.setUserName("root");
    //设置数据库的密码
    //db.setPassword("1593572846");
    db.setPassword("123456");
    //设置数据库的名字
    //db.setDatabaseName("educationSystem");
    db.setDatabaseName("ismysql");
    if(db.open()==false){
        QMessageBox::warning(NULL,"waring",db.lastError().text());
    }

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "uiDesign_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    uiDesign w;
    w.show();
    return a.exec();
}
