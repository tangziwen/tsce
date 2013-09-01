#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "codeeditor.h"
#include <QMainWindow>
#include "tabbar.h"
#include <QToolBar>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
private:
    void setupTabBar();
    CodeEditor * CreateEditor ();
    void setupMenuBar();
    void setupToolBar();
    void setupActions();
    TabBar * tab_widiget;
    void save(QString fileName);
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
signals:
    
public slots:
    void about();
    void newFile();
    void saveFile();
    void saveAsFile();
    void openFile(const QString &path = QString());
};

#endif // MAINWINDOW_H
