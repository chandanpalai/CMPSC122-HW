#include "device.h"
#include "process.h"

//  Simulate a request to an I/O device
//  Parameters:
//      pid	(input int)		which process is making the request
//      clock	(input int)		time at which request is made
//      tasks	(modified array)	Process information (to record events)
//      future	(modified ProcList)	record when operation is complete
void Device::request( int pid, int clock, Process *tasks[], ProcList &future )
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

//  Each device records a letter to be used in the summary display
//  and the time the operation is expected to take
Device disk('D', 200), 	// disk has 'slow' moving parts
net('N', 100), 	// networks are faster nowadays
cpu('X', 0);
Console	console('I', 1000);		// not external, but used for type compatibility
 
//  When each process runs with the CPU, it will use this list to identify
//  what it wishes to do next (using the cpu object to continue running)

