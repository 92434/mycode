package project;

public class ShowMsgBox {
	public static void main(String args[]) throws UnsatisfiedLinkError {
		MessageBox(0, "Created by the MessageBox() Win32 func",
				"Thinking in Java", 0);
	}

	/** @dll.import("USER32") */
	private static native int MessageBox(int hwndOwner, String text,
			String title, int fuStyle);
}
