#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <chrono>
#include "libraries/Matrix.cpp"

//                           unused  cell_value  cell_state
// unsigned char (8 bits) -    00       0000         00

// cell_state
// 00 - Hidden
// 01 - Flagged
// 10 - Revealed

// cell_value
// 0000 - 0 adjacent mines
// 0001 - 1 adjacent mines
// 0010 - 2 adjacent mines
// 0011 - 3 adjacent mines
// 0100 - 4 adjacent mines
// 0101 - 5 adjacent mines
// 0110 - 6 adjacent mines
// 0111 - 7 adjacent mines
// 1000 - 8 adjacent mines
// 1001 - mine

struct Color{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct settings{
    int grid_size;
    Color Hidden_cell_color;
    Color Flagged_cell_color;
    Color No_neighbor_cell_color;
    Color Numbered_cell_color;
    Color Mine_cell_color;
    Color Text_color;
    bool show_time;
    bool show_digs_and_flags;
};

struct statistics{
    unsigned int games_played;
    unsigned int games_won;
    unsigned int fastest_win;
    unsigned int win_streak;
    unsigned int max_win_streak;
    unsigned int total_mines_diffused;
    unsigned int total_time_spent;
};

void resetColor() {
    std::cout << "\033[0m";
}

void setForegroundColor(int r, int g, int b) {
    if (r > 255) r = 255;
    else if (r < 0) r = 0;
    if (g > 255) g = 255;
    else if (g < 0) g = 0;
    if (b > 255) b = 255;
    else if (b < 0) b = 0;
    std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m";
}

void load_settings(settings& sett){
    std::ifstream file("game data/settings.csv");
    std::string line;

    if (getline(file, line)){
        std::stringstream ss(line);
        std::string value;

        getline(ss, value, ',');
        sett.grid_size = stoi(value);

        getline(ss, value, ',');
        sett.Hidden_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Hidden_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Hidden_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Flagged_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Flagged_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Flagged_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.No_neighbor_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.No_neighbor_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.No_neighbor_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Numbered_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Numbered_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Numbered_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Mine_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Mine_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Mine_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Text_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Text_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Text_color.b = stoi(value);

        getline(ss, value, ',');
        sett.show_time = stoi(value);

        getline(ss, value, ',');
        sett.show_digs_and_flags = stoi(value);
    }

    file.close();
}

void save_settings(settings& sett){
    std::ofstream file("game data/settings.csv");
    if (!file.is_open()){
        std::cerr << "failed to open file for writing.\n";
        return;
    }

    file << std::to_string(sett.grid_size) + ",";
    file << std::to_string(sett.Hidden_cell_color.r) + ",";
    file << std::to_string(sett.Hidden_cell_color.g) + ",";
    file << std::to_string(sett.Hidden_cell_color.b) + ",";
    file << std::to_string(sett.Flagged_cell_color.r) + ",";
    file << std::to_string(sett.Flagged_cell_color.g) + ",";
    file << std::to_string(sett.Flagged_cell_color.b) + ",";
    file << std::to_string(sett.No_neighbor_cell_color.r) + ",";
    file << std::to_string(sett.No_neighbor_cell_color.g) + ",";
    file << std::to_string(sett.No_neighbor_cell_color.b) + ",";
    file << std::to_string(sett.Numbered_cell_color.r) + ",";
    file << std::to_string(sett.Numbered_cell_color.g) + ",";
    file << std::to_string(sett.Numbered_cell_color.b) + ",";
    file << std::to_string(sett.Mine_cell_color.r) + ",";
    file << std::to_string(sett.Mine_cell_color.g) + ",";
    file << std::to_string(sett.Mine_cell_color.b) + ",";
    file << std::to_string(sett.Text_color.r) + ",";
    file << std::to_string(sett.Text_color.g) + ",";
    file << std::to_string(sett.Text_color.b) + ",";
    file << std::to_string(sett.show_time) + ",";
    file << std::to_string(sett.show_digs_and_flags) + ",";

    file.close();
}

void load_statistics(statistics& stats){
    std::ifstream file("game data/stats.csv");
    std::string line;

    if (getline(file, line)){
        std::stringstream ss(line);
        std::string value;

        getline(ss, value, ',');
        stats.games_played = stoul(value);
        getline(ss, value, ',');
        stats.games_won = stoul(value);
        getline(ss, value, ',');
        stats.fastest_win = stoul(value);
        getline(ss, value, ',');
        stats.win_streak = stoul(value);
        getline(ss, value, ',');
        stats.max_win_streak = stoul(value);
        getline(ss, value, ',');
        stats.total_mines_diffused = stoul(value);
        getline(ss, value, ',');
        stats.total_time_spent = stoul(value);
    }

    file.close();
}

void save_statistics(statistics& stats){
    std::ofstream file("game data/stats.csv");
    if (!file.is_open()){
        std::cerr << "failed to open file for writing.\n";
        return;
    }

    file << std::to_string(stats.games_played) + ",";
    file << std::to_string(stats.games_won) + ",";
    file << std::to_string(stats.fastest_win) + ",";
    file << std::to_string(stats.win_streak) + ",";
    file << std::to_string(stats.max_win_streak) + ",";
    file << std::to_string(stats.total_mines_diffused) + ",";
    file << std::to_string(stats.total_time_spent) + ",";

    file.close();
}

std::vector<int> getUniqueRandomNumbers(int n, int m) {
    std::vector<int> pool(n + 1);
    
    for (int i = 0; i <= n; ++i) {
        pool[i] = i;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < m; ++i) {
        std::uniform_int_distribution<int> dist(i, n);
        int j = dist(gen);

        int temp = pool[i];
        pool[i] = pool[j];
        pool[j] = temp;
    }

    std::vector<int> result;
    for (int i = 0; i < m; ++i) {
        result.push_back(pool[i]);
    }

    return result;
}

void render_grid(Matrix<unsigned char>& grid, settings& sett, int digs, int flags, int no_of_mines, int time){
    int rows = grid.get_rows();
    int cols = grid.get_cols();
    
    setForegroundColor(sett.Hidden_cell_color.r, sett.Hidden_cell_color.g, sett.Hidden_cell_color.b);
    
    std::cout << "  ";
    for (int i = 0; i < sett.grid_size; i++) std::cout << "  ";
    for (int i = 0; i < cols; i++){
        std::cout << i+1;
        if (i < 9) std::cout << " ";
        for (int j = 0; j < sett.grid_size; j++) std::cout << "  ";
    }
    std::cout << std::endl;
    for (int i = 0; i < sett.grid_size; i++) std::cout << std::endl;

    for (int i = 0; i < rows; i++){
        std::cout << i+1;
        if (i < 9) std::cout << " ";
        for (int j = 0; j < sett.grid_size; j++) std::cout << "  ";
        for (int j = 0; j < cols; j++){
            unsigned char cell_state = grid[i][j] & 3;
            unsigned char cell_value = grid[i][j] >> 2;
            
            if (cell_state == 0) std::cout << "#";
            else if (cell_state == 1){
                setForegroundColor(sett.Flagged_cell_color.r, sett.Flagged_cell_color.g, sett.Flagged_cell_color.b);
                std::cout << "F";
                setForegroundColor(sett.Hidden_cell_color.r, sett.Hidden_cell_color.g, sett.Hidden_cell_color.b);
            }
            else {
                if (cell_value == 0){
                    setForegroundColor(sett.No_neighbor_cell_color.r, sett.No_neighbor_cell_color.g, sett.No_neighbor_cell_color.b);
                    std::cout << ".";
                    setForegroundColor(sett.Hidden_cell_color.r, sett.Hidden_cell_color.g, sett.Hidden_cell_color.b);
                }
                else if (cell_value == 9){
                    setForegroundColor(sett.Mine_cell_color.r, sett.Mine_cell_color.g, sett.Mine_cell_color.b);
                    std::cout << "*";
                    setForegroundColor(sett.Hidden_cell_color.r, sett.Hidden_cell_color.g, sett.Hidden_cell_color.b);
                }
                else{
                    setForegroundColor(sett.Numbered_cell_color.r, sett.Numbered_cell_color.g, sett.Numbered_cell_color.b);
                    std::cout << int(cell_value);
                    setForegroundColor(sett.Hidden_cell_color.r, sett.Hidden_cell_color.g, sett.Hidden_cell_color.b);
                }
            }
            std::cout << " ";
            for (int k = 0; k < sett.grid_size; k++) std::cout << "  ";
        }
        if (i < rows-1){
            std::cout << std::endl;
            for (int j = 0; j < sett.grid_size; j++) std::cout << std::endl;
        }
    }
    std::cout << std::endl << std::endl;
    setForegroundColor(sett.Text_color.r, sett.Text_color.g, sett.Text_color.b);
    if (sett.show_digs_and_flags) std::cout << "Digs: " << digs << ", Flags: " << flags << ", ";
    std::cout << "Mines: " << no_of_mines << std::endl;
    if (sett.show_time) std::cout << "Time: " << time << " seconds" << std::endl;
    std::cout << std::endl;

    resetColor();
}

std::vector<int> get_neighbors(int rows, int cols, int cell_no){
    std::vector<int> neighbors;

    bool condition1 = cell_no >= cols;
    bool condition2 = cell_no < (rows*cols)-cols;
    bool condition3 = cell_no % cols != 0;
    bool condition4 = (cell_no+1) % cols != 0;
    
    if (condition1){
        if (condition3) neighbors.push_back(cell_no-cols-1);
        neighbors.push_back(cell_no-cols);
        if (condition4) neighbors.push_back(cell_no-cols+1);
    }
    if (condition3) neighbors.push_back(cell_no-1);
    if (condition4) neighbors.push_back(cell_no+1);
    if (condition2){
        if (condition3) neighbors.push_back(cell_no+cols-1);
        neighbors.push_back(cell_no+cols);
        if (condition4) neighbors.push_back(cell_no+cols+1);
    }

    return neighbors;
}

bool is_revealed(unsigned char cell){
    if ((cell & 3) == 2) return true;
    return false;
}

void reveal_cell(unsigned char& cell){
    if (!is_revealed(cell)) cell += 2;
}

void reveal_cells(int position, Matrix<unsigned char>& grid){
    unsigned char row_index = position/grid.get_cols();
    unsigned char col_index = position%grid.get_cols();
    reveal_cell(grid[row_index][col_index]);

    unsigned char cell_value = grid[row_index][col_index] >> 2;

    if (cell_value == 0){
        std::vector<int> neighbors = get_neighbors(grid.get_rows(), grid.get_cols(), position);
        for (int i = 0; i < neighbors.size(); i++){
            if (!is_revealed(grid[neighbors[i]/grid.get_cols()][neighbors[i]%grid.get_cols()])) reveal_cells(neighbors[i], grid);
        }
    }
}

bool check_win(Matrix<unsigned char>& grid){
    for (int i = 0; i < grid.get_rows(); i++){
        for (int j = 0; j < grid.get_cols(); j++){
            if ((!is_revealed(grid[i][j])) && ((grid[i][j] >> 2) != 9)) return false;
        }
    }
    return true;
}

void play_game(int rows, int cols, int no_of_mines, bool is_custom, settings& sett, statistics& stats){
    Matrix<unsigned char> grid(rows, cols, 0);
    std::cout << std::endl;

    render_grid(grid, sett, 0, 0, no_of_mines, 0);

    setForegroundColor(sett.Text_color.r, sett.Text_color.g, sett.Text_color.b);
    std::cout << "Enter 0 to quit." << std::endl;

    int user_input;
    int row_no, col_no;
    while (true){
        std::cout << "Enter row number: ";
        std::cin >> user_input;
        if (std::cin.fail() || user_input < 0 || user_input > rows){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter a number from 0 to " << rows << "!" << std::endl;
            continue;
        }
        if (user_input == 0){
            while(true){
                std::cout << "Do you really want to quit (yes: 1, no: 0)?";
                std::cin >> user_input;
                if (std::cin.fail() || user_input < 0 || user_input > 1){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter 0 or 1 (yes: 1, no: 0)!"<< std::endl;
                    continue;
                }
                if (user_input == 1) return;
                else break;
            }
        }
        else{
            row_no = user_input;
            break;
        }
    }
    
    while (true){
        std::cout << "Enter column number: ";
        std::cin >> user_input;
        if (std::cin.fail() || user_input < 0 || user_input > cols){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter a number from 0 to " << cols << "!" << std::endl;
            continue;
        }
        if (user_input == 0){
            while(true){
                std::cout << "Do you really want to quit (yes: 1, no: 0)?";
                std::cin >> user_input;
                if (std::cin.fail() || user_input < 0 || user_input > 1){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter 0 or 1 (yes: 1, no: 0)!"<< std::endl;
                    continue;
                }
                if (user_input == 1) return;
                else break;
            }
        }
        else{
            col_no = user_input;
            break;
        }
    }

    int first_cell_position = ((row_no-1)*cols) + (col_no-1);
    std::vector<int> cells_to_ignore;
    {
        std::vector<int> neighbors = get_neighbors(rows, cols, first_cell_position);
        if (first_cell_position == 0){
            cells_to_ignore.push_back(first_cell_position);
            for (int i = 0; i < neighbors.size(); i++){
                cells_to_ignore.push_back(neighbors[i]);
            }
        }
        else if (first_cell_position == rows*cols-1){
            cells_to_ignore = neighbors;
            cells_to_ignore.push_back(first_cell_position);
        }
        else{
            for (int i = 0; i < neighbors.size(); i++){
                if (neighbors[i] > first_cell_position){
                    if (neighbors[i-1] < first_cell_position) cells_to_ignore.push_back(first_cell_position);
                }
                cells_to_ignore.push_back(neighbors[i]);
            }
        }
    }
    
    std::vector<int> position_of_mines = getUniqueRandomNumbers((rows * cols) - 1 - cells_to_ignore.size(), no_of_mines);
    for (int i = 0; i < position_of_mines.size(); i++){
        for (int j = 0; j < cells_to_ignore.size(); j++){
            if (position_of_mines[i] >= cells_to_ignore[j]) position_of_mines[i]++;
        }
        
        grid[position_of_mines[i]/cols][position_of_mines[i]%cols] += 36;
    }
    
    {
        std::vector<int> neighbors;
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                if (grid[i][j] >> 2 != 9){
                    neighbors = get_neighbors(rows, cols, i*cols+j);
                    unsigned char adjacent_mines = 0;
                    for (int k = 0; k < neighbors.size(); k++){
                        unsigned char cell_value = grid[neighbors[k]/cols][neighbors[k]%cols] >> 2;
                        if (cell_value == 9) adjacent_mines++;
                    }
                    grid[i][j] += adjacent_mines*4;
                }
            }
        }
    }

    reveal_cells(first_cell_position, grid);

    unsigned int win_streak = stats.win_streak;
    unsigned int total_time_spent = stats.total_time_spent;
    if (!is_custom){
        stats.games_played++;
        stats.win_streak = 0;
        save_statistics(stats);
    }

    using Clock = std::chrono::high_resolution_clock;
    auto start_time = Clock::now();
    auto current_time = Clock::now();
    std::chrono::duration<double> elapsed = current_time - start_time;
    double seconds_played = elapsed.count();
    
    render_grid(grid, sett, 1, 0, no_of_mines, seconds_played);
    std::string mode = "dig";
    bool has_won = false;
    int digs = 1, flags = 0;
    while (true){
        setForegroundColor(sett.Text_color.r, sett.Text_color.g, sett.Text_color.b);
        std::cout << "Enter 0 to quit." << std::endl;
        std::cout << "Enter -1 to switch (dig / flag).   Current mode: " << mode << std::endl;

        int user_input;
        int row_no, col_no;
        while (true){
            std::cout << std::endl;
            std::cout << "Enter row number: ";
            std::cin >> user_input;
            if (std::cin.fail() || user_input < -1 || user_input > rows){
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Enter a number from -1 to " << rows << ": " << std::endl;
                continue;
            }
            if (user_input == -1){
                if (mode == "dig") mode = "flag";
                else mode = "dig";
                std::cout << "Mode changed to: " << mode << std::endl;
            }
            else if (user_input == 0){
                while(true){
                    std::cout << "Do you really want to quit (yes: 1, no: 0)?";
                    std::cin >> user_input;
                    if (std::cin.fail() || user_input < 0 || user_input > 1){
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Enter 0 or 1 (yes: 1, no: 0)!"<< std::endl;
                        continue;
                    }
                    if (user_input == 1) return;
                    else break;
                }
            }
            else{
                row_no = user_input;
                break;
            }
        }

        while (true){
            std::cout << "Enter column number: ";
            std::cin >> user_input;
            if (std::cin.fail() || user_input < -1 || user_input > cols){
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Enter a number from -1 to " << cols << ": " << std::endl;
                continue;
            }
            if (user_input == -1){
                if (mode == "dig") mode = "flag";
                else mode = "dig";
                std::cout << "Mode changed to: " << mode << std::endl;
            }
            else if (user_input == 0){
                while(true){
                    std::cout << "Do you really want to quit (yes: 1, no: 0)?";
                    std::cin >> user_input;
                    if (std::cin.fail() || user_input < 0 || user_input > 1){
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Enter 0 or 1 (yes: 1, no: 0)!"<< std::endl;
                        continue;
                    }
                    if (user_input == 1) return;
                    else break;
                }
            }
            else{
                col_no = user_input;
                break;
            }
        }

        current_time = Clock::now();
        elapsed = current_time - start_time;
        seconds_played = elapsed.count();

        if (mode == "dig"){
            if (is_revealed(grid[row_no-1][col_no-1])){
                std::cout << "Cell already revealed! Please enter coordinates of a hidden cell!" << std::endl;
                continue;
            }
            else if ((grid[row_no-1][col_no-1] & 3) == 1){
                std::cout << "Cell flagged! Please enter coordinates of a hidden cell!" << std::endl;
                continue;
            }
            else if ((grid[row_no-1][col_no-1] >> 2) == 9){
                for (int i = 0; i < position_of_mines.size(); i++){
                    reveal_cell(grid[position_of_mines[i]/cols][position_of_mines[i]%cols]);
                }
                digs++;
                render_grid(grid, sett, digs, flags, no_of_mines, seconds_played);
                setForegroundColor(sett.Text_color.r, sett.Text_color.g, sett.Text_color.b);
                std::cout << "You Lost!" << std::endl;
                break;
            }
            else{
                reveal_cells(((row_no-1)*cols) + (col_no-1), grid);
                digs++;
            }
        }
        else{
            if (is_revealed(grid[row_no-1][col_no-1])){
                std::cout << "Revealed cell can't be flagged! Please enter coordinates of a hidden cell!" << std::endl;
                continue;
            }
            else{
                if ((grid[row_no-1][col_no-1] & 3) == 1){
                    grid[row_no-1][col_no-1]--;
                    flags--;
                }
                else{
                    grid[row_no-1][col_no-1]++;
                    flags++;
                }
            }
        }

        if (check_win(grid)){
            for (int i = 0; i < position_of_mines.size(); i++){
                reveal_cell(grid[position_of_mines[i]/cols][position_of_mines[i]%cols]);
            }
            render_grid(grid, sett, digs, flags, no_of_mines, seconds_played);
            setForegroundColor(sett.Text_color.r, sett.Text_color.g, sett.Text_color.b);
            std::cout << "You Won!" << std::endl;
            has_won = true;
            break;
        }

        render_grid(grid, sett, digs, flags, no_of_mines, seconds_played);
        if (!is_custom){
            stats.total_time_spent = total_time_spent + seconds_played;
            save_statistics(stats);
        }
    }

    if (!is_custom){
        if (has_won){
            stats.games_won++;
            if (stats.fastest_win > seconds_played) stats.fastest_win = seconds_played;
            stats.win_streak = win_streak+1;
            if (stats.win_streak > stats.max_win_streak) stats.max_win_streak = stats.win_streak;
            stats.total_mines_diffused += no_of_mines;
        }
        stats.total_time_spent = total_time_spent + seconds_played;

        save_statistics(stats);
    }
}

