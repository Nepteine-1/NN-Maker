#ifndef UTPBW_HPP
#define UTPBW_HPP

#include <QThread>
#include <QMutex>
#include <QWidget>
#include <iostream>
#include <unistd.h>

class UpdateTrainPBWorker : public QThread {
    Q_OBJECT

    public:
        UpdateTrainPBWorker(QMutex* worker_mutex, QWidget *parent = nullptr);
        ~UpdateTrainPBWorker();
        void stopProcess();

    protected:
        void run() override;

    signals:
        void on_updateTrainPB(bool*);

    private:
        bool op_done=false;
        bool exit=false;
        QMutex* m_worker_mutex;
};

#endif