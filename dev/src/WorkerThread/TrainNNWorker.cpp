#include <WorkerThread/TrainNNWorker.hpp>

#ifdef DEBUG
    #define DEBUG_MSG(str) { m_worker_mutex->lock(); std::cout << "TrainNNWorker::" << str << std::endl; m_worker_mutex->unlock(); }
    #define DEBUG_MSG_S(str) { m_worker_mutex->lock(); std::cout << "TrainNNWorker::" << str; }
    #define DEBUG_MSG_D() { std::cout << " - DONE"<< std::endl; m_worker_mutex->unlock(); }
#else
    #define DEBUG_MSG(str) { }
    #define DEBUG_MSG_S(str) { }
    #define DEBUG_MSG_D() { }
#endif

TrainNNWorker::TrainNNWorker(NeuralNetwork& nn, QMutex* worker_mutex ,QWidget *parent) : 
    QThread(parent), 
    m_nn(nn),
    m_worker_mutex(worker_mutex) 
{}

TrainNNWorker::~TrainNNWorker() {}

void TrainNNWorker::setApplicationWidget(QWidget* appWidget) { m_appWidget = appWidget; }

void TrainNNWorker::run() {
    DEBUG_MSG("TRAIN NETWORK");
    op_done=false;
    emit on_hideBuildRelatedButton(true, &op_done);
    while(!op_done) {}

    int nb_epoch = m_appWidget->findChild<QSpinBox*>("epochs")->value();
    double learning_rate = m_appWidget->findChild<QDoubleSpinBox*>("learningRate")->value();
    if(nb_epoch > 100) m_nn.setLossResultsEach(nb_epoch/100);
    else m_nn.setLossResultsEach(1);
    
    // LOAD DATA
    Matrix X_train(0,0,0.0f), Y_train(0,0,0.0f), X_test(0,0,0.0f), Y_test(0,0,0.0f);
    std::string selected_file{m_appWidget->findChild<QLineEdit*>("choosed_file")->text().toStdString()};
    bool launch_train=false;
    int nb_data = m_appWidget->findChild<QSpinBox*>("nb_data")->value();
    if(m_appWidget->findChild<QTabWidget*>("tabWidget")->currentIndex() == 1 && selected_file != "[Not selected]") {
        loadData(X_train, Y_train, X_test, Y_test, selected_file); 
    } else if(m_appWidget->findChild<QTabWidget*>("tabWidget")->currentIndex() == 0) {
        if(m_appWidget->findChild<QRadioButton*>("linear_data")->isChecked()) {
            X_train = Matrix(2, nb_data); 
            Y_train = Matrix(1, nb_data); 
            X_test = Matrix(2, nb_data); 
            Y_test = Matrix(1, nb_data);
            generateData_Linear(X_train, Y_train);
            generateData_Linear(X_test, Y_test);
        }
        else if(m_appWidget->findChild<QRadioButton*>("balanced_data")->isChecked()) {
            X_train = Matrix(2, nb_data); 
            Y_train = Matrix(1, nb_data); 
            X_test = Matrix(2, nb_data); 
            Y_test = Matrix(1, nb_data);
            generateData_Balanced(X_train, Y_train);
            generateData_Balanced(X_test, Y_test);
        }
        else if(m_appWidget->findChild<QRadioButton*>("circle_data")->isChecked()) {
            X_train = Matrix(2, nb_data); 
            Y_train = Matrix(1, nb_data); 
            X_test = Matrix(2, nb_data); 
            Y_test = Matrix(1, nb_data);
            generateData_Circle(X_train, Y_train);
            generateData_Circle(X_test, Y_test);
        }
        else if(m_appWidget->findChild<QRadioButton*>("class3_data")->isChecked()) {
            X_train = Matrix(2, nb_data); 
            Y_train = Matrix(3, nb_data); 
            X_test = Matrix(2, nb_data); 
            Y_test = Matrix(3, nb_data);
            generateData_3Class(X_train, Y_train);
            generateData_3Class(X_test, Y_test);
        }
    }

    auto list = m_appWidget->findChild<QListLayerWidget*>("listWidget");
    std::string temp = list->item(list->count()-1)->text().toStdString();
    std::istringstream listitem(temp);
    std::string nb_neurons;
    std::getline( listitem, nb_neurons,':');
    std::getline( listitem, nb_neurons,':');
    std::getline( listitem, nb_neurons,':');
    int nb_neuron_last_layer = std::stoi(nb_neurons);
    int features_nb = m_appWidget->findChild<QSpinBox*>("features")->value();

    if(X_train.row()!= features_nb) {
        DEBUG_MSG("TrainNNWorker::WRONG FEATURES NUMBER -> SET FEATURES NUMBERS TO " << X_train.row());
        std::string msg= "The number of entries in this network does not match the number of entries in your data\n\nSet [Nb. features] to " + std::to_string(X_train.row());
        op_done=false;
            emit on_showMessageBox(QString::fromStdString(msg), &op_done);
        while(!op_done) {}
    } else if (nb_neuron_last_layer != Y_train.row()) {
        DEBUG_MSG("There are not enough output neurons\n\n Add a layer with " << Y_train.row() << " NEURON(S)");
        std::string msg= "There are not enough output neurons\n\n Add a layer with " + std::to_string(Y_train.row()) + " neuron(s)";
        op_done=false;
            emit on_showMessageBox(QString::fromStdString(msg), &op_done);
        while(!op_done) {}
    } else {
        m_nn.train(X_train, Y_train, nb_epoch, learning_rate, false);
        m_nn.predict(X_test, Y_test);
        op_done=false;
            emit on_updateChart(&m_nn, &op_done);
        while(!op_done) {}
    }

    op_done=false;
    emit on_hideBuildRelatedButton(false, &op_done);
    while(!op_done) {}
    DEBUG_MSG("TRAIN NETWORK - DONE");
}