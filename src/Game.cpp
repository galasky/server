#include <sstream>
#include <iostream>
#include "Game.hh"
#include "Client.hh"

Game::Game(Client *p1, Client *p2) :
  _p1(p1),
  _p2(p2),
  _current(p1)
{
  _nbC = 0;
}

Game::~Game()
{

}

void
Game::play(Client *client, unsigned int nb)
{
  if (client != _current)
    return ;

  std::stringstream ss;
  ss << nb;
  std::string sNB = ss.str();

  std::string str;
  if (_queue.size() == _nbC)
    {
      _queue.push_back(nb);
      _nbC = 0;
      _current = _current->getOpponent();
      std::cout << client->getUser()->getPseudo() << " AJOUTE " << nb << std::endl;
      str = "YOUR TURN\t";
      str += sNB;
      str += "\n";
      client->emmet("HIS TURN\n");
      client->getOpponent()->emmet(str);
    }
  else
    {
      if (nb == _queue[_nbC])
	{
	  std::cout << client->getUser()->getPseudo() << " JOUE " << nb << std::endl;
	  str = "PLAY\t";
	  str += sNB;
	  str += "\n";
	  client->getOpponent()->emmet(str);
	  _nbC++;
	}
      else
	{
	  std::cout << client->getUser()->getPseudo() << " ERREUR " << nb << std::endl;
	  str = "ERREUR\t";
	  str += sNB;
	  str += "\n";
	  client->getOpponent()->win();
	  if (client->getOpponent()->getWin() > client->getWin() + 1 || (client->getOpponent()->getWin() >= 2 && client->getOpponent()->getWin() > client->getWin()))
	    {
	      client->getOpponent()->getUser()->win(client->getUser()->getScore());
	      client->getUser()->lose();
	      client->emmet("YOU LOSE");
	      client->getOpponent()->emmet("YOU WIN");
	    }
	  else
	    {
	      _current = _p1;
	      client->getOpponent()->emmet(str);
	      _nbC = 0;
	      _queue.clear();
	    }
	}
    }
}
