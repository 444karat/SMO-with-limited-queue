#include "smo_events_aggregator.h"

smo_events_aggregator::smo_events_aggregator() {
}


void smo_events_aggregator::add_event(double time, int state_change_factor){
	smo_event* e = new smo_event(time, state_change_factor);
	int size = events.size();
	int insert_index = size;
	for(int i = 0; i < size; i++)
	{
		if(events[i]-> time > time)
		{
			insert_index = i;
			break;
		}
	}
	events.insert(events.begin() + insert_index, e);
}
void smo_events_aggregator::add_request(double begin_time, double end_time){
	add_event(begin_time, 1);
	add_event(end_time, -1);
}
double smo_events_aggregator::get_state_total_time(int state){
	int count = 0;
	double prev_event_time = 0;
	double state_total_time = 0;
	for(smo_event* e : events)
	{
		if(count == state)
		{
			state_total_time += (e-> time - prev_event_time);
		}
		count += e-> state_change_factor;
		prev_event_time = e-> time;
	}
	return state_total_time;
}
int smo_events_aggregator::get_state_at_time(double time){
	int state = 0;
	for(smo_event* e : events)
	{
		if(e-> time > time)
		{
			break;
		}
		state += e-> state_change_factor;
	}
	return state;
}

smo_events_aggregator::~smo_events_aggregator() {
	for(int i = 0; i < events.size(); i++)
	{
		delete events[i];
	}
	events.clear();
}

