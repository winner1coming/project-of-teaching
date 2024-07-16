#include "uidesign.h"
//删除课程操作
void uiDesign::deleteCourse(QTableWidget *table,int rowCount)
{
    QPushButton *button=new QPushButton("删除");
    table->setCellWidget(rowCount,14,button);

    connect(button,&QPushButton::clicked,this,[=]()
            {
                QSqlQuery queryb;
                queryb.exec("SELECT Selection FROM myfunction WHERE Selection='0'");
                if(!queryb.next())
                {
                    QMessageBox::StandardButton tip;                    //创建一个对话框
                    tip=QMessageBox::question(nullptr,"删除课程",QString("确认要删除《%1》这门课吗？\n注意！删除后已选择本课程的学生将自动退课！").arg(table->item(rowCount,0)->text()),
                                                QMessageBox::Yes|QMessageBox::No);
                    if(tip==QMessageBox::Yes)
                    {
                        QSqlQuery query1;
                        QSqlQuery query2;
                        QSqlQuery query3;
                        query2.exec(QString("SELECT courseID FROM harrycourse WHERE courseName='%1'").arg(table->item(rowCount,0)->text()));
                        QString id;
                        if(query2.next())
                        {
                            id=query2.value(0).toString();
                        }
                        query1.prepare(QString("UPDATE harrycourse SET isAvailable=:number WHERE courseID=:coursename AND Teacher=:teacher"));
                        query1.bindValue(":number","0");
                        query1.bindValue(":coursename",table->item(rowCount,0)->text());
                        query1.bindValue(":teacher",table->item(rowCount,6)->text());
                        query3.exec(QString("DELETE FROM stuscores WHERE courseName='%1' AND Scores IS NULL").arg(id));
                        if(!query1.exec())
                        {
                            qDebug()<<"系统出错！删除课程失败"<<query1.lastError().text();
                        }
                        else
                        {
                            QMessageBox::information(this,"提示",QString("《%1》课程删除成功！").arg(table->item(rowCount,0)->text()));
                            table->removeRow(rowCount);
                        }
                    }
                }
                else
                {
                    QMessageBox::information(this,"提示","不在选课阶段！不可删除课程！");
                }
            });
}
//查看全校课程信息
void uiDesign::on_Aquery_pushButton_clicked()
{
    int rowCount=AdminPage->ACourseTableWidget->rowCount();
    for(int i=rowCount-1;i>0;--i)
    {
        AdminPage->ACourseTableWidget->removeRow(i);          //清空表格数据
    }
    rowCount=1;
    //获取输入的查询信息
    QString institute=AdminPage->Institute_comboBox->currentText();
    QString coursename=AdminPage->coursename_lineEdit->text();
    QString teacher=AdminPage->teachername_lineEdit->text();
    QString coursetype=AdminPage->type_comboBox->currentText();
    QString coursecode=AdminPage->code_lineEdit->text();
    QString week=AdminPage->week_comboBox->currentText();
    QString period=AdminPage->times_comboBox->currentText();
    //实现查询
    QSqlQuery query;
    QString queryString=FilterCoursesByInput("harrycourse",institute,coursename,teacher,coursetype,coursecode,week,period);
    if (query.exec(queryString))
    {
        while (query.next())
        {
            //在表格中增加一行数据
            addLineToCourseTable(AdminPage->ACourseTableWidget, query, rowCount);
            fixCourse(AdminPage->ACourseTableWidget,rowCount);
            deleteCourse(AdminPage->ACourseTableWidget,rowCount);
            rowCount++;
        }
        AdminPage->ACourseTableWidget->resizeColumnsToContents();        //设置表格格式
        AdminPage->ACourseTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    else
    {
        QMessageBox::warning(this,"课程查询提示","查询失败！");
    }
}

//添加课程的弹窗
class CourseInfoDialog:public QDialog
{
public:

    QLineEdit *courseName=new QLineEdit(this);
    QLineEdit *courseID=new QLineEdit(this);
    QComboBox *courseCollege=new QComboBox(this);
    QComboBox *courseType=new QComboBox(this);
    QLineEdit *courseCredit=new QLineEdit(this);
    QLineEdit *courseTime=new QLineEdit(this);
    QComboBox *courseTeacher=new QComboBox(this);
    QComboBox *courseTest=new QComboBox(this);
    QComboBox *classWeek=new QComboBox(this);
    QComboBox *classTime=new QComboBox(this);
    QComboBox *courseADdress=new QComboBox(this);
    QLineEdit *courseTarget=new QLineEdit(this);
    bool isAdd=false;
    void setisAdd(bool isA)
    {
        isAdd=isA;
    }
    CourseInfoDialog(QWidget *parent=nullptr):QDialog(parent)
    {
        setWindowTitle("管理课程");
        QGridLayout* layout=new QGridLayout(this);
        layout->setSizeConstraint(QLayout::SetFixedSize);
        layout->setSpacing(5);

        QLabel *note=new QLabel("有*为必填项！");
        layout->addWidget(note,0,1,1,1);
        QLabel *note1=new QLabel("学分0~10");
        layout->addWidget(note1,0,2,1,1);
        QLabel *note2=new QLabel("学时0~100");
        layout->addWidget(note2,0,3,1,1);

        QLabel *coursename=new QLabel("*课程名称");
        layout->addWidget(coursename,1,0,1,1);
        layout->addWidget(courseName,1,1,1,1);

        QLabel *courseid=new QLabel("*课程编号");
        layout->addWidget(courseid,1,5,1,1);
        layout->addWidget(courseID,1,6,1,1);

        QLabel *coursecollege=new QLabel("*开课学院");
        courseCollege->addItem("");
        courseCollege->addItem("格兰芬多");
        courseCollege->addItem("斯莱特林");
        courseCollege->addItem("拉文克劳");
        courseCollege->addItem("赫奇帕奇");
        layout->addWidget(coursecollege,1,9,1,1);
        layout->addWidget(courseCollege,1,10,1,1);

        QLabel *coursetype=new QLabel("*课程类别");
        courseType->addItem("");
        courseType->addItem("必修");
        courseType->addItem("选修");
        courseType->addItem("专业课");
        courseType->addItem("预习课程");
        layout->addWidget(coursetype,2,0,1,1);
        layout->addWidget(courseType,2,1,1,1);

        QLabel *coursecredit=new QLabel("*学分");
        layout->addWidget(coursecredit,2,5,1,1);
        layout->addWidget(courseCredit,2,6,1,1);

        QLabel *coursetime=new QLabel("*学时");
        layout->addWidget(coursetime,2,9,1,1);
        layout->addWidget(courseTime,2,10,1,1);

        QLabel *courseteacher=new QLabel("*主讲教师");
        layout->addWidget(courseteacher,3,0,1,1);
        courseTeacher->addItem("");
        QSqlQuery querystr;
        //查找本校所以老师并将他们的名字放入到选择框中
        QString str=QString("SELECT TeacherName FROM teacher");
        querystr.exec(str);
        while(querystr.next())
        {
            QString Str=querystr.value(0).toString();
            courseTeacher->addItem(Str);
        }

        QLabel *coursetest=new QLabel("*考试方式");
        layout->addWidget(coursetest,3,5,1,1);
        courseTest->addItem("");
        courseTest->addItem("笔试");
        courseTest->addItem("实践考试");
        courseTest->addItem("课程论文");
        courseTest->addItem("笔试+实践考试");
        courseTest->addItem("O.W.L");
        courseTest->addItem("N.E.W.T");
        courseTest->addItem("O.W.L+N.E.W.T");
        layout->addWidget(courseTest,3,6,1,1);

        QLabel *classweek=new QLabel("*上课星期");
        classWeek->addItem("");
        classWeek->addItem("周一");
        classWeek->addItem("周二");
        classWeek->addItem("周三");
        classWeek->addItem("周四");
        classWeek->addItem("周五");
        layout->addWidget(classweek,3,9,1,1);
        layout->addWidget(classWeek,3,10,1,1);

        QLabel *classtime=new QLabel("*上课时间");
        classTime->addItem("");
        classTime->addItem("8：00~10：20");
        classTime->addItem("10：40~12：10");
        classTime->addItem("15：30~18：00");
        classTime->addItem("20：00~22：20");
        layout->addWidget(classtime,4,0,1,1);
        layout->addWidget(classTime,4,1,1,1);

        QLabel *courseaddress=new QLabel("*上课地点");
        layout->addWidget(courseaddress,4,5,1,1);
        courseADdress->addItem(" ");
        courseADdress->addItem("霍格沃兹城堡大厅");
        courseADdress->addItem("海格的小屋外或禁林");
        courseADdress->addItem("霍格沃兹城堡二楼一间教室");
        courseADdress->addItem("霍格沃兹五楼讲堂");
        courseADdress->addItem("霍格沃兹三楼播报室");
        courseADdress->addItem("霍格沃兹城堡二楼实验室");
        courseADdress->addItem("霍格沃兹礼堂");
        courseADdress->addItem("霍格沃兹大教堂");
        courseADdress->addItem("霍格沃兹大讲堂");
        courseADdress->addItem("霍格沃兹城堡后花园");
        courseADdress->addItem("霍格沃兹三楼的一间教室");
        courseADdress->addItem("霍格沃兹城堡后面的温室");
        courseADdress->addItem("霍格沃兹城堡楼梯口旁的教室");
        courseADdress->addItem("霍格沃兹城堡地下魔药课教室");
        courseADdress->addItem("霍格沃兹城堡四层的魔咒课教室");
        courseADdress->addItem("霍格沃兹城堡四层的黑魔法防御术教室");
        courseADdress->addItem("霍格沃兹城堡二楼的魔法史教室");
        courseADdress->addItem("霍格沃兹城堡前禁林边的草坪");
        courseADdress->addItem("霍格沃兹城堡北塔楼的一个教室");
        layout->addWidget(courseADdress,4,6,1,1);

        QLabel *coursetarget=new QLabel("*修读对象");
        layout->addWidget(coursetarget,4,9,1,1);
        layout->addWidget(courseTarget,4,10,1,1);

        QPushButton *okButton=new QPushButton("确定",this);
        QPushButton *closeButton=new QPushButton("取消",this);

        connect(okButton,&QPushButton::clicked,this,[=]()
                {
                    if(!courseName->text().isEmpty()&&!courseID->text().isEmpty()&&!courseCollege->currentText().isEmpty()&&
                        !courseType->currentText().isEmpty()&&!courseCredit->text().isEmpty()&&!courseTime->text().isEmpty()
                        &&!courseTeacher->currentText().isEmpty()&&!courseTest->currentText().isEmpty()&&!classWeek->currentText().isEmpty()
                        &&!classTime->currentText().isEmpty()&&!courseADdress->currentText().isEmpty()&&!courseTarget->text().isEmpty())
                    {
                        //选修：年级的正则表达式判断
                        QRegularExpression *target=new QRegularExpression("^(?:(([一二三四五六七]+)、([一二三四五六七]+年级))|([一二三四五六七]+年级)|"
                                                                            "(([一二三四五六七]+)、([一二三四五六七]+)、([一二三四五六七]+年级))|"
                                                                            "(([一二三四五六七]+)、([一二三四五六七]+)、([一二三四五六七]+)、"
                                                                            "([一二三四五六七]+年级))|"
                                                                            "(([一二三四五六七]+)、([一二三四五六七]+)、([一二三四五六七]+)"
                                                                            "、([一二三四五六七]+)、([一二三四五六七]+年级))|"
                                                                            "(([一二三四五六七]+)、([一二三四五六七]+)、([一二三四五六七]+)、"
                                                                            "([一二三四五六七]+)、([一二三四五六七]+)、([一二三四五六七]+年级))|"
                                                                            "(([一二三四五六七]+)、([一二三四五六七]+)、([一二三四五六七]+)、"
                                                                            "([一二三四五六七]+)、([一二三四五六七]+)、([一二三四五六七]+)、"
                                                                            "([一二三四五六七]+年级)))$");
                        //必修只允许同一个年级一起上
                        QRegularExpression *target1=new QRegularExpression("^(?:([一二三四五六七]+年级))$");
                        QRegularExpressionMatch match1=target->match(courseTarget->text());
                        QRegularExpressionMatch match2=target1->match(courseTarget->text());

                        QSqlQuery query1;
                        //判断老师是否在规定时间内已经有别的课程了
                        QString findflag=QString("SELECT Teacher FROM harrycourse WHERE studyDay='%1' AND studyTime='%2' AND isAvailable='1' AND"
                                                   " Teacher='%3'").arg(classWeek->currentText(),classTime->currentText(),courseTeacher->currentText());
                        //如果添加老师是从整张数据表中查找
                        //如果是修改老师信息则要在除了原本的课程信息外查找
                        if(!isAdd)
                        {
                            findflag+=QString(" AND courseID!='%1'").arg(courseID->text());
                        }
                        query1.exec(findflag);
                        //判断是否为选修课程，为了后面判断年级的个数
                        bool ischoose=false;
                        if(courseType->currentText()=="选修")
                        {
                            ischoose=true;
                        }
                        QSqlQuery querymiss;
                        QString Miss=QString("SELECT * FROM harrycourse WHERE studyDay='%1' AND studyTime='%2' AND"
                                               " studySemester='%3' AND isAvailable='1' AND"
                                               " type!='选修'").arg(classWeek->currentText(),classTime->currentText(),courseTarget->text());
                        //如果进行修改操作，还要判断加上找不到自己的条件
                        if(!isAdd)
                        {
                            Miss+=QString(" AND courseID!='%1'").arg(courseID->text());

                        }
                        querymiss.exec(Miss);
                        //查找所以课程的id，保证课程id不重复
                        QSqlQuery queryid;
                        queryid.exec(QString("SELECT * FROM harrycourse WHERE courseID='%1'").arg(courseID->text()));
                        if(courseID->text().length()!=6)
                        {
                            QMessageBox::information(this,"提示","输入课程号有误！课程号长度规定为6！");
                        }
                        else if(isAdd&&queryid.next())
                        {
                            QMessageBox::information(this,"提示","该课程编号已存在！请重新设计课程编号！");
                        }
                        else if(courseCredit->text().toInt()<0||courseCredit->text().toInt()>10)
                        {
                            QMessageBox::critical(this,"提示","学分超出范围，请修改输入！");
                        }
                        else if(courseTime->text().toInt()<0||courseTime->text().toInt()>100)
                        {
                            QMessageBox::critical(this,"提示","学时超出范围，请修改输入！");
                        }
                        else if(query1.next())
                        {
                            QMessageBox::critical(this,"提示","该教师在规定时间段内已经有课程安排！\n请重新选择教师！");
                        }
                        else if(!match1.hasMatch())
                        {
                            QMessageBox::information(this,"提示","修读对象输入格式错误！\n说明：修读对象从一到七年级\n单个年级：一年级  多个年级：一、二年级");
                        }
                        else if(!ischoose&&!match2.hasMatch())
                        {
                            QMessageBox::information(this,"提示","修读对象输入格式错误！\n说明："
                                                                   "修读对象从一到七年级且除选修课外只支持一个年级选课！\n单个年级：一年级");
                        }
                        else if(querymiss.next())
                        {
                            QMessageBox::information(this,"提示","所选课程时间与现有该年级必修课/专业课/预习课程冲突！");
                        }
                        else
                        {
                            if(isAdd)
                            {
                                QMessageBox::information(this,"提示","添加课程信息成功！");
                            }
                            else
                            {
                                QMessageBox::information(this,"提示","修改课程信息成功！");
                            }
                            accept();
                        }
                    }
                    else
                    {
                        QMessageBox::critical(this,"错误","*为必填项，请完善!");
                    }
                });
        connect(closeButton,&QPushButton::clicked,this,&QDialog::reject);
        layout->addWidget(courseTeacher,3,1,1,1);
        QHBoxLayout *button=new QHBoxLayout();
        button->addWidget(okButton);
        button->addWidget(closeButton);
        layout->addLayout(button,5,5,1,2);
        setLayout(layout);
    }

    virtual ~CourseInfoDialog(){};

    void addCourseInfo(QString name,QString id,QString college,QString type,
                       QString credit,QString coursetime,QString courseteacher,QString test,
                       QString week,QString classtime,QString address,QString target)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO harrycourse(isAvailable,Teacher,studyDay,studyTime,courseName,"
                      "college,studyPlace,studySemester,type,courseID,credit,examinationForm,creditHour) "
                      "VALUES(1,:teacher,:week,:classtime,:name,:college,:address,:target,:type,:id,:credit,:test,:coursetime)");
        query.bindValue(":teacher",courseteacher);
        query.bindValue(":week",week);
        query.bindValue(":classtime",classtime);
        query.bindValue(":name",name);
        query.bindValue(":college",college);
        query.bindValue(":address",address);
        query.bindValue(":target",target);
        query.bindValue(":type",type);
        query.bindValue(":id",id);
        query.bindValue(":credit",credit);
        query.bindValue(":test",test);
        query.bindValue(":coursetime",coursetime);
        if(!query.exec())
        {
            qDebug() << "系统问题！添加失败:" << query.lastError().text();
        }
    }

    void fixCourseInfo(QString name,QString id,QString college,QString type,QString credit,QString coursetime,QString courseteacher,QString test,QString week,QString classtime,QString address,QString target)
    {
        QSqlQuery query;
        query.prepare(QString("UPDATE harrycourse SET Teacher=:teacher,studyDay=:week,"
                              "studyTime=:classtime,courseName=:name,college=:college,studyPlace=:address,"
                              "studySemester=:target,type=:type,credit=:credit,examinationForm=:test,creditHour=:coursetime WHERE courseID='%1'").arg(id));
        query.bindValue(":teacher",courseteacher);
        query.bindValue(":week",week);
        query.bindValue(":classtime",classtime);
        query.bindValue(":name",name);
        query.bindValue(":college",college);
        query.bindValue(":address",address);
        query.bindValue(":target",target);
        query.bindValue(":type",type);
        query.bindValue(":credit",credit);
        query.bindValue(":test",test);
        query.bindValue(":coursetime",coursetime);
        if(!query.exec())
        {
            qDebug() << "系统问题！修改失败:" << query.lastError().text();
        }
    }
};

