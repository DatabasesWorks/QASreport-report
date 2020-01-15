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

#include "desdialogs.h"


DialogSelBand::DialogSelBand(QWidget* parent):QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
QRadioButton *m_button;
QString name;
QGridLayout *gridlayout;
QVBoxLayout *layoutgen;
QHBoxLayout * layoutbut;
QPushButton *cancelbut, *okbut;
QString bandNames[] = {"Report Title", "Report Summary",
                       "Page Header", "Page Footer",
                       "Master Header", "Master Data", "Master Footer",
                       "Detail Header", "Detail Data", "Detail Footer",
                        "Overlay","Column Header", "Column Footer",
                       "Group Header", "Group Footer"};

m_groupbox = new QGroupBox("Band types",this);
gridlayout = new QGridLayout;
layoutgen = new QVBoxLayout();
layoutbut = new QHBoxLayout();

m_countband = 15;

int r = 0,c = 0;

for (int i = 0; i < m_countband; ++i,++r){
    name = bandNames[i];

    m_button = new QRadioButton(name,m_groupbox);

    if(i == 8){r = 0; c = 1;}

    gridlayout->addWidget(m_button,r,c);
}

((QRadioButton*)m_groupbox->children().at(0))->setChecked(true);

m_groupbox->setLayout(gridlayout);

layoutgen->addWidget(m_groupbox);

cancelbut = new QPushButton("Cancel");
okbut = new QPushButton("OK");

connect(okbut, SIGNAL(clicked()), SLOT(accept()));
connect(cancelbut, SIGNAL(clicked()), SLOT(reject()));

layoutbut->addWidget(cancelbut);
layoutbut->addWidget(okbut);

layoutgen->addLayout(layoutbut);

layoutgen->addStretch();

setLayout(layoutgen);

setWindowTitle("New band");

setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

int DialogSelBand::SelectedBand()
{
QObjectList list = m_groupbox->children();
QObject *child;
QRadioButton * button;
QString classname;

for (int i = 0; i < list.count(); ++i) {

    child = list.at(i);

    classname = child->metaObject()->className();

    if(classname !=  "QRadioButton") continue;

    button = (QRadioButton *)child;

    if(button->isChecked()){return i; break;}
}


return -1;
}

void DialogSelBand::UpdateEnabled(QList<int>list)
{
QRadioButton * button;
QObjectList objlist = m_groupbox->children();
int val;

for (int i = 0; i < list.count(); ++i){

    val = list.at(i);

    if(val > m_countband) continue;

    button = (QRadioButton *)objlist.at(val);

    button->setEnabled(false);
}

for (int i = 0; i < m_countband; ++i){
    button = (QRadioButton *)m_groupbox->children().at(i);

    if(!button->isEnabled()) continue;

    if(button->isEnabled()){button->setChecked(true); break;}

}
}

//----------------------------------------------------------------------------------------
QImageWidget::QImageWidget(QWidget* parent):QWidget(parent)
{
m_scaled = false;
m_drawrect = true;

setMinimumSize(QSize(100,100));
}


void QImageWidget::paintEvent(QPaintEvent *pe)
{
QPainter painter(this);

QPen pen;
pen.setColor(Qt::gray);
pen.setStyle(Qt::SolidLine);

QRect r = rect();
r.setRight(r.right() - 1);
r.setBottom(r.bottom() - 1);

painter.setPen(pen);

if(m_drawrect)
painter.drawRect(r);

if(m_scaled)
painter.drawPixmap(rect(),m_pixmap);
else
painter.drawPixmap(2, 2,m_pixmap);
}



