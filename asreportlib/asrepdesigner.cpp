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

#include "asrepdesigner.h"
#include "ui_asrepdesigner.h"


//-------------------------------------------------------------------------------
QASRepDesigner::QASRepDesigner(QWidget *parent,QASReportPrivate * report) :
    QMainWindow(parent),
    ui(new Ui::QASRepDesigner)
{
    ui->setupUi(this);

    m_Report = report;

    setAttribute( Qt::WA_DeleteOnClose, true);
    setWindowState(Qt::WindowMaximized);

    m_xmlDoc = new QDomDocument("Reports");

    connect(ui->actionSelObject,SIGNAL(triggered()),this,SLOT(on_actionCreateControl()));
    connect(ui->actionText,SIGNAL(triggered()),this,SLOT(on_actionCreateControl()));
    connect(ui->actionBand,SIGNAL(triggered()),this,SLOT(on_actionCreateControl()));
    connect(ui->actionPicture,SIGNAL(triggered()),this,SLOT(on_actionCreateControl()));
    connect(ui->actionLine,SIGNAL(triggered()),this,SLOT(on_actionCreateControl()));
    connect(ui->actionFigure,SIGNAL(triggered()),this,SLOT(on_actionCreateControl()));
    connect(ui->actionBarcode,SIGNAL(triggered()),this,SLOT(on_actionCreateControl()));

    QActionGroup * fieldgroup = new QActionGroup(this);
    fieldgroup->addAction(ui->actionSelObject);
    fieldgroup->addAction(ui->actionText);
    fieldgroup->addAction(ui->actionBand);
    fieldgroup->addAction(ui->actionPicture);
    fieldgroup->addAction(ui->actionLine);
    fieldgroup->addAction(ui->actionFigure);
    fieldgroup->addAction(ui->actionBarcode);

    connect(ui->actionAlign_bottom,SIGNAL(triggered()),this,SLOT(slotActionAlignment()));
    connect(ui->actionAlign_top,SIGNAL(triggered()),this,SLOT(slotActionAlignment()));
    connect(ui->actionAlign_left,SIGNAL(triggered()),this,SLOT(slotActionAlignment()));
    connect(ui->actionAlign_right,SIGNAL(triggered()),this,SLOT(slotActionAlignment()));
    connect(ui->actionAlign_center,SIGNAL(triggered()),this,SLOT(slotActionAlignment()));
    connect(ui->actionAlign_middle,SIGNAL(triggered()),this,SLOT(slotActionAlignment()));
    connect(ui->actionAlign_center_horizontal,SIGNAL(triggered()),this,SLOT(slotActionAlignment()));
    connect(ui->actionAlign_center_vertical,SIGNAL(triggered()),this,SLOT(slotActionAlignment()));
    connect(ui->actionAlign_center_page_horizontal,SIGNAL(triggered()),this,SLOT(slotActionAlignment()));
    connect(ui->actionAlign_by_center_page_vertical,SIGNAL(triggered()),this,SLOT(slotActionAlignment()));

    QActionGroup * fontHaligngroup = new QActionGroup(this);

    ui->actionAlign_left_2->setData(Qt::AlignLeft);
    ui->actionAlign_centre->setData(Qt::AlignHCenter);
    ui->actionAlign_right_2->setData(Qt::AlignRight);
    ui->actionAlign_justify->setData(Qt::AlignJustify);
    ui->actionAlign_top_2->setData(Qt::AlignTop);
    ui->actionAlign_bottom_2->setData(Qt::AlignBottom);
    ui->actionAlign_center_2->setData(Qt::AlignVCenter);

    fontHaligngroup->addAction(ui->actionAlign_left_2);
    fontHaligngroup->addAction(ui->actionAlign_centre);
    fontHaligngroup->addAction(ui->actionAlign_right_2);
    fontHaligngroup->addAction(ui->actionAlign_justify);

    QActionGroup * fontValigngroup = new QActionGroup(this);
    fontValigngroup->addAction(ui->actionAlign_top_2);
    fontValigngroup->addAction(ui->actionAlign_bottom_2);
    fontValigngroup->addAction(ui->actionAlign_center_2);

    ui->actionBorder_top->setData(QDesControl::Top);
    ui->actionBorder_left->setData(QDesControl::Left);
    ui->actionBorder_right->setData(QDesControl::Right);
    ui->actionBorder_bottom->setData(QDesControl::Bottom);

    m_fontList = new QComboBox(this);
    m_fontList->setFont(QFont(m_fontList->font().family(),9));
    m_fontList->resize(100, m_fontList->height());
    ui->toolBarFont->insertWidget(ui->actionBold,m_fontList);

    m_fontSize = new QComboBox(this);
    m_fontSize->setFont(QFont(m_fontSize->font().family(),9));
    m_fontSize->resize(30, m_fontSize->height());
    ui->toolBarFont->insertWidget(ui->actionBold,m_fontSize);

    updateListFonts();

    m_lineWidth = new QSpinBox(this);
    m_lineWidth->setMinimum(1);
    m_lineWidth->setMaximum(10);
    m_lineWidth->setFocusPolicy(Qt::NoFocus);
    ui->toolBarBorder->addWidget(m_lineWidth);

    m_fontList->setEnabled(false);
    m_fontSize->setEnabled(false);
    m_lineWidth->setEnabled(false);


    connect(ui->actionBold,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionItalic,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionUnderline,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionFont_color,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionAlign_left_2,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionAlign_centre,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionAlign_right_2,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionAlign_left_2,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionAlign_justify,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionAlign_top_2,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionAlign_bottom_2,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionAlign_center_2,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionBack_color,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionLine_color,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionStyle_line,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionBorder_top,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionBorder_left,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionBorder_right,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionBorder_bottom,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionBorder_free,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionBorder_all,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(ui->actionVerttext,SIGNAL(triggered()),this,SLOT(slotUpdatePropValues()));
    connect(m_fontList,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotFontChanged(QString)));
    connect(m_fontSize,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotFontChanged(QString)));
    connect(m_lineWidth,SIGNAL(valueChanged(int)),this,SLOT(slotLineWidthChanged(int)));
    connect(ui->actionEditor,SIGNAL(triggered()),SLOT(slotUpdateAddPropValues()));
    connect(ui->actionFormat_variable,SIGNAL(triggered()),SLOT(slotUpdateAddPropValues()));
    connect(ui->actionFont,SIGNAL(triggered()),SLOT(slotUpdateAddPropValues()));
    connect(ui->actionStretchable,SIGNAL(triggered()),SLOT(slotUpdateAddPropValues()));
    connect(ui->actionWord_wrap,SIGNAL(triggered()),SLOT(slotUpdateAddPropValues()));

    connect(ui->actionText_2,SIGNAL(triggered()),SLOT(on_actionStandart_triggered()));
    connect(ui->actionBorders,SIGNAL(triggered()),SLOT(on_actionStandart_triggered()));
    connect(ui->actionObjects,SIGNAL(triggered()),SLOT(on_actionStandart_triggered()));
    connect(ui->actionAlignment,SIGNAL(triggered()),SLOT(on_actionStandart_triggered()));

    ui->splitterGen->setStretchFactor(1,2);

    m_bandMenu = new QMenu(this);

    QString name;
    QAction * action;

    for (int i = 0; i < 15; ++i){
        name = bandNames[i];

        action = m_bandMenu->addAction(name);
        action->setData(i);
    }

    connect(m_bandMenu,
            SIGNAL(triggered(QAction*)),
            SLOT(slotBandMenu(QAction*))
            );

    m_propInspector = new DesPropInspector(this);
    m_propInspector->SetTreeView(ui->propView);

    m_ActivePage = 0;

    createServiceMenu();

    FormSettings = new DesSettings(this);

    readSettings();

    ui->tabWidget->clear();

    addPage();

    ui->treeObjects->expandAll();

    setDefParams();

    m_createControl = false;

    m_Changing = false;
    m_notSaveXml = false;

    m_typeLoad = tlFile;

    setWindowCaption(UNDEFINED);

    m_readporpvalues = false;

    updateSysVariables();
    updateFunctions();

    emit FormSettings->signalUpdateRules();
    emit FormSettings->signalShowBandHeaders();

    updateModels();
}

