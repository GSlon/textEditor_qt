#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // пройтись через this по функциям (чтобы всё не лежало в конструкторе)
    this->setWindowTitle("Лаба 1");
    this->setupControls();
    this->initDirectoriesTree();
    // this->setWindowState((windowState() & ~(Qt::WindowMinimized | Qt::WindowFullScreen)) | Qt::WindowMaximized);

    fileDir = "";   //директория
    fileDirNew = "";    //

    //resetWatch("F:/qt/projects/laba1Qt/textik.txt");
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupControls()
{
    this->addMenu();

    QWidget *w = new QWidget;

    //dir
    dirname = new QTreeView;
    dirname->setStatusTip("directories (only folders)");   //легенда
    dirname->setToolTip("directories");
    Q_ASSERT(connect(dirname, SIGNAL(clicked(const QModelIndex&)), this, SLOT(dirClicked(const QModelIndex&))));


    //file
    filename = new QListWidget;
    filename->setStatusTip("files (only files, without folders)");
    filename->setToolTip("files");
    Q_ASSERT(connect(filename, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item)  //лямбда-выражение
    {
        fileDirNew = fileDir + item->text();
        resetWatch(fileDirNew);
    }
    ));


    //watch
    WatchTxt = new QTextEdit;
    WatchTxt->setStatusTip("formatted text (in txt files)");
    WatchTxt->setToolTip("in txt file");
    //Q_ASSERT(connect(WatchTxt, SIGNAL(textChanged()), this, SLOT(WatchChanged())));

    //линия
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    //line->setFrameShadow(QFrame::Sunken);


    // Button, radiobox...
    rdbutCP = new QRadioButton;
    rdbutCP->setText("cp1251");
    rdbutCP->setStatusTip("encoding");
    rdbutCP->setToolTip("encoding");
    rdbutCP->setChecked(true);
    Q_ASSERT(connect(rdbutCP, SIGNAL(clicked()), this, SLOT(WatchChanged())));

    rdbutUTF = new QRadioButton;
    rdbutUTF->setText("utf-8");
    rdbutUTF->setStatusTip("encoding");
    rdbutUTF->setToolTip("encoding");
    Q_ASSERT(connect(rdbutUTF, SIGNAL(clicked()), this, SLOT(WatchChanged())));

    chkbxcl = new QCheckBox;
    chkbxcl->setText("полужирный");
    chkbxcl->setStatusTip("font");
    Q_ASSERT(connect(chkbxcl, SIGNAL(toggled(bool)), this, SLOT(WatchChanged())));

    chkbxit = new QCheckBox;
    chkbxit->setText("курсив");
    chkbxit->setStatusTip("font");
    Q_ASSERT(connect(chkbxit, SIGNAL(toggled(bool)), this, SLOT(WatchChanged())));

    chkbxun = new QCheckBox;
    chkbxun->setText("underline");
    chkbxun->setStatusTip("font");
    Q_ASSERT(connect(chkbxun, SIGNAL(toggled(bool)), this, SLOT(WatchChanged())));


    combox = new QComboBox;
    combox->setStatusTip("select color");
    combox->addItem("black");
    combox->addItem("green");
    combox->addItem("blue");
    combox->addItem("red");
    combox->addItem("xxx");
    Q_ASSERT(connect(combox, SIGNAL(currentIndexChanged(QString)), this, SLOT(WatchChanged())));


    but = new QPushButton;
    but->setText("default");
    but->setStatusTip("return to default setup");
    but->setToolTip("default");
    Q_ASSERT(connect(but, SIGNAL(clicked()), this, SLOT(ToDefault())));


    //сепаратор
    QSplitter *separator = new QSplitter;
    separator->setOrientation(Qt::Vertical);
    separator->addWidget(rdbutCP);
    separator->addWidget(rdbutUTF);
    separator->addWidget(chkbxcl);
    separator->addWidget(chkbxit);
    separator->addWidget(chkbxun);
    separator->addWidget(combox);
    separator->addWidget(but);
    // ///


    // надписи
    QLabel *labelDir = new QLabel("  Directory");
    QLabel *labelFile = new QLabel("  Files");
    QLabel *labelOut = new QLabel("  text");
    QLabel *labelSetup = new QLabel("  setup");

    \
    //layout
    QGridLayout *lay = new QGridLayout;
    lay->setMargin(5);
    lay->addWidget(labelDir, 0, 0);
    lay->addWidget(labelFile, 0, 1);
    lay->addWidget(labelOut, 0, 2);
    lay->addWidget(labelSetup, 0, 4);
    lay->addWidget(dirname, 1, 0);
    lay->addWidget(filename, 1, 1);
    lay->addWidget(WatchTxt, 1, 2);
    lay->addWidget(line, 1, 3);
    lay->addWidget(separator, 1, 4);  //кнопки управления


    w->setLayout(lay);
    setCentralWidget(w);   // "накладываем наш виджет на 'mainwidget' "
}


void MainWindow::addMenu()
{
    QMenu* fileMenu = new QMenu("File");

    fileMenu->addAction("Save as"
                        ,this
                        ,SLOT(SaveAs())
                        ,Qt::CTRL + Qt::Key_S);

    fileMenu->addAction("Exit"
                        ,this
                        ,SLOT(quitProgram())
                        ,Qt::CTRL + Qt::Key_Q);


    QMenu* aboutMenu = new QMenu("About");

    aboutMenu->addAction("About"
                         ,this
                         ,SLOT(About())
                         ,Qt::CTRL + Qt::Key_R);


    ui->menuBar->addMenu(fileMenu);       //вложенно в menubar
    ui->menuBar->addMenu(aboutMenu);  //сразу при нажатии
    ui->menuBar->show();
}


