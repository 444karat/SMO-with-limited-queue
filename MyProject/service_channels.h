#ifndef SERVICE_CHANNELS_H
#define SERVICE_CHANNELS_H
#include<iostream>
#include<cmath>
#include<vector>
#include<random>
#include <functional>

#include "stat_mod.h"
#include "smo_events_aggregator.h"
#include "smo_event.h"

class service_channels {
public:
	service_channels(int count);
	double get_service_end_time(int channel_index);
	int get_closest_service_end_time_channel();
	void update_channel_service_end_time(int channel_index, double new_time);
	~service_channels();
private:
	std::vector <double> channels_service_end_times;
	int channels_count;
};

#endif

