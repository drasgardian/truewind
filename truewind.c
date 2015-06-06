#include <stdio.h>
#include <math.h>
#include <stdbool.h>

double trueWindSpeed(double, double, double);
double trueWindDirection(double, double, double);
bool writeNMEATrueWind(double, double, double, char, char, char);

main() {
  double apparentWindSpeed = 16;
  double boatSpeed = 10;
  double apparentWindDirection = 35;
  char reference = 'T'; // TRUE
  char status = 'A'; // Data valid
  char units = 'K'; // Knots

  writeNMEATrueWind(boatSpeed, apparentWindSpeed, apparentWindDirection, reference, status, units);

  return 0;
}


/**
* returns the true wind speed given boat speed, apparent wind speed and apparent wind direction in degrees
**/
double trueWindSpeed(double boatSpeed, double apparentWindSpeed, double apparentWindDirection) {
  // convert degres to radians
  double apparentWindDirectionRadian = apparentWindDirection * (M_PI/180);

  return pow(pow(apparentWindSpeed*cos(apparentWindDirectionRadian) - boatSpeed,2) + (pow(apparentWindSpeed*sin(apparentWindDirectionRadian),2)), 0.5);
}

/**
* returns the true wind direction given boat speed, apparent wind speed and apparent wind direction in degrees
**/
double trueWindDirection(double boatSpeed, double apparentWindSpeed, double apparentWindDirection) {
  // convert degres to radians
  double apparentWindDirectionRadian = apparentWindDirection * (M_PI/180);

  double twdRadians = (90 * (M_PI/180)) - atan((apparentWindSpeed*cos(apparentWindDirectionRadian) - boatSpeed) / (apparentWindSpeed*sin(apparentWindDirectionRadian)));

  // convert radians back to degrees
  return twdRadians*(180/M_PI);
}

/**
* writes the NMEA string to file
* returns TRUE for success and FALSE for fail.
**/
bool writeNMEATrueWind(double boatSpeed, double apparentWindSpeed, double apparentWindDirection, char reference, char status, char units) {
  double tws = trueWindSpeed(boatSpeed, apparentWindSpeed, apparentWindDirection);
  double twd = trueWindDirection(boatSpeed, apparentWindSpeed, apparentWindDirection);

  printf("$CCMWV,%f,%c,%f,%c,%c\n", twd, reference, tws, units, status);


  return true;
}
