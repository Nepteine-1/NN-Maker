#include <App/GuiNN.hpp>

#ifdef DEBUG
    #define DEBUG_MSG(str) { global_mutex.lock(); std::cout << "QApp::" << str << std::endl; global_mutex.unlock(); }
    #define DEBUG_MSG_S(str) { global_mutex.lock(); std::cout << "QApp::" << str; }
    #define DEBUG_MSG_D() { std::cout << " - DONE"<< std::endl; global_mutex.unlock(); }
#else
    #define DEBUG_MSG(str) { }
    #define DEBUG_MSG_S(str) { }
    #define DEBUG_MSG_D() { }
#endif

GuiNN::GuiNN(void) : 
    m_nn(0, LossFunction::BINARY_CROSS_ENTROPY), 
    QWidget(nullptr),
    global_mutex(),
    m_build_worker(m_nn, &global_mutex),
    m_train_worker(m_nn, &global_mutex),
    m_trainpb_worker(&global_mutex)
{
    setMouseTracking(true);
    DEBUG_MSG_S("LOAD_GUI");
    QFile file("./assets/gui/network_setup_window.ui");
    file.open(QFile::ReadOnly);
    UiLoader loader;
    m_customWidget = loader.load(&file);
    file.close();
    DEBUG_MSG_D();

    m_customWidget->findChild<QLineEdit*>("choosed_file")->setText("assets/data/data_example.txt");

    m_build_worker.setApplicationWidget(m_customWidget);
    m_train_worker.setApplicationWidget(m_customWidget);

    QScreen* wid = QGuiApplication::primaryScreen();
    int screenWidth = wid->geometry().width();
    int screenHeight = wid->geometry().height();
    m_customWidget->setGeometry((screenWidth/2)-(m_customWidget->width()/2),(screenHeight/2)-(m_customWidget->height()/2),m_customWidget->width(),m_customWidget->height());
    m_customWidget->show();

    DEBUG_MSG_S("CONNECT_SIGNALS/SLOTS");
    QPushButton* btn = m_customWidget->findChild<QPushButton*>("addLayer");
    connect(btn, SIGNAL(clicked()), this, SLOT(slot_addLayer()));
    connect(btn, SIGNAL(clicked()), &m_build_worker, SLOT(terminate()));
    connect(btn, SIGNAL(clicked()), this, SLOT(slot_buildProcessTerminate()));

    m_customWidget->findChild<QProgressBar*>("pbar_build")->setVisible(false);
    m_customWidget->findChild<QProgressBar*>("pbar_train")->setVisible(false);

    btn = m_customWidget->findChild<QPushButton*>("clear");
    connect(btn, SIGNAL(clicked()), &m_train_worker, SLOT(terminate()));
    connect(btn, SIGNAL(clicked()), this, SLOT(slot_clear()));
    connect(btn, SIGNAL(clicked()), m_customWidget->findChild<QChartWidget*>("chart"), SLOT(slot_resetChart()));
    connect(btn, SIGNAL(clicked()), &m_build_worker, SLOT(terminate()));
    connect(btn, SIGNAL(clicked()), this, SLOT(slot_buildProcessTerminate()));

    btn = m_customWidget->findChild<QPushButton*>("build");
    connect(btn, SIGNAL(clicked()), this, SLOT(slot_build()));

    btn = m_customWidget->findChild<QPushButton*>("train");
    connect(btn, SIGNAL(clicked()), this, SLOT(slot_train()));

    btn = m_customWidget->findChild<QPushButton*>("browse");
    connect(btn, SIGNAL(clicked()), this, SLOT(slot_browse_file()));

    QComboBox* cbox = m_customWidget->findChild<QComboBox*>("loss");
    connect(cbox, SIGNAL(activated(int)), &m_build_worker, SLOT(terminate()));
    connect(cbox, SIGNAL(activated(int)), this, SLOT(slot_buildProcessTerminate()));

    QSpinBox* sbox = m_customWidget->findChild<QSpinBox*>("features");
    connect(sbox, SIGNAL(valueChanged(int)), &m_build_worker, SLOT(terminate()));
    connect(sbox, SIGNAL(valueChanged(int)), this, SLOT(slot_buildProcessTerminate()));

    QListLayerWidget* list= m_customWidget->findChild<QListLayerWidget*>("listWidget");
    connect(this, SIGNAL(on_QApp_addLayer(int, QString)), list, SLOT(slotAddLayerBtnClicked(int, QString)));
    connect(this, SIGNAL(on_QApp_clear(void)), list, SLOT(slotClearBtnClicked(void)));

    connect(&m_build_worker, SIGNAL(on_update_progressBar(int, bool*)), this, SLOT(slot_update_progressBar(int, bool*)));
    connect(&m_build_worker, SIGNAL(on_show_progressBar(bool, bool*)), this, SLOT(slot_show_progressBar(bool, bool*)));
    connect(&m_build_worker, SIGNAL(on_show_trainBtn(bool, bool*)), this, SLOT(slot_show_trainBtn(bool, bool*)));
    connect(&m_build_worker, SIGNAL(on_update_formatText_progressBar(QString, bool*)), this, SLOT(slot_update_formatText_progressBar(QString, bool*)));

    connect(&m_train_worker, SIGNAL(on_updateChart(NeuralNetwork*, bool*)), this, SLOT(slot_updateCharts(NeuralNetwork*, bool*)));
    connect(&m_train_worker, SIGNAL(on_hideBuildRelatedButton(bool, bool*)), this, SLOT(slot_hideBuildRelatedButton(bool, bool*)));
    connect(&m_train_worker, SIGNAL(on_showMessageBox(QString, bool*)), this, SLOT(slot_showMessageBox(QString, bool*)));

    connect(&m_trainpb_worker, SIGNAL(on_updateTrainPB(bool*)), this, SLOT(slot_updateTrainPB(bool*)));
    DEBUG_MSG_D();
}

