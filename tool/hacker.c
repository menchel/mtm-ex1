#include <stdlib.h>
#include <stdio.h>
#include "hacker.h"
Hacker* createHacker(char *studentId,char** courseNumbers,int courseSize,char** friendlyStudentsId,int friendsSize,char**rivalStudentsId,int rivalsSize)
{
    Hacker* newHacker=(Hacker*)malloc(sizeof(Hacker));
    if(newHacker==NULL)
    {
        return NULL;
    }
    newHacker->m_studentId=studentId;
    newHacker->m_courseNumbers=courseNumbers;
    newHacker->m_courseNumbersSize=courseSize;
    newHacker->m_friendlyStudentsId=friendlyStudentsId;
    newHacker->m_friendlyStudentsIdSize=friendsSize;
    newHacker->m_rivalStudentsId=rivalStudentsId;
    newHacker->m_rivalStudentsIdSize=rivalsSize;
    return newHacker;
}

void destroyHacker(Hacker* hackerToDestroy)
{
    char**temp=hackerToDestroy->m_courseNumbers;
    for(int i=0;i<hackerToDestroy->m_courseNumbersSize;i++)
    {
        free(temp[i]);
    }
    temp=hackerToDestroy->m_friendlyStudentsId;
    for(int i=0;i<hackerToDestroy->m_friendlyStudentsIdSize;i++)
    {
        free(temp[i]);
    }
    temp=hackerToDestroy->m_rivalStudentsId;
    for(int i=0;i<hackerToDestroy->m_rivalStudentsIdSize;i++)
    {
        free(temp[i]);
    }
    free(hackerToDestroy->m_courseNumbers);
    free(hackerToDestroy->m_friendlyStudentsId);
    free(hackerToDestroy->m_rivalStudentsId);
    free(hackerToDestroy->m_studentId);
    free(hackerToDestroy);
}
