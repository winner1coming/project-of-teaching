#include "uidesign.h"
//学生页面的内容
//判断年级string
bool isCourseGradeIncluded(QString stugrade,QString coursegrade)
{
    stugrade=stugrade.at(0);
    return coursegrade.contains(stugrade);
}
//产生学生课表
void uiDesign::GenerateStudentSchedule(QTableWidget* table)
{
    // 清空原来的表格数据
    for (int i = 1; i <=4; i++)
    {
        for(int j=2; j<=8; j++)
        {
            table->item(i,j)->setText("");
        }
    }
    //课表列宽
    int maxwidth=0;
    //在stuscore里一个个找出所有已选课程
    QSqlQuery querymycourse;
    QString querymycourseString=QString("SELECT * FROM stuscores WHERE StuID = '%1'").arg(id);
    if(querymycourse.exec(querymycourseString))
    {
        while(querymycourse.next())
        {
            if(!isCourseGradeIncluded(getStudentInfo('G'),querymycourse.value(4).toString()))//判断学生学年是否与课程学年吻合
            {
                continue;
            }
            QString courseid=querymycourse.value(1).toString();//当前已有课程的courseid
            //从harrycourse里找courseid相符的课程
            QSqlQuery query;
            //QString queryString=FilterCoursesByInput("harrycourse","","","","",courseid,"","");
            QString queryString=QString("SELECT * FROM harrycourse WHERE courseid = '%1'").arg(courseid);

            if (query.exec(queryString))
            {
                while(query.next())
                {
                    int weekday;
                    int hour;
                    QString week = query.value(2).toString();
                    QString studyhour = query.value(3).toString();
                    QString coursename = query.value(4).toString();
                    QString place = query.value(6).toString();
                    QString InfoToShow;
                    if(table == StudentPage->StudentScheduleTable)
                    {InfoToShow = coursename+"\n\n"+"上课地点:"+place;}
                    else
                    {InfoToShow = coursename;}
                    if(week=="周日") weekday=2;
                    else if(week=="周一") weekday=3;
                    else if(week=="周二") weekday=4;
                    else if(week=="周三") weekday=5;
                    else if(week=="周四") weekday=6;
                    else if(week=="周五") weekday=7;
                    else if(week=="周六") weekday=8;
                    else weekday= 9;

                    if(studyhour=="8：00~10：20") hour=1;
                    else if(studyhour=="10：40~12：10") hour=2;
                    else if(studyhour=="15：30~18：00") hour=3;
                    else if(studyhour=="20：00~22：20") hour=4;
                    else
                    {
                        hour=1;
                        QMessageBox::warning(this,"课程查询提示","数据库课表时间错误，查询失败！");
                    }

                    table->item(hour,weekday)->setText(InfoToShow);
                    if(table->fontMetrics().horizontalAdvance(coursename)>maxwidth)
                    {
                        maxwidth=table->fontMetrics().horizontalAdvance(InfoToShow);
                    }
                }
            }
            else
            {
                QMessageBox::warning(this,"课程查询提示","查询失败！");
            }

        }
    }
    table->resizeRowsToContents();
    table->resizeColumnToContents(1);
}

QString uiDesign::getStudentInfo(char command)
{
    QSqlQuery query;
    QString queryString=QString("SELECT * FROM student WHERE StuID = '%1'").arg(id);
    int Info;
    switch(command)
    {
    case 'N'://name
    {
        Info=0;
        break;
    }
    case 'A'://Academy学院
    {
        Info=5;
        break;
    }
    case 'G'://grade年级
    {
        Info=7;
        break;
    }
    case 'C'://绩点
    {
        Info=8;
        break;
    }
    default:
    {
        return "Error";
    }
    }
    if (query.exec(queryString))
    {
        while (query.next())
        {
            return query.value(Info).toString();
        }
    }
    return "Error";
}

