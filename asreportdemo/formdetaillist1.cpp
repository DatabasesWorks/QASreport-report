#include "formdetaillist1.h"
#include "ui_formdetaillist1.h"

FormDetailList1::FormDetailList1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDetailList1)
{
    ui->setupUi(this);

    ui->splitter->setSizes(QList<int>() << 600 << 280);

    m_db = MainForm->CurrentDB();

    m_customerliter = new QSqlQueryModel(this);
    m_customer = new QSqlQueryModel(this);

    m_customerliter->setQuery(QSqlQuery("Select substr(company,1,1) as LITERA  from customer  group by  substr(company,1,1)",m_db));

    m_customer->setQuery(QSqlQuery("Select * from customer order by Company",m_db));

    ui->tablemaster->setModel(m_customerliter);
    ui->tablemaster->resizeColumnsToContents();

    ui->tabledetail->setModel(m_customer);
    ui->tabledetail->resizeColumnsToContents();

    MainForm->report()->addModel("customerliter",m_customerliter);
    MainForm->report()->addModel("customerdetail",m_customer);

    QObject::connect(ui->tablemaster->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),SLOT(slotDetailSelect(QModelIndex,QModelIndex)));

    //initialization process of formation of the output child data of when passing records in the parent table
    QObject::connect(MainForm->report(),SIGNAL(signalModelOnNextRec(QObject*)),this,SLOT(slotModelOnNextRec(QObject*)));
}

void FormDetailList1::slotDetailSelect(const QModelIndex &current, const QModelIndex &previous)
{
   QString litera, filter;

   litera = m_customerliter->data(m_customerliter->index(current.row(),0)).toString();

   filter = "substr(company,1,1)='" + litera + "'";

   m_customer->setQuery(QSqlQuery("Select * from customer where " + filter + " order by Company",m_db));
}

void FormDetailList1::slotBeginPrint()
{
    ui->tablemaster->setCurrentIndex(m_customerliter->index(0,0));

    m_customer->query().first();
}

void FormDetailList1::initPrint()
{
  QObject::disconnect(MainForm->report(),SIGNAL(signalBeginReport()),0,0);
  QObject::connect(MainForm->report(),SIGNAL(signalBeginReport()),this,SLOT(slotBeginPrint()));


}

void FormDetailList1::slotModelOnNextRec(QObject *object)
{
    if(object == m_customerliter){
       QModelIndex index =  m_customerliter->index(m_customerliter->query().at(),0);

       if(index.isValid())
            ui->tablemaster->setCurrentIndex(index);
    }
}

FormDetailList1::~FormDetailList1()
{
    delete ui;
}