GuiNN::~GuiNN(void) {}

// SLOTS

void GuiNN::slot_addLayer(void) {
    DEBUG_MSG_S("ADD_LAYER");
    int nb_neurons = m_customWidget->findChild<QSpinBox*>("neurons")->text().toInt();
    QString activation = m_customWidget->findChild<QComboBox*>("activ")->currentText();
    emit on_QApp_addLayer(nb_neurons, activation);
    m_customWidget->findChild<QListLayerWidget*>("listWidget")->scrollToBottom();
    DEBUG_MSG_D();  
}

void GuiNN::slot_clear(void) {
    DEBUG_MSG("CLEAR_NEURAL_NETWORK");
    emit on_QApp_clear();
    m_nn.clear();
}

void GuiNN::slot_build(void) {
    DEBUG_MSG("BUILD_NEURAL_NETWORK");
    m_build_worker.start();
}

void GuiNN::slot_train(void) {
    DEBUG_MSG("TRAIN_NEURAL_NETWORK");
    m_train_worker.start();
}

void GuiNN::slot_browse_file(void) {
    DEBUG_MSG("SELECT DATA FILE");
    QFileDialog dialog(this);
    dialog.setOption(QFileDialog::DontUseNativeDialog);
    QString file_selected = dialog.getOpenFileName(this, "Choose neural network", "/home", "All (*.*)");
    if(!file_selected.isEmpty()&& !file_selected.isNull()) {
        m_customWidget->findChild<QLineEdit*>("choosed_file")->setText(file_selected);
        DEBUG_MSG("FILE SELECTED: " << file_selected.toStdString());
    }
    else {
        DEBUG_MSG("FILE NOT SELECTED OR NOT FOUND");
    } 
}

void GuiNN::slot_update_progressBar(int value, bool* updated) {
    DEBUG_MSG_S("UPDATE PROGRESS BAR " << value);
    m_customWidget->findChild<QProgressBar*>("pbar_build")->setValue(value);
    m_customWidget->findChild<QProgressBar*>("pbar_build")->update();
    *updated = true; 
    DEBUG_MSG_D();
}

void GuiNN::slot_show_progressBar(bool show, bool* updated) {
    DEBUG_MSG_S((show == true ? "SHOW" : "HIDE") << " PROGRESS BAR");
    m_customWidget->findChild<QProgressBar*>("pbar_build")->setVisible(show);
    m_customWidget->findChild<QPushButton*>("build")->setVisible(!show);
    *updated = true; 
    DEBUG_MSG_D();
}