QString uiDesign::getCourseInfo(char command,QString courseid)
{
    QSqlQuery query;
    QString queryString=QString("SELECT * FROM harrycourse WHERE courseID = '%1'").arg(courseid);
    int Info;
    switch(command)
    {
    case 'T'://Teacher
    {
        Info=1;
        break;
    }
    case 'D'://studyDay
    {
        Info=2;
        break;
    }
    case 't'://studyTime
    {
        Info=3;
        break;
    }
    case 'N'://courseName
    {
        Info=4;
        break;
    }
    case 'C'://college
    {
        Info=5;
        break;
    }
    case 'P'://studyPlace
    {
        Info=6;
        break;
    }
    case 'S'://Semester
    {
        Info=7;
        break;
    }
    case 'y'://type
    {
        Info=8;
        break;
    }
    case 'c'://credit
    {
        Info=10;
        break;
    }
    case 'E'://examine
    {
        Info=11;
        break;
    }
    case 'H'://hour
    {
        Info=12;
        break;
    }
    default:
    {
        return "Error";
    }
    }
    if (query.exec(queryString))
    {
        while (query.next())
        {
            return query.value(Info).toString();
        }
    }
    return "Error";
}

//根据条件创建Query用的string
QString FilterCoursesByInput(QString table,QString institute,QString coursename,QString teacher,QString coursetype,QString coursecode,QString week,QString period)
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


