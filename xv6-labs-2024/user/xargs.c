#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[]) {
    int n = 1;
    int arg_start = 1;

    // Xử lý flag -n
    if (argc > 2 && strcmp(argv[1], "-n") == 0) {
        n = atoi(argv[2]);
        arg_start = 3;
    }

    char buf[512];
    char* args[MAXARG];
    int i, len = 0, count = 0;

    // Copy các tham số từ dòng lệnh (trước khi nhận từ stdin)
    for (i = 0; i < argc - arg_start; i++) {
        args[i] = argv[i + arg_start];
    }
    int base_args = i;

    // Đọc đầu vào dòng chuẩn
    while (read(0, &buf[len], 1) == 1) {
        if (buf[len] == '\n') {
            buf[len] = 0;

            // Cấp phát vùng nhớ mới cho từng tham số
            args[base_args + count] = sbrk(len + 1);
            strcpy(args[base_args + count], buf);
            count++;
            len = 0;

            // Nếu đã đủ n tham số thì thực thi lệnh
            if (count == n) {
                args[base_args + count] = 0; // NULL kết thúc danh sách
                if (fork() == 0) {
                    exec(args[0], args);
                    exit(1); // Nếu exec thất bại
                } else {
                    wait(0);
                }
                count = 0; // Reset lại đếm
            }
        } else {
            len++;
        }
    }

    // Xử lý các tham số còn lại chưa đủ n
    if (count > 0) {
        args[base_args + count] = 0;
        if (fork() == 0) {
            exec(args[0], args);
            exit(1);
        } else {
            wait(0);
        }
    }

    exit(0);
}