/**
 * 
 */
package parcel;

/**
 * @author Administrator
 * 
 */
import innerscopes.*;

public class Parcel7 {
	public Wrapping wrap(int x) {
		// Base constructor call:
		return new Wrapping(x) {
			{
				System.out.println("Parcel7.wrap");
			}

			public int value() {
				System.out.println("Parcel7.wrap.value");
				return super.value() * 47;
			}
		}; // Semicolon required
	}

	public static void main(String[] args) {
		Parcel7 p = new Parcel7();
		Wrapping w = p.wrap(10);
		System.out.println("ok!");
		System.out.println(w.value());
	}
} // /:~
