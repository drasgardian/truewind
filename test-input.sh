#!/bin/bash

pipe=/tmp/trueWind

if [[ ! -p $pipe ]]; then
    echo "Reader not running"
    exit 1
fi

#start at 0
degrees=60
side='L'
speed=16
sideDegrees=60

while true; do
#  degrees=$(( RANDOM % 180))
#  if (($degrees % 2 == 0 )) ;  then
#    side='R'
#  else
#    side='L'
#  fi

  degrees=$(($degrees + 10));
  if (($degrees >= 360)) ; then
    degrees=$(($degrees - 360));
  fi

  if (($degrees > 180)) ; then
    sideDegrees=$((360 - $degrees));
    side='R';
  else
    sideDegrees=$degrees
    side='L';
  fi

  echo "\$IIVWR,$sideDegrees,$side,$speed,N,01.2,M,04.4,K" > $pipe
  #echo "\$IIMWV,141.11,R,10,N,A" > $pipe
  # waterspeed
  waterspeed=10
  echo "\$IIVHW,,T,,M,$waterspeed,N,09.26,K" > $pipe
  sleep 0.25
done
