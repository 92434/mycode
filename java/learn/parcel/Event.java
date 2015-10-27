/**
 * 
 */
package parcel;

/**
 * @author Administrator
 * 
 */
// : Event.java
// The common methods for any control event
abstract public class Event {
	private long evtTime;

	public Event(long eventTime) {
		evtTime = eventTime;
	}

	public boolean ready() {
		return System.currentTimeMillis() >= evtTime;
	}

	abstract public void action();

	abstract public String description();
} // /:~
