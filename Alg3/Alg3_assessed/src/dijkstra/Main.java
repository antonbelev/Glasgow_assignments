package dijkstra;

import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

import dijkstra.Graph;
import dijkstra.Vertex;

/**
 * 
 * @author Anton Belev
 *
 */

/**
 program to find shortest path using Dijkstra's algorithm
 */
public class Main {

	public static void main(String[] args) throws IOException {

		long start = System.currentTimeMillis();

		String inputFileName = args[0]; // dictionary
		String word1 = args[1]; // first word
		String word2 = args[2]; // second word
  
		FileReader reader = new FileReader(inputFileName);
		Scanner in = new Scanner(reader);		
		Scanner lineScanner;
		Graph graph = new Graph();
		while (in.hasNextLine())
		{
			lineScanner = new Scanner(in.nextLine());
			Vertex currVertex = new Vertex(lineScanner.next(), graph.getVertices().size());				
			for (Vertex vertex : graph.getVertices())
			{
				if (Graph.checkAdjacency(currVertex.getWord(), vertex.getWord()))
				{
					int weight = Graph.calcualteWeight(vertex.getWord(), currVertex.getWord());
					currVertex.addToAdjList(vertex.getIndex(), weight); 
					vertex.addToAdjList(currVertex.getIndex(), weight);
				}
			}
			graph.addToList(currVertex);
			
		}
		in.close();
		reader.close();
		
		
		graph.carryOutDijkstra(word1, word2);

		// end timer and print total time
		long end = System.currentTimeMillis();
		System.out.println("\nElapsed time: " + (end - start) + " milliseconds");
	}

}
