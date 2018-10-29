#ifndef MYGRAPHICVIEW_H
#define MYGRAPHICVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsEllipseItem>
#include <QDebug>
#include <QMouseEvent>
#include <QPointF>
#include <QModelIndex>
#include <QGraphicsEllipseItem>

class QGraphicsView;

class MyGraphicView: public QGraphicsView
{
    Q_OBJECT

public:
    explicit MyGraphicView(QWidget *parent = nullptr);
    ~MyGraphicView();

protected:
    virtual void mousePressEvent ( QMouseEvent *event );

private:
    QGraphicsScene *scene;
    int status;    // можно ли рисовать, и что рисовать

private slots:
    void treeClicked(const QModelIndex&);

};

#endif // MYGRAPHICVIEW_H
