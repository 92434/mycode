package designPattern.trashvisitor;

//: VCardboard.java 
//Cardboard for the visitor pattern 
import designPattern.trash.*;

public class VCardboard extends Cardboard implements Visitable {
	public VCardboard(double wt) {
		super(wt);
	}

	public void accept(Visitor v) {
		v.visit(this);
	}
} // /:~
