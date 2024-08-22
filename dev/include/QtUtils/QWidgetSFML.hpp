#ifndef QWIDGETSFML
#define QWIDGETSFML

#include <QtWidgets>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <QtUiTools>

class QWidgetSFML : public QWidget, public sf::RenderWindow
{
    Q_OBJECT

    public:
        explicit QWidgetSFML(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 0);

        virtual ~QWidgetSFML();

    private :

        virtual void OnInit() =0;

        virtual void OnUpdate() =0;

        virtual QPaintEngine* paintEngine() const;

        virtual void showEvent(QShowEvent*);

        virtual void paintEvent(QPaintEvent*);

        QTimer myTimer;
        bool   myInitialized;
};

#endif //__QSFML__H_