void QASRepDesigner::clearModel(QStandardItemModel *model)
{
QStandardItem *item;

while (model->rowCount()){
    item = model->item(0);

    if(item->hasChildren())
        model->removeRows(0,item->rowCount(),item->index());

        model->removeRow(0);
}
}


void QASRepDesigner::updateModels()
{
   updateSQLModels();

   updateOtherModels();
}

void QASRepDesigner::updateOtherModels()
{
    QStandardItem *parentStrings, *parentItems, *item, *childitem;
    QObject * model;
    QString name;

    parentStrings = new QStandardItem("STRINGS");
    parentStrings->setIcon(QIcon(":/new/designer/images/List.png"));

    parentItems = new QStandardItem("ITEMS");
    parentItems->setIcon(QIcon(":/new/designer/images/table.png"));

    FormSettings->Fields()->appendRow(parentStrings);
    FormSettings->Fields()->appendRow(parentItems);

    foreach(name, m_Report->models()){
        model = m_Report->getModel(name);

        if(model->inherits("QSqlQueryModel")) continue;

        if(model->inherits("QStringListModel")){
            item = new QStandardItem(name);

            parentStrings->appendRow(item);

            childitem = new QStandardItem("0");

            item->appendRow(childitem);

            continue;
        }

        item = new QStandardItem(name);

        parentItems->appendRow(item);

        for (int c = 0; c < ((QAbstractItemModel*) model)->columnCount(); ++c) {

            childitem = new QStandardItem(QString::number(c));

            item->appendRow(childitem);
        }
    }
}

void QASRepDesigner::updateSQLModels()
{
    QStringList list, dblist;
    QString table, field;
    QStandardItem *parentItem, *item, *childitem;
    QSqlRecord record;
    QObject * model;
    QSqlDatabase db;

    clearModel(FormSettings->Fields());

    dblist = QSqlDatabase::connectionNames();

    foreach (QString conname, dblist) {
        db = QSqlDatabase::database(conname, false);

        parentItem = new QStandardItem("SQLDB(" + conname + ")");
        parentItem->setIcon(QIcon(":/new/designer/images/database.png"));

        FormSettings->Fields()->appendRow(parentItem);

        if(!db.isOpen()) continue;

        list = db.tables();

        list.sort();

        foreach (table, m_Report->models()){
            if(list.indexOf(table.toUpper()) != -1) continue;

            item = new QStandardItem(table);

            FormSettings->Tables()->appendRow(item);

            model = m_Report->getModel(table);

            if(!model) continue;
            if(!model->inherits("QSqlQueryModel"))  continue;

            item = new QStandardItem(table);

            parentItem->appendRow(item);

            record = ((QSqlQueryModel *)model)->record();

            for (int f = 0; f < record.count(); ++f) {
                field = record.fieldName(f);

                if(field.isEmpty()) return;

                childitem = new QStandardItem(field);

                item->appendRow(childitem);
            }
        }


        foreach (table, list){
            item = new QStandardItem(table);

            FormSettings->Tables()->appendRow(item);

            item = new QStandardItem(table);

            parentItem->appendRow(item);

            model = m_Report->getModel(table);

            if(model){
                if(!model->inherits("QSqlQueryModel"))  continue;

                record = ((QSqlQueryModel*)model)->record();
            } else
                record = db.record(table);

            for (int f = 0; f < record.count(); ++f) {
                field = record.fieldName(f);

                if(field.isEmpty()) return;

                childitem = new QStandardItem(field);

                item->appendRow(childitem);
            }
        }

    }
}


void QASRepDesigner::readSettings()
{
QSettings settings(QDir::currentPath() + "/settings.ini",QSettings::IniFormat);
QList<QVariant> list;
QVariant  varvalue;

settings.beginGroup("MainWindow");

list = QList<QVariant>() << 200 << 600;
varvalue = settings.value("splitterGen",list);
ui->splitterGen->setSizes(QList<int>() << varvalue.toList()[0].toInt() << varvalue.toList()[1].toInt());

list = QList<QVariant>() << 400 << 400;
varvalue = settings.value("splitterProp",list);
ui->splitter->setSizes(QList<int>() << varvalue.toList()[0].toInt() << varvalue.toList()[1].toInt());

ui->actionStandart->setChecked(settings.value("actionStandart",true).toBool());
ui->toolBarMain->setVisible(ui->actionStandart->isChecked());

ui->actionText_2->setChecked(settings.value("actionText",true).toBool());
ui->toolBarFont->setVisible(ui->actionText_2->isChecked());

ui->actionBorders->setChecked(settings.value("actionBorders",true).toBool());
ui->toolBarBorder->setVisible(ui->actionBorders->isChecked());

ui->actionObjects->setChecked(settings.value("actionObjects",true).toBool());
ui->toolBarControls->setVisible(ui->actionObjects->isChecked());

ui->actionAlignment->setChecked(settings.value("actionAlignment",true).toBool());
ui->toolBarAlign->setVisible(ui->actionAlignment->isChecked());

settings.endGroup();

}

void QASRepDesigner::writeSettings()
{
QSettings settings(QDir::currentPath() + "/settings.ini",QSettings::IniFormat);
QList<QVariant> list;

settings.beginGroup("MainWindow");
list = QList<QVariant>() << ui->splitterGen->sizes()[0] << ui->splitterGen->sizes()[1];
settings.setValue("splitterGen",list);

list = QList<QVariant>() << ui->splitter->sizes()[0] << ui->splitter->sizes()[1];
settings.setValue("splitterProp",list);

settings.setValue("actionStandart",ui->actionStandart->isChecked());
settings.setValue("actionText",ui->actionText_2->isChecked());
settings.setValue("actionBorders",ui->actionBorders->isChecked());
settings.setValue("actionObjects",ui->actionObjects->isChecked());
settings.setValue("actionAlignment",ui->actionAlignment->isChecked());

settings.endGroup();

}


void QASRepDesigner::slotUpdateAddPropValues()
{
    QDesControl *control;
    const QMetaObject* metaObject;
    QVariant value;
    int index;
    bool ok;
    QAction *action;

    if(!m_ActivePage->SelectControls().count()) return;

    action = (QAction *)QObject::sender();

    if(action == ui->actionEditor)
    {
        control = m_ActivePage->SelectControls().at(0);
        control->CallEditor();
    }

    if(action == ui->actionFont)
    {
        QFont font;
        bool exist = false;

        foreach (control, m_ActivePage->SelectControls()){
            if(control->inherits("QDesText")){exist = true; break;}
        }

        if(!exist) return;

        font = QFont(((QDesText*)control)->fontFamily(),((QDesText*)control)->fontSize());
        font.setItalic(((QDesText*)control)->fontItalic());
        font.setBold(((QDesText*)control)->fontBold());
        font.setUnderline(((QDesText*)control)->fontUnderline());

        font = QFontDialog::getFont(&ok,font);

        if(!ok) return;

        foreach (control, m_ActivePage->SelectControls()){
            metaObject = control->metaObject();

            index = metaObject->indexOfProperty("FontFamily");
            if(index != -1)
                metaObject->property(index).write(control,font.family());

            index = metaObject->indexOfProperty("FontSize");
            if(index != -1)
                metaObject->property(index).write(control,font.pointSize());

            index = metaObject->indexOfProperty("FontBold");
            if(index != -1)
                metaObject->property(index).write(control,font.bold());

            index = metaObject->indexOfProperty("FontUnderline");
            if(index != -1)
                metaObject->property(index).write(control,font.underline());

            index = metaObject->indexOfProperty("FontItalic");
            if(index != -1)
                metaObject->property(index).write(control,font.italic());

            control->update();

            readToolBarValues();
        }
    }

    if(action == ui->actionStretchable)
    {
        foreach (control, m_ActivePage->SelectControls()){
            metaObject = control->metaObject();

            index = metaObject->indexOfProperty("Stretchable");
            if(index == -1) continue;

            value = ui->actionStretchable->isChecked();

            metaObject->property(index).write(control,value);
        }
    }

    if(action == ui->actionWord_wrap)
    {
        foreach (control, m_ActivePage->SelectControls()){
            metaObject = control->metaObject();

            index = metaObject->indexOfProperty("WordWrap");
            if(index == -1) continue;

            value = ui->actionWord_wrap->isChecked();

            metaObject->property(index).write(control,value);
        }
    }

    if(action == ui->actionFormat_variable)
    {
        QDesSelFormat seldialog(this);

        if(m_ActivePage->SelectControls().count())
        {
            control = m_ActivePage->SelectControls().at(0);

            metaObject = control->metaObject();

            index = metaObject->indexOfProperty("Format");

            if(index != -1)
                value = metaObject->property(index).read(control);

            seldialog.SetFormat(value.toString());
        }

        seldialog.exec();

        if(seldialog.result() == QDialog::Accepted){
            value = seldialog.Format();

            foreach (control, m_ActivePage->SelectControls()){
                metaObject = control->metaObject();

                index = metaObject->indexOfProperty("Format");
                if(index == -1) continue;

                metaObject->property(index).write(control,value);
            }
        }
    }

    m_Changing = true;
    m_notSaveXml = true;

}

