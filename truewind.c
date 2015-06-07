#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* see http://www.catb.org/gpsd/NMEA.html
* for a good NMEA reference
*/

double trueWindSpeed(double, double, double);
double trueWindDirection(double, double, double);
bool writeNMEATrueWind(double, double, double, char, char, char);

main() {
  double apparentWindSpeed = 16;
  double boatSpeed = 10;
  double apparentWindDirection = 35;
  char reference = 'T'; // TRUE
  char status = 'A'; // Data valid
  char units = 'N'; // Knots


  char nmeaSentence[100];
  int f;

  char nmeaInput[] = "/tmp/apparentAndBS";
  f = open(nmeaInput, O_RDONLY);

  while(true) {
    if (read(f, &nmeaSentence, sizeof(char)*100) > 0) {

      char nmeaType[4];
      strncpy(nmeaType, nmeaSentence+3, 3);

      if(strcmp(nmeaType, "VHW") == 0) {
        // if nmeaSentence contains a VHW string with knots, parse out the boatSpeed
        //$IIVHW,,T,,M,6.66,N,09.26,K
        char *running;
        running = strdup (nmeaSentence);

        char words[9][5];
        char *nextWord;
        int i;
        for (i = 0; i < 9; i++) {
          //todo add some validation
          nextWord = strsep(&running, ",");
          strcpy(words[i], nextWord);
        }
        sscanf(words[5], "%lf", &boatSpeed);

      }
      else if(strcmp(nmeaType, "VWR") == 0) {
        // if nmeaSentence contins a VWR string, parse out the wind direction and speed (knots)
        //$IIVWR,49,L,2.4,N,01.2,M,04.4,K
        char *running;
        running = strdup (nmeaSentence);

        char words[8][5];
        char *nextWord;
        int i;
        for (i = 0; i < 8; i++) {
          //todo add some validation
          nextWord = strsep(&running, ",");
          strcpy(words[i], nextWord);
        }
        sscanf(words[1], "%lf", &apparentWindDirection);
        char leftOrRight = *words[2];
        if (leftOrRight = 'L') {
          apparentWindDirection = 360 - apparentWindDirection;
        }
        // todo: support more than just knots
        sscanf(words[3], "%lf", &apparentWindSpeed);
      }
      else if(strcmp(nmeaType, "MWV") == 0) {
        // apparent wind could also come in the form of MWV with reference R (relatve)
        //$CCMWV,60.32,R,10.5,N
        //TODO: complete this bit
      }


      // apparentWindSpeed = 16;
      // apparentWindDirection = 35;

      if (boatSpeed > -1 && apparentWindSpeed > -1 && apparentWindDirection > -1) {
        writeNMEATrueWind(boatSpeed, apparentWindSpeed, apparentWindDirection, reference, status, units);
        // blank out our values to start searching again
        boatSpeed = -1;
        apparentWindSpeed = -1;
        apparentWindDirection = -1;
      }

    }
  }


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

  FILE *f = fopen("/tmp/trueWind", "w");
  if (f == NULL) {
      printf("Error opening file!\n");
      return false;
  }

  /* print some text */
  fprintf(f, "$CCMWV,%f,%c,%f,%c,%c\n", twd, reference, tws, units, status);

  fclose(f);


  return true;
}
