#include <iostream>
#include <algorithm>
#include <memory>
#include <iterator>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <Common/Timer.h>

class MySimpleClass
{
public:
  double m_x = 0.0;
  double m_y = 0.0;
  std::string m_name;
};

int main(int argc, char* argv[])
{
  size_t count = 10000000;

  if (argc == 2) count = boost::lexical_cast<size_t>(argv[1]);

  {
    std::vector<std::unique_ptr<MySimpleClass>> vptrObjects;
    vptrObjects.reserve(count);
    {
      Timer t("Build pointer vector");
      std::generate_n(std::back_inserter(vptrObjects), count, []()
      { return std::unique_ptr<MySimpleClass>(new MySimpleClass()); });
    }
    {
      Timer t("Modify Pointer Vector");
      std::for_each(vptrObjects.begin(), vptrObjects.end(),
                    [](std::unique_ptr<MySimpleClass>& ptr)
      {
        ptr->m_x = 2.0;
        ptr->m_y = 0.78 * ptr->m_x;
      });
    }
    {
      Timer t("Delete Pointer Vector");
      vptrObjects.resize(0);
    }
  }

  std::cout << std::endl;

  {
    std::vector<MySimpleClass> objects;
    objects.reserve(count);
    {
      Timer t("Build Object Vector");
      std::generate_n(std::back_inserter(objects), count, []()
      { return MySimpleClass(); });
    }
    {
      Timer t("Modify Object Vector");
      std::for_each(objects.begin(), objects.end(), [](MySimpleClass& m)
      {
        m.m_x = 2.0;
        m.m_y = 0.78 * m.m_x;
      });
    }
    {
      Timer t("Delete Object Vector");
      objects.resize(0);
    }
  }
}
