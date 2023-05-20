#include <iostream>
#include <chrono>
#include <cstdlib>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <path_to_executable> <output_type> <num_iterations>\n";
        return 1;
    }

    const std::string executable_path = argv[1];
    const std::string output_type = argv[2];

    const int num_iterations = std::stoi(argv[3]);
    int time_elapsed = 0;

    for (int i = 0; i < num_iterations; i++) {
        auto start = std::chrono::high_resolution_clock::now();

        std::system(executable_path.c_str());
        
        auto end = std::chrono::high_resolution_clock::now();
        time_elapsed += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    if (output_type == "text") {
        std::cout << "Time Elapsed: " << static_cast<double>(time_elapsed) / num_iterations << " ms\n";
        std::cout << "Number of Iterations: " << num_iterations << '\n';
    }
    else if (output_type == "json") {
        std::cout << "{ \"TimeElapsed\": " << static_cast<double>(time_elapsed) / num_iterations
                  << ", \"NumIterations\": " << num_iterations << " }\n";
    }
    else if (output_type == "xml") {
        std::cout << "<benchmark>\n";
        std::cout << "\t<time_elapsed>" << static_cast<double>(time_elapsed) / num_iterations << "</time_elapsed>\n";
        std::cout << "\t<num_iterations>" << num_iterations << "</num_iterations>\n";
        std::cout << "</benchmark>\n";
    }

    return 0;
}
