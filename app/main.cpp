// main.cpp
//
// ICS 46 Winter 2019
// Project #4: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.

#include "Digraph.hpp"
#include "InputReader.hpp"
#include "RoadMap.hpp"
#include "RoadMapReader.hpp"
#include "RoadSegment.hpp"
#include "Trip.hpp"
#include "TripMetric.hpp"
#include "TripReader.hpp"
#include <map>
#include <cmath>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <iomanip>

// time calculations
void za_warudo_toki_wo_tamare(float toki)
{
	// uncalcalculated hour
	double hr = toki;
	// minutes calculated from hour
	double min = toki*60;

	// floored hour
	double fHour = floor(hr);
	// floored minute
	double fMin = floor((hr-fHour)*60);
	// calculated seconds remainder
	double fSec = (((hr-fHour)*60)-fMin)*60;

	// if hour isn't 0
	if (hr >= 1)
	{
		std::cout << std::fixed << std::setprecision(0) << hr << " hours " << std::fixed << std::setprecision(0) << fMin << " minutes " << std::fixed << std::setprecision(2) << fSec << " seconds";
	}
	// if hour is 0 and min isnt 0
	else if (!(hr >= 1) && min >= 1)
	{
		std::cout << std::fixed << std::setprecision(0) << fMin << " min " << std::fixed << std::setprecision(2) << fSec << " seconds";
	}
	// else just use seconds
	else
	{
		std::cout << std::fixed << std::setprecision(2) << fSec << " seconds";
	}
}


// struct to keep track of street + metric
struct vertigo_tech
{
	// street name
	std::string OnevOneMeHereFoo;
	// miles
	double whyHaventWeSwitched2MetricSystemYet;
	// mph // could be blank
	double stillWaitingOnMetrics;
	// time // could be blank
	double TimeIsRelative;
};

// Dijkstra Algorithm input weight for distance
double Paff(const RoadSegment& roadkill)
{
	// return the miles from road segment
	return roadkill.miles;
}

// Dijkstra Algorithm input weight for time
double Timm(const RoadSegment& roadkill)
{
	// return hour // mi/hr / mi = hr
	return (roadkill.miles / roadkill.milesPerHour);
}

int main()
{
	// Input stream
	InputReader InTheZone = InputReader(std::cin);
	
	// Map
	RoadMapReader WhoNeedsAMap;
	
	// Actual Map
	RoadMap Mappo = WhoNeedsAMap.readRoadMap(InTheZone);
	
	// Trip
	TripReader DontTripBruh;
	
	// Actual Trips
	std::vector<Trip> WhyUTrippingBro = DontTripBruh.readTrips(InTheZone);
	
	// Iterate through the trips
	for (std::vector<Trip>::iterator dirks = WhyUTrippingBro.begin(); dirks != WhyUTrippingBro.end(); ++dirks)
	{
		// vector to iterate through to get struct of street name + metric
		std::vector<vertigo_tech> Trippin;
		std::map<int, int> pathfinder;
		// total distance
		double HowMuchURun = 0;
		// total time
		double HowFastUGoing = 0;
		// Starting point of route
		int start = dirks->startVertex;
		// end point of route
		int end = dirks->endVertex;

		// Distance condition
		if (dirks->metric == TripMetric::Distance)
		{
			// Print start
			std::cout << "Shortest distance from " << Mappo.vertexInfo(start) << " to " << Mappo.vertexInfo(end) << ":" << std::endl;

			// Dijkstra map for distance
			pathfinder = Mappo.findShortestPaths(dirks->startVertex, Paff);

			std::cout << "	Begin at " << Mappo.vertexInfo(start) << std::endl;

			// Back track the algorithm to find shortest path
			while (end != start)
			{
				// Total distance
				HowMuchURun += Mappo.edgeInfo(pathfinder[end], end).miles;
				// push the struct of information for each edge
				// only street + miles
				Trippin.push_back(vertigo_tech{
					.OnevOneMeHereFoo = Mappo.vertexInfo(end), 
					.whyHaventWeSwitched2MetricSystemYet = Mappo.edgeInfo(pathfinder[end], end).miles});
				// make end equal to its predecessor
				end = pathfinder[end];
			}
			// printing the vector backwards because it was pushed in backwards
			for (std::vector<vertigo_tech>::reverse_iterator itr = Trippin.rbegin(); itr != Trippin.rend(); ++itr)
			{
				std::cout << "	Continue to " << itr->OnevOneMeHereFoo << " (" << itr->whyHaventWeSwitched2MetricSystemYet << " miles)" << std::endl;
			}
			// printing total distance
			std::cout << "Total distance: " << HowMuchURun << " miles" << std::endl;
		}
		// Time Condition by default
		else
		{
			// Print Start
			std::cout << "Shortest driving time from " << Mappo.vertexInfo(start) << " to " << Mappo.vertexInfo(end) << ":" << std::endl;
			
			// Dijkstra Map for Time
			pathfinder = Mappo.findShortestPaths(dirks->startVertex, Timm);

			std::cout << "	Begin at " << Mappo.vertexInfo(start) << std::endl;

			// Back track the algorithm to find shortest timed path
			while (end != start)
			{
				// total time
				HowFastUGoing += ((Mappo.edgeInfo(pathfinder[end], end).miles)/Mappo.edgeInfo(pathfinder[end], end).milesPerHour);
				// push the struct of information for each edge
				// street + miles + mph + time
				Trippin.push_back(vertigo_tech{
					.OnevOneMeHereFoo = Mappo.vertexInfo(end), 
					.whyHaventWeSwitched2MetricSystemYet = Mappo.edgeInfo(pathfinder[end], end).miles,
					.stillWaitingOnMetrics = Mappo.edgeInfo(pathfinder[end], end).milesPerHour,
					.TimeIsRelative = ((Mappo.edgeInfo(pathfinder[end], end).miles)/Mappo.edgeInfo(pathfinder[end], end).milesPerHour)});
				// make end equal to its predecessor
				end = pathfinder[end];				
			}

			// printing the vector backwards because it was pushed in backwards
			for (std::vector<vertigo_tech>::reverse_iterator itr = Trippin.rbegin(); itr != Trippin.rend(); ++itr)
			{
				std::cout << "	Continue to " << itr->OnevOneMeHereFoo << " (" << itr->whyHaventWeSwitched2MetricSystemYet << " miles @ " << 
				itr->stillWaitingOnMetrics << "mph = ";
				za_warudo_toki_wo_tamare(itr->TimeIsRelative);
				std::cout << ")" << std::endl;
			}
			// printing total distance
			std::cout << "Total time: ";
			za_warudo_toki_wo_tamare(HowFastUGoing);
			std::cout << std::endl;
		}
		// new line to separate trips
		std::cout << std::endl;

	}



    return 0;
}

