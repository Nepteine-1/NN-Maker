#ifndef BUILDNNWORKER_HPP
#define BUILDNNWORKER_HPP

#include <fstream> 
#include <QFileDialog>
#include <QThread>
#include <QWidget>

#include <QtUtils/UiLoader.hpp>
#include <NeuralNetwork/NeuralNetwork.hpp>

class BuildNNWorker : public QThread {
    Q_OBJECT

    public: 
        BuildNNWorker(NeuralNetwork& nn, QMutex* worker_mutex, QWidget *parent = nullptr);
        ~BuildNNWorker();
        void setApplicationWidget(QWidget* appWidget);
    
    protected:
        void run() override;
    
    signals:
        void on_update_progressBar(int, bool*);
        void on_show_progressBar(bool, bool*);
        void on_show_trainBtn(bool, bool*);
        void on_update_formatText_progressBar(QString, bool*);

    private:
        NeuralNetwork& m_nn;
        QWidget* m_appWidget;
        bool op_done=false;
        QMutex* m_worker_mutex;
};

#endif