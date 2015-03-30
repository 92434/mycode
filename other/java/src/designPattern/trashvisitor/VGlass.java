package designPattern.trashvisitor;

//: VGlass.java 
//Glass for the visitor pattern 
import designPattern.trash.*;

public class VGlass extends Glass implements Visitable {
	public VGlass(double wt) {
		super(wt);
	}

	public void accept(Visitor v) {
		v.visit(this);
	}
} // /:~