//----------------------------------------------------------------------------------------
QDesImageEdit::QDesImageEdit(QWidget* parent):QDialog(parent)
{
    QBoxLayout *toplayout,*genlayout,  *botlayout, *sourcelayout;
    QPushButton *cancelbut, *okbut;
    QToolButton *loadbut, *clearbut, *butsource;
    QLabel *label;
    QWidget *toppanel;

    m_imageWidget = new QImageWidget(this);

    genlayout = new QVBoxLayout(this);

    toplayout = new QHBoxLayout(this);
    toplayout->setMargin(1);
    toplayout->setSpacing(1);

    toppanel = new QWidget(this);
    toppanel->setMinimumHeight(31);
    toppanel->setMaximumHeight(31);

    loadbut = new QToolButton(toppanel);
    loadbut->setText("Load");
    loadbut->setIcon(QIcon(":/new/designer/images/open.png"));
    loadbut->setToolTip(tr("Load image"));
    loadbut->setMinimumHeight(30);
    loadbut->setMinimumWidth(30);

    clearbut = new QToolButton(toppanel);
    clearbut->setText("Clear");
    clearbut->setIcon(QIcon(":/new/designer/images/delete.png"));
    clearbut->setToolTip(tr("Clear image"));
    clearbut->setMinimumHeight(30);
    clearbut->setMinimumWidth(30);

    m_butScale = new QToolButton(toppanel);
    m_butScale->setCheckable(true);
    m_butScale->setText("Scaled");
    m_butScale->setToolTip(tr("Scaled image"));
    m_butScale->setIcon(QIcon(":/new/preview/images/fit-page-24.png"));
    m_butScale->setMinimumHeight(30);
    m_butScale->setMinimumWidth(30);

    m_butCheckSource = new QToolButton(toppanel);
    m_butCheckSource->setCheckable(true);
    m_butCheckSource->setToolTip(tr("Select image source"));
    m_butCheckSource->setIcon(QIcon(":/new/designer/images/database_link.png"));
    m_butCheckSource->setMinimumHeight(30);
    m_butCheckSource->setMinimumWidth(30);

    connect(m_butCheckSource,SIGNAL(clicked(bool)),this,SLOT(slotButCheckSource()));

    sourcelayout = new QHBoxLayout(this);

    m_sourcewidget = new QWidget(this);
    m_sourcewidget->setMaximumHeight(31);
    m_sourcewidget->setVisible(false);

    label = new QLabel(tr("data field: "),m_sourcewidget);
    m_textsource = new QLineEdit(m_sourcewidget);

    butsource = new QToolButton(m_sourcewidget);
    butsource->setMaximumWidth(30);
    butsource->setToolTip(tr("Open list variables"));
    butsource->setIcon(QIcon(":/new/dialogs/images/variables.png"));
    butsource->setMinimumHeight(30);
    butsource->setMinimumWidth(30);

    connect(butsource,SIGNAL(clicked(bool)),this,SLOT(slotClickButSource()));

    sourcelayout->addWidget(label);
    sourcelayout->addWidget(m_textsource);
    sourcelayout->addWidget(butsource);
    sourcelayout->addStretch();
    sourcelayout->setMargin(2);
    sourcelayout->setSpacing(3);

    m_sourcewidget->setLayout(sourcelayout);

    connect(loadbut,SIGNAL(clicked()),this,SLOT(slotLoadImage()));
    connect(clearbut,SIGNAL(clicked()),this,SLOT(slotClearImage()));
    connect(m_butScale,SIGNAL(clicked()),this,SLOT(slotScaleImage()));

    toplayout->addWidget(loadbut);
    toplayout->addWidget(clearbut);
    toplayout->addWidget(m_butScale);
    toplayout->addWidget(m_butCheckSource);
    toplayout->addStretch();

    toppanel->setLayout(toplayout);

    genlayout->addWidget(toppanel);
    genlayout->addWidget(m_sourcewidget);
    genlayout->addWidget(m_imageWidget);

    cancelbut = new QPushButton("Cancel",this);
    okbut = new QPushButton("OK",this);

    connect(okbut, SIGNAL(clicked()), SLOT(accept()));
    connect(cancelbut, SIGNAL(clicked()), SLOT(reject()));

    botlayout = new QHBoxLayout(this);
    botlayout->addWidget(okbut);
    botlayout->addWidget(cancelbut);
    botlayout->insertStretch(0);

    genlayout->addLayout(botlayout);

    setLayout(genlayout);

    setWindowTitle(tr("Image edit"));

    QSettings settings(QDir::currentPath() + "/settings.ini",QSettings::IniFormat);
    settings.beginGroup("QDesImageEdit");
    resize(settings.value("size",QSize(560,370)).toSize());
    m_butScale->setChecked(settings.value("checkscale",false).toBool());
    settings.endGroup();
}

