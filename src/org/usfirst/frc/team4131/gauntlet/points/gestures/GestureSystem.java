package org.usfirst.frc.team4131.gauntlet.points.gestures;

import org.usfirst.frc.team4131.gauntlet.points.Keyframe;

/**
 * The gesture system takes in a stream of keyframes and converts it to a gesture.
 * @author endreman0
 */
public class GestureSystem{
	private final GestureTree root;
	private GestureTree current;
	private final Gesture nullGesture = new Gesture();
	public GestureSystem(){
		root = new GestureTree(null, null);
		current = root;
	}
	public Gesture addGesture(Keyframe... path){
		Gesture gesture = new Gesture();
		//Traverse down the tree, creating nodes as necessary, until the end of the path
		GestureTree tree = root;
		for(Keyframe frame : path)
			tree = tree.getOrCreateChild(frame);
		//Set the gesture for the node at the end of the path 
		tree.setGesture(gesture);
		
		return gesture;
	}
	public void addKeyframe(Keyframe keyframe){
		if(current == null) return;//An invalid path has been created, so just let the keyframes flow in
		current = current.getChild(keyframe);
	}
	public Gesture getCurrentGesture(){
		return current == null ? null : current.getGesture();
	}
	public Gesture getNullGesture(){
		return nullGesture;
	}
	public boolean hasPotentialGestures(){
		if(current == null) return false;
		else return current.getGesture() != null || current.hasChildren();
	}
	public void reset(){
		current = root;
	}
}
