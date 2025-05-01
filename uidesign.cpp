#include "uidesign.h"
#include "qevent.h"
#include "ui_uidesign.h"

uiDesign::uiDesign(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::uiDesign)
{
    ui->setupUi(this);
    StudentPage=new Ui::Student;
    TeacherPage=new Ui::Teacher;
    AdminPage=new Ui::Admin;
    setWindowTitle(tr("教务系统"));
    setWindowIcon(QIcon(":/icon/resource/icon/magic.png"));
    setCursor(QCursor(QPixmap(":/icon/resource/icon/mouse.png")));
    this->setAutoFillBackground(true);
    ui->mainPage->setCurrentIndex(0);
    //添加新页面
    StudentPage->setupUi(ui->mainPage);
    ui->mainPage->addWidget(StudentPage->courseInfoP);
    ui->mainPage->addWidget(StudentPage->courseChooseP);
    ui->mainPage->addWidget(StudentPage->scheduleP);
    ui->mainPage->addWidget(StudentPage->gradesP);
    ui->mainPage->addWidget(StudentPage->testP);
    ui->mainPage->addWidget(StudentPage->evaluationP);
    TeacherPage->setupUi(ui->mainPage);
    ui->mainPage->addWidget(TeacherPage->TeacherCourseInfoP);
    ui->mainPage->addWidget(TeacherPage->TeacherCourseStudentP);
    ui->mainPage->addWidget(TeacherPage->TeacherGradesP);
    ui->mainPage->addWidget(TeacherPage->TeacherEvaluationP);
    AdminPage->setupUi(ui->mainPage);
    ui->mainPage->addWidget(AdminPage->ACourseInfoP);
    ui->mainPage->addWidget(AdminPage->ATeacherInfoP);
    ui->mainPage->addWidget(AdminPage->AStudentInfoP);
    ui->mainPage->addWidget(AdminPage->APasswordChangeP);
    ui->mainPage->addWidget(AdminPage->ANoticeP);
    ui->mainPage->addWidget(AdminPage->AManagementP);
    ui->mainPage->addWidget(AdminPage->StuScores);


    //槽函数
    connect(StudentPage->Query_InfoPushBtn,SIGNAL(clicked()), this, SLOT(on_Query_InfoPushBtn_clicked()));
    connect(StudentPage->Query_chosenPushBtn,SIGNAL(clicked()),this, SLOT(on_Query_chosenPushBtn_clicked()));
    connect(StudentPage->Query_tochoosePushBtn,SIGNAL(clicked()),this, SLOT(on_Query_tochoosePushBtn_clicked()));
    connect(StudentPage->Query_TestPushBtn,SIGNAL(clicked()),this, SLOT(on_Query_TestPushBtn_clicked()));
    connect(StudentPage->StuSearchScore_PBtn,SIGNAL(clicked()),this, SLOT(on_StuSearchScore_PBtn_clicked()));
    connect(StudentPage->queryEvaluation_Btn,SIGNAL(clicked()),this,SLOT(on_queryEvaluation_Btn_clicked()));

    connect(TeacherPage->StuInfoButton,SIGNAL(released()),this,SLOT(on_TStuInfoButton_released()));
    connect(TeacherPage->scores_button,SIGNAL(released()),this,SLOT(on_scores_button_released()));
    connect(TeacherPage->evaluation_button,SIGNAL(released()),this,SLOT(on_evaluation_button_released()));
    connect(TeacherPage->evaluationCount_button,SIGNAL(released()),this,SLOT(on_evaluationCount_button_released()));
    connect(TeacherPage->evaluationAver_button,SIGNAL(released()),this,SLOT(on_evaluationAver_button_released()));

    connect(AdminPage->Aquery_pushButton,SIGNAL(clicked()),this,SLOT(on_Aquery_pushButton_clicked()));
    connect(AdminPage->query_pushButton1,SIGNAL(clicked()),this,SLOT(on_query_pushButton1_clicked()));
    connect(AdminPage->pushButton_6,SIGNAL(clicked()),this,SLOT(on_pushButton_6_clicked()));
    connect(AdminPage->query_pushButton2,SIGNAL(clicked()),this,SLOT(on_query_pushButton2_clicked()));
    connect(AdminPage->Start_pushButton,SIGNAL(clicked()),this,SLOT(on_Start_pushButton_clicked()));
    connect(AdminPage->close_pushButton,SIGNAL(clicked()),this,SLOT(on_close_pushButton_clicked()));
    connect(AdminPage->pushButton_11,SIGNAL(clicked()),this,SLOT(on_pushButton_11_clicked()));
    connect(AdminPage->pushButton_12,SIGNAL(clicked()),this,SLOT(on_pushButton_12_clicked()));
    connect(AdminPage->ScoreB_pushButton,SIGNAL(clicked()),this,SLOT(on_ScoreB_pushButton_clicked()));
    connect(AdminPage->ScoreC_pushButton,SIGNAL(clicked()),this,SLOT(on_ScoreC_pushButton_clicked()));
    connect(AdminPage->add_pushButton,SIGNAL(clicked()),this,SLOT(on_add_pushButton_clicked()));
    connect(AdminPage->addTer_pushButton,SIGNAL(clicked()),this,SLOT(on_addTer_pushButton_clicked()));
    connect(AdminPage->pushButton,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
    connect(AdminPage->pushButton_2,SIGNAL(clicked()),this,SLOT(on_pushButton_2_clicked()));
    connect(AdminPage->pushButton_3,SIGNAL(clicked()),this,SLOT(on_pushButton_3_clicked()));
    //以下为样式设置
    //字体
    int fontId = QFontDatabase::addApplicationFont(":/font/resource/myFont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontName);
    QApplication::setFont(font);
    //屏幕
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect rect = screen->availableGeometry();
    int w=rect.size().width()*0.95;
    int h=rect.size().height()*0.95;
    this->setFixedSize(w,h);
    //
    QFile file(":/qss/resource/all.qss");
    if(file.open(QFile::ReadOnly)){
        QString styleSheet = QLatin1String(file.readAll());  //QByteArray可直接转换为QString
        setStyleSheet(styleSheet);
        file.close();
    }else{
        QMessageBox::information(this,"tip","Failed to open file");
    }

    //其他控件初始设置
    //学生页面中的控件初始化设置
    StudentPage->scoreTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //StudentPage->StudentScheduleTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    StudentPage->StudentScheduleTable->resizeColumnsToContents();
    StudentPage->StudentScheduleTable->resizeRowsToContents();
    StudentPage->compulsoryCourse_checkBox->setChecked(true);//默认勾选学生查询成绩页面的所有课程类型
    StudentPage->electiveCourse_checkBox->setChecked(true);
    StudentPage->specializedCourse_checkBox->setChecked(true);
    StudentPage->previewCourse_checkBox->setChecked(true);
    InitializeScheduleTable(StudentPage->StudentScheduleTable);//初始化学生课程表
    InitializeScheduleTable(ui->HomeStudentScheduleTable);//初始化首页学生课表

    //教师页面控件初始化设置
    InitializeScheduleTable(TeacherPage->Schedule_table);
    InitializeScheduleTable(ui->TeacherSchedule_table);

    //菜单显示
    ui->PasswordChange->setVisible(false);
    ui->name->setVisible(false);
    ui->home->setVisible(false);
    ui->exit->setVisible(false);
}
uiDesign::~uiDesign()
{
    delete ui;
    delete StudentPage;
}

void uiDesign::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);//消除编译器对event未使用的警告
    QPainter painter(this);

    if(institutionRole==0) painter.drawPixmap(0,0,this->width(),this->height(), QPixmap(":/images/resource/img/background.png"));
    else if(institutionRole==1) painter.drawPixmap(0,0,this->width(),this->height(), QPixmap(":/images/resource/img/glfd.png"));
    else if(institutionRole==2) painter.drawPixmap(0,0,this->width(),this->height(), QPixmap(":/images/resource/img/sltl.png"));
    else if(institutionRole==3) painter.drawPixmap(0,0,this->width(),this->height(), QPixmap(":/images/resource/img/lwkl.png"));
    else painter.drawPixmap(0,0,this->width(),this->height(), QPixmap(":/images/resource/img/sqpq.png"));

}
void uiDesign::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        // 左键点击事件
        setCursor(QCursor(QPixmap(":/icon/resource/icon/click.png")));
    }
}
void uiDesign::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        // 左键释放事件
        setCursor(QCursor(QPixmap(":/icon/resource/icon/mouse.png")));
    }
}


