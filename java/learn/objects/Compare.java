/**
 * 
 */
package objects;

/**
 * @author Administrator
 * 
 */
// : Compare.java
// Interface for sorting callback:
public interface Compare {
	boolean lessThan(Object lhs, Object rhs);

	boolean lessThanOrEqual(Object lhs, Object rhs);
} // /:~
