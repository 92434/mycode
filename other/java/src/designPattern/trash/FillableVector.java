package designPattern.trash;

//: FillableVector.java  
//Adapter that makes a Vector Fillable 
import java.util.*;

public class FillableVector implements Fillable {
	private Vector v;

	public FillableVector(Vector vv) {
		v = vv;
	}

	public void addTrash(Trash t) {
		v.addElement(t);
	}
} // /:~
