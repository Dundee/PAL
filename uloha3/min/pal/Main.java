package pal;

import pal.domain.*;
import pal.heap.BinomialHeap;
import pal.disjointset.DisjointSet;
import pal.forrest.*;
import java.io.*;
import java.util.*;

public class Main {
	/** set this to true if you need the debugging printouts */
	public static final boolean DEBUG = false;

	/** set this to true if you want to load the input data from file */
	public static final boolean INPUT_FILE = false;
	
	/** file for input */
	public static final String FILE = "D:/Paja/CVUT/4_ctvrtak/1_semestr/PAL/" +
										"ulohy/03/Minimalni_kostra/input1.txt";
//	public static final String FILE = "D:/Paja/CVUT/4_ctvrtak/1_semestr/PAL/" +
//										"ulohy/03/Minimalni_kostra/post_graphs/graph_carmerini.in";

	/** the edges of the newly created cycle between components in S */
	private static ArrayList<Edge> C;

	/*
	 * Cycle contains the edges containing the cycles of the strongly connected
	 * components of the critical graph.
	 */
	private static ArrayList<Edge>[] cycle;


	/**
	 * Enter keeps track of the incoming edges of the strongly connected components
	 * in the critical graph. If v is the representative of the strongly
	 * connected component Sv, then enter[v] is the edge of the critical
	 * graph that points into Sv (or empty if no such edge exists).
	 */
	private static Edge[] enter;

	/*
	 * We use a rooted forrest of edges of G to keep track of the edges currently
	 * in the critical graph. Each time an edge is conceptually chosen as a critical edge,
	 * it is also added to F. If the edge points into a set of S
	 * that consists of more than one vertex of G, i.e., the set in S corresponds to
	 * a cycle, then the edges in the cycle become the children of the newly added
	 * edge in F. In this way, the roots of F are the edges of the current critical
	 * graph of the graph whose vertices are the sets in S.
	 */
	private static Forrest F;

	/**
	 * Incoming edges of the node n, thanks to the heap we are able to pick
	 * the minimal edge in O( log2(n) )
	 */
	private static BinomialHeap[] I;


	/**
	 * Lamda is an array of pointers, pointing to leaves of F. More specifically, if v is from V,
	 * then lambda[v] is the leaf of F whose head is v. If no such leaf exists, then
	 * lambda[v] is empty.
	 */
	private static ForrestElement[] lambda;

	private static final String LINE_SEPARATOR = System.getProperty("line.separator");

	/**
	 * contains possible roots of the MSA. If a supervertex represented by v
	 * has been chosen as root at the end of the algorithm, then min[v] is the
	 * vertex of G that will be the root of the MSA
	 */
	private static int[] min;

	/** Number of nodes in the graph */
	private static int NODES_COUNT;

	/**
	 * Root is the vertex that has been identifed as the final root in the critical
	 * graph. Note that this may be a supervertex, in which case min[root]
	 * is the vertex of G that is chosen as the root of the MSA.
	 */
	private static int root = -1;

	/**
	 * A Queue roots contains the strongly connected components that are roots in the
	 * current critical graph, i.e., no incoming edge has been chosen for these
	 * vertices/supervertices.
	 */
	private static Queue<Integer> roots;

	/**
	 * S is contains all current supervertices (strongly connected components in).
	 * When a cycle has been created, the sets corresponding to the vertices of
	 * the cycle are unioned into one component in S.
	 */
	private static DisjointSet S;


	private static StringBuilder output;

	/** vertices */
	private static int[] V;

	/*
	 * W is used for weakly connected components. Each vertex of G belongs to one
	 * weakly connected component in W. Hence, the addition of an edge (u, v).
	 */
	private static DisjointSet W;

	/** Guards duplicated edges. */
	private static Hashtable<String, Edge> inEdges;

