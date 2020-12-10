#pragma once
class CellModel
{
public:
    CellModel() : state(EMPTY) //observer
    {}

    CellModel(CellState cs)
        : state(cs)
    {}

public:
    CellState state = EMPTY;
};




