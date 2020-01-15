#include "formgroups.h"
#include "ui_formgroups.h"

FormGroups::FormGroups(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormGroups)
{
    ui->setupUi(this);

    m_db = MainForm->CurrentDB();

    m_groups = new QSqlQueryModel();

    m_groups->setQuery(QSqlQuery("Select o.orderno,o.custno,Company,Phone,Fax,saledate,i.partno,Description,ListPrice,qty from orders o left join customer c  on o.custno=c.custno left join items i on o.orderno=i.orderno  left join parts p on i.partno=p.partno   order by c.company",m_db));

    ui->tablegroups->setModel(m_groups);
    ui->tablegroups->resizeColumnsToContents();

    MainForm->report()->addModel("groups",m_groups);
}


void FormGroups::slotBeginPrint()
{
    ui->tablegroups->setCurrentIndex(m_groups->index(0,0));

    m_groups->query().first();
}

void FormGroups::initPrint()
{
    QObject::disconnect(MainForm->report(),SIGNAL(signalBeginReport()),0,0);
    QObject::connect(MainForm->report(),SIGNAL(signalBeginReport()),this,SLOT(slotBeginPrint()));
}

FormGroups::~FormGroups()
{
    delete ui;
}
