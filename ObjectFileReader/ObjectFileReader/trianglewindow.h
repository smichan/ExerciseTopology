#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include "openglwindow.h"
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QMatrix4x4>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>
#include <fstream>
#include <iostream>
#include <QFile>
#include <QTime>

using namespace std;

class TriangleWindow : public OpenGLWindow
{
	Q_OBJECT
public:
	TriangleWindow(QString fileName);
	~TriangleWindow();

	void initialize() Q_DECL_OVERRIDE;
	void render() Q_DECL_OVERRIDE;

	void mouseMoveEvent(QMouseEvent* eve) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent* eve) Q_DECL_OVERRIDE;

	void initializeRender();

	void readFile(QString fileName);
	void calculateSimplices();
	void drawConnectedComponent(std::vector<int> &visitStack, int triangle);
	void addToTrianglesAndEdges(int triangle);
	void setColor();
	void joinEdgesAndTriangles();
	void setScalingFactor(int i);
	void calculateObjectCenter();
	std::vector<int> findAdjacentTriangles(int k);
	std::vector<int> sharePoint(int p);
	std::vector<int> commonEdges(std::vector<int> &pointsA, std::vector<int> &pointsB, int k);

private:
	QMatrix4x4 matrix;

	GLuint vertBuffer;
	GLuint colBuffer;
	GLuint indBuffer;
	GLuint m_matrixUniform;

	QOpenGLShaderProgram *m_program;
	int m_frame;
	int lineSegments, triangleFaces;

	std::vector<float> vertices;
	std::vector<int> indices;

	std::vector<float> triangles;
	std::vector<float> edges;
	std::vector<float> edgeColors;
	std::vector<float> colors;

	bool* visited;
	float xCenter, yCenter, zDepth;
	int countAdjacent;
	float r, g, b;
};


static const char *vertexShaderSource =
	"attribute highp vec4 posAttr;\n"
	"attribute lowp vec4 colAttr;\n"
	"varying lowp vec4 col;\n"
	"uniform highp mat4 matrix;\n"
	"void main() {\n"
	"   col = colAttr;\n"
	"   gl_Position = matrix * posAttr;\n"
	"}\n";

static const char *fragmentShaderSource =
	"varying lowp vec4 col;\n"
	"void main() {\n"
	"   gl_FragColor = col;\n"
	"}\n";

#endif // TRIANGLEWINDOW_H
