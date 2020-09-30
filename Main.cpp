#include <QApplication >
#include "MainWindow.h"

/**
	ENTRY POINT
*/
int main(int argc, char* argv[])
{
	QApplication cGuiApp(argc, argv);

	MainWindow cMainWindow;
	cMainWindow.show();

	return cGuiApp.exec();
}
