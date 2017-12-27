#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSpinBox>
#include<QLCDNumber>
#include<QGridLayout>

#define MAX_RANGDOM_COUNT 5

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
    void timeUpdate();
    void on_action_F_triggered();
    void on_action_R_triggered();

    void on_pushButton_clicked();

    void on_action_W_triggered();

    void on_action_triggered();

    void setRandSelectedNum();

private:
    Ui::MainWindow *ui;
    QDialog *confDlg;
    QSpinBox *confBox;

    QList<QLCDNumber *> lcdDisplays;
    QList<QString> phoneList;
    QString curlFile;
    QList<QString> result;
    bool isStarted;
    int index;
    int count;
    bool loadFile(const QString &fileName);
    void openFile();
    const void reset();
    const void initLcdDisplay();
    bool trigger();
};

#endif // MAINWINDOW_H
