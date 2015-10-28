/**
 * 
 */
package chess;

/**
 * @author Administrator
 * 
 */

// : Chess.java
// Inheritance, constructors and arguments
class Game {
	Game(int i) {
		System.out.println("Game constructor");
	}
}

class BoardGame extends Game {
	BoardGame(int i) {
		super(i);
		System.out.println("BoardGame constructor");
	}
}

public class Chess extends BoardGame {

	Chess() {
		super(11);
		System.out.println("Chess constructor");
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Chess x = new Chess();
	}

}
