/*******************************************************************************************
File : server.c

description : server side of process controller adding, deleting and controlling the process.

*******************************************************************************************
						HEADER FILES
*******************************************************************************************/
#include "common.h"
#include "server.h"
int cl_count = 0;


/**** msg_action structure *******/
msg_action_t  msg_actions[MSG_MAX];
int process_count = 0;
process_info_t  *process_list_start = NULL;

/*****Function to delete the process from the list*******/
void delete_process_from_list(char *pname)
{
  process_info_t *temp = process_list_start, *prev;
  if (temp == NULL)
  {
    printf("Empty list \n");
    return;
  }
  if (temp != NULL && (strcmp(temp->process_name, pname) == 0))
  {
    process_list_start = temp->next; // Changed head
    free(temp); // free old head
    process_count--;
    return;
  }
  while (temp != NULL && (strcmp(temp->process_name, pname) != 0))
  {
    prev = temp;
    temp = temp->next;
  }
  if (temp == NULL)
    return;
  prev->next = temp->next;
  free(temp); // Free memory
  process_count--;
  return;
}

/*Function to unpause a process
Parameters: process name as character string and returns a integer value */
int unpause_process(char *pname)
{
  int cpid;
  process_info_t *temp = process_list_start;
  while(temp != NULL)
  {
    if (strcmp(temp->process_name, pname) == 0)
    {
      if (temp->state == STATE_RUNNING)
      {
        printf("Process %s already running %s \n", pname);
        return 0;
      }
      else if (temp->state == STATE_PAUSED)
      {
        printf("Un Pausing process %s with pid %d \n", pname, temp->process_id);
        kill(temp->process_id, SIGCONT);
        temp->state = STATE_RUNNING;
        return 0;
      }
      else 
      {
        printf("Could not pause process %s with pid %d, not in running state\n", 
            pname, temp->process_id);
        return -1;
      }
    }
    temp = temp->next;
  }
  printf("end of pause \n");
  return -1;
}

/*Function to kill a process
Parameters: process name as character string and returns a integer value */
int kill_process(char *pname)
{
  int cpid;
  process_info_t *temp = process_list_start;
  while(temp != NULL)
  {
    if (strcmp(temp->process_name, pname) == 0)
    {
      if (temp->state == STATE_ADDED)
      {
        printf("Process %s not started, unable to kill \n", pname);
        return -1;
      }
      else if (temp->state == STATE_RUNNING)
      {
        printf("Killing process %s with pid %d \n", pname, temp->process_id);
        kill(temp->process_id, SIGTERM);
        temp->state = STATE_ADDED;
        return 0;
      }
    }
    temp = temp->next;
  }
  return -1;
  printf("end of pause \n");
}

/*Function to pause a process
Parameters: process name as character string and returns a integer value */
int pause_process(char *pname)
{
  int cpid;
  process_info_t *temp = process_list_start;
  while(temp != NULL)
  {
    if (strcmp(temp->process_name, pname) == 0)
    {
      if (temp->state == STATE_PAUSED)
      {
        printf("Process %s already paused %s \n", pname);
        return 0;
      }
      else if (temp->state == STATE_RUNNING)
      {
        printf("Pausing process %s with pid %d \n", pname, temp->process_id);
        kill(temp->process_id, SIGSTOP);
        temp->state = STATE_PAUSED;
        return 0;
      }
      else 
      {
        printf("Could not pause process %s with pid %d, not in running state\n", 
            pname, temp->process_id);
        return -1;
      }
    }
    temp = temp->next;
  }
  return -1;
  printf("end of pause \n");
}

/*Function to start a process
Parameters: process name as character string and returns a integer value */
int start_process(char *pname)
{
  int cpid;
  process_info_t *temp = process_list_start;
  while(temp != NULL)
  {
    if (strcmp(temp->process_name, pname) == 0)
    {
      cpid = fork();
      if (cpid == 0)
      {
        int ret;
        /* child */
        ret = execve(pname, NULL, NULL);
        if (ret == -1)
        {
          printf("could not start process %s \n", pname);
          exit(0x1);
        }
      }
      else
      {
        /* parent */
        printf("Started process %s with pid %d \n", pname, cpid);
        temp->process_id = cpid;
        temp->state = STATE_RUNNING;
      }
    }
    temp = temp->next;
  }
  printf("end of start \n");
}

