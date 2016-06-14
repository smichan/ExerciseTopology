#include "trianglewindow.h"

TriangleWindow::TriangleWindow(QString fileName)
	: m_program(0)
	, m_frame(0)
{
	readFile(fileName);
	calculateSimplices();
}

TriangleWindow::~TriangleWindow()
{
	delete visited;
}

void TriangleWindow::initialize()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	m_program->link();
	m_matrixUniform = m_program->uniformLocation("matrix");
	vertBuffer = m_program->attributeLocation("posAttr");
	colBuffer = m_program->attributeLocation("colAttr");

}


void TriangleWindow::render()
{
	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_program->bind();

	QMatrix4x4 matrix;
	matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
	matrix.translate(0, 0, -2);
	matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

	m_program->setUniformValue(m_matrixUniform, matrix);

	vertBuffer = m_program->attributeLocation("posAttr");
	glVertexAttribPointer(vertBuffer, 3, GL_FLOAT, GL_FALSE, 0, &triangles[0]);

	colBuffer = m_program->attributeLocation("colAttr");
	glVertexAttribPointer(colBuffer, 3, GL_FLOAT, GL_FALSE, 0, &colors[0]);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, triangles.size()/3);

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
				  if (num.contains("/"))
				  {
					  indices.push_back(num.split("/").first().toInt()-1);
				  }
				  else
				  {
					  indices.push_back(num.toInt() - 1);
				  }
			  }
		  }
		  else if(value == "v")
		  {
			  foreach(QString num, list)
			  {
				  vertices.push_back(num.toFloat());
			  }
		  }
	   }
	   inputFile.close();
	}
}

void TriangleWindow::calculateSimplices()
{
	if (indices.size() > 0)
	{
		std::vector<int> visitStack;
		countEdges = 0;
		visited = new bool[indices.size()/3];

		for (int i = 0; i < indices.size()/3; i++)
		{
			visited[i] = false;
		}
		for (int j = 0; j < indices.size()/3; j++)
		{
			if(!visited[j] && indices[j*3] >= 0)
			{
				setColor();
				drawConnectedComponent(visitStack, j);
			}
		}
	}
	std::cout << "There are " << triangles.size()/9 << " triangle faces and " << countEdges << " edge faces." << std::endl;
}

void TriangleWindow::drawConnectedComponent(std::vector<int> &visitStack, int triangle)
{
	visited[triangle] = true;

	std::vector<int> adjacentTriangles = findAdjacentTriangles(triangle);
	addToTrianglesAndEdges(triangle);

	foreach (int i, adjacentTriangles)
	{
		visitStack.push_back(i);
	}

	while(!visitStack.empty())
	{
		int nextTriangle = visitStack[visitStack.size()-1];
		visitStack.pop_back();

		if (indices[nextTriangle*3] >= 0)
		{
			if(!visited[nextTriangle])
			{
				drawConnectedComponent(visitStack, nextTriangle);
			}
			else
			{

			}
		}
	}

}

void TriangleWindow::addToTrianglesAndEdges(int triangle)
{
	triangles.push_back(vertices[indices[triangle*3]*3]);
	triangles.push_back(vertices[indices[triangle*3]*3+1]);
	triangles.push_back(vertices[indices[triangle*3]*3+2]);
	triangles.push_back(vertices[indices[triangle*3+1]*3]);
	triangles.push_back(vertices[indices[triangle*3+1]*3+1]);
	triangles.push_back(vertices[indices[triangle*3+1]*3+2]);
	triangles.push_back(vertices[indices[triangle*3+2]*3]);
	triangles.push_back(vertices[indices[triangle*3+2]*3+1]);
	triangles.push_back(vertices[indices[triangle*3+2]*3+2]);

	colors.push_back(r);
	colors.push_back(g);
	colors.push_back(b);
	colors.push_back(r);
	colors.push_back(g);
	colors.push_back(b);
	colors.push_back(r);
	colors.push_back(g);
	colors.push_back(b);

	edges.push_back(vertices[indices[triangle*3]*3]);
	edges.push_back(vertices[indices[triangle*3]*3+1]);
	edges.push_back(vertices[indices[triangle*3]*3+2]);
	edges.push_back(vertices[indices[triangle*3+1]*3]);
	edges.push_back(vertices[indices[triangle*3+1]*3+1]);
	edges.push_back(vertices[indices[triangle*3+1]*3+2]);
	edges.push_back(vertices[indices[triangle*3+2]*3]);
	edges.push_back(vertices[indices[triangle*3+2]*3+1]);
	edges.push_back(vertices[indices[triangle*3+2]*3+2]);


	edgeColors.push_back(1.0);
	edgeColors.push_back(1.0);
	edgeColors.push_back(1.0);
	edgeColors.push_back(1.0);
	edgeColors.push_back(1.0);
	edgeColors.push_back(1.0);
	edgeColors.push_back(1.0);
	edgeColors.push_back(1.0);
	edgeColors.push_back(1.0);
}

