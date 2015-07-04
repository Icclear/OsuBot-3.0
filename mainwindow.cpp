#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    //Todo: Caption

    if(program.getError() != 0)
        close();

    SongFolderList = program.getSongList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{    
    if(ui->DiffList->currentItem() == (NULL || nullptr))
        return; //Nothing selected..:<

    std::string DiffName = ui->DiffList->currentItem()->text().toStdString();

     for(auto &i : Diffs)
     {
         if(i.find(DiffName) != std::string::npos)
         {
             DiffName = i;
             break;
         }
         else if(DiffName.compare("[]") == 0 && i.find("[") == std::string::npos)   //no diffname
         {
             DiffName = i;
             break;
         }
     }

     program.LoadBeatmap(program.getSongPath() +
                         ui->SongList->currentItem()->text().toStdString() + "\\" +
                         DiffName);
}

void MainWindow::on_SongSearch_textChanged()
{
    ui->SongList->clear();
    ui->DiffList->clear();

    std::string TextBox = ui->SongSearch->toPlainText().toStdString();
    std::transform(TextBox.begin(), TextBox.end(), TextBox.begin(), ::tolower);


    for(auto &i : *SongFolderList)
    {
        std::string tmp = i;
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);

        if(tmp.find(TextBox) != std::string::npos)
            ui->SongList->addItem(i.c_str());
    }
}

void MainWindow::on_SongList_itemSelectionChanged()
{
    std::string BeatmapDir = program.getSongPath() + ui->SongList->currentItem()->text().toStdString();

    ui->DiffList->clear();
    Diffs.clear();

     char szBuffer[MAX_PATH];
     GetCurrentDirectory(MAX_PATH, szBuffer);

     SetCurrentDirectory(BeatmapDir.c_str());

     WIN32_FIND_DATA fd;
     HANDLE hFind = FindFirstFile("*.osu", &fd);

     // Get all sub-folders:
     if (hFind != INVALID_HANDLE_VALUE)
     {
         do
         {
             std::string pszName = fd.cFileName;
             unsigned int difPos = pszName.find("[");

             if(difPos == std::string::npos)
             {
                 ui->DiffList->addItem("[]");
             }
             else
             {
                std::string temp = pszName.substr(difPos, pszName.length() - difPos - 4);
                ui->DiffList->addItem(temp.c_str());
             }

             Diffs.push_back(pszName);

         } while (FindNextFile(hFind, &fd));

         FindClose(hFind);
     }

     // Set the current folder back to what it was:
     SetCurrentDirectory(szBuffer);
}
