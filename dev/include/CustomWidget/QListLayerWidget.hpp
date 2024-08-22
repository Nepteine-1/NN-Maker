#ifndef QLISTLAYERW
#define QLISTLAYERW

#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QListWidget>
#include <NeuralNetwork/NeuralNetwork.hpp>

class QListLayerWidget : public QListWidget {
    
    Q_OBJECT

    public:
        QListLayerWidget(QWidget* Parent, const QPoint& Position= QPoint(0,0), const QSize& Size= QSize(0,0));
        void setNnPtr(NeuralNetwork* ptr);
        int previousCountItem(void);

    private slots:
        void slotAddLayerBtnClicked(int nb_neurons, QString activation_type);
        void slotClearBtnClicked();
        
    private:
        int m_previousCountItem{0};
};

#endif