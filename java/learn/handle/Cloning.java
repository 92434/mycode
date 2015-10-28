/**
 * 
 */
package handle;

/**
 * @author Administrator
 * 
 */
// : Cloning.java
// The clone() operation works for only a few
// items in the standard Java library.
import java.util.*;

class Int implements Cloneable {
	private int i;

	public Int(int ii) {
		i = ii;
	}

	public void increment() {
		i++;
	}

	public String toString() {
		return Integer.toString(i);
	}

	public Object clone() {
		Object o = null;
		try {
			o = super.clone();
		} catch (CloneNotSupportedException e) {
		}
		return o;
	}
}

public class Cloning {
	public static void main(String[] args) {
		Vector v = new Vector();
		for (int i = 0; i < 10; i++)
			v.addElement(new Int(i));
		System.out.println("v: " + v);
		Vector v2 = (Vector) v.clone();
		// Increment all v2's elements:
		for (Enumeration e = v2.elements(); e.hasMoreElements();)
			((Int) e.nextElement()).increment();
		// See if it changed v's elements:
		System.out.println("v: " + v);

		Int I = new Int(1);
		Int IClone = (Int) I.clone();
		I.increment();
		System.out.println("I: " + I);
		System.out.println("IClone: " + IClone);
	}
} // /:~
