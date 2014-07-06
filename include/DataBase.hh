#ifndef __DATABASE_HH__
#define __DATABASE_HH__

#include <map>
#include <string>
#include "User.hh"

class DataBase
{
public:
  ~DataBase();

  static DataBase	&instance();

  User			*getUser(const std::string &pseudo, const std::string &password);
  User			*getUser(const std::string &pseudo);
  bool			createUser(const std::string &pseudo, const std::string &password);
  bool			saveUser(User *);
  bool			userExist(const std::string &);
private:
  DataBase();

  std::map<std::string, User *>	_db;
};

#endif
