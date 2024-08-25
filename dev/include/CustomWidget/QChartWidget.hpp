#ifndef QCHARTW_HPP
#define QCHARTW_HPP

#include <QWidget>
#include <QtCharts>
#include <QMouseEvent>
#include <NeuralNetwork/NeuralNetwork.hpp>

class QChartWidget : public QChartView {
    Q_OBJECT
    /*void mouseDoubleClickEvent(QMouseEvent *ev) {
        std::cout << "left" << std::endl;
    }

    void mousePressEvent(QMouseEvent *ev) {
        std::cout << "left" << std::endl;
    }   

    void mouseMoveEvent(QMouseEvent *ev) {
        std::cout << "Moved" << std::endl;
    }
    void leaveEvent(QEvent *event) {
        std::cout << "leaved" << std::endl;
    }*/

    public:
        QChartWidget(QWidget* Parent, const QPoint& Position= QPoint(0,0), const QSize& Size= QSize(0,0));
        ~QChartWidget();
        void showLossResults(NeuralNetwork*);
        void resetChart(void);

        void wheelEvent(QWheelEvent *e) override;
        void mouseMoveEvent(QMouseEvent *ev) override;
        void mousePressEvent(QMouseEvent *ev) override;
        void mouseReleaseEvent(QMouseEvent *ev) override;
        void keyPressEvent(QKeyEvent *event) override;

    private slots:
        void slot_resetChart(void);

    private:
        QChart* m_chartWidget{nullptr};
        bool m_dragDrop{false};
        QPoint m_lastMousePos{QPoint(0,0)};
};

#endif