#ifndef	__SERVER_HH__
#define	__SERVER_HH__

#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <list>
#include "Client.hh"

#define MAXDATASIZE 100
#define BACKLOG 10     /* Nombre maxi de connections acceptées en file */

class	Server
{
public:
  Server(int port);
  ~Server();

  void	run();
  void	end();

  void		emmet(const std::string &);
  std::string	recept();

private:

  void	startProtocole();

  void	createSocket();
  void	bindSocket();
  void	listenSocket();

  std::list<Client *>	_listClient;
  int	_port;
  int sockfd, new_fd, numbytes;  /* Ecouter sock_fd, nouvelle connection sur new_fd */
  struct sockaddr_in my_addr;    /* Adresse */
  struct sockaddr_in their_addr; /* Adresse du connecté  */
  char buf[MAXDATASIZE];
  socklen_t sin_size;
};

#endif
