#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets\QtWidgets>
#include <QtGui\QtGui> 
#include <QtGui/QResizeEvent>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
	

public:
    //void resize(int w, int h); º¯ÊýÊÇpublic and not virtual,
    //i think this func is not supposted by to override.

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
	QHBoxLayout *mainLayout;
	QPushButton *button;
	QHBoxLayout *buttonLayout;
	QLabel *labelEmpty1;
	QLabel *label;
};

#endif // WIDGET_H
