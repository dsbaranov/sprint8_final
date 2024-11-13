#include "stat_reader.h"

#include <sstream>
#include <unordered_set>
// Bus 256
using namespace std::string_literals;

void ParseAndPrintStat(const TransportCatalogue &transport_catalogue,
                       std::string_view request, std::ostream &output)
{
  std::string_view bus_name = request.substr(
      request.find_first_of(' ') + 1,
      request.find_last_not_of(' ') - request.find_first_of(' '));

  Bus *target = transport_catalogue.FindBus(bus_name);
  output << "Bus "s << bus_name << ": "s;
  if (target == nullptr)
  {
    output << "not found\n";
    return;
  }
  std::unordered_set<Stop *> unique_stops(target->stops.begin(),
                                          target->stops.end());
  double distance = 0.;
  for (auto it = target->stops.begin(); it < target->stops.end(); ++it)
  {
    if (std::next(it) != target->stops.end())
    {
      distance += transport_catalogue.FindDistance(std::make_pair(*it, *(it + 1)));
    }
    else
    {
      distance += transport_catalogue.FindDistance(std::make_pair(*it, *target->stops.begin()));
    }
  }
  output << target->stops.size() << " stops on route, "
         << unique_stops.size() << " unique stops, "
         << distance << " route length"s
         << "\n"s;
}