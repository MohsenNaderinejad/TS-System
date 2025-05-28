#include "../include/GLOBAL_VARIABLES.hpp"

// GLOBAL SECOND
int ONE_SECOND = 1;
int TWO_SECOND = 1;
int THREE_SECOND = 1;

// GLOBAL INTERFACES
std::string UP_WINDOW_TILE =     "\n\e[1;93m#------------------------ TEACHER AND STUDENT SYSTEM ------------------------#\e[0m\n";
std::string BOTTOM_WINDOW_TILE = "\n\e[1;93m#----------------------------------------------------------------------------#\e[0m\n";
std::string INPUT_SENTENCE = " --> \tPlease Choose An Option: ";
std::string INVALID_INPUT_ERROR = "PLEASE CHOOSE VALID OPTIONS";

// PAGES - MAIN
std::vector<std::string> MAIN_PAGE_MENU = {
    "1. Login",
    "2. Signup\n",
    "\e[0;107m\e[0;37m0. Exit\e[0m"
};

std::vector<std::string> TEACHER_OR_STUDENT_DECIDE_PAGE_MENU = {
    "-1. Return\n",
    "1. Teacher",
    "2. Student\n",
    "0. Exit"
};

// PAGES - LOGIN & SIGNUP
std::vector<std::string> LOGIN_PAGE_MENU = {
    "-1. Return\n",
    "1. ID",
    "2. PASSWORD",
    "3. ROLE - (T/S)",
    "\n4. Enter\n",
    "0. Exit"
};

std::vector<std::string> SIGNUP_TEACHER_PAGE_MENU = {
    "-1. Return\n",
    "1. NAME",
    "2. LASTNAME",
    "3. COURSES",
    "4. ID",
    "5. PASSWORD",
    "6. EMAIL",
    "7. GENDER - (M/F) (O)",
    "8. AGE (O)",
    "9. FEILD OF STUDY (O)",
    "\n10. ENTER\n",
    "0. Exit"
};

std::vector<std::string> SIGNUP_STUDENT_PAGE_MENU = {
    "-1. Return\n",
    "1. NAME",
    "2. LASTNAME",
    "3. FEILD OF STUDY",
    "4. ID",
    "5. PASSWORD",
    "6. EMAIL",
    "7. GENDER - (M/F) (O)",
    "8. AGE (O)",
    "\n9. ENTER\n",
    "0. Exit"
};

// PAGES - TEACHER
std::vector<std::string> TEACHER_MAIN_PAGE_MENU = {
    "\n-2. Delete Teacher",
    "\n-1. Logout\n",
    "1. Create New Exam\n",
    "2. SELECT EXAM GUC",
    "3. VIEW EXAM",
    "4. VIEW SELECTED EXAM SCORE BOARD\n",
    "\n0. Exit"
};

std::vector<std::string> TEACHER_EXAM_CREATION_PAGE_MENU = {
    "-2. Logout",
    "-1. Return\n",
    " $$$ EXAM GENERATED UNIQUE CODE: ",
    "\n1. EXAM NAME",
    "2. SUBJECT",
    "3. TIME (MINUTES)",
    "4. TYPE - (i.e. Midterm, Finals, and etc.)",
    "5. PUBLISHED (Y/N)",
    "\n6. CREATE\n",
    "0. Exit"
};

std::vector<std::string> TEACHER_SPECIFIC_EXAM_VIEW_PAGE_MENU = {
    "-2. Logout",
    "-1. Return\n",
    "\e[47m\e[0;31m $$$ EXAM GENERATED UNIQUE CODE: ",
    "\n $$$ EXAM NAME",
    " $$$ SUBJECT",
    " $$$ TIME (MINUTES)",
    " $$$ TYPE - (i.e. Midterm, Finals, and etc.)",
    " $$$ PUBLISHED",
    "\e[0m\n1. EDIT\n",
    " $$$ --> CHANGE VIEW \n\t(MCQ --> Multiple Choice Questions)\n\t(SFQ --> SHORT FORM QUESTIONS)\n\t(FFQ --> FULL FORM QUESTION)\n",
    "0. Exit"
};

std::vector<std::string> TEACHER_EXAM_EDIT_PAGE_MENU = {
    "-2. Logout (With Saving)",
    "-1. Return Without Saving\n",
    " $$$ EXAM GENERATED UNIQUE CODE: ",
    "\n1. EXAM NAME (E)",
    "2. SUBJECT (E)",
    "3. TIME (MINUTES) (E)",
    "4. TYPE (E)",
    "5. PUBLISHED (Y/N) (E)",
    "\n6. ADD QUESTION",
    "7. EDIT QUESTION",
    "8. REMOVE QUESTION\n",
    " $$$ --> CHANGE VIEW \n\t(MCQ --> Multiple Choice Questions)\n\t(SFQ --> SHORT FORM QUESTIONS)\n\t(FFQ --> FULL FORM QUESTION)\n",
    "\e[1;91m9. DELETE EXAM\e[0m\n",
    "\e[1;95m10. SAVE & RETURN\e[0m\n"
};

