package innerscopes;

public class Wrapping {
	private int i;

	public Wrapping(int x) {
		System.out.println("Warpping.Warpping");
		i = x;
	}

	public int value() {
		System.out.println("Warpping.value");
		return i;
	}
} // /:~
