#ifndef __GAME_HH__
#define __GAME_HH__

#include <vector>

class	Client;

class Game
{
public:
  Game(Client *p1, Client *p2);
  ~Game();

  void	play(Client *, unsigned int);

private:
  Client	*_p1;
  Client	*_p2;
  Client	*_current;
  std::vector<unsigned int>	_queue;
  unsigned int			_nbC;
};

#endif
