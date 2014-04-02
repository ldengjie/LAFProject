#include "LafMgr.h"
#include <iostream>

int main(int argc, char *argv[])
{
  if ( argc != 2 ) {
      std::cout << "Usage: " << argv[0] << "  option.txt" << std::endl;
      return 1;
  }

  LafMgr laf(argv[1]);

  bool status = laf.initialize();

  if ( status ) laf.run();

  laf.finalize();

  return 0;
}
