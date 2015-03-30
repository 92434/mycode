package xiaofei;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class GridBagWindow extends JFrame {
	private JButton searchBtn;
	private JComboBox modeCombo;
	private JLabel tagLbl;
	private JLabel tagModeLbl;
	private JLabel previewLbl;
	private JTable resTable;
	private JTextField tagTxt;

	public GridBagWindow() {
		Container contentPane = getContentPane();
		GridBagLayout gridbag = new GridBagLayout();
		contentPane.setLayout(gridbag);
		GridBagConstraints c = new GridBagConstraints();
		// setting a default constraint value
		c.fill = GridBagConstraints.HORIZONTAL;
		tagLbl = new JLabel("Tags");
		c.gridx = 0; // x grid position
		c.gridy = 0; // y grid position
		gridbag.setConstraints(tagLbl, c); // associate the label with a
											// constraint object
		contentPane.add(tagLbl); // add it to content pane

		tagModeLbl = new JLabel("Tag Mode");
		c.gridx = 0;
		c.gridy = 1;
		gridbag.setConstraints(tagModeLbl, c);
		contentPane.add(tagModeLbl);
		tagTxt = new JTextField("plinth");
		c.gridx = 1;
		c.gridy = 0;
		c.gridwidth = 2;
		gridbag.setConstraints(tagTxt, c);
		contentPane.add(tagTxt);
		String[] options = { "all", "any" };
		modeCombo = new JComboBox(options);
		c.gridx = 1;
		c.gridy = 1;
		c.gridwidth = 1;
		gridbag.setConstraints(modeCombo, c);
		contentPane.add(modeCombo);
		searchBtn = new JButton("Search");
		c.gridx = 1;
		c.gridy = 2;
		gridbag.setConstraints(searchBtn, c);
		contentPane.add(searchBtn);
		resTable = new JTable(5, 3);
		c.gridx = 0;
		c.gridy = 3;
		c.gridwidth = 3;
		gridbag.setConstraints(resTable, c);
		contentPane.add(resTable);
		previewLbl = new JLabel("Preview goes here");
		c.gridx = 0;
		c.gridy = 4;
		gridbag.setConstraints(previewLbl, c);
		contentPane.add(previewLbl);
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.exit(0);
			}
		});
	}

	public static void main(String args[]) {
		GridBagWindow window = new GridBagWindow();
		window.setTitle("GridBagWindow");
		window.pack();
		window.setVisible(true);
	}
}
