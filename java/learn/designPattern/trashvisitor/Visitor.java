package designPattern.trashvisitor;

//: Visitor.java 
//The base interface for visitors 
import designPattern.trash.*;

interface Visitor {
	void visit(VAluminum a);

	void visit(VPaper p);

	void visit(VGlass g);

	void visit(VCardboard c);
} // /:~
