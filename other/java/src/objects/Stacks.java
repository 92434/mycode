/**
 * 
 */
package objects;

/**
 * @author Administrator
 * 
 */
// : Stacks.java
// Demonstration of Stack Class
import java.util.*;

public class Stacks {
	static String[] months = { "January", "February", "March", "April", "May",
			"June", "July", "August", "September", "October", "November",
			"December" };

	public static void main(String[] args) {
		Stack stk = new Stack();
		for (int i = 0; i < months.length; i++)
			stk.push(months[i] + " ");
		System.out.println("stk = " + stk);
		// Treating a stack as a Vector:
		stk.addElement("The last line");
		System.out.println("element 5 = " + stk.elementAt(5));
		System.out.println("element 1 = " + stk.elementAt(1));
		System.out.println("element 2 = " + stk.elementAt(2));
		System.out.println("element 3 = " + stk.elementAt(3));
		System.out.println("popping elements:");
		while (!stk.empty())
			System.out.println(stk.pop());
	}
} // /:~
