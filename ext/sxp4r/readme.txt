Documentation for C++ NORAD SGP4/SDP4 Implementation
Developed by Michael F. Henry
December, 2003

Copyright © 2003-2010. All rights reserved.
Permission to use for non-commercial purposes only. 
All other uses contact author at mfh@zeptomoby.com

The files in this package implement the SGP4 and SDP4 algorithms described in the 
December, 1980 NORAD document "Space Track Report No. 3". The report provides 
FORTRAN IV implementations of each. These two orbital models, one for "near-earth" 
objects and one for "deep space" objects, are widely used in satellite tracking software 
and can produce very accurate results when used with current NORAD two-line element 
datum.

The original NORAD FORTRAN IV SGP4/SDP4 implementations were converted to 
Pascal by Dr. TS Kelso in 1995. This work formed the basis of a modern, object-
oriented C++ implementation by the author in 2002.

The project files were compiled using Microsoft Visual Studio 7.0.

=========================================================================================

A brief description of important classes:

cTle – This class encapsulates a single set of NORAD two line elements.

cEci – This class encapsulates Earth-Centered Inertial coordinates and velocity for a 
given moment in time.

cOrbit – Given a cTle object, this class provides information about the orbit of the 
described satellite, including inclination, perigee, eccentricity, etc. Most importantly, it 
provides ECI coordinates/velocity for the satellite.

CSite – Describes a location on the earth. Given the ECI coordinates of a satellite, this 
class can generate Azimuth/Elevation look angles to the satellite.

cNoradBase, cNoradSGP4, cNoradSDP4 – These classes implement the NORAD 
SGP4/SDP4 algorithms. They are used by cOrbit to calculate the ECI 
coordinates/velocity of its associated satellite.

For excellent information on the underlying physics of orbits, visible satellite 
observations, current NORAD TLE data, and other related material, see 
http://www.celestrak.com which is maintained by Dr. TS Kelso.

Michael F. Henry
December, 2003
(Updated January, 2009)