void start_game(settings& sett, statistics& stats){
    while(true){
        setForegroundColor(sett.Text_color.r + (255-sett.Text_color.r)/2, sett.Text_color.g/2, sett.Text_color.b/2);
        std::cout << std::endl;
    
        std::cout << "Choose difficulty" << std::endl;
        std::cout << "1. Easy   ( 8 X 8  - 10 mines)" << std::endl;
        std::cout << "2. Medium (16 X 16 - 40 mines)" << std::endl;
        std::cout << "3. Hard   (16 X 30 - 99 mines)" << std::endl;
        std::cout << "4. Custom (Stats are not updated in custom mode!)" << std::endl;
        std::cout << "5. Go Back" << std::endl;
    
        std::string user_input;
        std::cout << "Enter: ";
        std::cin >> user_input;

        if (user_input == "1") play_game(8, 8, 10, false, sett, stats);
        else if (user_input == "2") play_game(16, 16, 40, false, sett, stats);
        else if (user_input == "3") play_game(16, 30, 99, false, sett, stats);
        else if (user_input == "4"){
            setForegroundColor(0, 180, 238);
            std::cout << std::endl;

            int rows, cols, mines;
            while(true){
                std::cout << "Enter no of rows: ";
                std::cin >> rows;
                if (std::cin.fail() || rows < 8 || rows > 32){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 8 to 32!" << std::endl;
                    continue;
                }
                break;
            }
            while(true){
                std::cout << "Enter no of cols: ";
                std::cin >> cols;
                if (std::cin.fail() || cols < 8 || cols > 32){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 8 to 32!" << std::endl;
                    continue;
                }
                break;
            }
            int max_no_of_mines = int((rows * cols) / 3);
            while(true){
                std::cout << "Enter no of mines: ";
                std::cin >> mines;
                if (std::cin.fail() || mines < 5 || mines > max_no_of_mines){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number between 5 and " << max_no_of_mines << "!" << std::endl;
                    continue;
                }
                break;
            }
            play_game(rows, cols, mines, true, sett, stats);
        }
        else if (user_input == "5") break;
        else std::cout << "Enter a valid input!" << std::endl;
    }

    resetColor();
}