void QASRepDesigner::createServiceMenu()
{
m_serviceMenu = new QMenu(this);

m_serviceMenu->addAction(ui->actionCut);
m_serviceMenu->addAction(ui->actionCopy);
m_serviceMenu->addAction(ui->actionPast);
m_serviceMenu->addAction(ui->actionSelect_All);
m_serviceMenu->addSeparator();
m_serviceMenu->addAction(ui->actionEditor);
m_serviceMenu->addAction(ui->actionFormat_variable);
m_serviceMenu->addAction(ui->actionFont);
m_serviceMenu->addSeparator();
m_serviceMenu->addAction(ui->actionStretchable);
m_serviceMenu->addAction(ui->actionWord_wrap);
}

void QASRepDesigner::addPage()
{
QScrollArea *sa = new QScrollArea(this);
QDesPageBack *pb = new QDesPageBack(sa);
QDesignPage * page = new QDesignPage(pb,ui->tabWidget->count() + 1);
int index;

sa->setWidget(pb);

index = ui->tabWidget->addTab(sa,"Page " + QString::number(ui->tabWidget->count() + 1));

connect(page,SIGNAL(signalEndCreateControl()),this,SLOT(slotEndCreateControl()));
connect(page,SIGNAL(signalReadProps(QList<QDesControl*>)),m_propInspector,SLOT(slotReadProps(QList<QDesControl*>)));
connect(page,SIGNAL(signalCloseProps()),m_propInspector,SLOT(slotCloseProps()));
connect(page,SIGNAL(signalUpdatePropValues()),m_propInspector,SLOT(slotUpdatePropValues()));
connect(page,SIGNAL(signalReadProps(QList<QDesControl*>)),this,SLOT(slotSelectControls(QList<QDesControl*>)));
connect(m_propInspector,SIGNAL(signalReadToolBarValues()),this,SLOT(slotReadToolBarValues()));
connect(page,SIGNAL(signalCloseProps()),this,SLOT(slotUnSelectControls()));
connect(page,SIGNAL(signalSetBackInd(QPoint,bool)),pb,SLOT(slotSetBackInd(QPoint,bool)));
connect(page,SIGNAL(signalSetExpaded(QModelIndex)),ui->treeObjects,SLOT(expand(QModelIndex)));
connect(page,SIGNAL(signalUpdateBandMenu()),this,SLOT(slotUpdateBandMenu()));
connect(page,SIGNAL(signalSelectInTree(QModelIndex)),ui->treeObjects,SLOT(setCurrentIndex(QModelIndex)));
connect(page,SIGNAL(signalClearSelectInTree()),ui->treeObjects,SLOT(clearSelection()));
connect(ui->treeObjects,SIGNAL(clicked(QModelIndex)),this,SLOT(slotSelectFromTree(QModelIndex)));
connect(page,SIGNAL(signalUpdateMenuUndo()),this,SLOT(slotUpdateMenuUndo()));
connect(page,SIGNAL(signalChanging()),this,SLOT(slotChanging()));
connect(FormSettings,SIGNAL(signalUpdateRules()),page,SLOT(slotUpdateRules()));


page->SetServiceMenu(m_serviceMenu);

ui->tabWidget->setCurrentIndex(index);

selectPage(index);
}
void QASRepDesigner::selectPage(int index)
{
QDesignPage * page;

page =  ui->tabWidget->widget(index)->findChild<QDesignPage *>();

m_ActivePage = page;

ui->treeObjects->setModel(m_ActivePage->ModelTreeObj());
ui->treeObjects->expandAll();

slotUpdateBandMenu();
slotUpdateMenuUndo();

emit m_ActivePage->signalReadProps(m_ActivePage->SelectControls());
}

void QASRepDesigner::on_tabWidget_currentChanged(int index)
{
if(!m_ActivePage)  return;

selectPage(index);
}

void QASRepDesigner::deletePage(int index)
{
if(index > ui->tabWidget->count() - 1) return;

QWidget *wid =  ui->tabWidget->widget(index);
QDesignPage * page  =  ui->tabWidget->widget(index)->findChild<QDesignPage *>();

page->ClearAll();

if(ui->tabWidget->count() == 1)
  return;

ui->tabWidget->removeTab(index);

delete wid;

}

void QASRepDesigner::clearAll()
{
int index = ui->tabWidget->count() - 1;

    while (index >= 0){
        deletePage(index);

        --index;
    }

    setWindowCaption(UNDEFINED);

    m_typeLoad = tlFile;

    m_fileName = "";

    slotUpdateBandMenu();
}

void QASRepDesigner::slotLineWidthChanged(int value)
{
QString propname = "BorderWidth";
int index;

if(m_readporpvalues) return;

if(!m_ActivePage->SelectControls().count()) return;

emit m_ActivePage->SelectControls().at(0)->signalSaveUndo(uaEdit,propname);

foreach (QDesControl *control,m_ActivePage->SelectControls()){

index = control->metaObject()->indexOfProperty(propname.toLocal8Bit().data());

if(index == -1) continue;

control->metaObject()->property(index).write(control, value);
control->update();
}

m_borderwidth = value;

slotChanging();

emit m_ActivePage->signalUpdatePropValues();
}


void QASRepDesigner::slotFontChanged(QString value)
{
QObject *sender = (QObject *)QObject::sender();
QString propname;
int index;

if(m_readporpvalues) return;

if(!m_ActivePage->SelectControls().count()) return;

propname = sender == m_fontList?"FontFamily":"FontSize";

emit m_ActivePage->SelectControls().at(0)->signalSaveUndo(uaEdit,propname);

foreach (QDesControl *control,m_ActivePage->SelectControls()){

index = control->metaObject()->indexOfProperty(propname.toLocal8Bit().data());

if(index == -1) continue;

control->metaObject()->property(index).write(control, value);
control->update();
}

if(propname == "FontFamily")
m_fontFamily = value;

if(propname == "FontSize")
m_fontsize = value.toInt();

slotChanging();

emit m_ActivePage->signalUpdatePropValues();
}

