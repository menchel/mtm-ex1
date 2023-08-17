CC=gcc
OBJS=IsraeliQueue.o israeliQueueElement.o node.o main.o HackEnrollment.o hacker.o Course.o Student.o
EXEC =program
DEBUG= -DNDEBUG
CFLAGS= -std=c99 -lm -I/new_home/courses/mtm/public/2223b/ex1 -Itool -Wall -pedantic-errors -Werror

$(EXEC) : $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG) $(OBJS) -o HackEnrollment
main.o: IsraeliQueue.c  israeliQueueElement.c node.c tool/main.c tool/HackEnrollment.c tool/hacker.c tool/Course.c tool/Student.c /new_home/courses/mtm/public/2223b/ex1/IsraeliQueue.h tool/israeliQueueElement.h tool/node.h tool/HackEnrollment.h tool/hacker.h tool/Course.h tool/Student.h
	$(CC) -c $(CFLAGS) $(DEBUG) $ tool/main.c
HackEnrollment.o: IsraeliQueue.c  israeliQueueElement.c node.c tool/HackEnrollment.c tool/hacker.c tool/Course.c tool/Student.c /new_home/courses/mtm/public/2223b/ex1/IsraeliQueue.h tool/israeliQueueElement.h tool/node.h tool/HackEnrollment.h tool/hacker.h tool/Course.h tool/Student.h
	$(CC) -c $(CFLAGS) $(DEBUG) $ tool/HackEnrollment.c
Course.o : tool/Course.c IsraeliQueue.c node.c israeliQueueElement.c tool/Course.h /new_home/courses/mtm/public/2223b/ex1/IsraeliQueue.h tool/node.h tool/israeliQueueElement.h
	$(CC) -c $(CFLAGS) $(DEBUG) $ tool/Course.c
hacker.o : tool/hacker.c tool/hacker.h
	$(CC) -c $(CFLAGS) $(DEBUG) $ tool/hacker.c
IsraeliQueue.o : IsraeliQueue.c node.c israeliQueueElement.c /new_home/courses/mtm/public/2223b/ex1/IsraeliQueue.h tool/node.h tool/israeliQueueElement.h
	$(CC) -c $(CFLAGS) $(DEBUG) $ IsraeliQueue.c
israeliQueueElement.o : israeliQueueElement.c tool/israeliQueueElement.h
	$(CC) -c $(CFLAGS) $(DEBUG) $ israeliQueueElement.c
node.o:  node.c tool/node.h
	$(CC) -c $(CFLAGS) $(DEBUG) $ node.c
Student.o: tool/Student.c tool/hacker.c tool/Student.h tool/hacker.h
	$(CC) -c $(CFLAGS) $(DEBUG) $ tool/Student.c
clean: 
	rm -f $(OBJS) HackEnrollment