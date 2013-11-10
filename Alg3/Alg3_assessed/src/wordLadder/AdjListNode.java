package wordLadder;

/**
 * 
 * @author Anton Belev
 *
 */

public class AdjListNode {
	
	private int vertexNumber;

	public AdjListNode(int n){
		vertexNumber = n;
	}
	
	public int getVertexNumber(){
		return vertexNumber;
	}
	
	@Override
	public String toString() {
		return "AdjListNode [vertexNumber=" + vertexNumber + "]";
	}

	public void setVertexNumber(int n){
		vertexNumber = n;
	}
	
}
