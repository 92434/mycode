package xiaofei;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;

public class Package {

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
        System.out.println(new Package().getClassName());
        System.out.println(new Package().getClass().getName());
        System.out.println(System.getProperties());

        try {
            System.out.println(new URL("file:/./Package.class"));
        } catch (MalformedURLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        try {
            System.out.println(new URL("file:./Package.class"));
        } catch (MalformedURLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        try {
            System.out.println(new URL("file:/./*123"));
        } catch (MalformedURLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        try {
            System.out.println(Package.class.getResource("./Package.class")
                               .openConnection().getInputStream());
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}
