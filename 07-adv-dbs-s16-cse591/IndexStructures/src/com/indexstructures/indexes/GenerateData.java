package com.indexstructures.indexes;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Random;

public class GenerateData {
	Random random = new Random();
	public String GenerateName(){
		int min = 0;
		int max = 10;
		String[] peoples = {"Bob","Jill","Tom","Brandon","Nitish","Mukund","Ryan","Vienna","Lana","Paul","Lori"};
        int randomIndex = (random.nextInt(max - min + 1) + min);
		
        return peoples[randomIndex];
	}
	
	public void GenerateID(){
		
	}
	
	public int GenerateAge(){
		int min = 22;
		int max = 60;
		int randomIndex = (random.nextInt(max - min + 1) + min);
		return randomIndex;
	}
	
	public int GenerateSalary(){
		int min = 20000;
		int max = 500000;
		int randomIndex = (random.nextInt(max - min + 1) + min);
		return randomIndex;
	}
	
	public void GenerateRow(){
		String name = GenerateName();
		int age = GenerateAge();
		int salary = GenerateSalary();
	}
}
