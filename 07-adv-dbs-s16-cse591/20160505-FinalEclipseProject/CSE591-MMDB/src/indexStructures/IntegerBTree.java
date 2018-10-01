package indexStructures;

public class IntegerBTree extends BTree<Integer, Integer> {
	public void insertData(int key, Integer value) {
		this.insert(key, value);
	}
	
	public void remove(int key) {
		this.delete(key);
	}
}
