#include <iostream>
#include <windows.h>

using namespace std;

string get_styled_text(string text,
string style);

int board[4][4] = {
    {0, 0, 1, 0},
    {0, 4, 0, 0},
    {0, 0, 2, 0},
    {0, 1, 0, 0},
};

bool editable[4][4];

int cur_r = 0, cur_c = 0;

// 檢查當前行是否有重複數字
void check_horizontal()
{
    for (int i = 0; i < 4; ++i)
    {
        bool seen[5] = {false}; // 1-4 數字檢查
        for (int j = 0; j < 4; ++j)
        {
            int num = board[i][j];
            if (num == 0) continue; // 忽略空格
            if (seen[num])
            {
                cout << "Row " << i << " has duplicate number: " << num << endl;
                return;
            }
            seen[num] = true;
        }
    }
}

// 檢查當前列是否有重複數字
void check_vertical()
{
    for (int j = 0; j < 4; ++j)
    {
        bool seen[5] = {false};
        for (int i = 0; i < 4; ++i)
        {
            int num = board[i][j];
            if (num == 0) continue;
            if (seen[num])
            {
                cout << "Column " << j << " has duplicate number: " << num << endl;
                return;
            }
            seen[num] = true;
        }
    }
}

// 檢查區塊是否有重複數字（2x2區塊）
void check_block()
{
    for (int br = 0; br < 4; br += 2)
    {
        for (int bc = 0; bc < 4; bc += 2)
        {
            bool seen[5] = {false};
            for (int i = br; i < br + 2; ++i)
            {
                for (int j = bc; j < bc + 2; ++j)
                {
                    int num = board[i][j];
                    if (num == 0) continue;
                    if (seen[num])
                    {
                        cout << "Block at (" << br << ", " << bc << ") has duplicate number: " << num << endl;
                        return;
                    }
                    seen[num] = true;
                }
            }
        }
    }
}

// 填入數字到當前游標位置
void fill_number(char num)
{
    if (!editable[cur_r][cur_c]) // 檢查當前格子是否可編輯
    {
        cout << get_styled_text("This cell cannot be modified!", "R") << endl;
        return;
    }


    if (num >= '0' && num <= '4')
    {
        board[cur_r][cur_c] = num - '0'; // 將輸入的字符轉換為整數
        check_horizontal();
        check_vertical();
        check_block();
    }
    else
    {
        cout << get_styled_text("Invalid number!", "R") << endl;
    }
}


// 移動游標
void move_cursor(char c){
    int new_r = cur_r, new_c = cur_c;

    while (true) {
        if (c == 'W' || c == 'w') new_r--;
        if (c == 'S' || c == 's') new_r++;
        if (c == 'A' || c == 'a') new_c--;
        if (c == 'D' || c == 'd') new_c++;

        // 檢查是否超出邊界
        if (new_r < 0 || new_r >= 4 || new_c < 0 || new_c >= 4) break;

        // 如果新位置是可編輯的，則移動光標
        if (editable[new_r][new_c]) {
            cur_r = new_r;
            cur_c = new_c;
            return;
        }

        // 如果移動的方向已到邊界，停止移動
        if ((c == 'W' || c == 'w') && new_r == 0) break;
        if ((c == 'S' || c == 's') && new_r == 3) break;
        if ((c == 'A' || c == 'a') && new_c == 0) break;
        if ((c == 'D' || c == 'd') && new_c == 3) break;
    }
}

// 檢查是否有無效行或列
bool is_invalid(int i, int j)
{
    bool seen_row[5] = {false};
    bool seen_col[5] = {false};

    for (int k = 0; k < 4; ++k)
    {
        int row_num = board[i][k];
        int col_num = board[k][j];

        if (row_num != 0)
        {
            if (seen_row[row_num]) return true;
            seen_row[row_num] = true;
        }

        if (col_num != 0)
        {
            if (seen_col[col_num]) return true;
            seen_col[col_num] = true;
        }
    }
    return false;
}

