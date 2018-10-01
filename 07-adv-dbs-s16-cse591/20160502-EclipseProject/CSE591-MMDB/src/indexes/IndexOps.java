package indexes;

import java.util.Hashtable;
import java.util.Scanner;

import datastore.DataTuple;
import datastore.DatastoreInterface;
import java.util.*;

public class IndexOps {

	public static void main(String[] args) {

		Scanner sc = new Scanner(System.in);
		DatastoreInterface dataStore = new DatastoreInterface();
		dataStore.initializeDataStore();
		
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
					
					Hashtable<Integer, Integer> hashTable = new Hashtable<>();
					HashTableOps.mainExecution(hashTable, dataStore);
					
				    System.out.println("number of items in hashTable: "+hashTable.size());
				    
				    long endTime = System.currentTimeMillis();
				    System.out.println("Total time taken to create the index and populate the datastore: "+ (endTime-startTime) + " seconds");
				    
				    
		/*		    //Dummy code to read everything 
				    startTime = System.currentTimeMillis();
				    for(Integer key: hashTable.keySet()) {
				    	int addr = hashTable.get(key);
				    	DataTuple dataTuple = new DataTuple();
				    	dataStore.read_data(addr, dataTuple);
				    	System.out.println(dataTuple.id + ", " + dataTuple.name + ", " + (int)dataTuple.age + ", " + dataTuple.salary);
				    }
				    endTime = System.currentTimeMillis();
				    System.out.println("Total time taken to create the index and populate the datastore: "+ (endTime-startTime) + " seconds");*/
				    
				    
				    while (true){
				    	System.out.println("Enter the Query type you want:");
				    	System.out.println("1. Given ID, find Data");
				    	System.out.println("2. Average salary of all the Data");
				    	System.out.println("3. Average salary of a given range of Data (in terms of ID)");
				    	
				    	int choice = sc.nextInt();
				    	
				    	switch (choice){
				    	
				    	case 1:
				    		System.out.println("Enter the ID to fetch Data");
				    		int ID = sc.nextInt();
				    		int addr = hashTable.get(ID);
				    		DataTuple dataTuple = new DataTuple();
				        	dataStore.read_data(addr, dataTuple);
				        	System.out.println("Data for ID: "+ID);
				        	System.out.println(dataTuple.id + ", " + dataTuple.name + ", " + dataTuple.age + ", " + dataTuple.salary);
				        	
				        	break;
				        	
				    	case 2:
				    		ArrayList<Integer> arrayList = new ArrayList();
				        	Integer sum = 0;
				        	for(Integer key: hashTable.keySet()) {
				    	    	int addr2 = hashTable.get(key);
				    	    	DataTuple dataTuple2 = new DataTuple();
				    	    	dataStore.read_data(addr2, dataTuple2);
				    	    	arrayList.add(dataTuple2.salary);
				    	    }
				        	if(!arrayList.isEmpty()) {
				        	    for (Integer salary : arrayList) {
				        	        sum += salary;
				        	    }
				        	}
				        	 double averageSalary = (double)sum/arrayList.size();
				        	 System.out.println("Average Salary: "+averageSalary);
				        	 
				        	 break;
				        	 
				        default:
				        	exit = true;
				        	
				    	
				    	}
				    if (exit = true)
				    	break;
				    }
				    //Dummy code to read everything
				    
				    //System.out.println("Total memory consumed in KB: " + (double) (Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory()) / 1024);
				    
					break;
				case 2:
					
					break;
					
				case 3:
					
					break;
					
				default:
					exit = true;
				}
				
				if(exit == true)
					break;
			}
		} finally {
			sc.close();
			dataStore.destroy_data_store();
		}

	}
		
	
	static {
		System.loadLibrary("bin/DatastoreInterface");
	}	

}
