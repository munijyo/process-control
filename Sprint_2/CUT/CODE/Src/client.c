//Header File//
#include "common.h"

int cl_sock_fd;
char *process_details[4];

char send_buffer[SEND_BUFFER_SIZE];
char recv_buffer[RECV_BUFFER_SIZE];

/*Function to handle the servere connection*/
void handle_server_close (int sockfd)
{
  close(sockfd);
}

/*Function to add a process
Parameters: process name as character string and returns a integer value */
int add_process(char *uname)
{
  	int ret;
  	msg_t msg, *rx_msg; 
  	memset(msg.msg_data, '\0', sizeof(msg.msg_data));
  	printf("Enter the process name : \n");
  	scanf("%s",uname);
  	msg.msg_type = MSG_ADD_PROCESS;
  	strncpy(msg.msg_data, uname, strlen(uname));
  	ret = send(cl_sock_fd, (char *) &msg, sizeof(msg), 0);
  	ret = recv(cl_sock_fd, (char *) recv_buffer, sizeof(recv_buffer), 0);
  	printf("Received %d bytes from Server \n", ret);
   	if (ret == 0)
   	{
     		printf("%s():%d Server has closed on socket fd = %d \n",__FUNCTION__, __LINE__, cl_sock_fd);
   		handle_server_close(cl_sock_fd);
    		return -2;
   	}
  	rx_msg = (msg_t *) recv_buffer;
  	return rx_msg->msg_type;
}

/*Function to delete a process
Parameters: process name as character string and returns a integer value */
int delete_process(char *uname)
{
  	int ret;
  	msg_t msg, *rx_msg; 
  	memset(msg.msg_data, '\0', sizeof(msg.msg_data));
  	printf("Enter the process name : \n");
  	scanf("%s",uname);
  	msg.msg_type = MSG_DELETE_PROCESS;
  	strncpy(msg.msg_data, uname, strlen(uname));

  	ret = send(cl_sock_fd, (char *) &msg, sizeof(msg), 0);
  	ret = recv(cl_sock_fd, (char *) recv_buffer, sizeof(recv_buffer), 0);

  	printf("Received %d bytes from Server \n", ret);

   	if (ret == 0)
   	{
     		printf("%s():%d Server has closed on socket fd = %d \n",__FUNCTION__, __LINE__, cl_sock_fd);
    		handle_server_close(cl_sock_fd);
    		return -2;
   	}
  	rx_msg = (msg_t *) recv_buffer;
  	return rx_msg->msg_type;
}

/*Function to start a process
Parameters: process name as character string and returns a integer value */
int start_process(char *uname)
{
  	int ret;
  	msg_t msg, *rx_msg; 

  	memset(msg.msg_data, '\0', sizeof(msg.msg_data));

  	printf("Enter the process name : \n");
  	scanf("%s",uname);

  	msg.msg_type = MSG_START_PROCESS;
  	strncpy(msg.msg_data, uname, strlen(uname));
  	ret = send(cl_sock_fd, (char *) &msg, sizeof(msg), 0);
  	ret = recv(cl_sock_fd, (char *) recv_buffer, sizeof(recv_buffer), 0);
  	printf("Received %d bytes from Server \n", ret);
   	if (ret == 0)
   	{
     		printf("%s():%d Server has closed on socket fd = %d \n",__FUNCTION__, __LINE__, cl_sock_fd);
    		handle_server_close(cl_sock_fd);
    		return -2;
   	}
  	rx_msg = (msg_t *) recv_buffer;
  	return rx_msg->msg_type;
}

/*Function to kill a process
Parameters: process name as character string and returns a integer value */
int kill_process(char *uname)
{
  	int ret;
  	msg_t msg, *rx_msg; 

  	memset(msg.msg_data, '\0', sizeof(msg.msg_data));

  	printf("Enter the process name : \n");
  	scanf("%s",uname);

  	msg.msg_type = MSG_KILL_PROCESS;
  	strncpy(msg.msg_data, uname, strlen(uname));

  	ret = send(cl_sock_fd, (char *) &msg, sizeof(msg), 0);
  	ret = recv(cl_sock_fd, (char *) recv_buffer, sizeof(recv_buffer), 0);

  	printf("Received %d bytes from Server \n", ret);
   	if (ret == 0)
   	{
     		printf("%s():%d Server has closed on socket fd = %d \n",__FUNCTION__, __LINE__, cl_sock_fd);
    		handle_server_close(cl_sock_fd);
    		return -2;
   	}
  	rx_msg = (msg_t *) recv_buffer;
  	return rx_msg->msg_type;
}

/*Function to kill a process
Parameters: process name as character string and returns a integer value */
int pause_process(char *uname)
{
  	int ret;
  	msg_t msg, *rx_msg; 

  	memset(msg.msg_data, '\0', sizeof(msg.msg_data));

  	printf("Enter the process name : \n");
  	scanf("%s",uname);

  	msg.msg_type = MSG_PAUSE_PROCESS;
  	strncpy(msg.msg_data, uname, strlen(uname));

  	ret = send(cl_sock_fd, (char *) &msg, sizeof(msg), 0);
  	ret = recv(cl_sock_fd, (char *) recv_buffer, sizeof(recv_buffer), 0);
  	printf("Received %d bytes from Server \n", ret);
   	if (ret == 0)
   	{
     		printf("%s():%d Server has closed on socket fd = %d \n",__FUNCTION__, __LINE__, cl_sock_fd);
    		handle_server_close(cl_sock_fd);
    		return -2;
   	}
  	rx_msg = (msg_t *) recv_buffer;
 	return rx_msg->msg_type;
}