void QDesImageEdit::slotButCheckSource()
{
    m_sourcewidget->setVisible(m_butCheckSource->isChecked());
}

void QDesImageEdit::slotClickButSource()
{
    QDesSelTreeVariables treevairables(this,true);

    treevairables.SetModelFields(FormSettings->Fields());
    treevairables.SetModelVariables(FormSettings->Variables());

    treevairables.updateModels();

    connect(&treevairables,SIGNAL(signalSendValue(QString)),this,SLOT(slotSendValue(QString)));

    treevairables.exec();
}

void QDesImageEdit::slotSendValue(QString value)
{
   m_textsource->setText(value);
}

void QDesImageEdit::setSource(QString value)
{
    m_textsource->setText(value);

    m_butCheckSource->setChecked(!m_textsource->text().isEmpty());

    slotButCheckSource();
}

void QDesImageEdit::SetImage(QPixmap pix)
{
    m_imageWidget->SetImage(pix);

    slotScaleImage();
}

void QDesImageEdit::slotLoadImage()
{
QString fileimage = QFileDialog::getOpenFileName(0, tr("Open Dialog"), "", tr("Images (*.png *.jpg *.bmp *.gif *.xpm)"));

if(fileimage.isEmpty()) return;

QImage image;
image.load(fileimage);

m_imageWidget->SetImage(QPixmap::fromImage(image));
}

void QDesImageEdit::slotClearImage()
{
m_imageWidget->SetImage(QPixmap());

}

void QDesImageEdit::slotScaleImage()
{
  m_imageWidget->SetScaled(m_butScale->isChecked());

}

QDesImageEdit::~QDesImageEdit()
{
    QSettings settings(QDir::currentPath() + "/settings.ini",QSettings::IniFormat);
    settings.beginGroup("QDesImageEdit");

    settings.setValue("size",size());
    settings.setValue("checkscale",m_butScale->isChecked());

    settings.endGroup();

}

//---------------------------------------------------------------------
QDesFigureEdit::QDesFigureEdit(QWidget* parent):QDialog(parent)
{
QBoxLayout *genlayout, *butlayout;
QPushButton *cancelbut, *okbut;

genlayout = new QVBoxLayout(this);
butlayout = new QHBoxLayout(this);

cancelbut = new QPushButton(tr("Cancel"),this);
okbut = new QPushButton("OK",this);

connect(okbut, SIGNAL(clicked()), SLOT(accept()));
connect(cancelbut, SIGNAL(clicked()), SLOT(reject()));

butlayout->addWidget(okbut);
butlayout->addWidget(cancelbut);
butlayout->insertStretch(0);

m_listtypes = new QComboBox(this);
m_listtypes->addItem(tr("Square"));
m_listtypes->addItem(tr("Triangle"));
m_listtypes->addItem(tr("Ellipse"));
m_listtypes->addItem(tr("Rounded rect"));

genlayout->addWidget(m_listtypes);
genlayout->addLayout(butlayout);

setWindowTitle(tr("Edit"));

setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}


