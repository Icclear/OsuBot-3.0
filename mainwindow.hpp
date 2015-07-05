#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <utility>
#include "program.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_SongSearch_textChanged();

    void on_SongList_itemSelectionChanged();

    void on_RelaxEnabled_stateChanged(int arg1);

    void on_AutoEnabled_toggled(bool checked);

    void on_RelaxEnabled_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    Program program;
    std::vector<std::string> *SongFolderList;
    std::vector<std::string> Diffs;
};

#endif // MAINWINDOW_HPP
