package designPattern.trash;

//: Paper.java  
//The Paper class with prototyping 

public class Paper extends Trash {
	private static double val = 0.10f;

	public Paper(double wt) {
		super(wt);
	}

	public double value() {
		return val;
	}

	public static void value(double newVal) {
		val = newVal;
	}
} // /:~
