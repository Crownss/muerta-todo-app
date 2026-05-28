#include <spdlog/spdlog.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <sys/ioctl.h>
#include <unistd.h>
#include "../../core/container/container.hpp"

int getTerminalWidth()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

void printCentered(const std::string &text)
{
    size_t termWidth = static_cast<size_t>(getTerminalWidth());
    size_t textLen = text.size();
    size_t padding = termWidth > textLen ? (termWidth - textLen) / 2 : 0;
    std::cout << std::string(padding, ' ') << text << "\n";
}

int getTerminalHeight()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}

std::string getPad()
{
    int termWidth = getTerminalWidth();
    size_t p = static_cast<size_t>(std::max(0, (termWidth - 24) / 2));
    return std::string(p, ' ');
}

void printTopPad()
{
    int termHeight = getTerminalHeight();
    int topPad = (termHeight - 11) / 2;
    for (int i = 0; i < topPad; i++)
        std::cout << "\n";
}

void clearScreen()
{
    std::cout << "\033[2J\033[1;1H";
}

void printMenu()
{
    printTopPad();
    std::string p = getPad();

    std::cout << p << "+----------------------+\n"
              << p << "|   Muerta CLI v1.0    |\n"
              << p << "+----------------------+\n"
              << p << "|  1. List all todos   |\n"
              << p << "|  2. Get todo by ID   |\n"
              << p << "|  3. Create todo      |\n"
              << p << "|  4. Update todo      |\n"
              << p << "|  5. Delete todo      |\n"
              << p << "|  0. Exit             |\n"
              << p << "+----------------------+\n"
              << p << "  Choice: ";
}

void printTodosTable(const std::vector<TodoItem> &todos)
{
    if (todos.empty())
    {
        std::cout << getPad() << "No todos found.\n";
        return;
    }
    printTopPad();
    std::string p = getPad();
    char id_str[37];

    const int w_id = 36;
    const int w_title = 15;
    const int w_desc = 15;
    const int w_status = 11;
    const int w_date = 19;

    auto trunc = [](const std::string &s, int w)
    {
        size_t sw = static_cast<size_t>(w);
        return s.size() > sw ? s.substr(0, sw - 2) + ".." : s;
    };

    std::string sep = "+" + std::string(w_id + 2, '-') + "+" + std::string(w_title + 2, '-') + "+" + std::string(w_desc + 2, '-') + "+" + std::string(w_status + 2, '-') + "+" + std::string(w_date + 2, '-') + "+";

    std::cout << sep << "\n"
              << "| " << std::left << std::setw(w_id) << "ID"
              << " | " << std::setw(w_title) << "Title"
              << " | " << std::setw(w_desc) << "Description"
              << " | " << std::setw(w_status) << "Status"
              << " | " << std::setw(w_date) << "Created At"
              << " |\n"
              << sep << "\n";

    for (const auto &t : todos)
    {
        uuid_unparse(t.id, id_str);
        std::cout << "| " << std::left
                  << std::setw(w_id) << id_str
                  << " | " << std::setw(w_title) << trunc(t.title, w_title)
                  << " | " << std::setw(w_desc) << trunc(t.description, w_desc)
                  << " | " << std::setw(w_status) << t.status
                  << " | " << std::setw(w_date) << toTimestamp(t.created_at).substr(0, w_date)
                  << " |\n";
    }
    std::cout << sep << "\n";
}

void pauseScreen()
{
    printTopPad();
    std::string pad = getPad();
    // std::cout << pad << "Press Enter to continue...";
    std::cin.get();
}

int main()
{
    auto cont = std::make_unique<Container>();
    clearScreen();
    auto todo_service = cont->getTodoService();
    std::string pad = getPad();
    std::cout << pad << "Welcome to Muerta CLI!\n";

    int choice;
    while (true)
    {
        printMenu();
        std::cin >> choice;
        std::cin.ignore();
        clearScreen();

        switch (choice)
        {
        case 1:
        {
            auto todos = todo_service.getAll();

            printTodosTable(todos);
            pauseScreen();
            break;
        }
        case 2:
        {
            printTopPad();
            std::string id_str;
            std::cout << pad << "Enter ID: ";
            std::getline(std::cin, id_str);
            clearScreen();
            printTopPad();
            auto todo = todo_service.getById(id_str);
            if (todo)
            {
                char uid[37];
                uuid_unparse(todo->id, uid);
                std::cout << pad << "┌─ Todo Detail ──────────────────────┐\n"
                          << pad << "│ ID     : " << uid << "\n"
                          << pad << "│ Title  : " << todo->title << "\n"
                          << pad << "│ Desc   : " << todo->description << "\n"
                          << pad << "│ Status : " << todo->status << "\n"
                          << pad << "│ Created: " << toTimestamp(todo->created_at) << "\n"
                          << pad << "└────────────────────────────────────┘\n";
            }
            else
            {
                std::cout << pad << "Todo not found.\n";
            }
            pauseScreen();
            break;
        }
        case 3:
        {
            printTopPad();
            std::string title, description;
            std::cout << pad << "Title: ";
            std::getline(std::cin, title);
            std::cout << pad << "Description: ";
            std::getline(std::cin, description);
            clearScreen();
            printTopPad();
            todo_service.create(title, description)
                ? std::cout << pad << "✓ Todo created.\n"
                : std::cout << pad << "✗ Failed to create todo.\n";
            pauseScreen();
            break;
        }
        case 4:
        {
            printTopPad();
            std::string id_str, title, desc, status;
            std::cout << pad << "Enter ID: ";
            std::getline(std::cin, id_str);
            std::cout << pad << "New title (blank = keep): ";
            std::getline(std::cin, title);
            std::cout << pad << "New description (blank = keep): ";
            std::getline(std::cin, desc);
            std::cout << pad << "New status (blank = keep): ";
            std::getline(std::cin, status);
            clearScreen();
            printTopPad();
            todo_service.update(id_str, title, desc, status)
                ? std::cout << pad << "✓ Todo updated.\n"
                : std::cout << pad << "✗ Failed to update todo.\n";
            pauseScreen();
            break;
        }
        case 5:
        {
            printTopPad();
            std::string id_str;
            std::cout << pad << "Enter ID: ";
            std::getline(std::cin, id_str);
            clearScreen();
            printTopPad();
            todo_service.remove(id_str)
                ? std::cout << pad << "✓ Todo deleted.\n"
                : std::cout << pad << "✗ Failed to delete todo.\n";
            pauseScreen();
            break;
        }
        case 0:
            clearScreen();
            printTopPad();
            std::cout << pad << "Bye!\n";
            return 0;
        default:
            printTopPad();
            std::cout << pad << "✗ Invalid choice.\n";
            pauseScreen();
        }
        clearScreen();
    }
}