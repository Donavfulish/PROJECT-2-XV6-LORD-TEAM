#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    int n = 1;
    int arg_start = 1;
    if (argc > 2 && strcmp(argv[1], "-n") == 0) {
        n = atoi(argv[2]);
        arg_start = 3;
    }
    
    char buf[512];
    char *args[MAXARG];
    int i, len = 0, count = 0;
    
    for (i = 0; i < argc - arg_start; i++) {
        args[i] = argv[i + arg_start];
    }
    int base_args = i;
    
    while (read(0, buf + len, 1) == 1) {
        if (buf[len] == '\n') {
            buf[len] = 0; // Kết thúc chuỗi
            args[base_args + count] = buf; // Gán tham số
            count++;
            len = 0;
            
            // Nếu đủ số lượng tham số (-n), thực thi lệnh
            if (count == n) {
                args[base_args + count] = 0; // Đặt NULL cho exec
                if (fork() == 0) {
                    exec(args[0], args);
                    exit(1); // Thất bại nếu exec không chạy được
                } else {
                    wait(0);
                }
                count = 0; // Reset bộ đếm tham số
            }
        } else {
            len++;
        }
    }
    
    // Xử lý các tham số còn lại nếu chưa đạt đến n
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
