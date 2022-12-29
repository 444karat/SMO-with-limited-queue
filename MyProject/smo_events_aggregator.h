#ifndef SMO_EVENTS_AGGREGATOR_H
#define SMO_EVENTS_AGGREGATOR_H
#include<iostream>
#include<cmath>
#include<vector>
#include<random>
#include <functional>

#include "stat_mod.h"
#include "smo_event.h"
#include "service_channels.h"

class smo_events_aggregator {
public:
	smo_events_aggregator();
	void add_request(double begin_time, double end_time);
	double get_state_total_time(int state);
	int get_state_at_time(double time);
	~smo_events_aggregator();
private:
	std :: vector <smo_event*> events;
	void add_event(double time, int state_change_factor);
};

#endif

