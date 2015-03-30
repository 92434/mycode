/**
 * 
 */
package set;

/**
 * @author Administrator
 * 
 */
// : Synchronization.java
// Using the Collections.synchronized methods
import java.util.*;

public class Synchronization {
	public static void main(String[] args) {
		Collection c = Collections.synchronizedCollection(new ArrayList());
		List list = Collections.synchronizedList(new ArrayList());
		Set s = Collections.synchronizedSet(new HashSet());
		Map m = Collections.synchronizedMap(new HashMap());
		System.out.println(c);
		System.out.println(list);
		System.out.println(s);
		System.out.println(m);
	}
} // /:~
