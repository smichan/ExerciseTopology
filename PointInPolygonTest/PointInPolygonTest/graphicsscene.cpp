#include "graphicsscene.h"

GraphicsScene::GraphicsScene(QObject *parent) :
	QGraphicsScene(parent)
{
	drawModeIsActive = false;
	setModeIsActive = false;
	xIsSet = false;

	polygon = this->addPolygon(m_polygon, QPen(Qt::black, 2), QBrush(Qt::blue));
	//polygon is always behind all objects drawn
	polygon->setZValue(-1);
}

GraphicsScene::~GraphicsScene()
{
	delete polygon;
	delete xPoint;
	clearAndDeletePolygon();
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (drawModeIsActive)
	{
		QPointF point = mouseEvent->scenePos();
		qDebug() << point;
		m_polygon << point;

		QGraphicsItem* cross = new Cross(point, QPen(Qt::black));
		polygonPoints.append(cross);
		this->addItem(cross);

	} else if (setModeIsActive)
	{
		setModeIsActive = false;
		QPointF point = mouseEvent->scenePos();
		qDebug() << point;

		xPoint = new Cross(point, QPen(Qt::red));
		xPoint->setZValue(2);
		this->addItem(xPoint);
		xIsSet = true;
		std::cout << "xIsSet: " << xIsSet << std::endl;
		emit(XIsSet(true));
		emit(setXPosition(point));
	}

}

void GraphicsScene::clearAndDeletePolygon()
{
	for (QList<QGraphicsItem*>::iterator i = polygonPoints.begin(); i != polygonPoints.end(); ++i)
	{
		this->removeItem(*i);
	}
	while (polygonPoints.size() != 0)
	{
		delete polygonPoints.takeLast();
	}
}

void GraphicsScene::drawPolygon()
{
	setModeIsActive = false;
	drawModeIsActive = !drawModeIsActive;
	std::cout << "DrawMode is active: " << drawModeIsActive << std::endl;

	if(drawModeIsActive)
	{
		emit(polygonExists(false));
		clearAndDeletePolygon();

		m_polygon.clear();
		polygon->setPolygon(m_polygon);

	} else {
		emit(polygonExists(false));

		polygon->setPolygon(m_polygon);
		emit(polygonExists(true));
		emit(setPolygon(m_polygon));
	}
}

void GraphicsScene::setX()
{
	if(drawModeIsActive)
	{
		std::cout << "Draw mode is active: Too bad, x cannot be set right now )m." << std::endl;
	} else
	{
		setModeIsActive = !setModeIsActive;
		std::cout << "SetMode is active: " << setModeIsActive << std::endl;

		if(setModeIsActive)
		{
			if(xIsSet)
			{
				this->removeItem(xPoint);
				delete xPoint;
			}
			xIsSet = false;
			emit(XIsSet(false));
		}
	}
}
