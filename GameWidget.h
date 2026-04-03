#pragma once

#include <QWidget>
#include <vector>

class GameWidget : public QWidget
{
    Q_OBJECT
    int n;
    int visibleCells;
    std::vector<std::vector<bool>> newGrid;

public:
    GameWidget(QWidget *parent = nullptr);


public slots:
    void importGrid();
    void exportGrid();
    void nextGeneration();
    void clear();
    void setGridSize(int newSize);
    void changeRules(const QString& rule);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    std::vector<std::vector<bool>> grid;
    std::vector<int> birthRules;
    std::vector<int> surviveRules;

    int countNeighbors(int row, int col) const;
};