void QASRepDesigner::slotUpdatePropValues()
{
QAction *action = (QAction *)QObject::sender();
QString propname;
QVariant value = 0;
int index;
QDesColorPanel *colorPanel;
QDesLinePanel *linepanel;
QWidget *but;
QPoint p;
QColor color;

if(!m_ActivePage->SelectControls().count()) return;

if(action == ui->actionBold)
    propname = "FontBold";

if(action == ui->actionUnderline)
    propname = "FontUnderline";

if(action == ui->actionItalic)
    propname = "FontItalic";

if(action == ui->actionFont_color)
    propname = "FontColor";

if(action == ui->actionAlign_left_2)
    propname = "TextAlignment";

if(action == ui->actionAlign_centre)
    propname = "TextAlignment";

if(action == ui->actionAlign_right_2)
    propname = "TextAlignment";

if(action == ui->actionAlign_justify)
    propname = "TextAlignment";

if(action == ui->actionAlign_top_2)
    propname = "TextAlignment";

if(action == ui->actionAlign_bottom_2)
    propname = "TextAlignment";

if(action == ui->actionAlign_center_2)
    propname = "TextAlignment";

if(action == ui->actionBack_color)
    propname = "BackColor";

if(action == ui->actionLine_color)
    propname = "BorderColor";

if(action == ui->actionStyle_line)
    propname = "BorderStyle";

if(action == ui->actionBorder_top)
    propname = "Border";

if(action == ui->actionBorder_top)
    propname = "Border";

if(action == ui->actionBorder_left)
    propname = "Border";

if(action == ui->actionBorder_right)
    propname = "Border";

if(action == ui->actionBorder_bottom)
    propname = "Border";

if(action == ui->actionBorder_free)
    propname = "Border";

if(action == ui->actionBorder_all)
    propname = "Border";

if(action == ui->actionVerttext)
    propname = "AngleType";

emit m_ActivePage->SelectControls().at(0)->signalSaveUndo(uaEdit,propname);

foreach (QDesControl *control,m_ActivePage->SelectControls()){

    index = control->metaObject()->indexOfProperty(propname.toLocal8Bit().data());

    if(index == -1) continue;

    value = control->metaObject()->property(index).read(control);

    if(action == ui->actionBold){
        value = ui->actionBold->isChecked();
        m_fontBold = value.toBool();
    }

   if(action == ui->actionItalic){
       value = ui->actionItalic->isChecked();
       m_fontItalic = value.toBool();
   }

   if(action == ui->actionUnderline){
       value = ui->actionUnderline->isChecked();
       m_fontUnderline = value.toBool();
   }
   if(action == ui->actionAlign_left_2){
       value = (value.toInt() & 0xf0) | (ui->actionAlign_left_2->isChecked()?Qt::AlignLeft:0);
       m_txtalignment = (Qt::Alignment)value.toInt();
   }

   if(action == ui->actionAlign_centre){
       value = (value.toInt() & 0xf0) | (ui->actionAlign_centre->isChecked()?Qt::AlignHCenter:0);
       m_txtalignment = (Qt::Alignment)value.toInt();
   }

   if(action == ui->actionAlign_right_2){
       value = (value.toInt() & 0xf0) | (ui->actionAlign_right_2->isChecked()?Qt::AlignRight:0);
       m_txtalignment = (Qt::Alignment)value.toInt();
   }

   if(action == ui->actionAlign_justify){
       value = (value.toInt() & 0xf0) | (ui->actionAlign_justify->isChecked()?Qt::AlignJustify:0);
       m_txtalignment = (Qt::Alignment)value.toInt();
   }

   if(action == ui->actionAlign_top_2){
       value = (value.toInt() & 0x0f) | (ui->actionAlign_top_2->isChecked()?Qt::AlignTop:0);
       m_txtalignment = (Qt::Alignment)value.toInt();
   }

   if(action == ui->actionAlign_bottom_2){
       value = (value.toInt() & 0x0f) | (ui->actionAlign_bottom_2->isChecked()?Qt::AlignBottom:0);
       m_txtalignment = (Qt::Alignment)value.toInt();
   }

   if(action == ui->actionAlign_center_2){
       value = (value.toInt() & 0x0f) | (ui->actionAlign_center_2->isChecked()?Qt::AlignVCenter:0);
       m_txtalignment = (Qt::Alignment)value.toInt();
   }

   if(action == ui->actionFont_color)
   {
       color = value.value<QColor>();

       colorPanel = new QDesColorPanel(this,color);
       but = ui->toolBarFont->widgetForAction(ui->actionFont_color);

       p = ui->toolBarFont->mapToGlobal(but->pos() + QPoint(but->width()/2,but->height()));

       colorPanel->move(p);
       connect(colorPanel,SIGNAL(signalSelectColor(QColor)),this,SLOT(slotSelectFontColor(QColor)));
       colorPanel->show();

       return;
   }

   if(action == ui->actionBack_color)
   {
       color = value.value<QColor>();
       colorPanel = new QDesColorPanel(this,color);//m_backColor
       but = ui->toolBarBorder->widgetForAction(ui->actionBack_color);

       p = ui->toolBarBorder->mapToGlobal(but->pos() + QPoint(but->width()/2,but->height()));

       colorPanel->move(p);
       connect(colorPanel,SIGNAL(signalSelectColor(QColor)),this,SLOT(slotSelectBackColor(QColor)));
       colorPanel->show();

       return;
   }

   if(action == ui->actionLine_color)
   {
       color = value.value<QColor>();
       colorPanel = new QDesColorPanel(this,color);//m_borderColor
       but = ui->toolBarBorder->widgetForAction(ui->actionLine_color);

       p = ui->toolBarBorder->mapToGlobal(but->pos() + QPoint(but->width()/2,but->height()));

       colorPanel->move(p);
       connect(colorPanel,SIGNAL(signalSelectColor(QColor)),this,SLOT(slotSelectBorderColor(QColor)));
       colorPanel->show();

       return;
   }

   if(action == ui->actionStyle_line)
   {
    linepanel = new QDesLinePanel(this, value.toInt());//m_borderstyle

    but = ui->toolBarBorder->widgetForAction(ui->actionStyle_line);

    p = ui->toolBarBorder->mapToGlobal(but->pos() + QPoint(but->width()/2,but->height()));

    linepanel->move(p);
    connect(linepanel,SIGNAL(signalSelectLine(int)),this,SLOT(slotSelectLineStyle(int)));
    linepanel->show();
   }

   if(action == ui->actionBorder_left){
       value = (value.toInt() & ~0x01) | (ui->actionBorder_left->isChecked()?QDesControl::Left:0);
       m_border = value.toInt();
   }

   if(action == ui->actionBorder_right){
       value = (value.toInt() & ~0x02) | (ui->actionBorder_right->isChecked()?QDesControl::Right:0);
       m_border = value.toInt();
   }

   if(action == ui->actionBorder_top){
       value = (value.toInt() & ~0x04)  | (ui->actionBorder_top->isChecked()?QDesControl::Top:0);
       m_border = value.toInt();
   }

   if(action == ui->actionBorder_bottom){
       value = (value.toInt() & ~0x08) | (ui->actionBorder_bottom->isChecked()?QDesControl::Bottom:0);
       m_border = value.toInt();
   }

   if(action == ui->actionBorder_free){
     value = 0;
     ui->actionBorder_top->setChecked(false); ui->actionBorder_left->setChecked(false);
     ui->actionBorder_right->setChecked(false); ui->actionBorder_bottom->setChecked(false);

     m_border = value.toInt();
   }

   if(action == ui->actionBorder_all){
     value = QDesControl::Top | QDesControl::Left | QDesControl::Right | QDesControl::Bottom;
     ui->actionBorder_top->setChecked(true); ui->actionBorder_left->setChecked(true);
     ui->actionBorder_right->setChecked(true); ui->actionBorder_bottom->setChecked(true);

     m_border = value.toInt();
   }

    if(action == ui->actionVerttext)
        value = ui->actionVerttext->isChecked()?QDesText::Vertical:QDesText::Horizontal;

    control->metaObject()->property(index).write(control, value);
    control->update();
}

if(m_ActivePage->SelectControls().count())
{
    slotChanging();

    emit m_ActivePage->signalUpdatePropValues();
}
}

void QASRepDesigner::setPropValue(QString propname, QVariant value)
{
int index;

if(m_ActivePage->SelectControls().count())
{
emit m_ActivePage->SelectControls().at(0)->signalSaveUndo(uaEdit,propname);

slotChanging();
}

foreach (QDesControl *control,m_ActivePage->SelectControls()){
    index = control->metaObject()->indexOfProperty(propname.toLocal8Bit().data());

    if(index == -1) continue;

    control->metaObject()->property(index).write(control, value);
    control->update();
}

emit m_ActivePage->signalUpdatePropValues();
}

void QASRepDesigner::slotSelectLineStyle(int style)
{
m_borderstyle =  style;

setPropValue("BorderStyle",m_borderstyle);
}

void QASRepDesigner::slotSelectFontColor(QColor color)
{
QPixmap pixmap = QPixmap(":/new/designer/images/Text color.png");
QColor tempcol = color;

if(color == Qt::transparent)
          tempcol = Qt::black;

m_fontColor = tempcol;
ui->actionFont_color->setIcon(QIcon(drawColor(pixmap,color)));

setPropValue("FontColor",m_fontColor);
}

