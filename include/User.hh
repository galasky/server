#ifndef __USER_HH__
#define __USER_HH__

#include <list>
#include <string>

class Client;

class User
{
public:
  User(const std::string &pseudo, const std::string &password);
  ~User();

  std::string			getPseudo();
  std::string			getPassword();
  bool				addFriend(User *);
  std::list<User *>		&getFriends();
  bool				getConnected();
  void				setConnected(bool);
  void				defiedBy(const std::string &login);
  std::string			getOpponent();
  void				setClient(Client *);
  Client			*getClient();
  void				setWaiting(bool);
  bool				getWaiting() const;
  void				setRefreshFriends(bool);
  bool				refreshFriends() const;
  void				win(unsigned int);
  void				lose();
  unsigned int			getScore() const;
private:
  Client			*_client;
  std::string			_pseudo;
  std::string			_password;
  std::list<User *>		_friends;
  bool				_connected;
  bool				_waiting;
  bool				_refreshFriends;
  std::string			_opponent;
  unsigned int			_score;
};

#endif
