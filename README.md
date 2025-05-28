Teacher-Student SYSTEM v2.1

--> for compiling it use the command below:

! Please Run the Commands in the main folder !

! The Difference between two commands for main_wins and main_unix is in Addressing in files if one doesn't run on your OS try the other one !

  --> For Windows:
  
	g++ src/*.cpp exam/*.cpp user/*.cpp user/teacher/*.cpp user/student/*.cpp -Iinclude -o TS-APP

  --> You can use "make" command in minGW too! (it will be saved as TS-APP_minGWC)
	--> only in minGW terminal!

!!!! if it does not work --- > replace the main.cpp in ./src with ./backup_main/main_unix.cpp !!!!

the prinited data are all stored in the ./report folder --> if you did chose the default directory
					--> if not, it has been created in the given directory

Author:
	Mohsen Naderinejad - March 2025
