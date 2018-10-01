---------------
Content
---------------
This project consists of the following files
 - crypto.c
 - crypto.h
 - crypto_attack.c
 - crypto_attack.h
 - crypto_eff.c
 - crypto_test.c
 - file_if.c
 - file_if.h
 - user_file_if.c
 - Makefile

--------------- 
Functionalities
---------------
All these file put together carry out the following functionalities

1. Reading a file from commandline, performing encryption and decryption 
   on the file provided the user enters a key.
   
2. Reading a file from commandline and performing decryption through a 
   brute force attack.
   
3. Reading a 32-bit plain-text and performing encryption on it with keys
   varying from 0 - 0xFFFFFFFF and calculating bit changes in the cipher
   text for every bit-change in the key.
   
4. Reading a 32-bit plain text and a 32-bit key, performing an encryption
   and returning a 32-bit cipher text.
   
--------------
Makefile
--------------
The makefile is to be used to produce different executables for each of 
the above mentioned functionalities. The command to be used to obtain each
executable is given below

Functionality #1
	make crypto
	
Fucntionality #2
	make crypto_attack
	
Functionality #3
	make crypto_eff
	
Functionality #4
	make crypto_test
	
