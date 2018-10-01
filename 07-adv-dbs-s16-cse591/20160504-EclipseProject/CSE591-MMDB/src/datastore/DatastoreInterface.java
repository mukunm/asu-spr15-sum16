package datastore;

public class DatastoreInterface {
	public static native int init_data_store();
	public static native void destroy_data_store();
	public static native int write_data(int id, String name, char age, int salary);
	public static native int update_data(int tupleAddr, int id, String name, char age, int salary);
	public static native void read_data(int tupleAddr, DataTuple tuple);
}