//表示信息
void showCourseInfo(QTableWidget *table,QString name,QString id,QString college,QString type,
                    QString credit,QString coursetime,QString courseteacher,QString test,
                    QString week,QString classtime,QString address,QString target,int row)
{
    table->setItem(row,0,new QTableWidgetItem(id));
    table->setItem(row,1,new QTableWidgetItem(name));
    table->setItem(row,2,new QTableWidgetItem(college));
    table->setItem(row,3,new QTableWidgetItem(type));
    table->setItem(row,4,new QTableWidgetItem(credit));
    table->setItem(row,5,new QTableWidgetItem(coursetime));
    table->setItem(row,6,new QTableWidgetItem(courseteacher));
    table->setItem(row,7,new QTableWidgetItem("0"));
    table->setItem(row,8,new QTableWidgetItem(test));
    table->setItem(row,9,new QTableWidgetItem(week));
    table->setItem(row,10,new QTableWidgetItem(classtime));
    table->setItem(row,11,new QTableWidgetItem(address));
    table->setItem(row,12,new QTableWidgetItem(target));
}
//修改课程信息
void uiDesign::fixCourse(QTableWidget *table,int row)
{
    QPushButton *fixbutton=new QPushButton("修改");
    table->setCellWidget(row,13,fixbutton);
    connect(fixbutton,&QPushButton::clicked,this,[=]()
            {
                QMessageBox::StandardButton tip;                    //创建一个对话框
                tip=QMessageBox::question(nullptr,"修改课程",QString("确认要修改《%1》这门课的相关信息吗？").arg(table->item(row,0)->text()),
                                            QMessageBox::Yes|QMessageBox::No);
                if(tip==QMessageBox::Yes)
                {
                    CourseInfoDialog dialog(nullptr);
                    dialog.setisAdd(false);
                    dialog.courseID->setText(table->item(row,0)->text());
                    dialog.courseName->setText(table->item(row,1)->text());
                    dialog.courseCollege->setCurrentText(table->item(row,2)->text());
                    dialog.courseType->setCurrentText(table->item(row,3)->text());
                    dialog.courseCredit->setText(table->item(row,4)->text());
                    dialog.courseTime->setText(table->item(row,5)->text());
                    dialog.courseTeacher->setCurrentText(table->item(row,6)->text());
                    dialog.courseTest->setCurrentText(table->item(row,8)->text());
                    dialog.classWeek->setCurrentText(table->item(row,9)->text());
                    dialog.classTime->setCurrentText(table->item(row,10)->text());
                    dialog.courseADdress->setCurrentText(table->item(row,11)->text());
                    dialog.courseTarget->setText(table->item(row,12)->text());
                    dialog.courseID->setReadOnly(true);
                    if(dialog.exec()==QDialog::Accepted)
                    {
                        QString name=dialog.courseName->text();
                        QString id=dialog.courseID->text();
                        QString college=dialog.courseCollege->currentText();
                        QString type=dialog.courseType->currentText();
                        QString credit=dialog.courseCredit->text();
                        QString coursetime=dialog.courseTime->text();
                        QString courseteacher=dialog.courseTeacher->currentText();
                        QString test=dialog.courseTest->currentText();
                        QString week=dialog.classWeek->currentText();
                        QString classtime=dialog.classTime->currentText();
                        QString address=dialog.courseADdress->currentText();
                        QString target=dialog.courseTarget->text();
                        dialog.fixCourseInfo(name,id,college,type,credit,coursetime,courseteacher,test,week,classtime,address,target);
                        showCourseInfo(AdminPage->ACourseTableWidget,name,id,college,type,credit,coursetime,courseteacher,test,week,classtime,address,target,row);
                        fixCourse(AdminPage->ACourseTableWidget,row);
                        deleteCourse(AdminPage->ACourseTableWidget,row);
                    }
                }

            });
}


