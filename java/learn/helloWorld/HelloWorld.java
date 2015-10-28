package helloWorld;

public class HelloWorld {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO �Զ���ɷ������
		int i = 64;
		i >>>= 3;
		System.out.println(i);
		Integer n1 = new Integer(47);
		Integer n2 = new Integer(47);
		System.out.println("n1.equals(n2) " + n1.equals(n2));
		System.out.println("n1 == n2 " + (n1 == n2));
		value v1 = new value();
		value v2 = new value();
		v1.i = v2.i = 100;
		System.out.println("v1.equals(v2) " + v1.equals(v2));
		System.out.println("v1 == v2 " + (v1 == v2));
		System.out.println("v1.i == v2.i " + (v1.i == v2.i));
		int i1 = 3, i2 = 4;
		while (i1 == i2) {
			System.out.println("hi");
		}
	}

}

class value {
	int i;
}