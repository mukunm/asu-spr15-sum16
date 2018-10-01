package datastore;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.Random;
import java.util.Scanner;

public class DatastoreInterface {
	private static int DEFAULT_DATASTORE_ADDRESS = 0;
	private static int MAX_NAME_LENGTH = 23;
	private static int MAX_RECORDS     = 1000;
	private static int MAX_ALPHABET    = 26;
	private static int MAX_ID_NUM      = 100000;
	private static int MAX_AGE         = 100;
	private static int MAX_SALARY      = 100000;

	private int dataStoreAddress = DEFAULT_DATASTORE_ADDRESS;
	
	public native int init_data_store();
	public native void destroy_data_store();
	public native int write_data(int id, String name, char age, int salary);
	public native int update_data(int tupleAddr, int id, String name, char age, int salary);
	public native void read_data(int tupleAddr, DataTuple tuple);
	
	public int[] populateDataStore() {
		Random rand = new Random();
		int[] tupleAddresses = new int[MAX_RECORDS];
		
		for(int i = 0; i < MAX_RECORDS; i++)  {
			
			/* Create Random Names */
			StringBuilder sb = new StringBuilder();
			for(int j = 0; j < MAX_NAME_LENGTH; j++) {
				int aValue = (int) 'a';
				int chValue = aValue + rand.nextInt(MAX_ALPHABET);
				char ch = (char) chValue;
				sb.append(ch);
			}
			
			char ageChar = (char) rand.nextInt(MAX_AGE);
			tupleAddresses[i] = 
					this.write_data(rand.nextInt(MAX_ID_NUM), sb.toString(), 
								    ageChar, rand.nextInt(MAX_SALARY));
		}
		
		return tupleAddresses;
	}

	public int[] initializeDataStore() {
		dataStoreAddress = this.init_data_store();
		int[] tupleAddresses = populateDataStore();
		return tupleAddresses;
	}
	
	public void readAllData(int[] tupleAddresses) throws IOException {
		File fout = new File("data.csv");
		FileOutputStream fos = new FileOutputStream(fout);
	 
		BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(fos));
		bw.write("Tuple Addresses," + "ID," + "Name," + "Age," + "Salary\n");
		for(int i = 0; i < tupleAddresses.length; i++) {
			DataTuple tupleData = new DataTuple();
			this.read_data(tupleAddresses[i], tupleData);
			int ageValue = (int) tupleData.age;
			bw.write(tupleAddresses[i] + "," + tupleData.id + "," + tupleData.name + "," + ageValue + "," + tupleData.salary + "\n");
		}
		
		System.out.println("Data has been written data.csv");
		
		bw.close();
	}
	
	public void destroyDataStore() {
		this.destroy_data_store();
	}
	
	public static void main(String[] args) throws InterruptedException {
		DatastoreInterface dsI = new DatastoreInterface();
		int[] tupleAddresses = dsI.initializeDataStore();
		try {
			while(true) {
				System.out.println("Data has been written to the data store");
				System.out.println("Hit 1 and press enter to read. Anything else to exit");
				Scanner sc = new Scanner(System.in);
				int input = sc.nextInt();
				if(input == 1) {
					dsI.readAllData(tupleAddresses);
				} else {
					dsI.destroyDataStore();
				}
				sc.close();
			}
		} catch(Exception e) {
			System.out.println(e.getMessage());
		} finally {
			dsI.destroy_data_store();
		}
	}
	
	//static {
	//	System.loadLibrary("bin/DatastoreInterface");
	//}		
}
