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


#ifndef DESDIALOGS_H
#define DESDIALOGS_H

#include <QtWidgets>
#include "desdefdata.h"
#include "desutils.h"
#include "dessettings.h"
#include "deseditors.h"

class QDesSelTreeField;

class DialogSelBand: public QDialog
{
    Q_OBJECT
public:
    DialogSelBand(QWidget* parent = 0);
    virtual ~DialogSelBand() {}

private:
QGroupBox *m_groupbox;

int m_countband;

public:

int SelectedBand();
void UpdateEnabled(QList<int>list);
};

//---------------------------------------------------------
class QImageWidget: public QWidget
{
    Q_OBJECT
public:
    QImageWidget(QWidget* parent = 0);
    virtual ~QImageWidget() {}

private:
QPixmap m_pixmap;

bool m_scaled;
bool m_drawrect;

protected:
virtual void paintEvent(QPaintEvent *pe);

public:
void SetImage(QPixmap pix){m_pixmap = pix; update();}
void SetScaled(bool val){ m_scaled = val;update();}
void SetDrawrect(bool val){ m_drawrect = val;}
QPixmap Image(){return m_pixmap;}

};



//---------------------------------------------------------
class QDesImageEdit: public QDialog
{
    Q_OBJECT
public:
    QDesImageEdit(QWidget* parent = 0);
    virtual ~QDesImageEdit();

private:
QImageWidget *m_imageWidget;
QToolButton *m_butScale;
QToolButton *m_butCheckSource;
QLineEdit * m_textsource;
QWidget * m_sourcewidget;

private slots:
void slotLoadImage();
void slotClearImage();
void slotScaleImage();
void slotClickButSource();
void slotSendValue(QString);
void slotButCheckSource();

public:
void SetImage(QPixmap pix);
QPixmap Image(){return m_imageWidget->Image();}
QString source(){return m_textsource->text();}
void setSource(QString value);

};

//-------------------------------------------------
class QDesFigureEdit: public QDialog
{
    Q_OBJECT
public:
    QDesFigureEdit(QWidget* parent = 0);
    virtual ~QDesFigureEdit() {}

private:
QComboBox *m_listtypes;

public:
void SetType(int val){m_listtypes->setCurrentIndex(val);}
int Type(){return m_listtypes->currentIndex();}

};

//------------------------------------------------------
class QDesPageProps : public QDialog
{
    Q_OBJECT
public:
    QDesPageProps(QWidget* parent = 0);
    virtual ~QDesPageProps() {}

private:
QComboBox * m_papesizes;
QLineEdit *m_widthmm;
QLineEdit *m_heighthmm;
QLabel *m_labelwidthmm;
QLabel *m_labelheigthhmm;
QRadioButton * m_landscape;
QRadioButton * m_portrait;
QLineEdit *m_margleft;
QLineEdit *m_margright;
QLineEdit *m_margtop;
QLineEdit *m_margbottom;

QImageWidget *m_image;

QPrinter::PaperSize m_papeSize;
QPrinter::Orientation m_orientation;

void updateImage();

private slots:
void slotPapesizesIndexChanged(int);
void slotClickedOrientation();

public:
QPrinter::Orientation Orientation(){return  m_orientation;}
void SetOrientation(QPrinter::Orientation orientation){m_orientation = orientation; if(m_orientation == QPrinter::Portrait)m_portrait->setChecked(true);else m_landscape->setChecked(true); updateImage();}
QPrinter::PaperSize PapeSize(){return m_papeSize;}
void SetPapeSize(QPrinter::PaperSize size){m_papeSize = size; m_papesizes->setCurrentIndex(size);}
int margLeft(){return m_margleft->text().toInt();}
int margRight(){return m_margright->text().toInt();}
int margTop(){return m_margtop->text().toInt();}
int margBottom(){return m_margbottom->text().toInt();}
void SetMargLeft(int val){m_margleft->setText(QString::number(val));}
void SetMargRight(int val){m_margright->setText(QString::number(val));}
void SetMargTop(int val){m_margtop->setText(QString::number(val));}
void SetMargBottom(int val){m_margbottom->setText(QString::number(val));}
int WidthMM(){return m_widthmm->text().toInt();}
int HeightMM(){return m_heighthmm->text().toInt();}
void SetWidthMM(int val){m_widthmm->setText(QString::number(val));}
void SetHeightMM(int val){m_heighthmm->setText(QString::number(val));}
};

