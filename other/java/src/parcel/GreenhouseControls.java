/**
 * 
 */
package parcel;

/**
 * @author Administrator
 * 
 */
// : GreenhouseControls.java
// This produces a specific application of the
// control system, all in a single class. Inner
// classes allow you to encapsulate different
// functionality for each type of event.
public class GreenhouseControls extends Controller {
	private boolean light = false;

	private boolean water = false;

	private String thermostat = "Day";

	static private int countForEvent;

	private class LightOn extends Event {
		public LightOn(long eventTime) {
			super(eventTime);
		}

		public void action() {
			// Put hardware control code here to
			// physically turn on the light.
			light = true;
		}

		public String description() {
			return "Light is on";
		}
	}

	private class LightOff extends Event {
		public LightOff(long eventTime) {
			super(eventTime);
		}

		public void action() {
			// Put hardware control code here to
			// physically turn off the light.
			light = false;
		}

		public String description() {
			return "Light is off";
		}
	}

	private class WaterOn extends Event {
		public WaterOn(long eventTime) {
			super(eventTime);
		}

		public void action() {
			// Put hardware control code here
			water = true;
		}

		public String description() {
			return "Greenhouse water is on";
		}
	}

	private class WaterOff extends Event {
		public WaterOff(long eventTime) {
			super(eventTime);
		}

		public void action() {
			// Put hardware control code here
			water = false;
		}

		public String description() {
			return "Greenhouse water is off";
		}
	}

	private class ThermostatNight extends Event {
		public ThermostatNight(long eventTime) {
			super(eventTime);
		}

		public void action() {
			// Put hardware control code here
			thermostat = "Night";
		}

		public String description() {
			return "Thermostat on night setting";
		}
	}

	private class ThermostatDay extends Event {
		public ThermostatDay(long eventTime) {
			super(eventTime);
		}

		public void action() {
			// Put hardware control code here
			thermostat = "Day";
		}

		public String description() {
			return "Thermostat on day setting";
		}
	}

	// An example of an action() that inserts a
	// new one of itself into the event list:
	private int rings;

	private class Bell extends Event {
		public Bell(long eventTime) {
			super(eventTime);
		}

		public void action() {
			// Ring bell every 2 seconds, rings times:
			System.out.println("Bing!");
			if (--rings > 0)
				addEvent(new Bell(System.currentTimeMillis() + 200),
						++countForEvent);
		}

		public String description() {
			return "Ring bell";
		}
	}

	private class Restart extends Event {
		public Restart(long eventTime) {
			super(eventTime);
		}

		public void action() {
			long tm = System.currentTimeMillis();
			// Instead of hard-wiring, you could parse
			// configuration information from a text
			// file here:
			rings = 5;
			addEvent(new ThermostatNight(tm), ++countForEvent);
			addEvent(new LightOn(tm + 100), ++countForEvent);
			addEvent(new LightOff(tm + 200), ++countForEvent);
			addEvent(new WaterOn(tm + 300), ++countForEvent);
			addEvent(new WaterOff(tm + 800), ++countForEvent);
			addEvent(new Bell(tm + 900), ++countForEvent);
			addEvent(new ThermostatDay(tm + 1000), ++countForEvent);
			// Can even add a Restart object!
			addEvent(new Restart(tm + 2000), ++countForEvent);
		}

		public String description() {
			return "Restarting system";
		}
	}

	public static void main(String[] args) {
		GreenhouseControls gc = new GreenhouseControls();
		long tm = System.currentTimeMillis();
		gc.addEvent(gc.new Restart(tm), ++countForEvent);
		gc.run();
	}
} // /:~
