#ifndef SMO_EVENT_H
#define SMO_EVENT_H

#include "stat_mod.h"
#include "smo_events_aggregator.h"
#include "service_channels.h"

struct smo_event {
	double time;
	int state_change_factor;
	smo_event(double time, int state_change_factor);
};

#endif

