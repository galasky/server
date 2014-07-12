#ifndef __CLIENT_HH__
#define __CLIENT_HH__

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
#include "Parser.hh"
#include <list>
#include "User.hh"
#include "Game.hh"

#define MAXDATASIZE 100
#define BACKLOG 10     /* Nombre maxi de connections acceptées en file */

class Client
{
public:
  Client(int socket);
  ~Client();

  void	populateFdSet(fd_set *readf, int *max_fd);
  void	checkFdSet(fd_set *);

  std::string	recept();
  void		emmet(const std::string &);
  bool		isConnect();
  void		end();
  void		setUser(User *);
  User		*getUser();
  void		connect(const std::string &login, const std::string &password);
  void		create(const std::string &login, const std::string &password);
  void		add(const std::string &);
  void		getFriends();
  void		defy(const std::string &);
  void		defiedBy(Client *);
  void		ok();
  void		ko();
  void		refresh();
  bool		isReady();
  void		setReady(bool);
  Client	*getOpponent();
  void		goHome();
  void		setGame(Game *);
  void		play(unsigned int);
  void		win();
  int		getWin() const;
  bool		winer();
private:
  Parser	_parser;
  int		_socket;
  bool		_connect;
  bool		_ready;
  User		*_user;
  Client	*_opponent;
  Game		*_game;
  int		_win;
  char		_buf[MAXDATASIZE];
};

#endif
