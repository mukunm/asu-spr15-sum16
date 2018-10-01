# s16_cse591
=========================================
Advances in Databases at ASU
=========================================

05-05-2016 - 10:03 AM
<hr/>
Committed by Mukund and Nitish
-------------------------------
1. Added final eclipse project with
	- All indexes implemented
	- Queries Implemented
	- Integrated with Datastore interface
2. Read/Write APIs in the C datastore

Issues Resolved
--------------------
- Synced Logs between Datstore DLL and Javd interface
- Fixed Memory corruption issues in the datastore

Test status
-------------
Tested
1. Insert Queries
2. Point Queries
3. Range Queries
4. Sweep Queries
for 100, 1000 and 10000 records.

Issues
------------
1. When memory allocated is ~> 418 MB, segmentation fault is seen when writing to
the memory location. Needs to be seen how this can be fixed. If not, the dataset
that can be created will be half of what was planned i.e. ~ 2 ^ 24 records.

2. Datastore crashes when more than ~14000 records are inserted.

Action Items
---------------
- Figure out datastore issues.

23-04-2016 - 7:05 PM
<hr/>
Committed by Mukund
-------------------
1. Updates to eclipse project
   a. Passing parameters to C (all types - int, string, char, pointers, objects)
   b. Read Write APIs from Java
   c. Synthetic data creation write and read back.
   d. Storage of the same in CSV.
   e. Storage of Tuple pointers
2. Read/Write APIs in the C datastore

Issues Resolved
--------------------
Figured out that the out of sync behavior between C and Java is only due to a lack of sync
between the print statements and not the order of execution itself.

Test status
-------------
Testing of read/writes completed for 1000 data items. 

Issues
------------
1. When memory allocated is ~> 418 MB, segmentation fault is seen when writing to
the memory location. Needs to be seen how this can be fixed. If not, the dataset
that can be created will be half of what was planned i.e. ~ 2 ^ 24 records.

Action Items
---------------
1. Indexes
---------------------------------
  a. Write the remaining indexing algorithms. <br/>

2. Integrate
---------------------------------
  a. Integration of indexes and read/write APIs
  
3. Performance Measurement
---------------------------------
  a. Measure time taken for insertion, queries and index sizes.
  
22-04-2016 - 2:50 PM
<hr/>
Committed by Nitish
-------------------
1. Eclipse Project containing
   a. Java code for creating synthetic dataset and the following indexes:
	1. Bplus Tree
	2. Chained bucket Hash
	3. Hash Table

Test status
-------------
Basic sanity test conducted on indexes. Functional and regression tests not done yet.

Issues
------------
1. When memory allocated is ~> 418 MB, segmentation fault is seen when writing to
the memory location. Needs to be seen how this can be fixed. If not, the dataset
that can be created will be half of what was planned i.e. ~ 2 ^ 24 records.

2. Interface from Java to C isn't smooth. Java executes to completion before the 
C function can complete.

Action Items
---------------
1. Figure out interface problems
---------------------------------
  a. How data types will be handled at the Java <-> C Interface?<br/>
  b. How to store Tuple pointers in Java?<br/>
  c. How to serialize structure returned from C API call?<br/>
  d. How to make Java program to wait on C call to return?<br/>
  e. How to pass arguments to the C function call from Java?<br/>

2. Indexes
---------------------------------
  a. Write the remaining indexing algorithms. <br/>
  
3. DataStore Creation
---------------------------------
  a. Create Dummy data and populate the datastore on init.<br/>

10-04-2016 - 11:27 PM
<hr/>
Committed by Mukund
-------------------
1. Eclipse Project containing
   a. Calling C init and destroy data store APIs from Java using JNI
   
   b. Header file from Java class generated using
      javah -jni DatastoreInterface
   
   c. DLL for the DataStore built using Visual C compiler
      cl -I"C:\Program Files (x86)\Java\jdk1.8.0_25\include" -I"C:\Program Files (x86)\Java\jdk1.8.0_25\include\win32"  -LD DatastoreInterface.c -FeDatastoreInterface.dll

Test status
-------------
Interface tested on a dummy API as well as the init and destroy data store APIs

Issues
------------
1. When memory allocated is ~> 418 MB, segmentation fault is seen when writing to
the memory location. Needs to be seen how this can be fixed. If not, the dataset
that can be created will be half of what was planned i.e. ~ 2 ^ 24 records.

2. Interface from Java to C isn't smooth. Java executes to completion before the 
C function can complete.

Action Items
---------------
1. Figure out interface problems
---------------------------------
  a. How data types will be handled at the Java <-> C Interface?<br/>
  b. How to store Tuple pointers in Java?<br/>
  c. How to serialize structure returned from C API call?<br/>
  d. How to make Java program to wait on C call to return?<br/>
  e. How to pass arguments to the C function call from Java?<br/>

2. Indexes
---------------------------------
  a. Write indexing algorithms? <br/>
  
3. DataStore Creation
---------------------------------
  a. Create Dummy data and populate the datastore on init.<br/>

24-03-2016 - 12:26 PM
<hr/>
Committed by Mukund
-------------------
1. Read/Write APIS
2. Dummy data Read/Write

Test status
-------------
APIs tested completely

Issues
------------
When memory allocated is ~> 418 MB, segmentation fault is seen when writing to
the memory location. Needs to be seen how this can be fixed. If not, the dataset
that can be created will be half of what was planned i.e. ~ 2 ^ 24 records.

Action Items
---------------
1. Figure out interface problems
---------------------------------
  a. How data types will be handled at the Java <-> C Interface?<br/>
  b. How to store Tuple pointers in Java?<br/>
  c. How to serialize structure returned from C API call?<br/>

2. Indexes
---------------------------------
  a. How to build indexes in Java? <br/>

24-03-2016 - 01:00 AM
<hr/>
Committed initial structure of the data store - Mukund

Action Items
---------------------
1. User Interface
---------------------
  a. Simple Command line interface<br/>
  b. GUI (if possible)<br/>

2. Data Store
---------------------
  a. Read/Write API Definition.<br/>
  b. Decision on Requirement of any metadata required.<br/>

3. Index
------------------
  a. Thinking of implementing index in Java<br/>

4. Index <-> Data Store Interface
---------------------------------
  a. Java to C API calls.<br/>
  b. Protocol on how pointers can be stored.<br/>
