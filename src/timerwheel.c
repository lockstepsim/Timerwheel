// timer module

int absslot = 0; //ongoing slot
int absrot = 0; //ongoing rotation

typedef void (*expiry_action)(int); //used in start_timer function


// timer struct
struct timerID
{
	//int absolute_time;
	struct timerID* prevtimer;
	struct timerID* nexttimer;
	int rotation;
	int slot;
	expiry_action callback;
	int callbackarg;
};

struct timerID* mainring[10] = {0}; //the timing wheel 


//expires the timers
void expiry_processing(struct timerID* texpire)
{
	texpire->callback(texpire->callbackarg);
}

// registers a timer
void start_timer(struct timerID* new, int interval, expiry_action op, int arg)
{
	new->prevtimer = NULL;
	new->nexttimer = NULL;
	new->rotation = absrot + (interval / 10);

	new->slot = ((interval + absslot) % 10);

	new->callback = op;
	new->callbackarg = arg;

	if(mainring[new->slot] == NULL)
	{
		mainring[new->slot] = new;
	}
	else
	{
		mainring[new->slot]->prevtimer = new;
		new->nexttimer = mainring[new->slot];
		mainring[new->slot] = new;
	}
	new = NULL;
}

// deregisters a timer
void stop_timer(struct timerID* tdisable, int execute)
{
	if(execute);
	else
	{
		//printf("\n\n *** timer executed:- rotation :- %d slot :- %d", tdisable->rotation, tdisable->slot);
		expiry_processing(tdisable);
	}

	if(mainring[tdisable->slot] == tdisable)
	{
		if(tdisable->nexttimer == NULL)
		{
			mainring[tdisable->slot] = NULL;
		}
		else
		{
			mainring[tdisable->slot] = tdisable->nexttimer;
			tdisable = NULL;
		}
	}
	else
	{
		tdisable->prevtimer->nexttimer = tdisable->nexttimer;
		tdisable = NULL;
	}
}

// checks the timers that must expire in the timing wheel
void per_tick_bookeeping(int t)
{
	absslot = (absslot + 1) % 10;
	//check timers in current slot, rotation & time
	for(struct timerID* ttimer = mainring[absslot]; ttimer != NULL; ttimer = ttimer->nexttimer)
	{
		if(ttimer->rotation == absrot)
		{
			stop_timer(ttimer, 0);
		}
		
	}
}
