/**
 * 
 */
package swing;

/**
 * @author Administrator
 * 
 */
// : Faces.java
// Icon behavior in JButtons
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;

public class Faces extends JPanel {
	public static Icon[] faces = { new ImageIcon("swing/face0.gif"),
			new ImageIcon("swing/face1.gif"), new ImageIcon("swing/face2.gif"),
			new ImageIcon("swing/face3.gif"), new ImageIcon("swing/face4.gif"), };

	JButton jb = new JButton("JButton", faces[3]),
			jb2 = new JButton("Disable");

	boolean mad = false;

	public Faces() {
		jb.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (mad) {
					jb.setIcon(faces[3]);
					mad = false;
				} else {
					jb.setIcon(faces[0]);
					mad = true;
				}
				jb.setVerticalAlignment(JButton.TOP);
				jb.setHorizontalAlignment(JButton.LEFT);
			}
		});
		jb.setRolloverEnabled(true);
		jb.setRolloverIcon(faces[1]);
		jb.setPressedIcon(faces[2]);
		jb.setDisabledIcon(faces[4]);
		jb.setToolTipText("Yow!");
		add(jb);
		jb2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (jb.isEnabled()) {
					jb.setEnabled(false);
					jb2.setText("Enable");
				} else {
					jb.setEnabled(true);
					jb2.setText("Disable");
				}
			}
		});
		add(jb2);
	}

	public static void main(String args[]) {
		Show.inFrame(new Faces(), 300, 200);
	}
} // /:~
