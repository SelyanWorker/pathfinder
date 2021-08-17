#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QGraphicsSceneWheelEvent>
#include <QWheelEvent>
#include <QtMath>
#include <QMultiMap>
#include <QVector>

#include <iostream>

struct Node
{
    bool isWall = false;

    QVector<quint32> children;
};

auto generate_map(qint32 width, qint32 height)
{
    assert(width >= 0 && height >= 0);

    QVector<Node> nodes{ width * height };

    for(qint32 i = 0; i < nodes.size(); ++i)
    {
        qint32 y = i / width;
        qint32 x = i - y * width;

        if (y - 1 >= 0)
        {
            nodes[i].children.push_back((y - 1) * width + x);
        }

        if (y + 1 < height)
        {
            nodes[i].children.push_back((y + 1) * width + x);
        }

        if (x - 1 >= 0)
        {
            nodes[i].children.push_back(y * width + x - 1);
        }

        if (x + 1 < width)
        {
            nodes[i].children.push_back(y * width + x + 1);
        }
    }

    return nodes;
}

class CustomGraphicsView : public QGraphicsView
{
public:
    CustomGraphicsView(QWidget* parent = nullptr)
    : QGraphicsView(parent)
    {}

    CustomGraphicsView(QGraphicsScene* scene)
    : QGraphicsView(scene)
    {
    }

protected:
    void wheelEvent(QWheelEvent *event) override
    {
        double factor = qPow(1.0005, event->angleDelta().y());
        scale(factor, factor);

        centerOn(m_target_scene_pos);

//        centerOn(m_target_scene_pos);
//        QPointF delta_viewport_pos =
//            m_target_viewport_pos - QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
//        QPointF viewport_center = mapFromScene(m_target_viewport_pos) - delta_viewport_pos;
//        centerOn(mapToScene(viewport_center.toPoint()));
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        m_target_viewport_pos = event->pos();
        m_target_scene_pos = mapToScene(event->pos());

        QGraphicsView::mouseMoveEvent(event);
    }


private:
    QPoint m_target_viewport_pos;
    QPointF m_target_scene_pos;

};

class CustomRectangle : public QGraphicsRectItem
{
public:
    CustomRectangle(QGraphicsItem* parent = nullptr)
    : QGraphicsRectItem(parent)
    {}

    CustomRectangle(const QRectF& rect)
    : QGraphicsRectItem(rect)
    {
        setAcceptHoverEvents(true);
    }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override
    {
        setBrush(Qt::green);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override
    {
        setBrush(Qt::white);
    }
};

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr)
    :   QWidget(parent)
    {
        m_scene = new QGraphicsScene;
        fill_scene(*m_scene, { 40, 40 }, { 2, 2 });

        m_view = new CustomGraphicsView(m_scene);


        m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтали
        m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключим скроллбар по вертикали

        m_width = new QSpinBox;
        m_width->setRange(0, 100);
        m_height = new QSpinBox;
        m_height->setRange(0, 100);

        QPushButton *generateButton = new QPushButton{ "Generate" };
        connect(generateButton,
                &QPushButton::pressed,
                [&]()
                {
                    fill_scene(*m_scene, { 40, 40 }, QSize{ m_width->value(), m_height->value() });
                });

        QHBoxLayout *sizeInput = new QHBoxLayout;
        sizeInput->addWidget(new QLabel{ "w:" });
        sizeInput->addWidget(m_width);
        sizeInput->addWidget(new QLabel{ "h:" });
        sizeInput->addWidget(m_height);

        QGridLayout* mainLayout = new QGridLayout;
        mainLayout->addWidget(m_view);
        mainLayout->addLayout(sizeInput, 1, 0);
        mainLayout->addWidget(generateButton);

        setLayout(mainLayout);
    }

private slots:
    void fill_scene(QGraphicsScene& scene, const QSize& rectSize, const QSize& size)
    {
        scene.clear();

        QPointF lastRect{ 0, 0 };

        for(size_t i = 0; i < size.height(); ++i)
        {
            for(size_t j = 0; j < size.width(); ++j)
            {
                auto rectPtr = new CustomRectangle(QRectF{ lastRect, rectSize });

                scene.addItem(rectPtr);
                lastRect.rx() += rectSize.width();
            }
            lastRect.rx() = 0;
            lastRect.ry() += rectSize.height();
        }
    }

private:
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    QSpinBox* m_width;
    QSpinBox* m_height;
};

int main(int argc, char **argv)
{
    auto map = generate_map(3, 3);

    QApplication a(argc, argv);

    MainWidget widget;
    widget.show();

    return a.exec();
}

#include "main.moc"
