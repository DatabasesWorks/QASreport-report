#include "formimages.h"
#include "ui_formimages.h"

FormImages::FormImages(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormImages)
{
    ui->setupUi(this);

    m_biolife = new QSqlQueryModel(this);

    m_biolife->setQuery("Select * from biolife",MainForm->CurrentDB());

    ui->tableView->setModel(m_biolife);

    ui->tableView->resizeColumnsToContents();

    QObject::connect(ui->tableView->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),SLOT(slotSelectRec(QModelIndex,QModelIndex)));

    ui->tableView->setCurrentIndex(m_biolife->index(0,0));

    ui->frameImage->installEventFilter(this);



}

bool FormImages::eventFilter(QObject* object, QEvent* event)
{
    if(object == ui->frameImage){
        if(event->type() == QEvent::Paint){

            drawImage();

            return true;
        }
    }

    return false;
}

void FormImages::slotSelectRec(const QModelIndex &current, const QModelIndex &previous)
{
   QVariant value =  m_biolife->data(m_biolife->index(current.row(),m_biolife->record().indexOf("Notes")));

   ui->textEdit->setPlainText(value.toString());

   value =  m_biolife->data(m_biolife->index(current.row(),m_biolife->record().indexOf("Graphic")));

   m_image  = QImage::fromData(value.toByteArray());

   ui->frameImage->repaint();

}

void FormImages::drawImage()
{
    m_painter.begin(ui->frameImage);

    m_painter.drawImage(0,0,m_image);

    m_painter.end();
}

void FormImages::initPrint()
{
    m_biolife->query().first();


}


FormImages::~FormImages()
{
    delete ui;
}
