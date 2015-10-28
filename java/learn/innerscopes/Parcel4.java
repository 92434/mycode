/**
 * 
 */
package innerscopes;

/**
 * @author Administrator
 * 
 */
public class Parcel4 {

	/**
	 * @param args
	 */
	public Destination dest(String s) {
		class PDestination implements Destination {
			private String label;

			private PDestination(String whereTo) {
				label = whereTo;
			}

			public String readLabel() {
				return label;
			}
		}
		return new PDestination(s);
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Parcel4 p = new Parcel4();
		Destination d = p.dest("Tanzania");
		System.out.println("ok!");
	}

}
