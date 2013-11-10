package wordLadder;

import java.io.FileReader;
import java.io.IOException;
import java.util.LinkedList;
import java.util.Scanner;

/**
 * 
 * @author Anton Belev
 *
 */

/**
 program to find word ladder with shortest distance for two words in a dictionary
 distance between elements of the word ladder is the absolute difference in the
 positions of the alphabet of the non-matching letter
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
			//System.out.println("New node created: " + currVertex);						
			for (Vertex vertex : graph.getVertices())
			{
				if (Graph.checkAdjacency(currVertex.getWord(), vertex.getWord()))
				{
					//currVertex.setAdjList((LinkedList<AdjListNode>)vertex.getAdjList().clone()); // currVertex should have the same adjacency list as the vertex + the vertex
					currVertex.addToAdjList(vertex.getIndex()); 
					vertex.addToAdjList(currVertex.getIndex());
				}
			}
			graph.addToList(currVertex);
			
		}
		reader.close();
		
		/*for (Vertex v : graph.getVertices())
		{
			System.out.println("Graph vertex: " + v);
		}*/
		
		graph.carryOutWordLadder(word1, word2);
		
		// create graph here
		
		
		// do the work here
		// do bfs starting from word1 and ending at word2
		

		// end timer and print total time
		long end = System.currentTimeMillis();
		System.out.println("\nElapsed time: " + (end - start) + " milliseconds");
	}

}
