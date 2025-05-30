#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <fstream>
#include <memory>
#include <chrono>
#include <sstream>
#include <mutex>

std::mutex cout_mutex;

//basic class
class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

//console output
class PrintCommand : public Command {
    std::string message;
public:
    PrintCommand(const std::string& msg) : message(msg) {}
    void execute() override {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << message << std::endl;
    }
};

//file output
class FileAppendCommand : public Command {
    std::string filename;
    std::string content;
public:
    FileAppendCommand(const std::string& file, const std::string& cnt)
        : filename(file), content(cnt) {}
    void execute() override {
        std::ofstream file(filename, std::ios::app);
        if (file) {
            file << content << std::endl;
        }
    }
};

//arithmetic operations
class MathCommand : public Command {
    double a, b;
    char op;
public:
    MathCommand(double x, double y, char operation)
        : a(x), b(y), op(operation) {}
    void execute() override {
        double result = 0;
        switch (op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/': result = (b != 0) ? a / b : 0; break;
        }
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Math: " << a << " " << op << " " << b << " = " << result << std::endl;
    }
};

//loops
class LoopCommand : public Command {
    int iterations;
    std::vector<std::unique_ptr<Command>> commands;
public:
    LoopCommand(int iter) : iterations(iter) {}
    void addCommand(std::unique_ptr<Command> cmd) {
        commands.push_back(std::move(cmd));
    }
    void execute() override {
        for (int i = 0; i < iterations; ++i) {
            for (auto& cmd : commands) {
                cmd->execute();
            }
        }
    }
};

// parsing string into commands
std::unique_ptr<Command> parseCommand(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    iss >> token;

    if (token == "loop") {
        int iterations;
        iss >> iterations;
        auto loop = std::make_unique<LoopCommand>(iterations);
        std::string subCmd;
        while (std::getline(iss, subCmd, ';')) {
            auto cmd = parseCommand(subCmd);
            if (cmd) loop->addCommand(std::move(cmd));
        }
        return loop;
    }
    else if (token == "print") {
        std::string message;
        std::getline(iss, message);
        return std::make_unique<PrintCommand>(message);
    }
    else if (token == "file") {
        std::string filename, content;
        iss >> filename;
        std::getline(iss, content);
        return std::make_unique<FileAppendCommand>(filename, content);
    }
    else if (token == "math") {
        double a, b;
        char op;
        iss >> a >> op >> b;
        return std::make_unique<MathCommand>(a, b, op);
    }
    return nullptr;
}


void executeCommands(const std::string& line, int lineNum) {
    auto start = std::chrono::steady_clock::now();
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Thread for line " << lineNum << " started at: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count()
                  << "ms" << std::endl;
    }

    auto cmd = parseCommand(line);
    if (cmd) cmd->execute();

    auto end = std::chrono::steady_clock::now();
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Thread for line " << lineNum << " finished at: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count()
                  << "ms" << std::endl;
    }
}

int main() {
    std::vector<std::string> lines;
    std::string line;

    std::cout << "Enter commands (type 'end' to finish):" << std::endl;
    while (std::getline(std::cin, line) && line != "end") {
        lines.push_back(line);
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < lines.size(); ++i) {
        threads.emplace_back(executeCommands, lines[i], i + 1);
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