//-------------------------------------------------------------------------------------------
class QDesSelDataSource: public QDialog
{
    Q_OBJECT
public:
    QDesSelDataSource(QWidget* parent = 0);
    virtual ~QDesSelDataSource() {}

private:
QComboBox * m_tables;

private slots:

public:
void SetTablesModel(QStandardItemModel *model){m_tables->setModel(model);}
QString Source(){return m_tables->currentText();}
void setSource(QString value){m_tables->setCurrentText(value);}

};

//------------------------------------------------------------------------------
class QDesSelDataField : public QDialog
{
    Q_OBJECT
public:
    QDesSelDataField(QWidget* parent = 0);
    virtual ~QDesSelDataField() {}


private:
QLineEdit *m_groupfield;

bool m_varenabled;

private slots:
void slotSelectField();
void slotSendValue(QString);

public:

void setVarEnabled(bool val){m_varenabled = val;}

void setGroupField(QString value){m_groupfield->setText(value);}
QString groupField(){return m_groupfield->text();}

};

//-------------------------------------------------------------------------------
class QDesSelTreeField: public QDialog
{
    Q_OBJECT
public:
    QDesSelTreeField(QWidget* parent = 0);
    virtual ~QDesSelTreeField() {}

private:
QTreeView *m_treeview;

private slots:
void slotDblClickTreeView(const QModelIndex&);

signals:
void signalSendValue(QString);

public:
void SetTablesModel(QStandardItemModel *model){m_treeview->setModel(model);}
};

//-----------------------------------------------------
class QDesSelTreeVariables: public QDialog
{
 Q_OBJECT
public:
QDesSelTreeVariables(QWidget* parent,bool varenabled = true);
virtual ~QDesSelTreeVariables() {}

private:
QTreeView *m_treeviewfields;
QTreeView *m_treeviewvariables;

bool m_varenabled;

private slots:
void slotDblClickTreeView(const QModelIndex&);

signals:
void signalSendValue(QString);

public:
void SetModelFields(QStandardItemModel *model){m_treeviewfields->setModel(model);}
void SetModelVariables(QStandardItemModel *model){m_treeviewvariables->setModel(model);}
void updateModels();

};

//-------------------------------------------------------------------------
class QDesSelFormat: public QDialog
{
    Q_OBJECT
public:
    QDesSelFormat(QWidget* parent = 0);
    virtual ~QDesSelFormat() {}

private:
QComboBox *m_typevalue;
QComboBox *m_selectformat;

QLineEdit * m_precision;
QLineEdit * m_otherformat;
QLabel * m_label;
QStringListModel m_listmodel;

private slots:
void slotTypevalueIndexChanged(int index);
void slotSelectformatCurrentIndexChanged(QString);

public:
void SetFormat(QString format);
QString Format();

};
//-------------------------------------------------------------------------
class QDesTextEditor: public QDialog
{
    Q_OBJECT
public:
    QDesTextEditor(QWidget* parent = 0);

    virtual ~QDesTextEditor() {}

private:

QTextEdit * m_texteditor;


public:

void SetStrings(QStringList);
QStringList Strings();
};

//-------------------------------------------
class QDesBarCodeEditor: public QDialog
{
    Q_OBJECT

public:
    QDesBarCodeEditor(QWidget* parent = 0);
    virtual ~QDesBarCodeEditor() {}

private:
QLineEdit * m_editor;
QToolButton * m_butfields;
QToolButton * m_butvariables;

private slots:
void slotButClicked();
void slotSelVariable(QString);

public:
QString Value(){return m_editor->text();}
void setValue(QString value){m_editor->setText(value);}
};






#endif // DESDIALOGS_H