//公告
void uiDesign::showNoice(QTableWidget* table, QString userID){
    deleteTabelItem(table);
    QSqlQuery noticep;
    noticep.exec("SELECT * FROM noticep");
    int rowCount=1;
    while(noticep.next()){
        QString time=noticep.value(0).toString();
        QString title=noticep.value(1).toString();
        QString content=noticep.value(2).toString();
        QString ID = noticep.value(3).toString();
        table->insertRow(rowCount);
        table->setItem(rowCount,0,new QTableWidgetItem(time));
        table->setItem(rowCount,1,new QTableWidgetItem(title));
        QPushButton *button = new QPushButton("查看详情");

        QSqlQuery isread_query;
        isread_query.prepare("SELECT COUNT(*) FROM isRead WHERE peopleID = ? AND NoticeID = ?"); //我需要登录的用户的账号
        isread_query.addBindValue(userID);
        isread_query.addBindValue(ID);
        isread_query.exec();
        isread_query.next();
        bool isread = isread_query.value(0).toInt()>0;
        QPixmap pixmap(10,10);
        if (isread) {
            pixmap.fill(Qt::green);
        }else {
            pixmap.fill(Qt::red);
        }

        QIcon icon(pixmap);
        button->setIcon(icon);
        button->setIconSize(QSize(10,10));

        table->setCellWidget(rowCount, 2, button);
        connect(button, &QPushButton::released, this, [=](){
            QMessageBox msgbox;
            msgbox.setIcon(QMessageBox::NoIcon);
            msgbox.setText(content);
            msgbox.setWindowTitle(time+" "+title);
            msgbox.exec();

            if(!isread){
                QSqlQuery updateRead;
                updateRead.prepare("INSERT INTO isread (peopleID, NocticeID) VALUES (?, ?)");
                updateRead.addBindValue(userID);
                updateRead.addBindValue(ID);
                updateRead.exec();

                QPixmap pixmap(10,10);
                pixmap.fill(Qt::green);
                QIcon icon(pixmap);
                button->setIcon(icon);
                button->setIconSize(QSize(10,10));
            }
        });
    }
    table->resizeColumnsToContents();
}
//是否显示密码
void showpassword(QLineEdit* line,QToolButton* button){
    QIcon nosee(QPixmap(":/icon/resource/icon/nosee.png"));
    QIcon see(QPixmap(":/icon/resource/icon/see.png"));
    if(button->text()=="不可见"){
        button->setText("可见");
        button->setIcon(see);
        line->setEchoMode(QLineEdit::Normal);
    }
    else{
        button->setText("不可见");
        button->setIcon(nosee);
        line->setEchoMode(QLineEdit::Password);
    }
}
//登录展示密码
void uiDesign::on_toolButton_released()
{
    showpassword(ui->password,ui->toolButton);
}
//学院身份
int InRole(QString academy){
    if(academy=="格兰芬多") return 1;
    else if(academy=="斯莱特林")return 2;
    else if(academy=="拉文克劳") return 3;
    else return 4;
}
void uiDesign::institutionrole(){
    if(role==1){
        QSqlQuery stu;
        stu.exec(QString("SELECT Academy FROM student WHERE StuID=%1").arg(id));
        stu.next();
        institutionRole=InRole(stu.value(0).toString());
    }
    else if(role==2){
        QSqlQuery teacher;
        teacher.exec(QString("SELECT college FROM teacher WHERE TeacherID=%1").arg(id));
        teacher.next();
        institutionRole=InRole(teacher.value(0).toString());
    }
    else{

    }
}
//登录按钮
void uiDesign::on_logButton_released()
{
    id=ui->ID->text();
    QString password;
    QString nameID;
    QSqlQuery query;
    if(ui->stuButton->isChecked()) {
        query.exec(QString("SELECT * FROM student WHERE StuID = %1 ").arg(id));
        if(!query.next()){
            QMessageBox::information(NULL, QStringLiteral("Error!"), QStringLiteral("账号不存在，请检查输入是否正确"));
            return;
        }
        password=query.value(6).toString();
        if(password==ui->password->text()){
            role=1;
            ui->mainPage->setCurrentIndex(1);
            nameID=query.value(0).toString();
            nameID+="(";
            nameID+=query.value(1).toString();
            nameID+=")";
            GenerateStudentSchedule(ui->HomeStudentScheduleTable);
            showNoice(ui->studentNotice_table, id);
            institutionrole();
        }
        else{
            QMessageBox::information(NULL, QStringLiteral("Error!"), QStringLiteral("密码错误，请检查输入是否正确"));
            return;
        }
    }
    else if(ui->teaButton->isChecked()) {
        query.exec(QString("SELECT * FROM teacher WHERE TeacherID = %1 ").arg(id));
        if(!query.next()){
            QMessageBox::warning(NULL, QStringLiteral("Error!"), QStringLiteral("账号不存在，请检查输入是否正确"));
            return;
        }
        password=query.value(2).toString();
        if(password==ui->password->text()){
            ui->mainPage->setCurrentIndex(2);
            role=2;
            nameID=query.value(0).toString();
            nameID+="(";
            nameID+=query.value(1).toString();
            nameID+=")";
            TeacherSchedule(ui->TeacherSchedule_table);
            showNoice(ui->teacherNotice_table, id);
            institutionrole();
        }
        else{
            QMessageBox::warning(NULL, QStringLiteral("Error!"), QStringLiteral("密码错误，请检查输入是否正确"));
            return;
        }
    }
    else if(ui->adminButton->isChecked()) {
        query.exec(QString("SELECT * FROM manager WHERE Account = '%1' ").arg(id));
        if(!query.next()){
            QMessageBox::warning(NULL, QStringLiteral("Error!"), QStringLiteral("账号不存在，请检查输入是否正确"));
            return;
        }
        password=query.value(2).toString();
        if(password==ui->password->text()){
            ui->mainPage->setCurrentIndex(3);
            role=3;
            nameID=query.value(0).toString();
            nameID+="(";
            nameID+=query.value(1).toString();
            nameID+=")";
        }
        else{
            QMessageBox::warning(NULL, QStringLiteral("Error!"), QStringLiteral("密码错误，请检查输入是否正确"));
            return;
        }
    }
    else {
        QMessageBox::warning(NULL, QStringLiteral("Error!"), QStringLiteral("请选择身份"));
        return;
    }
    ui->PasswordChange->setVisible(true);
    ui->name->setVisible(true);
    ui->home->setVisible(true);
    ui->exit->setVisible(true);
    ui->name->setText("欢迎您，"+nameID);
}

