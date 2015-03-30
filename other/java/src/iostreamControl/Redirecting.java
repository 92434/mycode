/**
 * 
 */
package iostreamControl;

/**
 * @author Administrator
 * 
 */
// : Redirecting.java
// Demonstrates the use of redirection for
// standard IO in Java 1.1
import java.io.*;

class Redirecting {
	public static void main(String[] args) {
		try {
			BufferedInputStream in = new BufferedInputStream(
					new FileInputStream(args[0]));
			// Produces deprecation message:
			PrintStream out = new PrintStream(new BufferedOutputStream(
					new FileOutputStream(args[1])));
			System.setIn(in);
			System.setOut(out);
			System.setErr(out);
			BufferedReader br = new BufferedReader(new InputStreamReader(
					System.in));
			String s;
			while ((s = br.readLine()) != null)
				System.out.println(s);
			out.close(); // Remember this!
			System.out.println("complete!");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
} // /:~
