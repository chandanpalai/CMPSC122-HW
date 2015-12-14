#include "device.h"
#include "process.h"

//  Simulate a request to an I/O device
//  Parameters:
//      pid	(input int)		which process is making the request
//      clock	(input int)		time at which request is made
//      tasks	(modified array)	Process information (to record events)
//      future	(modified ProcList)	record when operation is complete
//      arrival (modified array)  added for consistency will Console call of this method
//		size    (modified int)   added for consistency will Console call of this method
void Device::request( int pid, int clock, Process *tasks[], ProcList &future)
{
	if (clock >= readyTime) //device can be accessed now
	{
		tasks[pid]->addLog(clock, action); //process begins using device
		readyTime = clock + duration; //updates the next time the device is available
		future.insert(pid, readyTime, 'X'); // process is done using device and ready to run
		tasks[pid]->addLog(readyTime, '-'); //process is waiting
	}
	else //device not ready
	{
		tasks[pid]->addLog(readyTime, action); //logs time when it uses the device
		readyTime += duration;
		future.insert(pid, readyTime, 'X'); //inserts back into future with post device-use time
		tasks[pid]->addLog(readyTime, '-');  //logs the process as waiting post device-use
	}
}

//  Simulate a request to a user to launch new processes
//  Parameters:
//      pid	(input int)		which process is making the request
//      clock	(input int)		time at which request is made
//      tasks	(modified array)	Process information (to record events)
//      future	(modified ProcList)	record when operation is complete
//      arrival (modified array)  arrival times need to be added to accompany new processes
//		size    (modified int)   size of process and arrival times arrays need to be modified
void Device::request(int pid, int clock, Process *tasks[], ProcList &future, int arrival[], int &size)
{
	if (size < 20)
	{
		tasks[pid]->addLog(clock, 'I'); //shell begins interacting with user
		int select = rand() % 4; //used to choose which process to create. random number allows this simulation to be more dynamic

		if (clock + duration < console.readyTime || select == 3) //console in use, or user not interested in launching new process
		{
			clock = clock + duration;
			tasks[pid]->addLog(clock, '-');
			future.insert(pid, clock, 'X');
		}
		else if (select == 2) //user wants to create a download
		{
			if (size < 3) //won't be more than 1 existing download
			{
				tasks[size] = new Download(size);
				arrival[size] = console.readyTime;
				tasks[size]->restart();
				tasks[size]->addLog(arrival[size], '-');
				future.insert(size, arrival[size], 'X');
				clock = console.readyTime + 1;
			}
			else if (!(tasks[size - 1]->isDownload()) && !(tasks[size - 2]->isDownload())) //not likely to launch three downloads in a row
			{
				tasks[size] = new Download(size);
				arrival[size] = console.readyTime;
				tasks[size]->restart();
				tasks[size]->addLog(arrival[size], '-');
				future.insert(size, arrival[size], 'X');
				clock = console.readyTime + 1;
			}
			else
				clock = clock + duration;
			
		}
		else if (select == 1) //user wants to create a computation
		{
			if (size < 3) //won't be more than 1 existing computation
			{
				tasks[size] = new Computation(size);
				arrival[size] = console.readyTime;
				tasks[size]->restart();
				tasks[size]->addLog(arrival[size], '-');
				future.insert(size, arrival[size], 'X');
				clock = console.readyTime + 1;
			}
			else if (!(tasks[size - 1]->isComputation()) && !(tasks[size - 2]->isComputation())) //not likely to launch three computations in a row
			{
				tasks[size] = new Computation(size);
				arrival[size] = console.readyTime;
				tasks[size]->restart();
				tasks[size]->addLog(arrival[size], '-');
				future.insert(size, arrival[size], 'X');
				clock = console.readyTime + 1;
			}
			else
				clock = clock + duration;
		}
		else if (select == 0) //user wants to create interactive process, will only happen when the console is available 
		{	
				tasks[size] = new Interact(size);
				arrival[size] = console.readyTime;
				tasks[size]->restart();
				tasks[size]->addLog(arrival[size], '-');
				future.insert(size, arrival[size], 'X');
				clock = console.readyTime + 1;
		}
		
		future.insert(pid, clock, 'X');
		size++;
	}
	else //can't add anymore processes, behaves as if user doesn't want to add any processes
	{
		clock = clock + duration;
		tasks[pid]->addLog(clock, '-');
		future.insert(pid, clock, 'X');
	}
}

//  Each device records a letter to be used in the summary display
//  and the time the operation is expected to take
Device disk('D', 200), 	// disk has 'slow' moving parts
net('N', 100), 	// networks are faster nowadays
cpu('X', 0), // not external, but used for type compatibility
console('I', 1000), shell('I', 7000); //user might take a couple seconds to choose what to do next	
//  When each process runs with the CPU, it will use this list to identify
//  what it wishes to do next (using the cpu object to continue running)