void QASRepDesigner::slotSelectBackColor(QColor color)
{
    QPixmap pixmap = QPixmap(":/new/designer/images/back color.png");
    QColor tempcol = color;

    if(color == Qt::transparent)
              tempcol = Qt::white;

    m_backColor = tempcol;
    ui->actionBack_color->setIcon(QIcon(drawColor(pixmap,color)));

    setPropValue("BackColor",m_backColor);
}

void QASRepDesigner::slotSelectBorderColor(QColor color)
{
    QPixmap pixmap = QPixmap(":/new/designer/images/pen color.png");
    QColor tempcol = color;

    if(color == Qt::transparent)
              tempcol = Qt::black;

    m_borderColor = tempcol;
    ui->actionLine_color->setIcon(QIcon(drawColor(pixmap,color)));

    setPropValue("BorderColor",m_borderColor);
 }

QPixmap QASRepDesigner::drawColor(QPixmap inpixmap,QColor col)
{
    QPainter painter;
    QPixmap pixmap = inpixmap;

    painter.begin(&pixmap);

    QBrush brush;
    QPen pen;
    QRect rect = QRect(0,inpixmap.height() - 3,inpixmap.width(),inpixmap.height());

    pen.setColor(Qt::darkGray);
    pen.setStyle(Qt::SolidLine);//NoPen
    pen.setWidth(1);

    brush.setColor(col);

    if(col == Qt::transparent)
        brush.setStyle(Qt::NoBrush);
    else
    {
        brush.setStyle(Qt::SolidPattern);

    }

    painter.setPen(pen);
    painter.setBrush(brush);

    painter.drawRect(rect);

    painter.end();

    return pixmap;
}

void QASRepDesigner::setDefProps()
{
QDesControl * control;
int index;
QStringList proplist;
QVariant value = 0;

if(!m_ActivePage->SelectControls().count()) return;

control = m_ActivePage->SelectControls().at(0);

proplist = QString(PROPNAMES).split(',');

foreach(QString propname, proplist){

index = control->metaObject()->indexOfProperty(propname.toLocal8Bit().data());

if(index == -1) continue;


if(propname == "BackColor")
        value = m_backColor;

if(propname == "FontColor")
        value = m_fontColor;

if(propname == "BorderColor")
        value = m_borderColor;

if(propname == "BorderWidth")
        value = m_borderwidth;

if(propname == "Border")
        value = m_border;

if(propname == "BorderStyle")
        value = m_borderstyle;

if(propname == "TextAlignment")
        value = (int)m_txtalignment;

if(propname == "FontFamily")
        value = m_fontFamily;

if(propname == "FontSize")
        value = m_fontsize;

if(propname == "FontBold")
        value = m_fontBold;

if(propname == "FontUnderline")
        value = m_fontUnderline;

if(propname == "FontItalic")
        value = m_fontItalic;

 control->metaObject()->property(index).write(control, value);
}

control->update();

readToolBarValues();
}



void QASRepDesigner::setDefParams()
{
    m_backColor = Qt::white;
    m_fontColor = Qt::black;
    m_borderColor = Qt::black;

    m_txtalignment = Qt::AlignVCenter | Qt::AlignLeft;

    m_border = 0;
    m_borderstyle = Qt::SolidLine;//Qt::NoPen;
    m_borderwidth = 1;

    m_fontFamily = "Arial";
    m_fontsize = 10;
    m_fontBold = false;
    m_fontUnderline = false;
    m_fontItalic = false;

    m_fontList->setCurrentText(m_fontFamily);
    m_fontSize->setCurrentText(QString::number(m_fontsize));
    m_lineWidth->setValue(m_borderwidth);
    ui->actionBold->setChecked(m_fontBold);
    ui->actionItalic->setChecked(m_fontItalic);
    ui->actionUnderline->setChecked(m_fontUnderline);
    ui->actionAlign_left_2->setChecked(m_txtalignment & Qt::AlignLeft);
    ui->actionAlign_centre->setChecked(m_txtalignment & Qt::AlignHCenter);
    ui->actionAlign_right_2->setChecked(m_txtalignment & Qt::AlignRight);
    ui->actionAlign_justify->setChecked(m_txtalignment & Qt::AlignJustify);
    ui->actionAlign_top_2->setChecked(m_txtalignment & Qt::AlignTop);
    ui->actionAlign_bottom_2->setChecked(m_txtalignment & Qt::AlignBottom);
    ui->actionAlign_center_2->setChecked(m_txtalignment & Qt::AlignVCenter);
    ui->actionBorder_top->setChecked(m_border & QDesControl::Top);
    ui->actionBorder_left->setChecked(m_border & QDesControl::Left);
    ui->actionBorder_right->setChecked(m_border & QDesControl::Right);
    ui->actionBorder_bottom->setChecked(m_border & QDesControl::Bottom);
    ui->actionVerttext->setChecked(false);
}



void QASRepDesigner::resetToolBarValues()
{
ui->actionBorder_top->setChecked(false);
ui->actionBorder_left->setChecked(false);
ui->actionBorder_right->setChecked(false);
ui->actionBorder_bottom->setChecked(false);
ui->actionAlign_left_2->setChecked(false);
ui->actionAlign_centre->setChecked(false);
ui->actionAlign_right_2->setChecked(false);
ui->actionAlign_justify->setChecked(false);
ui->actionAlign_top_2->setChecked(false);
ui->actionAlign_bottom_2->setChecked(false);
ui->actionAlign_center_2->setChecked(false);
ui->actionBold->setChecked(false);
ui->actionItalic->setChecked(false);
ui->actionUnderline->setChecked(false);
ui->actionStretchable->setChecked(false);
ui->actionWord_wrap->setChecked(false);
ui->actionVerttext->setChecked(false);
}

void QASRepDesigner::readToolBarValues()
{
QListControls list = m_ActivePage->SelectControls();
QDesControl * control;
const QMetaObject* metaObject;
QVariant value;
int index;
QString propname;
QStringList proplist;

    if(!list.count()) return;
    if(list.count() > 1){resetToolBarValues(); return;}

    control = list.at(0);

   m_readporpvalues = true;

   proplist = QString(PROPNAMES).split(',');

    metaObject = control->metaObject();

    for (int i = 0; i < proplist.count(); ++i) {
        propname = proplist.at(i);

        index = metaObject->indexOfProperty(propname.toLocal8Bit().data());

        if(index == -1) continue;

        value = metaObject->property(index).read(control);

        if(propname == "BorderWidth")
               m_lineWidth->setValue(value.toInt());

        if(propname == "Border")
        {
            ui->actionBorder_top->setChecked(value.toInt() & QDesControl::Top);
            ui->actionBorder_left->setChecked(value.toInt() & QDesControl::Left);
            ui->actionBorder_right->setChecked(value.toInt() & QDesControl::Right);
            ui->actionBorder_bottom->setChecked(value.toInt() & QDesControl::Bottom);
       }

       if(propname == "TextAlignment")
       {
           ui->actionAlign_left_2->setChecked(value.toInt() & Qt::AlignLeft);
           ui->actionAlign_centre->setChecked(value.toInt() & Qt::AlignHCenter);
           ui->actionAlign_right_2->setChecked(value.toInt() & Qt::AlignRight);
           ui->actionAlign_justify->setChecked(value.toInt() & Qt::AlignJustify);
           ui->actionAlign_top_2->setChecked(value.toInt() & Qt::AlignTop);
           ui->actionAlign_bottom_2->setChecked(value.toInt() & Qt::AlignBottom);
           ui->actionAlign_center_2->setChecked(value.toInt() & Qt::AlignVCenter);
        }

       if(propname == "FontFamily")
            m_fontList->setCurrentText(value.toString());

        if(propname == "FontSize")
            m_fontSize->setCurrentText(value.toString());

        if(propname == "FontBold")
            ui->actionBold->setChecked(value.toBool());

        if(propname == "FontUnderline")
            ui->actionUnderline->setChecked(value.toBool());

        if(propname == "FontItalic")
             ui->actionItalic->setChecked(value.toBool());

        if(propname == "BackColor")
            ui->actionBack_color->setData(value.toInt());

        if(propname == "FontColor")
           ui->actionFont_color->setData(value.toInt());

        if(propname == "BorderColor")
           ui->actionLine_color->setData(value.toInt());

        if(propname == "WordWrap")
          ui->actionWord_wrap->setChecked(value.toBool());

        if(propname == "Stretchable")
            ui->actionStretchable->setChecked(value.toBool());

        if(propname == "AngleType")
            ui->actionVerttext->setChecked(value.toInt() == 1);
    }

    m_readporpvalues = false;
}

