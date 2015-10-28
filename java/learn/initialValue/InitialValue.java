/**
 * 
 */
package initialValue;

/**
 * @author Administrator
 * 
 */

// : InitialValues.java
// Shows default initial values
class Measurement {
	boolean t;

	char c;

	byte b;

	short s;

	int i;

	long l;

	float f;

	double d;

	void print() {
		System.out.println("Data type      Inital value\n" + "boolean        "
				+ t + "\n" + "char           " + c + "\n" + "byte           "
				+ b + "\n" + "short          " + s + "\n" + "int            "
				+ i + "\n" + "long           " + l + "\n" + "float          "
				+ f + "\n" + "double         " + d);
	}
}

public class InitialValue {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Measurement d = new Measurement();
		d.print();
		/*
		 * In this case you could also say: new Measurement().print();
		 */
	}

}
