#include "service_channels.h"


service_channels::service_channels(int count) {
	for (int channel = 0; channel < count; channel++)
	{
		channels_service_end_times.push_back(0);
	}
	channels_count = count;
}
double service_channels::get_service_end_time(int channel_index){
	return channels_service_end_times[channel_index];
}
int service_channels::get_closest_service_end_time_channel(){
	double closest_time = INFINITY;
	int channel_index = 0;
	for (int i = 0; i < channels_count; i++)
	{
		if (channels_service_end_times[i] < closest_time) {
			closest_time = channels_service_end_times[i];
			channel_index = i;
		}
	}
	return channel_index;
}
void service_channels::update_channel_service_end_time(int channel_index, double new_time){
	channels_service_end_times[channel_index] = new_time;
}
service_channels::~service_channels() {
}
