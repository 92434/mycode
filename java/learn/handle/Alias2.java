/**
 * 
 */
package handle;

/**
 * @author Administrator
 * 
 */
// : Alias2.java
// Method calls implicitly alias their
// arguments.
public class Alias2 {
	int i;

	Alias2(int ii) {
		i = ii;
	}

	static void f(Alias2 handle) {
		handle.i++;
	}

	public static void main(String[] args) {
		Alias2 x = new Alias2(7);
		// Alias2 y = x;
		Alias2 y = new Alias2(16);
		System.out.println("x: " + x.i);
		System.out.println("Calling f(x)");
		f(x);
		f(y);
		System.out.println("x: " + x.i);
		System.out.println("y: " + y.i);
	}
} // /:~
