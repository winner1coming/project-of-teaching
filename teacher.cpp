#include "uidesign.h"
#include "dialog.cpp"
//教师页面功能
//查询学生信息
void uiDesign::on_TStuInfoButton_released()
{
    uiDesign::deleteTabelItem(TeacherPage->StuInfoTable);
    //获取输入的筛选信息
    QString courseName=TeacherPage->StuInfoCourseBox->currentText();//课程名称
    QString time=TeacherPage->StuInfoTime_Box->currentText();//学年
    QString stuName=TeacherPage->StuInfoNameLine->text();
    QString stuID=TeacherPage->StuInfoINumLine->text();
    QString insititute=TeacherPage->StuInfoInstituteBox->currentText();
    QString grade=TeacherPage->StuInfoGradeBox->currentText();
    QSqlQuery queryCourseNameID;//查询课程名
    //isAvailable为0时课程被删除
    queryCourseNameID.exec(QString("SELECT courseName, courseID,isAvailable FROM teacher JOIN harrycourse ON teacher.TeacherName = harrycourse.Teacher WHERE teacher.TeacherID= %1").arg(id));
    int rowCount=1;
    while(queryCourseNameID.next()){
        QString courseNameNow;
        //判断是否符合课程信息
        if((courseName.isEmpty()||courseName==queryCourseNameID.value(0).toString())&&queryCourseNameID.value(2).toBool()) courseNameNow=queryCourseNameID.value(0).toString();
        else continue;
        QString selectStuscores="SELECT * FROM stuscores where courseName=%1";//按课程编号
        QSqlQuery Stuscores;
        QString courseID=queryCourseNameID.value(1).toString();
        Stuscores.exec(selectStuscores.arg(courseID));
        while(Stuscores.next()){//循坏遍历选了该门课的学生,逐个比较是否符合条件
            QSqlQuery studentList;
            //可能要显示的信息
            QString stuNameNow;
            QString stuIDNow;
            QString instituteNow;
            QString gradeNow;
            QString timeNow;//学年
            QString gender;
            QString phone;
            QString score;
            if(time.isEmpty()||time==Stuscores.value(6).toString()) timeNow=Stuscores.value(6).toString();//学年匹配
            else continue;
            if(stuID.isEmpty()||stuID==Stuscores.value(0).toString()) stuIDNow=Stuscores.value(0).toString();
            else continue;
            studentList.exec(QString("SELECT * FROM student WHERE StuID=%1").arg(stuIDNow));
            studentList.next();
            if(stuName.isEmpty()||stuName==studentList.value(0).toString()) stuNameNow=studentList.value(0).toString();
            else continue;
            if(insititute.isEmpty()||insititute==studentList.value(5).toString()) instituteNow=studentList.value(5).toString();
            else continue;
            if(grade.isEmpty()||grade==studentList.value(7).toString()) gradeNow=studentList.value(7).toString();
            else continue;
            phone=studentList.value(3).toString();
            gender=studentList.value(2).toString();
            score=Stuscores.value(2).toString();
            TeacherPage->StuInfoTable->insertRow(rowCount);
            QString number=QString::number(rowCount);
            TeacherPage->StuInfoTable->setItem(rowCount, 0, new QTableWidgetItem(number));
            TeacherPage->StuInfoTable->setItem(rowCount, 1, new QTableWidgetItem(stuIDNow));
            TeacherPage->StuInfoTable->setItem(rowCount, 2, new QTableWidgetItem(stuNameNow));
            TeacherPage->StuInfoTable->setItem(rowCount, 3, new QTableWidgetItem(gender));
            TeacherPage->StuInfoTable->setItem(rowCount, 4, new QTableWidgetItem(phone));
            TeacherPage->StuInfoTable->setItem(rowCount, 5, new QTableWidgetItem(instituteNow));
            TeacherPage->StuInfoTable->setItem(rowCount, 6, new QTableWidgetItem(gradeNow));
            TeacherPage->StuInfoTable->setItem(rowCount, 7, new QTableWidgetItem(courseNameNow));
            TeacherPage->StuInfoTable->setItem(rowCount, 8, new QTableWidgetItem(timeNow));
            TeacherPage->StuInfoTable->setItem(rowCount, 9, new QTableWidgetItem(score));
            rowCount++;
        }
    }
    TeacherPage->StuInfoTable->resizeRowsToContents();
    // for(int i=1;i<rowCount;i++){
    //     TeacherPage->StuInfoTable->resizeColumnToContents(i);
    // }
}
//录入学生成绩时弹出的窗口
class ScoresDialog : public QDialog
{
public:
    int score;
    ScoresDialog(QWidget *parent = nullptr) : QDialog(parent),score(-1)
    {
        setWindowTitle("学生成绩录入");
        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *promptLabel = new QLabel("请输入学生的成绩（0~100）：", this);
        layout->addWidget(promptLabel);

        QLineEdit *lineEdit = new QLineEdit(this);
        layout->addWidget(lineEdit);

        QPushButton *okButton = new QPushButton("确定", this);
        QPushButton *cancelButton = new QPushButton("取消", this);

        connect(okButton, &QPushButton::released, this, [=]() {
            bool ok;
            int rating = lineEdit->text().toInt(&ok);

            if (ok && rating >= 0 && rating <= 100)
            {
                // 处理有效的评分
                score=lineEdit->text().toInt(&ok);
                accept();
            }
            else
            {
                // 显示错误提示
                QMessageBox::critical(this, "错误", "请输入0-100之间的数字");
            }
        });

        connect(cancelButton, &QPushButton::released, this, &QDialog::reject);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);