//------------------------------------------------------------------------
QDesPageProps::QDesPageProps(QWidget* parent):QDialog(parent)
{
QTabWidget * tabwidget = new QTabWidget(this);
QGroupBox * box;
QLayout* layout, *tablayout;
QStringListModel *model = new QStringListModel(this);
QWidget * page;

model->setStringList(QString(PAPESIZES).split(','));

m_papesizes = new QComboBox(this);
m_papesizes->setModel(model);
m_papesizes->setCurrentIndex(0);

connect(m_papesizes,SIGNAL(currentIndexChanged(int)),this,SLOT(slotPapesizesIndexChanged(int)));

tablayout = new  QHBoxLayout(this);

layout = new  QGridLayout(this);
box = new QGroupBox(tr("Size"),this);
box->setLayout(layout);

((QGridLayout*)layout)->addWidget(m_papesizes,0,0,0,2,Qt::AlignTop);

m_labelwidthmm = new QLabel(tr("Width, mm"));
m_widthmm = new QLineEdit(this);
((QGridLayout*)layout)->addWidget(m_labelwidthmm,1,0);
((QGridLayout*)layout)->addWidget(m_widthmm,1,1);
((QGridLayout*)layout)->setColumnStretch(1,2);
m_labelwidthmm->setEnabled(false);
m_widthmm->setEnabled(false);

m_labelheigthhmm = new QLabel(tr("Height, mm"));
m_heighthmm = new QLineEdit;
((QGridLayout*)layout)->addWidget(m_labelheigthhmm,2,0);
((QGridLayout*)layout)->addWidget(m_heighthmm,2,1);
m_labelheigthhmm->setEnabled(false);
m_heighthmm->setEnabled(false);

((QHBoxLayout*)tablayout)->addWidget(box);

layout = new  QGridLayout(this);
box = new QGroupBox(tr("Orientation"),this);
box->setLayout(layout);

m_portrait = new QRadioButton(tr("Portrait"));
m_landscape = new QRadioButton(tr("Landscape"));

connect(m_portrait,SIGNAL(clicked()),this,SLOT(slotClickedOrientation()));
connect(m_landscape,SIGNAL(clicked()),this,SLOT(slotClickedOrientation()));

m_portrait->setChecked(true);

((QGridLayout*)layout)->addWidget(m_portrait,0,0);
((QGridLayout*)layout)->addWidget(m_landscape,1,0);

m_image = new QImageWidget(this);
m_image->setMinimumSize(32,32);
m_image->resize(32,32);
m_image->SetDrawrect(false);
m_image->SetImage(QPixmap(":/new/dialogs/images/layout-portrait-32.png"));
((QGridLayout*)layout)->addWidget(m_image,0,1);

((QHBoxLayout*)tablayout)->addWidget(box);

page = new QWidget(this);
page->setLayout(tablayout);

tabwidget->clear();
tabwidget->addTab(page,tr("Paper"));


tablayout = new  QHBoxLayout(this);
layout = new  QGridLayout(this);
box = new QGroupBox(tr("Margins"),this);
box->setLayout(layout);

m_margleft = new QLineEdit;
m_margright = new QLineEdit;
m_margtop = new QLineEdit;
m_margbottom = new QLineEdit;
m_margleft->setText("0");
m_margright->setText("0");
m_margtop->setText("0");
m_margbottom->setText("0");

((QGridLayout*)layout)->addWidget(new QLabel(tr("Left, mm")),0,0);
((QGridLayout*)layout)->addWidget(m_margleft,0,1);
((QGridLayout*)layout)->addWidget(new QLabel(tr("Right, mm")),0,2);
((QGridLayout*)layout)->addWidget(m_margright,0,3);
((QGridLayout*)layout)->addWidget(new QLabel(tr("Top, mm")),1,0);
((QGridLayout*)layout)->addWidget(m_margtop,1,1);
((QGridLayout*)layout)->addWidget(new QLabel(tr("Bottom, mm")),1,2);
((QGridLayout*)layout)->addWidget(m_margbottom,1,3);

tablayout->addWidget(box);

page = new QWidget(this);
page->setLayout(tablayout);

tabwidget->addTab(page,tr("Margins"));
tabwidget->setCurrentIndex(0);

QDialogButtonBox *butbox = new QDialogButtonBox(QDialogButtonBox::Ok| QDialogButtonBox::Cancel,this);

connect(butbox, SIGNAL(accepted()), SLOT(accept()));
connect(butbox, SIGNAL(rejected()), SLOT(reject()));

layout = new QVBoxLayout;
layout->addWidget(tabwidget);
layout->addWidget(butbox);
layout->setMargin(4);

setLayout(layout);

resize(250,200);

setWindowTitle(tr("Page properties"));
setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

void QDesPageProps::slotPapesizesIndexChanged(int index)
{
m_papeSize = (QPrinter::PaperSize)index;

m_labelwidthmm->setEnabled(m_papeSize == QPrinter::Custom);
m_labelheigthhmm->setEnabled(m_papeSize == QPrinter::Custom);
m_widthmm->setEnabled(m_papeSize == QPrinter::Custom);
m_heighthmm->setEnabled(m_papeSize == QPrinter::Custom);
}

void QDesPageProps::updateImage()
{
if(m_orientation == QPrinter::Portrait)
m_image->SetImage(QPixmap(":/new/dialogs/images/layout-portrait-32.png"));
else
m_image->SetImage(QPixmap(":/new/dialogs/images/layout-landscape-32.png"));

}

void QDesPageProps::slotClickedOrientation()
{
m_orientation = m_portrait->isChecked()?QPrinter::Portrait:QPrinter::Landscape;

updateImage();
}

//--------------------------------------------------------------------------------------
QDesSelDataSource::QDesSelDataSource(QWidget* parent):QDialog(parent)
{
QLayout* vlayout, *hlayout;
QLabel * label;
QDialogButtonBox * buttons;

vlayout = new QVBoxLayout(this);
hlayout = new QHBoxLayout(this);

buttons = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, this);

connect(buttons, SIGNAL(accepted()), SLOT(accept()));
connect(buttons, SIGNAL(rejected()), SLOT(reject()));

m_tables = new QComboBox(this);
m_tables->setEditable(true);

label = new QLabel(tr("Data source"));

hlayout->addWidget(label);
hlayout->addWidget(m_tables);

((QVBoxLayout*)vlayout)->addLayout(hlayout);
((QVBoxLayout*)vlayout)->addWidget(buttons);

setLayout(vlayout);

setWindowTitle(tr("Data source"));

setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}


