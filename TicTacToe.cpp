#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <clocale>
#include <limits>

using namespace std;

// Функция для отображения игрового поля
void printBoard(const vector<vector<char>>& board) {
    cout << "\n  1 2 3\n"; // Номера столбцов
    for (int i = 0; i < 3; ++i) {
        cout << i + 1 << " "; // Номер строки
        for (int j = 0; j < 3; ++j) {
            cout << board[i][j];
            if (j < 2) cout << "|";
        }
        cout << "\n";
        if (i < 2) cout << "  -----\n";
    }
    cout << "\n";
}

// Проверка на победу
bool checkWin(const vector<vector<char>>& board, char player) {
    // Проверка строк и столбцов
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return true;
    }
    // Проверка диагоналей
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return true;
    return false;
}

// Проверка на ничью
bool checkDraw(const vector<vector<char>>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == ' ') return false;
        }
    }
    return true;
}

// Ход игрока
void playerMove(vector<vector<char>>& board, char player) {
    int row, col;
    while (true) {
        cout << "Игрок " << player << ", введите строку (1-3) и столбец (1-3) через пробел: ";
        cin >> row >> col;

        // Проверка на корректность ввода
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод. Пожалуйста, введите числа от 1 до 3.\n";
            continue;
        }

        // Проверка диапазона
        if (row < 1 || row > 3 || col < 1 || col > 3) {
            cout << "Некорректный ввод. Числа должны быть от 1 до 3.\n";
            continue;
        }

        // Проверка, что клетка свободна
        if (board[row - 1][col - 1] != ' ') {
            cout << "Эта клетка уже занята. Выберите другую.\n";
            continue;
        }

        break;
    }
    board[row - 1][col - 1] = player;
}

// Ход компьютера (простая логика)
void computerMove(vector<vector<char>>& board, char computer) {
    // Сначала проверяем, может ли компьютер выиграть следующим ходом
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                board[i][j] = computer;
                if (checkWin(board, computer)) {
                    return;
                }
                board[i][j] = ' ';
            }
        }
    }

    // Затем проверяем, может ли игрок выиграть следующим ходом, и блокируем
    char player = (computer == 'X') ? 'O' : 'X';
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                board[i][j] = player;
                if (checkWin(board, player)) {
                    board[i][j] = computer;
                    return;
                }
                board[i][j] = ' ';
            }
        }
    }

    // Если центр свободен, занимаем его
    if (board[1][1] == ' ') {
        board[1][1] = computer;
        return;
    }

    // Иначе делаем случайный ход
    vector<pair<int, int>> freeCells;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                freeCells.emplace_back(i, j);
            }
        }
    }

    if (!freeCells.empty()) {
        int index = rand() % freeCells.size();
        board[freeCells[index].first][freeCells[index].second] = computer;
    }
}

// Режим игры с компьютером
void playVsComputer() {
    vector<vector<char>> board(3, vector<char>(3, ' '));
    char player, computer;

    // Выбор символа
    cout << "Выберите 'X' или 'O' (X ходит первым): ";
    cin >> player;
    player = toupper(player);
    while (player != 'X' && player != 'O') {
        cout << "Некорректный выбор. Введите 'X' или 'O': ";
        cin >> player;
        player = toupper(player);
    }
    computer = (player == 'X') ? 'O' : 'X';

    // Определение кто ходит первым
    bool playerTurn = (player == 'X');

    printBoard(board);

    while (true) {
        if (playerTurn) {
            playerMove(board, player);
        }
        else {
            cout << "Ход компьютера:\n";
            computerMove(board, computer);
        }

        printBoard(board);

        // Проверка на победу
        if (checkWin(board, playerTurn ? player : computer)) {
            cout << (playerTurn ? "Вы победили!" : "Компьютер победил!") << "\n";
            break;
        }

        // Проверка на ничью
        if (checkDraw(board)) {
            cout << "Ничья!\n";
            break;
        }

        playerTurn = !playerTurn;
    }
}

// Режим игры для двух игроков
void playTwoPlayers() {
    vector<vector<char>> board(3, vector<char>(3, ' '));
    char currentPlayer = 'X';

    printBoard(board);

    while (true) {
        cout << "Игрок " << currentPlayer << ", ваш ход.\n";
        playerMove(board, currentPlayer);

        printBoard(board);

        // Проверка на победу
        if (checkWin(board, currentPlayer)) {
            cout << "Игрок " << currentPlayer << " победил!\n";
            break;
        }

        // Проверка на ничью
        if (checkDraw(board)) {
            cout << "Ничья!\n";
            break;
        }

        // Смена игрока
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }
}

// Главное меню
void mainMenu() {
    while (true) {
        cout << "Крестики-нолики\n";
        cout << "1. Играть против компьютера\n";
        cout << "2. Играть вдвоем\n";
        cout << "3. Выход\n";
        cout << "Выберите режим: ";

        int choice;
        cin >> choice;

        // Проверка на корректность ввода
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод. Пожалуйста, введите число от 1 до 3.\n";
            continue;
        }

        switch (choice) {
        case 1:
            playVsComputer();
            break;
        case 2:
            playTwoPlayers();
            break;
        case 3:
            cout << "До свидания!\n";
            return;
        default:
            cout << "Некорректный выбор. Пожалуйста, введите число от 1 до 3.\n";
        }

        // Очистка доски для новой игры
        cout << "\nНажмите Enter чтобы продолжить...";
        cin.ignore();
        cin.get();
    }
}

int main() {
    // Установка локали для корректного отображения кириллицы
    setlocale(LC_ALL, "Russian");

    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned>(time(nullptr)));

    // Запуск главного меню
    mainMenu();

    return 0;
}