        layout->addLayout(buttonLayout);

        setLayout(layout);
    }
    virtual ~ScoresDialog(){}
};
//用来登记学生成绩的函数
void uiDesign::onScoreCheck(){
    deleteTabelItem(TeacherPage->scores_table);
    QString courseName=TeacherPage->scoresCourseName_Box->currentText();
    QString stuName=TeacherPage->scoresName_line->text();
    QString stuID=TeacherPage->scoresNum_line->text();
    QString year=TeacherPage->scoresYear_Box->currentText();
    QSqlQuery student;
    QSqlQuery stuscores;
    QSqlQuery course;//用来查课程信息的
    course.exec(QString("SELECT courseName, courseID,isAvailable FROM teacher JOIN harrycourse ON teacher.TeacherName = harrycourse.Teacher WHERE teacher.TeacherID= %1").arg(id));
    int rowCount=1;
    while(course.next()){
        QString courseNameNow;
        QString stuNameNow;
        QString stuIDNow;
        QString yearNow;
        QString scores;
        QString courseID;
        QSqlQuery myfunction;
        bool scoresOnly=TeacherPage->scores_checkBox->isChecked();
        //判断课程名称是否符合
        if((courseName.isEmpty()||course.value(0).toString()==courseName)&&course.value(2).toBool()) courseNameNow=course.value(0).toString();
        else continue;
        courseID=course.value(1).toString();
        stuscores.exec(QString("SELECT StuID,Scores,courseTime FROM stuscores WHERE courseName=%1").arg(courseID));
        //在学生课表里循环遍历所有选了该课的学生
        while(stuscores.next()){
            //判断学生学号
            if((stuID.isEmpty()||stuscores.value(0).toString()==stuID)) stuIDNow=stuscores.value(0).toString();
            else continue;
            student.exec(QString("SELECT StuName FROM student WHERE StuID=%1").arg(stuIDNow));
            student.next();
            //判断学生姓名
            if((stuName.isEmpty()||student.value(0).toString()==stuName)) stuNameNow=student.value(0).toString();
            else continue;
            //判断学年
            if((year.isEmpty()||stuscores.value(2).toString()==year)) yearNow=stuscores.value(2).toString();
            else continue;
            scores=stuscores.value(1).toString();
            if(scoresOnly&&!scores.isEmpty()) continue;//在只查询没有成绩的情况下，排除有成绩的学生
            TeacherPage->scores_table->insertRow(rowCount);
            TeacherPage->scores_table->setItem(rowCount,0,new QTableWidgetItem(stuIDNow));
            TeacherPage->scores_table->setItem(rowCount,1,new QTableWidgetItem(stuNameNow));
            TeacherPage->scores_table->setItem(rowCount,2,new QTableWidgetItem(courseNameNow));
            TeacherPage->scores_table->setItem(rowCount,3,new QTableWidgetItem(yearNow));
            myfunction.exec("SELECT ScoreEdit FROM myfunction");
            myfunction.next();
            if(scores.isEmpty()&&myfunction.value(0).toBool()){//开始选课时才有录入成绩的按钮
                //在表格末尾添加成绩的按钮
                QPushButton *button = new QPushButton("录入成绩");
                TeacherPage->scores_table->setCellWidget(rowCount, 4, button);
                connect(button, &QPushButton::released, this, [=](){
                    ScoresDialog dialog(nullptr);//弹出录入成绩的窗口
                    if (dialog.exec() == QDialog::Accepted)//如果录入成绩了
                    {
                        int tmpScore=dialog.score;//获取成绩
                        QSqlQuery updateQuery;
                        QString updateQueryString = QString("UPDATE stuscores SET Scores = :new_value,Acredit = :new_value1 WHERE courseName = '%1' AND StuID = '%2'").arg(courseID,stuIDNow);
                        updateQuery.prepare(updateQueryString);

                        // 绑定新值到占位符
                        float tmpAcredit=tmpScore/10.0-5;
                        updateQuery.bindValue(":new_value", QString::number(tmpScore));
                        updateQuery.bindValue(":new_value1", QString::number(tmpAcredit));
                        // 执行更新操作
                        if (updateQuery.exec())//成功更改数据库
                        {
                            onScoreCheck();//刷新页面，重新显示
                        }
                        else
                        {
                            QMessageBox::information(nullptr,"提示","录入成绩失败，请重新尝试");
                        }
                    }
                });
            }
            else TeacherPage->scores_table->setItem(rowCount,4,new QTableWidgetItem(scores));
        }
    }
    TeacherPage->scores_table->resizeRowsToContents();
}
//登记学生成绩
void uiDesign::on_scores_button_released()
{
    onScoreCheck();
}
//查询评教信息
void uiDesign::on_evaluation_button_released()
{
    deleteTabelItem(TeacherPage->evaluation_table);
    QSqlQuery stuscores;
    QSqlQuery teacher;
    QSqlQuery harrycourse;
    QString teacherName;
    QString courseID;
    QString courseName=TeacherPage->evaluationCourseName_Box->currentText();
    QString year=TeacherPage->evaluationYear_Box->currentText();
    QString evaluation;//评教分数
    QString comments;//评语
    teacher.exec(QString("SELECT TeacherName From teacher WHERE TeacherID =%1").arg(id));
    teacher.next();
    teacherName=teacher.value(0).toString();
    harrycourse.exec(QString("SELECT courseName,courseID FROM harrycourse WHERE isAvailable ='1' AND Teacher ='%1'").arg(teacherName));
    int rowCount=1;
    while(harrycourse.next()){
        QString courseNameNow;
        if(courseName.isEmpty()||courseName==harrycourse.value(0).toString()) courseNameNow=harrycourse.value(0).toString();
        else continue;
        courseID=harrycourse.value(1).toString();
        stuscores.exec(QString("SELECT evaluation,courseTime,evaluationComment FROM stuscores WHERE courseName =%1").arg(courseID));
        QString yearNow;
        while(stuscores.next()){
            if(year.isEmpty()||year==stuscores.value(1).toString()) yearNow=stuscores.value(1).toString();
            else continue;
            evaluation=stuscores.value(0).toString();
            comments=stuscores.value(2).toString();
            TeacherPage->evaluation_table->insertRow(rowCount);
            TeacherPage->evaluation_table->setItem(rowCount,0,new QTableWidgetItem(QString::number(rowCount)));
            TeacherPage->evaluation_table->setItem(rowCount,1,new QTableWidgetItem(courseID));
            TeacherPage->evaluation_table->setItem(rowCount,2,new QTableWidgetItem(courseNameNow));
            TeacherPage->evaluation_table->setItem(rowCount,3,new QTableWidgetItem(yearNow));
            TeacherPage->evaluation_table->setItem(rowCount,4,new QTableWidgetItem(evaluation));
            TeacherPage->evaluation_table->setItem(rowCount,5,new QTableWidgetItem(comments));
            rowCount++;
        }
    }
    TeacherPage->evaluation_table->resizeRowsToContents();
}
//查询本学年评教人数
void uiDesign::on_evaluationCount_button_released()
{
    QSqlQuery stuscores;
    QSqlQuery teacher;
    QSqlQuery harrycourse;
    QString teacherName;
    QString courseID;
    QString courseName=TeacherPage->evaluationCourseName_Box->currentText();
    QString year=TeacherPage->evaluationYear_Box->currentText();
    QString evaluation;//评教分数
    if(year!="2023") {
        QMessageBox::information(NULL, QStringLiteral("错误"), QStringLiteral("请选择当前学年"));
        return;
    }
    QSqlQuery myfunction;
    myfunction.exec("SELECT * FROM myfunction");
    if(!myfunction.value(1).toBool()){
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::NoIcon);
        msgbox.setText("当前评教未开始");
        msgbox.setWindowTitle("评教信息");
        msgbox.exec();
        return;
    }
    teacher.exec(QString("SELECT TeacherName From teacher WHERE TeacherID =%1").arg(id));
    teacher.next();
    teacherName=teacher.value(0).toString();
    harrycourse.exec(QString("SELECT courseName,courseID FROM harrycourse WHERE isAvailable ='1' AND Teacher ='%1'").arg(teacherName));
    QString title="学生评教信息";
    QString information="";
    while(harrycourse.next()){
        QString courseNameNow;
        if(courseName.isEmpty()||courseName==harrycourse.value(0).toString()) courseNameNow=harrycourse.value(0).toString();
        else continue;
        courseID=harrycourse.value(1).toString();
        stuscores.exec(QString("SELECT evaluation,courseTime FROM stuscores WHERE courseName =%1").arg(courseID));
        int count=0;
        int noEvaCount=0;
        while(stuscores.next()){
            QString yearNow;
            if(year==stuscores.value(1).toString()) yearNow=stuscores.value(1).toString();
            else continue;
            evaluation=stuscores.value(0).toString();
            if(evaluation.isEmpty()) noEvaCount++;
            count++;
        }
        information+=courseNameNow+"：";
        if(noEvaCount!=0)information+="还有"+QString::number(noEvaCount)+"名同学未评教；共"+QString::number(count)+"名同学\n";
        else information+="全部学生已评教\n";
    }
    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::NoIcon);
    msgbox.setText(information);
    msgbox.setWindowTitle(title);
    msgbox.exec();

}
void uiDesign::on_evaluationAver_button_released()
{
    deleteTabelItem(TeacherPage->evaluation_table);
    QSqlQuery stuscores;
    QSqlQuery teacher;
    QSqlQuery harrycourse;
    QString teacherName;
    QString courseID;
    QString courseName=TeacherPage->evaluationCourseName_Box->currentText();
    QString year=TeacherPage->evaluationYear_Box->currentText();
    int evaluation;//评教分数
    int yearNow;
    teacher.exec(QString("SELECT TeacherName From teacher WHERE TeacherID =%1").arg(id));
    teacher.next();
    teacherName=teacher.value(0).toString();
    harrycourse.exec(QString("SELECT courseName,courseID FROM harrycourse WHERE isAvailable ='1' AND Teacher ='%1'").arg(teacherName));
    int rowCount=1;
    if(year.isEmpty()) yearNow=2016;
    else yearNow=year.toInt();
    while(harrycourse.next()){
        evaluation=0;
        QString courseNameNow;
        if(courseName.isEmpty()||courseName==harrycourse.value(0).toString()) courseNameNow=harrycourse.value(0).toString();
        else continue;
        courseID=harrycourse.value(1).toString();
        if(year.isEmpty()) yearNow++;
        stuscores.exec(QString("SELECT evaluation,courseTime FROM stuscores WHERE courseName =%1").arg(courseID));
        int num=0;//学生人数
        while(stuscores.next()){
            if((!year.isEmpty())&&QString::number(yearNow)!=stuscores.value(1).toString()) continue;
            evaluation+=stuscores.value(0).toInt();
            num++;
        }
        if(evaluation==0) continue;
        if(num!=0)evaluation/=num;
        else continue;
        TeacherPage->evaluation_table->insertRow(rowCount);
        TeacherPage->evaluation_table->setItem(rowCount,0,new QTableWidgetItem(QString::number(rowCount)));
        TeacherPage->evaluation_table->setItem(rowCount,1,new QTableWidgetItem(courseID));
        TeacherPage->evaluation_table->setItem(rowCount,2,new QTableWidgetItem(courseNameNow));
        TeacherPage->evaluation_table->setItem(rowCount,3,new QTableWidgetItem(QString::number(yearNow)));
        TeacherPage->evaluation_table->setItem(rowCount,4,new QTableWidgetItem(QString::number(evaluation)));
        rowCount++;
    }
    TeacherPage->evaluation_table->resizeRowsToContents();
}

