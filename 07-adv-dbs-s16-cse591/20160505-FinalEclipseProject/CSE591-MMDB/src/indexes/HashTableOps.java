package indexes;

import java.util.Hashtable;

import dao.GenerateData;
import datastore.DatastoreInterface;

public class HashTableOps {

	static GenerateData genData = new GenerateData();
	public static void mainExecution(Hashtable<Integer, Integer> hashTable, int size){
		int aKey;

		/**
		 * Number of insert operations.
		 */
		int initSize = size; 
	    
		for (int i = 0; i < initSize; i++){
	    	aKey = i;
	    	String name = genData.GenerateName();
			char age = genData.GenerateAge();
			int salary = genData.GenerateSalary();
			int addr = DatastoreInterface.write_data(aKey, name, age, salary);
			//System.out.println(i + " - Address - " + addr);
	    	hashTable.put(aKey, addr);
	    }
	    


	}
	
}