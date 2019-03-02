#include "FSMLab.h"

#include <FiniteStateMachine/FiniteStateMachine.h>
#include <FiniteStateMachine/Utils.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>

namespace
{
	const QUrl SERVER_URL = "http://localhost:8070";
}

FSMLab::FSMLab(QWidget *parent)
	: QMainWindow { parent }
	, manager { new QNetworkAccessManager(this) }
{
	ui.setupUi(this);
	connect(manager, &QNetworkAccessManager::finished, this, &FSMLab::ParseSolution);

	connect(ui.mp_del_transition, &QAbstractButton::clicked, [this]()
	{
		ui.mp_transitions->removeRow(ui.mp_transitions->currentRow());
	});
	
	connect(ui.mp_add_transition, &QAbstractButton::clicked, [this]()
	{
		auto n = ui.mp_transitions->rowCount();
		ui.mp_transitions->setRowCount(n + 1);

		auto from = new QSpinBox;
		auto to   = new QSpinBox;
		auto chr  = new QLineEdit;

		chr->setPlaceholderText(FSM_EPS);

		ui.mp_transitions->setCellWidget(n, 0, from);
		ui.mp_transitions->setCellWidget(n, 1, chr);
		ui.mp_transitions->setCellWidget(n, 2, to);
	});

	connect(ui.mp_start, &QAbstractButton::clicked, [this]()
	{
		QJsonObject obj;

		obj.insert("query", "xwyw");
		obj.insert("w", ui.mp_w->text());

		QJsonObject fsm_obj;
		fsm_obj.insert("number_of_states", ui.mp_num_states->value());
		fsm_obj.insert("initial_state", ui.mp_initial_state->value() - 1);

		auto final_states_lst = ui.mp_final_states->text().split(",");
		QJsonArray final_states;
		std::transform(std::begin(final_states_lst), std::end(final_states_lst), std::back_inserter(final_states), [](auto el)
		{
			return el.trimmed().toInt() - 1;
		});
		fsm_obj.insert("final_states", final_states);

		QJsonArray transitions;
		for (int i = 0; i < ui.mp_transitions->rowCount(); ++i)
		{
			auto from = ui.mp_transitions->cellWidget(i, 0);
			auto chr = ui.mp_transitions->cellWidget(i, 1);
			auto to = ui.mp_transitions->cellWidget(i, 2);

			auto cur_char = qobject_cast<QLineEdit*>(chr)->text().length() > 0 ? qobject_cast<QLineEdit*>(chr)->text()[0] : FSM_EPS;

			QJsonObject transition;
			transition.insert("from", qobject_cast<QSpinBox*>(from)->value() - 1);
			transition.insert("to", qobject_cast<QSpinBox*>(to)->value() - 1);
			transition.insert("char", cur_char.unicode());

			transitions.push_back(transition);
		}
		fsm_obj.insert("transitions", transitions);
		obj.insert("fsm", fsm_obj);

		auto data = QJsonDocument(obj).toJson();

		QNetworkRequest request(SERVER_URL);
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
		request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.size()));

		ui.mp_solution->setText(u8"Очікуємо відповідь від сервера...");
		manager->post(request, data);

		/*
		FiniteStateMachines::FiniteStateMachine fsm(ui.mp_num_states->value());
		fsm.SetInitialState(ui.mp_initial_state->value() - 1);

		for(int i = 0; i < ui.mp_transitions->rowCount(); ++i)
		{
			auto from = ui.mp_transitions->cellWidget(i, 0);
			auto chr = ui.mp_transitions->cellWidget(i, 1);
			auto to = ui.mp_transitions->cellWidget(i, 2);

			auto cur_char = qobject_cast<QLineEdit*>(chr)->text().length() > 0 ? qobject_cast<QLineEdit*>(chr)->text()[0] : FSM_EPS;

			fsm.AddTransition(qobject_cast<QSpinBox*>(from)->value() - 1,
				qobject_cast<QSpinBox*>(to)->value() - 1,
				cur_char);
		}

		auto final_states = ui.mp_final_states->text().split(",");
		for(auto final_state : final_states)
		{
			fsm.MarkStateAsFinal(final_state.trimmed().toInt() - 1);
		}

		auto w = ui.mp_w->text();

		auto solution = FiniteStateMachines::Utils::CanProduceWordWithTwiceRepeatedW(fsm, w);
		if(solution.isEmpty())
		{
			ui.mp_solution->setText(u8"Неможливо вивести рядок вигляду xwyw :(");
		}
		else
		{
			ui.mp_solution->setText(u8"Один із можливих рядків вигляду xwyw - " + solution);
		}*/
	});
}

void FSMLab::ParseSolution(QNetworkReply* reply)
{
	if(reply->error() == QNetworkReply::NoError)
	{
		QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
		if(obj.value("status") != "OK")
		{
			ui.mp_solution->setText(QString(u8"Сталася помилка. %1").arg(obj.value("status").toString()));
		}
		else
		{
			QString solution = obj.value("solution").toString();
			if (solution.isEmpty())
			{
				ui.mp_solution->setText(u8"Неможливо вивести рядок вигляду xwyw :(");
			}
			else
			{
				ui.mp_solution->setText(u8"Один із можливих рядків вигляду xwyw - " + solution);
			}
		}
	}
	else
	{
		ui.mp_solution->setText(QString(u8"Сталася помилка. %1").arg(reply->error()));
	}
}
