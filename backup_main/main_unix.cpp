#include "../include/User.h"
#include "../include/Teacher.h"
#include "../include/Student.h"
#include "../include/Exam.hpp"
#include "../include/json.hpp"
#include "../include/MAIN_MENU_PAGE.h"
#include "../include/GLOBAL_VARIABLES.hpp"
#include <fstream>

using json = nlohmann::json;

int main(int argc, char ** argv) {

    // Getting Seprator for the system + Address
    std::string ADDRESS_SEPRATOR = "/";

    std::string TEACHERS_MAIN_JSON_ADDRESS = "data" + ADDRESS_SEPRATOR + "MAIN_DATAS" + ADDRESS_SEPRATOR + "teachers.json";
    std::string STUDENTS_MAIN_JSON_ADDRESS = "data" + ADDRESS_SEPRATOR + "MAIN_DATAS" + ADDRESS_SEPRATOR + "students.json";
    std::string EXAMS_MAIN_JSON_ADDRESS = "data" + ADDRESS_SEPRATOR + "MAIN_DATAS" + ADDRESS_SEPRATOR + "exams.json";
    std::string QUESTIONS_MAIN_JSON_ADDERSS = "data" + ADDRESS_SEPRATOR + "EXAM_QUESTIONS" + ADDRESS_SEPRATOR + "questions.json";

    // Opening Files And Handling Exceptions and errors (teachers.json - students.json - exams.json)
    std::ifstream TEACHER_READ_FILE(TEACHERS_MAIN_JSON_ADDRESS);
    std::ifstream STUDENT_READ_FILE(STUDENTS_MAIN_JSON_ADDRESS);
    std::ifstream EXAM_READ_FILE(EXAMS_MAIN_JSON_ADDRESS);
    std::ifstream QUESTIONS_READ_FILE(QUESTIONS_MAIN_JSON_ADDERSS);

    // Checking teachers.json for opening
    if (!TEACHER_READ_FILE.is_open()) {
        std::cout << "File 'teachers.json' does not exist. Creating a new file..." << std::endl;
        TEACHER_READ_FILE.open(TEACHERS_MAIN_JSON_ADDRESS, std::ios::out);
        if (!TEACHER_READ_FILE.is_open()) {
            std::cerr << "Error: Could not create file 'teachers.json'!" << std::endl;
            return 1;
        }
        TEACHER_READ_FILE.close();
        TEACHER_READ_FILE.open(TEACHERS_MAIN_JSON_ADDRESS, std::ios::in | std::ios::out);
    }

    // Checking students.json for opening
    if (!STUDENT_READ_FILE.is_open()) {
        std::cout << "File 'students.json' does not exist. Creating a new file..." << std::endl;
        STUDENT_READ_FILE.open(STUDENTS_MAIN_JSON_ADDRESS, std::ios::out);
        if (!STUDENT_READ_FILE.is_open()) {
            std::cerr << "Error: Could not create file 'students.json'!" << std::endl;
            return 1;
        }
        STUDENT_READ_FILE.close();
        STUDENT_READ_FILE.open(STUDENTS_MAIN_JSON_ADDRESS, std::ios::in | std::ios::out);
    }

    // Checking exams.json for opening
    if (!EXAM_READ_FILE.is_open()) {
        std::cout << "File 'exams.json' does not exist. Creating a new file..." << std::endl;
        EXAM_READ_FILE.open(EXAMS_MAIN_JSON_ADDRESS, std::ios::out); // Use `EXAM_READ_FILE`, not `STUDENT_READ_FILE`
        if (!EXAM_READ_FILE.is_open()) {
            std::cerr << "Error: Could not create file 'exams.json'!" << std::endl;
            return 1;
        }
        EXAM_READ_FILE.close();
        EXAM_READ_FILE.open(EXAMS_MAIN_JSON_ADDRESS, std::ios::in | std::ios::out);
    }

    // Checkign questions.json for opening
    if (!QUESTIONS_READ_FILE.is_open()) {
        std::cout << "File 'questions.json' does not exist. Creating a new file..." << std::endl;
        QUESTIONS_READ_FILE.open(QUESTIONS_MAIN_JSON_ADDERSS, std::ios::out);
        if (!QUESTIONS_READ_FILE.is_open()) {
            std::cerr << "Error: Could not create file 'questions.json'!" << std::endl;
            return 1;
        }
        QUESTIONS_READ_FILE.close();
        QUESTIONS_READ_FILE.open(QUESTIONS_MAIN_JSON_ADDERSS, std::ios::in | std::ios::out);
    }

    // Extracting JSON informations for FILES to JSON Object -- Teacher Section
    json TEACHER_JSON_FILE;
    if (TEACHER_READ_FILE.peek() == std::ifstream::traits_type::eof()) {
        TEACHER_JSON_FILE = json::array();
    } else {
        TEACHER_READ_FILE >> TEACHER_JSON_FILE;
    }

    // Extracting JSON informations for FILES to JSON Object -- Student Section
    json STUDENT_JSON_FILE;
    if (STUDENT_READ_FILE.peek() == std::ifstream::traits_type::eof()) {
        STUDENT_JSON_FILE = json::array();
    } else {
        STUDENT_READ_FILE >> STUDENT_JSON_FILE;
    }

    // Extracting JSON informations for FILES to JSON Object -- Student Section
    json EXAM_JSON_FILE;
    if (EXAM_READ_FILE.peek() == std::ifstream::traits_type::eof()) {
        json exam_json_section_guc = json::array();
        json exam_guc_object = json::object();
        exam_guc_object["NEW_EXAM_GUC"] = 1;
        exam_json_section_guc.push_back(exam_guc_object);
        json exam_json_section_exams = json::array();
        EXAM_JSON_FILE.push_back(exam_json_section_guc);
    } else {
        EXAM_READ_FILE >> EXAM_JSON_FILE;
    }

    json QUESTION_JSON_FILE;
    if (QUESTIONS_READ_FILE.peek() == std::ifstream::traits_type::eof()) {
        QUESTION_JSON_FILE = json::array();
    } else {
        QUESTIONS_READ_FILE >> QUESTION_JSON_FILE;
    }

    TEACHER_READ_FILE.close();
    STUDENT_READ_FILE.close();
    EXAM_READ_FILE.close();
    QUESTIONS_READ_FILE.close();

    // APP START
    std::string USER_INPUT;
    while (USER_INPUT != "0"){
        MainMenuInterface(USER_INPUT, TEACHER_JSON_FILE, STUDENT_JSON_FILE, EXAM_JSON_FILE, QUESTION_JSON_FILE);
    }

    std::ofstream TEACHER_WRITE_FILE(TEACHERS_MAIN_JSON_ADDRESS);
    std::ofstream STUDENT_WRITE_FILE(STUDENTS_MAIN_JSON_ADDRESS);
    std::ofstream EXAM_WRITE_FILE(EXAMS_MAIN_JSON_ADDRESS);
    std::ofstream QUESTION_WRITE_FILE(QUESTIONS_MAIN_JSON_ADDERSS);

    // Closing TEACHER_WRITE_FILE file
    TEACHER_WRITE_FILE.seekp(0);
    TEACHER_WRITE_FILE << TEACHER_JSON_FILE.dump(4);
    TEACHER_WRITE_FILE.close();

    // Closing STUDENT_WRITE_FILE file
    STUDENT_WRITE_FILE.seekp(0);
    STUDENT_WRITE_FILE << STUDENT_JSON_FILE.dump(4);
    STUDENT_WRITE_FILE.close();

    // Closing EXAM_WRITE_FILE file
    EXAM_WRITE_FILE.seekp(0);
    EXAM_WRITE_FILE << EXAM_JSON_FILE.dump(4);
    EXAM_WRITE_FILE.close();

    // Closing QUESTION_WRITE_FILE file
    QUESTION_WRITE_FILE.seekp(0);
    QUESTION_WRITE_FILE << QUESTION_JSON_FILE.dump(4);
    QUESTION_WRITE_FILE.close();

    return 0;
}