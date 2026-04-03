#include "MainWindow.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QPixmap>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(1024, 800);
    this->setStyleSheet("background-color: #444444;");

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    setupMenuPage();
    setupGamePage();
    setupInfoPage();

    stackedWidget->addWidget(menuPage);
    stackedWidget->addWidget(gamePage);
    stackedWidget->addWidget(infoPage1);
    stackedWidget->addWidget(infoPage2);
}

MainWindow::~MainWindow()
{}

void MainWindow::setupInfoPage()
{
    infoPage1 = new QWidget(this);
    infoPage2 = new QWidget(this);

    infoPage1->setStyleSheet("background-color: #444444;");
    infoPage2->setStyleSheet("background-color: #444444;");

    QLabel *infoRules1 = new QLabel(infoPage1);
    QPixmap qpixmap1("images/infoPage1.png");
    infoRules1->setPixmap(qpixmap1);

    QLabel *infoRules2 = new QLabel(infoPage2);
    QPixmap qpixmap2("images/infoPage2.png");
    infoRules2->setPixmap(qpixmap2);

    QVBoxLayout *infoLayout1 = new QVBoxLayout(infoPage1);
    infoLayout1->addWidget(infoRules1, 0, Qt::AlignCenter);
    infoPage1->setLayout(infoLayout1);

    QVBoxLayout *infoLayout2 = new QVBoxLayout(infoPage2);
    infoLayout2->addWidget(infoRules2, 0, Qt::AlignCenter);
    infoPage2->setLayout(infoLayout2);

    infoRules1->show();
    infoRules2->show();

    QPushButton *backInfoButton = new QPushButton("Назад в меню", infoPage1);
    QPushButton *nextInfoButton = new QPushButton("Додаткові правила", infoPage1);
    QPushButton *previousInfoButton = new QPushButton("Класичні правила", infoPage2);

    QFont font = backInfoButton->font();
    font.setPointSize(15);
    font.setBold(true);

    backInfoButton->setFont(font);
    backInfoButton->setStyleSheet("color: #0e9d23;");

    nextInfoButton->setFont(font);
    nextInfoButton->setStyleSheet("color: #0e9d23;");

    previousInfoButton->setFont(font);
    previousInfoButton->setStyleSheet("color: #0e9d23;");

    backInfoButton->setGeometry(10, 10, 215, 60);
    nextInfoButton->setGeometry(800, 10, 215, 60);
    previousInfoButton->setGeometry(10, 10, 215, 60);

    connect(backInfoButton, &QPushButton::clicked, [this]()
            {
                stackedWidget->setCurrentWidget(menuPage);
            });

    connect(nextInfoButton, &QPushButton::clicked, [this]()
            {
                stackedWidget->setCurrentWidget(infoPage2);
            });

    connect(previousInfoButton, &QPushButton::clicked, [this]()
            {
                stackedWidget->setCurrentWidget(infoPage1);
            });

}

