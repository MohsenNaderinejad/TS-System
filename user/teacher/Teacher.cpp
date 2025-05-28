#include "../include/Teacher.h"

Teacher::Teacher() 
    : User(), Field_Of_Study("") {
    Exam_IDs = std::set<long long>();
    Courses = std::set<std::string>();
}

Teacher::Teacher(std::string name, std::string last_name, std::string email, std::string passwd, std::string ID, json& file, std::set<std::string> Courses, std::string fos, std::string gender, int age)
    : User(name, last_name, email, passwd, ID, file, gender, age),
      Field_Of_Study(fos), Courses(Courses)
{
    for (auto& person : file) {
        if (person["ID"] == ID) {
            json course_list = json::array();
            json exam_lists = json::array();
            for (auto item: Courses) {
                course_list.push_back(item);
            }
            person["COURSES"] = course_list;
            person["EXAM_LIST"] = exam_lists;
            person["FOS"] = fos;
        }
    }

    SuccessfulMessage("SIGNED UP SUCCESSFULLY", TWO_SECOND);

}

void Teacher::login(std::string ID, std::string passwd, json& file){
    if (ID.empty() || passwd.empty()) {
        UnSuccessfulMessage("ID AND PASSWORD CANNOT BE EMPTY");
    }
    this->setID(ID);
    for (const auto& person: file){
        if (ID == person["ID"] && passwd == person["PASSWD"]){
            this->Name = person["NAME"];
            this->Last_Name = person["LAST_NAME"];
            this->ID = person["ID"];
            this->Email = person["EMAIL"];
            this->Gender = person["GENDER"];
            this->Age = person["AGE"];
            this->PassWD = person["PASSWD"];
            this->Field_Of_Study = person["FOS"];

            this->Courses.clear();
            for (const auto& course : person["COURSES"]) {
                this->Courses.emplace(course);
            }

            this->Exam_IDs.clear();
            for (const auto& exam_id : person["EXAM_LIST"]) {
                this->Exam_IDs.emplace(exam_id);
            }

            std::string WELCOME = "WELCOME " + this -> Name + " " + this -> Last_Name;
            SuccessfulMessage("LOGGED IN SUCCESSFULLY", TWO_SECOND);
            SuccessfulMessage(WELCOME, TWO_SECOND);
            return;
        }
    }
    throw std::invalid_argument("\n -->\t ID NOT FOUND! / PASSWORD INCORRECT!\n");
}

void Teacher::teacherFileUpdate(json& file){
    for (auto& person : file){
        if (person["ID"] == this->ID){
            person["AGE"] = this->Age;
            person["EMAIL"] = this->Email;
            person["FOS"] = this->Field_Of_Study;
            person["GENDER"] = this->Gender;
            person["LAST_NAME"] = this->Last_Name;
            person["NAME"] = this->Name;
            person["PASSWD"] = this->PassWD;

            json course_list = json::array();
            json exam_lists = json::array();
            for (auto item: this->Courses) {
                course_list.push_back(item);
            }
            for (auto item: this->Exam_IDs) {
                exam_lists.push_back(item);
            }
            person["COURSES"] = course_list;
            person["EXAM_LIST"] = exam_lists;
        }
    }
}

void Teacher::insertExamID(long long GUC){
    Exam_IDs.insert(GUC);
}

void Teacher::deleteExamID(long long GUC) {
    // I Did not put any fail-safe because I am using this function strictly
    // I Know when I Am deleting
    auto it = Exam_IDs.find(GUC);
    if (it != Exam_IDs.end()) {
        Exam_IDs.erase(it);
    }
}

void Teacher::viewExams(){
    std::cout << "\t___________________________________\n";
    std::cout << "\n --> \tEXAMS ID:\n";
    int Index = 1;
    for (const auto item: this->Exam_IDs){
        std::cout << "\n\t\t# " << Index << ". " << item;
        Index++;
    }
    std::cout << "\n\t___________________________________\n";
}