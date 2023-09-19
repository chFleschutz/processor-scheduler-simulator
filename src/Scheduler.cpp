//*********************//
// Christoph Fleschutz //
//*********************//

#include "Scheduler.h"

Scheduler::Scheduler()
{
	processCount = 0;
	averageWaitingTime = 0.0f;
}

void Scheduler::inputProcesses()
{
	//Prozesse ueber Konsole einlesen
	//Anzahl an Prozesse eingeben
	cout << "Enter Amount of Processes: ";
	cin >> processCount;
	cout << endl;

	//Werte je Prozess eingeben
	cout << "Enter: r e d " << endl;

	for (int i = 1; i <= processCount; i++)
	{
		int execTime, deadline, readyTime;

		cout << "       ";
		cin >> readyTime;
		cin >> execTime;
		cin >> deadline;

		processorQueue.push_back(Process{ i, readyTime, execTime, deadline, false });
	}

	//Eingegebene Prozesse nochmals ausgeben
	cout << endl;
	displayProcesses();
}

void Scheduler::loadProcesses()
{
	//ProcessNumber, ReadyTime, ExecutionTime, Deadline, Finished
	processorQueue.push_back(Process{ 1, 0, 4, 0, false });
	processorQueue.push_back(Process{ 2, 0, 1, 0, false });
	processorQueue.push_back(Process{ 3, 0, 8, 0, false });
	processorQueue.push_back(Process{ 4, 0, 7, 0, false });
	processorQueue.push_back(Process{ 5, 0, 15, 0, false });

	processCount = processorQueue.size();

	//Prozesse ausgeben
	cout << "Loaded " << processCount << " Processes" << endl;
	displayProcesses();
}

void Scheduler::displayProcesses()
{
	//Ausgabe der Prozesse auf Konsole
	cout << left << setw(12) << "Process-Num" << setw(12) << "Ready-Time" << setw(12) << "Exec-Time" << setw(12) << "Deadline" << setw(12) << "Finished" << endl;
	for (int i = 0; i < processCount; i++)
	{
		cout << setw(12) << processorQueue[i].processNumber << setw(12) << processorQueue[i].readyTime << setw(12) << processorQueue[i].executionTime << setw(12) << processorQueue[i].deadline << setw(12) << processorQueue[i].finished << endl;
	}
}

void Scheduler::selectSchedulingStrategy()
{
	//Scheduling Strategie auswaehlen und starten
	int strategy, system;

	cout << endl;
	cout << "Select Scheduling Strategy" << endl;
	cout << " 1 First-Come-First-Serve" << endl;
	cout << " 2 Shortest-Job-First" << endl;
	cout << " 3 Earliest-Deadline-First" << endl;
	cout << " 4 Least-Laxity-First (2 Processors NP)" << endl;
	cout << " 5 Round-Robin" << endl;
	cout << " > ";
	cin >> strategy;
	cout << endl;

	switch (strategy)
	{
	case 4:
		//Least-Laxity-first mit 2 Prozessoren
		LLF();
		break;

	case 5:
		//Round Robin
		cout << "Enter Quantum: ";
		cin >> system;
		cout << endl;
		RoundRobin(system);
		break;

	default:
		//Liste nach gewaehlter strategie sortieren
		switch (strategy)
		{
		case 2:
			//Shortest-Job-First
			sortSJF();
			break;
		case 3:
			//Earliest-Deadline-First
			sortEDF();
			break;
		default:
			break;
		}

		//Auswahl Preemptiv / Nonpreemtiv und ausfuehren
		cout << "Select System" << endl;
		cout << " 1 Preemptiv" << endl;
		cout << " 2 Non-preemptiv" << endl;
		cout << " > ";
		cin >> system;
		cout << endl;

		//Sortierte Liste auswerten
		switch (system)
		{
		case 1:
			//Preemptive
			calcPreemptiv();
			break;
		case 2:
			//Non-Preemptiv
			calcNonPreemptiv();
			break;
		default:
			break;
		}
		break;
	}
}

void Scheduler::outputAverageWaitingTime()
{
	//Durchschnittliche Wartezeit auf Konsole ausgeben
	cout << "Average Waiting-Time: " << averageWaitingTime << endl << endl;
}

void Scheduler::sortSJF()
{
	//Queue nach Bedienzeit sortieren (Bubblesort)
	int i, j;
	for (i = 0; i < processCount - 1; i++)
	{
		for (j = processCount - 2; j >= i; j--)
		{
			if (processorQueue[j].executionTime > processorQueue[j + 1].executionTime)
			{
				swap(processorQueue[j], processorQueue[j + 1]);
			}
		}
	}
}

void Scheduler::sortEDF()
{
	//Queue nach Deadlines sortieren (Bubblesort)
	int i, j;
	for (i = 0; i < processCount - 1; i++)
	{
		for (j = processCount - 2; j >= i; j--)
		{
			if (processorQueue[j].deadline > processorQueue[j + 1].deadline)
			{
				swap(processorQueue[j], processorQueue[j + 1]);
			}
		}
	}
}

