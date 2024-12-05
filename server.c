#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/epoll.h>
#include <stdbool.h>
#include <fcntl.h>

#define PORT "12345" // the port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold
#define MAX_BUFFER_SIZE 1024
#define NUMBER_OF_CONNECTIONS 10
#define MAX_EVENTS 20 // Maximum number of events to process at once in epoll

void sigchld_handler(int s)
{
  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
}

void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
  {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

typedef struct
{
  int Client1;
  int Client2;
  bool completed;
  char client1_ip[INET6_ADDRSTRLEN];
  char client2_ip[INET6_ADDRSTRLEN];
} Connection;

void init_connection(Connection *conn)
{
  conn->Client1 = -1;
  conn->Client2 = -1;
  conn->completed = false;
  memset(conn->client1_ip, 0, INET6_ADDRSTRLEN);
  memset(conn->client2_ip, 0, INET6_ADDRSTRLEN);
}

void send_connection_message(Connection *conn)
{
  char message1[MAX_BUFFER_SIZE] = "Black";
  char message2[MAX_BUFFER_SIZE] = "White";
  int randomnumber = rand() % 2;

  if (randomnumber == 1)
  {
    send(conn->Client1, message1, strlen(message1), 0);
    send(conn->Client2, message2, strlen(message2), 0);
  }
  else
  {
    send(conn->Client1, message2, strlen(message2), 0);
    send(conn->Client2, message1, strlen(message1), 0);
  }
}

void handle_client_communication(Connection *conn, int epoll_fd)
{
  char buffer[MAX_BUFFER_SIZE];
  int bytes_read;

  // Check Client1
  if (conn->Client1 != -1)
  {
    bytes_read = recv(conn->Client1, buffer, sizeof(buffer), 0);
    if (bytes_read > 0)
    {
      send(conn->Client2, buffer, bytes_read, 0);
    }
    else if (bytes_read <= 0)
    {
      close(conn->Client1);
      epoll_ctl(epoll_fd, EPOLL_CTL_DEL, conn->Client1, NULL);
      conn->Client1 = -1;
      send(conn->Client2, "Client1 has disconnected.\n", 27, 0);
    }
  }

  // Check Client2
  if (conn->Client2 != -1)
  {
    bytes_read = recv(conn->Client2, buffer, sizeof(buffer), 0);
    if (bytes_read > 0)
    {
      send(conn->Client1, buffer, bytes_read, 0);
    }
    else if (bytes_read <= 0)
    {
      close(conn->Client2);
      epoll_ctl(epoll_fd, EPOLL_CTL_DEL, conn->Client2, NULL);
      conn->Client2 = -1;
      send(conn->Client1, "Client2 has disconnected.\n", 27, 0);
    }
  }
}

int main(void)
{
  srand(time(NULL));
  struct sigaction sa;
  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sigaction(SIGCHLD, &sa, NULL);

  int sockfd, new_fd;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  int yes = 1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  Connection connectionSock[NUMBER_OF_CONNECTIONS];
  for (int i = 0; i < NUMBER_OF_CONNECTIONS; i++)
  {
    init_connection(&connectionSock[i]);
  }

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  for (p = servinfo; p != NULL; p = p->ai_next)
  {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
    {
      perror("server: socket");
      continue;
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
    {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo);

  if (listen(sockfd, BACKLOG) == -1)
  {
    perror("listen");
    exit(1);
  }

  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1)
  {
    perror("epoll_create1");
    exit(1);
  }

  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = sockfd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event) == -1)
  {
    perror("epoll_ctl: sockfd");
    exit(1);
  }

  printf("server: waiting for connections...\n");

  struct epoll_event events[MAX_EVENTS];

  while (1)
  {
    int num_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (num_fds == -1)
    {
      perror("epoll_wait");
      exit(1);
    }

    for (int i = 0; i < num_fds; i++)
    {
      if (events[i].data.fd == sockfd)
      {
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);

        int flag = 1;
        setsockopt(new_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
        fcntl(new_fd, F_SETFL, fcntl(new_fd, F_GETFL, 0) | O_NONBLOCK);

        event.events = EPOLLIN | EPOLLET;
        event.data.fd = new_fd;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_fd, &event);

        for (int j = 0; j < NUMBER_OF_CONNECTIONS; j++)
        {
          if (!connectionSock[j].completed)
          {
            if (connectionSock[j].Client1 == -1)
            {
              connectionSock[j].Client1 = new_fd;
              strncpy(connectionSock[j].client1_ip, s, INET6_ADDRSTRLEN);
            }
            else if (connectionSock[j].Client2 == -1)
            {
              connectionSock[j].Client2 = new_fd;
              strncpy(connectionSock[j].client2_ip, s, INET6_ADDRSTRLEN);
              connectionSock[j].completed = true;
              send_connection_message(&connectionSock[j]);
            }
            break;
          }
        }
      }
      else
      {
        for (int j = 0; j < NUMBER_OF_CONNECTIONS; j++)
        {
          if (connectionSock[j].completed &&
              (events[i].data.fd == connectionSock[j].Client1 ||
               events[i].data.fd == connectionSock[j].Client2))
          {
            handle_client_communication(&connectionSock[j], epoll_fd);
          }
        }
      }
    }
  }

  close(sockfd);
  return 0;
}
