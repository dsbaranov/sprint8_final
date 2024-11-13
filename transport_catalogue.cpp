#include "transport_catalogue.h"

void TransportCatalogue::AddStop(Stop &&stop) {
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

Stop *TransportCatalogue::FindStop(std::string_view name) const {
  if (stopname_to_stop_.find(name) == stopname_to_stop_.end()) {
    throw std::out_of_range("stop doesn't exist");
  }
  return stopname_to_stop_.at(name);
}

void TransportCatalogue::AddBus(Bus &&bus) {
  busses_.push_back(std::move(bus));
  Bus *target = &busses_.back();
  busname_to_bus_[target->name] = target;
}
Bus *TransportCatalogue::FindBus(std::string_view name) const {
  if (busname_to_bus_.find(name) == busname_to_bus_.end()) {
    return nullptr;
  }
  return busname_to_bus_.at(name);
}