/*Function to unpause a process
Parameters: process name as character string and returns a integer value */
int unpause_process(char *uname)
{
  	int ret;
  	msg_t msg, *rx_msg; 

  	memset(msg.msg_data, '\0', sizeof(msg.msg_data));

  	printf("Enter the process name : \n");
  	scanf("%s",uname);

  	msg.msg_type = MSG_UNPAUSE_PROCESS;
  	strncpy(msg.msg_data, uname, strlen(uname));

  	ret = send(cl_sock_fd, (char *) &msg, sizeof(msg), 0);
  	ret = recv(cl_sock_fd, (char *) recv_buffer, sizeof(recv_buffer), 0);
 	printf("Received %d bytes from Server \n", ret);
   	if (ret == 0)
   	{
     		printf("%s():%d Server has closed on socket fd = %d \n",__FUNCTION__, __LINE__, cl_sock_fd);
    		handle_server_close(cl_sock_fd);
    		return -2;
   	}
  	rx_msg = (msg_t *) recv_buffer;
  	return rx_msg->msg_type;
}

/*Function to display a process
Parameters: process name as character string and returns a integer value */
int display_status(char *uname)
{
  	int ret;
  	msg_t msg, *rx_msg; 
	
  	memset(msg.msg_data, '\0', sizeof(msg.msg_data));

  	printf("Enter the process name : \n");
  	scanf("%s",uname);

  	msg.msg_type = MSG_DISPLAY_PROCESS;
  	strncpy(msg.msg_data, uname, strlen(uname));
  	ret = send(cl_sock_fd, (char *) &msg, sizeof(msg), 0);
  	ret = recv(cl_sock_fd, (char *) recv_buffer, sizeof(recv_buffer), 0);
  	printf("Received %d bytes from Server \n", ret);
   	if (ret == 0)
   	{
     		printf("%s():%d Server has closed on socket fd = %d \n",__FUNCTION__, __LINE__, cl_sock_fd);
    		handle_server_close(cl_sock_fd);
    		return -2;
   	}
  	rx_msg = (msg_t *) recv_buffer;
  	return rx_msg->msg_type;
}

/*Main function with menu options*/
int main()
{
  int ret;
  int option;

  struct sockaddr_in server_addr;
  char menu_data_string1[MAX_INPUT_LEN];
  cl_sock_fd = socket(AF_INET,SOCK_STREAM, 0);
  if (cl_sock_fd < 0) 
  {
    printf("Error in connection.\n");
    exit(1);
  }

  printf("Client Socket is created.\n");

  memset(send_buffer, '\0', sizeof(send_buffer));
  memset(recv_buffer, '\0', sizeof(recv_buffer));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);

  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  ret = connect(cl_sock_fd, (struct sockaddr*)&server_addr,sizeof(server_addr));

  if (ret < 0) 
  {
    printf("Error in connection.\n");
    exit(1);
  }
  printf("Connected to Server.\n");
  while (1) 
  {
    printf("=================================================================\n");
    printf("\t\t\tMenu Interface\t\t\n");
    printf("=================================================================\n");

    printf("\t\t\t1. Add a process\n");
    printf("\t\t\t2. Delete a process\n");
    printf("\t\t\t3. Start a process\n");
    printf("\t\t\t4. Kill a process\n");
    printf("\t\t\t5. Pasue a process \n");
    printf("\t\t\t6. Un Pasue a process \n");
    printf("\t\t\t7. Display Status\n");
    printf("\t\t\t8. Logout \n");

    scanf("%d", &option);

    memset(menu_data_string1, '\0', sizeof(menu_data_string1));

    switch(option)
    {
      case 1:

        ret = add_process(menu_data_string1);
        
        if (ret == MSG_SUCCESS)
          printf("Process added to PM \n");
        else if (ret == MSG_MAX_PROCESS_LIMIT)
          printf("Max limit of 4 reached, failed to add process to PM \n");
        else if (ret == MSG_PROCESS_ALREADY_ADDED)
          printf("Process already added to PM, could not add again \n");
        break;

      case 2:
        ret = delete_process(menu_data_string1);

        if (ret == MSG_SUCCESS)
          printf("Process deleted in PM \n");
        else if (ret == MSG_NO_PROCESS)
          printf("Process list empty in PM \n");
        break;

      case 3:
        start_process(menu_data_string1);
        break;
      
      case 4:
        kill_process(menu_data_string1);
        break;

      case 5:
        pause_process(menu_data_string1);
        break;
      
      case 6:
        unpause_process(menu_data_string1);
        break;

      case 7:
        display_status(menu_data_string1);
        break;
      
      case 8:
             exit(0);

      default:
             printf("Invalid Option .. \n");
             break;
    }

  }

  return 0;
}

