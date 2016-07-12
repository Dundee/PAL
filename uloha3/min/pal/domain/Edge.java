package pal.domain;

import pal.heap.BinomialHeap;

public class Edge implements Comparable {
	// we can substitute the nodes with their indices to save some memory
	private final int tail;
	private final int head;
	private int weight;
	private int increase;
	private BinomialHeap heap;
	
	//--------------------------------------------------------------------------
	public Edge(int tail, int head, int weight) {
		this.tail = tail;
		this.head = head;
		setWeight(weight);
		increase = 0;
	}
	
	//--------------------------------------------------------------------------
	/**
	 * Raises the increase and returns the result.
	 * @param int i -add this to increase
	 */
	public void increase(int i) {
		this.increase+= i;
	}
	
	//--------------------------------------------------------------------------
	/**
	 * Returns an increase that raises the weight of the edge.
	 * @return int
	 */
	public int getIncrease() {
		return ( increase + getHeapIncrease() );
	}
	
	//--------------------------------------------------------------------------
	public int compareTo(Object o) {
		Edge e = (Edge) o;
		
		// sort by weight
		if( getIncreasedWeight()>e.getIncreasedWeight() ){
			return 1;
		}
		else if( getIncreasedWeight()<e.getIncreasedWeight() ){
			return -1;
		}
		else{
			return 0;
		}
	}
	
	//--------------------------------------------------------------------------
	/**
	 * Returns true if the head and tail of the edges match.
	 * @return boolean
	 */
	@Override
	public boolean equals(Object obj) {
		if( obj==null || obj.getClass()!=Edge.class ){
			return false;
		}
		Edge edge = (Edge) obj;
		if( getTail()==edge.getTail() && getHead()==edge.getHead() && getWeight()==edge.getWeight()){
			return true;
		}
		
		return false;
	}
	
	//--------------------------------------------------------------------------
	public int getHead() {
		return head;
	}

	public int getTail() {
		return tail;
	}

	public int getWeight() {
		return weight;
	}

	private int getHeapIncrease(){
		final int heapIncrease = (heap!=null ? heap.getIncrease() : 0);
		return heapIncrease;
	}
	
	/**
	 * Returns the weight of the edge raised by an increase.
	 * @return int
	 */
	public int getIncreasedWeight() {
		int incrementedWeight = getWeight() + getIncrease();
		return incrementedWeight;
	}
	
	public void setHeap(BinomialHeap heap) {
		this.heap = heap;
	}
	
	public void setWeight(int weight) {
		this.weight = weight;
	}
	
	//--------------------------------------------------------------------------
	/**
	 * Returns the hash of the edge.
	 * The hash is a sequence of the tail index and the head index.
	 * @return String
	 */
	public String hash() {
		StringBuilder sb = new StringBuilder(7);
		sb.append(getTail());
		sb.append(" ");
		sb.append(getHead());
		return sb.toString();
	}
	
	//--------------------------------------------------------------------------
	@Override
	public String toString() {
		return getTail() + " " + getHead() + " " + getWeight();
	}

}