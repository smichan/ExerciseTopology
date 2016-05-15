#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include "cross.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QList>
#include <QDebug>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsSceneEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <iostream>



class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	GraphicsScene(QObject *parent);
	virtual ~GraphicsScene();

	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void clearAndDeletePolygon();

public slots:
	void drawPolygon();
	void setX();

signals:
	void polygonExists(bool b);
	void XIsSet(bool b);
	void setPolygon(QPolygonF polygon);
	void setXPosition(QPointF point);

private:
	QPolygonF m_polygon;
	QPointF m_xLocation;

	QGraphicsPolygonItem* polygon;
	QList<QGraphicsItem*> polygonPoints;
	QGraphicsItem* xPoint;

	bool drawModeIsActive;
	bool setModeIsActive;
	bool xIsSet;
};

#endif // GRAPHICSSCENE_H
