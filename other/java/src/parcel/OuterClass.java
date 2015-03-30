/**
 * 
 */
package parcel;

/**
 * @author Administrator
 * 
 */
public class OuterClass {

	private int i_outer = 1;

	class InnerClass {
		void testInner() {
			System.out.println(OuterClass.this.i_outer);
		}
	};

	void testOuter() {
		final int iOuterVar1 = 1;
		final int[] iszOuterVar = new int[1];

		class InnerClass1 {
			void testInner() {
				System.out.println(iOuterVar1);
				System.out.println(iszOuterVar[0]++);
				System.out.println(iszOuterVar[0]++);
				System.out.println(iszOuterVar[0]++);
				System.out.println(iszOuterVar[0]++);
				System.out.println(iszOuterVar[0]++);

			}
		}

		InnerClass innerO = this.new InnerClass();
		InnerClass1 innerO1 = new InnerClass1();
		innerO1.testInner();

		innerO.testInner();

	}

	void anonymousClassTest() {
		Anonymous a = new /**
		 * @author Administrator
		 * 
		 */
		Anonymous() {
			void testAnonymous() {
				System.out.println("xiaofei1");
			}
		};

		a.testAnonymous();
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		InnerClass inner_o = new OuterClass().new InnerClass();
		inner_o.testInner();
		new OuterClass().testOuter();

		new OuterClass().anonymousClassTest();
	}

}
