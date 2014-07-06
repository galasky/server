#include <boost/lexical_cast.hpp>
#include "Parser.hh"
#include "Client.hh"
#include "DataBase.hh"

Parser::Parser(Client *client) :
  _client(client)
{

}


Parser::~Parser()
{
}

void
Parser::error()
{
  _client->emmet("KO\tParser Error " + _line);
}

void
Parser::parse(const std::string &line)
{
  _line = line;
  _i = _line.begin();
  switch (*_i)
    {
    case 'A':
      return caseAdd();
    case 'C':
      return caseFirstC();
    case 'D':
      return caseDefy();
    case 'G':
      return caseGetFriends();
    case 'K':
      return caseKo();
    case 'O':
      return caseOk();
    case 'P':
      return casePlay();
    case 'Q':
      return caseQuit();
    case 'R':
      return caseRefresh();
    default:
      return error();
    }
}

void
Parser::caseRefresh()
{
  std::string	tmp("EFRESH\n");

  for (std::string::iterator i = tmp.begin(); i != tmp.end(); i++)
    {
      _i++;
      if (*i != *_i)
        return error();
    }
  return _client->refresh();
}

void
Parser::casePlay()
{
  std::string	tmp("LAY\t");

  for (std::string::iterator i = tmp.begin(); i != tmp.end(); i++)
    {
      _i++;
      if (*i != *_i)
        return error();
    }
  _buff1 = "";
  return casePlayNb();
}

void
Parser::casePlayNb()
{
  _i++;
  switch (*_i)
    {
    case '\t':
      return error();
    case '\n':
      if (_buff1.size() != 1)
        return error();
      return _client->play(atoi(_buff1.c_str()));
    case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
      _buff1 += *_i;
      return casePlayNb();
    }
}

void
Parser::caseOk()
{
  std::string	tmp("K\n");

  for (std::string::iterator i = tmp.begin(); i != tmp.end(); i++)
    {
      _i++;
      if (*i != *_i)
        return error();
    }
  return _client->ok();
}

void
Parser::caseKo()
{
  std::string	tmp("O\n");

  for (std::string::iterator i = tmp.begin(); i != tmp.end(); i++)
    {
      _i++;
      if (*i != *_i)
        return error();
    }
  return _client->ko();
}


void
Parser::caseAdd()
{
  std::string	tmp("DD\t");

  for (std::string::iterator i = tmp.begin(); i != tmp.end(); i++)
    {
      _i++;
      if (*i != *_i)
        return error();
    }
  _buff1 = "";
  return caseAddUsername();
}

void
Parser::caseAddUsername()
{
  _i++;
  switch (*_i)
    {
    case '\t':
      return error();
    case '\n':
      if (_buff1.empty())
        return error();
      return _client->add(_buff1);
    default:
      _buff1 += *_i;
      return caseAddUsername();
    }
}

void
Parser::caseQuit()
{
  std::string	tmp("UIT\n");

  for (std::string::iterator i = tmp.begin(); i != tmp.end(); i++)
    {
      _i++;
      if (*i != *_i)
        return error();
    }
  return _client->end();
}


void
Parser::caseFirstC()
{
  _i++;
  switch (*_i)
    {
    case 'O':
      return caseConnect();
    case 'R':
      return caseCreateAccount();
    default:
      return error();
    }
}

void
Parser::caseConnect()
{
  std::string	tmp("NNECT\t");

  for (std::string::iterator i = tmp.begin(); i != tmp.end(); i++)
    {
      _i++;
      if (*i != *_i)
        return error();
    }
  _buff1 = "";
  return caseConnectUsername();
}

void
Parser::caseConnectUsername()
{
  _i++;
  switch (*_i)
    {
    case '\n':
      return error();
    case '\t':
      if (_buff1.empty())
        return error();
      _buff2 = "";
      return caseConnectPassword();
    default:
      _buff1 += *_i;
      return caseConnectUsername();
    }
}

void
Parser::caseConnectPassword()
{
  _i++;
  switch (*_i)
    {
    case '\n':
      if (_buff2.empty())
        return error();
      return _client->connect(_buff1, _buff2);
    case '\t':
      return error();
    default:
      _buff2 += *_i;
      return caseConnectPassword();
    }
}

void
Parser::caseCreateAccount()
{
  std::string	tmp("EATE\t");

  for (std::string::iterator i = tmp.begin(); i != tmp.end(); i++)
    {
      _i++;
      if (*i != *_i)
        return error();
    }
  _buff1 = "";
  return caseCreateAccountUsername();
}

void
Parser::caseCreateAccountUsername()
{
  _i++;
  switch (*_i)
    {
    case '\n':
      return error();
    case '\t':
      if (_buff1.empty())
        return error();
      _buff2 = "";
      return caseCreateAccountPassword();
    default:
      _buff1 += *_i;
      return caseCreateAccountUsername();
    }
}

void
Parser::caseCreateAccountPassword()
{
  _i++;
  switch (*_i)
    {
    case '\n':
      if (_buff2.empty())
        return error();
      return _client->create(_buff1, _buff2);
    case '\t':
      return error();
    default:
      _buff2 += *_i;
      return caseCreateAccountPassword();
    }
}

void
Parser::caseGetFriends()
{
  std::string	tmp("ET FRIENDS\n");

  for (std::string::iterator i = tmp.begin(); i != tmp.end(); i++)
    {
      _i++;
      if (*i != *_i)
        return error();
    }
  _buff1 = "";
  return _client->getFriends();
}

void
Parser::caseDefy()
{
  std::string	tmp("EFY\t");

  for (std::string::iterator i = tmp.begin(); i != tmp.end(); i++)
    {
      _i++;
      if (*i != *_i)
        return error();
    }
  _buff1 = "";
  return caseDefyUsername();
}

void
Parser::caseDefyUsername()
{
  _i++;
  switch (*_i)
    {
    case '\n':
      if (_buff1.empty())
        return error();
      return _client->defy(_buff1);
    case '\t':
      return error();
    default:
      _buff1 += *_i;
      return caseDefyUsername();
    }
}
