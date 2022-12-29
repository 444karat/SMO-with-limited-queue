#ifndef STAT_MOD_H
#define STAT_MOD_H

#include<iostream>
#include<cmath>
#include<vector>
#include<random>
#include <functional>

#include "smo_events_aggregator.h"
#include "smo_event.h"
#include "service_channels.h"


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution <> urd(0, 1);

double uniform()
{
	return urd(gen);
}
double exp_random_value(double lambda)
{
	return(-1.0 / lambda) * log(uniform());
}
std :: vector <double> get_request_times(double modeling_time, double lambda)
{
	std :: vector <double> request_times;
	double time = exp_random_value(lambda);
	do
	{
		request_times.push_back(time);
		time += exp_random_value(lambda);
	} while(time < modeling_time);
	return request_times;
}

double smo_modeling_process(int channels_count,
							std :: vector <double> request_times,
							std :: function <double()> get_service_time,
							int	max_queue_length,
							double modeling_time, 
							int smo_state)
{
	service_channels channels(channels_count);
	smo_events_aggregator queue;
	smo_events_aggregator smo;
	for(double request_time : request_times)
	{
		double request_exit_time = request_time;
		int channel = channels.get_closest_service_end_time_channel();
		double closest_service_end_time = channels.get_service_end_time(
																		channel);
		if(closest_service_end_time < request_time)
		{
			double service_time = get_service_time();
			request_exit_time = request_time + service_time;
			channels.update_channel_service_end_time(channel,
													 request_exit_time);
		}
		else if(queue.get_state_at_time(request_time) < max_queue_length)
		{
			
			queue.add_request(request_time, closest_service_end_time);
			double service_time = get_service_time();
			request_exit_time = closest_service_end_time + service_time;
			channels.update_channel_service_end_time(channel,
													 request_exit_time);
		}
		
		if(request_time < request_exit_time)
		{
			smo.add_request(request_time, request_exit_time);
		}
	}
	return smo.get_state_total_time(smo_state) / modeling_time;
}

const int MIN_ITER_COUNT = 50;
double expected_value(std :: function <double()> rand_distr, double precision,
					  int min_iter_count = MIN_ITER_COUNT)
{
	double sum = 0;
	double square_sum = 0;
	double n = 0;
	bool is_precision_obtained = false;
	while(!is_precision_obtained)
	{
		double r = rand_distr();
		sum += r;
		square_sum += (r * r);
		n++;
		if(n < min_iter_count){
			continue;
		}
		double dispersion = (1.0 / (n - 1)) * square_sum -(1.0 / (n *(n -
																	  1))) * sum * sum;
		double estimated_iter_count = 9 * dispersion / (precision *
														precision);
		
		is_precision_obtained = (n > estimated_iter_count);
	}
	return sum / n;
}

const double n = 3;
const double N = 7;

double operating_characteristic_numerical(int lambda, int m) {
	double mu = 60.0/ m;
	auto get_service_time = [&]() -> double
	{
		return exp_random_value(mu);
	};
	auto get_smo_state_prob = [&]() -> double
	{
		double modeling_time = 100;
		auto request_times = get_request_times(modeling_time , lambda);
		double sum1 = 0.0;
		double sum2 = 0.0;
		for(int j = 1; j < n; j++){
			sum1 += (double)j* smo_modeling_process(n ,
													request_times ,
													get_service_time ,
													N -n ,
													modeling_time ,
													j);
		}
		for(int j = n; j <= N; j++){
			sum1 +=  smo_modeling_process(n ,
										  request_times ,
										  get_service_time ,
										  N -n ,
										  modeling_time ,
										  j);
		}
		return sum1+(double)n* sum2;
	};
	return expected_value(get_smo_state_prob, 0.01);
}

double operating_characteristic_analytical(int lambda, int m)
{
	double mu = 60.0 / m;
	double ro = lambda / mu;
	double sum = 0.0;
	for (int k = 0; k < n; k++) {
		sum += pow(ro, k) / tgamma(k+1);
	}
	sum += pow(ro, n) * (1 - pow((ro / n), N - n - 1)) / tgamma(n + 1) / (1 - ro / n);
	double p0 = 1.0 / sum;
	
	sum = 0.0;
	for(int k = 1; k < n; k++){
		sum += pow(ro,k) / tgamma(k);
	}
	sum += pow(ro, n) * (1 - pow((ro / n), N - n - 1)) / tgamma(n) / (1 - ro / n);
	
	return sum* p0;
}

#endif