//--------------------------------------------------------------------------
QDesSelDataField::QDesSelDataField(QWidget* parent):QDialog(parent)
{
QLayout* vlayout, *hlayout;
QLabel * label;
QPushButton * selbut;
QDialogButtonBox *buttons;

m_varenabled = true;

vlayout = new QVBoxLayout(this);
hlayout = new QHBoxLayout(this);

buttons = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, this);

connect(buttons, SIGNAL(accepted()), SLOT(accept()));
connect(buttons, SIGNAL(rejected()), SLOT(reject()));

m_groupfield = new QLineEdit(this);
selbut = new QPushButton("...",this);
label = new QLabel(tr("Group condition"));

hlayout->setMargin(1);
selbut->setMaximumWidth(22);

connect(selbut,SIGNAL(clicked()),this,SLOT(slotSelectField()));

hlayout->addWidget(label);
hlayout->addWidget(m_groupfield);
hlayout->addWidget(selbut);

((QVBoxLayout*)vlayout)->addLayout(hlayout);
vlayout->addWidget(buttons);

setLayout(vlayout);

setMinimumWidth(320);

setWindowTitle(tr("Condition"));

setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

}

void QDesSelDataField::slotSelectField()
{
  QDesSelTreeVariables selfield(this,m_varenabled);

  selfield.SetModelFields(FormSettings->Fields());

  if(m_varenabled)
    selfield.SetModelVariables(FormSettings->Variables());

     selfield.updateModels();

  connect(&selfield,SIGNAL(signalSendValue(QString)),this,SLOT(slotSendValue(QString)));

  selfield.exec();
}

void QDesSelDataField::slotSendValue(QString value)
{
m_groupfield->setText(value);

}

//------------------------------------------------------------------------
QDesSelTreeField::QDesSelTreeField(QWidget* parent):QDialog(parent)
{
QLayout *vlayout;
QLabel * label;

vlayout = new QVBoxLayout(this);

label = new QLabel(tr("Fields:"));

m_treeview = new QTreeView(this);
m_treeview->setHeaderHidden(true);
m_treeview->setEditTriggers(QAbstractItemView::NoEditTriggers);

connect(m_treeview,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slotDblClickTreeView(QModelIndex)));

