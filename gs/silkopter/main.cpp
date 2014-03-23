#include "stdafx.h"
#include "Silkopter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Silkopter w;
	w.show();
	return a.exec();
}
