/***********************************************************************
 * Need to create tree of 6 processes with the form:
 *        p
 *       /  \
 *     p      p
 *   /      /   \
 * p       p      p
 * 
 * And need to output their ppids and pids to view hierarchy.
 **********************************************************************/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int left_child_level_01(void) {
    pid_t cpid, pid, ppid, w;
    cpid = fork();
    if(-1 == cpid) {
        perror("Fork fail\n");
        exit(EXIT_FAILURE);
    }
    switch(cpid) {
        /*Left left child 02*/
        case 0:
            pid =  getpid();
            ppid = getppid();
            printf("%d, %d ppid, pid of left left child level 02 process\n", ppid, pid);
            exit(EXIT_SUCCESS);
        break;
        /*Left child 01*/
        default:
            w = waitpid(cpid, NULL, 0);
            printf("    %d pid returned by waitpid 3\n", w);
        break;
    }
    return 0;
}

int right_child_level_01(void) {
    pid_t cpid_l, cpid_r, pid, ppid;
    
    cpid_l = fork();
    if(-1 == cpid_l) {
        perror("Fork fail\n");
        exit(EXIT_FAILURE);
    }
    switch(cpid_l) {
        /*Right left child 02*/
        case 0:
            pid =  getpid();
            ppid = getppid();
            printf("%d, %d ppid, pid of right left child level 02 process\n", ppid, pid);
            exit(EXIT_SUCCESS);
        break;
        /*Right child after right left child*/
        default:
            cpid_r = fork();
            if(-1 == cpid_r) {
                perror("Fork fail\n");
                exit(EXIT_FAILURE);
            }
            switch(cpid_r) {
                /*Right right child 02*/
                case 0:
                    pid =  getpid();
                    ppid = getppid();
                    printf("%d, %d ppid, pid of right right child level 02 process\n", ppid, pid);
                    exit(EXIT_SUCCESS);
                break;
            }
        break;
    }
    
    /* Wait for the end of the right left 02 and the right right 02 child */
    pid_t fin_right_left  = 0;
    pid_t fin_right_right = 0;
    pid_t ret;
    while(!(fin_right_left && fin_right_right)) {
        if(!fin_right_left) {
            /* using nonblocking option WNOHANG */
            ret = waitpid(cpid_l, NULL, WNOHANG);
            if(-1 == ret) {
                perror("waitpid right child 01\n");
                exit(EXIT_FAILURE);
            }
            if(ret) {
                fin_right_left = ret;
                printf("    %d pid returned by waitpid 4\n", fin_right_left);
            }
        }
        if(!fin_right_right) {
            /* using nonblocking option WNOHANG */
            ret = waitpid(cpid_r, NULL, WNOHANG);
            if(-1 == ret) {
                perror("waitpid right child 01\n");
                exit(EXIT_FAILURE);
            }
            if(ret) {
                fin_right_right = ret;
                printf("    %d pid returned by waitpid 5\n", fin_right_right);
            }
        }
    }
    return 0;
}

int main() {
    pid_t cpid_l, cpid_r, pid, ppid, w;
    
    cpid_l = fork();
    if(-1 == cpid_l) {
        perror("Fork fail\n");
        exit(EXIT_FAILURE);
    }    
    switch(cpid_l) {
        /*Left child 01*/
        case 0:
            left_child_level_01();
            pid =  getpid();
            ppid = getppid();
            printf("%d, %d ppid, pid of left child level 01 process\n", ppid, pid);
            exit(EXIT_SUCCESS);
        break;
        /*Parent after left child*/
        default:
            cpid_r = fork();
            if(-1 == cpid_r) {
                perror("Fork fail\n");
                exit(EXIT_FAILURE);
            }
            switch(cpid_r) {
                /*Right child 01*/
                case 0:
                    right_child_level_01();
                    pid =  getpid();
                    ppid = getppid();
                    printf("%d, %d ppid, pid of right child level 01 process\n", ppid, pid);
                    exit(EXIT_SUCCESS);
                break;
            }
            /*Parent after right child*/
            pid =  getpid();
            ppid = getppid();
            printf("%d, %d ppid, pid of parent level 00 process\n", ppid, pid);
        break;
    }
    /* Wait for the end of the left 01 and the right 01 child */
    pid_t fin_left  = 0;
    pid_t fin_right = 0;
    pid_t ret;
    while(!(fin_left && fin_right)) {
        if(!fin_left) {
            /* using nonblocking option WNOHANG */
            ret = waitpid(cpid_l, NULL, WNOHANG);
            if(-1 == ret) {
                perror("waitpid in main\n");
                exit(EXIT_FAILURE);
            }
            if(ret) {
                fin_left = ret;
                printf("    %d pid returned by waitpid 1\n", fin_left);
            }
        }
        if(!fin_right) {
            /* using nonblocking option WNOHANG */
            ret = waitpid(cpid_r, NULL, WNOHANG);
            if(-1 == ret) {
                perror("waitpid in main\n");
                exit(EXIT_FAILURE);
            }
            if(ret) {
                fin_right = ret;
                printf("    %d pid returned by waitpid 2\n", fin_right);
            }
        }
    }
    return 0;
}

