#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../asreportlib/asreport.h"
#include "formlist.h"
#include "formdetaillist1.h"
#include "formdetaillist2.h"
#include "formgroups.h"
#include "formanymodel.h"
#include "formimages.h"

namespace Ui {
class MainWindow;
}

class FormList;
class FormDetailList1;
class FormDetailList2;
class FormGroups;
class FormAnyModel;
class FormImages;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

private slots:

    void on_butPreview_clicked();

    void on_butDesigh_clicked();

    void slotGetValue(QString paramName, QVariant &paramValue);

    void slotTypeReportClicked();

   void slotSelectItem(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::MainWindow *ui;

    QASReport * m_Report;

    QSqlQueryModel* m_curSqlModel;
    QStringListModel m_tables;

    QString m_connectionName;

    FormList * m_FormList;
    FormDetailList1 * m_FormDetailList1;
    FormDetailList2 * m_FormDetailList2;
    FormGroups * m_FormGroups;
    FormAnyModel * m_FormAnyModel;
    FormImages * m_FormImages;

    bool createConnection();
    void updateSqlData(QString table);
    void showStackWidget(QWidget *widget);


public:

    QSqlDatabase CurrentDB();

    QASReport * report(){return m_Report;}

};


extern MainWindow * MainForm;

#endif // MAINWINDOW_H
