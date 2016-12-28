#include "PointSystem.h"

PointSystem::PointSystem(double errYaw, double errPitch, double errRadius){
	_numLocations = 0;//Start with 0, then add
	_errYaw = errYaw;
	_errPitch = errPitch;
	_errRadius = errRadius;
}
byte PointSystem::addLocation(double yaw, double pitch, double radius){
  _locations[_numLocations] = {yaw, pitch, radius};
	_numLocations++;
	return _numLocations;//After incrementing - effectively (_numLocations + 1) because keyframes are 1-indexed
}
byte PointSystem::getKeyframe(double yaw, double pitch, double radius){
	for(byte i=0; i<_numLocations; i++){
		if(test(_locations[i].yaw, yaw, _errYaw) && test(_locations[i].pitch, pitch, _errPitch) && test(_locations[i].radius, radius, _errRadius)){
			return i + 1;//1-indexed
		}
	}
	return 0;
}
bool PointSystem::test(double expectedValue, double rawValue, double error){
  if(expectedValue == IGNORE){
    return true;
  }else{
    return abs(expectedValue - rawValue) <= error;
  }
}
