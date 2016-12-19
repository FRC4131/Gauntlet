package org.usfirst.frc.team4131.gauntlet.points;

public class Location{
	/** Wildcard value for any of three fields. Any field with this value is not included in the determination of whether a point coincides with a location.*/
	public static final Float WILDCARD = new Float(0);
	private final Float yaw, pitch, radius;
	public Location(Float yaw, Float pitch, Float radius){
		this.yaw = yaw;
		this.pitch = pitch;
		this.radius = radius;
	}
	public Float getYaw(){return yaw;}
	public Float getPitch(){return pitch;}
	public Float getRadius(){return radius;}
}