void show_presets(){
    std::cout << std::endl;
    std::cout << "Saved presets:" << std::endl;
    std::ifstream file("game data/user_presets.csv");
    std::string line;

    int preset_number = 0;
    while (getline(file, line)){
        preset_number++;

        std::stringstream ss(line);
        std::string value;

        for (int i = 0; i < 22; i++){
            getline(ss, value, ',');
        }

        std::cout << preset_number << ". " << value << std::endl;
    }

    file.close();
}

void change_settings(settings& sett){
    std::string grid_sizes[3] = {"Small", "Medium", "Large"};
    std::string yes_no[2] = {"No", "Yes"};
    bool changes_saved = true;

    while (true){
        std::cout << std::endl;
        std::cout << "1. Change Grid Size" << std::endl;
        std::cout << "2. Change Hidden Cell Color" << std::endl;
        std::cout << "3. Change Flagged Cell Color" << std::endl;
        std::cout << "4. Change No Neighbor Cell Color" << std::endl;
        std::cout << "5. Change Numbered Cell Color" << std::endl;
        std::cout << "6. Change Mine Cell Color" << std::endl;
        std::cout << "7. Change Text Color" << std::endl;
        std::cout << "8. Set Show Time to " << yes_no[!sett.show_time] << std::endl;
        std::cout << "9. Set Show Digs and Flags to " << yes_no[!sett.show_digs_and_flags] << std::endl;
        std::cout << "10. Save Changes" << std::endl;
        std::cout << "11. Go back" << std::endl;

        std::string user_input;
        std::cout << "Enter: ";
        std::cin >> user_input;

        if (user_input == "1"){
            std::string user_input2;
            while (true){
                std::cout << "1. Small" << std::endl;
                std::cout << "2. Medium" << std::endl;
                std::cout << "3. Large" << std::endl;
                
                std::cout << "Enter: ";
                std::cin >> user_input2;
                
                if (user_input2 == "1") sett.grid_size = 0;
                else if (user_input2 == "2") sett.grid_size = 1;
                else if (user_input2 == "3") sett.grid_size = 2;
                else{
                    std::cout << "Enter a valid input!" << std::endl;
                    continue;
                }
                break;
            }

            changes_saved = false;
        }
        else if (user_input == "2"){
            int R, G, B;
            while (true){
                std::cout << "Enter value for R: ";
                std::cin >> R;
                if (std::cin.fail() || R < 0 || R > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            while (true){
                std::cout << "Enter value for G: ";
                std::cin >> G;
                if (std::cin.fail() || G < 0 || G > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            while (true){
                std::cout << "Enter value for B: ";
                std::cin >> B;
                if (std::cin.fail() || B < 0 || B > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            sett.Hidden_cell_color.r = R;
            sett.Hidden_cell_color.g = G;
            sett.Hidden_cell_color.b = B;
            changes_saved = false;
        }
        else if (user_input == "3"){
            int R, G, B;
            while (true){
                std::cout << "Enter value for R: ";
                std::cin >> R;
                if (std::cin.fail() || R < 0 || R > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            while (true){
                std::cout << "Enter value for G: ";
                std::cin >> G;
                if (std::cin.fail() || G < 0 || G > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            while (true){
                std::cout << "Enter value for B: ";
                std::cin >> B;
                if (std::cin.fail() || B < 0 || B > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            sett.Flagged_cell_color.r = R;
            sett.Flagged_cell_color.g = G;
            sett.Flagged_cell_color.b = B;
            changes_saved = false;
        }
        else if (user_input == "4"){
            int R, G, B;
            while (true){
                std::cout << "Enter value for R: ";
                std::cin >> R;
                if (std::cin.fail() || R < 0 || R > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            while (true){
                std::cout << "Enter value for G: ";
                std::cin >> G;
                if (std::cin.fail() || G < 0 || G > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            while (true){
                std::cout << "Enter value for B: ";
                std::cin >> B;
                if (std::cin.fail() || B < 0 || B > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            sett.No_neighbor_cell_color.r = R;
            sett.No_neighbor_cell_color.g = G;
            sett.No_neighbor_cell_color.b = B;
            changes_saved = false;
        }
        else if (user_input == "5"){
            int R, G, B;
            while (true){
                std::cout << "Enter value for R: ";
                std::cin >> R;
                if (std::cin.fail() || R < 0 || R > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            while (true){
                std::cout << "Enter value for G: ";
                std::cin >> G;
                if (std::cin.fail() || G < 0 || G > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            while (true){
                std::cout << "Enter value for B: ";
                std::cin >> B;
                if (std::cin.fail() || B < 0 || B > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            sett.Numbered_cell_color.r = R;
            sett.Numbered_cell_color.g = G;
            sett.Numbered_cell_color.b = B;
            changes_saved = false;
        }
        else if (user_input == "6"){
            int R, G, B;
            while (true){
                std::cout << "Enter value for R: ";
                std::cin >> R;
                if (std::cin.fail() || R < 0 || R > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            while (true){
                std::cout << "Enter value for G: ";
                std::cin >> G;
                if (std::cin.fail() || G < 0 || G > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            while (true){
                std::cout << "Enter value for B: ";
                std::cin >> B;
                if (std::cin.fail() || B < 0 || B > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            sett.Mine_cell_color.g = G;
            sett.Mine_cell_color.b = B;
            sett.Mine_cell_color.r = R;
            changes_saved = false;
        }
        else if (user_input == "7"){
            int R, G, B;
            while (true){
                std::cout << "Enter value for R: ";
                std::cin >> R;
                if (std::cin.fail() || R < 0 || R > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            while (true){
                std::cout << "Enter value for G: ";
                std::cin >> G;
                if (std::cin.fail() || G < 0 || G > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            while (true){
                std::cout << "Enter value for B: ";
                std::cin >> B;
                if (std::cin.fail() || B < 0 || B > 255){
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Enter a number from 0 to 255!" << std::endl;
                    continue;
                }
                break;
            }
            sett.Text_color.r = R;
            sett.Text_color.g = G;
            sett.Text_color.b = B;
            changes_saved = false;
        }
        else if (user_input == "8"){
            sett.show_time = !sett.show_time;
            changes_saved = false;
        }
        else if (user_input == "9"){
            sett.show_digs_and_flags = !sett.show_digs_and_flags;
            changes_saved = false;
        }
        else if (user_input == "10"){
            save_settings(sett);
            changes_saved = true;
        }
        else if (user_input == "11"){
            if (!changes_saved){
                while (true){
                    std::cout << "Do you want to save changes (yes: 1, no: 0)?";
                    std::string user_input2;
                    std::cin >> user_input2;

                    if (user_input2 == "1") save_settings(sett);
                    else if (user_input2 != "0"){
                        std::cout << "Enter 0 or 1 (yes: 1, no: 0)!"<< std::endl;
                        continue;
                    }
                    break;
                }
            }
            break;
        }
        else std::cout << "Enter a valid input!" << std::endl;
    }
}

void apply_preset(settings& SETT, std::string preset_name){
    settings sett;
    bool is_present = false;

    std::ifstream file("game data/user_presets.csv");
    std::string line;

    while (getline(file, line)){
        std::stringstream ss(line);
        std::string value;

        getline(ss, value, ',');
        sett.grid_size = stoi(value);

        getline(ss, value, ',');
        sett.Hidden_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Hidden_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Hidden_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Flagged_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Flagged_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Flagged_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.No_neighbor_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.No_neighbor_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.No_neighbor_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Numbered_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Numbered_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Numbered_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Mine_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Mine_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Mine_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Text_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Text_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Text_color.b = stoi(value);

        getline(ss, value, ',');
        sett.show_time = stoi(value);

        getline(ss, value, ',');
        sett.show_digs_and_flags = stoi(value);

        getline(ss, value, ',');
        if (value == preset_name){
            is_present = true;
            break;
        }
    }

    file.close();

    if (is_present){
        SETT = sett;
        save_settings(SETT);
        std::cout << preset_name + " applied successfully" << std::endl;
    }
    else{
        std::cout << preset_name + " not found!" << std::endl;
    }
}

void save_settings_as_preset(settings& SETT, std::string preset_name){
    std::vector<settings> presets;
    std::vector<std::string> names;
    bool is_present = false;

    std::ifstream file("game data/user_presets.csv");
    std::string line;

    settings sett;
    while (getline(file, line)){
        std::stringstream ss(line);
        std::string value;

        getline(ss, value, ',');
        sett.grid_size = stoi(value);

        getline(ss, value, ',');
        sett.Hidden_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Hidden_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Hidden_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Flagged_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Flagged_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Flagged_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.No_neighbor_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.No_neighbor_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.No_neighbor_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Numbered_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Numbered_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Numbered_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Mine_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Mine_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Mine_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Text_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Text_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Text_color.b = stoi(value);

        getline(ss, value, ',');
        sett.show_time = stoi(value);

        getline(ss, value, ',');
        sett.show_digs_and_flags = stoi(value);

        getline(ss, value, ',');
        if (value == preset_name){
            is_present = true;
            break;
        }
        names.push_back(value);
        presets.push_back(sett);
    }

    file.close();    

    if(is_present){
        std::cout << "Preset name already in use!" << std::endl;
        return;
    }

    presets.push_back(SETT);
    names.push_back(preset_name);
    
    {
        std::ofstream file("game data/user_presets.csv");
        if (!file.is_open()){
            std::cerr << "failed to open file for writing.\n";
            return;
        }

        settings sett;
        for (int i = 0; i < presets.size(); i++){
            sett = presets[i];

            file << std::to_string(sett.grid_size) + ",";
            file << std::to_string(sett.Hidden_cell_color.r) + ",";
            file << std::to_string(sett.Hidden_cell_color.g) + ",";
            file << std::to_string(sett.Hidden_cell_color.b) + ",";
            file << std::to_string(sett.Flagged_cell_color.r) + ",";
            file << std::to_string(sett.Flagged_cell_color.g) + ",";
            file << std::to_string(sett.Flagged_cell_color.b) + ",";
            file << std::to_string(sett.No_neighbor_cell_color.r) + ",";
            file << std::to_string(sett.No_neighbor_cell_color.g) + ",";
            file << std::to_string(sett.No_neighbor_cell_color.b) + ",";
            file << std::to_string(sett.Numbered_cell_color.r) + ",";
            file << std::to_string(sett.Numbered_cell_color.g) + ",";
            file << std::to_string(sett.Numbered_cell_color.b) + ",";
            file << std::to_string(sett.Mine_cell_color.r) + ",";
            file << std::to_string(sett.Mine_cell_color.g) + ",";
            file << std::to_string(sett.Mine_cell_color.b) + ",";
            file << std::to_string(sett.Text_color.r) + ",";
            file << std::to_string(sett.Text_color.g) + ",";
            file << std::to_string(sett.Text_color.b) + ",";
            file << std::to_string(sett.show_time) + ",";
            file << std::to_string(sett.show_digs_and_flags) + ",";

            file << names[i] + "\n";
        }

        file.close();
    }

    std::cout << preset_name + " saved successfully." << std::endl;
}

void delete_preset(std::string preset_name){
    std::vector<settings> presets;
    std::vector<std::string> names;
    bool is_present = false;

    std::ifstream file("game data/user_presets.csv");
    std::string line;

    settings sett;
    while (getline(file, line)){
        std::stringstream ss(line);
        std::string value;

        getline(ss, value, ',');
        sett.grid_size = stoi(value);

        getline(ss, value, ',');
        sett.Hidden_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Hidden_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Hidden_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Flagged_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Flagged_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Flagged_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.No_neighbor_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.No_neighbor_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.No_neighbor_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Numbered_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Numbered_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Numbered_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Mine_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Mine_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Mine_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Text_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Text_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Text_color.b = stoi(value);

        getline(ss, value, ',');
        sett.show_time = stoi(value);

        getline(ss, value, ',');
        sett.show_digs_and_flags = stoi(value);

        getline(ss, value, ',');
        if (value == preset_name){
            is_present = true;
            continue;
        }
        names.push_back(value);
        presets.push_back(sett);
    }

    file.close();    

    if (is_present){
        std::ofstream file("game data/user_presets.csv");
        if (!file.is_open()){
            std::cerr << "failed to open file for writing.\n";
            return;
        }

        settings sett;
        for (int i = 0; i < presets.size(); i++){
            sett = presets[i];

            file << std::to_string(sett.grid_size) + ",";
            file << std::to_string(sett.Hidden_cell_color.r) + ",";
            file << std::to_string(sett.Hidden_cell_color.g) + ",";
            file << std::to_string(sett.Hidden_cell_color.b) + ",";
            file << std::to_string(sett.Flagged_cell_color.r) + ",";
            file << std::to_string(sett.Flagged_cell_color.g) + ",";
            file << std::to_string(sett.Flagged_cell_color.b) + ",";
            file << std::to_string(sett.No_neighbor_cell_color.r) + ",";
            file << std::to_string(sett.No_neighbor_cell_color.g) + ",";
            file << std::to_string(sett.No_neighbor_cell_color.b) + ",";
            file << std::to_string(sett.Numbered_cell_color.r) + ",";
            file << std::to_string(sett.Numbered_cell_color.g) + ",";
            file << std::to_string(sett.Numbered_cell_color.b) + ",";
            file << std::to_string(sett.Mine_cell_color.r) + ",";
            file << std::to_string(sett.Mine_cell_color.g) + ",";
            file << std::to_string(sett.Mine_cell_color.b) + ",";
            file << std::to_string(sett.Text_color.r) + ",";
            file << std::to_string(sett.Text_color.g) + ",";
            file << std::to_string(sett.Text_color.b) + ",";
            file << std::to_string(sett.show_time) + ",";
            file << std::to_string(sett.show_digs_and_flags) + ",";

            file << names[i] + "\n";
        }

        file.close();
        std::cout << preset_name + " deleted successfully." << std::endl;
    }
    else std::cout << preset_name + " not found!" << std::endl;
}

void reset_to_default_settings(settings& sett){
    std::ifstream file("game data/default_settings.csv");
    std::string line;

    if (getline(file, line)){
        std::stringstream ss(line);
        std::string value;

        getline(ss, value, ',');
        sett.grid_size = stoi(value);

        getline(ss, value, ',');
        sett.Hidden_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Hidden_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Hidden_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Flagged_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Flagged_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Flagged_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.No_neighbor_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.No_neighbor_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.No_neighbor_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Numbered_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Numbered_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Numbered_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Mine_cell_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Mine_cell_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Mine_cell_color.b = stoi(value);

        getline(ss, value, ',');
        sett.Text_color.r = stoi(value);
        getline(ss, value, ',');
        sett.Text_color.g = stoi(value);
        getline(ss, value, ',');
        sett.Text_color.b = stoi(value);

        getline(ss, value, ',');
        sett.show_time = stoi(value);

        getline(ss, value, ',');
        sett.show_digs_and_flags = stoi(value);
    }

    file.close();

    save_settings(sett);

    std::cout << "Default settings restored successfully." << std::endl;
}

void show_settings(settings& sett){
    setForegroundColor(sett.Text_color.r + (255-sett.Text_color.r)/2, sett.Text_color.g/2, sett.Text_color.b/2);
    std::cout << std::endl;

    std::string grid_sizes[3] = {"Small", "Medium", "Large"};
    std::string yes_no[2] = {"No", "Yes"};

    std::cout << "Grid Size: " << grid_sizes[sett.grid_size] << std::endl;
    std::cout << "                          R   G   B" << std::endl;
    std::cout << "Hidden Cell Color      : " << std::setw(3) << int(sett.Hidden_cell_color.r) << std::setw(4) << int(sett.Hidden_cell_color.g) << std::setw(4) << int(sett.Hidden_cell_color.b) << std::endl;
    std::cout << "Flagged Cell Color     : " << std::setw(3) << int(sett.Flagged_cell_color.r) << std::setw(4) << int(sett.Flagged_cell_color.g) << std::setw(4) << int(sett.Flagged_cell_color.b) << std::endl;
    std::cout << "No Neighbor Cell Color : " << std::setw(3) << int(sett.No_neighbor_cell_color.r) << std::setw(4) << int(sett.No_neighbor_cell_color.g) << std::setw(4) << int(sett.No_neighbor_cell_color.b) << std::endl;
    std::cout << "Numbered Cell Color    : " << std::setw(3) << int(sett.Numbered_cell_color.r) << std::setw(4) << int(sett.Numbered_cell_color.g) << std::setw(4) << int(sett.Numbered_cell_color.b) << std::endl;
    std::cout << "Mine Cell Color        : " << std::setw(3) << int(sett.Mine_cell_color.r) << std::setw(4) << int(sett.Mine_cell_color.g) << std::setw(4) << int(sett.Mine_cell_color.b) << std::endl;
    std::cout << "Text Color             : " << std::setw(3) << int(sett.Text_color.r) << std::setw(4) << int(sett.Text_color.g) << std::setw(4) << int(sett.Text_color.b) << std::endl;
    std::cout << "Show Time           : " << yes_no[sett.show_time] << std::endl;
    std::cout << "Show Digs and Flags : " << yes_no[sett.show_digs_and_flags] << std::endl;

    while(true){
        std::cout << std::endl;
        std::cout << "1. Make changes" << std::endl;
        std::cout << "2. Apply a preset" << std::endl;
        std::cout << "3. Save current settings as a preset" << std::endl;
        std::cout << "4. Delete a preset" << std::endl;
        std::cout << "5. Reset to default settings" << std::endl;
        std::cout << "6. Go back" << std::endl;
        
        std::string user_input;
        std::cout << "Enter: ";
        std::cin >> user_input;

        if (user_input == "1") change_settings(sett);
        else if (user_input == "2"){
            show_presets();

            std::string preset_name;
            std::cout << "Enter name of preset: ";
            std::cin >> preset_name;

            apply_preset(sett, preset_name);
        }
        else if (user_input == "3"){
            show_presets();

            std::string preset_name;
            std::cout << "Enter name of preset: ";
            std::cin >> preset_name;

            save_settings_as_preset(sett, preset_name);
        }
        else if (user_input == "4"){
            show_presets();

            std::string preset_name;
            std::cout << "Enter name of preset: ";
            std::cin >> preset_name;

            delete_preset(preset_name);
        }
        else if (user_input == "5") reset_to_default_settings(sett);
        else if (user_input == "6") break;
        else std::cout << "Enter a valid input!" << std::endl;
    }

    resetColor();
}

void reset_statistics(statistics& stats){
    stats.games_played = 0;
    stats.games_won = 0;
    stats.fastest_win = 4294967295;
    stats.win_streak = 0;
    stats.max_win_streak = 0;
    stats.total_mines_diffused = 0;
    stats.total_time_spent = 0;
    
    save_statistics(stats);
}

void show_statistics(settings& sett, statistics& stats){
    setForegroundColor(sett.Text_color.r + (255-sett.Text_color.r)/2, sett.Text_color.g/2, sett.Text_color.b/2);
    std::cout << std::endl;

    std::cout << "Games Played             : " << stats.games_played << std::endl;
    std::cout << "Games Won                : " << stats.games_won << std::endl;
    if (stats.games_played == 0) std::cout << "Win Rate                 : 0 %" << std::endl;
    else std::cout << "Win Rate                 : " << (double(stats.games_won) / double(stats.games_played)) * 100 << " %" << std::endl;
    std::cout << "Fastest Win              : " << stats.fastest_win << " seconds" << std::endl;
    std::cout << "Win Streak               : " << stats.win_streak << std::endl;
    std::cout << "Max Win Streak           : " << stats.max_win_streak << std::endl;
    std::cout << "Total Mines Diffused     : " << stats.total_mines_diffused << std::endl;
    std::cout << "Total Time Spent Playing : " << stats.total_time_spent << " seconds" << std::endl;

    while (true){
        std::cout << std::endl;
        std::cout << "1. Reset stats" << std::endl;
        std::cout << "2. Go back" << std::endl;
        
        std::string user_input;
        std::cout << "Enter: ";
        std::cin >> user_input;
        
        if (user_input == "1"){
            while (true){
                std::string user_input2;
                std::cout << "Do you really want to reset stats (yes: 1, no: 0)? ";
                std::cin >> user_input2;

                if (user_input2 == "1") reset_statistics(stats);
                else if (user_input2 != "0"){
                    std::cout << "Enter 0 or 1!" << std::endl;
                    continue;
                }
                break;
            }
        }
        else if (user_input == "2") break;
        else std::cout << "Enter a valid input!" << std::endl;
    }

    resetColor();
}

void run_application(){
    settings sett;
    statistics stats;
    
    load_settings(sett);
    load_statistics(stats);
    
    while (true){
        setForegroundColor(sett.Text_color.r, sett.Text_color.g, sett.Text_color.b);
        std::cout << std::endl;

        std::cout << "1. Start game" << std::endl;
        std::cout << "2. Settings" << std::endl;
        std::cout << "3. Statistics" << std::endl;
        std::cout << "4. Quit" << std::endl;
        
        std::string user_input;
        std::cout << "Enter: ";
        std::cin >> user_input;
        
        if (user_input == "1") start_game(sett, stats);
        else if (user_input == "2") show_settings(sett);
        else if (user_input == "3") show_statistics(sett, stats);
        else if (user_input == "4") break;
        else std::cout << "Enter a valid input!" << std::endl;
    }

    resetColor();
}

int main(){
    run_application();

    return 0;
}