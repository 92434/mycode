/**
 * 
 */
package parcel;

/**
 * @author Administrator
 * 
 */
// : Parcel11.java
// Creating inner classes
public class Parcel11 {
	class Contents {
		private int i = 11;

		public int value() {
			return i;
		}
	}

	static class Destination {
		private String label;

		Destination(String whereTo) {
			label = whereTo;
		}

		String readLabel() {
			return label;
		}
	}

	public static void main(String[] args) {
		Parcel11 p = new Parcel11();
		// Must use instance of outer class
		// to create an instances of the inner class:
		Parcel11.Contents c = p.new Contents();
		System.out.println(c.value());
		// Parcel11.Destination d = p.new Destination("Tanzania");
		// System.out.println(d.readLabel());
		// System.out.println(p.new Destination("xiaofei").readLabel());
	}
} // /:~