// 檢查當前行或列是否已完成
bool is_done(int i, int j)
{
    bool seen_row[5] = {false};
    bool seen_col[5] = {false};

    for (int k = 0; k < 4; ++k)
    {
        if (board[i][k] == 0 || board[k][j] == 0)
            return false;

        seen_row[board[i][k]] = true;
        seen_col[board[k][j]] = true;
    }

    for (int n = 1; n <= 4; ++n)
    {
        if (!seen_row[n] || !seen_col[n])
            return false;
    }

    return true;
}

// 檢查是否完成整個遊戲
bool check_win()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (board[i][j] == 0 || !is_done(i, j))
                return false;
        }
    }
    return true;
}


bool is_moving_action(char c)
{
    return (c == 'W' || c == 'w' || c == 'S' || c == 's' ||
c == 'A' || c == 'a' || c == 'D' || c == 'd');
}

bool is_filling_action(char c)
{
    return (c >= '0' && c <= '4');
}

string get_styled_text(string text, string style)
{
    string color = "", font = "";
    for (char c : style)
    {
        if (c == 'R')
            color = "31";
        if (c == 'G')
            color = "32";
        if (c == 'E')
            color = "41";
        if (c == 'C')
            color = "106";
        if (c == 'B')
            font = ";1";
    }
    return "\x1b[" + color + font + "m" + text + "\x1b[0m";
}

void print_board()
{
    // Flush the screen
    cout << "\x1b[2J\x1b[1;1H";

    // Print usage hint.
    cout << get_styled_text("W/A/S/D: ", "B") << "move cursor" << endl;
    cout << get_styled_text("    1-4: ", "B") << "fill in number" << endl;
    cout << get_styled_text("      0: ", "B") << "clear the cell" << endl;
    cout << get_styled_text("      Q: ", "B") << "quit" << endl;
    cout << get_styled_text("      C: ", "B") << "answer" << endl;
    cout << endl;

    // Iterate through and print each cell.
    for (int i = 0; i < 4; ++i)
    {
        // Print horizontal divider.
        if (i && i % 2 == 0)
            cout << "---------------" << endl;

        // Print the first vertical divider in each line.
        cout << "|";
        for (int j = 0; j < 4; ++j)
        {
            // Set text style based on the state of the cell.
            string style = "";

            // Set style for the cell the cursor pointing to.
            if (cur_r == i && cur_c == j)
                style = "C";
            // Set style for the cell in an invalid line.
            else if (is_invalid(i, j))
                style = "E";
            // Set style for the cell in a finished line.
            else if (is_done(i, j))
                style = "G";

            // Set style for a the cell that is immutable.
            if (!editable[i][j])
                style += "B";

            // Print the cell out in styled text.
            // If the content is 0, print a dot, else print the number.
            if (board[i][j] == 0)
                cout << get_styled_text(" · ", style);
            else
                cout << get_styled_text(" " + to_string(board[i][j]) + " ", style);

            // Print the vertical divider for each block.
            if ((j + 1) % 2 == 0)
                cout << "|";
        }
        cout << endl;
    }
}

void initialize()
{
    // Set up styled text for Windows.
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // Mark editable cells
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            editable[i][j] = !board[i][j];

    // Print the initial board out.
    print_board();
}
//答案 
void Answer()
{
    // 填入完整解答
    int ans[4][4] = {
        {3, 2, 1, 4},
        {1, 4, 3, 2},
        {4, 3, 2, 1},
        {2, 1, 4, 3},
    };

    // 將解答複製到遊戲板
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            board[i][j] = ans[i][j];
        }
    }
    int cur_r = 4, cur_c = 4;
    print_board();
}

int main()
{
    char c;
    bool action_ok;

    initialize();
    while (cin >> c)
    {
        action_ok = false;
        if (is_moving_action(c))
        {
            action_ok = true;
            move_cursor(c);
        }

        if (is_filling_action(c))
        {
            action_ok = true;
            fill_number(c);
        }

        if (c == 'Q' || c == 'q')
            break;
		if (c=='C' || c == 'c') {
        	action_ok = true;
			Answer();
			cout << get_styled_text("Here is the correct answer!", "G") << endl;
			break;
			}
        print_board();

        if (check_win())
        {
            cout << "YOU WIN!" << endl;
            break;
        }

        if (!action_ok)
            cout << get_styled_text("!!! Invalid action !!!", "R");
        
    }

    return 0;
}
