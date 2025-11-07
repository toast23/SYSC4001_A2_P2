#include <sys/wait.h>

#include "shared_101256669_101298080.h"


int main() {
	// shm set up
	key_t key_shm = 1234;
	int shmid = shmget(key_shm, sizeof(struct shm_var), IPC_CREAT | 0666);

	if (shmid < 0) {
		perror("shmget failed");
		exit(1);
	}

	struct shm_var *shm = (struct shm_var *)shmat(shmid, NULL, 0);

	if (shm == (void*)-1) {
		perror("shmat failed");
		exit(1);
	}

	shm->multiple = 3;
	shm->counter = 0;
	shm->delay = 62500;


	// sem set up
	key_t key_sem = 5678;
	int semid = semget(key_sem, 1, IPC_CREAT | 0666);

	if (semid < 0) {
		perror("semget failed");
		exit(1);
	}

	if (semctl(semid, 0, SETVAL, 1) == -1) {
		perror("semctl SETVAL failed");
		exit(1);
	}


	// main program
	printf("fork started\n");

	pid_t pid = fork();

	if (pid < 0) {
		perror("fork failed\n");
		exit(1);
	}
	if (pid == 0) {
		execl("./child_101256669_101298080", "child_101256669_101298080", (char *)0);
		perror("exec failed\n");
	}
	else {
		// parent loop
		while (shm->counter <= 500) {
			sem_p(semid);
			if (shm->counter % shm->multiple == 0) {
				printf("[parent pid = %d] cycle = %d - %d (multiple of %d)\n", getpid(), shm->counter, shm->counter, shm->multiple);
			}
			else {
				printf("[parent pid = %d] cycle = %d - %d\n", getpid(), shm->counter, shm->counter);
			}
			shm->counter++;
			sem_v(semid);
			usleep(shm->delay);
		}
	}

	wait(NULL);
	shmdt(shm);
	shmctl(shmid, IPC_RMID, NULL);
	semctl(semid, 0, IPC_RMID);
	exit(0);
}
