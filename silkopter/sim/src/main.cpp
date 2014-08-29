#include "stdafx.h"
#include "Simulator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setOrganizationName("Silkopter Simulator");
	QCoreApplication::setOrganizationDomain("silkopter.com");
	QCoreApplication::setApplicationName("Silkopter Simulator");

	Simulator w;
	w.show();
	return a.exec();
}
