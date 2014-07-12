#include "User.hh"
#include "Client.hh"

User::User(const std::string &pseudo, const std::string &password) :
  _pseudo(pseudo),
  _password(password)
{
  _score = 0;
  _connected = false;
  _waiting = false;
  _opponent = "";
  _refreshFriends = false;
}

User::~User()
{

}

void
User::setConnected(bool set)
{
  _connected = set;
}

std::string
User::getPseudo()
{
  return _pseudo;
}

bool
User::getConnected()
{
  return (_connected);
}

std::string
User::getPassword()
{
  return _password;
}

bool
User::addFriend(User *f)
{
  if (f == this)
    return false;
  for (std::list<User *>::iterator it = _friends.begin(); it != _friends.end(); it++)
    {
      if (*it == f)
	return false;
    }
  _refreshFriends = true;
  _friends.push_back(f);
  return true;
}

std::list<User *> &
User::getFriends()
{
  return _friends;
}

void
User::defiedBy(const std::string &login)
{
  _opponent = login;
}

std::string
User::getOpponent()
{
  return _opponent;
}

void
User::setClient(Client *client)
{
  _client = client;
}

Client *
User::getClient()
{
  return _client;
}

void
User::setWaiting(bool waiting)
{
  _waiting = waiting;
}

bool
User::getWaiting() const
{
  return _waiting;
}

void
User::setRefreshFriends(bool s)
{
  _refreshFriends = s;
}

bool
User::refreshFriends() const
{
  return _refreshFriends;
}

void
User::win(unsigned int scoreOpponent)
{
  if (scoreOpponent <= 100)
    _score += 100;
  else
    _score += scoreOpponent / 2;
}

void
User::lose()
{
  _score /= 2;
}

unsigned int
User::getScore() const
{
  return _score;
}
