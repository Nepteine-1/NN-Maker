#include <WorkerThread/BuildNNWorker.hpp>

#ifdef DEBUG
    #define DEBUG_MSG(str) { m_worker_mutex->lock(); std::cout << "BuildNNWorker::" << str << std::endl; m_worker_mutex->unlock(); }
    #define DEBUG_MSG_S(str) { m_worker_mutex->lock(); std::cout << "BuildNNWorker::" << str; }
    #define DEBUG_MSG_D() { std::cout << " - DONE"<< std::endl; m_worker_mutex->unlock(); }
#else
    #define DEBUG_MSG(str) { }
    #define DEBUG_MSG_S(str) { }
    #define DEBUG_MSG_D() { }
#endif

BuildNNWorker::BuildNNWorker(NeuralNetwork& nn, QMutex* worker_mutex ,QWidget *parent) : 
    QThread(parent), 
    m_nn(nn),
    m_worker_mutex(worker_mutex) 
{}

BuildNNWorker::~BuildNNWorker() {}

void BuildNNWorker::setApplicationWidget(QWidget* appWidget) { m_appWidget = appWidget; }

void BuildNNWorker::run() {
    op_done = false;
    emit on_show_trainBtn(false, &op_done);
    while(!op_done) {}

    op_done = false;
    emit on_update_progressBar(0, &op_done);
    while(!op_done) {}

    op_done = false;
    emit on_update_formatText_progressBar("%p%", &op_done);
    while(!op_done) {}

    op_done = false;
    emit on_show_progressBar(true, &op_done);
    while(!op_done) {}

    DEBUG_MSG("START BUILDING PROCESS");
    QListLayerWidget* list = m_appWidget->findChild<QListLayerWidget*>("listWidget");
    float nb_layer = list->count();
    
    m_nn.clear();
    QSpinBox* nbFeatures = m_appWidget->findChild<QSpinBox*>("features");
    m_nn.setNbFeatures(nbFeatures->text().toInt());
    
    for(int i=0; i<nb_layer; i++) {
        DEBUG_MSG("CHANGE PROGRESS BAR VALUE: " << int((i/nb_layer)*100));
        op_done = false;
        emit on_update_progressBar(int((i/nb_layer)*100), &op_done);
        while(!op_done) {}
        std::istringstream oss(list->item(i)->text().toStdString());
        std::string word;

        getline(oss, word, ':');
        getline(oss, word, ':');
        int activation_type=0;
        if(word == "Sigmoid") activation_type = 0;
        else if (word == "ELU") activation_type = 1;
        else if (word == "RELU") activation_type = 2;

        getline(oss, word, ':');
        int neurons = std::stoi(word);

        m_nn.addLayer(neurons, static_cast<Activation>(activation_type));
    }

    op_done = false;
    emit on_update_progressBar(100, &op_done);
    while(!op_done) {}

    op_done = false;
    emit on_update_formatText_progressBar("Ready", &op_done);
    while(!op_done) {}

    QComboBox* lossFunc = m_appWidget->findChild<QComboBox*>("loss");
    m_nn.setLossFunction(static_cast<LossFunction>(lossFunc->currentIndex()));
    
    op_done = false;
    emit on_show_trainBtn(true, &op_done);
    while(!op_done) {}

    DEBUG_MSG("BUILD_NEURAL_NETWORK - DONE");
}