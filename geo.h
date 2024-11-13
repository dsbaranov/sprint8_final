#pragma once

#include <cmath>
#include <utility>

struct Coordinates {
  double lat;
  double lng;
  bool operator==(const Coordinates& other) const {
    return lat == other.lat && lng == other.lng;
  }
  bool operator!=(const Coordinates& other) const { return !(*this == other); }
  // def
  Coordinates() = default;
  // raw val
  Coordinates(double _lat, double _lng) : lat(_lat), lng(_lng) {}
  // copy
  Coordinates(Coordinates& other) : lat(other.lat), lng(other.lng) {}
  // const copy
  Coordinates(const Coordinates& other) : lat(other.lat), lng(other.lng) {}
  // move
  Coordinates(Coordinates&& other) {
    lat = std::exchange(other.lat, double());
    lng = std::exchange(other.lng, double());
  }

  Coordinates& operator=(Coordinates&& other) {
    lat = std::move(other.lat);
    lng = std::move(other.lng);
    return *this;
  }
};

inline double ComputeDistance(const Coordinates& from, const Coordinates& to) {
  using namespace std;
  if (from == to) {
    return 0;
  }
  static const double dr = 3.1415926535 / 180.;
  return acos(sin(from.lat * dr) * sin(to.lat * dr) +
              cos(from.lat * dr) * cos(to.lat * dr) *
                  cos(abs(from.lng - to.lng) * dr)) *
         6371000;
}