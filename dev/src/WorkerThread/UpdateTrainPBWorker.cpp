#include <WorkerThread/UpdateTrainPBWorker.hpp>

#ifdef DEBUG
    #define DEBUG_MSG(str) { m_worker_mutex->lock(); std::cout << "UpdateTrainPBWorker::" << str << std::endl; m_worker_mutex->unlock(); }
    #define DEBUG_MSG_S(str) { m_worker_mutex->lock(); std::cout << "UpdateTrainPBWorker::" << str; }
    #define DEBUG_MSG_D() { std::cout << " - DONE"<< std::endl; m_worker_mutex->unlock(); }
#else
    #define DEBUG_MSG(str) { }
    #define DEBUG_MSG_S(str) { }
    #define DEBUG_MSG_D() { }
#endif

UpdateTrainPBWorker::UpdateTrainPBWorker(QMutex* worker_mutex, QWidget *parent) : m_worker_mutex(worker_mutex), QThread(parent){}

UpdateTrainPBWorker::~UpdateTrainPBWorker() {}

void UpdateTrainPBWorker::run() {
    DEBUG_MSG("START TRAIN PB UPDATE WORKER");
    exit=false;
    while(!exit) {
        op_done=false;
        emit on_updateTrainPB(&op_done);
        while(!op_done) {}
        usleep(30);
    }
    DEBUG_MSG("STOP TRAIN PB UPDATE WORKER");
}

void UpdateTrainPBWorker::stopProcess() { exit=true; }