void Scheduler::LLF()
{
	//Queue nach Least-Laxity (d - r) - e sortieren (Bubblesort)
	int i, j;
	for (i = 0; i < processCount - 1; i++)
	{
		for (j = processCount - 2; j >= i; j--)
		{
			if ((processorQueue[j].deadline - processorQueue[j].readyTime) - processorQueue[j].executionTime >
				(processorQueue[j + 1].deadline - processorQueue[j + 1].readyTime) - processorQueue[j + 1].executionTime)
			{
				swap(processorQueue[j], processorQueue[j + 1]);
			}
		}
	}

	//Durchschnittliche Wartezeit ausrechnen
	int passedTimeP1 = 0;
	int passedTimeP2 = 0;
	int totalWaitTime = 0;
	int finishedProcesses = 0;

	while (finishedProcesses != processCount)
	{
		//Prozess für Prozessor 1 suchen 
		if (passedTimeP1 <= passedTimeP2)
		for (auto it = processorQueue.begin(); it != processorQueue.end();)
		{
			//Prüfen ob Prozess schon beendet ist oder noch nicht bereit
			if (!it->finished && passedTimeP1 >= it->readyTime)
			{
				//Ersten bereiten Prozess ausfuehren
				passedTimeP1 += it->executionTime;
				totalWaitTime += passedTimeP1 - it->readyTime;

				it->finished = true;
				finishedProcesses++;

				break;
			}
			else it++;
		}

		//Prozess für Prozessor 2 suchen
		if (passedTimeP2 < passedTimeP1)
		for (auto it = processorQueue.begin(); it != processorQueue.end();)
		{
			//Prüfen ob Prozess schon beendet ist oder noch nicht bereit
			if (!it->finished && passedTimeP2 >= it->readyTime)
			{
				//Ersten bereiten Prozess ausfuehren
				passedTimeP2 += it->executionTime;
				totalWaitTime += passedTimeP2 - it->readyTime;

				it->finished = true;
				finishedProcesses++;

				break;
			}
			else it++;
		}
	}

	averageWaitingTime = (float)totalWaitTime / (float)processCount;
}

void Scheduler::RoundRobin(int Q)
{
	//Durchschnittliche Wartezeit bei Round-Robin mit Quantum Q berechnen	
	int passedTime = 0;
	int totalWaitTime = 0;
	int finishedProcesses = 0;

	while (finishedProcesses != processCount)
	{
		for (auto it = processorQueue.begin(); it != processorQueue.end(); it++)
		{
			//Pruefen ob Prozess beendet oder noch nicht bereit ist
			if (!it->finished && passedTime >= it->readyTime)
			{
				//Jeden Prozess für Q-Zeiteinheiten rechnen lassen
				if (it->executionTime <= Q)
				{
					passedTime += it->executionTime;
					totalWaitTime += passedTime;

					it->finished = true;
					finishedProcesses++;
				}
				else
				{
					passedTime += Q;
					it->executionTime -= Q;
				}
			}
		}
	}

	averageWaitingTime = (float)totalWaitTime / (float)processCount;
}

void Scheduler::calcPreemptiv()
{
	//Durchschnittliche Wartezeit ausrechnen
	int passedTime = 0;
	int totalWaitTime = 0;
	int finishedProcesses = 0;

	while (finishedProcesses != processCount)
	{
		for (auto it = processorQueue.begin(); it != processorQueue.end();)
		{
			//Pruefen ob Prozess beendet oder noch nicht bereit ist
			if (!it->finished && passedTime >= it->readyTime)
			{
				//Kürzesten Prozess eine Zeiteinheit ausfuehren
				passedTime++;
				it->executionTime--;

				if (it->executionTime == 0)
				{
					totalWaitTime += passedTime - it->readyTime;
					it->finished = true;
					finishedProcesses++;
				}

				//Pointer auf Anfang der Liste setzen
				it = processorQueue.begin();
			}
			else it++;
		}
		//Deadlock vermeidung
		passedTime++;
	}

	averageWaitingTime = (float)totalWaitTime / (float)processCount;
}

void Scheduler::calcNonPreemptiv()
{
	//Durchschnittliche Wartezeit ausrechnen
	int passedTime = 0;
	int totalWaitTime = 0;
	int finishedProcesses = 0;

	while (finishedProcesses != processCount)
	{
		for (auto it = processorQueue.begin(); it != processorQueue.end();)
		{
			//Prüfen ob Prozess schon beendet ist oder noch nicht bereit
			if (!it->finished && passedTime >= it->readyTime)
			{
				//Ersten bereiten Prozess ausfuehren
				passedTime += it->executionTime;
				totalWaitTime += passedTime - it->readyTime;

				it->finished = true;
				finishedProcesses++;

				//Zeiger wieder auf Anfang der Liste setzen
				it = processorQueue.begin();
			}
			else it++;
		}
		//Deadlock vermeidung
		passedTime++;
	}

	averageWaitingTime = (float)totalWaitTime / (float)processCount;
}