vlayout->addWidget(label);
vlayout->addWidget(m_treeview);

setLayout(vlayout);

setWindowTitle(tr("Select field"));

resize(250,300);
}

void QDesSelTreeField::slotDblClickTreeView(const QModelIndex &index)
{
    QStandardItem *item, *parentitem;
    QString value;

    if(!m_treeview->model()) return;

    item = ((QStandardItemModel *)m_treeview->model())->itemFromIndex(index);

    if(!item->parent())
        return;

    if(item->hasChildren())
        return;

    value = item->text();

    parentitem = item->parent();

    if(parentitem)
        value = "DB." + parentitem->text() + "." + value;

    value = "[" + value + "]";

    emit signalSendValue(value);

    accept();
}

//------------------------------------------------------------------------------
QDesSelTreeVariables::QDesSelTreeVariables(QWidget* parent,bool varenabled):QDialog(parent)
{
    QLayout *baselayout;
    QLabel * label;
    QTabWidget * tabwidget = new QTabWidget(this);

    baselayout = new QVBoxLayout(this);

    m_treeviewvariables = Q_NULLPTR;

    m_varenabled = varenabled;

    tabwidget->clear();

    m_treeviewfields = new QTreeView(this);
    m_treeviewfields->setHeaderHidden(true);
    m_treeviewfields->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(m_treeviewfields,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slotDblClickTreeView(QModelIndex)));

    tabwidget->addTab(m_treeviewfields,tr("Fields"));

    if(m_varenabled){
        m_treeviewvariables = new QTreeView(this);
        m_treeviewvariables->setHeaderHidden(true);
        m_treeviewvariables->setEditTriggers(QAbstractItemView::NoEditTriggers);

        connect(m_treeviewvariables,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slotDblClickTreeView(QModelIndex)));

        tabwidget->addTab(m_treeviewvariables,tr("Variables"));
    }

    tabwidget->setTabPosition(QTabWidget::East);

    label = new QLabel(tr("Variables:"));

    baselayout->addWidget(label);
    baselayout->addWidget(tabwidget);

    setLayout(baselayout);

    setWindowTitle(tr("Select Variable"));

    resize(260,330);
}

void QDesSelTreeVariables::updateModels()
{
    if(m_varenabled)
        m_treeviewvariables->expandAll();

    QStandardItem * item;
    if(m_treeviewfields->model())
        for (int i = 0; i < m_treeviewfields->model()->rowCount(); ++i){
            item = ((QStandardItemModel*)m_treeviewfields->model())->item(i);

            if(item->hasChildren())
                m_treeviewfields->setExpanded(item->index(),true);
        }
}

void QDesSelTreeVariables::slotDblClickTreeView(const QModelIndex& index)
{
    QStandardItem *item, *parentitem;
    QString value;
    QTreeView *treeview = (QTreeView *)sender();

    if(!m_treeviewfields->model()) return;
     if(m_treeviewvariables && !m_treeviewvariables->model()) return;

    item = ((QStandardItemModel *)treeview->model())->itemFromIndex(index);

    if(!item->parent())
        return;

    if(item->hasChildren())
        return;

    value = item->text();

    if(treeview == m_treeviewfields){
        parentitem = item->parent();

        if(parentitem)
            value = "DB." + parentitem->text() + "." + value;
    }

    value = "[" + value + "]";

    emit signalSendValue(value);

    accept();
}

//------------------------------------------------------------------------------
QDesSelFormat::QDesSelFormat(QWidget* parent):QDialog(parent)
{
QGridLayout *gridlayout;
QLayout *vlayout;
QGroupBox *group;
QDialogButtonBox *buttons;

buttons = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, this);

connect(buttons, SIGNAL(accepted()), SLOT(accept()));
connect(buttons, SIGNAL(rejected()), SLOT(reject()));

gridlayout = new QGridLayout(this);
vlayout = new QVBoxLayout(this);

group = new QGroupBox(this);
group->setTitle(tr("Variable format"));

