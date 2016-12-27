package org.usfirst.frc.team4131.gauntlet.points.gestures;

import java.util.HashMap;
import java.util.Map;

import org.usfirst.frc.team4131.gauntlet.points.Keyframe;

class GestureTree{
	private Gesture gesture;
	private final GestureTree parent;
	private final Map<Keyframe, GestureTree> children = new HashMap<>();
	
	GestureTree(GestureTree parent, Gesture value){this.parent = parent; this.gesture = value;}
	
	Gesture getGesture(){return gesture;}
	void setGesture(Gesture gesture){this.gesture = gesture;}
	GestureTree getParent(){return parent;}
	
	GestureTree getChild(Keyframe keyframe){return children.get(keyframe);}
	boolean hasChildren(){return !children.isEmpty();}
	GestureTree getOrCreateChild(Keyframe keyframe){
		GestureTree child = children.get(keyframe);
		if(child != null) return child;
		else{
			child = new GestureTree(this, null);
			children.put(keyframe, child);
			return child;
		}
	}
}
