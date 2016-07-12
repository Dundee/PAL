package pal.disjointset;

/**
 *
 */
public class DisjointSet {
	private SetElement[] components;
	
	public DisjointSet(int size) {
		components = new SetElement[size];
	}
	
	//--------------------------------------------------------------------------
	public SetElement[] getAllElements(){
		return this.components;
	}

	//--------------------------------------------------------------------------
	/**
	 * Returns true if the set is empty.
	 * @return boolean
	 */
	public boolean isEmpty(){
		return( components.length==0 );
	}

	//--------------------------------------------------------------------------
	/**
	 * Finds the representative of the element value.
	 * Lowers the height of the tree, which optimizes the search time.
	 * @param int value
	 * @return int representative
	 */
	public int find(int value){
		if( isEmpty() ){
			throw new IllegalStateException("Error in method Disjoint.find . The set is empty.");
		}
		SetElement item = components[value];
		SetElement root = getNodeRoot(item);
		//root.setRank(1);
		return root.getValue();
	}
	
	//--------------------------------------------------------------------------
	/**
	 * This help method returns a root node of the leaf i. This node is a representative.
	 * Lowers the height of the tree, which optimizes the search time.
	 * @param SetElement i -leaf from which we step up the tree
	 * @return SetElement root
	 */
	private SetElement getNodeRoot(SetElement i){
		if( i.getParent().equals(i) ){
			return i;
		}
		else{
			SetElement parent = getNodeRoot(i.getParent());
			i.setParent(parent);
			i.setRank(0);
			return parent;
		}
	}
	
	//--------------------------------------------------------------------------
	/** Stores the element value in the set and makes him a representative. */
	public void makeSet(int value){
		SetElement item = new SetElement(value);
		components[value] = item;
	}
	
	//--------------------------------------------------------------------------
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("DisjointSet\n");
		for (int i = 0; i < components.length; i++) {
			int item = components[i].getValue();
			sb.append( find(item) + ", " );
		}
		
		return sb.toString();
	}
	
	//--------------------------------------------------------------------------
	/**
	 * Unions two trees. Firstly the representatives of both values are found and joined.
	 * Both values must be present in the heap already.
	 * @param int value1 -leaf of the tree
	 * @param int value2 -leaf of the tree
	 * @throws IllegalStateException if the representatives haven't been found
	 * @return int representative of the newly established tree
	 */
	public int union(int value1, int value2){
		if( components[value1]==null ){
			makeSet(value1);
		}
		if( components[value2]==null ){
			makeSet(value2);
		}
		
		// find the representants and connect the trees
		SetElement r1 = components[ find(value1) ];
		SetElement r2 = components[ find(value2) ];
		
		

		/*
		 balance the tree
		 attach the smaller tree to the root of the larger tree
		 rank is used to be aware of the height of the tree
		*/
		if( r1.getRank() > r2.getRank() ){
			r2.setParent(r1);
			return r1.getValue();
		}
		else if ( r1.getRank() < r2.getRank() ) {
			r1.setParent(r2);
			return r2.getValue();
		}
		else if ( !r1.equals(r2) ){ // merge the trees if they are not already in the same set
			r1.setParent(r2);
			r2.setRank(r2.getRank() + 1);
			return r2.getValue();
		}
		
		return r1.getValue();
	}

}