/*Function to display the statistical data*/
int display_data()
{
	int r=system("top");
}

/*Function to add a process to the list
Parameters: process name as character string and returns the process list which is added*/
process_info_t* add_process_to_list(char *pname)
{
  process_info_t *new_process = malloc(sizeof(process_info_t));
  process_info_t *temp = process_list_start;

  if (new_process == NULL)
  {
    printf("Failed to alloc memory \n");
    return NULL;
  }
  strncpy(new_process->process_name, pname, strlen(pname));
  new_process->state = STATE_ADDED;
  new_process->process_id = -1;
  new_process->next = NULL;

  if (process_list_start == NULL)
  {
    process_list_start = new_process;
    printf("Added first process \n");
    return new_process;
  }
  
  while(temp->next != NULL)
  	temp = temp->next;

  temp->next = new_process;
  printf("Added new process \n");

  return process_list_start;
}

/*Function to search a process in the list
Parameters: process name as character string and returns a integer value */
int search_process_in_list(char *pname)
{
  process_info_t *temp = process_list_start;

  if (temp == NULL)
  {
    printf("List is empty \n");
    return PROCESS_NOT_FOUND;
  }

  while(temp != NULL)
  {
    if (strcmp(temp->process_name, pname) == 0)
    {
      printf("Process %s found \n", pname);
        return PROCESS_FOUND;
    }

    temp = temp->next;
  }
  printf("Process %s not found \n", pname);
  return PROCESS_NOT_FOUND;
}

/*Function to handle the add process message*/
void handle_add_process(int sockfd, msg_t *msg)
{
  msg_t reply_msg;
  int ret;

  printf("Handling message: %d \n", msg->msg_type);
  printf("Process Name : %s \n", msg->msg_data);

  if (process_count == 4)
  {
    printf("Max process count %d reached \n", process_count);
    reply_msg.msg_type = MSG_MAX_PROCESS_LIMIT;
  }

  ret = search_process_in_list(msg->msg_data);

  if (ret == PROCESS_FOUND)
  {
    printf("Process %s already added in PM \n", msg->msg_data);
    reply_msg.msg_type = MSG_PROCESS_ALREADY_ADDED;
  }
  else
  {
    printf("Adding Process %s to PM\n", msg->msg_data);
    add_process_to_list(msg->msg_data);
    process_count++;
    reply_msg.msg_type = MSG_SUCCESS;
  }
   ret = send(sockfd, (char *) &reply_msg, sizeof(msg), 0);
   printf("Sent %d bytes reply .. \n", ret);

}

/*Function to handle the delete process message*/
void handle_delete_process(int sockfd, msg_t *msg)
{
  msg_t reply_msg;
  int ret;

  printf("Handling message: %d \n", msg->msg_type);
  printf("Process Name : %s \n", msg->msg_data);

  if (process_count == 0)
  {
    printf("No Process added to PM \n");
    reply_msg.msg_type = MSG_NO_PROCESS;
  }

  ret = search_process_in_list(msg->msg_data);
  if (ret == PROCESS_NOT_FOUND)
  {
    printf("Process %s not present in PM \n", msg->msg_data);
    reply_msg.msg_type = MSG_SUCCESS;
  }
  else
  {
    printf("Deleting Process %s to PM\n", msg->msg_data);
    delete_process_from_list(msg->msg_data);
    reply_msg.msg_type = MSG_SUCCESS;
  }
   ret = send(sockfd, (char *) &reply_msg, sizeof(msg), 0);
   printf("Sent %d bytes reply .. \n", ret);
}

/*Function to handle the start process message*/
void handle_start_process(int sockfd, msg_t *msg)
{
  msg_t reply_msg;
  int ret;

  printf("Handling message: %d \n", msg->msg_type);
  printf("Process Name : %s \n", msg->msg_data);

  if (process_count == 0)
  {
    printf("No Process added to PM \n");
    reply_msg.msg_type = MSG_NO_PROCESS;
    goto send_reply;
  }

  ret = search_process_in_list(msg->msg_data);
  if (ret == PROCESS_NOT_FOUND)
  {
    printf("Process %s not present in PM \n", msg->msg_data);
    reply_msg.msg_type = MSG_SUCCESS;
  }
  else
  {
    printf("Starting Process %s in PM\n", msg->msg_data);
    start_process(msg->msg_data);
    reply_msg.msg_type = MSG_SUCCESS;
  }

send_reply:
   ret = send(sockfd, (char *) &reply_msg, sizeof(msg), 0);
   printf("Sent %d bytes reply .. \n", ret);
}

