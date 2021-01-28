#pragma once
class PlayerMove // сущность хода который делает игрок
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