	//--------------------------------------------------------------------------
	/** Builds a graph with its nodes and edges. */
	public static void createGraph() throws IOException {
		// read the first line of the input and get the number of the nodes in the graph and create it
		BufferedReader in = null;
		if( INPUT_FILE ){		// load input from a file
			if( DEBUG ){ System.out.println("reading input from file"); }
			try{
				in = new BufferedReader( new FileReader(FILE) );
				String input = "";
				String line = "";
				while( (line=in.readLine())!=null ){
					input+= line + System.getProperty("line.separator");
				}
				in.close();
				if( DEBUG ){ System.out.println("input:\n" + input); }
				in = new BufferedReader( new StringReader(input));
			}
			catch(Exception e){
				if( DEBUG ){ System.out.println("Error when reading input file."); }
			}
		}
		else{		// standard input stream
			if( DEBUG ){ System.out.println("reading input from console"); }
			in = new BufferedReader( new InputStreamReader(System.in) );
		}

		// init Vertices and Edges
		NODES_COUNT = Integer.parseInt(in.readLine());
		if( DEBUG ){ System.out.println("nodesCount=" + NODES_COUNT); }
		V = new int[NODES_COUNT];
		I = new BinomialHeap[NODES_COUNT];
		inEdges = new Hashtable(NODES_COUNT/2);

		String line = "";

		// read the nodes and edges from the input
		while( !(line = in.readLine()).equals("0 0 0") ){
			StringTokenizer token = new StringTokenizer(line);
			int nodeTailIndex = Integer.parseInt(token.nextToken());
			int nodeHeadIndex = Integer.parseInt(token.nextToken());
			int weight = Integer.parseInt(token.nextToken());

			V[nodeTailIndex] = nodeTailIndex;
			V[nodeHeadIndex] = nodeHeadIndex;
			Edge e = new Edge( V[nodeTailIndex], V[nodeHeadIndex], weight );
			storeIncomingEdge(e);
		}
		in.close();
		in = null;

		// we eliminated the duplicities in edges so that we could fill the collection I
		for (Iterator<Edge> it = inEdges.values().iterator(); it.hasNext();) {
			Edge edge = it.next();
			int headIndex = edge.getHead();

			// the node doesn't have any incoming edges yet, we must init the heap for them
			if( I[headIndex]==null ){
				if( DEBUG ){ System.out.println("Creating new set of incomming edges for node " + headIndex); }
				I[headIndex] = new BinomialHeap();
			}

			I[headIndex].insert(edge);
		}
		inEdges = null;
	}

	//--------------------------------------------------------------------------
	/** Stores an incoming edge e. */
	private static void storeIncomingEdge( Edge e ){
		//final int headIndex = e.getHead().getIndex();
		if( DEBUG ){ System.out.println("Parsing edge " + e); }

		Edge oldEdge = inEdges.get(e.hash());
		if( oldEdge!=null ){	// e and oldEdge connect the same nodes
			if(DEBUG){System.out.println("We found an identical edge " + oldEdge);}
			if( e.getWeight()<oldEdge.getWeight() ){	// new edge is cheaper, update the old one
				if( DEBUG ){ System.out.println("Updating edge weight to " + e.getWeight()); }
				oldEdge.setWeight(e.getWeight());
			}
			else{
				if( DEBUG ){ System.out.println("Edge exists, weight needs no updating."); }
			}
		}
		else{	// the edge is new, we should store it
			if( DEBUG ){ System.out.println("Edge doesn't exist. Store it."); }
			inEdges.put(e.hash(), e);
			//I[headIndex].insert(e);
		}
	}

