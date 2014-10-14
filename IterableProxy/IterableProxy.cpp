#include <iostream>
#include "IterableProxy.hpp"
#include "Zoo.hpp"

int main(int argc, char* argv[])
{
  Zoo myZoo;

  auto iterableLions = make_iterable_proxy<Lion>([&myZoo](size_t index)
                                                 {
                                                   return myZoo.GetLion(index);
                                                 },
                                                 [&myZoo]()
                                                 {
                                                   return myZoo.LionCount();
                                                 });

  for (auto myLion : iterableLions)
  {
    std::cout << myLion->makeNoise() << std::endl;
  }
}