//菜单
//转到首页
void uiDesign::on_home_released()
{
    QString name = ui->name->text();
    QRegularExpression re("\\((\\d+)\\)");
    QRegularExpressionMatch match = re.match(name);

    if(match.hasMatch()){
        QString id = match.captured(1);
    }else {
        qDebug()<<"NO ID Found!";
    }

    if(role==1) {
        GenerateStudentSchedule(ui->HomeStudentScheduleTable);

        showNoice(ui->studentNotice_table, id);
        ui->mainPage->setCurrentIndex(1);
    }
    else if(role==2) {
        TeacherSchedule(ui->TeacherSchedule_table);
        showNoice(ui->teacherNotice_table, id);
        ui->mainPage->setCurrentIndex(2);
    }
    else if(role==3) ui->mainPage->setCurrentIndex(3);
}
//退出
void uiDesign::on_exit_released()
{
    ui->toolButton->setText("不可见");
    ui->toolButton->setIcon(QPixmap(":/icon/resource/icon/nosee.png"));
    ui->password->setEchoMode(QLineEdit::Password);
    institutionRole=0;
    role=0;
    ui->ID->setText("");
    ui->password->setText("");
    ui->mainPage->setCurrentIndex(0);
    ui->PasswordChange->setVisible(false);
    ui->name->setVisible(false);
    ui->home->setVisible(false);
    ui->exit->setVisible(false);
}

