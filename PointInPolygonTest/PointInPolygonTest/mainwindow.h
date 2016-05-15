#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsscene.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <iostream>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();

	void removeLine(QGraphicsLineItem *line);
	QGraphicsLineItem *drawLineThroughX();
	void resetLine();
	int calculateCrossings();
	bool doesCross(QPointF x, QPointF a, QPointF b);
	float determinante(QPointF x, QPointF a, QPointF b);

public slots:
	void testInsideOutside();
	void setPolygonExists(bool b);
	void setXExists(bool b);
	void setPolygon(QPolygonF polygon);
	void setXPosition(QPointF point);

private:
	Ui::MainWindow *ui;

	GraphicsScene *scene;

	QGraphicsLineItem *xLine;

	bool polygonExists;
	bool XExists;
	bool lineIsSet;
	bool isInPolygon;

	QPointF xPosition;
	QPolygonF polygonPoints;

};

#endif // MAINWINDOW_H
