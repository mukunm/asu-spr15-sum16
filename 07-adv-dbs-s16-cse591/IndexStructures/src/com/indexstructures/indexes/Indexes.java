package com.indexstructures.indexes;

import java.util.Scanner;

import com.indexstructure.bplustree.BTreeTest;
import com.indexstructure.chainedhashing.HashChain;
import com.indexstructure.hashing.HashTableOps;

public class Indexes {

	/**
	 * @param args
	 */
	HashTableOps hashTableOps = new HashTableOps();
	BTreeTest bTreeTest = new BTreeTest();
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int choice = 1;
		Scanner reader = new Scanner(System.in);
		
		
		
		System.out.println("Enter the number for the Index Structure: \n1. B+ Tree \n2. Chained Bucket Hashing" +
				"\n3. Hashing");
		choice = reader.nextInt();
		
		switch (choice){
		
		case 1: System.out.println("**********B+ Tree**********");
				BTreeTest.mainExecution();	
					break;
					
		case 2: System.out.println("**********Chained Bucket Hashing**********");
				HashChain.mainExecution();
					break;
					
		case 3: System.out.println("**********Hashing**********");
				HashTableOps.mainExecution();
					break;
		
		
		}

		
		
		reader.close();
	}

}