//转到密码修改页面
void uiDesign::on_PasswordChange_released()
{
    ui->mainPage->setCurrentIndex(4);
    ui->IDinPaswordChange->setText(id);
    ui->original_lineEdit->clear();
    ui->newpass_lineEdit->clear();
    ui->sure_lineEdit->clear();
    ui->original_lineEdit->setEchoMode(QLineEdit::Password);
    ui->newpass_lineEdit->setEchoMode(QLineEdit::Password);
    ui->sure_lineEdit->setEchoMode(QLineEdit::Password);
    ui->seePassword->setText("不可见");
    ui->seePassword->setIcon(QPixmap(":/icon/resource/icon/nosee.png"));
}
//查看原始密码
void uiDesign::on_seePassword_released()
{
    showpassword(ui->original_lineEdit,ui->seePassword);
}
//修改密码的具体操作
void uiDesign::fixPasskey(QSqlQuery &Query,QString passkey,QString table,QString IDname)
{
    if(!ui->original_lineEdit->text().isEmpty())
    {
        if(ui->original_lineEdit->text()==Query.value(0).toString())
        {
            if(!ui->newpass_lineEdit->text().isEmpty())
            {
                if(ui->newpass_lineEdit->text().size()>8||ui->newpass_lineEdit->text().size()<6)
                {
                    QMessageBox::information(this,"提示","输入的密码长度应在6~8之间！");
                }
                else
                {
                    if(ui->newpass_lineEdit->text()!=ui->original_lineEdit->text())
                    {
                        if(!ui->sure_lineEdit->text().isEmpty())
                        {
                            if(ui->sure_lineEdit->text()==ui->newpass_lineEdit->text())
                            {
                                QSqlQuery query;
                                QString str=QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg(table,passkey,ui->newpass_lineEdit->text(),IDname,ui->IDinPaswordChange->text());
                                if(query.exec(str))
                                {
                                    QMessageBox::information(this,"成功","修改密码成功！\n请重新登录！");
                                    on_exit_released();
                                }
                                else
                                {
                                    QMessageBox::information(this,"失败","系统故障，修改密码失败！");
                                }

                            }
                            else
                            {
                                QMessageBox::information(this,"提示","新密码与确定的密码不一致！");
                            }
                        }
                        else
                        {
                            QMessageBox::information(this,"提示","请进行确认密码！");
                        }
                    }
                    else
                    {
                        QMessageBox::information(this,"提示","新密码与旧密码不能相同！");
                    }
                }
            }
            else
            {
                QMessageBox::information(this,"提示","新密码不能为空！");
            }
        }
        else
        {
            QMessageBox::information(this,"提示","原密码错误！请核对后再输入！");
        }
    }
    else
    {
        QMessageBox::information(this,"提示","原密码不能为空！");
    }
}
//修改密码
void uiDesign::on_PasswordChangeButton_released()
{
    QString ID=ui->IDinPaswordChange->text();

    QSqlQuery query1;
    QSqlQuery query2;
    QSqlQuery query3;
    QString str1=QString("SELECT Passkey FROM student WHERE StuID='%1'").arg(ID);
    QString str2=QString("SELECT TPasskey FROM teacher WHERE TeacherID='%1'").arg(ID);
    QString str3=QString("SELECT MPasskey FROM manager WHERE Account='%1'").arg(ID);
    query1.exec(str1);
    query2.exec(str2);
    query3.exec(str3);
    if(query1.next())
    {
        fixPasskey(query1,"Passkey","student","StuID");
    }
    else if(query2.next())
    {
        fixPasskey(query2,"TPasskey","teacher","TeacherID");
    }
    else if(query3.next())
    {
        fixPasskey(query3,"MPasskey","manager","Account");
    }
}

