// 这个是qt 自动的默认 例子. 关于QBarSeries的. 
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts>
/*
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>*/

QT_CHARTS_USE_NAMESPACE; 

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QBarSet *set0 = new QBarSet("Jane");
	QBarSet *set1 = new QBarSet("John");
	QBarSet *set2 = new QBarSet("Axel");
	QBarSet *set3 = new QBarSet("Mary");
	QBarSet *set4 = new QBarSet("Samantha");

	*set0 << 1 << 2 << 3 << 4 << 5 << 6;
	*set1 << 5 << 0 << 0 << 4 << 0 << 7;
	*set2 << 3 << 5 << 8 << 13 << 8 << 5;
	*set3 << 5 << 6 << 7 << 3 << 4 << 5;
	*set4 << 9 << 7 << 5 << 3 << 1 << 2;

	QBarSeries *series = new QBarSeries();
	series->append(set0);
	series->append(set1);
	series->append(set2);
	series->append(set3);
	series->append(set4);


	QChart *chart = new QChart();
	chart->addSeries(series);
	chart->setTitle("Simple barchart example");
	chart->setAnimationOptions(QChart::SeriesAnimations);

	QStringList categories;
	categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
	QBarCategoryAxis *axis = new QBarCategoryAxis();
	axis->append(categories);
	chart->createDefaultAxes();
	chart->setAxisX(axis, series); // 尝试不要这个自定义的axisX, 而单单用上面的default axes. 

	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);

	QChartView *chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	QMainWindow window;
	window.setCentralWidget(chartView);
	window.resize(420, 300);
	window.show();

	return a.exec();
}