void QASRepDesigner::updateListFonts()
{
QStringListModel *fontmodel = new QStringListModel(this), *fontsizemodel = new QStringListModel(this);
QStringList list;
QFontDatabase database;

foreach (int size, database.standardSizes())
        list << QString::number(size);

fontsizemodel->setStringList(list);

m_fontSize->setModel(fontsizemodel);

list.clear();

foreach (QString family, database.families()) {
         if(database.isSmoothlyScalable(family))
          list << family;
}

fontmodel->setStringList(list);

m_fontList->setModel(fontmodel);
}

void QASRepDesigner::slotActionAlignment()
{
QObject *action = QObject::sender();
QTypeAlign type;

if(action == ui->actionAlign_bottom)
    type = taBottom;

if(action == ui->actionAlign_top)
    type = taTop;

if(action == ui->actionAlign_left)
    type = taLeft;

if(action == ui->actionAlign_right)
    type = taRight;

if(action == ui->actionAlign_center)
    type = taCenter;

if(action == ui->actionAlign_middle)
    type = taMiddle;

if(action == ui->actionAlign_center_horizontal)
    type = taEvenlyWidth;

if(action == ui->actionAlign_center_vertical)
    type = taEvenlyHeight;

if(action == ui->actionAlign_center_page_horizontal)
    type = taCenterPageHor;

if(action == ui->actionAlign_by_center_page_vertical)
    type = taCenterPageVer;


m_ActivePage->AlignmentSel(type);
}

void QASRepDesigner::slotBandMenu(QAction* action)
{
QBandType type = (QBandType)action->data().toInt();

m_ActivePage->SetCreateBand(type);
m_ActivePage->BeginCreateControl(dtBand);
}

void QASRepDesigner::slotUpdateBandMenu()
{
 QAction* action;
 int type;

 for (int i = 0; i < m_bandMenu->actions().count(); ++i){

        action = m_bandMenu->actions().at(i);
        type = action->data().toInt();        

        action->setEnabled(m_ActivePage->CanCreateBand((QBandType)type));
 }
}



void QASRepDesigner::slotEndCreateControl()
{
 ui->actionSelObject->setChecked(true);

 m_createControl = false;

 setDefProps();
}


void QASRepDesigner::on_actionCreateControl()
{
if(!m_ActivePage) return;

QObject *action = QObject::sender();
int type = -1;

if(action == ui->actionSelObject)
        type = -1;

if(action == ui->actionText)
        type = dtText;

if(action == ui->actionBand)
{
    QWidget *but =  ui->toolBarControls->widgetForAction(ui->actionBand);
    QPoint gp;

    gp = but->parentWidget()->mapToGlobal(but->pos());

    gp.setX(gp.x() + but->width()/2);
    gp.setY(gp.y() + but->height()/2);

    m_bandMenu->popup(gp);

}

if(action == ui->actionPicture)
        type = dtPicture;

if(action == ui->actionLine)
        type = dtLine;

if(action == ui->actionFigure)
        type = dtFigure;

if(action == ui->actionBarcode)
        type = dtBarCode;

m_ActivePage->BeginCreateControl(type);

m_createControl = true;
}


void QASRepDesigner::slotUpdateMenuUndo()
{
ui->actionUndo->setEnabled(m_ActivePage->UndoCount(urUndo));
ui->actionRedo->setEnabled(m_ActivePage->UndoCount(urRedo));
}

void QASRepDesigner::slotSelectFromTree(QModelIndex index)
{
QScrollArea *sa;
QDesControl*control;
int sp;
QRect vrect, wrect;

control = m_ActivePage->GetControlByIndex(index);

if(!control) return;

sa = (QScrollArea *)ui->tabWidget->currentWidget();

if(!sa)return;

wrect = sa->widget()->rect();
vrect = sa->viewport()->rect();

sp = sa->verticalScrollBar()->sliderPosition();

if(sp > (control->y() + PAGEPOS))
    sa->verticalScrollBar()->setSliderPosition((control->y() + PAGEPOS));

if((control->y() + PAGEPOS) > (sp + vrect.height()))
    sa->verticalScrollBar()->setSliderPosition(sa->verticalScrollBar()->maximum() - (wrect.height() - control->y()) + control->height() + PAGEPOS);

emit control->signalSelectControl(control,false);

}

void QASRepDesigner::slotSelectControls(QList<QDesControl*>list)
{
bool text = false, line = false, figure  = false, image = false, band = false, barcode = false;

foreach (QDesControl *control, list){
    if(!text && control->inherits("QDesText")) {text = true; }
    if(!line && control->inherits("QDesLine")) {line = true; }
    if(!figure && control->inherits("QDesFigure")) {figure = true;}
    if(!image && control->inherits("QDesImage")) {image = true;}
    if(!band && control->inherits("QDesBand")) {band = true;}
    if(!band && control->inherits("QDesBarCode")) {barcode = true;}
}

ui->actionCut->setEnabled(true);
ui->actionCopy->setEnabled(true);
ui->actionObject_down->setEnabled(true);
ui->actionObject_up->setEnabled(true);
ui->actionDelete->setEnabled(true);

ui->actionBold->setEnabled(text);
ui->actionItalic->setEnabled(text);
ui->actionUnderline->setEnabled(text);
ui->actionFont_color->setEnabled(text);
ui->actionAlign_left_2->setEnabled(text);
ui->actionAlign_centre->setEnabled(text);
ui->actionAlign_right_2->setEnabled(text);
ui->actionAlign_left_2->setEnabled(text);
ui->actionAlign_justify->setEnabled(text);
ui->actionAlign_top_2->setEnabled(text);
ui->actionAlign_bottom_2->setEnabled(text);
ui->actionAlign_center_2->setEnabled(text);
ui->actionBack_color->setEnabled(text || figure);
ui->actionLine_color->setEnabled(text || figure || image || line || barcode);
ui->actionStyle_line->setEnabled(text || figure || image || line || barcode);
ui->actionBorder_top->setEnabled(text ||  image || barcode);
ui->actionBorder_left->setEnabled(text ||  image || barcode);
ui->actionBorder_right->setEnabled(text ||  image || barcode);
ui->actionBorder_bottom->setEnabled(text ||  image || barcode);
ui->actionBorder_free->setEnabled(text ||  image || barcode);
ui->actionBorder_all->setEnabled(text ||  image || barcode);
ui->actionVerttext->setEnabled(text);
m_fontList->setEnabled(text);
m_fontSize->setEnabled(text);
m_lineWidth->setEnabled(text || figure || image || line || barcode);

ui->actionEditor->setEnabled(list.count() == 1);
ui->actionFormat_variable->setEnabled(list.count() == 1);
ui->actionFormat_variable->setVisible(text && !figure && !image && !line && !band && !barcode);
ui->actionFont->setVisible(text && !figure && !image && !line && !band && !barcode);
ui->actionStretchable->setVisible(text || band);
ui->actionWord_wrap->setVisible(text && !figure && !image && !line && !band && !barcode);

if(!m_createControl)
        readToolBarValues();
}

void QASRepDesigner::slotReadToolBarValues()
{
readToolBarValues();

slotChanging();
}

