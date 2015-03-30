package xiaofei;

import java.awt.*;

//输入所有的java.awt 类

public class WindowTest extends java.applet.Applet {

	/**
	 * 
	 */
	private static final long serialVersionUID = -4016905607082530305L;

	public void init() {

		resize(300, 100);// 设置窗口的大小

		GridBagConstraints gbc = new GridBagConstraints();// 使用类GridBagConstriants

		setLayout(new GridBagLayout());// 设定外观管理器为 GridBagLayout外观管理器

		gbc.fill = GridBagConstraints.BOTH;// ★所有的按钮都会把分配的剩余空间填满

		gbc.gridwidth = 1;// ★设置第一个按钮的大小

		gbc.gridheight = 1;// ★

		// Button Button1 = new Button("东");
		Button Button1 = new Button("east");

		((GridBagLayout) getLayout()).setConstraints(Button1, gbc);

		add(Button1);

		gbc.gridwidth = GridBagConstraints.REMAINDER;// ★第二个按钮填满整行空间

		// Button Button2 = new Button("西");
		Button Button2 = new Button("west");

		((GridBagLayout) getLayout()).setConstraints(Button2, gbc);

		add(Button2);

		gbc.gridheight = 4;// 设置第三个按钮的大小

		gbc.gridwidth = 1;

		// Button Button3 = new Button("南");
		Button Button3 = new Button("south");

		((GridBagLayout) getLayout()).setConstraints(Button3, gbc);

		add(Button3);

		gbc.gridheight = 2;// 设置第四个按钮的大小

		gbc.gridwidth = 2;// GridBagConstraints.REMAINDER;

		// Button Button4 = new Button("北");
		Button Button4 = new Button("north");

		((GridBagLayout) getLayout()).setConstraints(Button4, gbc);

		add(Button4);

		gbc.gridwidth = GridBagConstraints.REMAINDER;

		// Button Button5 = new Button("中");
		Button Button5 = new Button("middle");

		((GridBagLayout) getLayout()).setConstraints(Button5, gbc);

		add(Button5);

		gbc.insets = new Insets(5, 6, 7, 8);// ★设置第五个按钮的位置

		// Button Button6 = new Button("好酒在张弓");
		Button Button6 = new Button("ok!!!!!!");

		((GridBagLayout) getLayout()).setConstraints(Button6, gbc);
		add(Button6);
	}
}