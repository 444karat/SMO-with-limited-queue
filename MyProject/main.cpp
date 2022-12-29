#include <iostream>
#include "smo_events_aggregator.h"
#include "smo_event.h"
#include "service_channels.h"

int main(){
		
	std::cout << operating_characteristic_numerical(4,8) << std::endl;
	std::cout << operating_characteristic_analytical(4,8);
}
