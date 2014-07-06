#ifndef	__PARSER_HH__
#define	__PARSER_HH__

#include <string>
//#include "Server.hh"

class Client;

class	Parser
{
public:
  Parser(Client *);
  ~Parser();

  void	parse(const std::string &);
private:

  void	error();

  void	casePlay();
  void	casePlayNb();

  void	caseFirstC();

  void	caseConnect();
  void	caseConnectUsername();
  void	caseConnectPassword();

  void	caseCreateAccount();
  void	caseCreateAccountUsername();
  void	caseCreateAccountPassword();

  void	caseQuit();

  void	caseAdd();
  void	caseAddUsername();

  void	caseOk();
  void	caseKo();

  void	caseGetFriends();

  void	caseDefy();
  void	caseDefyUsername();

  void	caseRefresh();

  Client			*_client;
  std::string			_line;
  std::string::const_iterator	_i;
  std::string			_buff3;
  std::string			_buff2;
  std::string			_buff1;
};

#endif
