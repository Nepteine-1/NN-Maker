#ifndef QAPPWIDGETSFML
#define QAPPWIDGETSFML

#include <QtUtils/QWidgetSFML.hpp>
#include <NeuralNetwork/NeuralNetwork.hpp>

class QAppWidgetSFML : public QWidgetSFML
{
    Q_OBJECT
    
    public :
        QAppWidgetSFML(QWidget* Parent, const QPoint& Position = QPoint(0,0), const QSize& Size = QSize(0,0));
        void setNnPtr(NeuralNetwork* ptr);

    private :
        void OnInit();
        void OnUpdate();
        NeuralNetwork* m_appNN = nullptr;
};

#endif