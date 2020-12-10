#pragma once
class GameBoard
{
public:
    const static unsigned int SIZE = 3;
    CellState turn;

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
};

