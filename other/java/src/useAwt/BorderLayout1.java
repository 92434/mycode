/**
 * 
 */
package useAwt;

/**
 * @author Administrator
 * 
 */
// : BorderLayout1.java
// Demonstrating the BorderLayout
import java.awt.*;
import java.applet.*;

public class BorderLayout1 extends Applet {
	// private static final long serialVersionUID = -5836846270535785031L;
	public void init() {
		int i = 0;
		setLayout(new BorderLayout());
		add("North", new Button("Button " + i++));
		add("South", new Button("Button " + i++));
		add("East", new Button("Button " + i++));
		add("West", new Button("Button " + i++));
		add("Center", new Button("Button " + i++));
	}
} // /:~
