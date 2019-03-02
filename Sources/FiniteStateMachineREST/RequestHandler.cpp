#include "RequestHandler.h"

#include <FiniteStateMachine/FiniteStateMachine.h>
#include <FiniteStateMachine/Utils.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <memory>

using namespace FiniteStateMachines;

namespace
{
	std::shared_ptr<FiniteStateMachine> _ParseFSM(const QJsonObject &fsm_obj)
	{
		std::shared_ptr<FiniteStateMachine> fsm;

		auto n_states = fsm_obj.value("number_of_states");
		if (!n_states.isDouble())
			return nullptr;
		else
			fsm = std::make_shared<FiniteStateMachine>(n_states.toInt());

		auto initial_state = fsm_obj.value("initial_state");
		if (!initial_state.isDouble())
			return nullptr;
		else
			fsm->SetInitialState(initial_state.toInt());

		auto final_states = fsm_obj.value("final_states");
		if(!final_states.isNull())
		{
			if (!final_states.isArray())
				return nullptr;

			for(const auto el : final_states.toArray())
			{
				if (!el.isDouble())
					return nullptr;

				int state = el.toInt();
				if (0 > state || n_states.toInt() <= state)
					return nullptr;

				fsm->MarkStateAsFinal(state);
			}
		}

		auto transitions = fsm_obj.value("transitions");
		if(!transitions.isNull())
		{
			if (!transitions.isArray())
				return nullptr;

			for(const auto el : transitions.toArray())
			{
				if (!el.isObject())
					return nullptr;

				auto transition = el.toObject();
				auto from = transition.value("from");
				auto to	  = transition.value("to");
				auto chr  = transition.value("char");

				if (!from.isDouble() || !to.isDouble() || !chr.isDouble())
					return nullptr;

				fsm->AddTransition(from.toInt(), to.toInt(), QChar(chr.toInt()));
			}
		}

		return fsm;
	}
} //namespace

FsmRequestHandler::FsmRequestHandler(QObject* parent /*= Q_NULLPTR*/)
	: HttpRequestHandler { parent }
{	
}

void FsmRequestHandler::service(stefanfrings::HttpRequest& request, stefanfrings::HttpResponse& response)
{
	if(request.getMethod() != "POST")
	{
		response.setStatus(403);
		response.write("Error, this API supports only HTTP POST requests.", true);
		return;
	}

	if(request.getHeader("content-type") != "application/json")
	{
		response.setStatus(403);
		response.write("Error, this API supports only application/json content type.", true);
		return;
	}

	const QJsonDocument request_document = QJsonDocument::fromJson(request.getBody());
	QJsonObject response_obj;
	qDebug() << request_document.object();
	Q_ASSERT(request_document.isObject());
	const auto query = request_document.object().value("query").toString();
	
	if(query == "ping")
	{
		response.setStatus(200);
		response_obj.insert("status", "OK");
	}
	else if(query == "xwyw")
	{
		auto fsm = _ParseFSM(request_document.object().value("fsm").toObject());
		response.setStatus(200);
		if(!fsm)
		{
			response_obj.insert("status", "FAILED");
		}
		else
		{
			auto w = request_document.object().value("w").toString();
			auto solution = Utils::CanProduceWordWithTwiceRepeatedW(*fsm, w);
			response_obj.insert("status", "OK");
			response_obj.insert("solution", solution);
		}

	}
	else
	{
		response.setStatus(403);
		response.write("Error, unsupported query is given", true);
		return;
	}

	response.write(QJsonDocument(response_obj).toJson(), true);
}

