#include <QtWidgets/QApplication>
#include<mainwindow.h>


/**
 * @brief The main function of the program.
 *
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 *
 * @return The exit code of the program.
 */
int main(int argc, char* argv[]) {

	
	
	QApplication app(argc, argv);

	MainWindow mainWindow;
	mainWindow.show();
	
	return app.exec();
}

