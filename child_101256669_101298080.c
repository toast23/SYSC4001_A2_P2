#include "shared_101256669_101298080.h"

int main() {
	// shm set up
	key_t key_shm = 1234;
	int shmid = shmget(key_shm, sizeof(struct shm_var), 0666);

	if (shmid < 0) {
		perror("shmget failed");
		exit(1);
	}

	struct shm_var *shm = (struct shm_var *)shmat(shmid, NULL, 0);

	if (shm == (void*)-1) {
		perror("shmat failed");
		exit(1);
	}

	// sem set up
	key_t key_sem = 5678;
	int semid = semget(key_sem, 1, 0666);

	if (semid < 0) {
		perror("semget failed");
		exit(1);
	}

	// child loop
	while (shm->counter <= 100) {}

	while (shm->counter <= 500) {
		sem_p(semid);
		if (shm->counter % shm->multiple == 0) {
			printf("[child pid = %d] cycle number: %d - %d (multiple of %d)\n", getpid(), shm->counter, shm->counter, shm->multiple);
		}
		else {
			printf("[child pid = %d] cycle number: %d - %d\n", getpid(), shm->counter, shm->counter);
		}
		shm->counter++;
		sem_v(semid);
		usleep(shm->delay);
	}

	shmdt(shm);
	exit(0);
}
