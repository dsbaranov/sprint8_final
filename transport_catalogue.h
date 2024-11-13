#pragma once

#include <deque>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "geo.h"
/**
 * Структура остановки.
 * @param name string - имя остановки
 * @param latitude double - широта остановки
 * @param longitude double - долгота остановки
 * @note Сущность предназначена для хранения параметров одной остановки
 */
struct Stop {
  std::string name;
  Coordinates coordinates;
  Stop() = default;
  // Value ctor
  Stop(std::string _name, Coordinates _coordinates)
      : name(_name), coordinates(_coordinates) {}
  // Copy ctor
  Stop(Stop &other) = delete;
  // Stop(Stop &other) : name(other.name), coordinates(other.coordinates) {}
  // Copy const ctor
  Stop(const Stop &other) = delete;
  // Stop(const Stop &other) : name(other.name), coordinates(other.coordinates)
  // {} Move ctor
  Stop(Stop &&other)
      : name(std::move(other.name)),
        coordinates(std::move(other.coordinates)) {}
  // Move equality
  Stop &operator=(Stop &&other) {
    name = std::move(other.name);
    coordinates = std::move(other.coordinates);
    return *this;
  }
};

struct Bus {
  std::string name;
  std::vector<Stop *> stops;
};

struct DistancesHasher {
  size_t operator()(std::pair<Stop *, Stop *> stops) const {
    return reinterpret_cast<size_t>(stops.first) +
           reinterpret_cast<size_t>(stops.second) * 17;
  }
};

class TransportCatalogue {
 public:
  void AddStop(Stop &&stop) {
    stops_.push_back(std::move(stop));
    Stop *target = &stops_.back();
    stopname_to_stop_[target->name] = target;
    for (auto &second_stop : stopname_to_stop_) {
      double dst =
          ComputeDistance(target->coordinates, second_stop.second->coordinates);
      distances_[std::make_pair(target, second_stop.second)] = dst;
      distances_[std::make_pair(second_stop.second, target)] = dst;
    }
  }
  Stop *FindStop(std::string_view name) const {
    if (stopname_to_stop_.find(name) == stopname_to_stop_.end()) {
      throw std::out_of_range("stop doesn't exist");
    }
    return stopname_to_stop_.at(name);
  }

  void AddBus(const std::string &name, std::vector<std::string_view> stops) {
    std::vector<Stop *> bus_stops;
    for (std::string_view stop : stops) {
      bus_stops.push_back(FindStop(stop));
    }
    busses_.push_back({move(name), move(bus_stops)});
  }

 private:
  double CalculateDistance(const Stop *first, const Stop *second) { return 0; }

  std::deque<Stop> stops_;
  std::deque<Bus> busses_;
  std::unordered_map<std::string_view, Stop *> stopname_to_stop_;
  std::unordered_map<std::string_view, Bus> busname_to_stops_;
  std::unordered_map<std::pair<Stop *, Stop *>, double, DistancesHasher>
      distances_;
};
