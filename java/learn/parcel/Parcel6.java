package parcel;

//: Parcel6.java 
//A method that returns an anonymous inner class 
public class Parcel6 {

	Parcel6() {
		System.out.println("Parcel6 constructor");
	}

	public Contents cont() {
		return new Contents() {
			private int i = 11;

			public int value() {
				return i;
			}
		}; // Semicolon required in this case
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Parcel6 p = new Parcel6();
		Contents c = p.cont();
	}

}