//学生查询所有课程
void uiDesign::on_Query_InfoPushBtn_clicked()
{
    // 清空原来的表格数据
    int rowCount = StudentPage->CourseTableWidget->rowCount();
    for (int i = rowCount - 1; i > 0; --i)
    {
        StudentPage->CourseTableWidget->removeRow(i);
    }
    rowCount=1;

    //获取输入的查询信息
    QString institute=StudentPage->institute_InfoComboBox->currentText();
    QString coursename=StudentPage->coursename_InfolineEdit->text();
    QString teacher=StudentPage->teacherlineEdit->text();
    QString coursetype=StudentPage->coursetype_InfoComboBox->currentText();
    QString coursecode=StudentPage->coursecode_InfolineEdit->text();
    QString week=StudentPage->week_InfoComboBox->currentText();
    QString period=StudentPage->period_InfoComboBox->currentText();


    QSqlQuery query;
    QString queryString=FilterCoursesByInput("harrycourse",institute,coursename,teacher,coursetype,coursecode,week,period);

    if (query.exec(queryString))
    {
        while (query.next())
        {
            //在表格中增加一行数据
            addLineToCourseTable(StudentPage->CourseTableWidget, query, rowCount);
            rowCount++;
        }
        StudentPage->CourseTableWidget->resizeColumnsToContents();
        StudentPage->CourseTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    else
    {
        QMessageBox::warning(this,"课程查询提示","查询失败！");
    }
}

//学生查询已选课程
void uiDesign::on_Query_chosenPushBtn_clicked()
{
    // 清空原来的表格数据
    int rowCount = StudentPage->Course_chooseTableWidget->rowCount();
    for (int i = rowCount - 1; i > 0; --i)
    {
        StudentPage->Course_chooseTableWidget->removeRow(i);
    }
    rowCount=1;

    //获取输入的筛选信息
    QString institute=StudentPage->institute_chooseComboBox->currentText();
    QString coursename=StudentPage->coursename_chooselineEdit->text();
    QString teacher=StudentPage->teacher_chooselineEdit->text();
    QString coursetype=StudentPage->coursetype_chooseComboBox->currentText();
    QString coursecode=StudentPage->coursecode_chooselineEdit->text();
    QString week=StudentPage->week_chooseComboBox->currentText();
    QString period=StudentPage->period_chooseComboBox->currentText();

    //在stuscore里一个个找出所有已选课程
    QSqlQuery querymycourse;
    QString querymycourseString=QString("SELECT * FROM stuscores WHERE StuID = '%1'").arg(id);
    if(querymycourse.exec(querymycourseString))
    {
        while(querymycourse.next())
        {
            QString courseid=querymycourse.value(1).toString();//当前已有课程的courseid
            //判断coursecode是否符合
            if(!coursecode.isEmpty())//输入了coursecode
            {
                if(coursecode!=courseid)
                {
                    continue;
                }
            }
            if(querymycourse.value(4).toString()!= getStudentInfo('G'))
            {
                continue;
            }

            //从harrycourse里找courseid和其他条件都相符的课程
            QSqlQuery query;
            QString queryString=FilterCoursesByInput("harrycourse",institute,coursename,teacher,coursetype,courseid,week,period);

            if (query.exec(queryString))
            {
                while(query.next())
                {
                    //                    if(!isCourseGradeIncluded(getStudentInfo('G'),query.value(7).toString()))//判断学生学年是否与课程学年吻合
                    //                    {
                    //                        continue;
                    //                    }
                    //在表格中增加一行数据
                    addLineToCourseTable(StudentPage->Course_chooseTableWidget, query, rowCount);

                    QString type = query.value(8).toString();
                    bool iscancelable = true;
                    QSqlQuery querycancelable;
                    QString querycancelableString=QString("SELECT Selection FROM myfunction LIMIT 1");
                    if(querycancelable.exec(querycancelableString)&&querycancelable.next())
                    {
                        if(querycancelable.value(0).toInt()==0)
                            iscancelable = false;
                    }
                    if(type!="必修"&&type!="预习课程"&&iscancelable==true)//可退课，加按钮
                    {
                        QPushButton *button = new QPushButton("退课");
                        StudentPage->Course_chooseTableWidget->setCellWidget(rowCount, 13, button);
                        QString btn_coursename=query.value(4).toString();
                        QString score=querymycourse.value(2).toString();
                        // 连接按钮的点击事件
                        connect(button, &QPushButton::clicked, this, [=]()
                                {
                                    // 处理退课
                                    //QMessageBox::information(this,"提示",QString("此处是退课操作:id:'%1',课程名字：'%2'").arg(courseid,btn_coursename));
                                    // 创建一个消息框
                                    QMessageBox::StandardButton reply;
                                    reply = QMessageBox::question(nullptr, "确认退课", QString("确认要退课：《%1》吗？").arg(btn_coursename),
                                                                  QMessageBox::Yes|QMessageBox::No);

                                    // 判断用户的选择
                                    if (reply == QMessageBox::Yes)
                                    {
                                        // 用户点击了确定按钮，执行你的操作
                                        QSqlQuery deletequery;
                                        if(score.isNull())
                                        {
                                            QString deleteString=QString("DELETE FROM stuscores WHERE StuID = '%1' AND courseName = :value").arg(id);
                                            deletequery.prepare(deleteString);
                                            deletequery.bindValue(":value",courseid);
                                            if(deletequery.exec())
                                            {
                                                QMessageBox::information(this,"退课提示",QString("已成功退课《%1》").arg(btn_coursename));
                                                //刷新页面
                                                on_Query_chosenPushBtn_clicked();
                                                return;
                                            }
                                            else
                                            {
                                                QMessageBox::information(this,"退课提示","退课失败");
                                            }
                                        }
                                    }
                                });
                    }
                    rowCount++;
                }
            }
            else
            {
                QMessageBox::warning(this,"课程查询提示","查询失败！");
            }

        }
        StudentPage->Course_chooseTableWidget->resizeColumnsToContents();
    }
}

//学生查询可选课程
void uiDesign::on_Query_tochoosePushBtn_clicked()
{
    // 清空原来的表格数据
    int rowCount = StudentPage->Course_chooseTableWidget->rowCount();
    for (int i = rowCount - 1; i > 0; --i)
    {
        StudentPage->Course_chooseTableWidget->removeRow(i);
    }
    rowCount=1;

    //判断是否选课阶段
    QSqlQuery queryavailable;
    QString queryavailableString=QString("SELECT Selection FROM myfunction LIMIT 1");
    if(queryavailable.exec(queryavailableString)&&queryavailable.next())
    {
        if(queryavailable.value(0).toInt()==0)
        {
            QMessageBox::information(this,"选课提示","当前非选课阶段");
            return;
        }
    }

    //获取输入的筛选信息
    QString institute=StudentPage->institute_chooseComboBox->currentText();
    QString coursename=StudentPage->coursename_chooselineEdit->text();
    QString teacher=StudentPage->teacher_chooselineEdit->text();
    QString coursetype=StudentPage->coursetype_chooseComboBox->currentText();
    QString coursecode=StudentPage->coursecode_chooselineEdit->text();
    QString week=StudentPage->week_chooseComboBox->currentText();
    QString period=StudentPage->period_chooseComboBox->currentText();

    QSqlQuery query;
    QString queryString=FilterCoursesByInput("harrycourse",institute,coursename,teacher,coursetype,coursecode,week,period);

    if (query.exec(queryString))
    {
        while (query.next())
        {
            //判断学生学年是否与课程学年吻合
            if(!isCourseGradeIncluded(getStudentInfo('G'),query.value(7).toString()))
            {
                continue;
            }

            //判断这门课选过没有
            QString currentcourseid=query.value(9).toString();
            QSqlQuery queryischosen;
            queryischosen.exec(QString("SELECT COUNT(*) FROM stuscores WHERE courseName = '%1' AND StuID = '%2'").arg(currentcourseid,id));
            if(queryischosen.next())
            {
                int counter = queryischosen.value(0).toInt();
                if (counter > 0)//选过了
                {
                    continue;
                }
            }

            //判断是否是别人学院的专业课/预习课程
            QString currentcoursecollege = query.value(5).toString();
            QString currentcoursetype = query.value(8).toString();
            if(currentcoursetype=="专业课"||currentcoursetype=="预习课程")
            {
                if(currentcoursecollege!=getStudentInfo('A'))
                {
                    continue;
                }
            }

            //在表格中增加一行数据
            addLineToCourseTable(StudentPage->Course_chooseTableWidget, query, rowCount);
            //数据末尾添加选课按钮
            QPushButton *button = new QPushButton("选课");
            StudentPage->Course_chooseTableWidget->setCellWidget(rowCount, 13, button);
            //处理按钮事件要用的变量
            QString btn_coursename=query.value(4).toString();
            QString btn_coursetime=query.value(3).toString();
            QString btn_courseday=query.value(2).toString();
            connect(button, &QPushButton::clicked, this, [=]()
                    {
                        // 处理选课

                        //判断是否时间冲突
                        QSqlQuery queryhour;
                        QString queryhourString=QString("SELECT harrycourse.studyDay, harrycourse.studyTime, harrycourse.courseName "
                                                          "FROM stuscores "
                                                          "JOIN harrycourse ON stuscores.courseName = harrycourse.courseID "
                                                          "WHERE stuscores.StuID = '%1' AND courseGrade = '%2'").arg(id,getStudentInfo('G'));

                        if(queryhour.exec(queryhourString))
                        {
                            while(queryhour.next())
                            {
                                if(queryhour.value(0).toString()==btn_courseday&&queryhour.value(1).toString()==btn_coursetime)//与已有课程时间冲突
                                {
                                    QMessageBox::information(nullptr,"选课提示",QString("当前所选课程《%1》时间与你的课程《%2》时间冲突").arg(btn_coursename,queryhour.value(2).toString()));
                                    return;
                                }
                            }
                        }

                        // 创建一个消息框
                        QMessageBox::StandardButton reply;
                        reply = QMessageBox::question(nullptr, "确认选课", QString("确认要选课：《%1》吗？").arg(btn_coursename),
                                                      QMessageBox::Yes|QMessageBox::No);

                        // 判断用户的选择
                        if (reply == QMessageBox::Yes)
                        {
                            // 用户点击了确定按钮，执行你的操作
                            QSqlQuery insertquery;
                            QString insertqueryString="INSERT INTO stuscores (StuID, courseName, courseGrade, courseTime) VALUES (:value1, :value2, :value3, :value4)";

                            insertquery.prepare(insertqueryString);

                            // 绑定具体的值到占位符
                            insertquery.bindValue(":value1", id);
                            insertquery.bindValue(":value2", currentcourseid);  // 例如，整数类型的数据
                            insertquery.bindValue(":value3", getStudentInfo('G'));
                            insertquery.bindValue(":value4", 2023);

                            // 执行插入操作
                            if (insertquery.exec())
                            {
                                QMessageBox::information(this,"选课提示",QString("已成功选课《%1》").arg(btn_coursename));
                                //刷新页面
                                on_Query_tochoosePushBtn_clicked();
                                return;
                            }
                            else
                            {
                                QMessageBox::information(this,"选课提示","选课失败，请重新尝试");
                            }
                        }

                    });
            rowCount++;
        }
        StudentPage->Course_chooseTableWidget->resizeColumnsToContents();
    }
    else
    {
        QMessageBox::warning(this,"课程查询提示","查询失败！");
    }
}

//学生查询考试安排
void uiDesign::on_Query_TestPushBtn_clicked()
{
    // 清空原来的表格数据
    int rowCount = StudentPage->Course_TesttableWidget->rowCount();
    for (int i = rowCount - 1; i > 0; --i)
    {
        StudentPage->Course_TesttableWidget->removeRow(i);
    }
    rowCount=1;

    //获取输入的查询信息
    QString institute=StudentPage->institute_TestComboBox->currentText();
    QString coursename=StudentPage->coursename_TestlineEdit->text();
    QString teacher=StudentPage->teacher_TestlineEdit->text();
    QString coursetype=StudentPage->coursetype_TestcomboBox->currentText();
    QString coursecode=StudentPage->coursecode_TestlineEdit->text();

    //在stuscore里一个个找出所有已选课程
    QSqlQuery querymycourse;
    QString querymycourseString=QString("SELECT * FROM stuscores WHERE StuID = '%1'").arg(id);
    if(querymycourse.exec(querymycourseString))
    {
        while(querymycourse.next())
        {
            QString courseid=querymycourse.value(1).toString();//当前已有课程的courseid
            //判断coursecode是否符合
            if(!coursecode.isEmpty())//查询时输入了coursecode
            {
                if(coursecode!=courseid)
                {
                    continue;
                }
            }

            //从harrycourse里找courseid和其他条件都相符的课程
            QSqlQuery query;
            QString queryString=FilterCoursesByInput("harrycourse",institute,coursename,teacher,coursetype,courseid,"","");

            if (query.exec(queryString))
            {
                while(query.next())
                {
                    //判断学生学年是否与课程学年吻合
                    if(!isCourseGradeIncluded(getStudentInfo('G'),query.value(7).toString()))
                    {
                        continue;
                    }

                    //增加一行数据
                    StudentPage->Course_TesttableWidget->insertRow(rowCount);
                    // 处理查询结果
                    QString Teacher = query.value(1).toString();
                    QString studyDay = query.value(2).toString();
                    QString studyTime = query.value(3).toString();
                    QString courseName = query.value(4).toString();
                    QString college = query.value(5).toString();
                    QString studyPlace = query.value(6).toString();
                    if(studyPlace=="通常在海格的小屋外，有时也会到禁林中上课") studyPlace="禁林";
                    QString studySemester = query.value(7).toString();
                    QString type = query.value(8).toString();
                    //QString courseID = query.value(9).toString();
                    QString credit = query.value(10).toString();
                    QString examine = query.value(11).toString();
                    QString credithour=query.value(12).toString();

                    StudentPage->Course_TesttableWidget->setItem(rowCount, 0, new QTableWidgetItem(courseName));
                    StudentPage->Course_TesttableWidget->setItem(rowCount, 1, new QTableWidgetItem(college));
                    StudentPage->Course_TesttableWidget->setItem(rowCount, 2, new QTableWidgetItem(type));
                    StudentPage->Course_TesttableWidget->setItem(rowCount, 3, new QTableWidgetItem(credit));
                    StudentPage->Course_TesttableWidget->setItem(rowCount, 4, new QTableWidgetItem(credithour));
                    StudentPage->Course_TesttableWidget->setItem(rowCount, 5, new QTableWidgetItem(Teacher));
                    StudentPage->Course_TesttableWidget->setItem(rowCount, 6, new QTableWidgetItem(studyDay));
                    StudentPage->Course_TesttableWidget->setItem(rowCount, 7, new QTableWidgetItem(studyTime));
                    StudentPage->Course_TesttableWidget->setItem(rowCount, 8, new QTableWidgetItem(studySemester));
                    StudentPage->Course_TesttableWidget->setItem(rowCount, 9, new QTableWidgetItem(studyPlace));
                    StudentPage->Course_TesttableWidget->setItem(rowCount, 10, new QTableWidgetItem(examine));
                    rowCount++;
                }
                StudentPage->Course_TesttableWidget->resizeColumnsToContents();
            }
            else
            {
                QMessageBox::warning(this,"考试信息查询提示","查询失败！");
            }

        }
    }
}

//学生查询成绩单
void uiDesign::on_StuSearchScore_PBtn_clicked()
{
    // 清空原来的表格数据
    int rowCount = StudentPage->scoreTable->rowCount();
    for (int i = rowCount - 1; i > 0; --i)
    {
        StudentPage->scoreTable->removeRow(i);
    }
    rowCount=1;

    StudentPage->averageScorelabel->setText("");

    //获取输入的筛选信息
    bool isCompulsory=false;
    bool isElective=false;
    bool isSpecialized=false;
    bool isPreview=false;
    if(StudentPage->compulsoryCourse_checkBox->isChecked())//勾选“必修”
    {
        isCompulsory=true;
    }
    if(StudentPage->electiveCourse_checkBox->isChecked())//勾选“选修”
    {
        isElective=true;
    }
    if(StudentPage->specializedCourse_checkBox->isChecked())//勾选“专业课”
    {
        isSpecialized=true;
    }
    if(StudentPage->previewCourse_checkBox->isChecked())//勾选“预习课程”
    {
        isPreview=true;
    }
    QString InputGrade=StudentPage->grade_comboBox->currentText();

    //平均绩点
    float totalScore=0;
    float totalcredit=0;

    //在stuscore里一个个找出所有已选课程
    QSqlQuery querymycourse;
    QString querymycourseString=QString("SELECT * FROM stuscores WHERE StuID = '%1' ORDER BY courseName DESC").arg(id);
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
            if(InputGrade!="所有")
            {
                if(coursegrade!=InputGrade)
                    continue;
            }
            QString ispass;
            if(gradePoints.at(0)>='1') ispass="是";
            else if(gradePoints.at(0)<'1') ispass="否";

            QSqlQuery query;
            QString queryString=QString("SELECT * FROM harrycourse WHERE courseID = '%1'").arg(courseid);
            if (query.exec(queryString))
            {
                while(query.next())
                {
                    QString coursetype= query.value(8).toString();
                    if((coursetype=="必修"&&isCompulsory==true)||(coursetype=="选修"&&isElective==true)||(coursetype=="专业课"&&isSpecialized==true)||(coursetype=="预习课程"&&isPreview==true))
                    {


                        //增加一行数据
                        StudentPage->scoreTable->insertRow(rowCount);
                        // 处理查询结果
                        QString Teacher = query.value(1).toString();
                        QString courseName = query.value(4).toString();
                        //QString college = query.value(5).toString();
                        QString studySemester = querymycourse.value(4).toString();
                        QString credit = query.value(10).toString();
                        QString examine = query.value(11).toString();

                        //对平均绩点的操作
                        totalScore+=(gradePoints.toFloat())*credit.toFloat();
                        totalcredit+=credit.toFloat();


                        StudentPage->scoreTable->setItem(rowCount, 0, new QTableWidgetItem(coursetype));
                        StudentPage->scoreTable->setItem(rowCount, 1, new QTableWidgetItem(courseName));
                        StudentPage->scoreTable->setItem(rowCount, 2, new QTableWidgetItem(Teacher));
                        StudentPage->scoreTable->setItem(rowCount, 3, new QTableWidgetItem(studySemester));
                        StudentPage->scoreTable->setItem(rowCount, 4, new QTableWidgetItem(examine));
                        StudentPage->scoreTable->setItem(rowCount, 5, new QTableWidgetItem(credit));
                        StudentPage->scoreTable->setItem(rowCount, 6, new QTableWidgetItem(originalscores));
                        StudentPage->scoreTable->setItem(rowCount, 7, new QTableWidgetItem(gradePoints));
                        StudentPage->scoreTable->setItem(rowCount, 8, new QTableWidgetItem(ispass));

                        rowCount++;
                    }
                }

            }
            else
            {
                QMessageBox::warning(this,"成绩查询提示","查询失败！");
            }

        }
        if(totalcredit!=0)
        {
            //计算平均绩点
            float averagescore=totalScore/totalcredit;
            QString scoreString = QString::number(static_cast<double>(averagescore), 'f', 4);
            StudentPage->averageScorelabel->setText(scoreString);
        }
    }
}

