package dijkstra;

/**
 * 
 * @author Anton Belev
 *
 */

public class AdjListNode {
	
	private int vertexNumber;
	private int weight;

	public AdjListNode(int n){
		vertexNumber = n;
		weight = -1;
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
	
	public int getWeight(){
		return weight;
	}
	
	public void setWeight(int w){
		weight = w;
	}
	
}
