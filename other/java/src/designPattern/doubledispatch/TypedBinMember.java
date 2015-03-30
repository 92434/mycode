package designPattern.doubledispatch;

//: TypedBinMember.java 
//An interface for adding the double dispatching 
//method to the trash hierarchy without  
//modifying the original hierarchy. 

interface TypedBinMember {
	// The new method:
	boolean addToBin(TypedBin[] tb);
} // /:~
