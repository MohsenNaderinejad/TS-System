# Teacher-Student SYSTEM

## Description
This is a Teacher-Student interactive system that signups and logins teachers and students - Saving Datas Using JSON files

it provides:
- Teachers:
	- Login
   	- Signup
   	- Create Exams:
   	  	- Name
   	  	- Subject
   	  	- Type
   	  	- Time
   	  	- Published for Students
   	- Delete Exams
   	- View Their Exams
   	- Add New Questions:
   	  	- Multiple Choice Question (__No Limit__)
   	  	- Short Answer Questions
   	  	- Full Answer Questions
   	- Remove Questions
   	- Edit Info Questions
   	- Grade Students Answers:
   	  	- Add New Scores
   	  	- Delete Scores
   	- Get Report Cards of The Exam:
   	  	- All Scores
   	  	- Maximum Score
   	  	- Average Score
  		- __PRINTABLE__
   	- Delete Account

 - Students:
   	- Login
   	- Signup
   	- Enroll in Exams
   	- Participate in Exams (__Published Ones__) --> *IN LIMIED TIME*
   	- View Their Exam Report Cards:
   	  	- Updates After grading phase of Teacher
   	  	- Their Full Score
   	  	- Their Rank
   	  	- Maximum Score
   	  	- Average Score
   	- __PRINT THE EXAM REPORT CARD__
   	- Delete Account

## Notice !
__for compiling it use the instructions below__:

__*Please Run the Commands in the main folder*__

__The Difference between two commands for main_wins and main_unix is in Addressing in files if one doesn't run on your OS try the other one__

   *For Windows, Mac, and Linux-Based*:
  
	g++ src/*.cpp exam/*.cpp user/*.cpp user/teacher/*.cpp user/student/*.cpp -Iinclude -o TS-APP

   *You can use __"make"__ command in minGW too! ----> __(it will be saved as TS-APP_minGWC)__
	--> only in minGW terminal!*

#### if it does not work --- > replace the main.cpp in ./src with ./backup_main/main_unix.cpp

### the prinited data are all stored in the ./report folder: 
 - if you did chose the default directory
 - if not, it has been created in the given directory

### Author:
	Mohsen Naderinejad - March 2025