	//--------------------------------------------------------------------------
	public static void main(String[] args) throws Exception {
		long start = System.nanoTime();		// measure the time
		createGraph();

		C = new ArrayList<Edge>();
		cycle = new ArrayList[NODES_COUNT];
		enter = new Edge[NODES_COUNT];
		F = new Forrest();
		lambda = new ForrestElement[NODES_COUNT];
		min = new int[NODES_COUNT];
		output = new StringBuilder(256);
		roots = new LinkedList<Integer>();
		S = new DisjointSet(NODES_COUNT);
		W = new DisjointSet(NODES_COUNT);

		// initialize and fill the data structs
		for(int v : V){
			// do we need to sort the I by edges' tails?
			W.makeSet(v);
			S.makeSet(v);
			min[v] = v;
			roots.add(v);
		}
		
		while( !roots.isEmpty() ){
			int r = roots.remove();

			/* No incomming edges for node r. We probably have something to do with a root. */
			if( I[r]==null || I[r].size()==0 ){
				if(DEBUG){ System.out.println("Root " + r + " has no incomming edges."); }

				if( root==-1 ){	// r is supposed to be our first root
					root = r;
				}
				else{				// there are two roots, the graph is not connected
					if(DEBUG){ System.out.println("Graph is not connected."); }
					System.out.println("-1");
					return;
				}
				continue;
			}

			Edge minEdge = I[r].popMin();
			final int u = minEdge.getTail();
			final int v = minEdge.getHead();

			// Insert minEdge as a node in F with any edges in cycle[r] as children.
			// Cycle contains the edges containing the cycles of the strongly connected
			// components of the critical graph.
			ForrestElement n = F.add(minEdge);
			if( cycle[r]==null || cycle[r].isEmpty() ){
				lambda[v] = n;		// points to the leaf of the forrest
			}
			else{
				for( Edge child : cycle[r] ){
					n.addChild(child);
				}
			}

			int findU = W.find(u);
			int findV = W.find(v);
			if( findU!=findV ){
				W.union(findU, findV);
				enter[r] = minEdge;
			}
			else{
				C.clear();
				C.add(minEdge);
				Edge e = minEdge;

				while( true ) {
					e = enter[ S.find(e.getTail()) ];
					if( e==null ){
						break;
					}
					C.add(e);
					if (S.find(e.getTail()) == S.find(e.getHead())){
						break;
					}
				}

				// find the edge in the cycle with a maximal weight
				Edge C_maxEdge = minEdge;
				for( Edge tmpEdge : C ){
					if( tmpEdge.getIncreasedWeight()>C_maxEdge.getIncreasedWeight() ){
						C_maxEdge = tmpEdge;
					}
				}

				int[] X = new int[C.size()];
				// walk through the edges and raise their weight of those edges that enter the cycle
				int i=0;
				for(Edge ce : C) {
					int k = S.find(ce.getHead());
					X[i] = k;
					i++;
					I[k].increase(C_maxEdge.getIncreasedWeight() - ce.getIncreasedWeight());
				}

				int m = min[ S.find(C_maxEdge.getHead()) ];
				// unite all elements of X
				//int Xindex;
				for(int Xindex = 1; Xindex<X.length; Xindex++) {
					S.union(X[0], X[Xindex]);
				}
				int Xrepresentative = S.find(X[0]);

				min[Xrepresentative] = m;
				roots.add( V[Xrepresentative] );
				cycle[Xrepresentative] = C;

				// Merge the heaps of all nodes into one heap
				for (int ix=1; ix<X.length; ix++) {
					BinomialHeap h = I[ X[ix] ];
					I[Xrepresentative].merge(h);
				}
			}	// endif
        }	// endwhile

		String strRoot = null;
		//System.out.println("Root is " + root.getIndex());
		// root contains a root node of the minimal branching
		if(root!=-1){
			strRoot = Integer.toString(root);
			ForrestElement minLeaf = lambda[ min[root] ];
			if( minLeaf!=null ){
				F.deletePath(minLeaf);
			}
		}
		else{
			strRoot = "neznamy koren";
		}

		List<Edge> B = new ArrayList<Edge>();
		while( !F.empty() ){
			Edge e = F.getRoot();
			B.add(e);
			F.deletePath( lambda[ e.getHead() ] );
		}

		StringBuilder strMinPath = new StringBuilder(256);
		int weight = 0;
		for(Edge e : B){
			weight+= e.getWeight();
			strMinPath.append(e.toString());
			strMinPath.append(LINE_SEPARATOR);
		}
		output.append(weight);
		output.append(LINE_SEPARATOR);
		output.append(strRoot);
		output.append(LINE_SEPARATOR);
		output.append(strMinPath.toString());
		output.append("0 0 0");
		System.out.println(output.toString());

		if(DEBUG){ System.out.println("Time:" + ((System.nanoTime()-start)/1000000) + " ms." ); }
    }

}