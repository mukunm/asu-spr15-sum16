=============
Readme
=============
The aim of the project was to develop 

1. A file system emulator (a RAM disk of sorts) which persists 
   files in the RAM as long as the process is running
   
2. An application which uses the emulator to perform read/write
   operations in a multi threaded fashion. The file system had
   to handle concurrent writes and be fault tolerant.
   
The implementation consists of 2 modules Cell Storage System (CSS)
and the Journal Storage Manager(JSM) which run as 2 separate processes

The CSS acts like a back-end storing all file data and metadata.
The JSM acts like the front-end providing, read/write interface to the
user.

Further details are furnished in the report

==============
Build and Run
==============
Use the makefile to build in different configurations.
Run the CSS first in one terminal and the JSM in another terminal.
The print messages on JSM are self explanatory to take one through
usage of the system.

==============
Known issues
==============
Shared memory and memory allocated aren't freed when the processes are
terminated.
