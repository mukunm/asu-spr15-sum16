package dao;

import java.util.Random;

public class GenerateData {
	Random random = new Random();
	public String GenerateName(){
		int min = 0;
		int max = 10;
		String[] names = {"Bob","Jill","Tom","Brandon","Nitish","Mukund","Ryan","Vienna","Lana","Paul","Lori"};
        int randomIndex = (random.nextInt(max - min + 1) + min);
		
        return names[randomIndex];
	}
	
	public char GenerateAge(){
		int min = 22;
		int max = 60;
		int randomIndex = (random.nextInt(max - min + 1) + min);
		return (char)randomIndex;
	}
	
	public int GenerateSalary(){
		int min = 20000;
		int max = 500000;
		int randomIndex = (random.nextInt(max - min + 1) + min);
		return randomIndex;
	}
	

}
