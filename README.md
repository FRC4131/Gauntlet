# Gauntlet
Glove equipped with gesture recognition, for controlling FRC robots

## Terminology
- **Location**: a discreet position in the world. Represented as a triple of x/y/z or yaw/pitch/radius.
- **Keyframe**: an identifier that represents a location.
- **Path**: a series of keyframes in chronological order.
- **Gesture**: an identifier that represents a path.

## Systems
### Point System (`PointSystem.h`)
Interprets a stream of IMU data from the client and converts it to a stream of locations.

### Gesture System (`GestureSystem.h`)
Interprets a stream of locations and converts it to a path, then identifies and returns the gesture.

### Client (`Gauntlet.ino`)
Provides a stream of IMU data to the point system, and acts upon gestures identified by the gesture system. Only this system changes drastically between applications.

## Branches in this Repository
- `prototype`: prototype code (in Java) for development. The client takes in simulated IMU data from the user and outputs gestures back.
- `Arduino`: Arduino code for "production". The client takes in data from a 9DoF IMU and outputs gesture to the Serial monitor.
- `experiment`: deprecated on 12-Mar-2017: Arduino code for "production". The client takes in data from a 9DoF IMU and outputs gesture to the Serial monitor.
