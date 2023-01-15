//Server header files
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>


//defining  various macros
#define MAX_CLIENTS 1
#define MAX_USERS   5
#define PORT 4444
#define SEND_BUFFER_SIZE 512
#define RECV_BUFFER_SIZE 512
#define PROCESS_FOUND  1
#define PROCESS_NOT_FOUND 2
#define PROCESS_PASSWD_MATCH_FOUND 3
#define PROCESS_PASSWD_MATCH_NOT_FOUND 4

//defining various enums 
typedef enum process_state
{
  STATE_RUNNING,
  STATE_PAUSED,
  STATE_ADDED,
  STATE_MAX

} process_state_e;

//Structure for reading and writing process details
typedef struct process_info
{
  char process_name[32];
  process_state_e state;
  int  process_id;
  struct process_info *next;

} process_info_t;
