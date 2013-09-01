#include "tabbar.h"
#include <QtWidgets>
#include <QObject>
#include <QTabWidget>
#include <stdio.h>
TabBar::TabBar(QWidget *parent) :QTabWidget(parent)
{
    this->setTabsClosable (true);
    connect(this,SIGNAL(tabCloseRequested(int))
            ,this,SLOT(removeTabPage(int))
            );

}




void TabBar::removeTabPage(int index)
{
    this->removeTab (index);
}
