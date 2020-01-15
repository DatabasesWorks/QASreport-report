#include "formdetaillist2.h"
#include "ui_formdetaillist2.h"

FormDetailList2::FormDetailList2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDetailList2)
{
    ui->setupUi(this);

    ui->splitter->setSizes(QList<int>() << 600 << 280);

    m_db = MainForm->CurrentDB();

    m_customerorders = new QSqlQueryModel(this);
    m_itemsparts = new QSqlQueryModel(this);

    m_customerorders->setQuery(QSqlQuery("Select orderno,o.custno,Company,Phone,Fax,saledate from orders o left join customer c  on o.custno=c.custno order by c.company",m_db));

    m_itemsparts->setQuery(QSqlQuery("Select orderno,i.partno,Description,ListPrice,qty  from items i left join parts p on i.partno=p.partno",m_db));

    ui->tablecustomerorders->setModel(m_customerorders);
    ui->tablecustomerorders->resizeColumnsToContents();

    ui->tableitemsparts->setModel(m_itemsparts);
    ui->tableitemsparts->resizeColumnsToContents();

    MainForm->report()->addModel("customerorders",m_customerorders);
    MainForm->report()->addModel("itemsparts",m_itemsparts);

    QObject::connect(ui->tablecustomerorders->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),SLOT(slotDetailSelect(QModelIndex,QModelIndex)));

     //initialization process of formation of the output child data of when passing records in the parent table
    QObject::connect(MainForm->report(),SIGNAL(signalModelOnNextRec(QObject*)),this,SLOT(slotModelOnNextRec(QObject*)));

    ui->tablecustomerorders->setCurrentIndex(m_customerorders->index(0,0));

 }

void FormDetailList2::slotDetailSelect(const QModelIndex &current, const QModelIndex &previous)
{
    QString value, filter;

    value = m_customerorders->data(m_customerorders->index(current.row(),m_customerorders->record().indexOf("orderno"))).toString();

    filter = " orderno=" + value;

    m_itemsparts->setQuery(QSqlQuery("Select orderno,i.partno,Description,ListPrice,qty  from items i left join parts p on i.partno=p.partno where " + filter,m_db));

}

void FormDetailList2::slotModelOnNextRec(QObject * object)
{
     if(object == m_customerorders){         
        QModelIndex index =  m_customerorders->index(m_customerorders->query().at(),0);

        if(index.isValid())
             ui->tablecustomerorders->setCurrentIndex(index);
     }
}

void FormDetailList2::slotBeginPrint()
{
    ui->tablecustomerorders->setCurrentIndex(m_customerorders->index(0,0));

    m_itemsparts->query().first();
}

void FormDetailList2::slotBeginPrintOrder()
{
    QModelIndex index;

    //obligatory positioning the internal cursor for correct conclusion
    index = ui->tablecustomerorders->currentIndex();

    if(index.isValid())
        m_customerorders->query().seek(index.row());

}

//init begin print process report
void FormDetailList2::initPrint()
{
    QObject::disconnect(MainForm->report(),SIGNAL(signalBeginReport()),0,0);
    QObject::connect(MainForm->report(),SIGNAL(signalBeginReport()),this,SLOT(slotBeginPrint()));
}

//init begin print process order
void FormDetailList2::initPrintOrders()
{    
    QObject::disconnect(MainForm->report(),SIGNAL(signalBeginReport()),0,0);
    QObject::connect(MainForm->report(),SIGNAL(signalBeginReport()),this,SLOT(slotBeginPrintOrder()));
}

FormDetailList2::~FormDetailList2()
{
    delete ui;
}