//获取课表
void uiDesign::TeacherSchedule(QTableWidget* table){
    // 清空原来的表格数据
    for (int i = 1; i <=4; i++)
    {
        for(int j=2; j<=8; j++)
        {
            table->item(i,j)->setText("");
        }
    }
    //获取数据库信息
    QSqlQuery teacher;
    QSqlQuery harryCourse;
    QString name;
    QString courseName;
    QString studyDay;
    QString studyTime;
    teacher.exec(QString("SELECT TeacherName FROM teacher WHERE TeacherID = %1").arg(id));
    teacher.next();
    name=teacher.value(0).toString();
    harryCourse.exec(QString("SELECT * FROM harrycourse WHERE Teacher= '%1'").arg(name));
    while(harryCourse.next()){
        if(!harryCourse.value(0).toBool()) continue;//当value(0)为false时，代表该课程不予显示
        studyDay=harryCourse.value(2).toString();
        studyTime=harryCourse.value(3).toString();
        courseName=harryCourse.value(4).toString();
        //课表
        int day,time;
        day=getStudyDay(studyDay);
        time=getStudyTime(studyTime);
        table->item(time,day+1)->setText(courseName);
    }
    //调整列宽
    int num=table->columnCount();
    for(int i=0;i<num;i++){
        table->setColumnWidth(i,100);
    }
    table->setColumnWidth(0,70);
    table->setColumnWidth(1,120);
    //调整行宽
    num=table->rowCount();
    for(int i=0;i<num;i++){
        table->setRowHeight(i,50);
    }
}
//为组合框添加课程信息
void uiDesign::addTeacherCourseName(QComboBox* box){
    if(box->count()==1){
        QSqlQuery queryID;
        QString select="SELECT courseName,isAvailable FROM teacher JOIN harrycourse ON teacher.TeacherName = harrycourse.Teacher WHERE teacher.TeacherID= %1";
        queryID.exec(select.arg(id));
        if(!queryID.next()) return;
        else{
            do{
                QString courseName=queryID.value(0).toString();
                if(queryID.value(1).toBool()) box->addItem(courseName);
            }while(queryID.next());
        }
    }
}
//获取上课日期
int uiDesign::getStudyDay(QString day){
    if(day=="周日") return 1;
    else if(day=="周一") return 2;
    else if(day=="周二") return 3;
    else if(day=="周三") return 4;
    else if(day=="周四") return 5;
    else if(day=="周五") return 6;
    else return 7;
}
//获取上课时间
int uiDesign::getStudyTime(QString time){
    if(time=="8：00~10：20") return 1;
    else if(time=="10：40~12：10") return 2;
    else if(time=="15：30~18：00") return 3;
    else return 4;
}

