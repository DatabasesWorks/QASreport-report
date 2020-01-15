#ifndef FORMIMAGES_H
#define FORMIMAGES_H

#include <QWidget>
#include <QVariant>
#include "mainwindow.h"

namespace Ui {
class FormImages;
}

class FormImages : public QWidget
{
    Q_OBJECT

public:
    explicit FormImages(QWidget *parent = 0);
    ~FormImages();

protected:
  virtual bool eventFilter(QObject*, QEvent*);

private slots:
void slotSelectRec(const QModelIndex &current, const QModelIndex &previous);


private:
    Ui::FormImages *ui;


   QSqlQueryModel* m_biolife;

   QStringListModel m_notes;
   QImage m_image;

   QPainter m_painter;

   void drawImage();

public:
      void initPrint();

};

#endif // FORMIMAGES_H
