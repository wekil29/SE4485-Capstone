/*
 * This header file should allows us to enable seccomp filtering in both our server and our client.
 * This code is based on the following example: https://github.com/nolandda/seccomp-example
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>
#include <linux/audit.h>
#include <linux/bpf.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <linux/unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "seccomp-bpf.h"



static int install_filter(int nr, int arch, int error) {
  struct sock_filter filter[] = {
      BPF_STMT(BPF_LD + BPF_W + BPF_ABS, (offsetof(struct seccomp_data, arch))),
      BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, arch, 0, 3),
      BPF_STMT(BPF_LD + BPF_W + BPF_ABS, (offsetof(struct seccomp_data, nr))),
      BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, nr, 0, 1),
      BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_ERRNO | (error & SECCOMP_RET_DATA)),
      BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_ALLOW),
  };
  struct sock_fprog prog = {
      .len = (unsigned short)(sizeof(filter) / sizeof(filter[0])),
      .filter = filter,
  };
  if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
    perror("prctl(NO_NEW_PRIVS)");
    return 1;
  }
  if (prctl(PR_SET_SECCOMP, 2, &prog)) {
    perror("prctl(PR_SET_SECCOMP)");
    return 1;
  }
  return 0;
}






// void handle_sigsys(int sig)
// {
//     printf("XXX -- DEATH BY SIGSYS -- XXX\n");
//     exit(55);
// }


// static int install_syscall_filter(int allowpid)
// {
//     static int no_new_privs_set = 0;
//     struct sock_filter filter_yespid[] = {
//         /* Validate architecture. */
//         VALIDATE_ARCHITECTURE,
//         /* Grab the system call number. */
//         EXAMINE_SYSCALL,
//         /* List allowed syscalls. */
//         ALLOW_SYSCALL(rt_sigreturn),
// #ifdef __NR_sigreturn
//         ALLOW_SYSCALL(sigreturn),
// #endif
//         ALLOW_SYSCALL(exit_group),
//         ALLOW_SYSCALL(exit),
//         ALLOW_SYSCALL(read),
//         ALLOW_SYSCALL(write),
//         ALLOW_SYSCALL(fstat),
//         ALLOW_SYSCALL(set_robust_list),
//         ALLOW_SYSCALL(newfstatat),
//         ALLOW_SYSCALL(mmap),
//         ALLOW_SYSCALL(prctl),
//         ALLOW_SYSCALL(rt_sigprocmask),
//         ALLOW_SYSCALL(rt_sigaction),
//         ALLOW_SYSCALL(clock_nanosleep),
//         ALLOW_SYSCALL(nanosleep),
//         ALLOW_SYSCALL(clone),
//         ALLOW_SYSCALL(getrandom),
//         ALLOW_SYSCALL(fstat64),
//         ALLOW_SYSCALL(brk),
//         ALLOW_SYSCALL(wait4),
//         ALLOW_SYSCALL(getpid),
//         KILL_PROCESS,
//     };

//     struct sock_filter filter_nopid[] = {
//         /* Validate architecture. */
//         VALIDATE_ARCHITECTURE,
//         /* Grab the system call number. */
//         EXAMINE_SYSCALL,
//         /* List allowed syscalls. */
//         ALLOW_SYSCALL(rt_sigreturn),
// #ifdef __NR_sigreturn
//         ALLOW_SYSCALL(sigreturn),
// #endif
//         ALLOW_SYSCALL(exit_group),
//         ALLOW_SYSCALL(exit),
//         ALLOW_SYSCALL(read),
//         ALLOW_SYSCALL(write),
//         ALLOW_SYSCALL(fstat),
//         ALLOW_SYSCALL(set_robust_list),
//         ALLOW_SYSCALL(newfstatat),
//         ALLOW_SYSCALL(mmap),
//         ALLOW_SYSCALL(rt_sigprocmask),
//         ALLOW_SYSCALL(rt_sigaction),
//         ALLOW_SYSCALL(clock_nanosleep),
//         ALLOW_SYSCALL(nanosleep),
//         ALLOW_SYSCALL(clone),
//         ALLOW_SYSCALL(getrandom),
//         ALLOW_SYSCALL(fstat64),
//         ALLOW_SYSCALL(brk),
//         ALLOW_SYSCALL(wait4),
//         KILL_PROCESS,
//     };

//     struct sock_fprog prog_yespid = {
//         .len = (unsigned short)(sizeof(filter_yespid)/sizeof(filter_yespid[0])),
//         .filter = filter_yespid,
//     };
//     struct sock_fprog prog_nopid = {
//         .len = (unsigned short)(sizeof(filter_nopid)/sizeof(filter_nopid[0])),
//         .filter = filter_nopid,
//     };

//     const struct sock_fprog* progptr = (allowpid)?(&prog_yespid):(&prog_nopid);

//     if(!no_new_privs_set) {
//         if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
//             perror("prctl(NO_NEW_PRIVS)");
//             goto failed;
//         }
//         no_new_privs_set = 1;
//     }
//     if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, progptr)) {
//         perror("prctl(SECCOMP)");
//         goto failed;
//     }
//     return 0;

//  failed:
//     if (errno == EINVAL)
//         fprintf(stderr, "SECCOMP_FILTER is not available. :(\n");
//     return 1;
// }

// void parse_args(int argc, char** argv, int* before_allow, int* after_allow) {

//     if (argc != 2) {
//         printf("usage: doseccomp [restrict|unchanged|relax]\n");
//         exit(2);
//     }

//     if(strncmp(argv[1], "restrict", strlen("restrict"))==0) {
//         *before_allow = 1;
//         *after_allow = 0;
//     }
//     else if(strncmp(argv[1], "unchanged", strlen("unchanged"))==0) {
//         *before_allow = 1;
//         *after_allow = 1;
//     }
//     else if(strncmp(argv[1], "relax", strlen("relax"))==0) {
//         *before_allow = 0;
//         *after_allow = 1;
//     }
//     else {
//         printf("Bad arg \"%s\"\n", argv[1]);
//         exit(3);
//     }
//     return;
// }
