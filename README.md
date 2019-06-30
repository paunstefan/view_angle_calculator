# view_angle_calculator

Simple program to calculate the view angle(azimuth and elevation) between two points.

# How to use

There are two implementations available, one in Pyhton and one in C. If you care about speed use the C one.

To compile the C version run the following GCC command:
```
gcc -o angle_calc angle_calc.c -lm
```

To run the program use one of  the following commands:
```
python3 angle_calc.py [source_lat] [source_lon] [source_alt] [dest_lat] [dest_lon] [dest_alt]

// or

./angle_calc [source_lat] [source_lon] [source_alt] [dest_lat] [dest_lon] [dest_alt]
```

It will print the azimuth and the elevation angle from the source point to the destination point.

# Sources
[Geodetic datum](https://en.wikipedia.org/wiki/Geodetic_datum)

[Geographic coordinate conversion](https://en.wikipedia.org/wiki/Geographic_coordinate_conversion)

[Calculating view angle](https://gis.stackexchange.com/questions/58923/calculating-view-angle)

[Convert geodetic coordinates to geocentric (cartesian)](https://codereview.stackexchange.com/questions/195933/convert-geodetic-coordinates-to-geocentric-cartesian)