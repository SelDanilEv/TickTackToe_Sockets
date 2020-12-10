#pragma once
class PlayerMove
{
public:
    PlayerMove() : playerType(EMPTY)
    {}

    PlayerMove(CellState cs)
        : playerType(cs)
    {}

public:
    CellState playerType;
    int row, col;

public:
    void Write(OutputMemoryStream& inStream) const;
    void Read(InputMemoryStream& inStream);
};