void TriangleWindow::setSameTrianglesToNeg(std::vector<int> &sharesA, std::vector<int> &sharesB, std::vector<int> &sharesC)
{
	for (int i = 0; i < sharesA.size(); i++)
	{
		for (int j = 0; j < sharesB.size(); j++)
		{
			if (sharesA[i] == sharesB[j])
			{
				for (int k = 0; k < sharesC.size(); k++)
				{
					if (sharesA[i] == sharesC[k] && sharesC[k] == sharesB[j])
					{
						// das geht zwar schöner, aber es funktioniert.
						// share-Vektoren werden auf ungültige unterschiedliche Werte gesetzt und
						// in indices werden die Werte auf -1 gesetzt, um direkt darauf prüfen zu können,
						// z.B. in sharePoint(int a)
						if (!visited[sharesA[i]])
						{
							indices[3*sharesA[i]] = -1;
							indices[3*sharesA[i]+1] = -1;
							indices[3*sharesA[i]+2] = -1;
						}
						sharesA[i] = -2;
						sharesB[j] = -3;
						sharesC[k] = -4;
					}
				}
			}
		}
	}
}

void TriangleWindow::setColor()
{
	int number = 20;
	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());
	r = (qrand()%number)/(float)number;
	g = (qrand()%number)/(float)number;
	b = (qrand()%number)/(float)number;
}

std::vector<int> TriangleWindow::findAdjacentTriangles(int k)
{
	std::vector<int> triangles;

	int a = indices[3*k];
	int b = indices[3*k + 1];
	int c = indices[3*k + 2];

	std::vector<int> sharesA = sharePoint(a);
	std::vector<int> sharesB = sharePoint(b);
	std::vector<int> sharesC = sharePoint(c);

	setSameTrianglesToNeg(sharesA, sharesB, sharesC);

	std::vector<int> sharesAB = commonEdges(sharesA, sharesB, k);
	std::vector<int> sharesAC = commonEdges(sharesA, sharesC, k);
	std::vector<int> sharesBC = commonEdges(sharesB, sharesC, k);

	triangles.insert(triangles.end(), sharesAB.begin(), sharesAB.end());
	triangles.insert(triangles.end(), sharesAC.begin(), sharesAC.end());
	triangles.insert(triangles.end(), sharesBC.begin(), sharesBC.end());

	return triangles;
}

std::vector<int> TriangleWindow::sharePoint(int p)
{
	std::vector<int> sharePointP;
	for (unsigned int i = 0; i < indices.size(); i++)
	{
		if (p == indices[i] && p != -1)
		{
			sharePointP.push_back(i/3);
		}
	}
	return sharePointP;
}

std::vector<int> TriangleWindow::commonEdges(std::vector<int> &pointsA, std::vector<int> &pointsB, int k)
{
	std::vector<int> cutSet;
	//k refers to the triangle we already have -> we remove k from the cut set
	foreach (int i, pointsA)
	{
		foreach (int j, pointsB)
		{
			if (i == j && i != k)
			{
				cutSet.push_back(i);
			}
		}
	}
	return cutSet;
}
