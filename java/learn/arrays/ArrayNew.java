/**
 * 
 */
package arrays;

/**
 * @author Administrator
 * 
 */

// : ArrayNew.java
// Creating arrays with new.
import java.util.*;

public class ArrayNew {
	static Random rand = new Random();

	static int pRand(int mod) {
		return Math.abs(rand.nextInt()) % mod + 1;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int[] a;
		a = new int[pRand(20)];
		prt("length of a = " + a.length);
		for (int i = 0; i < a.length; i++)
			prt("a[" + i + "] = " + a[i]);
	}

	static void prt(String s) {
		System.out.println(s);
	}

}
