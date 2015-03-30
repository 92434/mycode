/**
 * 
 */
package rtti;

/**
 * @author Administrator
 * 
 */
// : ToyTest.java
// Testing class Class
interface HasBatteries {
}

interface Waterproof {
}

interface ShootsThings {
}

class Toy {
	// Comment out the following default
	// constructor to see
	// NoSuchMethodError from (*1*)
	Toy() {
	}

	Toy(int i) {
	}
}

class FancyToy extends Toy implements HasBatteries, Waterproof, ShootsThings {
	FancyToy() {
		super(1);
	}
}

public class ToyTest {
	public static void main(String[] args) {
		Class c = null;
		try {
			c = Class.forName("rtti.FancyToy");
		} catch (ClassNotFoundException e) {
			System.out.println("error");
			return;
		}
		printInfo(c);
		Class[] faces = c.getInterfaces();
		for (int i = 0; i < faces.length; i++)
			printInfo(faces[i]);
		Class cy = c.getSuperclass();
		Object o = null;
		try {
			// Requires default constructor:
			o = cy.newInstance(); // (*1*)
		} catch (InstantiationException e) {
		} catch (IllegalAccessException e) {
		}
		printInfo(o.getClass());
		while (o != null) {
			printInfo(o.getClass());
			o = GetSuclass(o.getClass());
		}
	}

	static void printInfo(Class cc) {
		System.out.println("Class name: " + cc.getName() + " is interface? ["
				+ cc.isInterface() + "]");
	}

	static Object GetSuclass(Class c) {
		Class Suc = c.getSuperclass();
		// Object oo = c.getSuperclass().newInstance();
		// Object oo = Suc.newInstance();
		// printInfo(oo.getClass());
		try {
			// Object oo = Suc.newInstance();
			// printInfo(oo.getClass());
			return Suc.newInstance();
		} catch (Exception e) {
			System.out.println("exception for superclass");
			return null;
		}
	}
} // /:~
