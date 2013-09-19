#include <iostream>
#include <vector>
#include <iterator>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
  try
  {
    po::variables_map vm;
    po::options_description generic("Generic options");
    generic.add_options()("version,v", "print version string")(
        "help,h", "produce help message");

    po::parsed_options parsed = po::command_line_parser(argc, argv)
                                    .options(generic)
                                    .allow_unregistered()
                                    .run();

    std::vector<std::string> to_pass_further =
        po::collect_unrecognized(parsed.options, po::include_positional);

    if (!to_pass_further.empty())
    {
      std::cerr << "\nUnrecognized program options: \n";
      std::copy(to_pass_further.begin(), to_pass_further.end(),
                std::ostream_iterator<std::string>(std::cerr, " "));
      std::cout << "\n\n" << generic << "\n";
      return 1;
    }

    po::store(parsed, vm);

    po::notify(vm);

    if (vm.count("help"))
    {
      std::cout << generic << "\n";
      return 1;
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  }
  catch (...)
  {
    std::cerr << "Exception of unknown type!\n";
  }

  return 0;
}
