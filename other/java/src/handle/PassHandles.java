/**
 * 
 */
package handle;

/**
 * @author Administrator
 * 
 */
// : PassHandles.java
// Passing handles around
public class PassHandles {
	static void f(PassHandles h) {
		System.out.println("h inside f(): " + h);
	}

	public static void main(String[] args) {
		PassHandles p = new PassHandles();
		PassHandles o = p;
		System.out.println("p inside main(): " + p);
		f(o);
	}
} // /:~
