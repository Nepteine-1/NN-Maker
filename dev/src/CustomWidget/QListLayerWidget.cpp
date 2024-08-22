#include <CustomWidget/QListLayerWidget.hpp>
#include <iostream>

QListLayerWidget::QListLayerWidget(QWidget* Parent, const QPoint& Position, const QSize& Size) : QListWidget(Parent) {
    move(Position);
    resize(Size);
}

void QListLayerWidget::slotAddLayerBtnClicked(int nb_neurons, QString activation_type) {
    std::string str = "Layer " + std::to_string(this->count()) + ":" + activation_type.toStdString() + ":" + std::to_string(nb_neurons);
    this->addItem(QString::fromStdString(str));

}

void QListLayerWidget::slotClearBtnClicked() { 
    this->clear();
}

int QListLayerWidget::previousCountItem(void) { return m_previousCountItem; }