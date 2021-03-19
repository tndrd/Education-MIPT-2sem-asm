#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    system("mpg123 /home/tndnrd/Projects/Education-MIPT-2sem-asm/HackEgor/Crack/EgorCrack/Assets/Барбарики.mp3 &");
    ui->setupUi(this);
    ui->saveButton->setEnabled(false);

    SetBackground("../EgorCrack/Assets/gavkoshmyg.jpg");
    SetButtonIcon(ui->soundButton, "../EgorCrack/Assets/mute.png");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetBackground(const char* filename)
{
    // background
    QPixmap bground (filename);
    bground = bground.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette = this -> palette();
    palette.setBrush(QPalette::Background, bground);
    this->setPalette(palette);

}

void MainWindow::SetLabelImg(QLabel* label, const char* filename)
{
    QPixmap image(filename);
    image = image.scaled(label->size(), Qt::IgnoreAspectRatio);
    label->setPixmap(image);
}

void MainWindow::SetButtonIcon(QPushButton* button, const char* filename)
{
    QPixmap pixmap (filename);
    pixmap = pixmap.scaled(button->size(), Qt::IgnoreAspectRatio);
    QIcon   icon   (pixmap);
    button->setIcon(icon);
}

void MainWindow::on_creditsButton_clicked()
{
    QString link = "https://github.com/tinderad";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_soundButton_clicked()
{
    SetLabelImg(ui -> logo_label, "../EgorCrack/Assets/smile.png");
    system("amixer sset 'Master' 100%");
}

void MainWindow::on_exitButton_clicked()
{
    system("mpg123 /home/tndnrd/Projects/Education-MIPT-2sem-asm/HackEgor/Crack/EgorCrack/Assets/fart.mp3");
    exit(0);
}

void MainWindow::on_patchButton_clicked()
{
    ui->saveButton->setEnabled(false);
    openedCode = nullptr;

    QByteArray filename_bytes = ui->lineEdit->text().toLocal8Bit();
    printf("Opening file: %s\n", filename_bytes.data());

    MyString* code = ReadFile(filename_bytes.data());

    if(!code)
    {
        QMessageBox::warning(this, "Fatal error", "Failed to start file processing."); return;
    }

    switch(code -> status)
    {
        case MyString::STRING_FILE_NOT_FOUND:    QMessageBox::warning(this, "Error", "Can't open file: doesn't exist."); free(code); return;
        case MyString::STRING_NOT_ENOUGH_MEMORY: QMessageBox::warning(this, "Error", "Can't open file: too large.");     free(code); return;
        case MyString::STRING_OK: break;
    }

    switch(DoPatch(code))
    {
        case PATCH_WRONG_FILESIZE: QMessageBox::warning(this, "Error", "Patching failed: wrong file hash."); free(code); return;
        case PATCH_WRONG_HASH:     QMessageBox::warning(this, "Error", "Patching failed: wrong file size."); free(code); return;
        case PATCH_OK: break;
    }

    QMessageBox::information(this, "Success!", "File has patched. You may save it now.");
    ui -> lineEdit -> setText("");
    openedCode = code;
    ui->saveButton->setEnabled(true);

}

void MainWindow::on_browseButton_clicked()
{
    ui -> lineEdit -> setText(QFileDialog::getOpenFileName(this, "Select file"));
}

void MainWindow::on_saveButton_clicked()
{
    assert(openedCode);

    QByteArray filename_bytes = ui->lineEdit->text().toLocal8Bit();
    printf("Saving file: %s\n", filename_bytes.data());

    if (SaveFile(openedCode, filename_bytes.data()) == 1)
    {
        QMessageBox::warning(this, "Error", "Failed to save patched code."); return;
    }

    QMessageBox::information(this, "Sucess", "Patched code saved sucessfully.");
    return;
}
