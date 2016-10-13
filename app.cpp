#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <iostream>

#include <boost/mpi.hpp>

namespace mpi = boost::mpi;

namespace tce
{
  auto make_random(int min = 0, int max = 30) {
    std::random_device device{};
    std::mt19937       rand{device()};
    std::uniform_int_distribution<> dist{min, max};
    return dist(rand); 
  }
  void print_potato(int potato, int dest)
  {
	std::cout << "Sending potato: " << potato
			  << " to processor #"  << dest 
			  << '\n';
  }
}

int main() {
    mpi::environment env{};
    mpi::communicator world{};

	static const int size = world.size();
	static const int rank = world.rank();

    if(world.rank() == 0) {
		auto potato = tce::make_random(0, 100);
		auto dest = tce::make_random(0, size);
		tce::print_potato(potato, dest);
		world.send(dest, 0, potato);
		world.recv(mpi::any_source, 0, potato);
		std::cout << "Received potato on the 0 processor, killing everything\n"; 
		for(auto i = 1; i < size; ++i) {
			std::cout << "Sending kill message to " << i << '\n';
			world.send(i, 0, -1);
		}
    } else {
		int potato = 0;
		while(true) {
			world.recv(mpi::any_source, 0, potato);
			if(potato < 0) break;
			auto dest = tce::make_random(0, size - 1);
			tce::print_potato(potato, dest);
			world.send(dest, 0, potato);	
		}
		std::cout << "Killing process " << rank << '\n';
    }
    return 0;
}
