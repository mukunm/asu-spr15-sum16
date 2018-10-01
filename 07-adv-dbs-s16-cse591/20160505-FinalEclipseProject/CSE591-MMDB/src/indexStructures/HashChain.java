// code from - http://www.java2s.com/Code/Java/Collections-Data-Structure/Hashtablewithseparatechaining.htm

package indexStructures;

import dao.GenerateData;
import datastore.DatastoreInterface;

class SortedList {
  private Link first;
  public SortedList() {
    first = null;
  }

  public void insert(Link theLink){
    int key = theLink.getKey();
    Link previous = null; // start at first
    Link current = first;
    // until end of list,
        //or current bigger than key,
    while (current != null && key > current.getKey()) { 
      previous = current;
      current = current.next; // go to next item
    }
    if (previous == null) // if beginning of list,
      first = theLink; 
    else
      // not at beginning,
      previous.next = theLink; 
    theLink.next = current; 
  }

  public void delete(int key){ 
    Link previous = null; 
    Link current = first;

    while (current != null && key != current.getKey()) { 
      previous = current;
      current = current.next; 
    }
    // disconnect link
    if (previous == null) //   if beginning of list delete first link
      first = first.next;       
    else
      //   not at beginning
      previous.next = current.next; //delete current link
  }

  public Link find(int key) {
    Link current = first; 
    while (current != null && current.getKey() <= key) { // or key too small,
      if (current.getKey() == key) // found, return link
        return current;  
      current = current.next; // go to next item
    }
    return null; // cannot find it
  }

  public void displayList() {
    System.out.print("List: ");
    Link current = first;
    while (current != null){
      current.displayLink(); 
      current = current.next;
    }
    System.out.println("");
  }
}

public class HashChain {
  private SortedList[] hashArray; 

  private int arraySize;
  static GenerateData genData = new GenerateData();

  public HashChain(int size) {
    arraySize = size;
    hashArray = new SortedList[arraySize];
    for (int i = 0; i < arraySize; i++)
      hashArray[i] = new SortedList(); 
  }

  public void displayTable() {
    for (int j = 0; j < arraySize; j++) {
      System.out.print(j + ". "); 
      hashArray[j].displayList(); 
    }
  }

  public int hashFunc(int key) {
    return key % arraySize;
  }

  public void insert(Link theLink) {
    int key = theLink.getKey();
    int hashVal = hashFunc(key); 
    hashArray[hashVal].insert(theLink); 
  }

  public void delete(int key) {
    int hashVal = hashFunc(key); // hash the key
    hashArray[hashVal].delete(key); 
  }

  public Link find(int key) {
	int hashVal = hashFunc(key); // hash the key
	Link theLink = hashArray[hashVal].find(key); // get link
	return theLink;
  }

  public static void mainExecution(HashChain hashTable, int size){
    int initSize = size;

    for (int i = 0; i < initSize; i++){
      
    	String name = genData.GenerateName();
		char age = genData.GenerateAge();
		int salary = genData.GenerateSalary();
		int addr = DatastoreInterface.write_data(i, name, age, salary);
		Link dataItem = new Link(i, addr);
      
		hashTable.insert(dataItem);
    }
  }

}
