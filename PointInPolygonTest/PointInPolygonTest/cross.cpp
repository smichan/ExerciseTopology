#include "cross.h"

Cross::Cross(QPointF pos, QPen pen, qreal radius, qreal penWidth){
	position = pos;
	offset = radius;
	lineWidth = penWidth;
	linePen = pen;
	linePen.setWidth(lineWidth);
}

Cross::~Cross()
{
}

QRectF Cross::boundingRect() const
{
	return QRectF(position.x() - offset - lineWidth/2, position.y() - offset - lineWidth/2,
				  2 * offset + lineWidth, 2 * offset + lineWidth);
}

void Cross::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(linePen);

	painter->drawLine(position.x() - offset, position.y() + offset, position.x() + offset, position.y() - offset);
	painter->drawLine(position.x() - offset, position.y() - offset, position.x() + offset, position.y() + offset);
}

QPointF Cross::getPosition()
{
	return position;
}
