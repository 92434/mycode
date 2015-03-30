package testObject;

import java.nio.ByteBuffer;
import java.util.Vector;

public class TestObject {
	Vector<ByteBuffer> vector = new Vector<ByteBuffer>();

	protected class A {
		A() {
			vector.addElement(ByteBuffer.allocate(5));
			System.out.println(vector.size());
			vector.get(0).putChar(3, '肖');
			vector.addElement(ByteBuffer.allocate(5));
			System.out.println(vector.size());
			vector.get(1).putChar(3, '飞');
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		TestObject obj = new TestObject();
		A a = obj.new A();
		obj.vector.size();
		System.out.println(obj.vector.get(0).getChar(3));
		System.out.println(obj.vector.get(1).getChar(3));
	}

}
