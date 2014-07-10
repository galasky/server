#include <iostream>
#include "Client.hh"
#include "DataBase.hh"

Client::Client(int socket) :
  _parser(this),
  _socket(socket),
  _connect(true)
{
  _user = NULL;
  _ready = false;
  _opponent = NULL;
  _game = NULL;
  _win = 0;
  emmet("HELLO\n");
}

Client::~Client()
{
  std::cout << "close" << std::endl;

  if (_opponent != NULL)
    {
      if (_opponent->getOpponent() == this)
	_opponent->goHome();
    }

  if (_user != NULL)
    {
      _user->setClient(NULL);
      _user->setConnected(false);
    }
}

void
Client::goHome()
{
  std::cout << "GO HOME" << std::endl;
  _opponent = NULL;
  _ready = false;
  _game = NULL;
  _win = 0;
  _user->setWaiting(false);
  emmet("GO HOME\n");
}

void
Client::populateFdSet(fd_set *readf, int *max_fd)
{
  FD_SET(_socket, readf);
  if (*max_fd < _socket)
    *max_fd = _socket;
}

void
Client::checkFdSet(fd_set *readf)
{
  if (FD_ISSET(_socket, readf))
    {
      std::string str = recept();
      _parser.parse(str);
    }
}

std::string
Client::recept()
{
  int		numbytes;
  std::string	str;

  if (_connect)
    {
      if ((numbytes=recv(_socket, _buf, MAXDATASIZE, 0)) == -1)
	{
	  perror("recv");
	  _connect = false;
	}
      if (!numbytes)
	_connect = false;
      
      _buf[numbytes] = '\0';
      str.append(_buf);
      std::cout << "RECEPT : " << str << std::endl;
    }
  return (str);
}

void
Client::emmet(const std::string &str)
{
  if (_connect)
    {
      std::cout << "EMMET : " << (_user ? _user->getPseudo() : "") << " " << str << std::endl;
      if (send(_socket, str.c_str(), str.size(), 0) == -1)
	perror("send");
    }
}

bool
Client::isConnect()
{
  return (_connect);
}

void
Client::end()
{
  _connect = false;
  emmet("OK\tYou are logout\n");
}

void
Client::setUser(User *user)
{
  _user = user;
}

User *
Client::getUser()
{
  return _user;
}

void
Client::win()
{
  _win++;
}

void
Client::add(const std::string &login)
{
  if (_user == NULL)
    return emmet("ADD\tKO\tPlease login\n");
  if (DataBase::instance().userExist(login))
    {
      if (_user->addFriend(DataBase::instance().getUser(login)))
	{
	  emmet("ADD\tOK\tAdding " + login + "\n");
	  return getFriends();
	}
      return emmet("ADD\tKO\t" + login + " is already your friend\n");
    }
  return emmet("ADD\tKO\tNo found " + login + "\n");
}

void
Client::connect(const std::string &login, const std::string &password)
{
  if (_user != NULL)
    return emmet("KO\tYou are already connected\n");
  User *user = DataBase::instance().getUser(login, password);
  if (user == NULL)
    return emmet("KO\tBad login or bad password\n");
  setUser(user);
  _user->setClient(this);
  _user->setRefreshFriends(true);
  _user->setConnected(true);
  emmet("OK\tConnect success\n");
  getFriends();
}
void
Client::create(const std::string &login, const std::string &password)
{
  if (_user != NULL)
    return emmet("KO\tYou are already connected\n");
  if (DataBase::instance().createUser(login, password))
    {
      setUser(DataBase::instance().getUser(login, password));
      _user->setClient(this);
      _user->setConnected(true);
      _user->setRefreshFriends(true);
      emmet("OK\tCreate success\n");
      return getFriends();
    }
  return emmet("KO\tLogin already used\n");
}

