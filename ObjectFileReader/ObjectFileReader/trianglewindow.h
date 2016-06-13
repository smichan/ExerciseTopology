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



using namespace std;

class TriangleWindow : public OpenGLWindow
{
public:
	TriangleWindow(QString fileName);

	void initialize() Q_DECL_OVERRIDE;
	void render() Q_DECL_OVERRIDE;

	void readFile(QString fileName);

private:
	GLuint m_posAttr;
	GLuint m_colAttr;
	GLuint m_matrixUniform;

	QOpenGLShaderProgram *m_program;
	int m_frame;
	std::vector<float> vertices;
	std::vector<float> colors;
	std::vector<int> indices;
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
