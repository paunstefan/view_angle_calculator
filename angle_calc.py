from math import cos, radians, sin, sqrt, acos, degrees, atan2
import sys

# Ellipsoid parameters: semi-major axis and flattening
WGS84 = 6378137, 298.257223563


def geodetic_to_geocentric(latitude, longitude, altitude):
	"""
	:param latitude: Latitude in decimal degrees
	:param longitude: Longitude in decimal degrees
	:param altitude: Altitude in meters above the ellipsoid
	:returns: The X, Y, Z geocentric coordinates
	"""
	lat_r = radians(latitude)
	lon_r = radians(longitude)
	sin_lat = sin(lat_r)
	a, rf = WGS84
	e2 = 1 - (1 - 1 / rf) ** 2
	n = a / sqrt(1 - e2 * sin_lat ** 2)
	r = (n + altitude) * cos(lat_r)
	x = r * cos(lon_r)
	y = r * sin(lon_r)
	z = (n * (1 - e2) + altitude) * sin_lat

	return x, y, z


def get_elevation(source, destination):
	"""
	:param source: Tuple with the x, y, z coordinates of the observer
	:param destination: Tuple with the x, y, z coordinates of the observable
	:returns: The elevation angle in decimal degrees of the observable
	"""
	x = source[0]
	y = source[1]
	z = source[2]
	dx = destination[0] - x
	dy = destination[1] - y
	dz = destination[2] - z

	cos_elevation = (x*dx + y*dy + z*dz) / sqrt((x**2 + y**2 + z**2)*(dx**2 + dy**2 + dz ** 2))
	angle = degrees(acos(cos_elevation))

	return 90 - angle


def get_azimuth(source, destination):
	"""
	:param source: Tuple with the x, y, z coordinates of the observer
	:param destination: Tuple with the x, y, z coordinates of the observable
	:returns: The azimuth angle in decimal degrees of the observable
	"""
	x = source[0]
	y = source[1]
	z = source[2]
	dx = destination[0] - x
	dy = destination[1] - y
	dz = destination[2] - z

	cos_azimuth = (-z*x*dx - z*y*dy + (x**2 + y**2)*dz) / \
					sqrt((x**2 + y**2) * (x**2 + y**2 + z**2) * (dx**2 + dy**2 + dz ** 2))
	sin_azimuth = (-y*dx + x*dy) / sqrt((x**2 + y**2) * (dx**2 + dy**2 + dz**2))

	angle = degrees(atan2(sin_azimuth, cos_azimuth))

	# if(angle < 0):
	# 	angle = 360 + angle
	# return angle
	return (360 + angle) % 360


if __name__ == "__main__":
	if(len(sys.argv) != 7):
		print("Usage: {} [source_lat] [source_lon] [source_alt] [dest_lat] [dest_lon] [dest_alt]".format(sys.argv[0]))
		sys.exit(1)

	source = geodetic_to_geocentric(float(sys.argv[1]), float(sys.argv[2]), float(sys.argv[3]))
	destination = geodetic_to_geocentric(float(sys.argv[4]), float(sys.argv[5]), float(sys.argv[6]))
	print("Azimuth: {} degrees".format(get_azimuth(source, destination)))
	print("Elevation: {} degrees".format(get_elevation(source, destination)))
