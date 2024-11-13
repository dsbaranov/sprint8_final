#include "transport_catalogue.h"

void TransportCatalogue::AddStop(Stop &&stop)
{
  stops_.push_back(std::move(stop));
  Stop *target = &stops_.back();
  stopname_to_stop_[target->name] = target;
}

Stop *TransportCatalogue::FindStop(std::string_view name) const
{
  if (stopname_to_stop_.find(name) == stopname_to_stop_.end())
  {
    return nullptr;
  }
  return stopname_to_stop_.at(name);
}

void TransportCatalogue::AddBus(Bus &&bus)
{
  busses_.push_back(std::move(bus));
  Bus *target = &busses_.back();
  busname_to_bus_[target->name] = target;
  for (Stop *stop : target->stops)
  {
    stop_to_busses_[stop].insert(target->name);
  }
}
Bus *TransportCatalogue::FindBus(std::string_view name) const
{
  if (busname_to_bus_.find(name) == busname_to_bus_.end())
  {
    return nullptr;
  }
  return busname_to_bus_.at(name);
}

std::set<std::string_view> TransportCatalogue::FindBusesByStop(Stop *stop) const
{
  if (stop_to_busses_.find(stop) == stop_to_busses_.end())
  {
    return {};
  }
  return stop_to_busses_.at(stop);
};
