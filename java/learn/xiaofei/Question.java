package xiaofei;


public class Question {

	public String getClassName() {
		return this.getClass().getName();
	}

	/**
	 * @param args
	 * @throws MalformedURLException
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		String clazzName_ = new Object() {
			public String getClassName() {
				String clazzName = this.getClass().getName();
				return clazzName;// .substring(0, clazzName.lastIndexOf('$'));
			}
		}.getClassName();
		System.out.println(clazzName_);
		System.out.println(new Question().getClassName());
		System.out.println(new Question().getClass().getName());
		System.out.println(System.getProperties());
		
		System.out.println(Integer.toString(Integer.parseInt("00f0", 16)).getClass());
	}
}
