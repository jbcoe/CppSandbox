#include <iostream>

struct PermissionFlags
{
  unsigned int is_write : 1;
  unsigned int is_read : 1;
  unsigned int is_execute : 1;

  operator int() const
  {
    return (is_write << 2) | (is_read << 1) | (is_execute);
  }

  void operator=(int value)
  {
    is_write = (value & 1u << 2) >> 2;
    is_read = (value & 1u << 1) >> 1;
    is_execute = (value & 1u << 0);
  }

  PermissionFlags() : is_write(0), is_read(0), is_execute(0)
  {
  }
};

int main(int argc, char* argv[])
{
  PermissionFlags permissions;
  std::cout << permissions.is_read << " " << permissions.is_write << " "
            << permissions.is_execute << std::endl;

  std::cout << "Size of permission struct = " << sizeof(permissions)
            << std::endl;

  permissions = 7;

  std::cout << permissions.is_read << " " << permissions.is_write << " "
            << permissions.is_execute << std::endl;

  std::cout << permissions << std::endl;
}
