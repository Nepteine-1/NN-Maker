#ifndef TRAINNNWORKER_HPP
#define TRAINNNWORKER_HPP

#include <fstream> 
#include <QFileDialog>
#include <QThread>
#include <QWidget>
#include <sstream>

#include <QtUtils/UiLoader.hpp>
#include <NeuralNetwork/NeuralNetwork.hpp>

class TrainNNWorker : public QThread {
    Q_OBJECT

    public: 
        TrainNNWorker(NeuralNetwork& nn, QMutex* worker_mutex, QWidget *parent = nullptr);
        ~TrainNNWorker();
        void setApplicationWidget(QWidget* appWidget);
    
    protected:
        void run() override;
    
    signals:
        void on_updateChart(NeuralNetwork*, bool*);
        void on_hideBuildRelatedButton(bool, bool*);
        void on_showMessageBox(QString, bool*);

    private:
        NeuralNetwork& m_nn;
        QWidget* m_appWidget;
        bool op_done=false;
        QMutex* m_worker_mutex;
};

#endif