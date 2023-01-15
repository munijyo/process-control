//Client header file
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

//defining various macros 
#define PORT 4444
#define SEND_BUFFER_SIZE 512
#define RECV_BUFFER_SIZE 512
#define MAX_DATA_LEN  128

#define MSG_SUCCESS  1
#define MSG_FAIL     2
#define MSG_INVALID  3
#define MAX_INPUT_LEN   32

//defining various enums 
typedef enum 
{
  MSG_ADD_PROCESS,
  MSG_DELETE_PROCESS,
  MSG_START_PROCESS,
  MSG_PAUSE_PROCESS,
  MSG_UNPAUSE_PROCESS,
  MSG_KILL_PROCESS,
  MSG_DISPLAY_PROCESS,
  MSG_MAX_PROCESS_LIMIT,
  MSG_PROCESS_ALREADY_ADDED,
  MSG_NO_PROCESS,
  //MSG_SUCCESS,
  //MSG_FAIL,
  MSG_MAX

} msg_type_e;

#define DATA_SIZE 1000

//Structure for reading and writing client details
typedef struct 
{
  msg_type_e  msg_type; 
  char        msg_data[MAX_DATA_LEN];
} msg_t;

//Structure for message actions
typedef struct 
{
  void        (*msg_action_function) (int, msg_t *);

} msg_action_t;
