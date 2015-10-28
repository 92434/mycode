/**
 * 
 */
package parcel;

/**
 * @author Administrator
 * 
 */
// : Parcel10.java
// Static inner classes
// abstract class Contents {
// abstract public int value();
// }
//
// interface Destination {
// String readLabel();
// }
public class Parcel10 {
	private static class PContents extends Contents {
		private int i = 11;

		public int value() {
			return i;
		}
	}

	protected static class PDestination implements Destination {
		private String label;

		private PDestination(String whereTo) {
			label = whereTo;
		}

		public String readLabel() {
			return label;
		}
	}

	public static Destination dest(String s) {
		return new PDestination(s);
	}

	public static Contents cont() {
		return new PContents();
	}

	static class Tester {
		public static void main(String[] args) {
			Contents c = cont();
			Destination d = dest("Tanzania");
		}
	}
} // /:~
