package com.indexstructure.bplustree;

import com.indexstructures.indexes.GenerateData;

public class BTreeTest {
	
	static GenerateData genData = new GenerateData();

	public static void mainExecution(){
		int aKey;
		String aValue;
		
		IntegerBTree tree = new IntegerBTree();
		System.out.println("This is running");
		// test for the btree on assignment4
		Long startTime = System.currentTimeMillis();
		for (int i=0; i<1000; i++){
			String name = genData.GenerateName();
			int age = genData.GenerateAge();
			int salary = genData.GenerateSalary();
	    	aValue = name + "," + age + "," + salary;
	    	//System.out.println("Key: "+i + "     Value: "+ aValue);
			tree.insertData(i,aValue);
		}
		
		//System.out.println("Everything inserted");
		Long endTime = System.currentTimeMillis();
		System.out.println("Time taken to insert: "+(endTime-startTime));
		System.out.println("KB: " + (double) (Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory()) / 1024);
		
		return;
	}
}


class IntegerBTree extends BTree<Integer, String> {
	public void insertData(int key, String value) {
		this.insert(key, value);
	}
	
	public void remove(int key) {
		this.delete(key);
	}
}