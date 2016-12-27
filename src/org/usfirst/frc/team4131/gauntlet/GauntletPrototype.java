package org.usfirst.frc.team4131.gauntlet;

import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.usfirst.frc.team4131.gauntlet.points.Keyframe;
import org.usfirst.frc.team4131.gauntlet.points.Location;
import org.usfirst.frc.team4131.gauntlet.points.PointSystem;
import org.usfirst.frc.team4131.gauntlet.points.gestures.Gesture;
import org.usfirst.frc.team4131.gauntlet.points.gestures.GestureSystem;

/**
 * Prototype client for the gesture recognition system.
 * @author endreman0
 */
public class GauntletPrototype{
	private final PointSystem points;
	private final Keyframe CENTER, UP, DOWN, FORWARD, RIGHT;
	private final GestureSystem gestures;
	private final Gesture FIRE, SEEK_FIRE, COLLECT;
	private GauntletPrototype(){
		points = new PointSystem(5f, 5f, 0.1f);
		CENTER = points.addLocation(new Location(Location.WILDCARD, Location.WILDCARD, 0f));
		UP = points.addLocation(new Location(Location.WILDCARD, 90f, 1f));
		DOWN = points.addLocation(new Location(Location.WILDCARD, -90f, 1f));
		FORWARD = points.addLocation(new Location(0f, 0f, 1f));
		RIGHT = points.addLocation(new Location(90f, 0f, 1f));
		
		gestures = new GestureSystem();
		FIRE = gestures.addGesture(CENTER, UP);
		SEEK_FIRE = gestures.addGesture(CENTER, RIGHT, UP);
		COLLECT = gestures.addGesture(DOWN, CENTER);
	}
	public void update(float yaw, float pitch, float radius){
		Keyframe keyframe = points.getLocation(yaw, pitch, radius);
		if(keyframe == points.getNullKeyframe()){
			System.out.println("No matched keyframes.");
			return;
		}
		
		gestures.addKeyframe(keyframe);
		
		if(!gestures.hasPotentialGestures()){
			System.out.println("No matched paths.");
			gestures.reset();
			return;
		}else if(gestures.getCurrentGesture() != gestures.getNullGesture()){
			Gesture gesture = gestures.getCurrentGesture();
			if(gesture == FIRE) System.out.println("Firing");
			else if(gesture == SEEK_FIRE) System.out.println("Seeking and firing");
			else if(gesture == COLLECT) System.out.println("Collecting");
		}
	}
	
	public static void main(String[] args){
		GauntletPrototype proto = new GauntletPrototype();
		final Pattern patternPoint;
		{
			String coord = "(-?\\d*(?:\\.\\d*)?)";//One coordinate
			String separator = "\\s*,\\s*";//Separator between coords: commas and whitespace
			patternPoint = Pattern.compile("\\(" + coord + separator + coord + separator + coord + "\\)");
		}
		Scanner in = new Scanner(System.in);
		while(true){
			String input = in.nextLine();
			if(input.equals("exit")) break;
			Matcher m = patternPoint.matcher(input);
			if(!m.find()){
				System.out.println("Invalid input.");
				continue;
			}
			float yaw = Float.parseFloat(m.group(1)), pitch = Float.parseFloat(m.group(2)), radius = Float.parseFloat(m.group(3));
			proto.update(yaw, pitch, radius);
		}
		in.close();
	}
}
