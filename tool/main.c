#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "HackEnrollment.h"
#define NUM_WITH_FLAG 7
#define NUM_WITHOUT_FLAG 6
int main(int argc,char** argv)
{
    if(argc!=NUM_WITHOUT_FLAG && argc!=NUM_WITH_FLAG)
    {
        return 0;
    }
    bool withBigCharacters=true;
    if(withBigCharacters)
    {

    }
    int startIndex=1;
    if(NUM_WITH_FLAG==argc)
    {
        if(strcmp(argv[1],"-i")==0)
        {
            withBigCharacters=false;
        }
        startIndex++;
    }
    FILE* students=fopen(argv[startIndex],"r");
    FILE* hackers=fopen(argv[startIndex+2],"r");
    FILE* courses=fopen(argv[startIndex+1],"r");
    FILE* queues=fopen(argv[startIndex+3],"r");
    FILE* out=fopen(argv[startIndex+4],"w");
    if(students!=NULL && hackers!=NULL && courses!=NULL && queues!=NULL && out!=NULL)
    {
        EnrollmentSystem system=createEnrollment(students,courses,hackers);
        if(system!=NULL)
        {
            updateFlag(system,withBigCharacters);
            system=readEnrollment(system,queues);
            if(system!=NULL)
            {
               hackEnrollment(system,out);
               deleteEnrollment(system);
            }
        }
    }
    fclose(students);
    fclose(hackers);
    fclose(courses);
    fclose(queues);
    fclose(out);
    return 0;
}

