#ifndef SHARED_H
#define SHARED_H


#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


struct shm_var {
	int multiple;
	int counter;
	int delay;
};

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

// lock
static void sem_p(int semid) {
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;

	if (semop(semid, &sb, 1) == -1) {
		perror("sem_p failed\n");
		exit(1);
	}
}

// unlock
static void sem_v(int semid) {
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;

	if (semop(semid, &sb, 1) == -1) {
		perror("sem_v failed\n");
		exit(1);
	}
}

#endif
