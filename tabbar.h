#ifndef TABBAR_H
#define TABBAR_H
#include <QObject>
#include <QTabWidget>
class TabBar : public QTabWidget
{
    Q_OBJECT
public:
    TabBar(QWidget *parent = 0);
private slots:
    void removeTabPage(int index);
private :
    void init_slot();
};

#endif // TABBAR_H
