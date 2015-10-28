package project;

import com.ms.win32.*;

public class UseWin32Package {
	public static void main(String args[]) {
		try {
			User32.MessageBeep(winm.MB_ICONEXCLAMATION);
			User32.MessageBox(0, "Created by the MessageBox() Win32 func",
					"Thinking in Java", winm.MB_OKCANCEL
							| winm.MB_ICONEXCLAMATION);
		} catch (UnsatisfiedLinkError e) {
			System.out.println("Can��t link Win32 API");
			System.out.println(e);
		}
	}
}
