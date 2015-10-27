/**
 * 
 */
package leaf;

/**
 * @author Administrator
 * 
 */
public class Leaf {

	private int i = 0;

	Leaf increment() {
		i++;
		return this;
	}

	void print() {
		System.out.println("i = " + i);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Leaf x = new Leaf();
		x.increment().increment().increment().print();
	}

}
