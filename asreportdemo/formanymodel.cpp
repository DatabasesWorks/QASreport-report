#include "formanymodel.h"
#include "ui_formanymodel.h"

FormAnyModel::FormAnyModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormAnyModel)
{
    ui->setupUi(this);

    ui->listStrings->setModel(&m_stringsmodel);

    ui->tableItem->setModel(&m_itemsmodel);

    updateData();

    MainForm->report()->addModel("stringsmodel",&m_stringsmodel);
    MainForm->report()->addModel("itemsmodel",&m_itemsmodel);

}


void FormAnyModel::updateData()
{
    QString value;
    QStringList strings;

    for (int i = 0; i < 20; ++i) {
        value = "Value " + QString::number(i);

        strings << value;
    }

    m_stringsmodel.setStringList(strings);

   m_itemsmodel.setColumnCount(4);
   m_itemsmodel.setRowCount(20);

   for (int r = 0; r < 20; ++r)
       for (int c = 0; c < 4; ++c){
             value = "Value " +  QString::number(r) + " " + QString::number(c);

             m_itemsmodel.setData(m_itemsmodel.index(r,c),value);
   }
}

void FormAnyModel::initPrint()
{

}

FormAnyModel::~FormAnyModel()
{
    delete ui;
}

void FormAnyModel::on_butAdd_clicked()
{
    QStringList list = m_stringsmodel.stringList();
    QString str = "Value " + QString::number(list.count() - 1);

    list << str;

    m_stringsmodel.setStringList(list);

}

void FormAnyModel::on_butDel_clicked()
{
    QStringList list = m_stringsmodel.stringList();
    int selrow;

    if(!list.count()) return;

    if(!ui->listStrings->currentIndex().isValid()) return;

    selrow = ui->listStrings->currentIndex().row();

    list.removeAt(selrow);

    m_stringsmodel.setStringList(list);
}
