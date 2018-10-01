package indexStructures;


public class Link {
	  private int data;
	  private int key;
	  public Link next;

	  public Link(int k, int d) {
	    data = d;
	    key = k;
	  }

	  public int getKey() {
	    return key;
	  }
	  
	  public int getData() {
		  return data;
	  }

	  public void displayLink() {
	    System.out.print(data + " ");
	  }
}