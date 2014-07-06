#include "Server.hh"
#include <iostream>

Server::Server(int port) :
  _port(port)
{
  createSocket();
  bindSocket();
}
Server::~Server()
{
  
}

void
Server::run()
{
  listenSocket();

  while (true) 
    {
      fd_set readf;
      int	max_fd;

      FD_ZERO(&readf);

      FD_SET(sockfd, &readf);
      max_fd = sockfd;
      for (std::list<Client *>::iterator it = _listClient.begin(); it != _listClient.end(); it++)
	(*it)->populateFdSet(&readf, &max_fd);
      
      if (select(max_fd + 1, &readf, NULL, NULL, NULL) == -1)
	perror("select");

      if (FD_ISSET(sockfd, &readf))
	{
	  printf("New client\n");
	  new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	  printf("serveur: Reçu connection de %s\n", inet_ntoa(their_addr.sin_addr));
	  _listClient.push_back(new Client(new_fd));
	}
      
      for (std::list<Client *>::iterator it = _listClient.begin(); it != _listClient.end(); it++)
	{
	  (*it)->checkFdSet(&readf);
	  if ((*it)->isConnect() == false)
	    {
	      delete *it;
	      _listClient.erase(it++);
	    }
	}
      /*
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
      
	printf("serveur: Reçu connection de %s\n", inet_ntoa(their_addr.sin_addr));
      
	if (!fork())
	startProtocole();
      
      
      
	close(new_fd);
      
	while(waitpid(-1,NULL,WNOHANG) > 0);
      */
    }
}

void
Server::emmet(const std::string &str)
{
  if (send(new_fd, str.c_str(), str.size(), 0) == -1)
    perror("send");
}

std::string
Server::recept()
{
  std::string str;

  if ((numbytes=recv(new_fd, buf, MAXDATASIZE, 0)) == -1)
    {
      perror("recv");
      exit(1);
    }
  
  buf[numbytes] = '\0';
  str.append(buf);
  return (str);
}

void
Server::end()
{
  emmet("END\n");
  close(new_fd);
  exit(0);
}

void
Server::startProtocole()
{
  std::string str;

  emmet("HELLO\n");
  while (true)
    {
      str = recept();
    }
}

void
Server::createSocket()
{
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  my_addr.sin_family = AF_INET;         /* host byte order */
  my_addr.sin_port = htons(_port);     /* short, network byte order */
  my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-remplissage avec mon IP */
  bzero(&(my_addr.sin_zero), 8);        /* zero pour le reste de struct */
}

void
Server::bindSocket()
{
  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
      == -1) {
    perror("bind");
    exit(1);
  }
}

void
Server::listenSocket()
{
  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }
  sin_size = sizeof(struct sockaddr_in);
}
