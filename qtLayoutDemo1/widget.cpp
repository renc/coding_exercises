#include <QtGui\QtGui>
#include <QtWidgets\QtWidgets>
#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
	mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(0);	
	setLayout(mainLayout);
	 
	for (int i = 0; i < 3; ++i)
	{

		QVBoxLayout *v1 = new QVBoxLayout(this);
		button = new QPushButton(QString("button") + QString::number(i), this);
		button->setStyleSheet(QLatin1String("background-color: rgb(230, 180, 195);"));
		button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

		buttonLayout = new QHBoxLayout(this);
		buttonLayout->addStretch();
		buttonLayout->addWidget(button);
		buttonLayout->addStretch();
		buttonLayout->setStretch(0, 1);
		buttonLayout->setStretch(1, 3);
		buttonLayout->setStretch(2, 1);

		//labelEmpty1 = new QLabel("", this);
		//labelEmpty1->setStyleSheet(QLatin1String("background-color: rgb(180, 255, 195);"));
		//labelEmpty1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

		label = new QLabel(QString("Label")+QString::number(i), this);
		label->setAlignment(Qt::AlignCenter);
		label->setStyleSheet(QLatin1String("background-color: rgb(189, 255, 251);"));
		label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

		v1->addLayout(buttonLayout);
		v1->addStretch(); // or //v1->addWidget(labelEmpty1);
		v1->addWidget(label);
		v1->setSpacing(0);
		v1->setContentsMargins(0, 0, 0, 0);
		v1->setStretch(0, 3);
		v1->setStretch(1, 1);
		v1->setStretch(2, 1);

		mainLayout->addLayout(v1);
		if (i < 2)
		{
			mainLayout->addStretch();
		}
	}
	mainLayout->setStretch(0, 50);
	mainLayout->setStretch(1, 8);
	mainLayout->setStretch(2, 50);
	mainLayout->setStretch(3, 8);
	mainLayout->setStretch(4, 50);
		
	setStyleSheet(QLatin1String("background-color: transparent;"));

	// initial size
	resize(200, 200);
}

Widget::~Widget()
{

}


/*virtual*/ 
void Widget::resizeEvent(QResizeEvent *e)
{
	// when this resizeEvent() is called, the widget already has its new geometry.
	// Warning: Calling resize() or setGeometry() inside resizeEvent() can lead to infinite recursion.
	// Question: widget->resize() then this func is invoked ?

	QWidget::resizeEvent(e);

	int h = height(); h;
	setFixedWidth(h * (3+16.0/50)); // this scale value should match with the mainLayout's setStretch(). 

	//button->setFixedHeight(wh*0.6);
	//button->resize(button->width(), wh*0.6); // wrong, the widget will fresh switch 
	//labelEmpty1->resize(labelEmpty1->width(), wh*0.2);
	//labelEmpty1->setFixedHeight(wh*0.2);
	//label->resize(label->width(), wh*0.2);
	//label->setFixedHeight(wh*0.2);
	//
} 