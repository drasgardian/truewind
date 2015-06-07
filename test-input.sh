#!/bin/bash

pipe=/tmp/wind

if [[ ! -p $pipe ]]; then
    echo "Reader not running"
    exit 1
fi

#start at 60. nice reach
degrees=60
side='L'
speed=16

while true; do
  degrees=$(( RANDOM %= 180))
  if (($degrees % 2 == 0 )) ;  then
    side='R'
  else
    side='L'
  fi



  echo "\$IIVWR,$degrees,$side,$speed,N,01.2,M,04.4,K" > $pipe
  #echo "\$IIMWV,141.11,R,10,N,A" > $pipe
  # waterspeed
  waterspeed=10
  echo "\$IIVHW,,T,,M,$waterspeed,N,09.26,K" > $pipe
  sleep 0.1
done
