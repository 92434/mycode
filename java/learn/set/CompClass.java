/**
 * 
 */
package set;

/**
 * @author Administrator
 * 
 */
// : CompClass.java
// A class that implements Comparable
import java.util.*;

public class CompClass implements Comparable {
	private int i;

	public CompClass(int ii) {
		i = ii;
	}

	public int compareTo(Object o) {
		// Implicitly tests for correct type:
		int argi = ((CompClass) o).i;
		if (i == argi)
			return 0;
		if (i < argi)
			return -1;
		return 1;
	}

	public static void print(Object[] a) {
		for (int i = 0; i < a.length; i++)
			System.out.print(a[i] + " ");
		System.out.println();
	}

	public String toString() {
		return i + "";
	}

	public static void main(String[] args) {
		CompClass[] a = new CompClass[20];
		for (int i = 0; i < a.length; i++)
			a[i] = new CompClass((int) (Math.random() * 100));

		// String[] a = Array1.randStrings(4, 10);
		// Array1.print(a);

		print(a);
		Arrays.sort(a);
		print(a);
		int loc = Arrays.binarySearch(a, a[3]);
		System.out.println("Location of " + a[3] + " = " + loc);
	}
} // /:~
