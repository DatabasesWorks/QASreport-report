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

#ifndef ASREPDESIGNER_H
#define ASREPDESIGNER_H


#include <QMainWindow>
#include <QWidget>
#include <QScrollArea>
#include <QMessageBox>
#include <QMouseEvent>
#include "designpage.h"
#include "descontrols.h"
#include "despropinspector.h"
#include "deseditors.h"
#include "asreport_p.h"
#include "dessettings.h"
#include "desvariables.h"

class QASReportPrivate;
class QDesControl;
class QDesignPage;
class DesPropInspector;

namespace Ui {
class QASRepDesigner;
}



class QASRepDesigner : public QMainWindow
{
    Q_OBJECT

public:
    explicit QASRepDesigner(QWidget *parent,QASReportPrivate * report);
    ~QASRepDesigner();


private slots:
    void on_actionNew_triggered();
    void on_actionClose_triggered();
    void on_actionSave_and_close_triggered();
    void on_actionCreateControl();
    void slotBandMenu(QAction*);
    void slotEndCreateControl();
    void on_actionSelect_All_triggered();
    void slotSelectControls(QList<QDesControl*>);
    void slotUnSelectControls();
    void slotUpdateBandMenu();
    void on_actionDelete_triggered();
    void on_actionObject_up_triggered();
    void on_actionObject_down_triggered();
    void slotActionAlignment();
    void slotSelectFromTree(QModelIndex index);
    void slotUpdatePropValues();
    void slotFontChanged(QString);
    void slotLineWidthChanged(int);
    void slotSelectFontColor(QColor);
    void slotSelectBackColor(QColor);
    void slotSelectBorderColor(QColor);
    void slotSelectLineStyle(int);
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void slotUpdateMenuUndo();
    void on_actionSave_triggered();
    void slotChanging();
    void on_actionOpen_triggered();
    void on_actionSave_as_triggered();
    void on_tabWidget_currentChanged(int index);
    void SaveReport();
    void on_actionPreview_triggered();
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionPast_triggered();
    void on_actionAdd_page_triggered();
    void on_actionDelete_page_triggered();
    void on_actionPage_parametres_triggered();
    void slotUpdateAddPropValues();
    void slotReadToolBarValues();
    void on_actionSettings_triggered();
    void SaveVariables();

    void on_actionList_of_variables_triggered();

    void on_actionStandart_triggered();

    void on_actionAbout_triggered();

private:
    Ui::QASRepDesigner *ui;

    QDesignPage * m_ActivePage;
    QDomDocument *m_xmlDoc;

    QMenu  *m_bandMenu;    
    QMenu *m_serviceMenu;

    DesPropInspector * m_propInspector;

    QComboBox *m_fontList;
    QComboBox *m_fontSize;
    QSpinBox *m_lineWidth;
    QColor m_backColor;
    QColor m_fontColor;
    QColor m_borderColor;
    int m_border;
    int m_borderstyle;
    int m_borderwidth;
    QString m_fontFamily;
    int m_fontsize;
    bool m_fontBold;
    bool m_fontUnderline;
    bool m_fontItalic;
    Qt::Alignment m_txtalignment;

    bool m_createControl;
    bool m_Changing;
    bool m_notSaveXml;

    QString m_fileName;

    QTypeLoad m_typeLoad;

    QASReportPrivate * m_Report;

    QList<QCopyRec*> m_copyControls;

    bool m_readporpvalues;

    void addPage();
    void updateListFonts();
    void readToolBarValues();
    void setDefParams();
    void resetToolBarValues();
    void setDefProps();
    QPixmap drawColor(QPixmap,QColor);
    void setPropValue(QString,QVariant);
    void saveToFile(QString);
    void loadReport();
    void selectPage(int index);
    void deletePage(int index);
    void clearAll();
    void clearCopyControls();
    void copySelControls();
    void createServiceMenu();
    void readSettings();
    void writeSettings();
    void updateVariables();
    void updateFunctions();
    void updateSysVariables();
    void clearModel(QStandardItemModel *model);
    void updateModels();
    void updateSQLModels();
    void updateOtherModels();

signals:    
    void signalSetMoveByGrid(bool);



public:
    void loadFromFile(QString);
    void loadFromStream(QString);
    void loadFromStream(QIODevice * dev);
    void loadFromStream(QByteArray & buffer);
    void setWindowCaption(QString);


};





#endif // ASREPDESIGNER_H





