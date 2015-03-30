package constructor;

class Tree {
	int height;

	Tree() {
		prt("Planting a seedling");
		height = 0;
	}

	Tree(int i) {
		prt("Creating new Tree that is " + i + " feet tall");
		height = i;
	}

	void info() {
		prt("Tree is " + height + " feet tall");
	}

	void info(String s) {
		prt(s + "Tree is " + height + " feet tall");
	}

	static void f1(short i) {
		prt("ok");
	}

	static void prt(String s) {
		System.out.println(s);
	}
}

public class Overloading {

	/**
	 * @param args
	 */
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO �Զ���ɷ������

		for (int i = 0; i < 5; i++) {
			Tree t = new Tree(i);
			t.info();
			t.info("overload method ");
			t.f1((byte) 1);
		}
	}

}
