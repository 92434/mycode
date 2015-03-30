/**
 * 
 */
package parcel;

/**
 * @author Administrator
 * 
 */
public class Parcel8 {
	// Argument must be final to use inside
	// anonymous inner class:
	public Destination dest(final String dest) {
		return new Destination() {
			private String label = dest;

			public String readLabel() {
				return label;
			}
		};
	}

	public static void main(String[] args) {
		Parcel8 p = new Parcel8();
		Destination d = p.dest("Tanzania");
		System.out.println(d.readLabel());

		Destination d1 = p.dest("xiaofei");
		System.out.println(d1.readLabel());
	}
} // /:~
