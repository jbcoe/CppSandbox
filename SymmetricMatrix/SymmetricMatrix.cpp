#include <iostream>
#include <random>
#include <vector>

template <typename T>
class SymmetricMatrix
{
public:
  SymmetricMatrix(size_t dim)
  {
    m_data.resize((dim * (dim + 1)) >> 1);
  }

  T& operator()(size_t row, size_t column)
  {
    if (row < column)
    {
      std::swap(row, column);
    }
    return m_data[((row * (row + 1)) >> 1) + column];
  }

private:
  std::vector<T> m_data;
};


int main(int argc, char* argv[])
{
  std::uniform_real_distribution<double> distribution(-1, 1);
  std::mt19937 engine;
  auto generator = [&]
  {
    return distribution(engine);
  };

  SymmetricMatrix<double> s(5);
  for (size_t i = 0; i < 5; ++i)
  {
    for (size_t j = 0; j < 5; ++j)
    {
      s(i, j) = generator();
    }
  }

  for (size_t i = 0; i < 5; ++i)
  {
    for (size_t j = 0; j < 5; ++j)
    {
      std::cout << s(i, j) << " ";
    }
    std::cout << "\n";
  }
}
