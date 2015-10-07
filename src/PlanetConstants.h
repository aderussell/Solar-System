#pragma once

// whether, or not to use real scale or fake one.
// if you use real then you won't see nothing. The solar system is real empty in reality!
#define USE_REAL_SCALES 0
#if USE_REAL_SCALES
	#define ASTROLOGICAL_UNIT 149600000.0
#else
	// the fake scale redefines the distance between the planets although keeps the ratio the same
	#define ASTROLOGICAL_UNIT 9960000.0
#endif

// the amount to scale all of the sizes by
#define SCALE_SIZES_AMMOUNT (20.0 / 696000.0)

// the edge of the solar system (this should be the scale for the sky box)
#define EDGE_OF_SOLAR_SYSTEM (50.0 * ASTROLOGICAL_UNIT)


#define SUN_RADIUS     696000.0
#define MERCURY_RADIUS 2439.0
#define VENUS_RADIUS   6051.8
#define EARTH_RADIUS   6371.8
#define MARS_RADIUS    3390.0
#define JUPITER_RADIUS 69911.0
#define SATURN_RADIUS  58232.0
#define URANUS_RADIUS  25362.0
#define NEPTUNE_RADIUS 24622.0
#define MOON_RADIUS    1737.1


#define MERCURY_DISTANCE_FROM_SUN (00.387 * ASTROLOGICAL_UNIT)
#define VENUS_DISTANCE_FROM_SUN   (00.723 * ASTROLOGICAL_UNIT)
#define EARTH_DISTANCE_FROM_SUN   (01.000 * ASTROLOGICAL_UNIT)
#define MARS_DISTANCE_FROM_SUN    (01.324 * ASTROLOGICAL_UNIT)
#define JUPITER_DISTANCE_FROM_SUN (05.203 * ASTROLOGICAL_UNIT)
#define SATURN_DISTANCE_FROM_SUN  (09.523 * ASTROLOGICAL_UNIT)
#define URANUS_DISTANCE_FROM_SUN  (19.208 * ASTROLOGICAL_UNIT)
#define NEPTUNE_DISTANCE_FROM_SUN (30.087 * ASTROLOGICAL_UNIT)
#define MOON_DISTANCE_FROM_EARTH  (0.00256 * ASTROLOGICAL_UNIT)

// spin durations (in Earth Days)
#define SUN_SPIN_DURATION     27.0
#define MERCURY_SPIN_DURATION 58.64
#define VENUS_SPIN_DURATION   243.01
#define EARTH_SPIN_DURATION   1.0
#define MARS_SPIN_DURATION    1.027
#define JUPITER_SPIN_DURATION 0.412
#define SATURN_SPIN_DURATION  0.445
#define URANUS_SPIN_DURATION  0.734
#define NEPTUNE_SPIN_DURATION 0.671
#define MOON_SPIN_DURATION    28.0

// orbit durations (in Earth Days)
#define MERCURY_ORBIT_DURATION 88.0
#define VENUS_ORBIT_DURATION   224.7
#define EARTH_ORBIT_DURATION   365.26
#define MARS_ORBIT_DURATION    686.68
#define JUPITER_ORBIT_DURATION 4331.98
#define SATURN_ORBIT_DURATION  10760.55
#define URANUS_ORBIT_DURATION  30685.49
#define NEPTUNE_ORBIT_DURATION 60191.19
#define MOON_ORBIT_DURATION    28.0
