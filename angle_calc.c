#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define deg_to_rad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define rad_to_deg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

// The WGS85 ellipsoid semi-major axis and flattening
float WGS85_axis = 6378137;
float WGS85_flattening = 298.257223563;


/**
 * @param lat latitude in decimal degrees
 * @param lon longitude in decimal degrees
 * @param alt altitude in meters
 * @return a float array containing the X, Y and Z coordinates
 */
float* geodetic_to_geocentric(float lat, float lon, float alt){
	float *coordinates = (float*)malloc(sizeof(float) * 3);
	float lat_r = deg_to_rad(lat);
	float lon_r = deg_to_rad(lon);

	float e2 = 1 - pow((1 - 1 / WGS85_flattening), 2);
	float n = WGS85_axis / sqrt(1 - e2 * pow(sin(lat_r), 2));
	float r = (n + alt) * cos(lat_r);

	coordinates[0] = r * cos(lon_r);
	coordinates[1] = r * sin(lon_r);
	coordinates[2] = (n * (1 - e2) + alt) * sin(lat_r);

	return coordinates;
}


/**
 * @param source float array containing the X, Y and Z coordinates of the observer
 * @param destination lfloat array containing the X, Y and Z coordinates of the observable
 * @return the angle of elevation in decimal degrees
 */
float get_elevation(float* source, float* destination){
	float x = source[0];
	float y = source[1];
	float z = source[2];
	float dx = destination[0] - x;
	float dy = destination[1] - y;
	float dz = destination[2] - z;

	float cos_elevation = (x*dx + y*dy + z*dz) / 
						sqrt((pow(x, 2) + pow(y, 2) + pow(z, 2))*(pow(dx, 2) + pow(dy, 2) + pow(dz, 2)));
	float angle = rad_to_deg(acos(cos_elevation));

	return  90 - angle;
}


/**
 * @param source float array containing the X, Y and Z coordinates of the observer
 * @param destination lfloat array containing the X, Y and Z coordinates of the observable
 * @return the azimuth angle in decimal degrees
 */
float get_azimuth(float* source, float* destination){
	float x = source[0];
	float y = source[1];
	float z = source[2];
	float dx = destination[0] - x;
	float dy = destination[1] - y;
	float dz = destination[2] - z;

	float cos_azimuth = (-z*x*dx - z*y*dy + (pow(x, 2) + pow(y, 2))*dz) / 
				sqrt((pow(x, 2) + pow(y, 2)) * (pow(x, 2) + pow(y, 2) + pow(z, 2)) * (pow(dx, 2) + pow(dy, 2) + pow(dz, 2)));
	float sin_azimuth = (-y*dx + x*dy) / sqrt((pow(x, 2) + pow(y, 2)) * (pow(dx, 2) + pow(dy, 2) + pow(dz, 2)));

	float angle = rad_to_deg(atan2(sin_azimuth, cos_azimuth));

	return fmod((360 + angle), (float)360);
}

int main(int argc, char** argv){
	if(argc != 7){
		printf("Usage: %s [source_lat] [source_lon] [source_alt] [dest_lat] [dest_lon] [dest_alt]\n", argv[0]);
		exit(1);
	}

	float *source = geodetic_to_geocentric(strtof(argv[1], NULL), strtof(argv[2], NULL), strtof(argv[3], NULL));
	float *destination = geodetic_to_geocentric(strtof(argv[4], NULL), strtof(argv[5], NULL), strtof(argv[6], NULL));

	float elevation = get_elevation(source, destination);
	float azimuth = get_azimuth(source, destination);

	printf("Azimuth: %f degrees\n", azimuth);
	printf("Elevation: %f degrees\n", elevation);

	free(source);
	free(destination);
	return 0;
}