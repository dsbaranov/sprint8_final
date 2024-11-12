#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <deque>
#include "geo.h"
#include <utility>
/**
 * Структура остановки.
 * @param name string - имя остановки
 * @param latitude double - широта остановки
 * @param longitude double - долгота остановки
 * @note Сущность предназначена для хранения параметров одной остановки
 */
struct Stop
{
	std::string name;
	Coordinates coordinates;
};

struct Bus
{
	std::string name;
	std::vector<Stop *> stops;
};

struct DistancesHasher
{
	size_t operator()(std::pair<Stop *, Stop *> stops)
	{
		return hasher_(reinterpret_cast<size_t>(stops.first) + reinterpret_cast<size_t>(stops.second));
	}

private:
	std::hash<size_t> hasher_;
};

class TransportCatalogue
{
public:
	void AddStop(const std::string &name, const Coordinates &coordinates)
	{
		stops_.push_back({std::move(name), std::move(coordinates)});
		stopname_to_stop_[stops_.back().name] = &stops_.back();
	}
	Stop *FindStop(std::string_view name) const
	{
		if (stopname_to_stop_.find(name) == stopname_to_stop_.end())
		{
			throw std::out_of_range("stop doesn't exist");
		}
		return stopname_to_stop_.at(name);
	}

	void AddBus(const std::string &name, std::vector<std::string_view> stops)
	{
		std::vector<Stop *> bus_stops;
		for (std::string_view stop : stops)
		{
			bus_stops.push_back(FindStop(stop));
		}
		busses_.push_back({move(name), move(bus_stops)});
	}

private:
	double CalculateDistance(const Stop *first, const Stop *second)
	{
		
	}

	std::deque<Stop> stops_;
	std::deque<Bus> busses_;
	std::unordered_map<std::string_view, Stop *> stopname_to_stop_;
	std::unordered_map<std::string_view, Bus> busname_to_stops_;
	std::unordered_map<std::pair<Stop *, Stop *>, double, DistancesHasher> distances;
};
