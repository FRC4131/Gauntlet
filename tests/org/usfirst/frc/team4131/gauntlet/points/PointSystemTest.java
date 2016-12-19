package org.usfirst.frc.team4131.gauntlet.points;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

import org.junit.BeforeClass;
import org.junit.Test;

public class PointSystemTest{
	private static PointSystem points;
	private static Keyframe standard, wildcard;
	@BeforeClass
	public static void setUpBeforeClass() throws Exception{
		points = new PointSystem(5f, 5f, 0.1f);
		standard = points.addLocation(new Location(90f, 45f, 0f));
		wildcard = points.addLocation(new Location(Location.WILDCARD, -80f, 8f));
	}
	@Test
	public void testAddLocation(){
		assertNotEquals(standard, wildcard);
		assertNotEquals(standard, null);
		assertNotEquals(wildcard, null);
	}
	@Test
	public void testGetLocation(){
		//For reference
		System.out.println("Standard: " + standard);
		System.out.println("Wildcard: " + wildcard);
		System.out.println("Null: " + points.getNullKeyframe());
		
		//Standard keyframe
		assertEquals(standard, points.getLocation(90, 45, 0));
		assertEquals(standard, points.getLocation(85.1f, 44.9f, 0.1f));
		
		//Wildcard keyframe
		assertEquals(wildcard, points.getLocation(0, -81, 8.05f));
		assertEquals(wildcard, points.getLocation(4895, -79.2f, 7.918f));
		assertEquals(wildcard, points.getLocation(Float.MAX_VALUE, -84.84f, 8.0999999f));
		
		//Null keyframe against standard
		assertEquals(points.getNullKeyframe(), points.getLocation(84.9f, 45f, 0f));
		assertEquals(points.getNullKeyframe(), points.getLocation(90f, 39.99f, 0f));
		assertEquals(points.getNullKeyframe(), points.getLocation(90f, 45f, 0.11f));
		
		//Null keyframe against wildcard
		assertEquals(points.getNullKeyframe(), points.getLocation(9806574f, -74.98f, 8f));
		assertEquals(points.getNullKeyframe(), points.getLocation(7435f, -80f, 8.90f));
		
		assertEquals(points.getNullKeyframe(), points.getLocation(84f, 984f, 5f));
		assertEquals(points.getNullKeyframe(), points.getLocation(361f, 92.00f, 96852.30f));
	}
}
