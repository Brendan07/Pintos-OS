#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

char status;

static void syscall_handler (struct intr_frame *);
void sysHalt(void);
void sysExit(void);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}
//Handles System calls
static void
syscall_handler (struct intr_frame *f UNUSED)
{
  uint32_t *p=f->esp; //gets the system call number which is define in 'Pintos/lib/syscall-nr.h'

  switch(*p){

    //Halt system call is working, can be tested in my.c under by calling halt();
    case SYS_HALT:{
      sysHalt();
    }
    break;

//Write system call is working, can be tested in my.c by printf anything.
    case SYS_WRITE:{
////Set file descriptor, buffer, and size from arguments
      int fd =*(int *)(f->esp+4);
      void *buffer = *(char**)(f->esp+8);
      unsigned size = *(unsigned *)(f->esp+12);

      if(fd==STDOUT_FILENO){ ////Write to the console and return file length
        putbuf((const char*)buffer,(unsigned )size);
      }
      else{
        printf("SYS_Write does not support fd output\n");
      }
    }
    break;

//Exit system call is working can be tested in my.c when exit is a success
    case SYS_EXIT:{
      sysExit();
    }
    break;

//If the cases are not selected, fall through to this one.
  default:
  printf ("SYS_CALL (%d) has not been implemented\n",p);
  thread_exit ();
}
}

//Halt System Call - Code will output when testing
void
sysHalt(void){
  printf("Pintos System Call: HALT has been implemented \n");
  shutdown_power_off(); 
}


//Exit System Call - Code will output when testing
void
sysExit(void){
  struct thread * current = thread_current();
  current->status = status;
  printf("Pintos System Call: EXIT has been implemented\n");
  thread_exit();
}

