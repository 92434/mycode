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
        } .getClassName();
        System.out.println(clazzName_);
        System.out.println(new Question().getClassName());
        System.out.println(new Question().getClass().getName());
        System.out.println(System.getProperties());

        System.out.println(Integer.toString(Integer.parseInt("00f0", 16)).getClass());

        byte[] strbyte = null;
        String s_new = null;
        String s = "肖飞";

        try {
            strbyte = s.getBytes("UTF-8");
            s_new = new String(strbyte, "UTF-8");
        } catch(Exception e) {
            e.printStackTrace();
        }

        finally{
        }

		for(byte i : strbyte){
			System.out.println(String.format("%02x", i));
		}
        System.out.println(s_new);
    }
}
