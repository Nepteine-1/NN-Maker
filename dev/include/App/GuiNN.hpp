#ifndef GUINN_HPP
#define GUINN_HPP

#include <QFileDialog>
#include <QMutexLocker>
#include <QMutex>
#include <QWidget>
#include <sstream>
#include <unistd.h>

#include <NeuralNetwork/NeuralNetwork.hpp>
#include <QtUtils/UiLoader.hpp>
#include <WorkerThread/BuildNNWorker.hpp>
#include <WorkerThread/TrainNNWorker.hpp>
#include <WorkerThread/UpdateTrainPBWorker.hpp>

class GuiNN : public QWidget{
    Q_OBJECT

    public:
        GuiNN(void);
        ~GuiNN(void);

    signals:
        void on_QApp_addLayer(int, QString);
        void on_QApp_clear(void);

    private slots:
        void slot_addLayer(void);
        void slot_clear(void);
        void slot_build(void);
        void slot_train(void);
        void slot_browse_file(void);
        void slot_update_progressBar(int, bool*);
        void slot_show_progressBar(bool, bool*);
        void slot_show_trainBtn(bool, bool*);
        void slot_update_formatText_progressBar(QString, bool*);
        void slot_updateCharts(NeuralNetwork*, bool*);
        void slot_buildProcessTerminate();
        void slot_hideBuildRelatedButton(bool, bool*);
        void slot_updateTrainPB(bool*);
        void slot_showMessageBox(QString, bool*);

    private:
        QWidget* m_customWidget;
        NeuralNetwork m_nn;
        BuildNNWorker m_build_worker;
        TrainNNWorker m_train_worker;
        UpdateTrainPBWorker m_trainpb_worker;
        QMutex global_mutex;
};

#endif