void MainWindow::initDirectoriesTree()
{
    model = new QFileSystemModel;
    model->setRootPath("");
    model->iconProvider()->setOptions(QFileIconProvider::DontUseCustomDirectoryIcons);
    model->setFilter(QDir::AllDirs);  //фильтр

    dirname->setModel(model);
    dirname->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::DoubleClicked);
}


void MainWindow::resetFilesList(QString directoryName)
{
    QDir directory(directoryName);

    if (!directory.exists())
    {
        QMessageBox::information(this, "Информация", "Файл заблокирован!", QMessageBox::Ok);
        // filename->clear();
        return;
    }

    QStringList filesInDir = directory.entryList(QStringList(), QDir::Files);  //возвращает лист имен
                                                                               //файлов в текущей dir

    filename->clear();
    filename->addItems(filesInDir);   //заполняем именами файлов
}


//считанивание из файла
void MainWindow::resetWatch(QString filename)
{
    QFile inFile(filename);

    //проверка
    if (!inFile.exists())
    {
        QMessageBox::information(this, "Информация", "Файл заблокирован!", QMessageBox::Ok);
        WatchTxt->clear();
        return;
    }

    //проверка на расширение
    if ((filename.indexOf(".txt") == -1) &&
            (filename.indexOf(".ini") == -1))
    {
        QMessageBox::information(this, "Информация", "Недопустимое расширение", QMessageBox::Ok);
        WatchTxt->clear();
        return;
    }

    QTextStream inStream(&inFile);

    inFile.open(QIODevice::ReadOnly | QIODevice::Text);

    if (inStream.status() != QTextStream::Ok)
    {
        QMessageBox::information(this, "Информация", "Файл заблокирован!", QMessageBox::Ok);
        WatchTxt->clear();
        return;
    }

    WatchTxt->clear();

    WatchTxt->insertPlainText(inStream.readAll());

    inFile.close();
}


void MainWindow::quitProgram()
{
    this->close();
}

void MainWindow::About()
{
    QMessageBox::information(this, "About program:", "Прохватилов С. С. , М80-213Б-17", QMessageBox::Ok);
}

void MainWindow::ToDefault()
{
    rdbutCP->setChecked(true);
    //QFont font;
    //font.set);
    //font.setFamily(font.defaultFamily());
    WatchTxt->setFont(QFont("Times", 8));
    WatchTxt->setFontItalic(false);
    WatchTxt->setFontUnderline(false);
    chkbxcl->setChecked(false);
    chkbxit->setChecked(false);
    chkbxun->setChecked(false);
    combox->setCurrentText("black");

    WatchChanged();
}

void MainWindow::dirClicked(const QModelIndex& index)
{
    if (!index.isValid())
    {
        QMessageBox::information(this, "Информация", "Файл заблокирован!", QMessageBox::Ok);
        return;
    }

    //QVariant data = index.data();

    fileDir = model->filePath(index) + '/';
    resetFilesList(model->filePath(index) + '/');  //обновляем второй столбец
}

void MainWindow::SaveAs()
{
    QFileDialog dlg;
    QString strFileName;

    strFileName = dlg.getSaveFileName(this
                                      ,"file to save"
                                      ,QString("F:////")
                                      ,"*.txt");

    QFile outFile(strFileName);

    if (!outFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this,
                                 "ошибка"
                                 ,"Ошибка сохранения файла"
                                 ,QMessageBox::Ok
                                 );
    }
    else
    {
        QTextStream stream(&outFile);
        // stream.setCodec(QTextCodec::codecForName("CP1251"));

        QString strtxt = "";

        //сохранение в файл с помощью манипуляторов (.toUTF8)  вызов функции
        strtxt = WatchTxt->toPlainText();

        stream << strtxt;
    }

    outFile.close();
}

//вызов resetWatch
void MainWindow::WatchChanged()
{
    //промежуточная проверка, чтобы не сигнализировать на каждое нажатие
    //QFile dir(fileDirNew);
    //if (!dir.exists())
    //   return;

    //dir.close();

    //перерисовываем
    //resetWatch(fileDirNew);

    //обновляем стиль
    txtStyleChanged();
}

void MainWindow::txtStyleChanged()
{
    if (chkbxcl->isChecked())
        WatchTxt->setFont(QFont("Times", 10));
    else
    {
       WatchTxt->setFont(QFont("Times", 8));
    }


    if (chkbxit->isChecked())
        WatchTxt->setFontItalic(true);
    else
        WatchTxt->setFontItalic(false);


    if (chkbxun->isChecked())
        WatchTxt->setFontUnderline(true);
    else
        WatchTxt->setFontUnderline(false);


    if (rdbutUTF->isChecked())
    {
         QByteArray qbCP = QString(WatchTxt->toPlainText()).toLocal8Bit();
         WatchTxt->clear();
         WatchTxt->insertPlainText(qbCP.constData());
    }
    else if (rdbutCP->isChecked())
    {
        QByteArray qbUTF = QString(WatchTxt->toPlainText()).toUtf8();
        WatchTxt->clear();
        WatchTxt->insertPlainText(qbUTF.constData());
    }

    if (!combox->currentText().compare("black"))
    {
        QPalette qplt;
        //qplt.setColor();

    }
    //else if


    //WatchTxt->toPlainText();
    //WatchTxt->insertPlainText(str);
    //WatchTxt->insertPlainText(str2);
    //str2.insert(str.unicode());
    //WatchTxt->
    //if (rdbutCP->hasFocus())
    //    inStream.setCodec("cp1251");
    //else if (rdbutUTF->hasFocus())
      //      inStream.setCodec("utf-8");
    //WatchTxt->setTextColor();
    //WatchTxt->setStyle();
    //WatchTxt->setStyleSheet()

    //WatchTxt->setFont()
    //QString str;
    //str.toUtf8()

}
