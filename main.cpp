#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

namespace utils
{
    void fill_scene(QGraphicsScene& scene, const QSize& rectSize, const QSize& size)
    {
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
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr)
    :   QWidget(parent)
    {
        QGraphicsScene* scene = new QGraphicsScene;
        utils::fill_scene(*scene, { 40, 40 }, { 2, 2 });

        QGraphicsView* view = new QGraphicsView(scene);

        QSpinBox* w = new QSpinBox;
        w->setRange(0, 100);
        QSpinBox* h = new QSpinBox;
        h->setRange(0, 100);

        QPushButton* generateButton = new QPushButton{ "Generate" };

        QHBoxLayout* sizeInput = new QHBoxLayout;
        sizeInput->addWidget(new QLabel{ "w:" });
        sizeInput->addWidget(w);
        sizeInput->addWidget(new QLabel{ "h:" });
        sizeInput->addWidget(h);

        QGridLayout* mainLayout = new QGridLayout;
        mainLayout->addWidget(view);
        mainLayout->addLayout(sizeInput, 1, 0);
        mainLayout->addWidget(generateButton);

        setLayout(mainLayout);
    }

};

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    MainWidget widget;
    widget.show();

    return a.exec();
}

#include "main.moc"
