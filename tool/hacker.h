#ifndef HACKER_H
#define HACKER_H

#define STUDENT_ID_LENGTH 9

typedef struct Hacker
{
    char * m_studentId;
    char** m_courseNumbers;
    int m_courseNumbersSize;
    char** m_friendlyStudentsId;
    int m_friendlyStudentsIdSize;
    char** m_rivalStudentsId;
    int m_rivalStudentsIdSize;
}Hacker;

//Creates a new pointer to a hacker
//Returns NULL if malloc failes
Hacker* createHacker(char *studentId,char** courseNumbers,int courseSize,char** friendlyStudentsId,int friendsSize,char**rivalStudentsId,int rivalsSize);

//Destroies the hacker
void destroyHacker(Hacker* hackerToDestroy);

#endif // HACKER_H