m_typevalue = new QComboBox(group);
m_selectformat = new QComboBox(group);

m_precision = new QLineEdit(group);
m_otherformat = new QLineEdit(group);
m_label = new QLabel(" ",group);

m_precision->setText("2");

m_precision->setInputMask("9");

m_typevalue->insertItem(0,tr("Logical"));
m_typevalue->insertItem(0,tr("Time"));
m_typevalue->insertItem(0,tr("Date"));
m_typevalue->insertItem(0,tr("Number"));
m_typevalue->insertItem(0,tr("Text"));
m_typevalue->setCurrentIndex(0);

m_selectformat->setMinimumWidth(120);

m_precision->setMaximumWidth(40);
m_precision->setVisible(false);
m_otherformat->setVisible(false);

connect(m_typevalue,SIGNAL(currentIndexChanged(int)),this,SLOT(slotTypevalueIndexChanged(int)));
connect(m_selectformat,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotSelectformatCurrentIndexChanged(QString)));

gridlayout->addWidget(m_typevalue,0,0);
gridlayout->addWidget(m_selectformat,0,1);
gridlayout->addWidget(m_label,1,0,1,1,Qt::AlignRight);
gridlayout->addWidget(m_precision,1,1,1,1,Qt::AlignLeft);
gridlayout->addWidget(m_otherformat,1,1);

gridlayout->setRowMinimumHeight(1,25);

group->setLayout(gridlayout);

vlayout->addWidget(group);
vlayout->addWidget(buttons);

setLayout(vlayout);

setWindowTitle(tr("Variable format"));

slotTypevalueIndexChanged(0);

setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

void QDesSelFormat::slotTypevalueIndexChanged(int index)
{
if(index == -1) return;

QString strlist;
QStringList list;

switch (index){
case tvText: strlist = "[No]"; break;
case tvNumber:  strlist = LISTNUMS; break;
case tvDate:  strlist = LISTDATES; break;
case tvTime: strlist = LISTTIMES;  break;
case tvBoolean:  strlist = LISTLOGICS; break;
 }

  list = strlist.split("%");

  m_listmodel.setStringList(list);
  m_selectformat->setModel(&m_listmodel);

   m_selectformat->setCurrentIndex(0);

  m_precision->setVisible(false);
  m_otherformat->setVisible(false);
  m_otherformat->setText("");
  m_label->setText(" ");

  if(index == tvNumber)
  {
      m_precision->setVisible(true);
      m_label->setText(tr("precision"));
  }

}

void QDesSelFormat::slotSelectformatCurrentIndexChanged(QString value)
{
        m_otherformat->setVisible(value == "other");

        if(m_typevalue->currentIndex() != tvNumber)
        {
            if(value == "other")
                m_label->setText(tr("format"));
            else
                m_label->setText(" ");
        }
}

void QDesSelFormat::SetFormat(QString format)
{
QStringList list;
int typevalue = 0, typeformat = 0;
QString param = "";
bool ok;

list = format.split("%");

 if(list.count() > 0)
 {
     typevalue = list.at(0).toInt(&ok);

     if(!ok) typevalue = 0;
 }

 if(list.count() > 1)
 {
     typeformat = list.at(1).toInt(&ok);

     if(!ok) typevalue = 0;
 }

  if(list.count() > 2)
     param = list.at(2);

 m_typevalue->setCurrentIndex(typevalue);

slotTypevalueIndexChanged(typevalue);

 m_selectformat->setCurrentIndex(typeformat);

 if(typevalue == tvNumber)
     m_precision->setText(param);
 else if(typevalue != tvText && m_selectformat->currentText() == "other")
     m_otherformat->setText(param);
}

