#include "stat_reader.h"

#include <sstream>
#include <unordered_set>
// Bus 256
using namespace std::string_literals;

void ParseAndPrintStat(const TransportCatalogue &transport_catalogue,
                       std::string_view request, std::ostream &output)
{
  size_t first_of_space = request.find_first_of(' ');
  size_t first_not_of_space = request.find_first_not_of(' ');
  size_t last_not_of_space = request.find_last_not_of(' ');
  std::string_view command_name = request.substr(first_not_of_space, first_of_space - first_not_of_space);
  std::string_view command_id = request.substr(
      first_of_space + 1,
      last_not_of_space - first_of_space);
  if (command_name == "Bus"s)
  {
    Bus *target = transport_catalogue.FindBus(command_id);
    output << "Bus "s << command_id << ": "s;
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
        distance += ComputeDistance((*it)->coordinates, (*next(it))->coordinates);
      }
    }
    output << target->stops.size() << " stops on route, "
           << unique_stops.size() << " unique stops, "
           << distance << " route length"s
           << "\n"s;
  }
  else if (command_name == "Stop"s)
  {
    output << "Stop "s << command_id << ": "s;
    Stop *target_stop = transport_catalogue.FindStop(command_id);
    if (target_stop == nullptr)
    {
      output << "not found\n"s;
      return;
    }
    std::set<std::string_view> busses = transport_catalogue.FindBusesByStop(target_stop);
    if (busses.empty())
    {
      output << "no buses\n";
    }
    else
    {
      output << "buses ";
      bool is_first = true;
      for (std::string_view bus : busses)
      {
        if (!is_first)
        {
          output << " "s;
        }
        output << bus;
        is_first = false;
      }
      output << "\n"s;
    }
  }
}