#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QTimer>
#include "GameWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *stackedWidget;
    QWidget *menuPage;
    QWidget *infoPage1;
    QWidget *infoPage2;
    QTimer *timer;
    GameWidget *gamePage;
    QPushButton *gameButton;

    QString clearButtonStyle =
        "QPushButton {"
        "border: none;"
        "background-image: url(images/resetButton.png);"
        "}"
        "QPushButton:hover {"
        "border: none;"
        "background-image: url(images/resetHoveredButton.png);"
        "}"
        "QPushButton:pressed {"
        "border: none;"
        "background-image: url(images/resetPressedButton.png);"
        "}";

    QString nextButtonStyle =
        "QPushButton {"
        "border: none;"
        "background-image: url(images/nextButton.png);"
        "}"
        "QPushButton:hover {"
        "border: none;"
        "background-image: url(images/nextHoveredButton.png);"
        "}"
        "QPushButton:pressed {"
        "border: none;"
        "background-image: url(images/nextPressedButton.png);"
        "}";

    QString playButtonStyle =
        "QPushButton {"
        "border: none;"
        "background-image: url(images/playButton.png);"
        "}"
        "QPushButton:hover {"
        "border: none;"
        "background-image: url(images/playHoveredButton.png);"
        "}"
        "QPushButton:pressed {"
        "border: none;"
        "background-image: url(images/playPressedButton.png);"
        "}";

    QString pauseButtonStyle =
        "QPushButton {"
        "border: none;"
        "background-image: url(images/pauseButton.png);"
        "}"
        "QPushButton:hover {"
        "border: none;"
        "background-image: url(images/pauseHoveredButton.png);"
        "}"
        "QPushButton:pressed {"
        "border: none;"
        "background-image: url(images/pausePressedButton.png);"
        "}";

    bool gameRunning = false;

    void setupInfoPage();
    void setupGamePage();
    void setupMenuPage();
    void toggleGame();
};