//添加课程
void uiDesign::on_pushButton_clicked()
{
    CourseInfoDialog dialog(nullptr);
    dialog.setisAdd(true);
    if(dialog.exec()==QDialog::Accepted)
    {
        QString name=dialog.courseName->text();
        QString id=dialog.courseID->text();
        QString college=dialog.courseCollege->currentText();
        QString type=dialog.courseType->currentText();
        QString credit=dialog.courseCredit->text();
        QString coursetime=dialog.courseTime->text();
        QString courseteacher=dialog.courseTeacher->currentText();
        QString test=dialog.courseTest->currentText();
        QString week=dialog.classWeek->currentText();
        QString classtime=dialog.classTime->currentText();
        QString address=dialog.courseADdress->currentText();
        QString target=dialog.courseTarget->text();
        dialog.addCourseInfo(name,id,college,type,credit,coursetime,courseteacher,test,week,classtime,address,target);
        int row=AdminPage->ACourseTableWidget->rowCount();
        AdminPage->ACourseTableWidget->insertRow(row);
        showCourseInfo(AdminPage->ACourseTableWidget,name,id,college,type,credit,coursetime,
                       courseteacher,test,week,classtime,address,target,row);
        fixCourse(AdminPage->ACourseTableWidget,row);
        deleteCourse(AdminPage->ACourseTableWidget,row);
    }
}


//用于方便输入学生信息，方便重用
void uiDesign::StuMass(QTableWidget *table,QSqlQuery &queryStu)
{
    QString StuName=queryStu.value(1).toString();
    QString StuID=queryStu.value(0).toString();
    QString Academy=queryStu.value(5).toString();

    QPushButton *button=new QPushButton("重置");
    int row=table->rowCount();
    table->insertRow(row);
    table->setItem(row,1,new QTableWidgetItem(StuID));
    table->setItem(row,0,new QTableWidgetItem(StuName));
    table->setItem(row,2,new QTableWidgetItem(Academy));
    table->setCellWidget(row,3,button);

    connect(button,&QPushButton::clicked,this,[=]()
            {
                QMessageBox::StandardButton tip;                    //创建一个对话框
                tip=QMessageBox::question(nullptr,"重置密码",QString("确认要重置%1的密码吗？").arg(table->item(row,1)->text()),
                                            QMessageBox::Yes|QMessageBox::No);
                if(tip==QMessageBox::Yes)
                {
                    QSqlQuery query1;
                    QSqlQuery query;
                    query1.exec(QString("SELECT Passkey FROM student WHERE StuID='%1'").arg(table->item(row,0)->text()));
                    if(query1.next())
                    {
                        QString Originalpasskey=query1.value(0).toString();
                        if(Originalpasskey==table->item(row,0)->text())
                        {
                            QMessageBox::information(table,"错误",QString("该学生密码为初始密码！"));
                        }
                        else
                        {
                            query.prepare(QString("UPDATE student SET Passkey=:id WHERE StuID='%1'").arg(table->item(row,0)->text()));
                            query.bindValue(":id",table->item(row,0)->text());
                            if(!query.exec())
                            {
                                qDebug() << "系统出错，重置密码失败！——" << query.lastError().text();
                            }
                            else
                            {
                                QMessageBox::information(table,"提示",QString("重置%1学生密码成功！\n已重置为初始密码！").arg(table->item(row,1)->text()));
                            }
                        }
                    }
                }
            });
}
//用于方便输入老师信息，方便重用
void uiDesign::TerMass(QTableWidget *table,QSqlQuery &queryTer)
{
    QString TerName=queryTer.value(1).toString();
    QString TerID=queryTer.value(0).toString();
    QString College=queryTer.value(3).toString();

    QPushButton *button=new QPushButton("重置");
    int row=table->rowCount();
    table->insertRow(row);
    table->setItem(row,1,new QTableWidgetItem(TerID));
    table->setItem(row,0,new QTableWidgetItem(TerName));
    table->setItem(row,2,new QTableWidgetItem(College));
    table->setCellWidget(row,3,button);
    connect(button,&QPushButton::clicked,this,[=]()
            {
                QMessageBox::StandardButton tip;                    //创建一个对话框
                tip=QMessageBox::question(nullptr,"重置密码",QString("确认要重置%1的密码吗？").arg(table->item(row,1)->text()),
                                            QMessageBox::Yes|QMessageBox::No);
                if(tip==QMessageBox::Yes)
                {
                    QSqlQuery query;
                    QSqlQuery query1;
                    query1.exec(QString("SELECT TPasskey FROM teacher WHERE TeacherID='%1'").arg(table->item(row,0)->text()));
                    if(query1.next())
                    {
                        QString Originalpasskey=query1.value(0).toString();
                        if(Originalpasskey==table->item(row,0)->text())
                        {
                            QMessageBox::information(table,"错误",QString("该老师密码为初始密码！"));
                        }
                        else
                        {
                            query.prepare(QString("UPDATE teacher SET TPasskey=:id WHERE TeacherID='%1'").arg(table->item(row,0)->text()));
                            query.bindValue(":id",table->item(row,0)->text());
                            if(!query.exec())
                            {
                                qDebug() << "系统出错，重置密码失败！——" << query.lastError().text();
                            }
                            else
                            {
                                QMessageBox::information(table,"提示",QString("重置%1老师密码成功！\n已重置为初始密码！").arg(table->item(row,1)->text()));
                            }
                        }
                    }
                }
            });
}

//密码重置页面学生信息的查询函数
QString JudQuery1(QString table,QString name,QString ID,QString institue)
{
    QString QueryString="SELECT * FROM ";
    QueryString+=table;
    QueryString+=" WHERE ";
    if(!name.isEmpty())
    {
        QueryString+=QString("StuName='%1' AND ").arg(name);
    }

    if(!ID.isEmpty())
    {
        QueryString+=QString("StuID='%1' AND ").arg(ID);
    }

    if(!institue.isEmpty())
    {
        QueryString +=QString("Academy='%1' AND ").arg(institue);
    }

    QueryString=QueryString.left(QueryString.length()-4);
    QueryString+=" ORDER BY StuID ASC";
    return QueryString;
}

//密码重置页面老师信息的查询函数
QString JudQuery2(QString table,QString name,QString ID,QString institue)
{
    QString QueryString="SELECT * FROM ";
    QueryString+=table;
    QueryString+=" WHERE ";
    if(!name.isEmpty())
    {
        QueryString+=QString("TeacherName='%1' AND ").arg(name);
    }
    if(!ID.isEmpty())
    {
        QueryString+=QString("TeacherID='%1' AND ").arg(ID);
    }

    if(!institue.isEmpty())
    {
        QueryString +=QString("college='%1' AND ").arg(institue);
    }

    QueryString=QueryString.left(QueryString.length()-4);
    return QueryString;
}

//老师信息和学生信息查询输出——密码设置页面
void uiDesign::on_query_pushButton1_clicked()
{
    int rowCount=AdminPage->AChangetableWidget->rowCount();
    for(int i=rowCount-1;i>0;--i)
    {
        AdminPage->AChangetableWidget->removeRow(i);                //清空表格信息
    }
    rowCount=1;
    //获取界面查询输入的信息
    QString name=AdminPage->name_lineEdit->text();
    QString ID=AdminPage->id_lineEdit->text();
    QString institue=AdminPage->college_comboBox->currentText();
    QString identity=AdminPage->identity_comboBox->currentText();
    QSqlQuery query1;
    QString QueryString;
    //查询开始
    if(identity=="老师")               //身份判断
    {
        QueryString=JudQuery2("teacher",name,ID,institue);          //形成老师信息查询语句
        QSqlQuery query2;
        if(query2.exec(QueryString))                                //进行查询
        {
            while(query2.next())
            {
                TerMass(AdminPage->AChangetableWidget,query2);       //用设计好的函数输出到表格中
            }
        }
    }
    else if(identity=="学生")                                        //学生信息查询与上同理
    {
        QueryString=JudQuery1("student",name,ID,institue);
        if(query1.exec(QueryString))
        {
            while(query1.next())
            {
                StuMass(AdminPage->AChangetableWidget,query1);
            }
        }
    }
    else                                                                  //如果没有选择身份，那么信息全部输出
    {
        QueryString=JudQuery1("student",name,ID,institue);
        if(query1.exec(QueryString))
        {
            while(query1.next())
            {
                StuMass(AdminPage->AChangetableWidget,query1);
            }
        }

        QueryString=JudQuery2("teacher",name,ID,institue);
        QSqlQuery query2;
        if(query2.exec(QueryString))
        {
            while(query2.next())
            {
                TerMass(AdminPage->AChangetableWidget,query2);
            }
        }

    }
}

//计算学生平均绩点
QString CalculateAverageScore(QString stuid)
{
    float totalScore=0;
    float totalcredit=0;

    //在stuscore里一个个找出所有已选课程
    QSqlQuery querymycourse;
    QString querymycourseString=QString("SELECT * FROM stuscores WHERE StuID = '%1' ORDER BY courseName DESC").arg(stuid);
    //SELECT * FROM your_table ORDER BY your_primary_key_column ASC;

    if(querymycourse.exec(querymycourseString))
    {
        while(querymycourse.next())
        {
            QString courseid=querymycourse.value(1).toString();//当前已有课程的courseid
            QString originalscores=querymycourse.value(2).toString();
            QString gradePoints=querymycourse.value(3).toString();
            QString coursegrade=querymycourse.value(4).toString();
            if(gradePoints.isEmpty()||originalscores.isEmpty())
            {
                continue;
            }
            QSqlQuery query;
            QString queryString=QString("SELECT * FROM harrycourse WHERE courseID = '%1'").arg(courseid);
            if (query.exec(queryString))
            {
                while(query.next())
                {

                    QString credit = query.value(10).toString();

                    //对平均绩点的操作
                    totalScore+=(gradePoints.toFloat())*credit.toFloat();
                    totalcredit+=credit.toFloat();
                }

            }
            else
            {
                QMessageBox::warning(nullptr,"成绩查询提示","查询失败！");
            }

        }
        if(totalcredit!=0)
        {
            //计算平均绩点
            float averagescore=totalScore/totalcredit;
            QString scoreString = QString::number(static_cast<double>(averagescore), 'f', 4);
            return scoreString;
        }
    }
}

