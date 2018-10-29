#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Laba 1");
    this->setupControls();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupControls()
{
    this->addMenu();

    QWidget *w = new QWidget(this);

    model = new QStandardItemModel(1, 1, this);  // модель для представления в Tree
    model->setHeaderData(0,  Qt::Horizontal, "menu");

    QModelIndex index = model->index(0, 0);
    model->setData(index, "figure");

    model->insertRows(0, 2, index);
    model->insertColumns(0, 2, index);

    model->setData(model->index(0, 0, index), "circle");
    model->setData(model->index(1, 0, index), "ellipse");


    view = new MyGraphicView(this);
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);   //чтобы никуда не уплывали фигуры

    tree = new QTreeView(this);
    tree->setMinimumSize(QSize(120,10));   // не будет сдвига фигур
    tree->setMaximumSize(QSize(120,2000));
    tree->setModel(model);
    tree->setToolTip("figure");
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);  //no edit
    Q_ASSERT(connect(tree, SIGNAL(clicked(const QModelIndex&)), view, SLOT(treeClicked(const QModelIndex&))));


    lay = new QHBoxLayout(this);
    lay->setMargin(5);
    lay->addWidget(tree);
    lay->addWidget(view);

    w->setLayout(lay);
    setCentralWidget(w);
}

void MainWindow::addMenu()
{
    QMenu* fileMenu = new QMenu("&File");

    fileMenu->addAction("&Exit"
                        ,this
                        ,SLOT(quitProgram())
                        ,Qt::CTRL + Qt::Key_Q);


    QMenu* aboutMenu = new QMenu("&About");

    aboutMenu->addAction("&About"
                         ,this
                         ,SLOT(About())
                         ,Qt::CTRL + Qt::Key_R);

    aboutMenu->addAction("&Help"
                         ,this
                         ,SLOT(Help())
                         ,Qt::CTRL + Qt::Key_H);


    ui->menuBar->addMenu(fileMenu);       //вложенно в menubar
    ui->menuBar->addMenu(aboutMenu);  //сразу при нажатии
    ui->menuBar->show();
}

void MainWindow::quitProgram()
{
    this->close();
}

void MainWindow::About()
{
    QMessageBox::information(this, "About program", "Прохватилов С. С. , М80-213Б-17", QMessageBox::Ok);
}

void MainWindow::Help()
{
    QString str;
    QDir dir;
    // str = QApplication::applicationFilePath();
    dir.setPath(dir.absolutePath());  // путь до сборки
    str = dir.absolutePath();
    qDebug() << str;

    QFile file(str + "/help.txt");

    //не откроется, значит была включена теневая сборка (двигаемся к pro-файлу)
    if (!file.isOpen())
    {
        dir.cdUp();  //к каталогу
        str = dir.absolutePath();
        file.setFileName(str + "/laba1poc/help.txt");
        file.open(QIODevice::ReadOnly);
        if (!file.isOpen())
        {
            qDebug() << "help.txt не найден";
            QMessageBox::information(this, "error", "файл help.txt не найден");
            return;
        }
    }


    QTextEdit *edit = new QTextEdit(this);
    edit->setReadOnly(true);

    QTextStream text(&file);
    str.clear();
    str = text.readAll();
    edit->setPlainText(str);

    widget = new QWidget(this, Qt::Window);
    widget->setGeometry(QRect(450, 250, 500, 300));
    widget->setWindowTitle("help");

    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->addWidget(edit);
    widget->setLayout(lay);

    widget->activateWindow();
    widget->show();
}
