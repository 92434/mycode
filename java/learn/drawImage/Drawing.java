package drawImage;

import java.awt.*;
import java.awt.event.*;

public class Drawing extends Frame implements MouseListener,
		MouseMotionListener {
	int x0, y0, x, y;
	StringBuffer str[];

	public Drawing() {
		setLayout(null);
		setSize(800, 600);
		setVisible(true);
		addMouseListener(this);
		addMouseMotionListener(this);
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.exit(0);
			}
		});
	}

	public void mouseClicked(MouseEvent e) {
	}

	public void mouseEntered(MouseEvent e) {
	}

	public void mousePressed(MouseEvent e) {
		x0 = e.getX();
		y0 = e.getY();

	}

	public void mouseReleased(MouseEvent e) {
		/*
		 * x1=e.getX(); y1=e.getY();
		 */
	}

	public void mouseExited(MouseEvent e) {
	}

	public void mouseDragged(MouseEvent e) {
		x = e.getX();
		y = e.getY();
		Graphics g = getGraphics();
		g.drawLine(x0, y0, x, y);
		x0 = x;
		y0 = y;
	}

	public void mouseMoved(MouseEvent e) {
	}

	public static void main(String args[]) {
		new Drawing();
	}

}