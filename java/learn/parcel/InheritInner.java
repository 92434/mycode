/**
 * 
 */
package parcel;

/**
 * @author Administrator
 * 
 */
// : InheritInner.java
// Inheriting an inner class
class WithInner {
	class Inner {
		Inner() {
			System.out.println("xiaofei:Inner is contructed!");
		}
	}
}

public class InheritInner extends WithInner.Inner {
	// ! InheritInner() {} // Won't compile
	InheritInner(WithInner wi) {
		wi.super();
		System.out.println("xiaofie subclass contructed!");
	}

	public static void main(String[] args) {
		WithInner wi = new WithInner();
		InheritInner ii = new InheritInner(wi);
	}
} // /:~