//转到学生页面
void uiDesign::on_courseInfo_released()//查看所有课程信息
{
    ui->mainPage->setCurrentIndex(5);
    // 清空原来的表格数据
    for (int i = StudentPage->CourseTableWidget->rowCount() - 1; i > 0; --i)
    {
        StudentPage->CourseTableWidget->removeRow(i);
    }
    //初始化各种combobox
    StudentPage->institute_InfoComboBox->setCurrentIndex(0);
    StudentPage->coursename_InfolineEdit->clear();
    StudentPage->teacherlineEdit->clear();
    StudentPage->coursetype_InfoComboBox->setCurrentIndex(0);
    StudentPage->coursecode_InfolineEdit->clear();
    StudentPage->week_InfoComboBox->setCurrentIndex(0);
    StudentPage->period_InfoComboBox->setCurrentIndex(0);
}
void uiDesign::on_courseChoose_released()//选课页面
{
    ui->mainPage->setCurrentIndex(6);
    // 清空原来的表格数据
    for (int i = StudentPage->Course_chooseTableWidget->rowCount() - 1; i > 0; --i)
    {
        StudentPage->Course_chooseTableWidget->removeRow(i);
    }
    //初始化各种combobox
    StudentPage->institute_chooseComboBox->setCurrentIndex(0);
    StudentPage->coursename_chooselineEdit->clear();
    StudentPage->teacher_chooselineEdit->clear();
    StudentPage->coursetype_chooseComboBox->setCurrentIndex(0);
    StudentPage->coursecode_chooselineEdit->clear();
    StudentPage->week_chooseComboBox->setCurrentIndex(0);
    StudentPage->period_chooseComboBox->setCurrentIndex(0);
}
void uiDesign::on_schedule_released()//课表页面
{
    ui->mainPage->setCurrentIndex(7);
    GenerateStudentSchedule(StudentPage->StudentScheduleTable);
}
void uiDesign::on_grades_released()//成绩页面
{
    ui->mainPage->setCurrentIndex(8);
    // 清空原来的表格数据
    for (int i = StudentPage->scoreTable->rowCount() - 1; i > 0; --i)
    {
        StudentPage->scoreTable->removeRow(i);
    }
    StudentPage->compulsoryCourse_checkBox->setChecked(true);//默认勾选学生查询成绩页面的所有课程类型
    StudentPage->electiveCourse_checkBox->setChecked(true);
    StudentPage->specializedCourse_checkBox->setChecked(true);
    StudentPage->previewCourse_checkBox->setChecked(true);
    StudentPage->grade_comboBox->setCurrentIndex(0);
    //清除平均绩点
    StudentPage->averageScorelabel->setText("");
    //设置学年combobox
    QString studentGrade=getStudentInfo('G');
    QStringList gradeList = { "一年级", "二年级", "三年级", "四年级", "五年级","六年级", "七年级"};
    //清除原来的combobox
    for(int i=0; i<7;i++)
    {
        if(StudentPage->grade_comboBox->findText(gradeList.at(i))!=-1)
        {
            StudentPage->grade_comboBox->removeItem(StudentPage->grade_comboBox->findText(gradeList.at(i)));
        }
    }
    //设置现在的combobox
    for(int i=0; i<7;i++)
    {
        if(studentGrade==gradeList.at(i))
        {
            for(int j=0; j<i; j++)
            {
                if(StudentPage->grade_comboBox->findText(gradeList.at(j))==-1)
                {
                    StudentPage->grade_comboBox->addItem(gradeList.at(j));
                }
            }
            break;
        }
    }
}
void uiDesign::on_testInfo_released()//考试信息查看页面
{
    ui->mainPage->setCurrentIndex(9);
    // 清空原来的表格数据
    for (int i = StudentPage->Course_TesttableWidget->rowCount() - 1; i > 0; --i)
    {
        StudentPage->Course_TesttableWidget->removeRow(i);
    }
}
void uiDesign::on_evaluation_released()//评教
{
    ui->mainPage->setCurrentIndex(10);
    StudentEvaluationOnChosenCourses();
}

