#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow * MainForm;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainForm = this;

    m_Report = new QASReport(this);

    m_curSqlModel = new QSqlQueryModel(this);

    m_FormList = Q_NULLPTR;
    m_FormDetailList1 = Q_NULLPTR;
    m_FormDetailList2 =  Q_NULLPTR;
    m_FormGroups =  Q_NULLPTR;
    m_FormAnyModel = Q_NULLPTR;
    m_FormImages = Q_NULLPTR;

   if(createConnection()){
        QSqlDatabase db = CurrentDB();
        QStringList list = db.tables();

        m_tables.setStringList(list);

        ui->listTables->setModel(&m_tables);

        if(!list.isEmpty()){
            ui->listTables->setCurrentIndex(m_tables.index(0));

            updateSqlData(list.at(0));
        }

        m_FormList = new FormList(this);

        m_FormList->initPrint();

        showStackWidget(m_FormList);
   }

   ui->tableView->setModel(m_curSqlModel);

   ui->tabWidget->setCurrentIndex(0);

  connect(ui->radioList,SIGNAL(clicked()),SLOT(slotTypeReportClicked()));
  connect(ui->radiodetail1,SIGNAL(clicked()),SLOT(slotTypeReportClicked()));
  connect(ui->radiodetail2,SIGNAL(clicked()),SLOT(slotTypeReportClicked()));
  connect(ui->radioorders,SIGNAL(clicked()),SLOT(slotTypeReportClicked()));
  connect(ui->radiomultpages,SIGNAL(clicked()),SLOT(slotTypeReportClicked()));
  connect(ui->radiogroups,SIGNAL(clicked()),SLOT(slotTypeReportClicked()));  
  connect(ui->radioAnyModel,SIGNAL(clicked()),SLOT(slotTypeReportClicked()));
  connect(ui->radioImages,SIGNAL(clicked()),SLOT(slotTypeReportClicked()));

  connect(ui->listTables->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),SLOT(slotSelectItem(QModelIndex,QModelIndex)));

}

void MainWindow::slotSelectItem(const QModelIndex &current, const QModelIndex &previous)
{
    QString table = current.data().toString();

    updateSqlData(table);
}

void MainWindow::slotTypeReportClicked()
{
    QObject * sender = QObject::sender();
    QWidget *showwid = Q_NULLPTR;

    if(sender == ui->radioList || sender == ui->radiomultpages){
        if(m_FormList == Q_NULLPTR)
            m_FormList = new FormList(this);

        m_FormList->initPrint();

        showwid = m_FormList;
    }

    if(sender == ui->radiodetail1){
        if(m_FormDetailList1 == Q_NULLPTR)
            m_FormDetailList1 = new FormDetailList1(this);

        m_FormDetailList1->initPrint();

        showwid = m_FormDetailList1;
    }

    if(sender == ui->radiodetail2 || sender == ui->radioorders){
        if(m_FormDetailList2 == Q_NULLPTR)
            m_FormDetailList2 = new FormDetailList2(this);

        showwid = m_FormDetailList2;
    }

    if(sender == ui->radiogroups){
        if(m_FormGroups == Q_NULLPTR)
            m_FormGroups = new FormGroups(this);

        m_FormGroups->initPrint();

        showwid = m_FormGroups;
    }

     if(sender == ui->radioAnyModel){
         if(m_FormAnyModel == Q_NULLPTR)
              m_FormAnyModel = new FormAnyModel(this);

              m_FormAnyModel->initPrint();

              showwid = m_FormAnyModel;
     }

      if(sender == ui->radioImages){
          if(m_FormImages == Q_NULLPTR)
            m_FormImages = new FormImages(this);

          m_FormImages->initPrint();

          showwid = m_FormImages;
      }


    if(showwid == Q_NULLPTR) return;

    showStackWidget(showwid);

}

void MainWindow::slotGetValue(QString paramName, QVariant &paramValue)
{

}

