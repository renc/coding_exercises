#include "mainwindow.h"
#include <QtSvg/QtSvg>
#include <QtWidgets/QtWidgets>

class SvgPushButton : public QPushButton
{
public:
    SvgPushButton(QString filePath, QWidget *parent=0)
        : QPushButton(parent)
    {
        //setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        resize(300, 300);// this is important.

        QSvgWidget *icon = new QSvgWidget(filePath, this);
        setLayout( new QHBoxLayout(this) );
        layout()->addWidget(icon);

        setText("button custom");
    }
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // create a scene with items,
    // use the view to show the scene
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *gv = new QGraphicsView(this);
    gv->setScene(scene);
    if (gv->scene())
    {
        printf("renc: scene is not null.\n");

        // these two lines are at scene origin pos.
        scene->addLine(0, 0, 100, 0, QPen(Qt::red));
        scene->addLine(0, 0, 0, 100, QPen(Qt::green));

        /*QGraphicsRectItem *rect = */
        scene->addRect(QRectF(0,0,50,50), QPen(Qt::red), QBrush(Qt::green));

        // svg image
        QGraphicsSvgItem *svgItem = new QGraphicsSvgItem(
            "D:/rect2.svg");
        scene->addItem((svgItem));
        svgItem->setPos(200, 0);
        bool rValid1 = svgItem->renderer()->isValid();
        printf("--%d--\n", rValid1);

        // QWidget to scene
        QPushButton *btn = new QPushButton("button 1");// at (0,0) by default.
        QGraphicsProxyWidget *btnWidget = scene->addWidget(btn);
        btnWidget->setPos(0, 100);

        //
        {
            SvgPushButton *btn2 = new SvgPushButton("D:/method-draw-image1.svg");
            QGraphicsProxyWidget *btnWidget2 = scene->addWidget(btn2);
            btnWidget2->setPos(0, 150);
        }
    }
    else
        printf("renc: scene is null.\n");
    setCentralWidget(gv);


    setWindowTitle("Demo: graphics view");
}

MainWindow::~MainWindow()
{

}

// renc: 2016/3/26
// in this demo, try to learn a bit of the Graphics View framework.
// 1. scene, item, view;
// 2. load and display svg;
// 3. add widget to scene, graphicsitem->graphicsobject(signal/slot)
//    ->graphicswidget(custom widget)->graphicsProxyWidget(use with QWidget).
// 4. svg as icon to a button. by using button, we have the hover state.


