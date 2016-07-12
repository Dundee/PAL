package pal.forrest;

import java.util.Collection;
import pal.domain.Edge;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map;

/** Holds the paths from root nodes to the leaves. */
public class Forrest {
	/**
	 * Map of nodes containing the edges.
	 * The key is represented by the hashcode of the edge.
	 */
	private Map<String, ForrestElement> roots;
	
	//--------------------------------------------------------------------------
	public Forrest() {
		roots = new Hashtable<String, ForrestElement>();
	}
	
	//--------------------------------------------------------------------------
	/**
	 * Envolepes the edge with a ForrestElement and adds it into the forest as a root node.
	 * @param Edge edge -edge to be added
	 * @return ForrestElement
	 */
	public ForrestElement add( Edge edge ){
		ForrestElement n = new ForrestElement(edge);
		roots.put(edge.hash(), n);
		return n;
	}
	
	//--------------------------------------------------------------------------
	/**
	 * Deletes and returns the first root node or null if the forrest is empty.
	 * @see getRoots()
	 * @return Edge or null
	 */
	public Edge getRoot(){
		Iterator<ForrestElement> rootIterator = getRoots().iterator();
		if( rootIterator.hasNext() ){
			ForrestElement root = rootIterator.next();
			Edge e = root.getEdge();
			rootIterator.remove();
			return e;
		}
		else{
			return null;
		}
	}

	//--------------------------------------------------------------------------
	/**
	 * Returns all root nodes.
	 * @return Collection<ForrestElement>
	 */
	public Collection<ForrestElement> getRoots(){
		return roots.values();
	}

	//--------------------------------------------------------------------------
	/**
	 * Returns true if the forrest has no roots.
	 * @return boolean
	 */
	public boolean empty(){
		return roots.isEmpty();
	}
	
	//--------------------------------------------------------------------------
	/**
	 * Idetifies and deletes the path from the leaf to the root and all arcs directed out of these nodes.
	 * The remaining subtrees are made root trees placed on the highest level.
	 * @param ForrestElement leaf -leaf from which we start finding the root on the way up
	 */
	public void deletePath( ForrestElement leaf ){
		if( leaf.getParent()!=null ){
			leaf.getParent().getChildren().remove(leaf.getEdge().hash());
		}

		for(Iterator<ForrestElement> it = leaf.getChildren().values().iterator(); it.hasNext();){
			ForrestElement child = it.next();
			child.setParent(null);
			roots.put(child.getEdge().hash(), child);
		}
		leaf.getChildren().clear();
		if( leaf.getParent()==null ){
			roots.remove(leaf.getEdge().hash());
			return;
		}
		deletePath(leaf.getParent());
	}
}