//学生信息管理之查询功能
void uiDesign::on_pushButton_6_clicked()
{
    int rowCount=AdminPage->AStudenttableWidget->rowCount();
    for(int i=rowCount-1;i>0;--i)
    {
        AdminPage->AStudenttableWidget->removeRow(i);
    }
    rowCount=1;
    //清空查询框内容
    QString name=AdminPage->lineEdit_5->text();
    QString ID=AdminPage->lineEdit_6->text();
    QString institue=AdminPage->comboBox->currentText();
    //查询学生信息并写入表格
    QSqlQuery query;
    QString QueryString;
    QueryString=JudQuery1("student",name,ID,institue);
    if(query.exec(QueryString))
    {
        while(query.next())
        {
            QString StuName=query.value(0).toString();
            QString StuID=query.value(1).toString();
            QString Sex=query.value(2).toString();
            QString Birthday=query.value(4).toString();
            QString Phone=query.value(3).toString();
            QString Academy=query.value(5).toString();
            QString Grate=query.value(7).toString();
            QString Credit=CalculateAverageScore(StuID);

            int row =AdminPage->AStudenttableWidget->rowCount();
            AdminPage->AStudenttableWidget->insertRow(row);
            AdminPage->AStudenttableWidget->setItem(row,1,new QTableWidgetItem(StuName));
            AdminPage->AStudenttableWidget->setItem(row,0,new QTableWidgetItem(StuID));
            AdminPage->AStudenttableWidget->setItem(row,2,new QTableWidgetItem(Sex));
            AdminPage->AStudenttableWidget->setItem(row,3,new QTableWidgetItem(Birthday));
            AdminPage->AStudenttableWidget->setItem(row,4,new QTableWidgetItem(Phone));
            AdminPage->AStudenttableWidget->setItem(row,5,new QTableWidgetItem(Academy));
            AdminPage->AStudenttableWidget->setItem(row,6,new QTableWidgetItem(Grate));
            AdminPage->AStudenttableWidget->setItem(row,7,new QTableWidgetItem(Credit));
            fixStuInfo(AdminPage->AStudenttableWidget,row);
            deleteStu(AdminPage->AStudenttableWidget,row);
        }
    }
}

//学生信息添加弹框
class StuInfoDialog:public QDialog{
public:
    bool Add;
    void isAdd(bool A)
    {
        Add=A;
    }
    QLineEdit *idEdit=new QLineEdit(this);
    QLineEdit *nameEdit=new QLineEdit(this);
    QComboBox *sexEdit=new QComboBox(this);
    QLineEdit *birthEdit=new QLineEdit(this);
    QLineEdit *phoneEdit=new QLineEdit(this);
    QComboBox *academyEdit=new QComboBox(this);
    QComboBox *gradeEdit=new QComboBox(this);
    StuInfoDialog(QWidget*parent=nullptr):QDialog(parent)   //构造函数就直接形成弹框
    {
        setWindowTitle("学生信息管理");
        QGridLayout *layout=new QGridLayout (this);
        layout->setSizeConstraint(QLayout::SetFixedSize);
        QLabel *note=new QLabel("有*为必填项",this);
        layout->addWidget(note,0,0,1,1);

        QLabel*stuid=new QLabel("*学号",this);
        layout->addWidget(stuid,1,0,1,1);
        layout->addWidget(idEdit,1,1,1,1);

        QLabel*stuname=new QLabel("*姓名",this);
        layout->addWidget(stuname,1,5,1,1);
        layout->addWidget(nameEdit,1,6,1,1);

        QLabel *stusex=new QLabel("*性别",this);
        sexEdit->addItem("");
        sexEdit->addItem("男");
        sexEdit->addItem("女");
        layout->addWidget(stusex,2,0,1,1);
        layout->addWidget(sexEdit,2,1,1,1);

        QLabel *stubirth=new QLabel("*生日",this);
        layout->addWidget(stubirth,2,5,1,1);
        layout->addWidget(birthEdit,2,6,1,1);

        QLabel *stuphone=new QLabel("*联系方式",this);
        layout->addWidget(stuphone,3,0,1,1);
        layout->addWidget(phoneEdit,3,1,1,1);

        QLabel *stuacademy=new QLabel("*学院",this);
        academyEdit->addItem("");
        academyEdit->addItem("格兰芬多");
        academyEdit->addItem("斯莱特林");
        academyEdit->addItem("拉文克劳");
        academyEdit->addItem("赫奇帕奇");
        layout->addWidget(stuacademy,3,5,1,1);
        layout->addWidget(academyEdit,3,6,1,1);

        QLabel *stugrade=new QLabel("*年级",this);
        gradeEdit->addItem("");
        gradeEdit->addItem("一年级");
        gradeEdit->addItem("二年级");
        gradeEdit->addItem("三年级");
        gradeEdit->addItem("四年级");
        gradeEdit->addItem("五年级");
        gradeEdit->addItem("六年级");
        gradeEdit->addItem("七年级");
        layout->addWidget(stugrade,4,0,1,1);
        layout->addWidget(gradeEdit,4,1,1,1);

        QLabel *note1=new QLabel("密码初始为学号！",this);
        layout->addWidget(note1,5,0,1,3);

        QPushButton *okButton=new QPushButton("确定",this);
        QPushButton *noButton=new QPushButton("取消",this);
        //ok按钮和点击连接
        connect(okButton,&QPushButton::clicked,this,[=](){

            if(!idEdit->text().isEmpty()&&!nameEdit->text().isEmpty()&&!sexEdit->currentText().isEmpty()&&!birthEdit->text().isEmpty()&&!phoneEdit->text().isEmpty()&&!academyEdit->currentText().isEmpty()
                &&!gradeEdit->currentText().isEmpty())
            {
                QRegularExpression dateType(R"(\d{1,2}/\d{1,2}/\d{4})");
                QRegularExpressionMatch match=dateType.match(birthEdit->text());
                QSqlQuery queryid;
                queryid.exec(QString("SELECT * FROM student WHERE StuID='%1'").arg(idEdit->text()));
                if(idEdit->text().length()!=6)
                {
                    QMessageBox::information(this,"提示","学号输入错误！学号长度应为6！");
                }
                else if(Add&&queryid.next())
                {
                    QMessageBox::information(this,"提示","学号输入错误！\n该学号已存在，请重新设计！");
                }
                else if(!match.hasMatch())
                {
                    QMessageBox::information(this,"提示","生日格式输入错误！请重试！\n参考格式：日/月/年 02/12/2004");
                }
                else if(phoneEdit->text().length()!=11)
                {
                    QMessageBox::information(this,"提示","联系方式错误！请输入正确的电话号码！");
                }
                else
                {
                    if(Add)
                    {
                        QMessageBox::information(this,"提示","添加学生信息成功！");
                    }
                    else
                    {
                        QMessageBox::information(this,"提示","修改学生信息成功！");
                    }
                    accept();
                }
            }
            else
            {
                QMessageBox::critical(this,"错误","*为必填项，请完善！");
            }

        });
        connect(noButton,&QPushButton::clicked,this,&QDialog::reject);
        QHBoxLayout* buttonLayout=new QHBoxLayout();
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(noButton);
        layout->addLayout(buttonLayout,6,3,1,2);
        setLayout(layout);
    }
    virtual ~StuInfoDialog(){};
    void fixStuInfo(QString name,QString id,QString sex,QString birth,QString phone,QString academy,QString grade)
    {
        QSqlQuery query;
        query.prepare(QString("UPDATE student SET StuName=:name,Sex=:sex,Phone=:phone,Academy=:academy,"
                              "Grade=:grade, WHERE StuID='%1'").arg(id));
        query.bindValue(":name",name);
        query.bindValue(":sex",sex);
        query.bindValue(":birth",birth);
        query.bindValue(":phone",phone);
        query.bindValue(":academy",academy);
        query.bindValue(":grade",grade);
        if(!query.exec())
        {
            qDebug()<<"系统错误！修改信息失败！";
        }
    }
};

//表示学生信息
void showStuInfo(QTableWidget *table,QString name,QString id,QString sex,QString birth,QString phone,QString academy,QString grade,QString credit,int row)
{
    table->setItem(row,0,new QTableWidgetItem(id));
    table->setItem(row,1,new QTableWidgetItem(name));
    table->setItem(row,2,new QTableWidgetItem(sex));
    table->setItem(row,3,new QTableWidgetItem(birth));
    table->setItem(row,4,new QTableWidgetItem(phone));
    table->setItem(row,5,new QTableWidgetItem(academy));
    table->setItem(row,6,new QTableWidgetItem(grade));
    table->setItem(row,7,new QTableWidgetItem(credit));
}


//添加学生信息
void uiDesign::on_add_pushButton_clicked()
{
    StuInfoDialog dialog(nullptr);
    if(dialog.exec()==QDialog::Accepted)
    {
        dialog.isAdd(true);
        QString name=dialog.nameEdit->text();
        QString id=dialog.idEdit->text();
        QString sex=dialog.sexEdit->currentText();
        QString birth=dialog.birthEdit->text();
        QString phone=dialog.phoneEdit->text();
        QString academy=dialog.academyEdit->currentText();
        QString grade=dialog.gradeEdit->currentText();
        QString passkey=dialog.idEdit->text();
        QString credit="0";
        QSqlQuery query;
        query.prepare("INSERT INTO student(StuName,StuID,Sex,Phone,Birthday,Academy,Passkey,Grade) VALUES(:name,:id,:sex,:phone,:birthday,:academy,:passkey,:grade)");
        query.bindValue(":name",name);
        query.bindValue(":id",id);
        query.bindValue(":sex",sex);
        query.bindValue(":phone",phone);
        query.bindValue(":birthday",birth);
        query.bindValue(":academy",academy);
        query.bindValue(":passkey",passkey);
        query.bindValue(":grade",grade);
        if(!query.exec())
        {
            qDebug() << "系统出错！添加失败:" << query.lastError().text();
        }
        int row=AdminPage->AStudenttableWidget->rowCount();
        AdminPage->AStudenttableWidget->insertRow(row);
        showStuInfo(AdminPage->AStudenttableWidget,name,id,sex,phone,birth,academy,grade,credit,row);
        fixStuInfo(AdminPage->AStudenttableWidget,row);
        deleteStu(AdminPage->AStudenttableWidget,row);
    }

}

