//*********************//
// Christoph Fleschutz //
//*********************//

#pragma once
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class Scheduler
{
public:
	Scheduler();

	void inputProcesses();
	void loadProcesses();
	void displayProcesses();
	void selectSchedulingStrategy();
	void outputAverageWaitingTime();

private:
	void sortSJF();
	void sortEDF();
	
	void LLF();
	void RoundRobin(int Q);

	void calcPreemptiv();
	void calcNonPreemptiv();

	struct Process
	{
		int processNumber;
		int readyTime;
		int executionTime;
		int deadline;
		bool finished;
	};

	vector<Process> processorQueue;
	unsigned int processCount;
	float averageWaitingTime;
};
