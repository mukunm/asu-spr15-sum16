jsm:
	gcc -Wall -pthread -o jsm.out journal_storage_manager.c 

css:
	gcc -Wall -pthread -o css.out cell_storage_system.c
	
css_thread_lock:
	gcc -Wall -pthread -o css.out -D THREAD_LOCKING_TEST cell_storage_system.c
	
jsm_faulty:
	gcc -Wall -pthread -o jsm.out -D FAULTY_OPERATIONS journal_storage_manager.c

css_faulty:
	gcc -Wall -pthread -o css.out -D FAULTY_OPERATIONS cell_storage_system.c