//修改学生信息
void uiDesign::fixStuInfo(QTableWidget *table,int row)
{
    QPushButton *fixbutton=new QPushButton("修改");
    table->setCellWidget(row,8,fixbutton);
    connect(fixbutton,&QPushButton::clicked,this,[=](){
        QMessageBox::StandardButton tip;
        tip=QMessageBox::question(nullptr,"修改学生信息",QString("确认要修改学生：%1 %2的信息吗？").arg(table->item(row,0)->text(),table->item(row,1)->text()),
                                    QMessageBox::Yes|QMessageBox::No);
        if(tip==QMessageBox::Yes)
        {
            StuInfoDialog dialog(nullptr);
            dialog.isAdd(false);
            dialog.nameEdit->setText(table->item(row,1)->text());
            dialog.idEdit->setText(table->item(row,0)->text());
            dialog.sexEdit->setCurrentText(table->item(row,2)->text());
            dialog.birthEdit->setText(table->item(row,3)->text());
            dialog.phoneEdit->setText(table->item(row,4)->text());
            dialog.academyEdit->setCurrentText(table->item(row,5)->text());
            dialog.gradeEdit->setCurrentText(table->item(row,6)->text());
            dialog.idEdit->setReadOnly(true);
            if(dialog.exec()==QDialog::Accepted)
            {
                QString name=dialog.nameEdit->text();
                QString sex=dialog.sexEdit->currentText();
                QString birth=dialog.birthEdit->text();
                QString phone=dialog.phoneEdit->text();
                QString academy=dialog.academyEdit->currentText();
                QString grade=dialog.gradeEdit->currentText();
                QString credit=AdminPage->AStudenttableWidget->item(row,7)->text();
                dialog.fixStuInfo(name,dialog.idEdit->text(),sex,birth,phone,academy,grade);
                showStuInfo(AdminPage->AStudenttableWidget,name,dialog.idEdit->text(),sex,birth,phone,academy,grade,credit,row);
                fixStuInfo(AdminPage->AStudenttableWidget,row);
                deleteStu(AdminPage->AStudenttableWidget,row);
            }
        }
    });
}

//删除学生信息功能
void uiDesign::deleteStu(QTableWidget *table,int rowCount)
{
    QPushButton *stuDbutton=new QPushButton("删除");
    table->setCellWidget(rowCount,9,stuDbutton);
    connect(stuDbutton,&QPushButton::clicked,this,[=]()
            {
                QMessageBox::information(this,"注意！","选择删除学生后其所有信息将全部消失！\n一旦确认该操作无法撤回！");
                QMessageBox::StandardButton tip;
                tip=QMessageBox::question(nullptr,"删除学生",QString("确认要删除学生%1 %2吗？").arg(table->item(rowCount,0)->text(),table->item(rowCount,1)->text()),
                                            QMessageBox::Yes|QMessageBox::No);
                if(tip==QMessageBox::Yes)
                {
                    QSqlQuery coursequery;
                    coursequery.exec(QString("DELETE FROM stuscores WHERE StuID='%1'").arg(table->item(rowCount,0)->text()));
                    QSqlQuery stuquery;
                    stuquery.exec(QString("DELETE FROM student WHERE StuID='%1'").arg(table->item(rowCount,0)->text()));
                    QMessageBox::information(this,"成功",QString("删除学生%1 %2所有信息成功！").arg(table->item(rowCount,0)->text(),table->item(rowCount,1)->text()));
                    table->removeRow(rowCount);
                }
            });
}

//弹框弹出选择老师
class MyDialog:public QDialog{
public:
    QComboBox *choose= new QComboBox(this);
    MyDialog(QSqlQuery &query,QWidget *parent=nullptr):QDialog(parent)
    {
        setWindowTitle("替课");
        QGridLayout *layout=new QGridLayout(this);
        QLabel *label=new QLabel("选择一个教师接替工作：",this);
        layout->addWidget(label,1,0,1,2);
        choose->addItem(" ");
        QString str=QString("SELECT TeacherName FROM teacher WHERE ");
        bool jud=false;
        while(query.next())
        {
            str+="TeacherName!=";
            str+="'"+query.value(0).toString()+"'";
            str+=" AND ";
            jud=true;
        }

        if(jud)
        {
            str=str.left(str.length()-4);
        }
        else
        {
            str=str.left(str.length()-6);
        }

        QSqlQuery query1;
        query1.exec(str);
        while(query1.next())
        {
            QString teName=query1.value(0).toString();
            choose->addItem(teName);
        }
        layout->addWidget(choose,2,4,1,2);
        QPushButton *okButton=new QPushButton("确定接替",this);
        QPushButton *noButton=new QPushButton("取消接替",this);
        connect(okButton,&QPushButton::clicked,this,[=](){
            accept();
        });
        connect(noButton,&QPushButton::clicked,this,&QDialog::reject);
        QHBoxLayout *button=new QHBoxLayout(this);
        button->addWidget(okButton);
        button->addWidget(noButton);
        layout->addLayout(button,3,3,1,4);
        setLayout(layout);
    }
};

//删除老师信息
void uiDesign::deleteTerInfo(QTableWidget *table,int row)
{
    QPushButton *terDbutton=new QPushButton("删除");
    table->setCellWidget(row,5,terDbutton);
    connect(terDbutton,&QPushButton::clicked,this,[=]()
            {
                QMessageBox::information(this,"说明！","若删除有课教师，该教师上的所有课需交由另一教师教授！\n一旦确认，删除操作无法撤回！");
                QMessageBox::StandardButton tip;
                tip=QMessageBox::question(nullptr,"删除老师",QString("确认要删除老师：%1 %2吗？").arg(table->item(row,0)->text(),table->item(row,1)->text()),
                                            QMessageBox::Yes|QMessageBox::No);
                if(tip==QMessageBox::Yes)
                {
                    if(table->item(row,3)==NULL)
                    {
                        QMessageBox::information(this,"提示",QString("该教师未开设课程，直接删除！\n删除老师%1 %2所有信息成功！").arg(table->item(row,0)->text(),table->item(row,1)->text()));
                        QSqlQuery deletequery;
                        deletequery.exec(QString("DELETE FROM teacher WHERE TeacherID='%1' AND TeacherName='%2'").arg(table->item(row,0)->text(),table->item(row,1)->text()));
                        QMessageBox::information(this,"成功",QString("删除教师%1 %2信息成功！").arg(table->item(row,1)->text(),table->item(row,2)->text()));
                        table->removeRow(row);
                    }
                    else
                    {
                        QMessageBox::information(this,"提醒","该教师有教授课程，请安排另外的教师上课！");
                        QSqlQuery coursequery;
                        coursequery.exec(QString("SELECT courseID,studyDay,studyTime FROM harrycourse WHERE Teacher='%1'").arg(table->item(row,1)->text()));
                        if(coursequery.next())
                        {
                            QSqlQuery querytime;
                            querytime.exec(QString("SELECT Teacher FROM harrycourse WHERE studyDay='%1' AND studyTime='%2' AND isAvailable='1'").arg(coursequery.value(1).toString(),coursequery.value(2).toString()));
                            MyDialog dialog(querytime,nullptr);
                            if(dialog.exec()==QDialog::Accepted)
                            {
                                QSqlQuery query3;
                                query3.prepare(QString("UPDATE harrycourse SET Teacher=:teacher WHERE courseID='%1'").arg(coursequery.value(0).toString()));
                                query3.bindValue(":teacher",dialog.choose->currentText());
                                if(query3.exec())
                                {
                                    QMessageBox::information(this,"提示","教师替换成功！");
                                }
                                else
                                {
                                    QMessageBox::information(this,"提示","系统故障！替换失败！");
                                }
                                QSqlQuery query4;
                                query4.exec(QString("DELECT FROM teacher WHERE TeacherName='%1'").arg(table->item(row,1)->text()));
                                QMessageBox::information(this,"成功",QString("删除教师%1 %2信息成功！").arg(table->item(row,1)->text(),table->item(row,2)->text()));
                                table->removeRow(row);
                                on_query_pushButton2_clicked();
                            }
                            else{
                                QMessageBox::information(this,"失败",QString("删除教师%1 %2信息失败！").arg(table->item(row,1)->text(),table->item(row,2)->text()));
                            }
                        }

                    }
                }
            });
}

//查找老师信息的功能
void uiDesign::on_query_pushButton2_clicked()
{
    int rowCount=AdminPage->AteachertableWidget->rowCount();
    for(int i=rowCount-1;i>0;--i)
    {
        AdminPage->AteachertableWidget->removeRow(i);
    }
    AdminPage->AteachertableWidget->setColumnWidth(3,150);
    rowCount=1;
    //获取信息
    QString name=AdminPage->lineEdit_3->text();
    QString ID=AdminPage->lineEdit_4->text();
    QString institue=AdminPage->comboBox_2->currentText();
    //可能要使用到连接表的操作
    QSqlQuery query;
    QString QueryString1;
    QueryString1=QString("SELECT TeacherID,TeacherName,college FROM teacher WHERE ");
    if(!name.isEmpty())
    {
        QueryString1+=QString("TeacherName='%1' AND ").arg(name);
    }
    else if(!ID.isEmpty())
    {
        QueryString1+=QString("TeacherID='%1' AND ").arg(ID);
    }
    else if(!institue.isEmpty())
    {
        QueryString1 +=QString("college='%1' AND ").arg(institue);
    }
    QueryString1=QueryString1.left(QueryString1.length()-4);
    if(query.exec(QueryString1))
    {
        while(query.next())
        {
            QString TerID=query.value(0).toString();
            QString TerName=query.value(1).toString();
            QString TerCollege=query.value(2).toString();
            int row=AdminPage->AteachertableWidget->rowCount();
            AdminPage->AteachertableWidget->insertRow(row);
            AdminPage->AteachertableWidget->setItem(row,0,new QTableWidgetItem(TerID));
            AdminPage->AteachertableWidget->setItem(row,1,new QTableWidgetItem(TerName));
            AdminPage->AteachertableWidget->setItem(row,2,new QTableWidgetItem(TerCollege));
            fixTerInfo(AdminPage->AteachertableWidget,row);
            deleteTerInfo(AdminPage->AteachertableWidget,row);
            QSqlQuery query1;
            QString str=QString("SELECT courseName FROM harrycourse WHERE Teacher='%1' AND isAvailable='1'").arg(query.value(1).toString());
            query1.exec(str);
            if(query1.next())
            {
                QString TerClass=query1.value(0).toString();
                AdminPage->AteachertableWidget->setItem(row,3,new QTableWidgetItem(TerClass));
                while(query1.next())
                {
                    TerClass=query1.value(0).toString();
                    int row=AdminPage->AteachertableWidget->rowCount();
                    AdminPage->AteachertableWidget->insertRow(row);
                    AdminPage->AteachertableWidget->setItem(row,0,new QTableWidgetItem(TerID));
                    AdminPage->AteachertableWidget->setItem(row,1,new QTableWidgetItem(TerName));
                    AdminPage->AteachertableWidget->setItem(row,2,new QTableWidgetItem(TerCollege));
                    AdminPage->AteachertableWidget->setItem(row,3,new QTableWidgetItem(TerClass));
                    fixTerInfo(AdminPage->AteachertableWidget,row);
                    deleteTerInfo(AdminPage->AteachertableWidget,row);
                }
            }
        }
    }
}

