#include "formlist.h"
#include "ui_formlist.h"

FormList::FormList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormList)
{
    ui->setupUi(this);

    m_customer = new QSqlQueryModel(this);

    QSqlDatabase db = MainForm->CurrentDB();

    m_customer->setQuery(QSqlQuery("Select * from customer order by Company",db));

    ui->tableView->setModel(m_customer);

    ui->tableView->resizeColumnsToContents();

    MainForm->report()->addModel("customer",m_customer);
}

void FormList::slotBeginPrint()
{
    m_customer->query().first();
}

void FormList::slotGetValue(QString paramName, QVariant &paramValue)
{
    if(!paramName.compare("Company name"))
          paramValue = m_customer->query().value("Company").toString();

    if(!paramName.compare("Company address"))
          paramValue = m_customer->query().value("Addr1").toString();
}

void FormList::initPrint()
{
    QObject::disconnect(MainForm->report(),SIGNAL(signalBeginReport()),0,0);               //disconnected from m_Report signals
    QObject::disconnect(MainForm->report(),SIGNAL(signalGetValue(QString,QVariant&)),0,0);

    QObject::connect(MainForm->report(),SIGNAL(signalBeginReport()),this,SLOT(slotBeginPrint())); //connected  m_Report to  slotBeginPrint
    QObject::connect(MainForm->report(),SIGNAL(signalGetValue(QString,QVariant&)),this,SLOT(slotGetValue(QString,QVariant&))); //init read m_Report custom variables

}

QTableView * FormList::tableView()
{
    return ui->tableView;
}

FormList::~FormList()
{
    delete ui;
}
