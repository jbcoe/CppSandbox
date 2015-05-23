#include <iostream>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

struct Cat
{
  friend class cereal::access;

  std::string s_;

  Cat(std::string s) : s_(std::move(s))
  {
  }

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(cereal::make_nvp("Name", s_));
  }
};

int main(int argc, char* argv[])
{
  std::ostringstream ss;
  std::vector<Cat> cats = {Cat("Sammy"), Cat("Toby")};

  {
    cereal::JSONOutputArchive a(std::cout);
    a(cereal::make_nvp("Cats", cats));
  }

  std::cout << ss.str() << "\n";
}

