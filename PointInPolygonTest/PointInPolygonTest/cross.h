#ifndef CROSS_H
#define CROSS_H

#include <QPainter>
#include <QGraphicsScene>
#include <QPointF>
#include <QList>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <iostream>

class Cross : public QGraphicsItem
{
public:
	Cross(QPointF pos, QPen pen, qreal offset = 5.0, qreal penWidth = 2.0);
	virtual ~Cross();

	QRectF boundingRect() const;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QPointF getPosition();

private:
	QPointF position;
	QPen linePen;
	qreal offset;
	qreal lineWidth;

};

#endif // CROSS_H
