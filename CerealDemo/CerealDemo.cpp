#include <iostream>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>

struct Animal
{
  virtual ~Animal(){}

  virtual const char* Noise() const = 0;
  virtual const char* Name() const = 0;

  template <class Archive>
  void serialize(Archive& archive)
  {
  }
};

struct Cat : Animal
{
  std::string s_;

  Cat(std::string s) : s_(s) {}

  const char* Name() const { return s_.c_str(); }
  
  const char* Noise() const { return "Meoow!"; }

  template <class Archive>
  static void load_and_construct(Archive& archive,
                                 cereal::construct<Cat>& construct)
  {
    std::string s;
    archive(cereal::make_nvp("Name", s));
    construct(s);
  }
  
  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(cereal::make_nvp("Name", s_));
  }
};

CEREAL_REGISTER_TYPE(Cat);

struct Dog : Animal
{
  std::string s_;

  Dog(std::string s) : s_(s) {}

  const char* Name() const { return s_.c_str(); }
  
  const char* Noise() const { return "Woof!"; }

  template <class Archive>
  static void load_and_construct(Archive& archive,
                                 cereal::construct<Dog>& construct)
  {
    std::string s;
    archive(cereal::make_nvp("Name", s));
    construct(s);
  }
  
  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(cereal::make_nvp("Name", s_));
  }
};

CEREAL_REGISTER_TYPE(Dog);

int main(int argc, char* argv[])
{
  std::ostringstream oss;
  
  std::vector<std::unique_ptr<Animal>> xs;
  xs.push_back(std::make_unique<Dog>("Fido"));
  xs.push_back(std::make_unique<Cat>("Thomas"));
  xs.push_back(std::make_unique<Cat>("Felix"));
  xs.push_back(std::make_unique<Dog>("Rex"));

  {
    cereal::JSONOutputArchive oa(oss);
    oa(cereal::make_nvp("Animals", xs));
  }

  std::cout << oss.str() << "\n";
  
  std::istringstream iss(oss.str());
  cereal::JSONInputArchive ia(iss);
  std::vector<std::unique_ptr<Animal>> rxs;
  ia(rxs);

  for (const auto& rx: rxs)
  {
    std::cout << rx->Name() << '\n';
    std::cout << rx->Noise() << "\n\n";
  }
}

