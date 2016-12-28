#include "Arduino.h"

struct point{
  double yaw;
  double pitch;
  double radius;
};
typedef struct point Point;

/*
 * Point systems are used to restrict input yaw, pitch, and radius values coming from an IMU to a set of known locations.
 * Terminology:
 *   Points are the raw information coming from the IMU. They are represented as a (yaw, pitch, radius) triple.
 *   Locations are the set of known points created in setup.
 *   Keyframes are identifiers for locations. When you add a location, you get a keyframe back which matches up with the provided location.
 * Points are represented as (yaw, pitch, radius) triples. This system doesn't care where 0 is or what units you use. As long as the units are consistent for the error values,
 * locations' coordinates, and points' coordinates, the system will work.
 */
class PointSystem{
  public:
    PointSystem(double errYaw, double errPitch, double errRadius);
    /* Any field with this value is ignored when determining if a point coincides with a location. */
    constexpr static double IGNORE = 361.0;
    /* Adds the given location, and returns its keyframe. */
    byte addLocation(double yaw, double pitch, double radius);
    /*
     * Returns the keyframe for the first location that coincides with the given point.
     * A location coincides with a point if, for each of yaw, pitch, and radius, the location's value is IGNORE, or the difference between the location's value and the point's value is
     * within the error value specified in the constructor.
     * 
     * If no locations coincide, 0 is returned.
    */
    byte getKeyframe(double yaw, double pitch, double radius);
  private:
    /* Number of locations added. Note that this is not the same as numLocations in the constructor; numLocations is the maximum number of points that can be added. */
    byte _numLocations;
    double _errYaw;
    double _errPitch;
    double _errRadius;
    Point _locations[10];
    /*
     * Test a raw coordinate's coincidence a location's coordinate.
     * If the location's coordinate is IGNORE, the coordinate is coincident.
     * If not, the point's coordinate must be within +/- error to coincide.
     */
    bool test(double expectedValue, double rawValue, double error);
};
