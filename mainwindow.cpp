#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QSpacerItem"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), settingsWindow(new SettingsWindow(this))
{
    ui->setupUi(this);

    //QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //ui->servers_VLayout->addSpacerItem(verticalSpacer);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::openSettings);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showContextMenu(QPushButton *button, const QPoint &pos)
{
    // Создаем контекстное меню
    QMenu contextMenu;

    // Добавляем действие "Удалить"
    QAction *deleteAction = new QAction("Удалить", &contextMenu);
    contextMenu.addAction(deleteAction);

    connect(deleteAction, &QAction::triggered, this, [this, button]() {removeButton(button);});

    contextMenu.exec(button->mapToGlobal(pos));
}

void MainWindow::removeButton(QPushButton *button)
{
    ui->servers_VLayout->removeWidget(button);
    button->deleteLater();
}

void MainWindow::on_pushButton_clicked()
{
    QPushButton *newButton = new QPushButton(QString::number(ui->servers_VLayout->count()), this);
    newButton->setFixedHeight(50);
    newButton->setFixedWidth(50);
    QRect rect(0,0,48,48);
    QRegion region(rect, QRegion::Ellipse);
    newButton->setMask(region);

    // Устанавливаем политику для вызова контекстного меню
    newButton->setContextMenuPolicy(Qt::CustomContextMenu);

    // Подключаем сигнал для обработки вызова контекстного меню
    connect(newButton, &QPushButton::customContextMenuRequested, this, [this, newButton](const QPoint &pos) {
        showContextMenu(newButton, pos);
    });

    ui->servers_VLayout->insertWidget(ui->servers_VLayout->count() - 1, newButton);
}

void MainWindow::openSettings()
{
    settingsWindow->exec();  // Открытие окна настроек
}
