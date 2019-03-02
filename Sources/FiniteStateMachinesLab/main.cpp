#include "FSMLab.h"
#include <QtWidgets/QApplication>

#include <FiniteStateMachine/FiniteStateMachine.h>
#include <FiniteStateMachine/Utils.h>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	FiniteStateMachines::FiniteStateMachine fsm(6);
	fsm.SetInitialState(0);
	fsm.MarkStateAsFinal(5);
	fsm.AddTransition(0, 1, FSM_EPS);
	fsm.AddTransition(0, 2, FSM_EPS);
	fsm.AddTransition(2, 4, 'a');
	fsm.AddTransition(2, 3, 'c');
	fsm.AddTransition(4, 3, 'a');
	fsm.AddTransition(3, 2, 'p');
	fsm.AddTransition(1, 3, 'b');
	fsm.AddTransition(4, 5, 'b');
	fsm.AddTransition(3, 5, 'a');
	fsm.AddTransition(2, 0, 'a');

	FiniteStateMachines::Utils::CanProduceWordWithTwiceRepeatedW(fsm, "a");
	
	FSMLab w;
	w.show();
	return a.exec();
}
