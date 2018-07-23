
#include <QtWidgets>
#include <QtSql>
  
static bool createConnection()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	//db.setDatabaseName(":memory:");
	db.setDatabaseName(QApplication::applicationDirPath() + "/db1.db");
	if (!db.open()) {
		QMessageBox::critical(0, qApp->tr("Cannot open database"),
			qApp->tr("Unable to establish a database connection.\n"
				"This example needs SQLite support. Please read "
				"the Qt SQL driver documentation for information how "
				"to build it.\n\n"
				"Click Cancel to exit."), QMessageBox::Cancel);
		return false;
	}

	QSqlQuery query;
	query.exec("create table person (id int primary key, "
		"firstname varchar(20), lastname varchar(20))");
	query.exec("insert into person values(101, 'Danny', 'Young')");
	query.exec("insert into person values(102, 'Christine', 'Holand')");
	query.exec("insert into person values(103, 'Lars', 'Gordon')");
	query.exec("insert into person values(104, 'Roberto', 'Robitaille')");
	query.exec("insert into person values(105, 'Maria', 'Papadopoulos')");

	return true;
}

void initializeModel(QSqlTableModel *model)
{
    model->setTable("person");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("First name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last name"));
}

QTableView *createView(QSqlTableModel *model, const QString &title = "")
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle(title);
    return view;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (!createConnection())
        return 1;

    QSqlTableModel model;

    initializeModel(&model);

    QTableView *view1 = createView(&model, QObject::tr("Table Model (View 1)"));
    QTableView *view2 = createView(&model, QObject::tr("Table Model (View 2)"));

    view1->show();
    view2->move(view1->x() + view1->width() + 20, view1->y());
    view2->show();

    return app.exec();
}
