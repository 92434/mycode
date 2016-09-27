package com.xiaofei.test;

public class StaticClassMember {

    public static class A {
        int mI = 0;

        public A() {
            setValue(0);
        }

        public A(int i) {
            setValue(i);
        }

        void setValue(int i) {
            mI = i;
        }

        int getValue() {
            return mI;
        }
    }

    public static class B {
        static int mI = 0;

        public B() {
            setValue(0);
        }

        public B(int i) {
            setValue(i);
        }

        void setValue(int i) {
            mI = i;
        }

        int getValue() {
            return mI;
        }
    }

    public static void main(String[] args) {
        StaticClassMember o = new StaticClassMember();

        System.exit(o.testStaticClassMember());
    }

    public int testStaticClassMember() {
        A a0 = new A(0);
        A a1 = new A(1);
        A a2 = new A(2);

        System.out.println(a0.getValue());
        System.out.println(a1.getValue());
        System.out.println(a2.getValue());

        B b0 = new B(0);
        B b1 = new B(1);
        B b2 = new B(2);

        System.out.println(b0.getValue());
        System.out.println(b1.getValue());
        System.out.println(b2.getValue());
	return 0;
    }
}
