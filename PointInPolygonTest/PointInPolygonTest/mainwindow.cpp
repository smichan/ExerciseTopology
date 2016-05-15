#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	scene = new GraphicsScene(this);
	scene->setStickyFocus(true);
	ui->graphicsView->setScene(scene);
	ui->graphicsView->setSceneRect(0,0,300,200);

	XExists = false;
	polygonExists = false;
	lineIsSet = false;
	isInPolygon = false;

	connect(ui->DrawPolygonButton, SIGNAL(clicked()), scene, SLOT(drawPolygon()));
	connect(ui->SetXButton, SIGNAL(clicked()), scene, SLOT(setX()));
	connect(ui->InPolygonTestButton, SIGNAL(clicked()), this, SLOT(testInsideOutside()));
	connect(scene, SIGNAL(XIsSet(bool)), this, SLOT(setXExists(bool)));
	connect(scene, SIGNAL(polygonExists(bool)),this,SLOT(setPolygonExists(bool)));
	connect(scene, SIGNAL(setPolygon(QPolygonF)), this, SLOT(setPolygon(QPolygonF)));
	connect(scene, SIGNAL(setXPosition(QPointF)), this, SLOT(setXPosition(QPointF)));
}

MainWindow::~MainWindow()
{
	delete ui;
	delete scene;
	delete xLine;
}

void MainWindow::removeLine(QGraphicsLineItem* line)
{
	scene->removeItem(line);
}


QGraphicsLineItem* MainWindow::drawLineThroughX()
{
	qDebug() << xPosition;
	resetLine();
	QGraphicsLineItem* line = scene->addLine(-500, xPosition.y(), 500, xPosition.y(), QPen(QBrush(Qt::gray), 2));
	line->setZValue(1);

	return line;
}

void MainWindow::resetLine()
{
	if (lineIsSet)
	{
		lineIsSet = false;
		scene->removeItem(xLine);
		delete xLine;
	}
}

int MainWindow::calculateCrossings()
{
	int result = 0;

	if(doesCross(xPosition, polygonPoints.at(0), polygonPoints.at(polygonPoints.size()-1)))
	{
		result++;
	}
	for(int i = 0; i < polygonPoints.size()-1; i++)
	{
		if(doesCross(xPosition, polygonPoints.at(i), polygonPoints.at(i+1)))
		{
			result++;
		}
	}
	std::cout << "There are " << result << " crossings right of X!" << std::endl;
	if (isInPolygon)
	{
		result = 0;
	}
	return result;
}

bool MainWindow::doesCross(QPointF x, QPointF a, QPointF b)
{
	bool result = false;
	if (a.y() >= b.y())
	{
		QPointF temp = a;
		a = b;
		b = temp;
	}
	if (x.y() >= a.y() && x.y() <= b.y())
	{
		// otherwise the line is not crossed by the half-line
		float det = determinante(x, a, b);

		if (det == 0)
		{
			//x,a,b are on a line
			isInPolygon = true;
		} else if (det > 0)
		{
			result = true;
		}
	}
	return result;
}

float MainWindow::determinante(QPointF x, QPointF a, QPointF b)
{
	qDebug() << "x is " << x << " a is " << a << " and b is " << b;
	float result = a.x()*b.y() - a.y()*b.x() -(x.x()*b.y() - x.y()*b.x()) + x.x()*a.y() - x.y()*a.x();
	std::cout << "The result is " << result << std::endl;
	return result;
}

void MainWindow::testInsideOutside()
{
	std::cout << "Test button was triggered!" << std::endl;
	if (polygonExists && XExists)
	{
		std::cout << "Polygon and X do exist!" << std::endl;
		xLine = drawLineThroughX();
		lineIsSet = true;

		int crossings = calculateCrossings();
		if (crossings % 2 == 0)
		{
			ui->statusBar->clearMessage();
			ui->statusBar->showMessage("X is outside the polygon! (╯°□°）╯︵ ┻━┻", 3000);
		} else {
			ui->statusBar->clearMessage();
			ui->statusBar->showMessage("X is inside the polygon! ¯\\_(ツ)_/¯", 3000);
		}

	} else
	{
		std::cout << "Polygon and/or X do not exist!" << std::endl;
	}
}

void MainWindow::setPolygonExists(bool b)
{
	polygonExists = b;
	resetLine();
}

void MainWindow::setXExists(bool b)
{
	XExists = b;
	resetLine();
}

void MainWindow::setPolygon(QPolygonF polygon)
{
	polygonPoints = polygon;
}

void MainWindow::setXPosition(QPointF point)
{
	xPosition = point;
}
