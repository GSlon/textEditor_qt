#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QListWidget>
#include <QTreeView>
#include <QVBoxLayout>
#include <QHBoxLayout>
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
#include "mygraphicview.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QStringListModel>
#include <QTreeWidgetItem>
#include <QWindow>
#include <QMessageBox>


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

    Ui::MainWindow *ui;

    MyGraphicView *view;
    QHBoxLayout *lay;
    QTreeView *tree;
    QStandardItemModel *model;
    QWidget *widget;

private slots:
    void quitProgram();
    void About();
    void Help();

};

#endif // MAINWINDOW_H