void GuiNN::slot_show_trainBtn(bool show, bool* updated) {
    DEBUG_MSG_S((show == true ? "SHOW" : "HIDE") << " TRAIN BUTTON");
    m_customWidget->findChild<QPushButton*>("train")->setEnabled(show);
    *updated = true; 
    DEBUG_MSG_D();
}

void GuiNN::slot_buildProcessTerminate() {
    if(m_customWidget->findChild<QListLayerWidget*>("listWidget")->count() > 0) m_customWidget->findChild<QPushButton*>("build")->setEnabled(true);
    else m_customWidget->findChild<QPushButton*>("build")->setEnabled(false);
    m_customWidget->findChild<QPushButton*>("build")->setVisible(true);
    m_customWidget->findChild<QProgressBar*>("pbar_build")->setVisible(false);
    m_customWidget->findChild<QPushButton*>("train")->setEnabled(false);
}

void GuiNN::slot_update_formatText_progressBar(QString format, bool* updated) {
    DEBUG_MSG_S("SET PROGRESS BAR TEXT FORMAT: " << format.toStdString());
    m_customWidget->findChild<QProgressBar*>("pbar_build")->setFormat(format);
    *updated = true; 
    DEBUG_MSG_D();
}

void GuiNN::slot_updateCharts(NeuralNetwork*, bool* updated) {
    DEBUG_MSG_S("UPDATE CHARTS");
    m_customWidget->findChild<QChartWidget*>("chart")->showLossResults(&m_nn);
    *updated = true;
    DEBUG_MSG_D();
}

void GuiNN::slot_hideBuildRelatedButton(bool value,bool* updated) {
    if(value) {
        DEBUG_MSG("HIDE BUILD RELATED BUTTON");
        m_customWidget->findChild<QProgressBar*>("pbar_train")->setValue(0);
        m_customWidget->findChild<QProgressBar*>("pbar_train")->update();
        m_customWidget->findChild<QPushButton*>("addLayer")->setEnabled(false);
        m_customWidget->findChild<QPushButton*>("clear")->setEnabled(false);
        m_customWidget->findChild<QSpinBox*>("features")->setEnabled(false);
        m_customWidget->findChild<QComboBox*>("loss")->setEnabled(false);
        m_customWidget->findChild<QPushButton*>("train")->setVisible(false);
        m_customWidget->findChild<QProgressBar*>("pbar_train")->setVisible(true);
        m_customWidget->findChild<QPushButton*>("browse")->setEnabled(false);
        m_trainpb_worker.start();
    } else {
        DEBUG_MSG("SHOW BUILD RELATED BUTTON");
        m_customWidget->findChild<QPushButton*>("addLayer")->setEnabled(true);
        m_customWidget->findChild<QPushButton*>("clear")->setEnabled(true);
        m_customWidget->findChild<QSpinBox*>("features")->setEnabled(true);
        m_customWidget->findChild<QComboBox*>("loss")->setEnabled(true);
        m_customWidget->findChild<QPushButton*>("train")->setVisible(true);
        m_customWidget->findChild<QProgressBar*>("pbar_train")->setVisible(false);
        m_customWidget->findChild<QPushButton*>("browse")->setEnabled(true);
        m_trainpb_worker.stopProcess();
    }
    *updated = true;
}

void GuiNN::slot_updateTrainPB(bool* updated) {
    m_customWidget->findChild<QProgressBar*>("pbar_train")->setValue(m_nn.getTrainPercent());
    m_customWidget->findChild<QProgressBar*>("pbar_train")->update();
    *updated = true;
}

void GuiNN::slot_showMessageBox(QString msg, bool* updated) {
    DEBUG_MSG("SHOW WARNING MESSAGE BOX");
    QMessageBox* m = new QMessageBox(m_customWidget);
    m->setText(msg);
    m->setDefaultButton(QMessageBox::Ok);
    m->setIcon(QMessageBox::Icon::Warning);
    m->setWindowTitle(tr("NN Maker - Warning"));
    m->exec();
    *updated = true;
    DEBUG_MSG("CLOSE WARNING MESSAGE BOX");
}