//学生评教
class RatingDialog : public QDialog
{
public:
    int ratingValue;
    QString ratingComment;
    RatingDialog(QWidget *parent = nullptr) : QDialog(parent),ratingValue(-1)
    {
        setWindowTitle("评教");
        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *promptLabel = new QLabel("请输入你的评分（0~100）：", this);
        layout->addWidget(promptLabel);

        QLineEdit *lineEdit = new QLineEdit(this);
        layout->addWidget(lineEdit);

        QLabel *wordLabel = new QLabel("请输入你的评语：", this);
        layout->addWidget(wordLabel);

        QLineEdit *wordlineEdit = new QLineEdit(this);
        layout->addWidget(wordlineEdit);

        QPushButton *okButton = new QPushButton("确定", this);
        QPushButton *cancelButton = new QPushButton("取消", this);

        connect(okButton, &QPushButton::clicked, this, [=]() {
            bool ok;
            int rating = lineEdit->text().toInt(&ok);
            QString comment = wordlineEdit->text();
            int commentlength = comment.length();
            int test = commentlength;
            if (ok && rating >= 0 && rating <= 100 && comment.length()<255)
            {
                // 处理有效的评分
                ratingValue=lineEdit->text().toInt(&ok);
                ratingComment = wordlineEdit->text();
                accept();
            }
            else
            {
                // 显示错误提示
                if((rating < 0 || rating > 100) && comment.length()>=255)
                {
                    QMessageBox::critical(this, "错误", "评语超出字数限制；另外评分请输入0-100之间的数字");
                }
                else if(rating < 0 || rating > 100)
                {
                    QMessageBox::critical(this, "错误", "评分请输入0-100之间的数字");
                }
                else if(comment.length()>=255)
                {
                    QMessageBox::critical(this, "错误", "评语超出字数限制");
                }
            }
        });

        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);

