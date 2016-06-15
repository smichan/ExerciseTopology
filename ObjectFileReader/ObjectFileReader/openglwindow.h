#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit OpenGLWindow(QWindow *parent = 0);
	~OpenGLWindow();

	virtual void render(QPainter *painter);
	virtual void render();

	virtual void initialize();

	void setAnimating(bool animating);

public slots:
	void renderLater();
	void renderNow();

protected:
	virtual void mouseMoveEvent(QMouseEvent* eve);
	virtual void wheelEvent(QWheelEvent* eve);
	bool event(QEvent *event) Q_DECL_OVERRIDE;

	void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

private:
	bool m_update_pending;
	bool m_animating;

	QOpenGLContext *m_context;
	QOpenGLPaintDevice *m_device;
};
#endif
