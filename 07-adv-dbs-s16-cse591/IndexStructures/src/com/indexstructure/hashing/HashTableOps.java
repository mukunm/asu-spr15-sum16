package com.indexstructure.hashing;

import java.util.Hashtable;

import com.indexstructures.indexes.GenerateData;

public class HashTableOps {

	static GenerateData genData = new GenerateData();
	public static void mainExecution(){
		int aKey;
		String aValue;
		Hashtable<Integer, String> hashTable = new Hashtable<Integer, String>();
		
		long startTime = System.currentTimeMillis();
		int initSize = 100; // initsize is the number of times insert operation will be executed, 
									//that is the size of the hash table
	    for (int i = 0; i < initSize; i++){
	    	aKey = i;
	    	/*aValue = (aKey*7 + 3)%200;
	    	//System.out.println("Key: "+aKey + " Value: "+aValue);
	    	 **/
	    	String name = genData.GenerateName();
			int age = genData.GenerateAge();
			int salary = genData.GenerateSalary();
	    	aValue = name + "," + age + "," + salary;
	    	System.out.println("Key: "+aKey + "     Value: "+ aValue);
	    	hashTable.put(aKey, aValue);
	    	//System.out.println("Number of times run: "+i);
	    }
	    
	    System.out.println("number of items in hashTable: "+hashTable.size());
	    
	    long endTime = System.currentTimeMillis();
	    System.out.println("Total time taken: "+(endTime-startTime)+ " seconds");
	    
	    System.out.println("Total memory consumed in KB: " + (double) (Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory()) / 1024);

	}
	
}
