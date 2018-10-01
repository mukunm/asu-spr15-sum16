package indexes;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Scanner;

import datastore.DataTuple;
import datastore.DatastoreInterface;
import indexStructures.BTreeTest;
import indexStructures.HashChain;
import indexStructures.IntegerBTree;
import indexStructures.Link;


public class IndexOps {

	public static void main(String[] args) {

		Scanner sc = new Scanner(System.in);
		int original_addr = DatastoreInterface.init_data_store();
		System.out.println("DS Address = " + original_addr);
		
		try {
			while(true) {
				System.out.println("Enter the size of the dataset that you'd like to work with (1 - 10000)");
				int size = sc.nextInt();
				System.out.println("Which Index? ");
				System.out.println("1 - Extendible Hashing");
				System.out.println("2 - B+ Tree");
				System.out.println("3 - Chained Bucket Hashing");
				System.out.println("Anything else - Exit");
				
				int input =  sc.nextInt();
				boolean exit = false;
				
				switch(input) {
				case 1:
					long startTime = System.currentTimeMillis();
					
					Hashtable<Integer, Integer> hashTable = new Hashtable<>();
					HashTableOps.mainExecution(hashTable, size);
				
				    System.out.println("number of items in hashTable: " + hashTable.size());
				    
				    long endTime = System.currentTimeMillis();
				    System.out.println("Creation Time: "+ (endTime-startTime) + "ms");
				    
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
				    		startTime = System.currentTimeMillis();
				    		int addr = hashTable.get(ID);
				    		QueryFunctions.PointQuery(addr, ID);
				    		endTime = System.currentTimeMillis();
				    		System.out.println("Point Query Time: "+ (endTime-startTime) + "ms");
				        	break;
				        	
				    	case 2:
				        	long sum = 0;
				        	startTime = System.currentTimeMillis();
				        	for(Integer key: hashTable.keySet()) {
				    	    	int addr2 = hashTable.get(key);
				    	    	DataTuple dataTuple2 = new DataTuple();
				    	    	DatastoreInterface.read_data(addr2, dataTuple2);
				    	    	sum += dataTuple2.salary;
				    	    }
				        	double averageSalary = sum/(double)size;
				    		endTime = System.currentTimeMillis();
				        	System.out.println("Average Salary: "+averageSalary);
				    		System.out.println("Average Salary Query Time: "+ (endTime-startTime) + "ms");
				        	break;
				        	
				    	case 3:
				    		ArrayList<Integer> arrayList3 = new ArrayList<Integer>();
				         	Integer sum3 = 0;
				         	System.out.println("Enter min age range");
				         	int minAge = sc.nextInt();
				         	System.out.println("Enter max age range");
				         	int maxAge = sc.nextInt();
				         	startTime = System.currentTimeMillis();				         	
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
				         	double averageSalary3 = sum3/ (double)arrayList3.size();
				         	endTime = System.currentTimeMillis();
				         	System.out.println("Average Salary for Age group" + minAge + " - " + maxAge + " : "+averageSalary3);
				         	System.out.println("Range Query Time: "+ (endTime-startTime) + "ms");				         	
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
					BTreeTest.mainExecution(Btree, size);
					long endTimeBTree = System.currentTimeMillis();
					System.out.println("Creation Time: "+ (endTimeBTree-startTimeBTree) + "ms");
					
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
				    		startTimeBTree = System.currentTimeMillis();
				    		int addr = Btree.search(ID);
				    		QueryFunctions.PointQuery(addr, ID);
				    		endTimeBTree = System.currentTimeMillis();
				    		System.out.println("Point Query Time: "+ (endTimeBTree-startTimeBTree) + "ms");
				        	break;
				        	
				    	case 2:
				    		/****************************************************************
				    		For checking the average, change the value of i in the for loop below
				    		and change the sizeBtree variable. Same in case 3.
				    		******************************************************************/
				    		
				        	long sum = 0;
				        	int sizeBtree = size;
				        	startTimeBTree = System.currentTimeMillis();
				        	for(int i=0; i < sizeBtree; i++)
				        	{
				    	    	int addr2 = Btree.search(i);
				    	    	DataTuple dataTuple2 = new DataTuple();
				    	    	DatastoreInterface.read_data(addr2, dataTuple2);
				    	    	sum += dataTuple2.salary;
				    	    }
				        	double averageSalary = sum/(double)sizeBtree;
				        	endTimeBTree = System.currentTimeMillis();
							System.out.println("Average Salary: "+averageSalary);
							System.out.println("Average Salary Query Time: "+ (endTimeBTree-startTimeBTree) + "ms");
				        	break;
				        	
				    	case 3:
				    		ArrayList<Integer> arrayList3 = new ArrayList<Integer>();
				         	Integer sum3 = 0;
				         	Integer size3 = size;
				         	System.out.println("Enter min age range");
				         	int minAge = sc.nextInt();
				         	System.out.println("Enter max age range");
				         	int maxAge = sc.nextInt();
				        	startTimeBTree = System.currentTimeMillis();				         	
				         	for(int i=0; i < size3; i++) {
				         		int addr3 = Btree.search(i);
				         		DataTuple dataTuple3 = new DataTuple();
				         		DatastoreInterface.read_data(addr3, dataTuple3);
				         		
				         		if (((dataTuple3.age < maxAge) && (dataTuple3.age > minAge))){
				         	    	arrayList3.add(dataTuple3.salary);
				         		}
				     	    }
				         	
				         	if(!arrayList3.isEmpty()) {
				         	    for (Integer salary : arrayList3)
				         	        sum3 += salary;
							}
				         	
				         	double averageSalary3 = sum3/(double)arrayList3.size();
				         	endTimeBTree = System.currentTimeMillis();
				         	System.out.println("Average Salary for Age group" + minAge + " - " + maxAge + " : "+averageSalary3);
				         	System.out.println("Range Query Time: "+ (endTimeBTree-startTimeBTree) + "ms");
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
					int sizeCBH = size;
					HashChain chainedHashTable = new HashChain(sizeCBH);
					HashChain.mainExecution(chainedHashTable, size);
					
					long endTimeCBH = System.currentTimeMillis();
					System.out.println("Creation Time: "+ (endTimeCBH-startTimeCBH) + " ms");
					
					while (true){
				    	System.out.println("Enter the Query type you want:");
				    	System.out.println("1. Given ID, find Data");
				    	System.out.println("2. Average salary of all the Data");
				    	System.out.println("3. Average salary of a given range of Data (in terms of age)");
				    	System.out.println("Anything else - Exit");
				    	
				    	int choice = sc.nextInt();
				    	
				    	switch (choice){
				    	
				    	case 1:
				    		System.out.println("Enter the ID to fetch Data");
				    		int ID = sc.nextInt();
				    		Link dataItem;
				    		startTimeCBH = System.currentTimeMillis();
				    		dataItem = chainedHashTable.find(ID);
				    		int addr = dataItem.getData();
				    		QueryFunctions.PointQuery(addr, ID);
				    		endTimeCBH = System.currentTimeMillis();
				    		System.out.println("Point Query Time: "+ (endTimeCBH-startTimeCBH) + " ms");
				        	break;
				        	
				    	case 2:
				    		/****************************************************************
				    		For checking the average, change the value of i in the for loop below
				    		and change the sizeCBH variable. Same in case 3.
				    		******************************************************************/
				    		
				        	long sum = 0;
				        	int sizeCBH2 = size;

				        	startTimeCBH = System.currentTimeMillis();
				        	for (int i=0; i < sizeCBH2;i++)
				        	{
				        		Link dataItemCBH;
				        		
					    		dataItemCBH = chainedHashTable.find(i);
					    		int addr2 = dataItemCBH.getData();
				    	    	DataTuple dataTuple2 = new DataTuple();
				    	    	DatastoreInterface.read_data(addr2, dataTuple2);
				    	    	sum += dataTuple2.salary;
				    	    }
				        	
				        	double averageSalary = sum/(double)sizeCBH2;
				        	endTimeCBH = System.currentTimeMillis();
				        	System.out.println("Average Salary: "+averageSalary);
				        	System.out.println("Average Salary Query Time: "+ (endTimeCBH-startTimeCBH) + " ms");
				        	break;
				        	
				    	case 3:
				    		ArrayList<Integer> arrayList3 = new ArrayList<Integer>();
				         	Integer sum3 = 0;
				         	Integer size3 = size;
				         	System.out.println("Enter min age range");
				         	int minAge = sc.nextInt();
				         	System.out.println("Enter max age range");
				         	int maxAge = sc.nextInt();
				         	startTimeCBH = System.currentTimeMillis();
				         	for(int i=0; i < size3; i++) {
				         		Link dataItemCBH;
					    		dataItemCBH = chainedHashTable.find(i);
					    		int addr3 = dataItemCBH.getData();
					    		
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
				         	endTimeCBH = System.currentTimeMillis();
				         	double averageSalary3 = sum3/(double)arrayList3.size();
				         	System.out.println("Average Salary for Age group" + minAge + " - " + maxAge + " : "+averageSalary3);
				         	System.out.println("Range Query Time: "+ (endTimeCBH-startTimeCBH) + " ms");
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
