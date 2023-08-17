#include <stdlib.h>
#include "Student.h"

Student* createStudent(int gpa,unsigned int totalCredits,char* studentId,char*name,char* surename,char* city,char* department)
{
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if(newStudent==NULL)
    {
        return NULL;
    }
    newStudent->m_gpa = gpa;
    newStudent->m_totalCredits = totalCredits;
    newStudent->m_studentId = studentId;
    newStudent->m_name = name;
    newStudent->m_surename = surename;
    newStudent->m_city = city;
    newStudent->m_department = department;
    newStudent->m_hacker=NULL;
    return newStudent;
}

void destroyStudent(Student* studentToDestoy)
{
    free(studentToDestoy->m_studentId);
    free(studentToDestoy->m_name);
    free(studentToDestoy->m_surename);
    free(studentToDestoy->m_city);
    free(studentToDestoy->m_department);
    free(studentToDestoy);
}
