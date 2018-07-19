#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "systemrun.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //fixed window size
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());

    //oepn drops
    ui->lineEdit->setAcceptDrops(false);
    ui->textEdit->setAcceptDrops(false);
    setAcceptDrops(true);

    //menubar
    ui->menuBar->addAction("配置");
    ui->menuBar->addAction("ARM转机器码");
    ui->menuBar->addAction("打开JADX");
    ui->menuBar->addAction("打开JD-GUI");
    ui->menuBar->addMenu("关于")->addAction("开源地址");
    connect(ui->menuBar,SIGNAL(triggered(QAction*)),this,SLOT(trigerMenu(QAction*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return;
    this->dFile=urls.first().toLocalFile();
    ui->lineEdit->setText(this->dFile);
}

void MainWindow::on_pushButton_clicked()
{
    //Select apk file
    QUrl filePath = QFileDialog::getOpenFileUrl(this,"选择APK文件",QUrl("."),"APK Files(*.apk)");
    if(filePath.isEmpty())
        return;
    this->dFile = filePath.toLocalFile();
    ui->lineEdit->setText(filePath.toLocalFile());
}

void MainWindow::on_pushButton_2_clicked()
{
    QFileInfo apkinfo(this->dFile);
    if(apkinfo.isFile()&& apkinfo.suffix() == "apk")
    {
        SystemRun *jadx = new SystemRun(this);
        connect(jadx,SIGNAL(updateLog(QString)),this,SLOT(on_updateLog(QString)));
        QString outDir = apkinfo.filePath().remove(apkinfo.filePath().length() - apkinfo.suffix().length() - 1,apkinfo.suffix().length() + 1);
        jadx->SetCommand("java -jar .\\tool\\apktool.jar -o " + outDir + " d " + this->dFile);
        jadx->start();
    }
    else
        QMessageBox::information(NULL, "提示", "未发现APK文件", QMessageBox::Ok);
}

void MainWindow::on_updateLog(QString LogText)
{
    ui->textEdit->append(LogText);
}

void MainWindow::trigerMenu(QAction* act)
{
    if(act->text() == "配置")
    {
        QMessageBox::information(NULL, "提示", "开发ing", QMessageBox::Ok);
        return;
    }
    else if(act->text() == "ARM转机器码")
    {
        QMessageBox::information(this, "提示", "开发ing", QMessageBox::Ok);
        return;
    }
    else if(act->text() == "打开JADX")
    {
        SystemRun *dexDecompile = new SystemRun(this);
        connect(dexDecompile,SIGNAL(updateLog(QString)),this,SLOT(on_updateLog(QString)));
#ifdef Q_OS_WIN
        dexDecompile->SetCommand(".\\tool\\jadx-0.7.1\\bin\\jadx-gui.bat");
#else
        dexDecompile->SetCommand(".\\tool\\jadx-0.7.1\\bin\\jadx-gui");
#endif
        dexDecompile->start();
    }
    else if(act->text() == "打开JD-GUI")
    {
        //jd-gui-1.4.0.jar
        SystemRun *jd_gui = new SystemRun(this);
        connect(jd_gui,SIGNAL(updateLog(QString)),this,SLOT(on_updateLog(QString)));
        jd_gui->SetCommand("java -jar .\\tool\\jd-gui-1.4.0.jar");
        jd_gui->start();
    }
    else if(act->text() == "开源地址")
    {
        QString URL = "https://github.com/BackTrackCRoot/AndroidToolBox";
        QDesktopServices::openUrl(QUrl(URL.toLatin1()));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QFileInfo apkinfo(this->dFile);
    if(apkinfo.isDir())
    {
        QUrl saveFilePath = QFileDialog::getSaveFileUrl(this,"保存APK文件",QUrl("."),"APK Files(*.apk)");
        if(saveFilePath.isEmpty())
            return;
        SystemRun *BuildApk = new SystemRun(this);
        connect(BuildApk,SIGNAL(updateLog(QString)),this,SLOT(on_updateLog(QString)));
        BuildApk->SetCommand("java -jar .\\tool\\apktool.jar -o " + saveFilePath.toLocalFile() + " b " + this->dFile);
        BuildApk->start();
    }
    else
    {
        QMessageBox::information(this,"提示","不是有效目录",QMessageBox::Ok);
    }
}
