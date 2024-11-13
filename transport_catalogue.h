#pragma once

#include <algorithm>
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
struct Stop
{
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
  Stop &operator=(Stop &&other)
  {
    name = std::move(other.name);
    coordinates = std::move(other.coordinates);
    return *this;
  }
};

struct Bus
{
  std::string name;
  std::vector<Stop *> stops;

  Bus() = default;
  Bus(std::string &&_name, std::vector<Stop *> &&_stops)
      : name(move(_name)),
        stops(std::move_iterator(_stops.begin()),
              std::move_iterator(_stops.end()))
  {
    _stops.clear();
  }
  Bus(Bus &other)
      : name(other.name), stops(other.stops.begin(), other.stops.end()) {}
  Bus(const Bus &other)
      : name(other.name), stops(other.stops.begin(), other.stops.end()) {}
  Bus(Bus &&other)
      : name(std::exchange(other.name, std::string())),
        stops(std::move_iterator(other.stops.begin()),
              std::move_iterator(other.stops.end()))
  {
    other.stops.clear();
  }
  Bus &operator=(Bus &&other)
  {
    name = std::exchange(other.name, std::string());
    std::move(other.stops.begin(), other.stops.end(), stops.begin());
    other.stops.clear();
    return *this;
  }
};

struct DistancesHasher
{
  size_t operator()(std::pair<Stop *, Stop *> stops) const
  {
    return reinterpret_cast<size_t>(stops.first) +
           reinterpret_cast<size_t>(stops.second) * 17;
  }
};

class TransportCatalogue
{
public:
  void AddStop(Stop &&stop);

  Stop *FindStop(std::string_view name) const;

  void AddBus(Bus &&bus);

  Bus *FindBus(std::string_view name) const;

  double FindDistance(const std::pair<Stop *, Stop *> target) const;

private:
  std::deque<Stop> stops_;
  std::deque<Bus> busses_;
  std::unordered_map<std::string_view, Stop *> stopname_to_stop_;
  std::unordered_map<std::string_view, Bus *> busname_to_bus_;
  std::unordered_map<std::pair<Stop *, Stop *>, double, DistancesHasher>
      distances_;
};
