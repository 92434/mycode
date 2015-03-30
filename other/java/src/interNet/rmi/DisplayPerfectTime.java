package interNet.rmi;

//: DisplayPerfectTime.java 
//Uses remote object PerfectTime 
import java.rmi.*;
import java.rmi.registry.*;

public class DisplayPerfectTime {
	public static void main(String[] args) {
		System.setSecurityManager(new RMISecurityManager());
		try {
			PerfectTimeI t = (PerfectTimeI) Naming
					.lookup("//24D6BB2FAD48452.:2005/PerfectTime");
			for (int i = 0; i < 10; i++)
				System.out.println("Perfect time = " + t.getPerfectTime());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
} // /:~
