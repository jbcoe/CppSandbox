#include <iostream>
#include <stack>
#include <iterator>
#include <algorithm>
#include <functional>

struct Node
{
  Node(int data) : data_(data) {}
  Node() {}

  static std::unique_ptr<Node> Instance(int data)
  {
    return std::make_unique<Node>(data);
  }

  int data_ = 0;
  std::unique_ptr<Node> left_;
  std::unique_ptr<Node> right_;
};


struct Incrementable
{
  operator int ()
  {
    return i_++;
  }
  
  void operator++()
  {
    i_++;
  }

  int i_ = 0;
};


void PrintNodes_Recursive(Node& node)
{
  std::cout << node.data_ << ' ';
  
  if ( node.left_ )
  {
    PrintNodes_Recursive(*node.left_);
  }

  if ( node.right_ )
  {
    PrintNodes_Recursive(*node.right_);
  }
}

void PrintNodes_FakeRecursive(Node& node, std::stack<std::function<void()>>& fakeStack)
{
  std::cout << node.data_ << ' ';
  
  if ( node.right_ )
  {
    fakeStack.push([&]{PrintNodes_FakeRecursive(*node.right_, fakeStack);});
  }
  
  if ( node.left_ )
  {
    fakeStack.push([&]{PrintNodes_FakeRecursive(*node.left_, fakeStack);});
  }
}

void PrintNodes_FakeRecursive(Node& node)
{
  std::stack<std::function<void()>> fakeStack;
  PrintNodes_FakeRecursive(node, fakeStack);

  while(!fakeStack.empty())
  {
    auto f = std::move(fakeStack.top());
    fakeStack.pop();
    f();
  }
}

template<typename Generator_T>
void AddNodes(Node& root, Generator_T&& generator, size_t levels)
{
  if ( levels == 0 ) { return; }

  root.left_  = generator();
  if(root.left_){ AddNodes(*root.left_,  generator, levels - 1); }

  root.right_ = generator();
  if (root.right_) { AddNodes(*root.right_, generator, levels - 1); }
}

int main(int argc, char* argv[])
{                          
  Incrementable i;
  Node rootNode(i);
  AddNodes(rootNode, [&i]{return Node::Instance(i);}, 4);

  PrintNodes_Recursive(rootNode); 
  std::cout << std::endl;
  
  PrintNodes_FakeRecursive(rootNode); 
  std::cout << std::endl;
}

