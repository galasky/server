#include "Server.hh"

int	main(int ac, char **av)
{
  if (ac == 2)
    {
      Server	server(atoi(av[1]));
      server.run();
    }
  fprintf(stderr,"usage: server port\n");
  return (0);
}
