#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->triangulationButton, SIGNAL(clicked()), this, SLOT(checkTriangulation()));
	connect(ui->mobiusButton, SIGNAL(clicked()), this, SLOT(setMobiusInput()));
	connect(ui->ringButton, SIGNAL(clicked()), this, SLOT(setRingInput()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

std::vector<int> MainWindow::readIndices()
{
	std::vector<int> output;

	QString text = ui->triangulationLine->text();
	QStringList list = text.split(",", QString::SkipEmptyParts);
	foreach(QString num, list)
	{
		output.push_back(num.toInt());
	}
	return output;
}

std::vector<int> MainWindow::findAdjacentTriangles(int k)
{
	std::vector<int> triangles;

	int a = indices[3*k];
	int b = indices[3*k + 1];
	int c = indices[3*k + 2];

	std::vector<int> sharesA = sharePoint(a);
	std::vector<int> sharesB = sharePoint(b);
	std::vector<int> sharesC = sharePoint(c);

	std::vector<int> sharesAB = commonEdges(sharesA, sharesB, k);
	std::vector<int> sharesAC = commonEdges(sharesA, sharesC, k);
	std::vector<int> sharesBC = commonEdges(sharesB, sharesC, k);

	triangles.insert(triangles.end(), sharesAB.begin(), sharesAB.end());
	triangles.insert(triangles.end(), sharesAC.begin(), sharesAC.end());
	triangles.insert(triangles.end(), sharesBC.begin(), sharesBC.end());

	return triangles;
}

std::vector<int> MainWindow::sharePoint(int p)
{
	std::vector<int> sharePointP;
	for (unsigned int i = 0; i < indices.size(); i++)
	{
		if (p == indices[i])
		{
			sharePointP.push_back(i/3);
		}
	}
	return sharePointP;
}

std::vector<int> MainWindow::commonEdges(std::vector<int> &pointsA, std::vector<int> &pointsB, int k)
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

bool MainWindow::isOrientable()
{
	indices = readIndices();
	int numberOfTriangles = indices.size()/3;
	bool isOrientable = true;
	if (numberOfTriangles > 0)
	{
		std::vector<int> visitStack;
		visitStack.push_back(0);

		flipped = new bool[numberOfTriangles];
		visited = new bool[numberOfTriangles];

		for (int i = 0; i < numberOfTriangles; i++)
		{
			flipped[i] = false;
			visited[i] = false;
		}

		while(!visitStack.empty())
		{
			int triangle = visitStack[visitStack.size()-1];
			visitStack.pop_back();

			if(!visited[triangle])
			{
				if(!swapOrientationForAdjacentTriangles(triangle, visitStack))
				{
					isOrientable = false;
				}
			}
		}
	} else
	{
		//if there are no triangles just return false;
		isOrientable = false;
	}

	return isOrientable;
}

bool MainWindow::isOrientableForTriangle(int k, int adjacentTriangle)
{
	//for triangle k we check for any common edge between adjacentTriangle and k and flip it when necessary
	bool isOrientableForTriangle = true;

	int a = indices[3*k];
	int b = indices[3*k + 1];
	int c = indices[3*k + 2];

	//the special case (all points are shared) is always true, because these two triangles are either the same,
	//or they are the flipped version, which would be orientable too.
	if (a == indices[3*adjacentTriangle] || a == indices[3*adjacentTriangle + 1] || a == indices[3*adjacentTriangle + 2])
	{
		//a is shared, and b or c are shared
		while (a != indices[3*adjacentTriangle])
		{
			rotateTriangle(adjacentTriangle);
		}
		if (b == indices[3*adjacentTriangle + 1] || c == indices[3*adjacentTriangle + 2])
		{
			if(!canBeFlipped(adjacentTriangle))
			{
				isOrientableForTriangle = false;
			}
		}
	} else
	{
		//b and c are shared points!
		while (b != indices[3*adjacentTriangle])
		{
			rotateTriangle(adjacentTriangle);
		}
		if (c == indices[3*adjacentTriangle + 1])
		{
			if(!canBeFlipped(adjacentTriangle))
			{
				isOrientableForTriangle = false;
			}
		}
	}
	return isOrientableForTriangle;
}

bool MainWindow::swapOrientationForAdjacentTriangles(int k, std::vector<int> &visitStack)
{
	//for triangle k swap the orientation of all adjacent triangles if necessary
	//if not possible, return false
	bool isOrientableForAllAdjacentTriangles = true;
	visited[k] = true;

	std::vector<int> adjacentTriangles = findAdjacentTriangles(k);

	foreach (int i, adjacentTriangles)
	{
		visitStack.push_back(i);
		if (!isOrientableForTriangle(k, i))
		{
			isOrientableForAllAdjacentTriangles = false;
		}
	}
	return isOrientableForAllAdjacentTriangles;
}

bool MainWindow::canBeFlipped(int k)
{
	bool canBeFlipped = true;
	if (flipped[k] || visited[k])
	{
		canBeFlipped = false;
	} else
	{
		flipped[k] = true;
		int temp = indices[3*k];
		indices[3*k] = indices[3*k + 1];
		indices[3*k + 1] = temp;
	}
	return canBeFlipped;
}

void MainWindow::rotateTriangle(int k)
{
	int temp = indices[3*k];
	indices[3*k] = indices[3*k + 1];
	indices[3*k + 1] = indices[3*k + 2];
	indices[3*k + 2] = temp;
}

void MainWindow::checkTriangulation()
{
	QMessageBox msgBox;
	if(isOrientable())
	{
		msgBox.setText("The triangulation is orientable! °˖✧◝(⁰▿⁰)◜✧˖°");
	} else {
		msgBox.setText("Are you insulting me?!\nThis surface is non-orientable. (๑･`▱´･๑)");
	}

	msgBox.exec();
}

void MainWindow::setMobiusInput()
{
	ui->triangulationLine->setText("1,0,2,1,2,3,3,2,4,3,4,5,5,4,1,1,0,5");
}

void MainWindow::setRingInput()
{
	ui->triangulationLine->setText("0,2,1,1,2,3,3,2,4,3,4,5,5,4,0,1,5,0");
}



