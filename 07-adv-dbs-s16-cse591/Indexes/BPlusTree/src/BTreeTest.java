public class BTreeTest {
	public static void main(String args[]){
		IntegerBTree tree = new IntegerBTree();
		System.out.println("This is running");
		// test for the btree on assignment4
		Long startTime = System.currentTimeMillis();
		for (int i=0; i<1000; i++){
			tree.insert(i);
		}
		
		System.out.println("Everything inserted");
		Long endTime = System.currentTimeMillis();
		System.out.println("Time taken to insert: "+(endTime-startTime));
		
		return;
	}
}


class IntegerBTree extends BTree<Integer, Integer> {
	public void insert(int key) {
		this.insert(key, key);
	}
	
	public void remove(int key) {
		this.delete(key);
	}
}