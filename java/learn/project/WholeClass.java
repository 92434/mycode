package project;

/** @dll.import("USER32") */
class MyUser32Access {
	public static native int MessageBox(int hwndOwner, String text,
			String title, int fuStyle);

	public native static boolean MessageBeep(int uType);
}

public class WholeClass {
	public static void main(String args[]) throws UnsatisfiedLinkError {
		MyUser32Access.MessageBeep(4);
		MyUser32Access.MessageBox(0, "Created by the MessageBox() Win32 func",
				"Thinking in Java", 0);
	}
}