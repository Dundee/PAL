package pal.forrest;

import java.util.Hashtable;
import java.util.Map;
import pal.domain.Edge;

/**
 */
public class ForrestElement {
	private final Edge edge;
	private ForrestElement parent;
	private Map<String, ForrestElement> children;
	
	//--------------------------------------------------------------------------
	public ForrestElement(Edge edge) {
		this.edge = edge;
		setParent(null);
	}
	
	//--------------------------------------------------------------------------
	/**
	 * Adds a child element to this element.
	 * This element becomes a parent of the child element.
	 * @return ForrestElement -new element that envelopes the added edge
	 */
	public ForrestElement addChild(Edge e) {
		ForrestElement child = new ForrestElement(e);
		child.setParent(this);
		getChildren().put(e.hash(), child);
		return child;
	}

	//--------------------------------------------------------------------------
	/**
	 * Returns true if the contained edges are equal.
	 * @return boolean
	 */
	@Override
	public boolean equals(Object obj) {
		if( obj==null || obj.getClass()!=ForrestElement.class ){
			return false;
		}
		ForrestElement elem = (ForrestElement) obj;
		return getEdge().equals(elem.getEdge());
	}
	
	//--------------------------------------------------------------------------
	public Edge getEdge() {
		return edge;
	}

	//--------------------------------------------------------------------------
	public ForrestElement getParent() {
		return parent;
	}

	//--------------------------------------------------------------------------
	public Map<String, ForrestElement> getChildren() {
		if (children == null) {
			children = new Hashtable<String, ForrestElement>();
		}
		return children;
	}
	
	//--------------------------------------------------------------------------
	public void setParent(ForrestElement parent) {
		this.parent = parent;
	}

}