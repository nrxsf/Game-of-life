#include "GameWidget.h"
#include <QPainter>
#include <QColor>
#include <QFile>
#include <QFileDialog>
#include <QDataStream>
#include <QVector>
#include <QMessageBox>
#include <QMouseEvent>
#include <algorithm>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), n(206), visibleCells(56)
{
    grid.resize(n, std::vector<bool>(n, false));
    newGrid = grid;
}

void GameWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    int realCells = visibleCells - 6;

    double cellWidth = static_cast<double>(width()) / realCells;
    double cellHeight = static_cast<double>(height()) / realCells;
    double cellSize = std::min(cellWidth, cellHeight);

    double xOffset = 0;
    double yOffset = (height() - realCells * cellSize) / 2.0;

    QColor color("#444444");
    painter.fillRect(rect(), color);

    int center = n / 2 - visibleCells / 2;

    for (int r = 3; r < visibleCells - 3; ++r)
    {
        for (int c = 3; c < visibleCells - 3; ++c)
        {
            if (grid[center + r][center + c])
            {
                QRectF rect(
                    xOffset + (c - 3) * cellSize,
                    yOffset + (r - 3) * cellSize,
                    cellSize, cellSize
                    );
                QColor boxColor("#0e9d23");
                painter.fillRect(rect, boxColor);
            }
        }
    }

    QColor borderColor("#FFFFFFF");
    painter.setPen(borderColor);
    for (int i = 0; i <= realCells; ++i) {
        painter.drawLine(
            xOffset + i * cellSize, yOffset,
            xOffset + i * cellSize, yOffset + realCells * cellSize
            );
        painter.drawLine(
            xOffset, yOffset + i * cellSize,
            xOffset + realCells * cellSize, yOffset + i * cellSize
            );
    }
}

void GameWidget::mousePressEvent(QMouseEvent *event)
{
    int realCells = visibleCells - 6;

    double cellWidth = static_cast<double>(width()) / realCells;
    double cellHeight = static_cast<double>(height()) / realCells;
    double cellSize = std::min(cellWidth, cellHeight);

    double xOffset = 0;
    double yOffset = (height() - realCells * cellSize) / 2.0;

    int col = (event->pos().x() - xOffset) / cellSize + 3;
    int row = (event->pos().y() - yOffset) / cellSize + 3;

    if (row >= 3 && row < visibleCells - 3 && col >= 3 && col < visibleCells - 3)
    {
        int center = n / 2 - visibleCells / 2;
        grid[center + row][center + col] = !grid[center + row][center + col];
        update();
    }
}

int GameWidget::countNeighbors(int row, int col) const
{
    int count = 0;
    for (int dr = -1; dr <= 1; ++dr)
    {
        for (int dc = -1; dc <= 1; ++dc)
        {
            if (dr == 0 && dc == 0) continue;
            int r_1 = row + dr;
            int c_1 = col + dc;
            if (r_1 >= 0 && r_1 < n && c_1 >= 0 && c_1 < n && grid[r_1][c_1])
                ++count;
        }
    }
    return count;
}

void GameWidget::nextGeneration()
{
    for (int r = 0; r < n; ++r)
    {
        for (int c = 0; c < n; ++c)
        {
            int neighbors = countNeighbors(r, c);

            if (grid[r][c])
            {
                newGrid[r][c] = std::find(surviveRules.begin(), surviveRules.end(), neighbors) != surviveRules.end();
            }
            else
            {
                newGrid[r][c] = std::find(birthRules.begin(), birthRules.end(), neighbors) != birthRules.end();
            }
        }
    }

    grid.swap(newGrid);
    update();
}

void GameWidget::clear()
{
    for (auto& row : grid)
        std::fill(row.begin(), row.end(), false);
    update();
}

void GameWidget::setGridSize(int count)
{
    visibleCells = count;
    update();
}

void GameWidget::changeRules(const QString &rule)
{
    birthRules.clear();
    surviveRules.clear();

    bool birth = false;
    for (QChar ch : rule)
    {
        if (ch == 'B')
        {
            birth = true;
        }
        else if (ch == 'S')
        {
            birth = false;
        }
        else if (ch.isDigit())
        {
            int num = ch.digitValue();
            if (birth)
            {
                birthRules.push_back(num);
            }
            else
            {
                surviveRules.push_back(num);
            }
        }
    }
}

void GameWidget::exportGrid()
{
    const char FILE_SIGNATURE[4] = {'G', 'L', 'I', 'F'};

    QString initialDir = QCoreApplication::applicationDirPath() + "/structExamples";
    QDir().mkpath(initialDir);

    QString filename = QFileDialog::getSaveFileName(
        this,
        "Зберегти стан гри",
        initialDir + "/",
        "Бінарні файли (*.bin);;Усі файли (*)"
        );

    if (filename.isEmpty()) return;

    if (!filename.endsWith(".bin", Qt::CaseInsensitive))
        filename += ".bin";

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Помилка", "Не вдалося відкрити файл для запису.");
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_0);

    out.writeRawData(FILE_SIGNATURE, sizeof(FILE_SIGNATURE));

    int rows = grid.size();
    int cols = rows > 0 ? grid[0].size() : 0;

    out << rows << cols;

    for (const auto &row : grid)
        for (bool cell : row)
            out << cell;

    file.close();
    QMessageBox::information(this, "Експорт завершено", "Стан гри успішно збережено.");
}

void GameWidget::importGrid()
{
    const char FILE_SIGNATURE[4] = {'G', 'L', 'I', 'F'};

    QString initialDir = QCoreApplication::applicationDirPath() + "/structExamples";
    QDir().mkpath(initialDir);

    QString filename = QFileDialog::getOpenFileName(
        this,
        "Відкрити стан гри",
        initialDir,
        "Бінарні файли (*.bin);;Усі файли (*)"
        );

    if (filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Помилка", "Не вдалося відкрити файл для читання.");
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_0);

    char fileSignature[4];
    if (file.read(fileSignature, sizeof(fileSignature)) != sizeof(fileSignature)) {
        QMessageBox::warning(this, "Помилка", "Файл пошкоджено або некоректний.");
        return;
    }

    if (memcmp(fileSignature, FILE_SIGNATURE, sizeof(FILE_SIGNATURE)) != 0) {
        QMessageBox::warning(this, "Помилка", "Цей файл не є файлом гри Життя.");
        return;
    }

    int rows = 0, cols = 0;
    in >> rows >> cols;

    if (rows <= 0 || cols <= 0) {
        QMessageBox::warning(this, "Помилка", "Некоректний розмір поля у файлі.");
        return;
    }

    grid.resize(rows);
    for (int i = 0; i < rows; ++i) {
        grid[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            bool cell;
            in >> cell;
            grid[i][j] = cell;
        }
    }

    file.close();

    update();
}
