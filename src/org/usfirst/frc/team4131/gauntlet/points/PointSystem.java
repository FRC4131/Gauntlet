package org.usfirst.frc.team4131.gauntlet.points;

import java.util.HashMap;
import java.util.Map;

/**
 * The point system converts yaw/pitch/radius coordinates into keyframes.
 * Call {@link #addLocation(Location)} to add location/keyframe pairs in initialization, and {@link #getLocation(float, float, float)} for
 * every point as it is read from the IMU. 
 * @author endreman0
 */
public class PointSystem{
	/**
	 * For any location {@code loc}, all triples {@code (loc.yaw +/- errorYaw, loc.pitch +/- errorPitch, loc.radius +/- errorRadius)} are considered
	 * coincident with {@code loc}.
	 * {@see Location#WILDCARD}
	 */
	private final float errorYaw, errorPitch, errorRadius;
	private final Map<Location, Keyframe> locations = new HashMap<>();
	/**
	 * Getting this keyframe returned from {@link #getLocation(float, float, float)} means that no location fit the given points.
	 */
	private final Keyframe nullKeyframe = new Keyframe();
	/**
	 * Creates a new point system. Point systems should be singletons per client.
	 * @param errorYaw Value of {@link #errorYaw}
	 */
	public PointSystem(float errorYaw, float errorPitch, float errorRadius){
		this.errorYaw = errorYaw;
		this.errorPitch = errorPitch;
		this.errorRadius = errorRadius;
	}
	/**
	 * Adds a location to the list of locations this point system knows.
	 * @param loc Location to add
	 * @return the keyframe for the given location
	 */
	public Keyframe addLocation(Location loc){
		Keyframe frame = new Keyframe();
		locations.put(loc, frame);
		return frame;
	}
	public Keyframe getLocation(float yaw, float pitch, float radius){
		for(Location loc : locations.keySet()){
			if(test(loc.getYaw(), yaw, errorYaw) && test(loc.getPitch(), pitch, errorPitch) && test(loc.getRadius(), radius, errorRadius))
				return locations.get(loc);
		}
		return nullKeyframe;
	}
	/**
	 * Test a raw coordinate's coincidence a location's coordinate.<br>
	 * If the location's coordinate is {@link Location#WILDCARD}, the coordinate is coincident.
	 * If not, the raw coordinate must be within {@code +/- error} (inclusive) of the location's coordinate to coincide.
	 * @param locCoord The coordinate from the location object
	 * @param rawCoord The raw coordinate to be tested
	 * @param error The error interval for the test
	 * @return {@code true} if the raw coordinate is coincident with the location's, {@code false} otherwise
	 */
	private boolean test(Float locCoord, float rawCoord, float error){
		return locCoord == Location.WILDCARD ? true : Math.abs(locCoord - rawCoord) <= error;
	}
	/**
	 * {@see #nullKeyframe}
	 * @return This point system's null keyframe
	 */
	public Keyframe getNullKeyframe(){
		return nullKeyframe;
	}
}
