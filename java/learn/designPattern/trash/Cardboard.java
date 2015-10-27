package designPattern.trash;

//: Cardboard.java  
//The Cardboard class with prototyping 

public class Cardboard extends Trash {
	private static double val = 0.23f;

	public Cardboard(double wt) {
		super(wt);
	}

	public double value() {
		return val;
	}

	public static void value(double newVal) {
		val = newVal;
	}
} // /:~
