#include <QtCore/QCoreApplication>
#include <httplistener.h>

#include <QDebug>
#include "RequestHandler.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QSettings settings(QSettings::Scope::UserScope, "KNU", "FSMLab server", &a);
	settings.setValue("host", "localhost");
	settings.setValue("port", 8070);

	using namespace  stefanfrings;

	new HttpListener(&settings, new FsmRequestHandler(&a), &a);

	return a.exec();
}
