package test;

class A {
	int i = 5;
	static int si = 1;

	A() {
	}

	A(int i) {
		this.i = i;
		si = 2;
		System.out.println(si);
	}

	A(int i1, int i2) {
		i = i1;
		si = i2;
	}

	static void f() {
		si++;
		System.out.println("xiaofei " + si);
	}

	static class AInner {
		static int i = 35;
		int j = 8;

		static void f() {
			i++;
			System.out.println(i);
		}

		void NonStatic() {
			System.out.println(j);
		}
	}

	class AAInner {
		int i = 7;
	}
}

class B extends A {

}

public class MyTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		A a = new A(1);
		System.out.println(a.i);
		System.out.println(A.si);
		A.AAInner aa;
		aa = a.new AAInner();
		A b = new A(2, 6);
		System.out.println(b.i);
		b.f();
		A.f();
		A.AInner.f();
		A.AInner.f();
		A.AInner AInnerI = new A.AInner();
		AInnerI.NonStatic();

		B bI = new B();
		System.out.println(bI.i);
		System.out.println(bI.si);
		System.out.println(new A().new AAInner().i++);
	}

}
