package indexStructures;

import dao.GenerateData;
import datastore.DatastoreInterface;

public class BTreeTest {
	
	static GenerateData genData = new GenerateData();

	public static void mainExecution(IntegerBTree tree){
		int aKey;
		String aValue;
		
		//IntegerBTree tree = new IntegerBTree();
		System.out.println("This is running");
		// test for the btree on assignment4
		Long startTime = System.currentTimeMillis();
		for (int i=0; i<1000; i++){
			String name = genData.GenerateName();
			char age = genData.GenerateAge();
			int salary = genData.GenerateSalary();
	    	//aValue = name + "," + age + "," + salary;
	    	//System.out.println("Key: "+i + "     Value: "+ aValue);
			int addr = DatastoreInterface.write_data(i, name, age, salary);
			tree.insertData(i,addr);
		}
		
		//System.out.println("Everything inserted");
		Long endTime = System.currentTimeMillis();
		System.out.println("Time taken to insert: "+(endTime-startTime));
		System.out.println("KB: " + (double) (Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory()) / 1024);
		
		return;
	}
}


/*class IntegerBTree extends BTree<Integer, String> {
	public void insertData(int key, String value) {
		this.insert(key, value);
	}
	
	public void remove(int key) {
		this.delete(key);
	}
}*/