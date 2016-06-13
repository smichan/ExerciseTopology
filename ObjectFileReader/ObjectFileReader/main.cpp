#include "trianglewindow.h"

#include <QApplication>
#include <QFileDialog>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QSurfaceFormat format;
	format.setSamples(16);

	QString fileName = QFileDialog::getOpenFileName(0,
		"Open File", "../ObjectFileReader/files", "Object Files (*.obj)");
	TriangleWindow window(fileName);
	window.setFormat(format);
	window.resize(640, 480);
	window.show();

	window.setAnimating(true);

	return app.exec();
}
