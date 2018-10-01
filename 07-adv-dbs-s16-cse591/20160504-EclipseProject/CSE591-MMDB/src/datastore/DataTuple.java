package datastore;

public class DataTuple {
	public int id;
	public String name;
	public char age;
	public int salary;
	
	public DataTuple() {
		id = 0;
		name = "";
		age = 0;
		salary = 0;
	}
	
	public DataTuple(int id, String name, char age, int salary) {
		this.id = id;
		this.name = name;
		this.age = age;
		this.salary = salary;
	}
}
