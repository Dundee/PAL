package pal.heap;

import java.util.PriorityQueue;
import pal.domain.Edge;

public class BinomialHeap {
	protected PriorityQueue<Edge> edges;
	protected int increase;

	//----------------------------------------------------------------------------------------------
	public BinomialHeap() {
		edges = new PriorityQueue<Edge>();
		this.increase = 0;
	}
	
	//----------------------------------------------------------------------------------------------
	public int getIncrease() {
		return increase;
	}

	//----------------------------------------------------------------------------------------------
	/**
	 * Increases the increment of all items. The increment i is propagated to all items.
	 * @param int i -how much we add to each item
	 */
	public synchronized void increase( int i ){
		this.increase+= i;
	}
	
	//----------------------------------------------------------------------------------------------
	/**
	 * Inserts the node to the heap. A reference to this heap is passed to the inserted edge.
	 * @param Edge edge -edge to be inserted into the heap
	 */
	public synchronized void insert(Edge edge) {
		edge.setHeap(this);
		edges.add(edge);
	}
	
	//----------------------------------------------------------------------------------------------
	/**
	 * Merges two heaps into one.
	 * @param BinomialHeap heap -will be merged into this instance
	 * @return BinomialHeap -heap being merged
	 */
	public synchronized BinomialHeap merge( BinomialHeap heap ){
		if( this==heap ){
			return this;
		}
		
		for(Edge e : heap.edges){
			e.increase(heap.getIncrease());
			e.setHeap(this);
		}
		edges.addAll(heap.edges);
		heap.edges.clear();
		heap.increase = 0;
		return this;
	}
	
	//----------------------------------------------------------------------------------------------
	/**
	 * Returns the lowest value.
	 * @throws IllegalStateException if the heap is empty
	 * @return Edge -minimum
	 */
	public synchronized Edge min(){
		Edge minEdge = edges.peek();
		return minEdge;
	}
	
	//----------------------------------------------------------------------------------------------
	/**
	 * Deletes and returns the edge with minimal value.
	 * We propagate the heap increase to the edge.
	 * @return Edge -minimum
	 */
	public synchronized Edge popMin(){
		if( edges.isEmpty() ){
			return null;
		}
		Edge minEdge = edges.poll();
		minEdge.increase(getIncrease());
		minEdge.setHeap(null);
		return minEdge;
	}
	
	//----------------------------------------------------------------------------------------------
	/**
	 * Returns the size of the heap. Ie the number of the items stored in the heap.
	 * @return int
	 */
	public int size(){
		return edges.size();
	}
}