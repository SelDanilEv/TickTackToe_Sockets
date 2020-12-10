#pragma once
class PlayerMove
{
    PlayerMove() : playerType(EMPTY)
    {}

    PlayerMove(CellState cs,int row,int col)
        : playerType(cs),row(row),col(col)
    {}

public:
    CellState playerType;
    int row, col;

public:
    void Write(OutputMemoryStream& inStream) const;
    void Read(InputMemoryStream& inStream);
};