//构建一个添加弹窗
class TerInfoDialog:public QDialog
{
public:
    bool Add;
    void setAdd(bool isAdd)
    {
        Add=isAdd;
    }
    QLineEdit *idEdit=new QLineEdit(this);
    QLineEdit *nameEdit=new QLineEdit(this);
    QComboBox *collegeEdit=new QComboBox(this);

    TerInfoDialog(QWidget *parent=nullptr):QDialog(parent)
    {
        setWindowTitle("老师信息管理");
        QGridLayout *layout =new QGridLayout(this);
        layout->setSizeConstraint(QLayout::SetFixedSize);
        layout->setSpacing(5);
        QLabel *note=new QLabel("有*为必填项！",this);
        layout->addWidget(note,0,1,1,1);
        if(!Add)
        {
            QLabel *note1=new QLabel("工号不可修改！",this);
            layout->addWidget(note1,0,2,1,1);
        }

        QLabel *terid=new QLabel("*工号",this);
        layout->addWidget(terid,1,0,1,1);
        layout->addWidget(idEdit,1,1,1,1);

        QLabel *tername=new QLabel("*姓名",this);
        layout->addWidget(tername,1,5,1,1);
        layout->addWidget(nameEdit,1,6,1,1);

        QLabel *tercollege=new QLabel("*学院",this);
        collegeEdit->addItem("");
        collegeEdit->addItem("格兰芬多");
        collegeEdit->addItem("斯莱特林");
        collegeEdit->addItem("拉文克劳");
        collegeEdit->addItem("赫奇帕奇");
        layout->addWidget(tercollege,2,0,1,1);
        layout->addWidget(collegeEdit,2,1,1,1);

        QLabel *note1=new QLabel("密码初始为工号！",this);
        layout->addWidget(note1,3,0,1,3);

        QPushButton *okButton=new QPushButton("确定",this);
        QPushButton *closeButton=new QPushButton("取消",this);

        connect(okButton,&QPushButton::clicked,this,[=]()
                {
                    if(!idEdit->text().isEmpty()&&!nameEdit->text().isEmpty()&&!collegeEdit->currentText().isEmpty())
                    {
                        QSqlQuery queryid;
                        queryid.exec(QString("SELECT * FROM teacher WHERE TeacherID='%1'").arg(idEdit->text()));

                        if(idEdit->text().length()!=6)
                        {
                            QMessageBox::information(this,"提示","工号不合法！工号长度应为6！");
                        }
                        else if(Add&&queryid.next())
                        {
                            QMessageBox::information(this,"提示","工号输入错误!\n该工号已存在，请重新设计！");
                        }
                        else
                        {
                            if(Add)
                            {
                                QMessageBox::information(this,"提示","添加老师信息成功！");
                            }
                            else
                            {
                                QMessageBox::information(this,"提示","修改老师信息成功！请刷新后查看！");
                            }
                            accept();
                        }
                    }
                    else
                    {
                        QMessageBox::critical(this,"错误","*号为必填项，请完善！");
                    }
                });
        connect(closeButton,&QPushButton::clicked,this,&QDialog::reject);
        QHBoxLayout *buttonLayout=new QHBoxLayout();
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(closeButton);
        layout->addLayout(buttonLayout,4,3,1,2);
        setLayout(layout);
    }
    virtual ~TerInfoDialog(){};

    //添加老师信息数据库更新
    void addTerInfo(QString name,QString id,QString passkey,QString college)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO teacher(TeacherName,TeacherID,TPasskey,college) VALUES(:TerName,:TerID,:TerPasskey,:TerCollege)");
        query.bindValue(":TerName",name);
        query.bindValue(":TerID",id);
        query.bindValue(":TerPasskey",passkey);
        query.bindValue(":TerCollege",college);
        if(!query.exec())
        {
            qDebug() << "系统出错！添加失败:" << query.lastError().text();
        }
    }


    //修改老师信息数据库更新
    void fixTer(QString name,QString id,QString college)
    {
        QSqlQuery query;
        query.prepare(QString("UPDATE teacher SET TeacherName=:name,college=:college WHERE TeacherID='%1'").arg(id));
        query.bindValue(":name",name);
        query.bindValue(":college",college);
        if(!query.exec())
        {
            qDebug() << "系统出错！修改失败:" << query.lastError().text();
        }
    }
};

//展示老师信息函数
void showTerInfo(QTableWidget *table,QString id,QString name,QString college,int row)
{
    table->setItem(row,0,new QTableWidgetItem(id));
    table->setItem(row,1,new QTableWidgetItem(name));
    table->setItem(row,2,new QTableWidgetItem(college));
}

//修改老师信息：工号不可修改，课程不能在此修改！
void uiDesign::fixTerInfo(QTableWidget *table,int row)
{
    QPushButton *fixbutton=new QPushButton("修改");
    table->setCellWidget(row,4,fixbutton);
    connect(fixbutton,&QPushButton::clicked,this,[=](){
        QMessageBox::StandardButton tip;
        tip=QMessageBox::question(nullptr,"修改老师信息",QString("确认要修改老师：%1 %2的信息吗？").arg(table->item(row,0)->text(),table->item(row,1)->text()),
                                    QMessageBox::Yes|QMessageBox::No);
        if(tip==QMessageBox::Yes)
        {
            TerInfoDialog dialog(nullptr);
            dialog.setAdd(false);
            dialog.nameEdit->setText(table->item(row,1)->text());
            dialog.idEdit->setText(table->item(row,0)->text());
            dialog.collegeEdit->setCurrentText(table->item(row,2)->text());
            dialog.idEdit->setReadOnly(true);
            if(dialog.exec()==QDialog::Accepted)
            {
                QString name=dialog.nameEdit->text();
                QString id=dialog.idEdit->text();
                QString college=dialog.collegeEdit->currentText();
                dialog.fixTer(name,id,college);
                showTerInfo(AdminPage->AStudenttableWidget,id,name,college,row);
                fixTerInfo(AdminPage->AStudenttableWidget,row);
                deleteTerInfo(AdminPage->AStudenttableWidget,row);
                on_query_pushButton2_clicked();
            }
        }
    });
}

//添加老师信息

void uiDesign::on_addTer_pushButton_clicked()
{
    TerInfoDialog dialog(nullptr);
    if(dialog.exec()==QDialog::Accepted)
    {
        dialog.setAdd(true);
        QString name=dialog.nameEdit->text();
        QString id=dialog.idEdit->text();
        QString college=dialog.collegeEdit->currentText();
        QString passkey=dialog.idEdit->text();
        dialog.addTerInfo(name,id,passkey,college);
        int row=AdminPage->AteachertableWidget->rowCount();
        AdminPage->AteachertableWidget->insertRow(row);
        showTerInfo(AdminPage->AteachertableWidget,id,name,college,row);
        fixTerInfo(AdminPage->AStudenttableWidget,row);
        deleteTerInfo(AdminPage->AteachertableWidget,row);
        on_query_pushButton2_clicked();
    }

}

//评价开始功能
void uiDesign::on_Start_pushButton_clicked()
{
    QSqlQuery Jude;
    Jude.exec("SELECT Evaluation FROM myfunction");
    Jude.next();
    QString temp=Jude.value(0).toString(); //用于检查是否再评教阶段
    if(temp=='1')
    {
        QMessageBox::information(this,"提示！","评教功能已开放，请勿重复操作！");
    }
    else
    {
        QMessageBox::StandardButton tip;                    //创建一个对话框
        tip=QMessageBox::question(nullptr,"开放评教功能",QString("确认要开放学生的评教功能吗？"),
                                    QMessageBox::Yes|QMessageBox::No);
        if(tip==QMessageBox::Yes)
        {
            QSqlQuery Open;
            QString Jud="1";                 //成功的赋值，用于标记
            Open.prepare(QString("UPDATE myfunction SET Evaluation=:value WHERE Evaluation='0'"));       //修改一下数据库的内容
            Open.bindValue(":value",Jud);
            Open.exec();
            QSqlQuery Jude1;
            Jude1.exec("SELECT Evaluation FROM myfunction");
            Jude1.next();
            QString temp=Jude1.value(0).toString(); //用于检查是否修改成功
            if(temp=='1')
            {
                QMessageBox::information(this,"开放成功：","已成功开放学生系统评教功能！");
            }
            else{
                QMessageBox::information(this,"开放失败：","系统出现故障！");
            }
        }
        else
        {
            QMessageBox::information(this,"取消开放：","已取消开放评教功能.");
        }
    }

}

//关闭评教系统
void uiDesign::on_close_pushButton_clicked()
{
    QSqlQuery Jude;
    Jude.exec("SELECT Evaluation FROM myfunction");
    Jude.next();
    QString temp=Jude.value(0).toString(); //用于检查是否再评教阶段
    if(temp=='0')
    {
        QMessageBox::information(this,"提示！","评教功能已关闭，请勿重复操作！");
    }
    else
    {
        QMessageBox::StandardButton tip;                    //创建一个对话框
        tip=QMessageBox::question(nullptr,"关闭评教功能",QString("确认要关闭学生的评教功能吗？"),
                                    QMessageBox::Yes|QMessageBox::No);
        if(tip==QMessageBox::Yes)
        {
            QSqlQuery Close;
            QString Jud="0";                 //成功的赋值
            Close.prepare(QString("UPDATE myfunction SET Evaluation=:value WHERE Evaluation='1'"));
            Close.bindValue(":value",Jud);
            Close.exec();
            QSqlQuery Jude1;
            Jude1.exec("SELECT Evaluation FROM myfunction");
            Jude1.next();
            QString temp1=Jude1.value(0).toString(); //用于检查是否修改成功
            if(temp1=='0')
            {
                QMessageBox::information(this,"关闭成功：","已成功关闭学生系统评教功能！");
            }
            else{
                QMessageBox::information(this,"关闭失败：","系统出现故障！");
            }
        }
        else
        {
            QMessageBox::information(this,"取消关闭：","已取消关闭评教功能.");
        }
    }

}

