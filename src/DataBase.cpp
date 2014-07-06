#include "DataBase.hh"
#include "md5.h"

DataBase::DataBase()
{
}

DataBase::~DataBase()
{
}

DataBase &
DataBase::instance()
{
  static DataBase instance;

  return instance;
}

User *
DataBase::getUser(const std::string &pseudo, const std::string &password)
{
  std::string str = password + md5(pseudo);

  str = md5(str);
  if (_db.find(pseudo) != _db.end())
    {
      if (_db[pseudo]->getPassword() == str)
	return _db[pseudo];
    }
  return NULL;
}

User *
DataBase::getUser(const std::string &pseudo)
{
  //  std::cout << "PSEUDO " << pseudo << std::endl;
  if (_db.find(pseudo) != _db.end())
    {
      return _db[pseudo];
    }
  return NULL;
}

bool
DataBase::createUser(const std::string &pseudo, const std::string &password)
{
  std::string str = password + md5(pseudo);

  str = md5(str);
  if (_db.find(pseudo) == _db.end())
    {
      _db[pseudo] = new User(pseudo, str);
      return true;
    }
  return false;  
}

bool
DataBase::saveUser(User *user)
{
  _db[user->getPseudo()] = user;
  return true;
}

bool
DataBase::userExist(const std::string &pseudo)
{
  return (_db.find(pseudo) != _db.end());
}
