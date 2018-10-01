#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTreeView>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QComboBox>
#include <QStringList>
#include <QDebug>
#include <QtDebug>
#include <QMenu>
#include <QSplitter>
#include <QDir>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QTextStream>
#include <QTextCodec>
#include <QMessageBox>
#include <QAction>
#include <QModelIndex>
#include <QDirModel>
#include <QFlag>
#include <QFlags>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QVariant>
#include <QFileInfo>
#include <QTextEdit>
#include <QState>
#include <QFont>
#include <QColor>
#include <QGroupBox>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupControls();
    void addMenu();
    void initDirectoriesTree();
    void resetFilesList(QString directoryName);
    void resetWatch(QString fileName);
    void txtStyleChanged();

    Ui::MainWindow *ui;
    QListWidget *filename;
    QTextEdit *WatchTxt;
    QTreeView *dirname;
    QRadioButton *rdbutUTF, *rdbutCP;
    QCheckBox *chkbxcl, *chkbxit, *chkbxun;  //полужирный, курсив, подчёркнутый
    QComboBox *combox;
    QPushButton *but;
    QFileSystemModel  *model;
    QString fileDir, fileDirNew;


private slots:
    void quitProgram();
    void About();
    void SaveAs();
    void ToDefault();
    void dirClicked(const QModelIndex& index);
    void WatchChanged();


};

#endif // MAINWINDOW_H
