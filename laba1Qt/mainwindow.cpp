#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // пройтись через this по функциям (чтобы всё не лежало в конструкторе)
    this->setWindowTitle("Laba 1");
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
    WatchTxt->setEnabled(false);
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
    Q_ASSERT(connect(rdbutCP, SIGNAL(toggled(bool)), this, SLOT(CodirChanged())));

    rdbutUTF = new QRadioButton;
    rdbutUTF->setText("utf-8");
    rdbutUTF->setStatusTip("encoding");
    rdbutUTF->setToolTip("encoding");
    rdbutUTF->setChecked(false);
    Q_ASSERT(connect(rdbutUTF, SIGNAL(toggled(bool)), this, SLOT(CodirChanged())));


    chkbxcl = new QCheckBox;
    chkbxcl->setText("bold");
    chkbxcl->setStatusTip("font");
    Q_ASSERT(connect(chkbxcl, SIGNAL(toggled(bool)), this, SLOT(WatchChanged())));

    chkbxit = new QCheckBox;
    chkbxit->setText("italics");
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
    combox->addItem("gray");
    Q_ASSERT(connect(combox, SIGNAL(currentIndexChanged(QString)), this, SLOT(WatchChanged())));


    but = new QPushButton;
    but->setText("default");
    but->setStatusTip("return to default setup");
    but->setToolTip("default");
    Q_ASSERT(connect(but, SIGNAL(clicked()), this, SLOT(ToDefault())));


    //сепаратор
    QVBoxLayout *separator1 = new QVBoxLayout;
    separator1->addWidget(rdbutCP);
    separator1->addWidget(rdbutUTF);

    QVBoxLayout *separator2 = new QVBoxLayout;
    separator2->addWidget(chkbxcl);
    separator2->addWidget(chkbxit);
    separator2->addWidget(chkbxun);
    separator2->addWidget(combox);

    QVBoxLayout *separator3 = new QVBoxLayout;
    separator3->addWidget(but);


    QGroupBox *grboxEn = new QGroupBox;
    grboxEn->setTitle("encoding");
    grboxEn->setLayout(separator1);

    QGroupBox *grboxSt = new QGroupBox;
    grboxSt->setTitle("style");
    grboxSt->setLayout(separator2);

    QGroupBox *grboxBut = new QGroupBox;
    grboxBut->setTitle("default");
    grboxBut->setLayout(separator3);


    //чтобы выводилось всё в одной ячейке
    QVBoxLayout *combGrbx = new QVBoxLayout;
    combGrbx->addWidget(grboxEn);
    combGrbx->addWidget(grboxSt);
    combGrbx->addWidget(grboxBut);
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
    lay->addLayout(combGrbx, 1, 4);  //кнопки управления

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
    model->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);  //фильтр

    dirname->setModel(model);
    dirname->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::DoubleClicked);
}


void MainWindow::resetFilesList(QString directoryName)
{
    QDir directory(directoryName);

    if (!directory.exists())
    {
        QMessageBox::information(this, "information", "Файл заблокирован!", QMessageBox::Ok);
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
        QMessageBox::information(this, "information", "Файл заблокирован!", QMessageBox::Ok);
        WatchTxt->clear();
        WatchTxt->setEnabled(false);
        return;
    }

    //проверка на расширение
    if ((filename.indexOf(".txt") == -1) &&
            (filename.indexOf(".ini") == -1))
    {
        QMessageBox::information(this, "information", "Недопустимое расширение", QMessageBox::Ok);
        WatchTxt->clear();
        WatchTxt->setEnabled(false);
        return;
    }

    QTextStream inStream(&inFile);

    inFile.open(QIODevice::ReadOnly | QIODevice::Text);

    if (inStream.status() != QTextStream::Ok)
    {
        QMessageBox::information(this, "information", "Файл заблокирован!", QMessageBox::Ok);
        WatchTxt->clear();
        WatchTxt->setEnabled(false);
        return;
    }

    WatchTxt->clear();
    WatchTxt->setEnabled(true);

    //корректный вывод на экран
    if (rdbutCP->isChecked()) {
        inStream.setCodec(QTextCodec::codecForName("CP1251"));
    }

    if (rdbutUTF->isChecked()) {
        inStream.setCodec(QTextCodec::codecForName("utf8"));
    }

    WatchTxt->insertPlainText(inStream.readAll());

    inFile.close();
}


void MainWindow::quitProgram()
{
    this->close();
}

void MainWindow::About()
{
    QMessageBox::information(this, "About program", "Прохватилов С. С. , М80-213Б-17", QMessageBox::Ok);
}

void MainWindow::ToDefault()
{
    rdbutCP->setChecked(true);
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
        QMessageBox::information(this, "information", "Файл заблокирован!", QMessageBox::Ok);
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
                                 "error"
                                 ,"Ошибка сохранения файла"
                                 ,QMessageBox::Ok
                                 );
    }
    else
    {
        QTextStream stream(&outFile);

        QString strtxt = "";

        if (rdbutCP->isChecked()){
            stream.setCodec(QTextCodec::codecForName("CP1251"));
        }

        if (rdbutUTF->isChecked()) {
            stream.setCodec(QTextCodec::codecForName("utf8"));
        }

        strtxt = WatchTxt->toPlainText();

        stream << strtxt;
    }

    outFile.close();
}

//вызов resetWatch
void MainWindow::WatchChanged()
{
    /* промежуточная проверка, чтобы не сигнализировать на каждое нажатие
    QFile dir(fileDirNew);
    if (!dir.exists())
       return;

    dir.close();

    перерисовываем
    resetWatch(fileDirNew); */

    //обновляем стиль
    txtStyleChanged();
}

void MainWindow::txtStyleChanged()
{
    if (chkbxcl->isChecked())
        WatchTxt->setFont(QFont("System", 8));
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


    QColor color;
    if (!combox->currentText().compare("black")) {
        color.setRgb(0,0,0);  //черный
    }
    else if (!combox->currentText().compare("green")) {
            color.setRgb(0,100,0);
    }
    else if (!combox->currentText().compare("blue")) {
            color.setRgb(0,0,100);
    }
    else if (!combox->currentText().compare("red")) {
        color.setRgb(100,0,0);
    }
    else if (!combox->currentText().compare("gray"))
        color.setRgb(100,100,100);


    WatchTxt->setTextColor(color);

    //перерисовываем весь Watch
    QString str = WatchTxt->toPlainText();
    WatchTxt->clear();
    WatchTxt->setPlainText(str);
}

void MainWindow::CodirChanged()
{
    //перекодировка
    // QMessageBox::information(this,"", QCoreApplication::applicationDirPath());  //путь до скомпилированного файла
    QFile file(fileDirNew);
    file.open(QIODevice::ReadOnly);
    QTextStream  instream(&file);


    // раскодируем в новой
    if (rdbutCP->isChecked()) {
        instream.setCodec(QTextCodec::codecForName("CP1251"));
    }

    if (rdbutUTF->isChecked()) {
        instream.setCodec(QTextCodec::codecForName("utf8"));
    }

    WatchTxt->clear();
    WatchTxt->insertPlainText(instream.readAll());
}
