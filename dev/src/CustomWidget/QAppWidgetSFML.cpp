#include <CustomWidget/QAppWidgetSFML.hpp>

QAppWidgetSFML::QAppWidgetSFML(QWidget* Parent, const QPoint& Position, const QSize& Size ) : QWidgetSFML(Parent, Position, Size) {}

void QAppWidgetSFML::OnInit() {}

void QAppWidgetSFML::OnUpdate() {}

void QAppWidgetSFML::setNnPtr(NeuralNetwork* ptr) {
    m_appNN = ptr;
}