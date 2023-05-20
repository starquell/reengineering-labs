#include <cstdint>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <string>
#include <memory>
#include <fstream>
#include <string_view>

#include <nlohmann/json.hpp>
#include <pugixml.hpp>

using json = nlohmann::json;

class OutputStrategy {
public:
    virtual void print(int time_elapsed, int num_iterations) = 0;
    virtual ~OutputStrategy() = default;
};

class TextOutputStrategy : public OutputStrategy 
{
public:
    void print(int time_elapsed, int num_iterations) override {
        std::cout << "Time Elapsed: " << static_cast<double>(time_elapsed) / num_iterations << " ms\n";
        std::cout << "Number of Iterations: " << num_iterations << '\n';
    }
};

class JsonOutputStrategy : public OutputStrategy {
public:
    void print(int time_elapsed, int num_iterations) override 
    {
        json j;
        j["time_elapsed"] = static_cast<double>(time_elapsed) / num_iterations;
        j["num_iterations"] = num_iterations;
        std::cout << j.dump(4) << std::endl;
    }
};

class XmlOutputStrategy : public OutputStrategy {
public:
    void print(int time_elapsed, int num_iterations) override 
    {
        pugi::xml_document doc;
        auto root = doc.append_child("benchmark");
        root.append_child("time_elapsed").text() = static_cast<double>(time_elapsed) / num_iterations;
        root.append_child("num_iterations").text() = num_iterations;
        doc.save(std::cout);
    }
};


enum class OutputFormat {
    Text,
    Json,
    Xml
};

std::unique_ptr<OutputStrategy> createOutputStrategy(OutputFormat format) {
    switch (format) {
        case OutputFormat::Text: return std::make_unique<TextOutputStrategy>();
        case OutputFormat::Json: return std::make_unique<JsonOutputStrategy>();
        case OutputFormat::Xml:  return std::make_unique<XmlOutputStrategy>();
        default: throw std::runtime_error("Unknowm output format");
    }
}

struct InputArgs {
    std::filesystem::path executable_path;
    OutputFormat output_format;
    int num_iterations;
};

InputArgs parse_input_arguments(int argc, char* argv[]) {
    if (argc < 4) {
        throw std::runtime_error("Usage: " + std::string(argv[0]) + " <path_to_executable> <output_type> <num_iterations>");
    }

    InputArgs args;
    args.executable_path = argv[1];
    args.output_format = [type = std::string_view{argv[2]}] {
        if (type == "text") {
            return OutputFormat::Text;
        }
        else if (type == "json") {
            return OutputFormat::Json;
        }
        else if (type == "xml") {
            return OutputFormat::Xml;
        }
        else {
            throw std::runtime_error("Invalid output type");
        }
    }();

    args.num_iterations = std::stoi(argv[3]);

    return args;
}

auto benchmark(const std::filesystem::path& executable_path, int num_iterations) -> uint64_t
{
    uint64_t time_elapsed = 0;
    for (int i = 0; i < num_iterations; i++) {
        auto start = std::chrono::high_resolution_clock::now();

        std::system(executable_path.string().c_str());

        auto end = std::chrono::high_resolution_clock::now();
        time_elapsed += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }
    return time_elapsed;
}

int main(int argc, char* argv[]) {
    try {
        auto [exe, output_format, n_iterations] = parse_input_arguments(argc, argv);
        auto time_elapsed = benchmark(exe, n_iterations);
        auto output_strategy = createOutputStrategy(output_format);
        output_strategy->print(time_elapsed, n_iterations);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
