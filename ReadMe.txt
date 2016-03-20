Implement a LinkedList Library and expose it via an API


Overview:
---------

1. An Application should include LinkedListLib.h and invoke LinkedListLib_Init()
to create a LinkedListLib Instance for the app.

2. The LinkedListLib Instance is implemented as a Singleton - so an App
should invoke LinkedListLib_Init() only once.

3. An Application can have LINKEDLISTLIB_MAX_NUM_LISTS linked lists at the same time - all
managed by LinkedListLib Instance

4. Each linked list can have a maximum of LINKEDLISTLIB_MAX_NUM_NODES

5. Elements can be added and deleted from both rear and from front

6. The library will give an App a "listHandle" - which the app should use
to perform any operation on the list

7. LinkedListLib Instance will create a log file named LinkedListLib-<pid>.log
   4 log levels are supported.


List of files & What they do:
-----------------------------

LinkedListLib.h : API declaration. Apps should #include this file
LinkedListLib.c : Implementation of functions declared in LinkedListLib.h

LinkedListLib_int.h : Function declarations of internally used functions
LinkedListLib_int.c : Definitions of functions declared in LinkedListLib_int.h

LinkedListLib_Log.h : 4 Log levels & corresponding Logging macros are exposed.
	              The logFile created would have a name "LinkedListLib-<pid>.log"
LinkedListLib_Log.c : Implementation of Logging macros, multiple log levels,
		      formatting log message prefix & logging to file.

LinkedListLibTestApp.c : Test App to exercise the LinkedListLib
                         Lots of Tests that exercise API defined in LinkedListLib.h
LinkedListLibTestApp.h : 


TODO / Future Work:
-------------------

1. Data type can be only be "int" right now. 
   Extend to support any generic type of data.

2. Expose an API to configure LinkedListLib Instance logging levels
   Currently, it is set to LOG_LEVEL_DEFAULT at compile time.

3. Expose an API to search / retrieve elements within a particular linked
   list or within the entire LinkedListLib Instance.