std::vector<std::string> TEACHER_ADD_EDIT_QUESTION_PAGE_MENU = {
    "-2. Logout (Without Saving)",
    "-1. Return (Without Saving)\n",    
    "\e[1;92m $$$ QUESTION NUMBER: ",
    "\e[0m\n $ 1. QUESTION FACE (E)",
    " $ 2. ANSWER(S) (E) (NOT FOR FULL QUESTIONS)",
    " $ 3. CORRECT SCORE (E) (NOT FOR FULL QUESTIONS)",
    " $ 4. WRONG SCORE (E) (NOT FOR FULL QUESTIONS)",
    "\n\e[1;93m5. ENTER\e[0m"
};

std::vector<std::string> TEACHER_SPECIFIC_EXAM_SCORE_BOARD_PAGE_MENU = {
    "-2. Logout\n",
    "-1. Return\n",
    "\e[1;92m $$$ EXAM GENERATED UNIQUE CODE: ",
    "\e[0m\n $$$ EXAM NAME",
    " $$$ SUBJECT",
    " $$$ TIME (MINUTES)",
    " $$$ TYPE - (i.e. Midterm, Finals, and etc.)",
    " $$$ PUBLISHED",
    "\n ### MAXIMUM SCORE: ",
    " ### AVERAGE SCORE: ",
    "\n1. SELECT STUDENT ID",
    "2. GRADE SELECTED STUDENT",
    "\n3. PRINT SCORE BOARD\n",
    "0. Exit"
};

std::vector<std::string> TEACHER_SPECIFIC_STUDENT_GRADING_PAGE_MENU = {
    "-2. Logout (With Saving)\n",
    "-1. Return (Without Saving)\n",
    "\e[1;92m $$$ EXAM GENERATED UNIQUE CODE: ",
    " $$$ EXAM NAME",
    "\n $$$ STUDENT ID: ",
    " $$$ STUDENT NAME: ",
    " $$$ STUDENT SCORE (TILL NOW): ",
    "\e[0m\n1. GRADE QUESTION",
    "2. DELETE GRADE",
    "\n3. SAVE & RETURN"
};

// PAGES - STUDENT
std::vector<std::string> STUDENT_MAIN_PAGE_MENU = {
    "\n-2. Delete Student",
    "\n-1. Logout\n",
    "1. Enroll In New Exam",
    "\n2. Select An Exam To Participate",
    "3. Start The Selected Exam\n",
    "4. Select A Participated Exam to View",
    "5. View Participated Exam",
    "\n0. Exit"
};

std::vector<std::string> STUDENT_EXAM_PARTICIPATION_PAGE_MENU = {
    "-1. Logout (ENDING THE EXAM)\n",
    "\n $$$ --> CHANGE VIEW \n\t(MCQ --> Multiple Choice Questions)\n\t(SFQ --> SHORT FORM QUESTIONS)\n\t(FFQ --> FULL FORM QUESTION)\n",
    "1. ANSWER QUESTION",
    "2. DELETE ANSWER\n",
    "\n3. END EXAM"
};

std::vector<std::string> STUDENT_EXAM_REPORT_PAGE_MENU = {
    "-2. Logout\n",
    "-1. Return\n",
    "\n $$$ --> CHANGE VIEW \n\t(MCQ --> Multiple Choice Questions)\n\t(SFQ --> SHORT FORM QUESTIONS)\n\t(FFQ --> FULL FORM QUESTION)\n",
    "1. PRINT EXAM REPORT",
    "\n0. Exit"
};

std::string quote(const std::string& s) {
    return "\"" + s + "\"";
}

std::string ReplaceWhitespaceWithDash(const std::string& input) {
    std::string result = input;
    std::replace(result.begin(), result.end(), ' ', '-');
    return result;
}
std::string (*RWD)(const std::string&) = ReplaceWhitespaceWithDash;

void Clear() {
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

std::string ADDRESS_CREATOR(std::vector<std::string> args) {
    std::string ADDRESS_SEPRATOR;
    #ifdef _WIN32
        ADDRESS_SEPRATOR = "\\";
    #else
        ADDRESS_SEPRATOR = "/";
    #endif
    std::string address = "";
    for (const auto& arg : args) {
        address += arg + ADDRESS_SEPRATOR;
    }
    address.pop_back();
    return address;
}

void CreateDirectory(std::string DirectoryName) {
    #if defined _WIN32
        system(("mkdir " + DirectoryName).c_str());
    #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
        system(("mkdir -p " + DirectoryName).c_str());
    #elif defined (__APPLE__)
        system(("mkdir -p " + DirectoryName).c_str());
    #endif
}

void CatchException(const std::invalid_argument& error, const int Time){
    Clear();
    std::cerr << UP_WINDOW_TILE << "\e[1;91m" << error.what() << "\e[0m" << BOTTOM_WINDOW_TILE;
    std::this_thread::sleep_for(std::chrono::seconds(Time));
    Clear();
}

void SuccessfulMessage(std::string Message, const int Time){
    Clear();
    std::cout << UP_WINDOW_TILE;
    std::string FullMessage = "\n\t \e[1;92m$$$ --> " + Message + "!\n\e[0m";
    std::cout << FullMessage;
    std::cout << BOTTOM_WINDOW_TILE;
    std::this_thread::sleep_for(std::chrono::seconds(Time));
    Clear();
}

void UnSuccessfulMessage(std::string Message){
    std::string FullMessage = "\n\t !!! --> " + Message + "!\n";
    throw std::invalid_argument(FullMessage);
}