QString QDesSelFormat::Format()
{
QString param = "", format = QString::number(m_typevalue->currentIndex()) + "%" +
                 QString::number(m_selectformat->currentIndex());
bool ok;
int val;
QStringList strlist;

if(m_typevalue->currentIndex() == tvNumber)
{
    val = m_precision->text().toInt(&ok);
    if(!ok) val = 0;

    param = QString::number(val);
} else
{
    if(m_selectformat->currentText() == "other")
        param = m_otherformat->text();
    else{
        if(m_typevalue->currentIndex() == tvBoolean)
            param = m_selectformat->currentText();
        else{
            if(m_typevalue->currentIndex() == tvDate)
                strlist = QString(FORMATDATES).split("%");

            if(m_typevalue->currentIndex() == tvTime)
                strlist = QString(FORMATTIMES).split("%");

            if(m_typevalue->currentIndex() == tvDate || m_typevalue->currentIndex() == tvTime)
            param = strlist.at(m_selectformat->currentIndex());
        }
    }
}

format += "%" + param;

return format;
}

//------------------------------------------------------------------------
QDesTextEditor::QDesTextEditor(QWidget* parent):QDialog(parent)
{
QLayout *vlayout;
QDialogButtonBox *buttons;

buttons = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, this);

connect(buttons, SIGNAL(accepted()), SLOT(accept()));
connect(buttons, SIGNAL(rejected()), SLOT(reject()));

vlayout = new QVBoxLayout(this);

m_texteditor = new QTextEdit(this);

vlayout->addWidget(m_texteditor);
vlayout->addWidget(buttons);
vlayout->setMargin(5);

resize(300, 400);

setWindowTitle(tr("Editor"));
}

void QDesTextEditor::SetStrings(QStringList list)
{
m_texteditor->clear();

foreach (QString value, list)
    m_texteditor->append(value);
}


QStringList QDesTextEditor::Strings()
{
QString value = m_texteditor->toPlainText();

return value.split('\n');

}
//-------------------------------------------------
QDesBarCodeEditor::QDesBarCodeEditor(QWidget* parent):QDialog(parent)
{
QLayout *vlayout, *hlayout;
QDialogButtonBox *buttons;
QLabel *label;

buttons = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, this);

connect(buttons, SIGNAL(accepted()), SLOT(accept()));
connect(buttons, SIGNAL(rejected()), SLOT(reject()));

label = new QLabel(tr("Insert value:"),this);
label->setMargin(0);

m_editor = new QLineEdit(this);
m_editor->setMinimumWidth(200);

vlayout = new QVBoxLayout(this);
hlayout = new QHBoxLayout(this);

m_butfields = new QToolButton(this);
m_butfields->setText("...");
m_butfields->setMaximumWidth(22);
m_butfields->setToolTip(tr("Insert field value"));

m_butvariables = new QToolButton(this);
m_butvariables->setText("...");
m_butvariables->setMaximumWidth(22);
m_butvariables->setToolTip(tr("Insert variables"));

connect(m_butfields,SIGNAL(clicked()),this,SLOT(slotButClicked()));
connect(m_butvariables,SIGNAL(clicked()),this,SLOT(slotButClicked()));

hlayout->addWidget(m_editor);
hlayout->addWidget(m_butfields);
hlayout->addWidget(m_butvariables);
hlayout->setMargin(1);

vlayout->addWidget(label);
((QVBoxLayout*)vlayout)->addLayout(hlayout);
vlayout->addWidget(buttons);

setLayout(vlayout);

setWindowTitle(tr("Barcode value"));

setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

void QDesBarCodeEditor::slotButClicked()
{
QToolButton *but = (QToolButton *)QObject::sender();
QDesTreeSelect * treeselect;
QPoint p;

treeselect = new QDesTreeSelect(this);

if(but == m_butfields){
   treeselect->setExpandAll(false);
    treeselect->SetModel(FormSettings->Fields());
}
else
treeselect->SetModel(FormSettings->Variables());

p = but->parentWidget()->mapToGlobal(but->pos() + QPoint(but->width()/2,but->height()));

connect(treeselect,SIGNAL(signalSelectValue(QString)),this,SLOT(slotSelVariable(QString)));

treeselect->resize(200,200);
treeselect->move(p);

treeselect->show();

}

void QDesBarCodeEditor::slotSelVariable(QString value)
{
m_editor->setText("[" + value + "]");
}