void
Client::getFriends()
{
  if (_user == NULL)
    return emmet("KO\tPlease login\n");
  if (_game != NULL)
    return emmet("KO\tGET FRIENDS You are playing\n");

  std::list<User *> list =  _user->getFriends();
  std::string str = "Friends :";
  for (std::list<User *>::iterator it = list.begin(); it != list.end(); it++)
    {
      str += "\t" + (*it)->getPseudo() + "\t" + ((*it)->getConnected() ? "OK" : "KO") + "\t" + ((*it)->getPseudo() == _user->getOpponent() ? "OK" : "KO");
    }
  str += "\n";
  return emmet(str);
}

void
Client::defy(const std::string &login)
{
  User	*user;

  if (_user == NULL)
    return emmet("DEFY\tKO\tPlease login\n");
  if (_game != NULL)
    return emmet("DEFY\tKO\tDEFY You are playing\n");
  if (_user->getPseudo() == login)
    return emmet("DEFY\tKO\tYou can't defy your self\n");
  if (DataBase::instance().userExist(login))
    {
      user = DataBase::instance().getUser(login);
      if (user->getConnected() && user->getWaiting() == false)
	{
	  if (user->getClient() == NULL)
	    return emmet("DEFY\tKO\tCLIENT EST NULL CONNARD\n");
	  _opponent = user->getClient();
	  _user->setWaiting(true);
	  user->getClient()->defiedBy(this);
	  return emmet("DEFY\tOK\tFind\n");
	}
      else
	return emmet("DEFY\tKO\t" + login + " is not joinable\n");
    }
  return emmet("DEFY\tKO\t" + login + " does not exist\n");
}

void
Client::defiedBy(Client *opponent)
{
  _opponent = opponent;
  emmet("DEFIED BY\t" + opponent->getUser()->getPseudo() + "\n");
}

void
Client::ok()
{
  if (_user->getWaiting() == false && _opponent != NULL)
    {
      _ready = true;
      _opponent->setReady(true);
      _game = new Game(_opponent, this);
      _opponent->setGame(_game);
      _opponent->getUser()->setWaiting(false);
      _opponent->emmet("ACCEPT\tOK\n");
      emmet("BEGIN\tP2\n");
    }
}

void
Client::ko()
{
  if (_user->getWaiting() == false)
    {
      if (_opponent && _opponent->getOpponent() == this)
	{
	  _opponent->getUser()->setWaiting(false);
	  _opponent->emmet("ACCEPT\tKO\n");
	}
    }
  else
    {
      _user->setWaiting(false);
    }  
}
 
void
Client::play(unsigned int nb)
{
  if (_game == NULL)
    return emmet("KO\tYou are not playing\n");
  _game->play(this, nb);
}

void
Client::refresh()
{
  if (_user == NULL)
    return emmet("KO\tPlease login\n");
  if (_game != NULL)
    return emmet("KO\tREFRESH You are playing\n");

  if (_user->refreshFriends())
    {
      _user->setRefreshFriends(false);
      return getFriends();
    }
  if (_user->getWaiting())
    {
      if (_opponent && _opponent->isReady() && _opponent->getOpponent() == this)
	{
	  _user->setWaiting(false);
	  _ready = true;
	}
    }
  if (_user->getWaiting() == false && _opponent != NULL)
    {
      if (_ready == false && _opponent->getUser()->getWaiting())
	return emmet("DEFIED BY\t" + _opponent->getUser()->getPseudo() + "\n");    
      if (_ready == true && _opponent->isReady())
	{
	  _game = new Game(this, _opponent);
	  _opponent->setGame(_game);
	  _opponent->emmet("BEGIN\tP2\n");
	  return emmet("BEGIN\tP1\n");
	}
    }
  return emmet("NOTHING\n");
}

bool
Client::isReady()
{
  return _ready;
}

void
Client::setReady(bool ok)
{
  _ready = ok;
}

Client *
Client::getOpponent()
{
  return _opponent;
}

void
Client::setGame(Game *game)
{
  _game = game;
}

bool
Client::winer()
{
  if (_win >= 3)
    return true;
  return false;
}
