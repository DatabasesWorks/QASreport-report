/********************************************
*         QASReport ver.1.01                *
*           Report class                    *
*                                           *
*  Copyright (c) 2015-2016  Sergey Zgukov   *
*   E-mail:  serg_@ukr.net                  *
********************************************/
/*
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "asreportview.h"
#include "ui_asreportview.h"


QASReportView::QASReportView(QWidget *parent,QObject *report) :
    QDialog(parent),
    ui(new Ui::QASReportView)
{
    ui->setupUi(this);

    setAttribute( Qt::WA_DeleteOnClose, true);
    setWindowState(Qt::WindowMaximized);
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);

   // m_Report = (QASReport*)parent;
    m_Report = (QASReportPrivate*)report;

    connect(m_Report,SIGNAL(signalEndReport()),this,SLOT(slotEndReport()));

    QToolBar * ptb = new QToolBar(this);

    ptb->addAction(QPixmap(":/new/preview/images/fit-width-24.png"),"Fit to width", this, SLOT(slotFitToWidth()));
    ptb->addAction(QPixmap(":/new/preview/images/fit-page-24.png"),"Fit in view", this, SLOT(slotfitInView()));
    ptb->addSeparator();

    m_ZoomCombo = new QComboBox(this);
    m_ZoomCombo->setMaximumSize(70,20);
    m_ZoomCombo->setMinimumSize(70,20);
    m_ZoomCombo->setEditable(true);
    m_ZoomCombo->setValidator(new QDoubleValidator(this));
    ptb->addWidget(m_ZoomCombo);


    ptb->addAction(QPixmap(":/new/preview/images/zoom-out-24.png"),tr("Zoom out"), this, SLOT(slotZoomOut()));
    ptb->addAction(QPixmap(":/new/preview/images/zoom-in-24.png"),tr("Zoom in"), this, SLOT(slotZoomIn()));
    ptb->addSeparator();
    ptb->addAction(QPixmap(":/new/preview/images/go-first-24.png"),tr("First page"), this, SLOT(slotSetCurPage()))->setData(cpFirst);
    ptb->addAction(QPixmap(":/new/preview/images/go-previous-24.png"),tr("Previous page"), this, SLOT(slotSetCurPage()))->setData(cpPrev);

    m_CurPage = new QLineEdit(this);
    m_CurPage->setMaximumSize(40,20);
    m_CurPage->setMinimumSize(40,20);
    ptb->addWidget(m_CurPage);
    m_CountPage = new QLabel(this);
    m_CountPage->setMaximumSize(20,20);
    m_CountPage->setMinimumSize(20,20);
    ptb->addWidget(m_CountPage);

    ptb->addAction(QPixmap(":/new/preview/images/go-next-24.png"),tr("Next page"), this, SLOT(slotSetCurPage()))->setData(cpNext);
    ptb->addAction(QPixmap(":/new/preview/images/go-last-24.png"),tr("Last page"), this, SLOT(slotSetCurPage()))->setData(cpLast);

    ptb->addSeparator();
    ptb->addAction(QPixmap(":/new/preview/images/view-page-one-24.png"),tr("View page one"), this, SLOT(slotSetViewPage()))->setData(QPrintPreviewWidget::SinglePageView);
    ptb->addAction(QPixmap(":/new/preview/images/view-page-sided-24.png"),tr("View page sided"), this, SLOT(slotSetViewPage()))->setData(QPrintPreviewWidget::FacingPagesView);
    ptb->addAction(QPixmap(":/new/preview/images/view-page-multi-24.png"),tr("View page multi"), this, SLOT(slotSetViewPage()))->setData(QPrintPreviewWidget::AllPagesView);
    ptb->addSeparator();
    ptb->addAction(QPixmap(":/new/preview/images/print-24.png"),tr("Print"), this, SLOT(slotPrintPages()));
    ptb->addSeparator();
    ptb->addAction(QPixmap(":/new/preview/images/PDF-24.png"),tr("Export to PDF"), this, SLOT(slotPrintToPDF()));


    m_PreviewWidget = new QPrintPreviewWidget(this);
    ui->verticalLayout->addWidget(ptb);
    ui->verticalLayout->addWidget(m_PreviewWidget);    
    m_PreviewWidget->show();


double val = 12.50;
    for (int i = 0; i < 7; ++i,val *= 2)
        m_ZoomCombo->addItem(QString::number(val) + "%");

     connect(m_ZoomCombo,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(slotZoomComboIndexChanged(const QString&)));

   updateZoomText();

    m_CurPage->setText(QString::number(1));
}

void QASReportView::slotSetCurPage()
{
QObject* obj = sender();

if(!obj) return;

QVariant v = ((QAction*)obj)->data();
CurPagePos pos = (CurPagePos)v.toInt();
int curpage = m_PreviewWidget->currentPage();

switch (pos) {
case cpFirst:
    curpage = 1;
    break;

case cpPrev:
    curpage--;

     if(curpage < 1) curpage = 1;
    break;

case cpNext:
    curpage++;

    if(curpage > m_PreviewWidget->pageCount())
        curpage =  m_PreviewWidget->pageCount();
    break;

case cpLast:
    curpage = m_PreviewWidget->pageCount();
    break;
}

m_PreviewWidget->setCurrentPage(curpage);
m_CurPage->setText(QString::number(m_PreviewWidget->currentPage()));

}


void QASReportView::slotSetViewPage()
{
QObject* obj = sender();

if(!obj) return;

QVariant v = ((QAction*)obj)->data();

m_PreviewWidget->setViewMode((QPrintPreviewWidget::ViewMode)v.toInt());
}

void QASReportView::slotEndReport()
{
  m_CountPage->setText(" " + QString::number(m_Report->pageCount()));
}

void QASReportView::slotZoomComboIndexChanged(const QString& value)
{
    QString str = value;
    int n;
    bool ok = false;

    n = str.indexOf("%");

    if(n != -1)
        str = str.mid(0,n);

    str.toFloat(&ok);

    if(!ok) return;

    qreal val = str.toFloat();

    m_PreviewWidget->setZoomFactor(val/100);

}


void QASReportView::updateZoomText()
{
 m_ZoomCombo->setEditText(QString::number(m_PreviewWidget->zoomFactor() * 100,'f',1) + "%");

}

void QASReportView::slotZoomOut()
{
    m_PreviewWidget->zoomOut();

updateZoomText();
}

void QASReportView::slotZoomIn()
{
m_PreviewWidget->zoomIn();

updateZoomText();
}


QPrintPreviewWidget * QASReportView::PreviewWidget()
{
 return  m_PreviewWidget;
}

void QASReportView::slotFitToWidth()
{
  m_PreviewWidget->fitToWidth();

  updateZoomText();


}

void QASReportView::slotfitInView()
{
  m_PreviewWidget->fitInView();

updateZoomText();

}

void QASReportView::slotPrintPages()
{
    m_Report->reportPrint();
}

void QASReportView::slotPrintToPDF()
{
   m_Report->exportTo(QASReportPrivate::PDF);
}


QASReportView::~QASReportView()
{
    delete ui;

}
