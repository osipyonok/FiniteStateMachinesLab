#pragma once

#include <httprequesthandler.h>

using namespace stefanfrings;

class FsmRequestHandler 
	: public HttpRequestHandler
{
//	Q_OBJECT
	Q_DISABLE_COPY(FsmRequestHandler)

public:
	FsmRequestHandler(QObject *parent = Q_NULLPTR);

	~FsmRequestHandler() = default;

	void service(HttpRequest& request, HttpResponse& response) override;
};