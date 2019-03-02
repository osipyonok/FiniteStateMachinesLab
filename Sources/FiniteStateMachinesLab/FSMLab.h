#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FSMLab.h"

class QNetworkReply;
class QNetworkAccessManager;

class FSMLab : public QMainWindow
{
	Q_OBJECT

public:
	FSMLab(QWidget *parent = Q_NULLPTR);

private slots:
	void ParseSolution(QNetworkReply *reply);

private:
	Ui::FSMLabClass ui;
	QNetworkAccessManager *manager;
};