//教师页面
//
//转到教师页面
//转到教师课程信息
void uiDesign::on_TeacherCourseInfo_released()
{
    uiDesign::deleteTabelItem(TeacherPage->CourseInfo_table);
    //获取数据库信息
    QSqlQuery teacher;
    QSqlQuery harryCourse;
    QString name;
    QString courseID;
    QString courseName;
    QString institution;
    QString type;
    QString credit;
    QString creditHour;
    QString num;
    QString examForm;
    QString studyDay;
    QString studyTime;
    QString studyPlace;
    QString studySemester;
    teacher.exec(QString("SELECT TeacherName FROM teacher WHERE TeacherID = %1").arg(id));
    teacher.next();
    name=teacher.value(0).toString();
    harryCourse.exec(QString("SELECT * FROM harrycourse WHERE Teacher= '%1'").arg(name));
    int rowCount=1;
    while(harryCourse.next()){
        if(!harryCourse.value(0).toBool()) continue;//当value(0)为false时，代表该课程不予显示
        studyDay=harryCourse.value(2).toString();
        studyTime=harryCourse.value(3).toString();
        courseName=harryCourse.value(4).toString();
        institution=harryCourse.value(5).toString();
        studyPlace=harryCourse.value(6).toString();
        studySemester=harryCourse.value(7).toString();
        type=harryCourse.value(8).toString();
        courseID=harryCourse.value(9).toString();
        credit=harryCourse.value(10).toString();
        examForm=harryCourse.value(11).toString();
        creditHour=harryCourse.value(12).toString();
        num=QString::number(getCourseChosenAmount(courseID));
        //课程信息
        TeacherPage->CourseInfo_table->insertRow(rowCount);
        TeacherPage->CourseInfo_table->setItem(rowCount, 0, new QTableWidgetItem(courseID));
        TeacherPage->CourseInfo_table->setItem(rowCount, 1, new QTableWidgetItem(courseName));
        TeacherPage->CourseInfo_table->setItem(rowCount, 2, new QTableWidgetItem(institution));
        TeacherPage->CourseInfo_table->setItem(rowCount, 3, new QTableWidgetItem(type));
        TeacherPage->CourseInfo_table->setItem(rowCount, 4, new QTableWidgetItem(credit));
        TeacherPage->CourseInfo_table->setItem(rowCount, 5, new QTableWidgetItem(creditHour));
        TeacherPage->CourseInfo_table->setItem(rowCount, 6, new QTableWidgetItem(num));
        TeacherPage->CourseInfo_table->setItem(rowCount, 7, new QTableWidgetItem(examForm));
        TeacherPage->CourseInfo_table->setItem(rowCount, 8, new QTableWidgetItem(studyDay));
        TeacherPage->CourseInfo_table->setItem(rowCount, 9, new QTableWidgetItem(studyTime));
        TeacherPage->CourseInfo_table->setItem(rowCount, 10, new QTableWidgetItem(studyPlace));
        TeacherPage->CourseInfo_table->setItem(rowCount, 11, new QTableWidgetItem(studySemester));
        rowCount++;
    }
    TeacherPage->CourseInfo_table->resizeColumnsToContents();
    TeacherSchedule(TeacherPage->Schedule_table);
    ui->mainPage->setCurrentIndex(11);
}
//转到查看学生信息
void uiDesign::on_TeacherCourseStudent_released()
{
    TeacherPage->StuInfoCourseBox->setCurrentIndex(0);
    TeacherPage->StuInfoGradeBox->setCurrentIndex(0);
    TeacherPage->StuInfoINumLine->clear();
    TeacherPage->StuInfoNameLine->clear();
    TeacherPage->StuInfoInstituteBox->setCurrentIndex(0);
    TeacherPage->StuInfoTime_Box->setCurrentIndex(1);
    addTeacherCourseName(TeacherPage->StuInfoCourseBox);
    //清空原来的数据
    for (int i = TeacherPage->StuInfoTable->rowCount() - 1; i > 0; --i)
    {
        TeacherPage->StuInfoTable->removeRow(i);
    }
    TeacherPage->StuInfoCourseBox->setCurrentIndex(0);
    TeacherPage->StuInfoGradeBox->setCurrentIndex(0);
    TeacherPage->StuInfoINumLine->clear();
    TeacherPage->StuInfoNameLine->clear();
    TeacherPage->StuInfoInstituteBox->setCurrentIndex(0);
    ui->mainPage->setCurrentIndex(12);
}
//转到登记学生成绩
void uiDesign::on_TeacherGrades_released()
{
    TeacherPage->scoresYear_Box->setCurrentIndex(1);
    TeacherPage->scoresCourseName_Box->setCurrentIndex(0);
    TeacherPage->scoresName_line->clear();
    TeacherPage->scoresNum_line->clear();
    TeacherPage->scores_checkBox->setChecked(false);
    deleteTabelItem(TeacherPage->scores_table);
    addTeacherCourseName(TeacherPage->scoresCourseName_Box);
    TeacherPage->scoresName_line->clear();
    TeacherPage->scoresNum_line->clear();
    ui->mainPage->setCurrentIndex(13);
}
//转到评教成绩查看
void uiDesign::on_TeacherEvaluation_released()
{
    TeacherPage->evaluationYear_Box->setCurrentIndex(1);
    TeacherPage->evaluationCourseName_Box->setCurrentIndex(0);
    addTeacherCourseName(TeacherPage->evaluationCourseName_Box);

    deleteTabelItem(TeacherPage->evaluation_table);
    ui->mainPage->setCurrentIndex(14);
}

