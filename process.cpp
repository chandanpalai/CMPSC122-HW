#include "process.h"
#include "device.h"

//  Run a Process for some time
//  The process would like to complete its current CPU burst,
//  but is currently only allowed what is granted to it.
//  Parameters:
//      clock           (modified int)  time on simulation clock
//      allowance       (input int)     time allowed to run with
//      next            (output Device) what device should be used next
//		next is from { disk, net, console, cpu )
//  Post-Condition:
//      the clock will have advanced until either the allowed time
//      has been used up, or the current CPU burst is complete
//      (whichever happens earlier).  There will be no idle CPU time.
//  Also:  The history log for this Process will record what is known
//      at this time

void Process::run( int &clock, int allowance, Device *&next )
{
	addLog(clock, 'X');		// running now
	if (allowance >= remainingTime)
	{
		clock += remainingTime;		// use all the time needed
		next = nextRequest[currentCycle];	// and get ready for next
		currentCycle++;			// to move on
		remainingTime = usages[currentCycle];
	}
	else
	{
		clock += allowance;		// uses the time that was given
		remainingTime -= allowance;	// and save some work for later
		next = &cpu;			// wish to keep running
	}
	if (next == NULL)
		addLog(clock, 'Q');	// all done!
	else
		addLog(clock, '-');	// wait for something
}

Computation::Computation( int id )
{
    myId = id;
    bursts = 10 + rand() % 3;	// several lengthy CPU bursts
    for (int i=0; i < bursts; i++)
    {
        usages[i] = 200 + rand() % 120;
	    nextRequest[i] = &disk;	// some disk activity
    }
    nextRequest[bursts-1] = NULL;	// all done!
}

Download::Download( int id )
{
    myId = id;
    bursts = 9;		// 4 chances to move data, then wrap up
    for (int i=0; i < bursts; i++)
    {
        usages[i] = 40 + rand() % 20;   // not much CPU needed
	if (i%2 == 0)
	    nextRequest[i] = &net;	// alternate network
	else
	    nextRequest[i] = &disk;	// and disk
    }
    nextRequest[bursts-1] = NULL;	// all done!
}

Interact::Interact( int id )
{
    myId = id;
    bursts = 4;		// enough to simulate till others are all done
    for (int i=0; i < bursts; i++)
    {
        usages[i] = 30 + rand() % 20;
	    nextRequest[i] = &console;	// work with console
    }
    nextRequest[bursts-1] = NULL;	// all done!
}
