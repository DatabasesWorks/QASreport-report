/********************************************
*         QASReport ver.1.01                *
*           Report class                    *
*                                           *
*  Copyright (c) 2015-2016  Sergey Zgukov   *
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


#include "despropinspector.h"


//---------------------------------------------------------------------------
DesPropDelegate::DesPropDelegate(QObject *parent):QItemDelegate(parent)
{
    m_propInspector = (DesPropInspector*)parent;

}

void DesPropDelegate::paint(QPainter *painter,
           const QStyleOptionViewItem &option,
           const QModelIndex &index)const
{
 QPen pen;
 QRect rect = option.rect;
 int x;

 pen.setColor(QColor(200,200,200));
 pen.setStyle(Qt::SolidLine);
 pen.setWidthF(0.5);

 painter->setPen(pen);

 x = index.column() == 0?0:rect.left();

 painter->drawLine(x,rect.bottom(),rect.right(),rect.bottom());

 painter->drawLine(rect.right(),rect.top(),rect.right(),rect.bottom());

//--
if(index.column() == 1 && index.data().type() == QVariant::Bool)
     drawCheck(painter, option, option.rect, index.data().toBool() ? Qt::Checked : Qt::Unchecked);
else
if(!m_propInspector->paintDelegate(painter,option,index))
                QItemDelegate::paint(painter,option,index);
}

QWidget *DesPropDelegate::createEditor(
            QWidget *parent,
            const QStyleOptionViewItem &option,
            const QModelIndex &index)const
{
if(index.column() == 0)
                    return Q_NULLPTR;

QStandardItemModel * model = (QStandardItemModel *)index.model();
QString propname, parentname = "";
QStandardItem *item, *propitem;
QWidget *editor = Q_NULLPTR;

item = model->itemFromIndex(index);

if(item->parent()){
    propitem = item->parent()->child(item->row(),0);
    parentname = item->parent()->text();
}
else
    propitem = model->item(index.row());

propname = propitem->text();

if(propname == "Alignment") return Q_NULLPTR;
if(propname == "Border") return Q_NULLPTR;

if(propname == "Font")
{
    QPropButton *  button = new QPropButton(parent);
    button->setValue(index.data());
    connect(button,SIGNAL(signalClicked()),m_propInspector,SLOT(slotChangeFont()));

    editor =  button;
}

if(propname == "Text" || propname == "Picture")
{
    QPropButton *  button = new QPropButton(parent);
    connect(button,SIGNAL(signalClicked()),m_propInspector,SLOT(slotCallEditor()));

    editor =  button;
}

if(propname == "Format")
{
    QPropButton *  button = new QPropButton(parent);
    connect(button,SIGNAL(signalClicked()),m_propInspector,SLOT(slotCallFormatEditor()));

    editor =  button;
}

if(m_propInspector->isEnumType(propname,parentname))
{
    QMetaEnum enumer = m_propInspector->getEnum(propname,parentname);
    QComboBox* enumcombo = new QComboBox(parent);

    for (int i = 0; i < enumer.keyCount(); ++i)
        enumcombo->addItem(enumer.valueToKey(enumer.value(i)));

    enumcombo->setCurrentText(index.data().toString());

    editor =  enumcombo;
}

if(propname == "FontFamily")
{
    QFontDatabase fontdata;
    QComboBox* fontlist = new QComboBox(parent);
    fontlist->addItems(fontdata.families());
    fontlist->setCurrentText(index.data().toString());

    editor =  fontlist;
}

if(propname == "Source")
{
    QComboBox* tablelist = new QComboBox(parent);
    tablelist->setEditable(true);
    tablelist->setModel(FormSettings->Tables());

    editor =  tablelist;
}

if(propname == "BarcodeAngle")
{
QStringList list = QString(BARCODEANGLES).split(',');
QComboBox* anglecombo = new QComboBox(parent);
anglecombo->setEditable(false);

for (int i = 0; i < list.count(); ++i)
    anglecombo->addItem(list.at(i));

editor =   anglecombo;
}


if(index.data().type() == QVariant::Bool)
{
    QPropCheckBox * checkbox = new  QPropCheckBox(parent);
    checkbox->setChecked(index.data().toBool());

    editor =  checkbox;
}

if(propitem->data(Qt::UserRole + 2).toInt() == QVariant::Color)
{
    QColorComboBox *colorcombo = new  QColorComboBox(parent);
    colorcombo->SetColor(QColor(index.data().toString()));

    editor =  colorcombo;
}

if(editor == Q_NULLPTR)
editor = QItemDelegate::createEditor(parent,option,index);


if(editor->inherits("QSpinBox"))
        connect((QSpinBox*)editor,SIGNAL(valueChanged(QString)),m_propInspector,SLOT(slotValueChanged(QString)));

if(editor->inherits("QLineEdit"))
        connect((QLineEdit*)editor,SIGNAL(textEdited(QString)),m_propInspector,SLOT(slotValueChanged(QString)));

if(editor->inherits("QComboBox"))
        connect((QComboBox*)editor,SIGNAL(currentIndexChanged(QString)),m_propInspector,SLOT(slotValueChanged(QString)));

if(editor->inherits("QCheckBox"))
        connect((QCheckBox*)editor,SIGNAL(stateChanged(int)),m_propInspector,SLOT(slotStateChanged(int)));

if(editor->inherits("QPropCheckBox"))
        connect((QPropCheckBox*)editor,SIGNAL(stateChanged(int)),m_propInspector,SLOT(slotStateChanged(int)));

if(editor->inherits("QColorComboBox"))
        connect((QColorComboBox*)editor,SIGNAL(stateChanged(int)),m_propInspector,SLOT(slotStateChanged(int)));

return editor;
}


void  DesPropDelegate::setModelData(
        QWidget *editor,
        QAbstractItemModel *model,
        const QModelIndex& index)const
{
QVariant value;

if(editor->inherits("QSpinBox"))
    value = static_cast<QSpinBox *>(editor)->value();

if(editor->inherits("QLineEdit"))
    value = static_cast<QLineEdit *>(editor)->text();

if(editor->inherits("QPropCheckBox"))
    value = static_cast<QPropCheckBox *>(editor)->isChecked();

if(editor->inherits("QColorComboBox"))
    value = static_cast<QColorComboBox *>(editor)->Color();

if(editor->inherits("QComboBox"))
    value = static_cast<QComboBox *>(editor)->currentText();

if(editor->inherits("QPropButton")){
    QPropButton *button = static_cast<QPropButton *>(editor);

    if(!button->change()) return;

    value.setValue(button->Value());

    if(button->Value().typeName() == QString("QDesFont").toLatin1())
    {
        QDesFont font = button->Value().value<QDesFont>();
        value.setValue(font);
        m_propInspector->updateObjectPropItems(&font,static_cast<QStandardItemModel*>(model)->item(index.row()));
    }//else return;
}

model->setData(index, value);
}


QSize DesPropDelegate::sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
  return QSize(250,20);
}

void DesPropDelegate::updateEditorGeometry(QWidget *editor,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

//-----------------------------------------------------------------------
DesPropInspector::DesPropInspector(QObject* parent):QObject(parent)
{
m_treeView = 0;

m_model = new QStandardItemModel(this);
m_model->setColumnCount(2);
m_model->setHeaderData(0,Qt::Horizontal,tr("Property"));
m_model->setHeaderData(1,Qt::Horizontal,tr("Value"));
connect(m_model,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(slotItemChanged(QStandardItem*)));


m_canCnange = false;
}

void DesPropInspector::slotChangeFont()
{
QPropButton *button = (QPropButton *)QObject::sender();
QFont font;
QDesFont desfont = button->Value().value<QDesFont>();
bool ok;
QVariant value;

font.setFamily(desfont.fontFamily());
font.setPointSize(desfont.fontSize());
font.setBold(desfont.fontBold());
font.setItalic(desfont.fontItalic());
font.setUnderline(desfont.fontUnderline());

font = QFontDialog::getFont(&ok,font);

if(!ok) return;

desfont.SetFontFamily(font.family());
desfont.SetFontSize(font.pointSize());
desfont.SetFontBold(font.bold());
desfont.SetFontItalic(font.italic());
desfont.SetFontUnderline(font.underline());

value.setValue(desfont);
button->setValue(value);

emit m_desPropDelegate->commitData(button);
}

void DesPropInspector::slotCallEditor()
{
QDesControl *control;

if(m_selList.count() > 1) return;

control = m_selList.at(0);

control->CallEditor();
}

void DesPropInspector::slotCallFormatEditor()
{
QDesControl *control;
int index;
const QMetaObject *metaObject;
QPropButton *button = (QPropButton *)QObject::sender();

if(m_selList.count() > 1) return;

control = m_selList.at(0);

QDesSelFormat seldialog((QMainWindow*)this->parent());

metaObject = control->metaObject();

index = metaObject->indexOfProperty("Format");

if(index == -1) return;

metaObject->property(index).read(control);

seldialog.SetFormat(metaObject->property(index).read(control).toString());

seldialog.exec();

if(seldialog.result() == QDialog::Accepted){
    metaObject->property(index).write(control,seldialog.Format());

   button->setValue(seldialog.Format());

   emit m_desPropDelegate->commitData(button);
}

}

bool DesPropInspector::paintDelegate(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index)
{
QString propname;
QStandardItem *item, *propitem;
QPen pen;
QBrush brush;
QRect r;

if(index.column() == 1)
{
     item = m_model->itemFromIndex(index);

    if(item->parent())
        propitem = item->parent()->child(item->row(),0);
    else
        propitem = m_model->item(index.row());

    propname = propitem->text();

    if(propname == "Text" || propname == "Picture")
      return true;

    if(propitem->data(Qt::UserRole + 2).toInt() == QVariant::Color)
    {
    r = option.rect;

    r.setLeft(r.left() + 4);
    r.setRight(r.right() -4);
    r.setTop(r.top() + 4);
    r.setBottom(r.bottom() - 5);

    pen.setColor(Qt::gray);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    brush.setColor(QColor(index.data().toString()));
    brush.setStyle(Qt::SolidPattern);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(r);

    return true;
    }
    return false;
}

return false;
}


QMetaProperty DesPropInspector::seekProperty(QObject*control, QString propname)
{
const QMetaObject* metaObject;
QMetaProperty prop;
int index;

metaObject = control->metaObject();

index = metaObject->indexOfProperty(propname.toLocal8Bit().data());

if(index != -1)
prop = metaObject->property(index);

return prop;
}

QMetaProperty DesPropInspector::getProperty(QString propname,QString parentname)
{
QMetaProperty prop, parentprop;
QObject *control = m_selList.at(0);

if(parentname.isEmpty())
    prop = seekProperty(control,propname);
else
{
 parentprop =  seekProperty(control,parentname);

 if(parentname == "Font")
 {
  QDesFont font =  parentprop.read(control).value<QDesFont>();
  prop = seekProperty(&font, propname);
 }

  if(parentname == "Alignment")
  {
   QDesAlignment alignment =  parentprop.read(control).value<QDesAlignment>();
   prop = seekProperty(&alignment, propname);
  }
}

return prop;
}

bool DesPropInspector::isEnumType(QString propname,QString parentname)
{
QMetaProperty prop = getProperty(propname,parentname);

return prop.isEnumType() && !prop.isFlagType();
}

QMetaEnum DesPropInspector::getEnum(QString propname,QString parentname)
{
QMetaEnum enumer;
QMetaProperty prop = getProperty(propname,parentname);

if(prop.isEnumType() && !prop.isFlagType())
        enumer = prop.enumerator();

return   enumer;
}

void DesPropInspector::SetTreeView(QTreeView *tree)
{
m_treeView = tree;
m_treeView->setModel(m_model);

m_treeView->setColumnWidth(0, 110);
m_treeView->setColumnWidth(1, 40);

m_desPropDelegate = new DesPropDelegate(this);

m_treeView->setItemDelegate(m_desPropDelegate);

//m_treeView->setItemDelegate(new DesPropDelegate(this));

}

void DesPropInspector::updateObjectPropItems(QObject *objectprop,QStandardItem *parentitem)
{
const QMetaObject* metaObject = objectprop->metaObject();
QStandardItem *item, *itemval;
int index;
QMetaProperty prop;

m_canCnange = false;

for (int i = 0; i < parentitem->rowCount(); ++i){
    item = parentitem->child(i);
    itemval = parentitem->child(i,1);

    index =  metaObject->indexOfProperty(item->text().toLatin1());

    if(index == -1) continue;

    prop = metaObject->property(index);

    itemval->setData(prop.read(objectprop),Qt::DisplayRole);
}

m_canCnange = true;
}

void DesPropInspector::UpdatePropValue(QObject* control,QString propname,QVariant value,QStandardItem * parentitem)
{
int index, numvalue, keyval;
QMetaProperty prop;
const QMetaObject* metaObject = control->metaObject();
QMetaEnum enumer;
QModelIndex indprop;

if(parentitem)
index = metaObject->indexOfProperty(parentitem->text().toLocal8Bit().data());
else
index = metaObject->indexOfProperty(propname.toLocal8Bit().data());

if(index == -1) return;

prop = metaObject->property(index);

if(prop.type() == QVariant::UserType){
    if(prop.typeName() == QString("QDesFont").toLatin1() && parentitem)
    {
        QDesFont font =  prop.read(control).value<QDesFont>();
        UpdatePropValue(&font,propname,value);

        value.setValue(font);
    }

    if(prop.typeName() == QString("QDesAlignment").toLatin1())
    {
        QDesAlignment alignment = prop.read(control).value<QDesAlignment>();
        UpdatePropValue(&alignment,propname,value);

        value.setValue(alignment);
    }
}

if(prop.isEnumType())
{
    enumer = prop.enumerator();

    if(prop.enumerator().isFlag())
    {
        numvalue = prop.read(control).toInt();

        keyval = enumer.keyToValue(propname.toLatin1());

        numvalue = value.toBool()?(numvalue | keyval):(numvalue ^ keyval);

        value = numvalue;

        if(parentitem){
            m_canCnange = false;
            indprop = m_model->index(parentitem->row(),1);
            m_model->setData(indprop, enumer.valueToKeys(value.toInt()));
            m_canCnange = true;
        }else
            value =  enumer.keyToValue(value.toString().toLatin1());
    }
}

prop.write(control, value);
}

void DesPropInspector::slotItemChanged(QStandardItem * item)
{
if(!m_canCnange) return;
if(!m_selList.count()) return;

QModelIndex indprop;
QString propname;
QVariant value;
QStandardItem * parentitem = 0;

value = item->data(Qt::EditRole);
if(value.isNull()) return;

parentitem = item->parent();

if(parentitem)
    propname = parentitem->child(item->row(),0)->text();
else
{
    indprop = m_model->index(item->row(), 0);
    propname = indprop.data().toString();
}

if(m_selList.count() > 1 && propname == "Name")
                                return;

foreach (QObject* control, m_selList){
  UpdatePropValue(control,propname,value,parentitem);

if(control->inherits("QWidget"))
((QWidget*)control)->update();
}

emit signalReadToolBarValues();
}

QVariant DesPropInspector::ExpectValue(QString propname,QVariant value)
{
const QMetaObject* metaObject;
QVariant curval, retvalue = value;
QMetaProperty prop;
int index;

foreach (QObject*control, m_selList){
    if(control == m_selList.at(0)) continue;

    metaObject  = control->metaObject();

    index = control->metaObject()->indexOfProperty(propname.toLatin1());
    if(index == -1) continue;

    prop = metaObject->property(index);

    curval = prop.read(control);    

    if(curval != value){
        if(prop.isEnumType() && prop.isFlagType())
            retvalue =  retvalue.toInt() &  curval.toInt();
        else
            retvalue.clear();
        break;
    }
}

return retvalue;
}

void DesPropInspector::UpdatePropsValues(QStandardItem *parentitem,QObject*control, bool expprop)
{
const QMetaObject* metaObject;
QString propname, keyname;
QMetaProperty prop;
QMetaEnum enumer;
QVariant value;
QStandardItem *item;
int keyvalue, index, count;
QModelIndex ind;

if(!m_selList.count()) return;

metaObject = control->metaObject();

count = parentitem?parentitem->rowCount():m_model->rowCount();

for(int  r = 0; r < count; ++r){
    if(parentitem)
        item = parentitem->child(r);
    else
        item = m_model->item(r);

    propname = item->text();

    if(m_selList.count() > 1 && propname == "Name")
        continue;

        index = metaObject->indexOfProperty(propname.toLocal8Bit().data());

        if(index == -1) continue;

        prop = metaObject->property(index);

        value = prop.read(control);

        if(prop.type() == QVariant::UserType){
            if(prop.typeName() ==  QString("QDesFont").toLatin1())
            {
                QDesFont font =  value.value<QDesFont>();
                UpdatePropsValues(item,&font,false);
            }

            if(prop.typeName() == QString("QDesAlignment").toLatin1())
            {
                QDesAlignment desa = value.value<QDesAlignment>();
                UpdatePropsValues(item,&desa,false);
            }
        }

        if(prop.type() !=  QVariant::Bool && prop.type() != QVariant::UserType && expprop)
            value = ExpectValue(propname, value);

        if(prop.isEnumType())
        {
            enumer = prop.enumerator();

            if(enumer.isFlag()){
                for (int k = 0; k < enumer.keyCount(); ++k){
                    keyname = enumer.key(k);
                    keyvalue = enumer.value(k);

                    //child = item->child(k);

                    ind = m_model->index(k,1,item->index());
                    m_model->setData(ind,value.toInt() & keyvalue?true:false,Qt::DisplayRole);
                    m_model->setData(ind,keyvalue,Qt::UserRole + 2);
                }

                value =  enumer.valueToKeys(value.toInt());
            }else
                value = enumer.valueToKey(value.toInt());
        }

        if(!parentitem)
        {
             ind = m_model->index(item->row(),1);
             m_model->setData(ind,value,Qt::DisplayRole);
        }else
        {
            ind = m_model->index(item->row(),1,parentitem->index());
            m_model->setData(ind,value,Qt::DisplayRole);
        }
}
}

bool DesPropInspector::PropExistInSel(QString propname)
{
foreach (QObject*control, m_selList) {
    if(control == m_selList.at(0)) continue;

    if(control->metaObject()->indexOfProperty(propname.toLatin1()) == -1)
        return false;
}

 return true;
}


void DesPropInspector::UpdatePropsList(QStandardItem *parentitem,QObject*control, bool expprop)
{
const QMetaObject* metaObject;
QString propname, keyname;
QMetaProperty prop;
QMetaEnum enumer;
int  row = 0; //keyvalue,
QStandardItem *item, *child;
QStringList subprops;

if(!control) return;

if(control->inherits("QDesControl"))
    subprops = ((QDesControl*)control)->subProps().split(',');

metaObject  = control->metaObject();

for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
{
    prop = metaObject->property(i);

    propname = QString::fromLatin1(prop.name());

    if(subprops.indexOf(propname) != -1) continue;

    if(expprop && !PropExistInSel(propname))continue;

    item = new QStandardItem(propname);
    item->setData((int)prop.type(),Qt::UserRole + 2);

    if(!parentitem)
        m_model->setItem(row,0,item);
    else{
        parentitem->setColumnCount(2);
        parentitem->setChild(row,0,item);
    }

    if(prop.type() == QVariant::UserType){
        if(prop.typeName() ==  QString("QDesFont").toLatin1())
        {
            QDesFont font =  prop.read(control).value<QDesFont>();
            UpdatePropsList(item,&font,false);
        }

        if(prop.typeName() == QString("QDesAlignment").toLatin1())
        {
            QDesAlignment desa = prop.read(control).value<QDesAlignment>();
            UpdatePropsList(item,&desa,false);
        }
    }

    if(prop.isEnumType())
    {
        enumer = prop.enumerator();

        if(enumer.isFlag()){
            item->setColumnCount(2);

            for (int k = 0; k < enumer.keyCount(); ++k){
                keyname = enumer.key(k);
                //keyvalue = enumer.value(k);

                child = new QStandardItem(keyname);

                item->appendRow(child);
            }
        }
    }

    ++row;
}
}

void DesPropInspector::slotReadProps(QList<QDesControl*>list)
{
    if(!list.count()){
        if(m_selList.count())
        {
            m_selList.clear();
            clearModel();
        }
        return;
    }

m_canCnange = false;

m_selList.clear();
m_selList.append(list);

clearModel();

UpdatePropsList(0,m_selList.at(0));
UpdatePropsValues(0,m_selList.at(0));

m_canCnange = true;
}

void DesPropInspector::slotUpdatePropValues()
{
m_canCnange = false;

UpdatePropsValues(0,m_selList.at(0));

m_canCnange = true;
}

void DesPropInspector::clearModel()
{
QStandardItem *item;
bool res = false;
QModelIndex ind;

if(!m_model->rowCount()) return;

    ind = m_model->index(0,0);
    m_treeView->setCurrentIndex(ind);

while (m_model->rowCount()){
    item = m_model->item(0);

    if(item->hasChildren()){
        res = m_model->removeRows(0,item->rowCount(),item->index());
    }

    res = m_model->removeRow(0);
}
}

void DesPropInspector::slotCloseProps()
{
m_selList.clear();

m_canCnange = false;

clearModel();

m_canCnange = true;

}

void DesPropInspector::slotValueChanged(const QString &text)
{
   emit m_desPropDelegate->commitData((QWidget *)sender());
}

void DesPropInspector::slotStateChanged(int state)
{
   emit m_desPropDelegate->commitData((QWidget *)sender());
}




