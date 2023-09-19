//*********************//
// Christoph Fleschutz //
//*********************//

#include <iostream>
#include <conio.h>
#include "Scheduler.h"

using namespace std;

int main()
{
	while (true)
	{
		//Prozesse mit Zeiten einlesen
		Scheduler scheduler;
		scheduler.inputProcesses();
		//scheduler.loadProcesses();

		//Strategie auswählen
		scheduler.selectSchedulingStrategy();

		//Durschnittliche Wartezeit ausgeben
		scheduler.outputAverageWaitingTime();

		cout << endl << "Press any key to continue ";
		_getch();
		system("cls");
	}
	return 0;
}
