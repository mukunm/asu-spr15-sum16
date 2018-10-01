package indexes;

import java.util.Hashtable;

import dao.GenerateData;
import datastore.DatastoreInterface;

public class HashTableOps {

	static GenerateData genData = new GenerateData();
	public static void mainExecution(Hashtable<Integer, Integer> hashTable, DatastoreInterface dataStore){
		int aKey;

		
		int initSize = 100; // initsize is the number of times insert operation will be executed, 
							//that is the size of the hash table
	    
		for (int i = 0; i < initSize; i++){
	    	aKey = i;
	    	String name = genData.GenerateName();
			char age = genData.GenerateAge();
			int salary = genData.GenerateSalary();
			int addr = dataStore.write_data(aKey, name, age, salary);
	    	hashTable.put(aKey, addr);
	    }
	    


	}
	
}