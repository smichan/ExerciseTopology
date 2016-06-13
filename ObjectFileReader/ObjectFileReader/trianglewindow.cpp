#include "trianglewindow.h"

TriangleWindow::TriangleWindow(QString fileName)
	: m_program(0)
	, m_frame(0)
{
	readFile(fileName);
}

void TriangleWindow::initialize()
{
	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	m_program->link();
	m_posAttr = m_program->attributeLocation("posAttr");
	m_colAttr = m_program->attributeLocation("colAttr");
	m_matrixUniform = m_program->uniformLocation("matrix");
}

void TriangleWindow::render()
{
	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	glClear(GL_COLOR_BUFFER_BIT);

	m_program->bind();

	QMatrix4x4 matrix;
	matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
	matrix.translate(0, 0, -2);
	matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

	m_program->setUniformValue(m_matrixUniform, matrix);

	GLfloat vertices[] = {
		0.0f, 0.707f,
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.0f, 0.707f,
		1.0f, 0.707f,
		0.5f, -0.5f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	m_program->release();

	++m_frame;
}

void TriangleWindow::readFile(QString fileName)
{
	QFile inputFile(fileName);
	if (inputFile.open(QIODevice::ReadOnly))
	{
	   QTextStream in(&inputFile);
	   while (!in.atEnd())
	   {
		  QString line = in.readLine();
		  QStringList list = line.split(" ");
		  QString value = list.at(0);
		  list.removeFirst();
		  if(value == "f")
		  {
			  foreach(QString num, list)
			  {
				  indices.push_back(num.toInt());
			  }
		  }
		  else if(value == "v")
		  {
			  foreach(QString num, list)
			  {
				  vertices.push_back(num.toFloat());
			  }
		  }
		  else
		  {
			  cout << "No valid line" << endl;
		  }
	   }
	   inputFile.close();
	   cout << indices.size() << " " << vertices.size() << endl;
	}
}