/*Function to handle the unpause process message*/
void handle_unpause_process(int sockfd, msg_t *msg)
{
  msg_t reply_msg;
  int ret;

  printf("Handling message: %d \n", msg->msg_type);
  printf("Process Name : %s \n", msg->msg_data);

  if (process_count == 0)
  {
    printf("No Process added to PM \n");
    reply_msg.msg_type = MSG_NO_PROCESS;
    goto send_reply;
  }

  ret = search_process_in_list(msg->msg_data);

  if (ret == PROCESS_NOT_FOUND)
  {
    printf("Process %s not present in PM \n", msg->msg_data);
    reply_msg.msg_type = MSG_SUCCESS;
  }
  else
  {
    printf("Un pausing Process %s in PM\n", msg->msg_data);
    ret = unpause_process(msg->msg_data);
    if (ret == 0)
      reply_msg.msg_type = MSG_SUCCESS;
    else
      reply_msg.msg_type = MSG_FAIL;
  }

send_reply:
   ret = send(sockfd, (char *) &reply_msg, sizeof(msg), 0);

   printf("Sent %d bytes reply .. \n", ret);
}

/*Function to handle the pause process message*/
void handle_pause_process(int sockfd, msg_t *msg)
{
  msg_t reply_msg;
  int ret;

  printf("Handling message: %d \n", msg->msg_type);
  printf("Process Name : %s \n", msg->msg_data);

  if (process_count == 0)
  {
    printf("No Process added to PM \n");
    reply_msg.msg_type = MSG_NO_PROCESS;
    goto send_reply;
  }

  ret = search_process_in_list(msg->msg_data);

  if (ret == PROCESS_NOT_FOUND)
  {
    printf("Process %s not present in PM \n", msg->msg_data);
    reply_msg.msg_type = MSG_SUCCESS;
  }
  else
  {
    printf("Pausing Process %s in PM\n", msg->msg_data);
    ret = pause_process(msg->msg_data);
    if (ret == 0)
      reply_msg.msg_type = MSG_SUCCESS;
    else
      reply_msg.msg_type = MSG_FAIL;
  }

send_reply:
   ret = send(sockfd, (char *) &reply_msg, sizeof(msg), 0);
   printf("Sent %d bytes reply .. \n", ret);
}

/*Function to handle the kill process message*/
void handle_kill_process(int sockfd, msg_t *msg)
{
  	msg_t reply_msg;
  	int ret;

  	printf("Handling message: %d \n", msg->msg_type);
  	printf("Process Name : %s \n", msg->msg_data);

  	if (process_count == 0)
  	{
    	printf("No Process added to PM \n");
    	reply_msg.msg_type = MSG_NO_PROCESS;
    	goto send_reply;
  	}

  	ret = search_process_in_list(msg->msg_data);

  	if (ret == PROCESS_NOT_FOUND)
  	{
    	printf("Process %s not present in PM \n", msg->msg_data);
    	reply_msg.msg_type = MSG_SUCCESS;
  	}
  	else
 	 {
    	printf("Killing Process %s in PM\n", msg->msg_data);
    	ret = kill_process(msg->msg_data);
    	if (ret == 0)
      	reply_msg.msg_type = MSG_SUCCESS;
    	else
      	reply_msg.msg_type = MSG_FAIL;
  }

	send_reply:
   	ret = send(sockfd, (char *) &reply_msg, sizeof(msg), 0);
   	printf("Sent %d bytes reply .. \n", ret);
}

/*Function to handle the display process message*/
void handle_display_process(int sockfd, msg_t *msg)
{
	display_data();
}

/*Function to setup the message handlers*/
void setup_message_handlers()
{
  	msg_actions[MSG_ADD_PROCESS].msg_action_function     = handle_add_process;
  	msg_actions[MSG_DELETE_PROCESS].msg_action_function  = handle_delete_process;
  	msg_actions[MSG_START_PROCESS].msg_action_function   = handle_start_process;
  	msg_actions[MSG_PAUSE_PROCESS].msg_action_function   = handle_pause_process;
  	msg_actions[MSG_UNPAUSE_PROCESS].msg_action_function = handle_unpause_process;
 	msg_actions[MSG_KILL_PROCESS].msg_action_function    = handle_kill_process;
  	msg_actions[MSG_DISPLAY_PROCESS].msg_action_function = handle_display_process;
}

