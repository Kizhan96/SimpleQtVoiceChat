#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QSpacerItem"
#include "QLabel"


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

    // Добавляем действия
    QAction *deleteAction = new QAction("Удалить", &contextMenu);
    QAction *changeAction = new QAction("Изменить", &contextMenu);
    QAction *connectAction = new QAction("Подключиться", &contextMenu);

    contextMenu.addAction(connectAction);
    contextMenu.addAction(changeAction);
    contextMenu.addAction(deleteAction);


    connect(connectAction, &QAction::triggered, this, [this, button]() {contextMenuConnectButton(button);});
    connect(changeAction, &QAction::triggered, this, [this, button]() {contextMenuChangeButton(button);});
    connect(deleteAction, &QAction::triggered, this, [this, button]() {contextMenuRemoveButton(button);});

    contextMenu.exec(button->mapToGlobal(pos));
}

void MainWindow::contextMenuConnectButton(QPushButton *button)
{
    QString ipAddress = button->text().split(":")[0];

    uint16_t port = button->text().split(":")[1].toUInt();

    qDebug() << "Try to connect to ip: " << ipAddress << " port: " << port;

    try
    {
        client = new VoiceClient(ipAddress, port, this);
    }
    catch (...)
    {
        qWarning() << "Failed to create VoiceClient.";
    }
}

void MainWindow::contextMenuChangeButton(QPushButton *button)
{
    // Создаем диалоговое окно
    QDialog dialog(this);
    dialog.setWindowTitle("Изменение сервера");

    QGridLayout layout(&dialog);
    QLabel ipLabel("IP:", &dialog);
    QLineEdit ipLineEdit(&dialog);
    QLabel portLabel("Port:", &dialog);
    QLineEdit portLineEdit(&dialog);
    QPushButton applyButton("Применить", &dialog);
    QPushButton cancelButton("Отмена", &dialog);

    layout.addWidget(&ipLabel, 0, 0);
    layout.addWidget(&ipLineEdit, 0, 1);
    layout.addWidget(&portLabel, 1, 0);
    layout.addWidget(&portLineEdit, 1, 1);
    layout.addWidget(&applyButton, 2, 0);
    layout.addWidget(&cancelButton, 2, 1);

    // Обработчик нажатия кнопки "Применить"
    connect(&applyButton, &QPushButton::clicked, &dialog, [&]() {
        QString ip = ipLineEdit.text().trimmed();
        QString port = portLineEdit.text().trimmed();

        // Проверка на пустые поля
        if (ip.isEmpty() || port.isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "IP или Port не могут быть пустыми!");
            return; // Не закрываем окно, если данные некорректны
        }

        // Успешное добавление, обновляем кнопку
        button->setText(ip + ":" + port);
        button->setStyleSheet("QPushButton {background-color: #A3C1DA; color: green;}");

        // Сохраняем изменения в файл настроек
        QSettings settings(QDir::currentPath() + "/settings.ini", QSettings::IniFormat);
        settings.setValue("servers/ip", ip);
        settings.setValue("servers/port", port);

        dialog.accept(); // Закрываем окно
    });

    // Обработчик нажатия кнопки "Отмена"
    connect(&cancelButton, &QPushButton::clicked, &dialog, [&]() {
        dialog.reject(); // Закрываем окно без изменений
    });

    // Открываем окно
    dialog.exec();
}

void MainWindow::contextMenuRemoveButton(QPushButton *button)
{
    if (client != nullptr)
        qDebug() << "button->text()";
    else
        qDebug() << "else";
    ui->servers_VLayout->removeWidget(button);
    button->deleteLater();
}

void MainWindow::on_mainWindowServerAddButton_clicked()
{
    QPushButton *newButton = new QPushButton("", this);
    newButton->setFixedHeight(50);
    newButton->setFixedWidth(50);
    QRect rect(0,0,48,48);
    QRegion region(rect, QRegion::Ellipse);
    newButton->setMask(region);

    // Устанавливаем политику для вызова контекстного меню
    newButton->setContextMenuPolicy(Qt::CustomContextMenu);

    // Подключаем сигнал для обработки вызова контекстного меню
    connect(newButton, &QPushButton::customContextMenuRequested,    this, [this, newButton](const QPoint &pos) {showContextMenu(newButton, pos);});
    connect(newButton, &QPushButton::clicked,                       this, [this, newButton]() { contextMenuConnectButton(newButton);});

    contextMenuChangeButton(newButton);

    ui->servers_VLayout->insertWidget(ui->servers_VLayout->count() - 1, newButton);

}

void MainWindow::openSettings()
{
    settingsWindow->exec();  // Открытие окна настроек
}