        layout->addLayout(buttonLayout);

        setLayout(layout);
    }
    virtual ~RatingDialog(){}
};
void uiDesign::StudentEvaluationOnChosenCourses()
{
    // 清空原来的表格数据
    int rowCount = StudentPage->evaluation_table->rowCount();
    for (int i = rowCount - 1; i > 0; --i)
    {
        StudentPage->evaluation_table->removeRow(i);
    }
    rowCount=1;

    //获取输入的筛选信息
    QString coursename=StudentPage->evl_course_lineEdit->text();
    QString teacher=StudentPage->evl_teacher_lineEdit->text();

    //在stuscore里一个个找出所有已选课程且没有评教过的
    QSqlQuery querymycourse;
    QString querymycourseString=QString("SELECT * FROM stuscores WHERE StuID = '%1' AND courseGrade = '%2' ORDER BY evaluation IS NULL DESC").arg(id,getStudentInfo('G'));
    if(querymycourse.exec(querymycourseString))
    {
        while(querymycourse.next())
        {
            QString courseid=querymycourse.value(1).toString();//当前已有课程的courseid
            //从harrycourse里找条件都相符的课程
            QSqlQuery query;
            QString queryString;
            if(coursename!=""||teacher!="")
            {
                queryString = "SELECT * FROM harrycourse WHERE";
                // 判断并添加筛选条件（Teacher列）
                if (!teacher.isEmpty())
                {
                    queryString += QString(" Teacher LIKE '%%1%' AND").arg(teacher);
                }
                if (!coursename.isEmpty())// （courseName列）
                {
                    queryString += QString(" courseName LIKE '%%1%' AND").arg(coursename);
                }
                queryString+=QString(" courseID = '%1'").arg(courseid);
            }
            else
            {
                queryString=QString("SELECT * FROM harrycourse WHERE courseID = '%1'").arg(courseid);
            }
            //QString queryString=QString("SELECT * FROM harrycourse WHERE courseID = '%1' AND courseName = '%2' AND Teacher = '%3'").arg(courseid,coursename,teacher);
            if (query.exec(queryString))
            {
                while(query.next())
                {
                    //增加一行
                    StudentPage->evaluation_table->insertRow(rowCount);
                    // 处理查询结果
                    QString Teacher = query.value(1).toString();
                    QString college = query.value(5).toString();
                    QString type = query.value(8).toString();
                    QString courseName=query.value(4).toString();
                    //QString courseID = query.value(9).toString();

                    StudentPage->evaluation_table->setItem(rowCount, 0, new QTableWidgetItem(QString::number(rowCount)));
                    StudentPage->evaluation_table->setItem(rowCount, 1, new QTableWidgetItem(courseid));
                    StudentPage->evaluation_table->setItem(rowCount, 2, new QTableWidgetItem(courseName));
                    StudentPage->evaluation_table->setItem(rowCount, 3, new QTableWidgetItem(type));
                    StudentPage->evaluation_table->setItem(rowCount, 4, new QTableWidgetItem(college));
                    StudentPage->evaluation_table->setItem(rowCount, 5, new QTableWidgetItem(Teacher));

                    bool isavailable = true;
                    QSqlQuery queryavailable;
                    QString queryavailableString=QString("SELECT Evaluation FROM myfunction LIMIT 1");
                    if(queryavailable.exec(queryavailableString)&&queryavailable.next())
                    {
                        if(queryavailable.value(0).toInt()==0)
                        {
                            isavailable = false;
                        }
                    }

                    if(querymycourse.value(5).isNull())//还没评分，加lineEdit
                    {
                        if(isavailable==false)
                        {
                            StudentPage->evaluation_table->setItem(rowCount, 6, new QTableWidgetItem("未评分"));
                            rowCount++;
                            continue;
                        }
                        QPushButton *button = new QPushButton("去评分");
                        StudentPage->evaluation_table->setCellWidget(rowCount, 6, button);
                        QString stuGrade=getStudentInfo('G');
                        // 连接按钮的点击事件
                        connect(button, &QPushButton::clicked, this, [courseid,stuGrade,this]()
                                {

                                    RatingDialog dialog(nullptr);//弹出评分窗口并执行评分
                                    if (dialog.exec() == QDialog::Accepted)//如果评分了
                                    {
                                        int evaluationScore=dialog.ratingValue;//获取评分
                                        QString evaluationComment = dialog.ratingComment;//获取评语
                                        QSqlQuery updateQuery;
                                        QString updateQueryString = QString("UPDATE stuscores SET evaluation = :new_value, evaluationComment = :new_valueComment WHERE courseName = '%1' AND courseGrade = '%2' AND StuID = '%3'").arg(courseid,stuGrade,id);
                                        updateQuery.prepare(updateQueryString);

                                        // 绑定新值到占位符
                                        updateQuery.bindValue(":new_value", QString::number(evaluationScore));
                                        updateQuery.bindValue(":new_valueComment", evaluationComment);

                                        // 执行更新操作
                                        if (updateQuery.exec())//成功更改数据库
                                        {
                                            StudentEvaluationOnChosenCourses();//刷新页面，重新显示
                                        }
                                        else
                                        {
                                            QMessageBox::information(nullptr,"评教提示","评教失败，请重新尝试");
                                        }
                                    }
                                });
                    }
                    else//已经评分过了
                    {
                        StudentPage->evaluation_table->setItem(rowCount, 6, new QTableWidgetItem(querymycourse.value(5).toString()));
                    }
                    rowCount++;
                }
            }
            else
            {
                //QMessageBox::warning(this,"课程查询提示","查询失败！");
                break;
            }

        }
        StudentPage->evaluation_table->resizeColumnsToContents();
    }
}

void uiDesign::on_queryEvaluation_Btn_clicked()
{
    StudentEvaluationOnChosenCourses();
}
