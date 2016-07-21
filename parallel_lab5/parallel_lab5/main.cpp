#include <QCoreApplication>
#include <omp.h>
#include <QTextStream>
#include <QTimer>
#include <QThread>
#include <QObject>
using namespace std;

class Worker : public QObject {
 Q_OBJECT
 public:
  Worker();
  virtual ~Worker();
 public slots:
     void process();

 signals:
     void finished();
     //void error(QString err);
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#pragma omp parallel
 {
QTextStream(stdout) << "Hi" << endl;

 }
    QTextStream(stdout) << "" << endl;
    // Создание потока
    QThread* thread = new QThread;
    Worker* worker = new Worker();
    // Передаем права владения "рабочим" классом, классу QThread.
    worker->moveToThread(thread);
    // Связываем сигнал об ошибки со слотом обработки ошибок(не показан).
    //QObject::connect(worker, SIGNAL(error(QString)), this, SLOT(errorHandler(QString)));

    // Соединяем сигнал started потока, со слотом process "рабочего" класса, т.е. начинается выполнение нужной работы.
    QObject::connect(thread, SIGNAL(started()), worker, SLOT(process()));

    // По завершению выходим из потока, и удаляем рабочий класс
    QObject::connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

    // Удаляем поток, после выполнения операции
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
    return a.exec();
}
