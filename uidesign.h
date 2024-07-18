#ifndef UIDESIGN_H
#define UIDESIGN_H

#include <QMainWindow>
#include <QScreen>
#include <QPainter>
#include <QStyleOptionButton>
#include <QFileInfo>
#include <QMessageBox>
#include <qfontdatabase.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <qicon.h>
#include <qtextedit.h>
#include <QTextBrowser>
#include "ui_Student.h"
#include "ui_Teacher.h"
#include "ui_admin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class uiDesign; }
QT_END_NAMESPACE

class uiDesign : public QMainWindow
{
    Q_OBJECT

public:
    uiDesign(QWidget *parent = nullptr);
    ~uiDesign();

private:
    Ui::uiDesign *ui;
    Ui::Student* StudentPage;
    Ui::Teacher* TeacherPage;
    Ui::Admin* AdminPage;
    //函数所需的变量
    int role=0;
    int institutionRole=0;
    QString id;

private slots:
    void institutionrole();
    void on_courseInfo_released();

    void on_courseChoose_released();

    void on_schedule_released();

    void on_grades_released();

    void on_testInfo_released();

    void on_evaluation_released();

    void on_home_released();

    void on_exit_released();

    void on_logButton_released();

    void on_PasswordChange_released();

    void on_PasswordChangeButton_released();
    //学生页面
    void on_Query_InfoPushBtn_clicked();

    void on_Query_chosenPushBtn_clicked();

    void on_Query_tochoosePushBtn_clicked();

    void on_Query_TestPushBtn_clicked();

    void on_StuSearchScore_PBtn_clicked();

    void on_queryEvaluation_Btn_clicked();

    //教师页面
    void on_TeacherCourseInfo_released();

    void on_TeacherCourseStudent_released();

    void on_TeacherGrades_released();

    void on_TStuInfoButton_released();
    void on_scores_button_released();

    void TeacherSchedule(QTableWidget* table);

    void on_TeacherEvaluation_released();

    void on_evaluation_button_released();
    void on_evaluationCount_button_released();

    void on_evaluationAver_button_released();

    int getStudyDay(QString day);
    int getStudyTime(QString time);
    void onScoreCheck();

    //管理员界面

    void on_courseInfo_2_released();

    void on_Aquery_pushButton_clicked();

    void on_teacherInfo_2_released();

    void on_studentInfo_2_released();

    void on_PassKeyInfo_2_released();

    void on_ManageWork_2_released();

    void on_query_pushButton1_clicked();

    void on_pushButton_6_clicked();

    void on_query_pushButton2_clicked();

    void on_Start_pushButton_clicked();

    void on_close_pushButton_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_add_pushButton_clicked();

    void on_addTer_pushButton_clicked();

    void on_pushButton_clicked();

    void on_ScoreB_pushButton_clicked();

    void on_ScoreC_pushButton_clicked();

    void on_StuScoreMa_1_released();

    void on_pushButton_2_clicked();

    void on_NoteInfo_2_released();

    void on_pushButton_3_clicked();

    //功能函数
    void deleteTabelItem(QTableWidget* table);
    void addTeacherCourseName(QComboBox* box);
    void addLineToCourseTable(QTableWidget* table,QSqlQuery& query, int& rowCount);
    QString FilterCoursesByInput(QString table,QString institute,QString coursename,QString teacher,QString coursetype,QString coursecode,QString week,QString period);
    int getCourseChosenAmount(QString courseid);

    void showNoice(QTableWidget* table,QString userID);//修改
    void fixPasskey(QSqlQuery &Query,QString passkey,QString table,QString IDname);

    void on_toolButton_released();

    void on_seePassword_released();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QString getStudentInfo(char command);
    QString getCourseInfo(char command,QString courseid);
    void StudentEvaluationOnChosenCourses();
    void GenerateStudentSchedule(QTableWidget* table);
    void InitializeScheduleTable(QTableWidget* table);
    void showSchedule();
    //void homeEntered(QEnterEvent *event);
    void deleteCourse(QTableWidget *table,int rowCount);
    void fixCourse(QTableWidget *table,int row);
    void StuMass(QTableWidget *table,QSqlQuery &queryStu);
    void TerMass(QTableWidget *table,QSqlQuery &queryTer);
    void fixStuInfo(QTableWidget *table,int row);
    void deleteStu(QTableWidget *table,int rowCount);
    void fixTerInfo(QTableWidget *table,int row);
    void deleteTerInfo(QTableWidget *table,int row);
    void fixStuScore(QTableWidget *table,int row);
    void deleteNotice(QTableWidget *table,int row);
    void showNotice(QTableWidget *table,int row);
};
#endif // UIDESIGN_H
