#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>        // For file work
#include <QFileDialog>  // For "Browse file" button functionality
#include <QTextStream>  // For editing files
#include <QMessageBox>  // For pop-up message windows
#include <QPixmap>
#include <QDesktopServices>
#include <QUrl>

#include "patch.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_soundButton_clicked();

    void on_creditsButton_clicked();

    void on_exitButton_clicked();

    void on_patchButton_clicked();

    void on_browseButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
    MyString* openedCode = nullptr;
    void SetBackground(const char* filename);
    void SetButtonIcon(QPushButton* button, const char* filename);
    void SetLabelImg(QLabel* label, const char* filename);
};
#endif // MAINWINDOW_H