//开放选课功能
void uiDesign::on_pushButton_11_clicked()
{
    QSqlQuery Jude;
    Jude.exec("SELECT Selection FROM myfunction");
    Jude.next();
    QString temp=Jude.value(0).toString(); //用于检查是否再评教阶段
    if(temp=='1')
    {
        QMessageBox::information(this,"提示！","选课功能已开放，请勿重复操作！");
    }
    else
    {
        QMessageBox::StandardButton tip;                    //创建一个对话框
        tip=QMessageBox::question(nullptr,"开放选课功能",QString("确认要开放学生的选课功能吗？"),
                                    QMessageBox::Yes|QMessageBox::No);
        if(tip==QMessageBox::Yes)
        {
            QSqlQuery Open;
            QString Jud="1";                 //成功的赋值
            Open.prepare(QString("UPDATE myfunction SET Selection=:value WHERE Selection='0'"));
            Open.bindValue(":value",Jud);
            Open.exec();
            QSqlQuery Jude1;
            Jude1.exec("SELECT Selection FROM myfunction");
            Jude1.next();
            QString temp1=Jude1.value(0).toString(); //用于检查是否修改成功
            if(temp1=='1')
            {
                QMessageBox::information(this,"开放成功：","已成功开放学生系统选课功能！");
            }
            else{
                QMessageBox::information(this,"开放失败：","系统出现故障！");
            }
        }
        else
        {
            QMessageBox::information(this,"取消开放：","已取消开放选课功能.");
        }
    }
}

//关闭选课功能
void uiDesign::on_pushButton_12_clicked()
{
    QSqlQuery Jude;
    Jude.exec("SELECT Selection FROM myfunction");
    Jude.next();
    QString temp=Jude.value(0).toString(); //用于检查是否再评教阶段
    if(temp=="0")
    {
        QMessageBox::information(this,"提示！","选课功能已关闭，请勿重复操作！");
    }
    else
    {
        QMessageBox::StandardButton tip;                    //创建一个对话框
        tip=QMessageBox::question(nullptr,"关闭选课功能",QString("确认要关闭学生的选课功能吗？"),
                                    QMessageBox::Yes|QMessageBox::No);
        if(tip==QMessageBox::Yes)
        {
            QSqlQuery Close;
            QString Jud="0";                 //成功的赋值
            Close.prepare(QString("UPDATE myfunction SET Selection=:value WHERE Selection='1'"));
            Close.bindValue(":value",Jud);
            Close.exec();
            QSqlQuery Jude1;
            Jude1.exec("SELECT Selection FROM myfunction");
            Jude1.next();
            QString temp1=Jude1.value(0).toString(); //用于检查是否修改成功
            if(temp1=="0")
            {
                QMessageBox::information(this,"关闭成功：","已成功关闭学生系统选课功能！");
            }
            else{
                QMessageBox::information(this,"关闭失败：","系统出现故障！");
            }
        }
        else
        {
            QMessageBox::information(this,"取消关闭：","已取消关闭选课功能.");
        }
    }
}

void uiDesign::on_ScoreB_pushButton_clicked()
{
    QSqlQuery Jude;
    Jude.exec("SELECT ScoreEdit FROM myfunction");
    Jude.next();
    QString temp=Jude.value(0).toString(); //用于检查是否再评教阶段
    if(temp=='1')
    {
        QMessageBox::information(this,"提示！","录入成绩功能已开放，请勿重复操作！");
    }
    else
    {
        QMessageBox::StandardButton tip;                    //创建一个对话框
        tip=QMessageBox::question(nullptr,"开放老师录入成绩功能",QString("确认要开放老师录入成绩功能吗？"),
                                    QMessageBox::Yes|QMessageBox::No);
        if(tip==QMessageBox::Yes)
        {
            QSqlQuery Open;
            QString Jud="1";                 //成功的赋值
            Open.prepare(QString("UPDATE myfunction SET ScoreEdit=:value WHERE ScoreEdit='0'"));
            Open.bindValue(":value",Jud);
            Open.exec();
            QSqlQuery Jude1;
            Jude1.exec("SELECT ScoreEdit FROM myfunction");
            Jude1.next();
            QString temp1=Jude1.value(0).toString(); //用于检查是否修改成功
            if(temp1=='1')
            {
                QMessageBox::information(this,"开放成功：","已成功开放老师录入成绩功能！");
            }
            else{
                QMessageBox::information(this,"开放失败：","系统出现故障！");
            }
        }
        else
        {
            QMessageBox::information(this,"取消开放：","已取消开放老师录入成绩功能.");
        }
    }
}


void uiDesign::on_ScoreC_pushButton_clicked()
{
    QSqlQuery Jude;
    Jude.exec("SELECT ScoreEdit FROM myfunction");
    Jude.next();
    QString temp=Jude.value(0).toString(); //用于检查是否再评教阶段
    if(temp=="0")
    {
        QMessageBox::information(this,"提示！","录入成绩功能已关闭，请勿重复操作！");
    }
    else
    {
        QMessageBox::StandardButton tip;                    //创建一个对话框
        tip=QMessageBox::question(nullptr,"关闭录入成绩功能",QString("确认要关闭教师录入成绩的功能吗？"),
                                    QMessageBox::Yes|QMessageBox::No);
        if(tip==QMessageBox::Yes)
        {
            QSqlQuery Close;
            QString Jud="0";                 //成功的赋值
            Close.prepare(QString("UPDATE myfunction SET ScoreEdit=:value WHERE ScoreEdit='1'"));
            Close.bindValue(":value",Jud);
            Close.exec();
            QSqlQuery Jude1;
            Jude1.exec("SELECT ScoreEdit FROM myfunction");
            Jude1.next();
            QString temp1=Jude1.value(0).toString(); //用于检查是否修改成功
            if(temp1=="0")
            {
                QMessageBox::information(this,"关闭成功：","已成功关闭教师录入成绩功能！");
            }
            else{
                QMessageBox::information(this,"关闭失败：","系统出现故障！");
            }
        }
        else
        {
            QMessageBox::information(this,"取消关闭：","已取消关闭老师录入成绩功能.");
        }
    }
}


//修改学生的成绩
class StuScoreDialog:public QDialog
{
public:
    QLineEdit *score=new QLineEdit(this);
    QLineEdit *Name=new QLineEdit("姓名",this);
    QLineEdit *ID=new QLineEdit("学号",this);
    QLineEdit *College=new QLineEdit("学院",this);
    QLineEdit *courseID=new QLineEdit("课程编号",this);
    QLineEdit *courseName=new QLineEdit("课程名称",this);
    QLineEdit *Score=new QLineEdit("分数",this);
    StuScoreDialog(QWidget *parent=nullptr):QDialog(parent)
    {
        setWindowTitle("修改学生成绩!");
        QGridLayout *layout =new QGridLayout(this);
        layout->setSizeConstraint(QLayout::SetFixedSize);
        QLabel *nameLabel=new QLabel("学生姓名：",this);
        layout->addWidget(nameLabel,1,0,1,1);
        layout->addWidget(Name,1,1,1,1);

        QLabel *idLabel=new QLabel("学生学号:",this);
        layout->addWidget(idLabel,1,2,1,1);
        layout->addWidget(ID,1,3,1,1);

        QLabel *collegeLabel=new QLabel("学生学院:",this);
        layout->addWidget(collegeLabel,1,4,1,1);
        layout->addWidget(College,1,5,1,1);

        QLabel *courseid=new QLabel("课程编号:",this);
        layout->addWidget(courseid,2,0,1,1);
        layout->addWidget(courseID,2,1,1,1);

        QLabel *coursename=new QLabel("课程名称:",this);
        layout->addWidget(coursename,2,2,1,1);
        layout->addWidget(courseName,2,3,1,1);

        QLabel *scoreLabel=new QLabel("目前成绩:",this);
        layout->addWidget(scoreLabel,2,4,1,1);
        layout->addWidget(Score,2,5,1,1);
        QLabel *Fix=new QLabel("修改分数",this);
        QLabel *range=new QLabel("(0~100)：",this);
        layout->addWidget(Fix,3,0,1,1);
        layout->addWidget(range,3,1,1,1);
        layout->addWidget(score,4,2,1,2);
        QPushButton *okButton=new QPushButton("确定",this);
        QPushButton *closeButton=new QPushButton("取消",this);
        connect(okButton,&QPushButton::clicked,this,[=](){
            if(score->text().toInt()<0||score->text().toInt()>100)
            {
                QMessageBox::information(this,"错误","分数不合法！请输入0~100的分数！");
            }
            else
            {
                QMessageBox::information(this,"成功","修改学生分数成功");
                accept();
            }
        });
        connect(closeButton,&QPushButton::clicked,this,&QDialog::reject);
        QHBoxLayout *button=new QHBoxLayout();
        button->addWidget(okButton);
        button->addWidget(closeButton);
        layout->addLayout(button,5,2,1,2);
        setLayout(layout);
    }
    virtual ~StuScoreDialog(){};

    void fixScore(QString ID,QString courseID,QString score)
    {
        QSqlQuery query;
        query.prepare(QString("UPDATE stuscores SET Scores=:score,Acredit=:credit WHERE StuID='%1' AND courseName='%2' AND courseTime='2023'").arg(ID,courseID));
        query.bindValue(":score",score);
        QString credit = QString::number(float((score.toInt() - 50)%100), 'f', 1);
        query.bindValue(":credit",credit);
        if(!query.exec())
        {
            qDebug()<<"系统出错！修改失败！";
        }
    }
};