void MainWindow::showStackWidget(QWidget *widget)
{
    int index;

      if((index = ui->stackedWidget->indexOf(widget)) == -1){
                   ui->stackedWidget->addWidget(widget);

                   index = ui->stackedWidget->count() - 1;
      }

      ui->stackedWidget->setCurrentIndex(index);
}

void MainWindow::updateSqlData(QString table)
{
    if(table.isEmpty()) return;

    QSqlDatabase db = CurrentDB();

    m_curSqlModel->setQuery(QSqlQuery("Select * from " + table,db));

    ui->tableView->resizeColumnsToContents();
}


QSqlDatabase MainWindow::CurrentDB()
{
    return QSqlDatabase::database(m_connectionName, false);
}

bool MainWindow::createConnection()
{
    if(!m_connectionName.isEmpty())
            QSqlDatabase::removeDatabase(m_connectionName);

        m_connectionName = "CURDB";

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",m_connectionName);
        db.setDatabaseName("dbdemos.db");

        if (!db.open()) {
            db = QSqlDatabase();
            QSqlDatabase::removeDatabase(m_connectionName);
            m_connectionName = "";

            qDebug() << db.lastError().text();

            return false;
        }

   return true;
}

MainWindow::~MainWindow()
{
    CurrentDB().close();

    if(!m_connectionName.isEmpty())
        QSqlDatabase::removeDatabase(m_connectionName);


    delete ui;
}

void MainWindow::on_butPreview_clicked()
{
    QString filename, dir = QDir::currentPath();
    bool load = false;

    if(ui->radioList->isChecked()){
        filename =  dir + "/forms/list.frm";
    }

    if(ui->radiodetail1->isChecked()){
        filename = dir + "/forms/detaillist1.frm";
    }

    if(ui->radiodetail2->isChecked()){
        filename = dir + "/forms/detaillist2.frm";

        m_FormDetailList2->initPrint();
    }

    if(ui->radioorders->isChecked()){
        filename = dir + "/forms/order.frm";

        m_FormDetailList2->initPrintOrders();
    }

    if(ui->radiomultpages->isChecked()){
        filename = dir + "/forms/multipages.frm";
    }

    if(ui->radiogroups->isChecked()){
        filename = dir + "/forms/groups.frm";
    }

    if(ui->radioAnyModel->isChecked()){
        filename = dir + "/forms/anymodel.frm";
    }

    if(ui->radioImages->isChecked()){
        filename = dir + "/forms/imagefields.frm";
    }


    if(filename.isEmpty()) return;

    if(!QFile::exists(filename)) return;

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
        return;

    load = m_Report->load(&file);

    if(!load){
        QMessageBox::warning(0,"warning","File report didn't find!");

        return;
    }

        m_Report->reportView();
}

void MainWindow::on_butDesigh_clicked()
{
    QString filename, dir = QDir::currentPath();

    if(ui->radioList->isChecked()){
        filename = dir + "/forms/list.frm";
    }

    if(ui->radiodetail1->isChecked()){
        filename = dir + "/forms/detaillist1.frm";
    }

    if(ui->radiodetail2->isChecked()){
        filename = dir + "/forms/detaillist2.frm";

       m_FormDetailList2->initPrint();
    }

    if(ui->radioorders->isChecked()){
        filename = dir + "/forms/order.frm";

       m_FormDetailList2->initPrintOrders();
    }

    if(ui->radiomultpages->isChecked()){
        filename = dir + "/forms/multipages.frm";
    }

    if(ui->radiogroups->isChecked()){
        filename = dir + "/forms/groups.frm";
    }

    if(ui->radioAnyModel->isChecked()){
        filename = dir + "/forms/anymodel.frm";
    }

    if(ui->radioImages->isChecked()){
        filename = dir + "/forms/imagefields.frm";
    }

    if(filename.isEmpty() || !QFile::exists(filename))
        m_Report->runDesigner();
    else
        m_Report->edit(filename);
}


