package pal.disjointset;

/**
 * Item represents a component (a cyclic subgraph). The component consists of the set of nodes.
 * The component has a representative. The representative is the root of a tree.
 */
public class SetElement {
	private SetElement parent;
	private final int value;
	
	/**
	 * Whenever two trees of the same rank r are united, the rank of the result is r+1.
	 * Rank is one-element trees have a rank of zero
	 */
	private int rank;

	public SetElement(int value) {
		this.value = value;
		setParent(this);
		setRank(0);
	}
	
	@Override
	public boolean equals(Object o) {
		if( o==null || !o.getClass().equals(SetElement.class) ){
			return false;
		}
		SetElement item = (SetElement) o;
		return ( getValue()==item.getValue() );
	}
	
	public SetElement getParent() {
		return parent;
	}

	public int getRank() {
		return rank;
	}
	
	public int getValue() {
		return value;
	}

	public void setParent(SetElement parent) {
		this.parent = parent;
	}

	public void setRank(int rank) {
		this.rank = rank;
	}

	@Override
	public String toString() {
		return Integer.toString( getValue() );
	}
}