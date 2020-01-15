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

#ifndef QASREPORTVIEW_H
#define QASREPORTVIEW_H


#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewWidget>
#include <QtWidgets>
#include <QDialog>
#include "asreport_p.h"




namespace Ui {
class QASReportView;
}

class QASReportPrivate;

class QASReportView : public QDialog
{
    Q_OBJECT

enum CurPagePos{cpFirst,cpPrev,cpNext,cpLast};

public:
    //explicit QASReportView(QWidget *parent);
    explicit QASReportView(QWidget *parent,QObject *report);
    ~QASReportView();

    QPrintPreviewWidget * PreviewWidget();

private:
    Ui::QASReportView *ui;

    QPrintPreviewWidget * m_PreviewWidget;
    QASReportPrivate * m_Report;

    QComboBox * m_ZoomCombo;
    QLineEdit * m_CurPage;
    QLabel * m_CountPage;

    void updateZoomText();

private  slots:
    void slotPrintPages();
    void slotPrintToPDF();
    void slotFitToWidth();
    void slotfitInView();
    void slotZoomOut();
    void slotZoomIn();
    void slotZoomComboIndexChanged(const QString&);
    void slotEndReport();
    void slotSetCurPage();
    void slotSetViewPage();

public slots:

signals:




};

#endif // QASREPORTVIEW_H
