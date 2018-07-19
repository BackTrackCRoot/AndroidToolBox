#ifndef SYSTEMRUN_H
#define SYSTEMRUN_H
#include <QThread>
#include <QProcess>

class SystemRun: public QThread
{
    Q_OBJECT

signals:
    void updateLog(QString logText);
public:
    SystemRun(QObject *parent = nullptr);
    int SetCommand(QString cmd);
private slots:
    void readFromStdOut();
    void readFromStdError();
private:
    void run() override;
    QProcess *CommandPro;
    QString Command;
};

#endif // SYSTEMRUN_H
