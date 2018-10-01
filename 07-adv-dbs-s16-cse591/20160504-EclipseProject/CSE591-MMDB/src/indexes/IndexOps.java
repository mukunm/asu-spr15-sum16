package indexes;

import indexStructures.BTree;
import indexStructures.BTreeTest;
import indexStructures.HashChain;
import indexStructures.IntegerBTree;
import indexStructures.Link;

import java.math.BigInteger;
import java.util.Hashtable;
import java.util.Scanner;

import datastore.DataTuple;
import datastore.DatastoreInterface;
import java.util.ArrayList;


public class IndexOps {

	public static void main(String[] args) {

		Scanner sc = new Scanner(System.in);
		DatastoreInterface.init_data_store();

		
			//Test code to test writing of salary.
			/*char age = 33;
			int salary = 145948;
			int addr = dataStore.write_data(0, "Mukund", age, salary);
			DataTuple tuple = new DataTuple();
			dataStore.read_data(addr, tuple);
			System.out.println("Id = " + tuple.id);
			System.out.println("Name = " + tuple.name);
			System.out.println("Age = " + (int) tuple.age);
			System.out.println("Salary = " + tuple.salary); 
			dataStore.destroy_data_store();
			sc.close();*/
	
		try {
			while(true) {
				System.out.println("Which Index? ");
				System.out.println("1 - Extendible Hashing");
				System.out.println("2 - Chained Bucket Hashing");
				System.out.println("3 - B+ Tree");
				System.out.println("Anything else - Exit");
				
				int input =  sc.nextInt();
				boolean exit = false;
				
				switch(input) {
				case 1:
					long startTime = System.currentTimeMillis();
					
					Hashtable<Integer, Integer> hashTable = new Hashtable<Integer, Integer>();
					HashTableOps.mainExecution(hashTable);
				
				    System.out.println("number of items in hashTable: " + hashTable.size());
				    
				    long endTime = System.currentTimeMillis();
				    System.out.println("Total time taken to create the index and populate the datastore: "+ (endTime-startTime) + " seconds");
				    
				    while (true){
				    	System.out.println("Enter the Query type you want:");
				    	System.out.println("1. Given ID, find Data");
				    	System.out.println("2. Average salary of all the Data");
				    	System.out.println("3. Average salary of a given range of Data (in terms of ID)");
				    	System.out.println("Anything else - Exit");
				    	
				    	int choice = sc.nextInt();
				    	
				    	switch (choice){
				    	
				    	case 1:
				    		System.out.println("Enter the ID to fetch Data");
				    		int ID = sc.nextInt();
				    		int addr = hashTable.get(ID);
				    		QueryFunctions.PointQuery(addr, ID);
				        	break;
				        	
				    	case 2:
				        	long sum = 0;
				        	Integer size = hashTable.size();
				        	for(Integer key: hashTable.keySet()) {
				    	    	int addr2 = hashTable.get(key);
				    	    	DataTuple dataTuple2 = new DataTuple();
				    	    	DatastoreInterface.read_data(addr2, dataTuple2);
				    	    	sum += dataTuple2.salary;
				    	    }
				        	double averageSalary = sum/(double)size;
							System.out.println("Average Salary: "+averageSalary);
				        	break;
				        	
				    	case 3:
				    		ArrayList<Integer> arrayList3 = new ArrayList<Integer>();
				         	Integer sum3 = 0;
				         	Integer size3 = hashTable.size();
				         	int minAge = 20;
				         	int maxAge = 40;
				         	for(Integer key: hashTable.keySet()) {
				         		int addr3 = hashTable.get(key);
				         		DataTuple dataTuple3 = new DataTuple();
				         		DatastoreInterface.read_data(addr3, dataTuple3);
				         		
				         		if (((dataTuple3.age < maxAge)&&(dataTuple3.age > minAge))){
				         	    	arrayList3.add(dataTuple3.salary);
				         		}
				     	    }
				         	if(!arrayList3.isEmpty()) {
				         	    for (Integer salary : arrayList3)
				         	        sum3 += salary;
				         	    }
				         	double averageSalary3 = sum3/(double)size3;
				         	System.out.println("Average Salary for Age group 20 - 40: "+averageSalary3);
				    		break;
				        	 
				        default:
				        	exit = true;
				    	}
				    if (exit == true)
				    	break;
				    }
				    
					break;
				case 2:
					// code to call the index structure
					long startTimeBTree = System.currentTimeMillis();
					IntegerBTree Btree = new IntegerBTree();
					BTreeTest.mainExecution(Btree);
					
					long endTimeBTree = System.currentTimeMillis();
					System.out.println("Total time taken to create the index and populate the datastore: "+ (endTimeBTree-startTimeBTree) + " seconds");
					
					while (true){
				    	System.out.println("Enter the Query type you want:");
				    	System.out.println("1. Given ID, find Data");
				    	System.out.println("2. Average salary of all the Data");
				    	System.out.println("3. Average salary of a given range of Data (in terms of ID)");
				    	System.out.println("Anything else - Exit");
				    	
				    	int choice = sc.nextInt();
				    	
				    	switch (choice){
				    	
				    	case 1:
				    		System.out.println("Enter the ID to fetch Data");
				    		int ID = sc.nextInt();
				    		int addr = Btree.search(ID);
				    		QueryFunctions.PointQuery(addr, ID);
				        	break;
				        	
				    	case 2:
				    		/****************************************************************
				    		For checking the average, change the value of i in the for loop below
				    		and change the sizeBtree variable. Same in case 3.
				    		******************************************************************/
				    		
				        	long sum = 0;
				        	int sizeBtree = 10000;
				        	//Integer size = Btree.getK
				        	//for(Integer key: hashTable.keySet()) 
				        	for (int i=0;i<10000;i++)
				        	{
				    	    	int addr2 = Btree.search(i);
				    	    	DataTuple dataTuple2 = new DataTuple();
				    	    	DatastoreInterface.read_data(addr2, dataTuple2);
				    	    	sum += dataTuple2.salary;
				    	    }
				        	double averageSalary = sum/(double)sizeBtree;
							System.out.println("Average Salary: "+averageSalary);
				        	break;
				        	
				    	case 3:
				    		ArrayList<Integer> arrayList3 = new ArrayList<Integer>();
				         	Integer sum3 = 0;
				         	Integer size3 = 10000;
				         	int minAge = 20;
				         	int maxAge = 40;
				         	for(int i=0;i<10000;i++) {
				         		int addr3 = Btree.search(i);
				         		DataTuple dataTuple3 = new DataTuple();
				         		DatastoreInterface.read_data(addr3, dataTuple3);
				         		
				         		if (((dataTuple3.age < maxAge)&&(dataTuple3.age > minAge))){
				         	    	arrayList3.add(dataTuple3.salary);
				         		}
				     	    }
				         	if(!arrayList3.isEmpty()) {
				         	    for (Integer salary : arrayList3)
				         	        sum3 += salary;
				         	    }
				         	double averageSalary3 = sum3/(double)size3;
				         	System.out.println("Average Salary for Age group 20 - 40: "+averageSalary3);
				    		break;
				        	 
				        default:
				        	exit = true;
				    	}
				    if (exit == true)
				    	break;
				    }
					
					break;
					
				case 3:
					// Chained Bucket Hashing
					long startTimeCBH = System.currentTimeMillis();
					int sizeCBH = 10000;
					HashChain chainedHashTable = new HashChain(sizeCBH);
					HashChain.mainExecution(chainedHashTable);
					
					
					
					
					long endTimeCBH = System.currentTimeMillis();
					System.out.println("Total time taken to create the index and populate the datastore: "+ (endTimeCBH-startTimeCBH) + " seconds");
					
					while (true){
				    	System.out.println("Enter the Query type you want:");
				    	System.out.println("1. Given ID, find Data");
				    	System.out.println("2. Average salary of all the Data");
				    	System.out.println("3. Average salary of a given range of Data (in terms of ID)");
				    	System.out.println("Anything else - Exit");
				    	
				    	int choice = sc.nextInt();
				    	
				    	switch (choice){
				    	
				    	case 1:
				    		System.out.println("Enter the ID to fetch Data");
				    		int ID = sc.nextInt();
				    		Link dataItem;
				    		dataItem = chainedHashTable.find(ID);
				    		int addr = dataItem.getKey();
				    		QueryFunctions.PointQuery(addr, ID);
				        	break;
				        	
				    	case 2:
				    		/****************************************************************
				    		For checking the average, change the value of i in the for loop below
				    		and change the sizeCBH variable. Same in case 3.
				    		******************************************************************/
				    		
				        	long sum = 0;
				        	int sizeCBH2 = 10000;
				        	//Integer size = Btree.getK
				        	//for(Integer key: hashTable.keySet()) 
				        	for (int i=0;i<10000;i++)
				        	{
				        		Link dataItemCBH;
					    		dataItemCBH = chainedHashTable.find(i);
					    		int addr2 = dataItemCBH.getKey();
				    	    	DataTuple dataTuple2 = new DataTuple();
				    	    	DatastoreInterface.read_data(addr2, dataTuple2);
				    	    	sum += dataTuple2.salary;
				    	    }
				        	double averageSalary = sum/(double)sizeCBH2;
							System.out.println("Average Salary: "+averageSalary);
				        	break;
				        	
				    	case 3:
				    		ArrayList<Integer> arrayList3 = new ArrayList<Integer>();
				         	Integer sum3 = 0;
				         	Integer size3 = 10000;
				         	int minAge = 20;
				         	int maxAge = 40;
				         	for(int i=0;i<10000;i++) {
				         		Link dataItemCBH;
					    		dataItemCBH = chainedHashTable.find(i);
					    		int addr3 = dataItemCBH.getKey();
					    		
				         		DataTuple dataTuple3 = new DataTuple();
				         		DatastoreInterface.read_data(addr3, dataTuple3);
				         		
				         		if (((dataTuple3.age < maxAge)&&(dataTuple3.age > minAge))){
				         	    	arrayList3.add(dataTuple3.salary);
				         		}
				     	    }
				         	if(!arrayList3.isEmpty()) {
				         	    for (Integer salary : arrayList3)
				         	        sum3 += salary;
				         	    }
				         	double averageSalary3 = sum3/(double)size3;
				         	System.out.println("Average Salary for Age group 20 - 40: "+averageSalary3);
				    		break;
				        	 
				        default:
				        	exit = true;
				    	}
				    if (exit == true)
				    	break;
				    }
					
					break;
					
					
				default:
					exit = true;
				}
				
				if(exit == true)
					break;
			}
		} finally {
			sc.close();
			DatastoreInterface.destroy_data_store();
		}

	}
		
	
	static {
		System.loadLibrary("bin/DatastoreInterface");
	}	

}
