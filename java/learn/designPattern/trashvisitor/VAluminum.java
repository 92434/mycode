package designPattern.trashvisitor;

//: VAluminum.java 
//Aluminum for the visitor pattern 
import designPattern.trash.*;

public class VAluminum extends Aluminum implements Visitable {
	public VAluminum(double wt) {
		super(wt);
	}

	public void accept(Visitor v) {
		v.visit(this);
	}
} // /:~