void QASRepDesigner::slotUnSelectControls()
{
ui->actionCut->setEnabled(false);
ui->actionCopy->setEnabled(false);
ui->actionObject_down ->setEnabled(false);
ui->actionObject_up->setEnabled(false);
ui->actionDelete->setEnabled(false);

ui->actionBold->setEnabled(false);
ui->actionItalic->setEnabled(false);
ui->actionUnderline->setEnabled(false);
ui->actionFont_color->setEnabled(false);
ui->actionAlign_left_2->setEnabled(false);
ui->actionAlign_centre->setEnabled(false);
ui->actionAlign_right_2->setEnabled(false);
ui->actionAlign_left_2->setEnabled(false);
ui->actionAlign_justify->setEnabled(false);
ui->actionAlign_justify->setEnabled(false);
ui->actionAlign_top_2->setEnabled(false);
ui->actionAlign_bottom_2->setEnabled(false);
ui->actionAlign_center_2->setEnabled(false);
ui->actionBack_color->setEnabled(false);
ui->actionLine_color->setEnabled(false);
ui->actionBack_color->setEnabled(false);
ui->actionStyle_line->setEnabled(false);
ui->actionBorder_top->setEnabled(false);
ui->actionBorder_left->setEnabled(false);
ui->actionBorder_right->setEnabled(false);
ui->actionBorder_bottom->setEnabled(false);
ui->actionBorder_free->setEnabled(false);
ui->actionBorder_all->setEnabled(false);
ui->actionVerttext->setEnabled(false);

m_fontList->setEnabled(false);
m_fontSize->setEnabled(false);
m_lineWidth->setEnabled(false);

ui->actionEditor->setEnabled(false);
ui->actionFormat_variable->setVisible(false);
ui->actionFont->setVisible(false);
ui->actionStretchable->setVisible(false);
ui->actionWord_wrap->setVisible(false);

}

QASRepDesigner::~QASRepDesigner()
{
    writeSettings();

    if(m_Changing)
    {
        if(m_typeLoad == tlFile) {
            int n = QMessageBox::question(0,
                                          tr("Message"),
                                          tr("The report has changed"
                                             "\n Do you want to save the changes?"),
                                          QMessageBox::Yes | QMessageBox::No,
                                          QMessageBox::Yes
                                          );

            if (n == QMessageBox::Yes)
                on_actionSave_triggered();
        }else
            on_actionSave_triggered();
    }

    clearCopyControls();

    delete FormSettings;
    delete m_xmlDoc;
    delete ui;
}

void QASRepDesigner::on_actionNew_triggered()
{
    if(m_Changing)
    {
        int n = QMessageBox::question(0,
                                     tr("Message"),
                                     tr("The report has changed"
                                        "\n Do you want to save the changes?"),
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::Yes
                                     );

        if (n == QMessageBox::Yes)
            on_actionSave_triggered();
    }

    clearAll();
}

void QASRepDesigner::on_actionClose_triggered()
{
 close();
}

void QASRepDesigner::on_actionSave_and_close_triggered()
{
   if(m_Changing)on_actionSave_triggered();

    close();
}

void QASRepDesigner::on_actionSelect_All_triggered()
{
   m_ActivePage->SelectAll();
}

void QASRepDesigner::on_actionDelete_triggered()
{
 m_ActivePage->DeleteSel();
}

void QASRepDesigner::on_actionObject_up_triggered()
{
 m_ActivePage->ZReordSelControl(zoRise);
}

void QASRepDesigner::on_actionObject_down_triggered()
{
 m_ActivePage->ZReordSelControl(zoLow);
}


void QASRepDesigner::on_actionUndo_triggered()
{
   m_ActivePage->Undo(urUndo);
}

void QASRepDesigner::on_actionRedo_triggered()
{
   m_ActivePage->Undo(urRedo);
}


void QASRepDesigner::saveToFile(QString filename)
{
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly)) {
        QTextStream txtstream(&file);
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");

        txtstream.setCodec(codec);

        txtstream << m_xmlDoc->toString();
        file.close();

    }
}

void QASRepDesigner::SaveReport()
{
QDesignPage * page;
QDomElement reports, report;

if(m_Changing)
{
m_xmlDoc->clear();
m_xmlDoc->insertBefore(QDomNode(m_xmlDoc->createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"")),m_xmlDoc->firstChild());

reports = m_xmlDoc->createElement("Reports");
m_xmlDoc->appendChild(reports);

 for (int i = 0; i < ui->tabWidget->count(); ++i){

     report = m_xmlDoc->createElement("Report");
     reports.appendChild(report);

     page  =  ui->tabWidget->widget(i)->findChild<QDesignPage *>();

     if(page) page->Save(m_xmlDoc,report);
 }

 SaveVariables();

m_notSaveXml = false;
}
}

void QASRepDesigner::on_actionSave_triggered()
{
    SaveReport();

    m_Changing  = false;
    ui->actionSave->setEnabled(m_Changing);

    if(m_typeLoad == tlFile)
    {
        if(m_fileName.isEmpty())
        {
            QString filereport = QFileDialog::getSaveFileName(this, tr("Save Dialog"), "","Report file (*" EXTREPFILE ")");
            if(filereport.isEmpty()) return;

            m_fileName = filereport;
            setWindowCaption(m_fileName);
        }

        saveToFile(m_fileName);
    }else
        emit  m_Report->signalSaveToStream(m_xmlDoc);
}

void QASRepDesigner::slotChanging()
{
 m_Changing  = true;
 m_notSaveXml = true;
 ui->actionSave->setEnabled(m_Changing);
}

void QASRepDesigner::updateSysVariables()
{
QStringList list = QString(SYSVARIABLES).split(',');
QString value;
QStandardItem *parent, *item;

clearModel(FormSettings->SysVars());

parent = new QStandardItem("System variables");
FormSettings->SysVars()->appendRow(parent);

foreach (value, list){
    item = new QStandardItem(value.toLower());
    parent->appendRow(item);
}
}

void QASRepDesigner::updateFunctions()
{
QStandardItem *parent, *funcs, *item;
QStringList list;

parent = new QStandardItem("Functions");

FormSettings->Functions()->appendRow(parent);

funcs = new QStandardItem("Agregate");

parent->appendRow(funcs);

list = QString(AGREGATEFUNCS).split(',');

foreach (QString value, list) {
    item = new QStandardItem(value.toLower());
    funcs->appendRow(item);
}

}

void QASRepDesigner::updateVariables()
{
QStandardItem *groupitem, *item;
QDomElement variables;
QDomNodeList nodelist;
QDomNode node, groupnode;

clearModel(FormSettings->Variables());

nodelist = m_xmlDoc->elementsByTagName("Variables");

if(!nodelist.count()) return;

variables = nodelist.at(0).toElement();

node = variables.firstChild();
while(!node.isNull()){
    if(node.toElement().tagName() != "Variable"){node = node.nextSibling(); continue;}

    item = new QStandardItem(node.toElement().attribute("Text"));

    FormSettings->Variables()->appendRow(item);

    node = node.nextSibling();
}

nodelist = variables.elementsByTagName("Group");
for (int i = 0; i < nodelist.count(); ++i)
{
    groupnode = nodelist.at(i);

    groupitem = new QStandardItem(groupnode.toElement().attribute("Text"));

    FormSettings->Variables()->appendRow(groupitem);

    node = groupnode.firstChild();
    while (!node.isNull()){
     if(node.toElement().tagName() != "Variable"){node = node.nextSibling(); continue;}

     item = new QStandardItem(node.toElement().attribute("Text"));

     groupitem->appendRow(item);

     node = node.nextSibling();
    }
}
}

void QASRepDesigner::SaveVariables()
{
QDomElement variables, groupelem, reports, elem;
QStandardItem  *item, *childitem;
QDomNodeList list;

variables = m_xmlDoc->elementById("Variables");

if(!variables.isNull())
    variables.clear();

list = m_xmlDoc->elementsByTagName("Reports");
if(!list.count()) return;

reports = list.at(0).toElement();

variables = m_xmlDoc->createElement("Variables");
reports.appendChild(variables);

for (int i = 0; i < FormSettings->Variables()->rowCount(); ++i)
{
    item = FormSettings->Variables()->item(i);

    if(!item->parent())
    {
        groupelem = m_xmlDoc->createElement("Group");
        groupelem.setAttribute("Text",item->text());

        variables.appendChild(groupelem);

        for (int c = 0; c < item->rowCount(); ++c){
            childitem = item->child(c);

            elem = m_xmlDoc->createElement("Variable");
            elem.setAttribute("Text",childitem->text());

            groupelem.appendChild(elem);
        }
    } else{
        elem = m_xmlDoc->createElement("Variable");
        elem.setAttribute("Text",item->text());

        variables.appendChild(elem);
    }
}
}