//修改学生成绩
void uiDesign::fixStuScore(QTableWidget *table,int row)
{
    QPushButton *fix=new QPushButton("修改");
    table->setCellWidget(row,6,fix);
    connect(fix,&QPushButton::clicked,this,[=](){
        QMessageBox::StandardButton tip;
        tip=QMessageBox::question(nullptr,"修改学生成绩",QString("确认要修改学生：%1 %2 %3的成绩吗？").arg(table->item(row,0)->text(),table->item(row,1)->text(),table->item(row,4)->text()),
                                    QMessageBox::Yes|QMessageBox::No);
        if(tip==QMessageBox::Yes)
        {
            StuScoreDialog dialog(nullptr);
            dialog.Name->setText(table->item(row,1)->text());
            dialog.ID->setText(table->item(row,0)->text());
            dialog.College->setText(table->item(row,2)->text());
            dialog.courseID->setText(table->item(row,3)->text());
            dialog.courseName->setText(table->item(row,4)->text());
            dialog.Score->setText(table->item(row,5)->text());
            dialog.Name->setReadOnly(true);
            dialog.ID->setReadOnly(true);
            dialog.College->setReadOnly(true);
            dialog.courseID->setReadOnly(true);
            dialog.courseName->setReadOnly(true);
            if(dialog.Score->text().isEmpty())
            {
                dialog.Score->setText("暂无成绩");
            }
            dialog.Score->setReadOnly(true);
            if(dialog.exec()==QDialog::Accepted)
            {
                QString score=dialog.score->text();
                dialog.fixScore(table->item(row,0)->text(),table->item(row,3)->text(),score);
                AdminPage->AStuScoreWidget_2->setItem(row,5,new QTableWidgetItem(score));
                fixStuScore(AdminPage->AStuScoreWidget_2,row);
            }
        }
    });
}

//学生成绩查询：只能查到本年的学生
void uiDesign::on_pushButton_2_clicked()
{
    int rowCount=AdminPage->AStuScoreWidget_2->rowCount();
    for(int i=rowCount-1;i>0;--i)
    {
        AdminPage->AStuScoreWidget_2->removeRow(i);
    }
    rowCount=1;
    QString name=AdminPage->lineEdit->text();
    QString ID=AdminPage->lineEdit_2->text();
    QString institue=AdminPage->comboBox_3->currentText();
    QString courseID=AdminPage->lineEdit_7->text();
    QString courseName=AdminPage->lineEdit_8->text();
    //查询学生信息并写入表格
    QSqlQuery query;
    QSqlQuery query1;
    QSqlQuery query2;
    QString QueryString="SELECT * FROM student WHERE ";

    int isstu1=0;
    if(!name.isEmpty())
    {
        QueryString+=QString("StuName='%1' AND ").arg(name);
        isstu1=1;
    }
    if(!ID.isEmpty())
    {
        QueryString+=QString("StuID='%1' AND ").arg(ID);
        isstu1=1;
    }
    if(!institue.isEmpty())
    {
        QueryString+=QString("Academy='%1' AND ").arg(institue);
        isstu1=1;
    }


    if(isstu1==1)
    {
        QueryString=QueryString.left(QueryString.length()-4);
    }
    else
    {
        QueryString=QueryString.left(QueryString.length()-6);
    }


    if(query.exec(QueryString))
    {
        while(query.next())
        {
            QString QueryString1="SELECT * FROM stuscores WHERE courseTime='2023'";
            if(!courseID.isEmpty())
            {
                QueryString1+=QString(" AND courseName='%1'").arg(courseID);
            }
            QueryString1+=QString(" AND StuID='%1'").arg(query.value(1).toString());
            if(query1.exec(QueryString1))
            {
                while(query1.next())
                {
                    QString QueryString2="SELECT courseName,courseID FROM harrycourse WHERE ";
                    if(!courseName.isEmpty())
                    {
                        QueryString2+=QString("courseName='%1' AND ").arg(courseName);
                    }
                    QueryString2+=QString("courseID='%1'").arg(query1.value(1).toString());
                    if(query2.exec(QueryString2))
                    {
                        while(query2.next())
                        {
                            QString Name=query.value(0).toString();
                            QString id=query.value(1).toString();
                            QString college=query.value(5).toString();
                            QString courseid=query1.value(1).toString();
                            QString coursename=query2.value(0).toString();
                            QString score=query1.value(2).toString();
                            int row=AdminPage->AStuScoreWidget_2->rowCount();

                            AdminPage->AStuScoreWidget_2->insertRow(row);
                            AdminPage->AStuScoreWidget_2->setItem(row,0,new QTableWidgetItem(id));
                            AdminPage->AStuScoreWidget_2->setItem(row,1,new QTableWidgetItem(Name));
                            AdminPage->AStuScoreWidget_2->setItem(row,2,new QTableWidgetItem(college));
                            AdminPage->AStuScoreWidget_2->setItem(row,3,new QTableWidgetItem(courseid));
                            AdminPage->AStuScoreWidget_2->setItem(row,4,new QTableWidgetItem(coursename));
                            AdminPage->AStuScoreWidget_2->setItem(row,5,new QTableWidgetItem(score));
                            fixStuScore(AdminPage->AStuScoreWidget_2,row);
                        }
                    }
                }
            }
        }
    }
}

//内容编辑弹框
class NoticeDialog:public QDialog
{
public:
    QLineEdit *Nname=new QLineEdit(this);
    QTextEdit *Ncontent=new QTextEdit(this);
    NoticeDialog(QWidget *parent=nullptr):QDialog(parent)
    {
        QGridLayout *layout=new QGridLayout(this);
        QLabel *name=new QLabel("公告名称：");
        layout->addWidget(name,1,0,1,1);
        layout->addWidget(Nname,1,2,1,4);
        QLabel *content=new QLabel("内容填写:");
        layout->addWidget(content,2,0,1,1);
        layout->addWidget(Ncontent,3,0,4,5);

        QPushButton *okButton=new QPushButton("确定",this);
        QPushButton *closeButton=new QPushButton("取消",this);
        connect(okButton,&QPushButton::clicked,this,[=]()
                {
                    if(!Nname->text().isEmpty()&&!Ncontent->toPlainText().isEmpty())
                    {
                        QSqlQuery querynotice;
                        querynotice.exec(QString("SELECT * FROM noticep WHERE NoticeName='%1'").arg(Nname->text()));
                        if(querynotice.next())
                        {
                            QMessageBox::information(this,"提示","公告名称已存在！请重新编辑！");
                        }
                        else
                        {
                            accept();
                        }
                    }
                    else
                    {
                        QMessageBox::information(this,"提示","公告题目和内容不能为空！");
                    }
                });
        connect(closeButton,&QPushButton::clicked,this,&QDialog::reject);
        QHBoxLayout *button=new QHBoxLayout();
        button->addWidget(okButton);
        button->addWidget(closeButton);
        layout->addLayout(button,7,2,1,2);
    }
};

//查看公告的窗口
class showDialog:public QDialog{
public:
    QLabel *time=new QLabel("发布时间：");
    QLabel *name=new QLabel("公告名称：");
    QTextBrowser *content=new QTextBrowser(this);
    showDialog(QWidget *parent=nullptr):QDialog(parent)
    {
        QGridLayout *layout=new QGridLayout(this);
        layout->addWidget(time,1,0,1,2);
        layout->addWidget(name,2,0,1,2);
        layout->addWidget(content,3,0,4,5);
        QPushButton *button= new QPushButton("确定");
        layout->addWidget(button,7,3,1,1);
        connect(button,&QPushButton::clicked,this,&QDialog::reject);
        setLayout(layout);
    }
};

//发布
void uiDesign::on_pushButton_3_clicked()
{
    int rowCount=AdminPage->ANoticeWidget_2->rowCount();
    for(int i=rowCount-1;i>0;--i)
    {
        AdminPage->ANoticeWidget_2->removeRow(i);
    }
    NoticeDialog dialog(nullptr);
    if(dialog.exec()==QDialog::Accepted)
    {
        QString Name=dialog.Nname->text();
        QString Content=dialog.Ncontent->toPlainText();
        QSqlQuery query;
        query.prepare("INSERT INTO noticep(currentTime,NoticeName,Content) VALUES(:time,:name,:content)");
        QDateTime time=QDateTime::currentDateTime();
        query.bindValue(":time",time);
        query.bindValue(":name",Name);
        query.bindValue(":content",Content);
        if(!query.exec())
        {
            QMessageBox::information(this,"提示","系统错误！发布失败！");
        }

        QSqlQuery query1;
        query1.exec("SELECT * FROM noticep ORDER BY currentTime DESC");
        while(query1.next())
        {
            int row=AdminPage->ANoticeWidget_2->rowCount();
            AdminPage->ANoticeWidget_2->insertRow(row);
            AdminPage->ANoticeWidget_2->setItem(row,0,new QTableWidgetItem(query1.value(0).toString()));
            AdminPage->ANoticeWidget_2->setItem(row,1,new QTableWidgetItem(query1.value(1).toString()));
            AdminPage->ANoticeWidget_2->setItem(row,2,new QTableWidgetItem(query1.value(2).toString()));
            showNotice(AdminPage->ANoticeWidget_2,row);
            deleteNotice(AdminPage->ANoticeWidget_2,row);
        }
        QMessageBox::information(this,"提示","发布公告成功！");
    }
}

//删除公告按钮
void uiDesign::deleteNotice(QTableWidget *table,int row)
{
    QPushButton *fixNotice=new QPushButton("删除");
    table->setCellWidget(row,4,fixNotice);
    connect(fixNotice,&QPushButton::clicked,this,[=]()
            {
                QMessageBox::StandardButton tip;
                tip=QMessageBox::question(nullptr,"删除公告",QString("确认要删除《%1》公告？").arg(table->item(row,1)->text()),
                                            QMessageBox::Yes|QMessageBox::No);
                if(tip==QMessageBox::Yes)
                {
                    QSqlQuery querynotice;
                    QString str= QString("DELETE FROM noticep WHERE NoticeName='%1'").arg(table->item(row,1)->text());
                    if(querynotice.exec(str))
                    {
                        QMessageBox::information(this,"提示","删除公告成功！");
                        table->removeRow(row);
                    }
                    else
                    {
                        QMessageBox::information(this,"提示","系统错误！删除公告失败！");
                    }
                }
            });
}

//查看公告内容：
void uiDesign::showNotice(QTableWidget *table,int row)
{
    QPushButton *showNotice=new QPushButton("查看");
    table->setCellWidget(row,3,showNotice);
    connect(showNotice,&QPushButton::clicked,this,[=]()
            {
                showDialog dialog(nullptr);
                dialog.time->setText(table->item(row,0)->text());
                dialog.name->setText(table->item(row,1)->text());
                dialog.content->setPlainText(table->item(row,2)->text());
                dialog.exec();
            });
}
