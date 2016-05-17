#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QMessageBox>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	std::vector<int> readIndices();
	std::vector<int> findAdjacentTriangles(int k);
	std::vector<int> sharePoint(int p);
	std::vector<int> commonEdges(std::vector<int> &pointsA, std::vector<int> &pointsB, int k);

	bool isOrientable();
	bool isOrientableForTriangle(int k, int adjacentTriangle);
	bool swapOrientationForAdjacentTriangles(int k, std::vector<int> &visitStack);
	bool canBeFlipped(int k);

	void rotateTriangle(int k);


public slots:

	void checkTriangulation();
	void setMobiusInput();
	void setRingInput();

private:
	Ui::MainWindow *ui;

	std::vector<int> indices;
	bool* flipped;
	bool* visited;
};

#endif // MAINWINDOW_H