//管理员界面
//
// 转到管理员界面
//管理员页面跳转
void uiDesign::on_courseInfo_2_released()   //课程查询
{
    ui->mainPage->setCurrentIndex(15);     //转到页面
    for(int i=AdminPage->ACourseTableWidget->rowCount()-1;i>0;--i)      //清空表格内容
    {
        AdminPage->ACourseTableWidget->removeRow(i);
    }

    AdminPage->Institute_comboBox->setCurrentIndex(0);         //以往的选择清空
    AdminPage->type_comboBox->setCurrentIndex(0);
    AdminPage->week_comboBox->setCurrentIndex(0);
    AdminPage->times_comboBox->setCurrentIndex(0);
    AdminPage->coursename_lineEdit->clear();
    AdminPage->teachername_lineEdit->clear();
    AdminPage->code_lineEdit->clear();
}
//跳转到老师信息管理页面
void uiDesign::on_teacherInfo_2_released()
{
    ui->mainPage->setCurrentIndex(16);
    for(int i=AdminPage->AteachertableWidget->rowCount()-1;i>0;--i)
    {
        AdminPage->AteachertableWidget->removeRow(i);
    }
    //清空上次查询内容
    AdminPage->comboBox_2->setCurrentIndex(0);
    AdminPage->lineEdit_3->clear();
    AdminPage->lineEdit_4->clear();
}
//跳转到学生信息管理页面
void uiDesign::on_studentInfo_2_released()
{
    ui->mainPage->setCurrentIndex(17);
    for(int i=AdminPage->AStudenttableWidget->rowCount()-1;i>0;--i)
    {
        AdminPage->AStudenttableWidget->removeRow(i);
    }
    //清空上次查询输入
    AdminPage->lineEdit_5->clear();
    AdminPage->lineEdit_6->clear();
    AdminPage->comboBox->setCurrentIndex(0);
}
//跳转到管理选课和退课以及开放评教的页面
void uiDesign::on_ManageWork_2_released()
{
    ui->mainPage->setCurrentIndex(20);
}
//跳转到学生成绩管理页面
void uiDesign::on_StuScoreMa_1_released()
{
    ui->mainPage->setCurrentIndex(21);
    for(int i=AdminPage->AStuScoreWidget_2->rowCount()-1;i>0;--i)
    {
        AdminPage->AStuScoreWidget_2->removeRow(i);
    }

    AdminPage->lineEdit->clear();
    AdminPage->lineEdit_2->clear();
    AdminPage->lineEdit_7->clear();
    AdminPage->lineEdit_8->clear();
    AdminPage->comboBox_3->setCurrentIndex(0);
}
//转到管理密码页面
void uiDesign::on_PassKeyInfo_2_released()
{
    ui->mainPage->setCurrentIndex(18);      //转到界面
    for(int i=AdminPage->AChangetableWidget->rowCount()-1;i>0;--i)             //清空表格信息
    {
        AdminPage->AChangetableWidget->removeRow(i);
    }
    //清空上一次的查询
    AdminPage->identity_comboBox->setCurrentIndex(0);
    AdminPage->college_comboBox->setCurrentIndex(0);
    AdminPage->name_lineEdit->clear();
    AdminPage->id_lineEdit->clear();
}

//转公告页面
void uiDesign::on_NoteInfo_2_released()
{
    ui->mainPage->setCurrentIndex(19);
    //AdminPage->ANoticeWidget_2->setRowHeight(1,30);
    AdminPage->ANoticeWidget_2->setColumnWidth(0,180);
    AdminPage->ANoticeWidget_2->setColumnWidth(1,150);
    AdminPage->ANoticeWidget_2->setColumnWidth(2,200);
    AdminPage->ANoticeWidget_2->setColumnWidth(3,100);
    deleteTabelItem(AdminPage->ANoticeWidget_2);
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
}