void QASRepDesigner::loadReport()
{
QDomElement report;
QDesignPage *page;

for (int i = 0; i < m_xmlDoc->documentElement().childNodes().count(); ++i)
{
    if(m_xmlDoc->documentElement().childNodes().at(i).toElement().tagName() != "Report")continue;

    report = m_xmlDoc->documentElement().childNodes().at(i).toElement();

    if(i > 0) addPage();

    page = ui->tabWidget->widget(i)->findChild<QDesignPage *>();

    page->Load(report);
}

ui->tabWidget->setCurrentIndex(0);

updateVariables();

m_Changing  = false;
m_notSaveXml = false;
ui->actionSave->setEnabled(m_Changing);
}

void QASRepDesigner::setWindowCaption(QString textfile)
{
 setWindowTitle("Report Designer (" + textfile +  ")");
}

void QASRepDesigner::loadFromFile(QString filename)
{
if(!QFile::exists(filename)) return;

QFile file(filename);

if (!file.open(QIODevice::ReadOnly))
    return;

if (!m_xmlDoc->setContent(&file)) {
    file.close();
    return;
}

file.close();

loadReport();

m_fileName = filename;
m_typeLoad = tlFile;

setWindowCaption(filename);
}

void QASRepDesigner::loadFromStream(QString xmltext)
{
if(!m_xmlDoc->setContent(xmltext)) return;

loadReport();

m_typeLoad = tlStream;

setWindowCaption(m_Report->reportName());
}

void QASRepDesigner::loadFromStream(QIODevice * dev)
{
    if(!m_xmlDoc->setContent(dev)) return;

    loadReport();

    m_typeLoad = tlStream;

    setWindowCaption(m_Report->reportName());
}

void QASRepDesigner::loadFromStream(QByteArray & buffer)
{
    if(!m_xmlDoc->setContent(buffer)) return;

    loadReport();

    m_typeLoad = tlStream;

    setWindowCaption(m_Report->reportName());
}

void QASRepDesigner::on_actionOpen_triggered()
{
if(m_Changing)
{
    int n = QMessageBox::question(0,
    tr("Message"),
    tr("The report has changed"
    "\n Do you want to save the changes?"),
    QMessageBox::Yes | QMessageBox::No,
    QMessageBox::Yes
    );

if (n == QMessageBox::Yes)
    on_actionSave_triggered();
}

QString filereport = QFileDialog::getOpenFileName(this, tr("Open file Dialog"), "", tr("Report file (*" EXTREPFILE ")"));
if(filereport.isEmpty()) return;

clearAll();

loadFromFile(filereport);
}

void QASRepDesigner::on_actionSave_as_triggered()
{
   if(m_Changing)SaveReport();

    QString filereport = QFileDialog::getSaveFileName(this, tr("Save Dialog"), "", "Report file (*" EXTREPFILE ")");
    if(filereport.isEmpty()) return;

    saveToFile(filereport);
}


void QASRepDesigner::on_actionPreview_triggered()
{
//if(m_Changing)
//       on_actionSave_triggered();

    if(m_notSaveXml)
             SaveReport();

if(!m_Report) return;

bool load = m_Report->load(m_xmlDoc->toString());

 if(load)
       m_Report->reportView();

}

void QASRepDesigner::on_actionAdd_page_triggered()
{
int index;

emit m_ActivePage->signalCloseProps();

addPage();

index = ui->tabWidget->count() - 1;

ui->tabWidget->setCurrentIndex(index);

selectPage(index);
}

void QASRepDesigner::on_actionDelete_page_triggered()
{
QDesignPage * page;

 if(ui->tabWidget->count() == 1) return;

  deletePage(ui->tabWidget->currentIndex());
  selectPage(ui->tabWidget->currentIndex());

  slotChanging();

  for (int i = 0; i < ui->tabWidget->count(); ++i) {
    page  =  ui->tabWidget->widget(i)->findChild<QDesignPage *>();
    page->SetIndex(i + 1);

    ui->tabWidget->setTabText(i, "Page " + QString::number(i + 1));
  }

}

void QASRepDesigner::on_actionPage_parametres_triggered()
{
QDesPageProps pageProps(this);

pageProps.SetOrientation(m_ActivePage->Orientation());
pageProps.SetPapeSize(m_ActivePage->PapeSize());
pageProps.SetMargLeft(m_ActivePage->MarginLeft());
pageProps.SetMargRight(m_ActivePage->MarginRight());
pageProps.SetMargTop(m_ActivePage->MarginTop());
pageProps.SetMargBottom(m_ActivePage->MarginBotom());
pageProps.SetWidthMM(m_ActivePage->SizePageMM().width());
pageProps.SetHeightMM(m_ActivePage->SizePageMM().height());

pageProps.exec();

if(pageProps.result() == QDialog::Accepted)
{
 QDesignPage *page;

 for (int i = 0; i < ui->tabWidget->count(); ++i) {

    page = ui->tabWidget->widget(i)->findChild<QDesignPage *>();

    page->SetOrientation(pageProps.Orientation());
    page->SetMarginLeft(pageProps.margLeft());
    page->SetMarginRight(pageProps.margRight());
    page->SetMarginTop(pageProps.margTop());
    page->SetMarginBotom(pageProps.margBottom());

    if(pageProps.PapeSize() == QPrinter::Custom)
      page->SetSizeMM(QSize(pageProps.WidthMM(),pageProps.HeightMM()));

    page->SetPapeSize(pageProps.PapeSize());
}


    slotChanging();

}
}

void QASRepDesigner::clearCopyControls()
{
QCopyRec *copyRec;

while (m_copyControls.count()) {
    copyRec = m_copyControls.at(0);
    delete copyRec;
    m_copyControls.removeAt(0);
}
}

void QASRepDesigner::copySelControls()
{
QCopyRec *copyRec;
const QMetaObject* metaObject;
QMetaProperty property;

    foreach(QDesControl *control, m_ActivePage->SelectControls()){
        copyRec = new QCopyRec;
        copyRec->type = control->Type();

        metaObject = control->metaObject();
        for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i){
            property = metaObject->property(i);

            if(!strcmp(property.name(), "Name"))continue;

            copyRec->propValues[property.name()] = property.read(control);
        }

        m_copyControls.append(copyRec);
     }
}

void QASRepDesigner::on_actionCopy_triggered()
{
clearCopyControls();
copySelControls();
}

void QASRepDesigner::on_actionCut_triggered()
{
clearCopyControls();
copySelControls();

m_ActivePage->DeleteSel();
}

void QASRepDesigner::on_actionPast_triggered()
{
m_ActivePage->PastControls(m_copyControls);
}

void QASRepDesigner::on_actionSettings_triggered()
{
QTypeUnit type = FormSettings->typeRuleMeasure();

FormSettings->exec();

if(FormSettings->result() == QDialog::Accepted)
{
if(FormSettings->typeRuleMeasure() != type)
        emit FormSettings->signalUpdateRules();

emit FormSettings->signalShowBandHeaders();
}
}

void QASRepDesigner::on_actionList_of_variables_triggered()
{
QDesVariables variables(this);

variables.exec();
}

void QASRepDesigner::on_actionStandart_triggered()
{
QAction *action = (QAction*)QObject::sender();
QToolBar *toolbar;

if(action == ui->actionStandart)
    toolbar = ui->toolBarMain;

if(action == ui->actionText_2)
    toolbar = ui->toolBarFont;

if(action == ui->actionBorders)
    toolbar = ui->toolBarBorder;

if(action == ui->actionObjects)
    toolbar = ui->toolBarControls;

if(action == ui->actionAlignment)
    toolbar =  ui->toolBarAlign;


toolbar->setVisible(action->isChecked());

}

void QASRepDesigner::on_actionAbout_triggered()
{
    QString about = "<font size='+2' color='#0000FF'><strong><CENTER>ASReport<br>designer</CENTER></strong></font>"
                    "<br>ver. " + QString(VERSION) + ""
                    "<br>Programming: <i>Sergey Zgukov</i>"
                    "<br><br>email: <a href='mailto:serg_@ukr.net'>serg_@ukr.net</a>"
                    "<font size='-1'><br><br>Copyright ©2015-"+ QDate::currentDate().toString("yyyy") + " Sergey Zgukov"
                                                                                                        "<br>All rights reserved</font>";

    QMessageBox::about(this,"About",about);


}
