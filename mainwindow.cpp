#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QDateTime>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include<QVBoxLayout>


#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lcdNumber->setDigitCount(11);
    this->reset();
//    std::cout<<this->phoneList.size()<<"create"<<endl;
    this->confDlg = new QDialog(this);
    this->confDlg->setToolTip(tr("设置随机数量"));
    this->confBox = new QSpinBox(this->confDlg);
    this->confBox->setMinimum(1);
    this->confBox->setWrapping(true);
    QPushButton *btn= new QPushButton(tr("确定"), this->confDlg);
    QVBoxLayout *layout= new QVBoxLayout(this->confDlg);
    layout->addWidget(this->confBox);
    layout->addWidget(btn);
    connect(btn, &QPushButton::clicked, this, &MainWindow::setRandSelectedNum);

    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout, this, &MainWindow::timeUpdate);
    timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}
const void MainWindow::reset(){
    this->isStarted=false;
    this->phoneList.clear();
    this->result.clear();
    this->count=1;
}

bool MainWindow::trigger()
{
    if(this->isStarted){
        this->isStarted = false;
        ui->pushButton->setText("开始");
    }else{
        this->isStarted = true;
        ui->pushButton->setText("停止");
    }
}

void MainWindow::timeUpdate(){
    if(this->isStarted && !this->phoneList.isEmpty()){
//        this->phoneList.append(this->result);
//        this->result.clear();
//        int randNum=0;
//        int randSum = this->phoneList.size();
//        for(int i=0;i<this->count;i++){
//            randNum =qrand()%(randSum -i);
//            this->result.append(this->phoneList.takeAt(randNum));
//        }
        this->index = qrand()%this->phoneList.count();
        double res= this->phoneList.at(this->index).toLong();
        ui->lcdNumber->display(res);
    }
}

bool MainWindow::loadFile(const QString &fileName){
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,tr("prize draw"),
                             tr("无法读取文件 %1:/n %2").arg(fileName).arg(file.errorString()));
        return false;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QStringList list;
    list.clear();
    QTextStream in(&file);
    this->reset();
    while(!in.atEnd()){
        this->phoneList<<in.readLine().split(",", QString::SkipEmptyParts).at(0);
    }
    file.close();
    QApplication::restoreOverrideCursor();
    int maxTemp = this->phoneList.size();
    maxTemp = maxTemp >5? 5:maxTemp;
    this->confBox->setMaximum(maxTemp);
    return true;
}

void MainWindow::openFile()
{
    this->isStarted=false;
    ui->pushButton->setText(tr("开始"));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Excel file"), qApp->applicationDirPath (),
                                                    tr("Files (*.csv)"));
    if (!fileName.isEmpty()){
        this->loadFile(fileName);
    }
}

void MainWindow::on_action_F_triggered()
{
    this->openFile();
}

void MainWindow::on_action_R_triggered()
{
    this->isStarted =false;
    ui->pushButton->setText(tr("开始"));
    this->phoneList.append(this->result);
    this->result.clear();
}

void MainWindow::on_pushButton_clicked()
{
//    std::cout<<this->phoneList.size()<<"on start"<<endl;
    if(this->phoneList.isEmpty()){
        this->openFile();
        return;
    }
    if(this->isStarted){
        this->trigger();
        this->result.append(this->phoneList.takeAt(this->index));
    }else{
        this->trigger();
    }
}


void MainWindow::on_action_W_triggered()
{
    this->close();
}

void MainWindow::on_action_triggered()
{
    this->confDlg->show();
}

void MainWindow::setRandSelectedNum()
{
    this->confDlg->close();
    bool ok=true;
    int temp = this->confBox->text().toInt(&ok,10);
    if(ok){
        this->count = temp;
    }
}
