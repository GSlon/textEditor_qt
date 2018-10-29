#include "mygraphicview.h"

MyGraphicView::MyGraphicView(QWidget *parent)
    : QGraphicsView(parent)   // вызов родительского конструктора
{
    // this->setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
    // this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету

    //не указываем размеры сцены - будет изменяться, пытаясь уместить все элементы внутри себя (скроллы)
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    scene->addText("");  // калибровка сцены ( иначе первый раз рисует в (0,0) )
    // scene->setSceneRect(0, 0, this->width(), this->height());

    status = 0;  // рисовать нельзя, пока не выбрана фигура
}

MyGraphicView::~MyGraphicView()
{
}


void MyGraphicView::mousePressEvent(QMouseEvent *event)
{
    if ((status == 0) || (event->button() != Qt::MouseButton::LeftButton))
        return;

    QPointF pos = this->mapToScene(event->pos().x(), event->pos().y());
    qDebug() << pos.x() << pos.y();

    if (status == 1)
    {
        // scene->addEllipse(pos.x() - 8, pos.y() - 8, 20, 20);   // 8 потому что по умолчанию рисует не на кончике курсора
        QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(pos.x() - 8, pos.y() - 8, 20, 20);
        circle->setFlag(QGraphicsItem::ItemIsMovable);
        scene->addItem(circle);
    }
    else
        if (status == 2)
        {
            // scene->addEllipse(pos.x() - 18, pos.y() - 8, 40, 20);
            QGraphicsEllipseItem *ell = new QGraphicsEllipseItem(pos.x() - 18, pos.y() - 8, 40, 20);
            ell->setFlag(QGraphicsItem::ItemIsMovable);
            scene->addItem(ell);
        }
}

void MyGraphicView::treeClicked(const QModelIndex &index)
{
    QVariant name = index.data();
    // qDebug() << name.toString();

    if (name.toString() == "figure")
        status = 0;
    else
        if (name.toString() == "circle")
            status = 1;
    else
        if (name.toString() == "ellipse")
            status = 2;
}
