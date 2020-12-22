#pragma once
class GameBoard
{
public:
    const static unsigned int SIZE = 3;
    CellState turn;
    string message;
    bool isGameOver = false;

    GameBoard()
    {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                cells[i][j] = new CellModel();
            }
        }
        turn = CROSS;
    }

    CellModel* cells[SIZE][SIZE];
public:
    void Write(OutputMemoryStream& inStream) const;
    void Read(InputMemoryStream& inStream);
    void ChangeTurn();

    char GetCharFromCell(int cell) {
        switch (cell)
        {
        case 0:
            return 'X';
        case 1:
            return 'O';
        default:
            return '-';
        }
    }
};

