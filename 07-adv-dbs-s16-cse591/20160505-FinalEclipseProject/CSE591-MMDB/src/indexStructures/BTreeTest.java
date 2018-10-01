package indexStructures;

import dao.GenerateData;
import datastore.DatastoreInterface;

public class BTreeTest {
	
	static GenerateData genData = new GenerateData();

	public static void mainExecution(IntegerBTree tree, int size){

		for (int i=0; i < size; i++) {
			String name = genData.GenerateName();
			char age = genData.GenerateAge();
			int salary = genData.GenerateSalary();
			int addr = DatastoreInterface.write_data(i, name, age, salary);
			tree.insertData(i,addr);
		}

		return;
	}
}
