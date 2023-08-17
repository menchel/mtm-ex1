#ifndef STUDENT_H
#define STUDENT_H
#include "hacker.h"
typedef struct Student
{
    int m_gpa;
    unsigned int m_totalCredits;
    char* m_studentId;
    char * m_name;
    char* m_surename;
    char * m_city;
    char* m_department;
    Hacker* m_hacker;
}Student;

//Create a new pointer to the student
//Returns NULL if fails
Student* createStudent(int gpa,unsigned int totalCredits,char* studentId,char* name,char* surename,char* city,char* department);

//Destroies the student given
void destroyStudent(Student* studentToDestoy);
#endif // STUDENT_H
