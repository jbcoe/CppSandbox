#include <iostream>
#include <array>

template <typename T>
bool IsAlignedOnBoundary(const T& t, uintptr_t size)
{
  return (uintptr_t)(&t) % size == 0;
}

#define CHECK_ALIGNMENT(x, y)                                                  \
  std::cout << #x " at " << std::hex << &x                                     \
            << " aligned on " #y " byte boundary " << std::boolalpha           \
            << IsAlignedOnBoundary(x, y) << std::endl;

template <typename Data_T, size_t N = 0>
struct alignas(N) AlignedData
{
  AlignedData()
  {
  }

  AlignedData(Data_T data_) : data(std::move(data_))
  {
  }

  operator Data_T&()
  {
    return data;
  }
  Data_T data;
};

int main(int argc, char* argv[])
{
  int x_0 = 5;
  int x_1 = 5;
  int x_2 = 5;
  int x_3 = 5;

  CHECK_ALIGNMENT(x_0, 8);
  CHECK_ALIGNMENT(x_1, 8);
  CHECK_ALIGNMENT(x_2, 8);
  CHECK_ALIGNMENT(x_3, 8);

  std::cout << std::endl;

  AlignedData<int, 128> y_0_128 = 5;
  AlignedData<int, 128> y_1_128 = 5;
  AlignedData<int, 128> y_2_128 = 5;
  AlignedData<int, 128> y_3_128 = 5;

  CHECK_ALIGNMENT(y_0_128, 128);
  CHECK_ALIGNMENT(y_1_128, 128);
  CHECK_ALIGNMENT(y_2_128, 128);
  CHECK_ALIGNMENT(y_3_128, 128);

  std::cout << std::endl;

  AlignedData<std::array<double, 1>, 16> realNumbers_0_16;
  AlignedData<std::array<double, 1>, 16> realNumbers_1_16;
  AlignedData<std::array<double, 1>, 16> realNumbers_2_16;
  AlignedData<std::array<double, 1>, 16> realNumbers_3_16;

  CHECK_ALIGNMENT(realNumbers_0_16, 64);
  CHECK_ALIGNMENT(realNumbers_1_16, 64);
  CHECK_ALIGNMENT(realNumbers_2_16, 64);
  CHECK_ALIGNMENT(realNumbers_3_16, 64);

  std::cout << std::endl;
}