/*Function to handle the client connection*/
void handle_client_close(int sock_fd)
{

  	printf("Client on socket %d closed \n", sock_fd);
  	close(sock_fd);

}

/*Function to process the client message*/
void process_client_messages(int sockfd, char *recv_buffer)
{
  	msg_t *m = (msg_t *) recv_buffer;
  	switch (m->msg_type)
  	{
    		case  MSG_ADD_PROCESS:
    		case  MSG_DELETE_PROCESS:
    		case  MSG_START_PROCESS:
    		case  MSG_PAUSE_PROCESS:
    		case  MSG_UNPAUSE_PROCESS:
    		case  MSG_KILL_PROCESS:
    		case  MSG_DISPLAY_PROCESS:
      					printf("Received msg type %d socket = %d ... \n", m->msg_type, sockfd);
      					msg_actions[m->msg_type].msg_action_function(sockfd, m);
      					break;
    		default:
      			printf("Received invalid msg type ... \n");
      			break;
  	}
  	return;
}

/* Thread to handle clients */
void* client_handler(void *client_sock_fd)
{
  	int *cl_sock_fd = (int *) client_sock_fd;
  	int ret = 0;
  	char send_buffer[SEND_BUFFER_SIZE];
  	char recv_buffer[RECV_BUFFER_SIZE];
  	printf("%s():%d Client Fd = %d\n",__FUNCTION__, __LINE__, *cl_sock_fd);
  	while(1)
  	{
    		printf("%s():%d Waiting on recv for fd = %d \n",__FUNCTION__, __LINE__, *cl_sock_fd);
    		ret = recv(*cl_sock_fd, recv_buffer, sizeof(recv_buffer), 0);
    		if (ret == 0)
    		{
      		printf("%s():%d Client has closed on socket fd = %d \n",__FUNCTION__, __LINE__, *cl_sock_fd);
      		handle_client_close(*cl_sock_fd);
      		cl_count--;
      		pthread_exit(NULL);
    		}
    		process_client_messages(*cl_sock_fd, recv_buffer);
  	}
  	pthread_exit(NULL);
}


/* Main Function*/
int main()
{
  	int server_fd, ret;
  	struct sockaddr_in server_addr;
  	int cl_sock_fd[MAX_CLIENTS];  
  	setup_message_handlers();
  	pthread_t cl_threads[MAX_CLIENTS];
  	server_fd = socket(AF_INET, SOCK_STREAM, 0);
  	if (server_fd < 0) 
  	{
    		printf("Error in connection.\n");
    		exit(1);
  	}
  	printf("TCP Server Socket is created.\n");
  	memset(&server_addr, '\0',  sizeof(server_addr));
  	server_addr.sin_family = AF_INET;
  	server_addr.sin_port = htons(PORT);
  	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  	ret = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  	if (ret < 0) 
  	{
   		 printf("Error in binding.\n");
    		exit(1);
  	}
  	if (listen(server_fd, 10) == 0) 
  	{
    		printf("Listening...\n\n");
  	}
  	while (1) 
  	{
    		ret = accept(server_fd, NULL, NULL);
    		if (ret < 0) 
    		{
      		perror("accept failed: ");
      		exit(1);
    		}
    		cl_sock_fd[cl_count] = ret;
    		printf("cl_count = %d fd = %d clfd = %d \n",cl_count, ret, cl_sock_fd[cl_count]);
    		ret = pthread_create(&cl_threads[cl_count], NULL, client_handler,&cl_sock_fd[cl_count]);
    		if (ret == 0)
    		{
     		 	printf("A new thread is created for client on fd: %d \n",cl_sock_fd[cl_count]);
      		cl_count++;
      		printf("Total clients connected : %d\n\n", cl_count);
    		}  
    		if (cl_count == MAX_CLIENTS)
    		{
      		printf("Max clients %d are connected..No more connections will be accepted\n",cl_count);
      		break;
    		}
  	}
  	for (int i = 0; i < cl_count; i++)
  	{  
   	 	pthread_join(cl_threads[i], NULL);
  	}
  	return 0;
}

