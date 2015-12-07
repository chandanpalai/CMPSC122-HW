#include "scheduler.h"
#include "device.h"
#include "string"
#include "cmath"

//  Scheduler Simulation
//  Simulates a process scheduler for a collecdtion of tasks
//  Parameters:
//  	tasks		(modified process array)description of tasks to run
//  	arrival		(input int array)	arrival in simulation time
//  	size		(input int)		number of elements in arrays
//  	allowance	(input int)		maximal CPU allowance 
//		(a large 'allowance' simulates First Come First Served)
//  Each Process includes a history of its activity which is
//  pupulated during the simulation and displayed afterwards.
//  
//  The scheduler includes a readySet of processes that are currently
//  wishing to use the CPU, and a future list of events that will
//  occur later in the simulation.  A simulation clock advances to
//  identify which events occur in which order.

void Scheduler::runScheduler(Process *tasks[], int arrival[], int &size)
{
	int pid;			// process wanting action
	int newpid;			// newly arriving process
	char nextAct;		// and the action it wants
	Device *nextDevice;  //points to the device next desired by a process after running
	double turnaround;  //average time it took a process to finish from arrival time to completion
	int endTime;		//time the process finished
	ProcIterator *log_iterator; //used to iterate through process logs
	ProcList *interaction_log = new ProcList(); //logs time between interaction
	int nextTime;		//for use with the avg time between interactions
	double avgInteraction; //average time between interactions
	int largestInteraction; //longest time between two interactions
	int numInteractions;
	int start_end;	//for use in display of response times
	ProcList *interactionTimes = new ProcList(); //will store times between interactions. ProcId and state won't matter, time is amount of time between interactions.

	cout << name << " Scheduler:" << endl << endl;

	for (int i = 0; i < size; i++)
	{
		future.insert(i, arrival[i], 'X');	// all want to run
		tasks[i]->restart();			// and start at beginning
		tasks[i]->addLog(arrival[i], '-');	// might have to wait
	}

	//restart device readyTimes
	disk.restart();
	net.restart();
	console.restart();
	shell.restart();

	clock = 0;		// initialize the clock

	while (!future.empty() || !noneReady())
	{
		if (noneReady())		// nothing avaliable now, but future is not empty
		{				

			clock = future.leadTime();		//next available task
			future.popFront(pid, nextAct);	// get that task
			addProcess(pid);

		}
		else 				// else run with what's already scheduled to run
		{
			chooseProcess(pid);		//removes first process in readySet

			tasks[pid]->run(clock, allowance(), nextDevice); //runs this process

			//  some tasks may have arrived in the meantime, so get those
			while (!future.empty() && clock >= future.leadTime())
			{
				future.popFront(newpid, nextAct);
				addProcess(newpid);

			}

			/*
			This structure determines what to do with process that just ran.
			It either wants to keep using the processor, wants a new device, or is finished running.
			If it doesn't want the cpu, it doesn't need to go back into future yet.
			*/
			if (nextDevice == &cpu )
			{
				addProcess(pid);
			}
			else 
			{
				if (nextDevice == NULL)
				{ 
					//process is finished, no need to do anything
				}
				else if(nextDevice == &shell) //process wants a new device
				{
					shell.request(pid, clock, tasks, future, arrival, size);
				}
				else
				{
					(*nextDevice).request(pid, clock, tasks, future);
				}
			}
		}
	}

	cout << "Performance Stats: " << endl;

	//average turnaround time computation
	//Pre: all processes have finished running, process log is populated
	turnaround = 0;

	for (int j = 0; j < size; j++)
	{
		log_iterator = &(tasks[j]->getLog().begin());
		while (log_iterator->state() != 'Q')
		{
			log_iterator->advance();
		}
		endTime = log_iterator->time();

		turnaround += (endTime - arrival[j]);  //sum durations
	}

	turnaround = turnaround / size; //divide durations for average turnaround
	cout << "Average turnaround time: " << turnaround << endl;

	//Populates log of interaction start and end times across the processes
	for (int k = 0; k < size; k++)
	{
		if (tasks[k]->isInteractive())
		{
			log_iterator = &(tasks[k]->getLog().begin());
			while (log_iterator->state() != 'Q')
			{
				if (log_iterator->state() == 'I')
				{
					(*interaction_log).insert(0, log_iterator->time(), 'S'); //interaction start time
					(*log_iterator).advance();
					(*interaction_log).insert(0, log_iterator->time(), 'E'); //interaction end time
				}
				(*log_iterator).advance();
			}
		}
	}

	(*interaction_log).pushBack(0,0,'Q'); //marks end of log
	(*interaction_log).popFront(pid, nextAct); //removes first element so that log begins with the first end time

	avgInteraction = 0.0; //starts the average at 0
	endTime = 0;
	nextTime = 0;  
	log_iterator = &((*interaction_log).begin()); //use log iterator to parse interaction log
	largestInteraction = 0;
	numInteractions = 0;
	start_end = 0;  //0 is end time, 1 is start time

	while (!(*interaction_log).empty() && (*log_iterator).state() != 'Q')
	{
		if (start_end == 0) //end time
		{
			endTime = (*log_iterator).time();
			start_end = 1;
		}
		else if (start_end == 1) //start time
		{
			nextTime = (*log_iterator).time();
			avgInteraction += (nextTime - endTime);
			numInteractions++;
			(*interactionTimes).insert(0, (nextTime - endTime), 'I');
			if ((nextTime - endTime) > largestInteraction)
				largestInteraction = (nextTime - endTime);
			start_end = 0;
		}

		(*log_iterator).advance();


	}

	if (!(*interaction_log).empty())
		avgInteraction = avgInteraction / numInteractions;

	cout << "Average Response Time: " << avgInteraction << endl;
	cout << "Maximum Response Time: " << largestInteraction << endl;

	(*interactionTimes).pushBack(0, 0, 'Q'); //gives list an easily found end

	//standard dev = sqrt(avg(squares of deviations from the mean))
	log_iterator = &((*interactionTimes).begin());
	double avgDeviation = 0;
	double standardDeviation;

	while ((*log_iterator).state() != 'Q')
	{
		avgDeviation += (((*log_iterator).time() - avgInteraction) * ((*log_iterator).time() - avgInteraction));
		(*log_iterator).advance();
	}

	if (numInteractions == 0)
		avgDeviation = 0;
	else
		avgDeviation = avgDeviation / numInteractions;
	
	standardDeviation = sqrt(avgDeviation);

	cout << "Standard Deviation of Response Time: " << standardDeviation << endl;
	cout << endl;
}