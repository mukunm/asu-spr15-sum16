package indexes;

import java.util.Hashtable;

import dao.GenerateData;
import datastore.DataTuple;
import datastore.DatastoreInterface;

public class HashTableOps {

	static GenerateData genData = new GenerateData();
	public static void mainExecution(Hashtable<Integer, Integer> hashTable){
		int aKey;

		/**
		 * Number of insert operations.
		 */
		int initSize = 100; 
	    
		for (int i = 0; i < initSize; i++){
	    	aKey = i;
	    	String name = genData.GenerateName();
			char age = genData.GenerateAge();
			int salary = genData.GenerateSalary();
			System.out.println("Writing - " + aKey + ", " + name + ", " + (int)age + ", " + salary);
			int addr = DatastoreInterface.write_data(aKey, name, age, salary);
	    	hashTable.put(aKey, addr);
	    }
	    


	}
	
}