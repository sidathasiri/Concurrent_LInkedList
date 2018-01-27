---------------------------
To compile c the program
---------------------------

gcc -g -Wall -o <output_file_name> <c_file_name>.c -lpthread -lm

For serial c code: gcc -g -Wall -o serial serial.c -lpthread -lm
For mutex c code: gcc -g -Wall -o mutex mutex.c -lpthread -lm
For read/write lock c code: gcc -g -Wall -o lock lock.c -lpthread -lm

*Note: make sure to include timer.h file to root directory when compiling

------------------------
To run the program
------------------------

./<output_file_name> <initialEntries> <operationsCount> <memberFraction> <insertFraction> <deleteFraction> <threadCount>

*Note: <threadCount> is only required when executing mutex.c and lock.c. For serial.c <threadCount> is not required


    initialEntries = Number of initial unique values in the linked list
    operationsCount = Number of random Member, Insert, and Delete operations performed on the linked list
    memberFraction =  Fraction of Member operations
    insertFraction = Fraction of Insert operations
    deleteFraction = Fraction of Delete operations
    threadCount = number of threads
	

Example for linux: 

For mutex :./mutex 1000 10000 0.99 0.005 0.005 1
For serial :./serial 1000 10000 0.99 0.005 0.005
 
Example for windows: 
For mutex :.\mutex 1000 10000 0.99 0.005 0.005 1
For serial :.\mutex 1000 10000 0.99 0.005 0.005 