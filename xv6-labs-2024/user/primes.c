#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int fd) __attribute__((noreturn)); // Thông báo cho trình biên dịch: hàm sẽ không đệ quy vô hạn

void primes(int left) {
    int right[2];
    pipe(right);

    int prime;
    if (read(left, &prime, sizeof(prime)) == 0) {
        close(left);
        close(right[0]);
        close(right[1]);
        exit(0);
    }

    printf("prime %d\n", prime);

    int pid = fork();

    if (pid > 0) { // tiến trình cha
        close(right[0]);

        int num;
        while (read(left, &num, sizeof(num)) == sizeof(num)) {
            if (num % prime != 0) {
                write(right[1], &num, sizeof(num));
            }
        }
        close(left);
        close(right[1]);
        wait(0); // chờ tiến trình con kết thúc
    } else if (pid == 0) { // tiến trình con
        close(right[1]);
        close(left);
        primes(right[0]);
    }
    exit(0);
}

int main() {
    int p[2];
    pipe(p);

    int pid = fork();

    if (pid > 0) { // tiến trình cha
        close(p[0]);
        for (int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0);
    } else if (pid == 0) { // tiến trình con
        close(p[1]);
        primes(p[0]);
    } else { // Fail
        printf("Fork failed!\n");
        exit(1); // Thoát chương trình với lỗi
    }
    exit(0);
}