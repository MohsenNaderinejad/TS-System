#include "User.h"

void User::setName(std::string name){
    std::regex pattern("([a-zA-Z]+)");
    if (!regex_match(name, pattern)) {
        throw std::invalid_argument("\n --> \tNAME: DOES NOT FOLLOW PATTERN!\n --> \tUSE A REAL ONE :)\n");
    }
    this->Name = name;
}

void User::setLast_Name(std::string last_name){
    std::regex pattern("([a-zA-Z]+)");
    if (!regex_match(last_name, pattern)) {
        throw std::invalid_argument("\n --> \tLAST NAME: DOES NOT FOLLOW PATTERN!\n --> \tUSE A REAL ONE :)\n");
    }
    this->Last_Name = last_name;
}

void User::setEmail(std::string email, json& file){
    for (const auto& user : file){
        if (user["EMAIL"] == email){
            UnSuccessfulMessage("EMAIL: ALREADY IN USE");
        }
    }
    std::regex pattern("^([a-zA-Z0-9._]+@[a-zA-Z0-9.]+\\.[a-zA-Z]{2,})$");
    if (!regex_match(email, pattern)){
        throw std::invalid_argument("\n --> \tEMAIL: IT IS NOT AN EMAIL!\n --> \tUSE A REAL ONE :)\n");
    }
    this->Email = email;
}

void User::setPassWD(std::string passwd){
    if (passwd.length() <= 7){
        UnSuccessfulMessage("PASSWROD SHOULD BE AT LEAST 8 CHARACTERS!");
    }
    this->PassWD = passwd;
}

void User::setID(std::string ID){
    std::regex pattern("^([0-9])+$");
    if (!regex_match(ID, pattern)){
        throw std::invalid_argument("\n --> \tID: IT IS NOT AN ID!\n --> \tUSE A REAL ONE :)\n");
    }
    this->ID = ID;
}

void User::setGender(std::string gender){
    this->Gender = gender;
}

void User::setAge(int age){
    this->Age = age;
}

User::User(std::string name, std::string last_name, std::string email, std::string passwd, std::string ID, json& file, std::string gender="", int age=0){
    if (name.empty() || last_name.empty() ||
    email.empty() || passwd.empty() || ID.empty()) {
        UnSuccessfulMessage("PLEASE FILL OUT ALL OF THE NECESSARY SLOTS");
    }
    
    for (const auto& person: file){
        if (ID == person["ID"]){
            UnSuccessfulMessage("ID: ALREADY IN USE");
        }
    }
    User::setName(name);
    User::setLast_Name(last_name);
    User::setEmail(email, file);
    User::setPassWD(passwd);
    User::setID(ID);
    User::setGender(gender);
    User::setAge(age);

    json new_person = {
        {"NAME", name},
        {"LAST_NAME", last_name},
        {"ID", ID},
        {"EMAIL", email},
        {"PASSWD", passwd},
        {"GENDER", gender},
        {"AGE", age}
    };

    file.push_back(new_person);
}

bool User::DeleteUser(json& file) {
    std::string flag;
    std::cout << "\n\t !!!! ARE YOU SURE THAT ARE YOU SURE ???? (Y/n): ";
    std::getline(std::cin, flag);
    if (flag == "Y") {
        for (auto person = file.begin(); person != file.end(); person++) { 
            if ((*person)["ID"] == this->ID) { 
                person = file.erase(person);
                SuccessfulMessage("USER DELETED SUCCESSFULLY", TWO_SECOND);
                return true;
                break;
            }
        }
    }
    return false;
}

User::User() 
    : Name(""), Last_Name(""), Email(""), PassWD(""), Gender(""), ID(""), Age(0) {
}
std::string User::getName(){
    return this->Name;
}
std::string User::getLast_Name(){
    return this->Last_Name;
}
std::string User::getEmail(){
    return this->Email;
}
std::string User::getPassWD(){
    return this->PassWD;
}
std::string User::getID(){
    return this->ID;
}
std::string User::getGender(){
    return this->Gender;
}
int User::getAge(){
    return this->Age;
}
