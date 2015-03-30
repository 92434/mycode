package hide;

//Overloading a base-class method name 
//in a derived class does not hide the 
//base-class versions 

class Homer {
	char doh(char c) {
		System.out.println("doh(char)");
		return 'd';
	}

	float doh(float f) {
		System.out.println("doh(float)");
		return 1.0f;
	}
}

class Milhouse {
}

class Bart extends Homer {
	void doh(Milhouse m) {
		System.out.println("xiaofei");
	}

	char doh(char c) {
		System.out.println("hide doh(char)");
		return 'd';
	}
}

class Hide {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Bart b = new Bart();
		b.doh(1); // doh(float) used
		b.doh('x');
		b.doh(1.0f);
		b.doh(new Milhouse());
	}

}
