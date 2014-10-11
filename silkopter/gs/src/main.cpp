#include "stdafx.h"
#include "GS.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));

	QApplication a(argc, argv);
	QCoreApplication::setOrganizationName("Silkopter");
	QCoreApplication::setOrganizationDomain("silkopter.com");
	QCoreApplication::setApplicationName("Silkopter");

    a.setQuitOnLastWindowClosed(true);

    GS w;
	w.show();
	return a.exec();
}
