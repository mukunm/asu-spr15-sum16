package indexes;

import java.util.Scanner;

import datastore.DataTuple;
import datastore.DatastoreInterface;

public class QueryFunctions {

	Scanner sc = new Scanner(System.in);
	
	public static void PointQuery(int addr, int ID){
		DataTuple dataTuple = new DataTuple();
		DatastoreInterface.read_data(addr, dataTuple);
    	System.out.println("Data for ID: "+ID);
    	System.out.println(dataTuple.id + ", " + dataTuple.name + ", " + (int)dataTuple.age + ", " + dataTuple.salary);
	}
	
	
	
}
