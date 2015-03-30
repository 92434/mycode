package designPattern.trashvisitor;

//: VPaper.java 
//Paper for the visitor pattern 
import designPattern.trash.*;

public class VPaper extends Paper implements Visitable {
	public VPaper(double wt) {
		super(wt);
	}

	public void accept(Visitor v) {
		v.visit(this);
	}
} // /:~
