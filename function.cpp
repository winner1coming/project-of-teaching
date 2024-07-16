#include "uidesign.h"

//清空表格数据
void uiDesign::deleteTabelItem(QTableWidget* table){
    // 清空原来的数据
    int rowCount = table->rowCount();
    for (int i = rowCount - 1; i > 0; --i)
    {
        table->removeRow(i);
    }
}

//初始化课表的表格
void uiDesign::InitializeScheduleTable(QTableWidget* table)
{
    for (int i = 1; i <=4; i++)
    {
        for(int j=2; j<=8; j++)
        {
            table->setItem(i, j, new QTableWidgetItem(""));
        }
    }
    table->setColumnWidth(1,97);
    table->setColumnWidth(2,80);
    table->setColumnWidth(3,80);
    table->setColumnWidth(4,80);
    table->setColumnWidth(5,80);
    table->setColumnWidth(6,80);
    table->setColumnWidth(7,80);
    table->setColumnWidth(8,80);
}
//获取本学年已选某课人数
int uiDesign::getCourseChosenAmount(QString courseid)
{
    int amount=0;
    QSqlQuery query;
    if(query.exec(QString("SELECT * FROM stuscores WHERE courseName = '%1' AND courseTime = '2023'").arg(courseid)))
    {
        while(query.next())
        {
            amount++;
        }
    }
    return amount;
}
void uiDesign::addLineToCourseTable(QTableWidget* table,QSqlQuery& query, int& rowCount)
{
    //增加一行
    table->insertRow(rowCount);
    // 处理查询结果
    QString Teacher = query.value(1).toString();
    QString studyDay = query.value(2).toString();
    QString studyTime = query.value(3).toString();
    QString courseName = query.value(4).toString();
    QString college = query.value(5).toString();
    QString studyPlace = query.value(6).toString();
    QString studySemester = query.value(7).toString();
    QString type = query.value(8).toString();
    QString courseID = query.value(9).toString();
    QString stuAmount=QString::number(getCourseChosenAmount(query.value(9).toString()));
    QString credit = query.value(10).toString();
    QString examine = query.value(11).toString();
    QString credithour=query.value(12).toString();


    table->setItem(rowCount, 0, new QTableWidgetItem(courseID));
    table->setItem(rowCount, 1, new QTableWidgetItem(courseName));
    table->setItem(rowCount, 2, new QTableWidgetItem(college));
    table->setItem(rowCount, 3, new QTableWidgetItem(type));
    table->setItem(rowCount, 4, new QTableWidgetItem(credit));
    table->setItem(rowCount, 5, new QTableWidgetItem(credithour));
    table->setItem(rowCount, 6, new QTableWidgetItem(Teacher));
    //table->setItem(rowCount, 6, new QTableWidgetItem(Teacher));
    table->setItem(rowCount, 7, new QTableWidgetItem(stuAmount));
    table->setItem(rowCount, 8, new QTableWidgetItem(examine));
    table->setItem(rowCount, 9, new QTableWidgetItem(studyDay));
    table->setItem(rowCount, 10, new QTableWidgetItem(studyTime));
    table->setItem(rowCount, 11, new QTableWidgetItem(studyPlace));
    table->setItem(rowCount, 12, new QTableWidgetItem(studySemester));
}

//根据条件创建Query用的string
QString uiDesign::FilterCoursesByInput(QString table,QString institute,QString coursename,QString teacher,QString coursetype,QString coursecode,QString week,QString period)
{
    //QString queryString = "SELECT * FROM harrycourse WHERE";
    QString queryString = "SELECT * FROM ";
    queryString+=table;
    queryString+=" WHERE isAvailable='1' AND";
    // 判断并添加筛选条件（Teacher列）
    if (!teacher.isEmpty())
    {
        queryString += QString(" Teacher LIKE '%%1%' AND").arg(teacher);
    }
    if (!week.isEmpty())//（studyDay列）
    {
        queryString += QString(" studyDay = '%1' AND").arg(week);
    }
    if (!period.isEmpty())//（studyTime列）
    {
        if(period=="第一节") period="8：00~10：20";
        else if(period=="第二节") period="10：40~12：10";
        else if(period=="第三节") period="15：30~18：00";
        else if(period=="第四节") period="20：00~22：20";
        queryString += QString(" studyTime = '%1' AND").arg(period);
    }
    if (!coursename.isEmpty())// （courseName列）
    {
        queryString += QString(" courseName LIKE '%%1%' AND").arg(coursename);
    }
    if (!institute.isEmpty())//（college列）
    {
        queryString += QString(" college = '%1' AND").arg(institute);
    }
    if (!coursetype.isEmpty())//（type列）
    {
        queryString += QString(" type = '%1' AND").arg(coursetype);
    }
    if (!coursecode.isEmpty())//（courseID列）
    {
        queryString += QString(" courseID = '%1' AND").arg(coursecode);
    }
    // 去除最后一个 AND
    queryString = queryString.left(queryString.length() - 4);

    return queryString;//返回一个QSqlQuery可执行的query语句
}

