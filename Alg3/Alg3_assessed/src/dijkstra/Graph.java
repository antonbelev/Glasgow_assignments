package dijkstra;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.PriorityQueue;
import java.util.Stack;

/**
 * 
 * @author Anton Belev
 *
 */

public class Graph {
	private ArrayList<Vertex> vertices;

	// private int size;

	public Graph() {
		// this.size = 0;
		this.vertices = new ArrayList<Vertex>();
	}

	public ArrayList<Vertex> getVertices() {
		return vertices;
	}

	public void setVertices(ArrayList<Vertex> vertices) {
		this.vertices = vertices;
	}

	public void addToList(Vertex v) {
		this.vertices.add(v);
	}

	public static boolean checkAdjacency(String s1, String s2) {
		int countDiff = 0;
		for (int i = 0; i < s1.length(); i++) {
			if (s1.charAt(i) != s2.charAt(i))
				countDiff++;
			if (countDiff > 1)
				return false;
		}
		return true;
	}

	public void carryOutDijkstra(String start, String end) {
		Vertex vEnd = dijkstra(start, end);
		if (vEnd == null){
			System.out.println(start + " " + end + "\nNo ladder is possible!");
			return;
		}

		Stack<String> path = new Stack<String>();
		
		int currIndex = vEnd.getIndex();
		int predecessorIndex = vEnd.getPredecessor();
		String output = start + " " + end + "\nTotal weight " + vEnd.getDistance();

		while (currIndex != predecessorIndex)
		{
			Vertex current = vertices.get(currIndex);
			path.push(current.getWord());
			currIndex = predecessorIndex;
			predecessorIndex = vertices.get(currIndex).getPredecessor();
		}
		
		Vertex current = vertices.get(currIndex);
		path.push(current.getWord());
		
		String pathTrase = "";
		while (path.size() != 0){
			if (path.size() > 1)
				pathTrase += path.pop() + "->";
			else
				pathTrase += path.pop();
		}
		System.out.println(output + "\n" + pathTrase);
		
	}
	
	public class VertexComparator implements Comparator<Vertex>
	{
	    @Override
	    public int compare(Vertex x, Vertex y)
	    {
	        if (x.getDistance() < y.getDistance())
	        {
	            return -1;
	        }
	        if (x.getDistance() > y.getDistance())
	        {
	            return 1;
	        }
	        return 0;
	    }
	}

	public Vertex dijkstra(String start, String end) {
		Vertex vStart = new Vertex();
		Vertex vEnd = new Vertex();
		for (Vertex v : vertices) {
			if (v.getWord().equalsIgnoreCase(start))
				vStart = v;
			else if (v.getWord().equalsIgnoreCase(end))
				vEnd = v;
			v.setDistance(-1);
			v.setVisited(false);
		}
		if (vStart.isEmpty() || vEnd.isEmpty())
			return null;
		
		Comparator<Vertex> comparator = new VertexComparator();
		
		PriorityQueue<Vertex> queue = new PriorityQueue<Vertex>(128, comparator);
		
		vStart.setVisited(true);
		vStart.setDistance(0); // initialize distance
		vStart.setPredecessor(vStart.getIndex()); // v was initial/starting
													// vertex
		queue.add(vStart);
		
		while (!queue.isEmpty()) { // while vertices to process
			Vertex u = queue.remove(); // get next vertex to process
			LinkedList<AdjListNode> list = u.getAdjList(); // get adjacency list of the vertex
			
			for (AdjListNode node : list) { // go through the adjacency list...
				Vertex w = vertices.get(node.getVertexNumber());				
				if (!w.isVisited()) { // ...for vertices that have not been
										// visited
					w.setVisited(true); // they have now been visited
					w.setPredecessor(u.getIndex());
					w.setDistance(u.getDistance() + node.getWeight());
					queue.add(w); 
				}
				if (w.equals(vEnd))
					return vEnd;
			}
		}
		
		return null;
	}

	public static int calcualteWeight(String s1, String s2) {
		for (int i = 0; i < s1.length(); i++) { // I assume that the two strings differ in only one letter
			if (s1.charAt(i) != s2.charAt(i))   // since I've already checked if they are adjacent
				return Math.abs((int)s1.charAt(i) - (int)s2.charAt(i));
		}
		return 0;
	}

}
