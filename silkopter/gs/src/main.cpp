#include "stdafx.h"
#include "GS.h"
#include <QtWidgets/QApplication>
#include "HAL_Window.h"

int main(int argc, char *argv[])
{
	q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));

    Q_INIT_RESOURCE(res);

	QApplication a(argc, argv);
	QCoreApplication::setOrganizationName("Silkopter");
	QCoreApplication::setOrganizationDomain("silkopter.com");
	QCoreApplication::setApplicationName("Silkopter");

    a.setQuitOnLastWindowClosed(true);

    GS w;
	w.show();
	return a.exec();
}
