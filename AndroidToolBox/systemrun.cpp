#include "systemrun.h"

SystemRun::SystemRun(QObject *parent): QThread(parent)
{
    this->CommandPro = new QProcess(this);
}

void SystemRun::run(){
    if(this->Command.isEmpty() || this->Command.isNull())
    {
        return;
    }
    connect(this->CommandPro,SIGNAL(readyReadStandardOutput()),this,SLOT(readFromStdOut()));
    connect(this->CommandPro,SIGNAL(readyReadStandardError()),this,SLOT(readFromStdError()));
    this->CommandPro->start(this->Command);
    this->CommandPro->waitForStarted();
    this->CommandPro->waitForFinished();
    emit updateLog("Ok.");
}

int SystemRun::SetCommand(QString cmd)
{
    this->Command = cmd;
    return 0;
}

void SystemRun::readFromStdOut()
{
    emit updateLog(this->CommandPro->readAllStandardOutput());
}

void SystemRun::readFromStdError()
{
    emit updateLog(this->CommandPro->readAllStandardError());
}
