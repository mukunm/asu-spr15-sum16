package com.hashtable.main;

import java.io.IOException;
import java.util.Hashtable;

public class HashTableOperations {

	public static void main(String[] args) throws IOException{
		int aKey;
		int aValue;
		Hashtable<Integer, Integer> hashTable = new Hashtable<Integer, Integer>();
		
		long startTime = System.currentTimeMillis();
		int initSize = 1000; // initsize is the number of times insert operation will be executed, 
									//that is the size of the hash table
	    for (int i = 0; i < initSize; i++){
	    	aKey = i;
	    	aValue = (aKey*7 + 3)%200;
	    	//System.out.println("Key: "+aKey + " Value: "+aValue);
	    	hashTable.put(aKey, aValue);
	    	//System.out.println("Number of times run: "+i);
	    }
	    
	    System.out.println("number of items in hashTable: "+hashTable.size());
	    
	    long endTime = System.currentTimeMillis();
	    System.out.println("Total time taken: "+(endTime-startTime));
	    
	    System.out.println("KB: " + (double) (Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory()) / 1024);

	}
	
}
