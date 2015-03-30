package designPattern.trashvisitor;

//: Visitable.java 
//An interface to add visitor functionality to  
//the Trash hierarchy without modifying the  
//base class. 
import designPattern.trash.*;

interface Visitable {
	// The new method:
	void accept(Visitor v);
} // /:~
