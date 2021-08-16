#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr)
    :   QWidget(parent)
    {
        m_scene = new QGraphicsScene;
        fill_scene(*m_scene, { 40, 40 }, { 2, 2 });

        m_view = new QGraphicsView(m_scene);
        //view->scale(2, 2);

        m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтали
        m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключим скроллбар по вертикали

        m_width = new QSpinBox;
        m_width->setRange(0, 100);
        m_height = new QSpinBox;
        m_height->setRange(0, 100);

        QPushButton *generateButton = new QPushButton{ "Generate" };
        connect(generateButton,
                QPushButton::pressed,
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
                scene.addRect(QRectF{ lastRect, rectSize });
                lastRect.rx() += rectSize.width();
            }
            lastRect.rx() = 0;
            lastRect.ry() += rectSize.height();
        }
    }

    void scale_scene(qreal scale)
    {

    }

private:
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    QSpinBox* m_width;
    QSpinBox* m_height;
};

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    MainWidget widget;
    widget.show();

    return a.exec();
}

#include "main.moc"