void MainWindow::setupGamePage()
{
    timer = new QTimer(this);
    timer->setInterval(500);

    gamePage = new GameWidget(this);
    gamePage->setStyleSheet("background-color: #444444;");

    connect(timer, &QTimer::timeout, gamePage, &GameWidget::nextGeneration);

    QPushButton *backGameButton = new QPushButton("Назад в меню", gamePage);
    QPushButton *importGameButton = new QPushButton("Завантажити стан", gamePage);
    QPushButton *exportGameButton = new QPushButton("Зберегти стан", gamePage);
    QPushButton *stepButton = new QPushButton(gamePage);
    QPushButton *clearButton = new QPushButton(gamePage);
    gameButton = new QPushButton(gamePage);

    gameButton->setStyleSheet(playButtonStyle);
    stepButton->setStyleSheet(nextButtonStyle);
    clearButton->setStyleSheet(clearButtonStyle);

    QFont font = backGameButton->font();
    font.setPointSize(15);
    font.setBold(true);

    backGameButton->setFont(font);
    backGameButton->setStyleSheet("color: #0e9d23;");
    importGameButton->setFont(font);
    importGameButton->setStyleSheet("color: #0e9d23;");
    exportGameButton->setFont(font);
    exportGameButton->setStyleSheet("color: #0e9d23;");

    backGameButton->setGeometry(805, 10, 215, 40);
    importGameButton->setGeometry(805, 255, 215, 40);
    exportGameButton->setGeometry(805, 300, 215, 40);
    clearButton->setGeometry(850, 650, 120, 120);
    stepButton->setGeometry(850, 525, 120, 120);
    gameButton->setGeometry(850, 400, 120, 120);


    QString sliderStyle =
        "QSlider::groove:horizontal {"
        "height: 10px;"
        "background: #888;"
        "margin: 0px;"
        "}"

        "QSlider::handle:horizontal {"
        "width: 10px;"
        "height: 20px;"
        "margin: -5px 0;"
        "background: #174907;"
        "border: 2px solid #000000;"
        "}";



    QLabel *speedLabel = new QLabel("Швидкість симуляції:", gamePage);
    speedLabel->setGeometry(805, 55, 215, 40);
    speedLabel->setFont(font);
    speedLabel->setStyleSheet("color: #0e9d23;");

    QLabel *sizeLabel = new QLabel("Розмір поля:", gamePage);
    sizeLabel->setGeometry(805, 125, 215, 40);
    sizeLabel->setFont(font);
    sizeLabel->setStyleSheet("color: #0e9d23;");

    QSlider *speedSlider = new QSlider(Qt::Horizontal, gamePage);
    speedSlider->setGeometry(805, 90, 215, 40);
    speedSlider->setRange(2, 20);
    speedSlider->setValue(2);

    QSlider *sizeSlider = new QSlider(Qt::Horizontal, gamePage);
    sizeSlider->setGeometry(805, 165, 215, 40);
    sizeSlider->setRange(16, 206);
    sizeSlider->setValue(56);

    sizeSlider->setStyleSheet(sliderStyle);
    speedSlider->setStyleSheet(sliderStyle);

    QComboBox *ruleBox = new QComboBox(gamePage);
    ruleBox->addItem("B3/S23 (Classic)");
    ruleBox->addItem("B36/S23 (HighLife)");
    ruleBox->addItem("B2/S (Seeds)");
    ruleBox->addItem("B3/S012345678 (NoDeath)");
    ruleBox->addItem("B34/S34 (34 Life)");

    ruleBox->setEditable(false);
    ruleBox->setFocusPolicy(Qt::NoFocus);

    ruleBox->setCurrentIndex(0);
    gamePage->changeRules("B3/S23");

    ruleBox->setGeometry(805, 205, 215, 40);
    ruleBox->setFont(font);
    ruleBox->setStyleSheet("color: #0e9d23;");

    connect(ruleBox, &QComboBox::currentTextChanged, [this](const QString& text)
            {
                QString rule = text.split(' ').first();
                gamePage->changeRules(rule);
            });

    connect(speedSlider, &QSlider::valueChanged, [this](int value)
            {
                timer->setInterval(1000/value);
            });

    connect(sizeSlider, &QSlider::valueChanged, [this](int value)
            {
                gamePage->setGridSize(value);
            });

    connect(gameButton, &QPushButton::clicked, this, &MainWindow::toggleGame);
    connect(clearButton, &QPushButton::clicked, gamePage, &GameWidget::clear);
    connect(stepButton, &QPushButton::clicked, gamePage, &GameWidget::nextGeneration);
    connect(importGameButton, &QPushButton::clicked, [this]()
            {
                if(gameRunning)
                {
                    this->toggleGame();
                }
            });
    connect(importGameButton, &QPushButton::clicked, gamePage, &GameWidget::importGrid);
    connect(exportGameButton, &QPushButton::clicked, [this]()
            {
                if(gameRunning)
                {
                    this->toggleGame();
                }
            });
    connect(exportGameButton, &QPushButton::clicked, gamePage, &GameWidget::exportGrid);

    connect(backGameButton, &QPushButton::clicked, [this]()
            {
                if(gameRunning)
                {
                    this->toggleGame();
                }
                stackedWidget->setCurrentWidget(menuPage);
            });
}

void MainWindow::toggleGame()
{
    if (gameRunning)
    {
        timer->stop();
        gameButton->setStyleSheet(playButtonStyle);
    }
    else
    {
        timer->start();
        gameButton->setStyleSheet(pauseButtonStyle);
    }
    gameRunning = !gameRunning;
}

void MainWindow::setupMenuPage()
{
    menuPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(menuPage);

    QPushButton *startButton = new QPushButton("ПОЧАТИ");
    QPushButton *infoButton = new QPushButton("ДОВІДКА");
    QPushButton *exitButton = new QPushButton("ВИХІД");

    QLabel *logo = new QLabel(this);
    QPixmap qpixmap1("images/logo.png");
    logo->setPixmap(qpixmap1);

    QFont font = startButton->font();
    font.setPointSize(20);
    font.setBold(true);
    startButton->setFont(font);
    infoButton->setFont(font);
    exitButton->setFont(font);


    startButton->setStyleSheet("color: #0e9d23;");
    infoButton->setStyleSheet("color: #0e9d23;");
    exitButton->setStyleSheet("color: #0e9d23;");

    startButton->setFixedSize(500, 100);
    infoButton->setFixedSize(500, 100);
    exitButton->setFixedSize(500, 100);


    layout->addStretch();
    layout->addWidget(logo, 0, Qt::AlignCenter);
    layout->addWidget(startButton, 0, Qt::AlignCenter);
    layout->addWidget(infoButton, 0, Qt::AlignCenter);
    layout->addWidget(exitButton, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(startButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(gamePage);
    });

    connect(infoButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(infoPage1);
    });

    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
}
