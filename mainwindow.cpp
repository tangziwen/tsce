#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupTabBar ();
    setCentralWidget (tab_widiget);
    setupMenuBar();
    setupActions ();
    setupToolBar ();

}

CodeEditor * MainWindow::CreateEditor ()
{
    CodeEditor * editor =new CodeEditor();
    editor->setWindowTitle(QObject::tr("tzw-C-editor"));
    return editor;
}

void MainWindow::about()
{
    QMessageBox::about(NULL, "About", "tzw simple C editor");
}

void MainWindow::newFile()
{

    tab_widiget->addTab (CreateEditor(),tr("new file"));
                tab_widiget->setCurrentIndex (tab_widiget->currentIndex ()+1);
}

void MainWindow::saveFile()
{
    CodeEditor * editor=(CodeEditor *)tab_widiget->currentWidget ();
    QString  fileName=editor->getFileName ();
    cout<<"the file is "<<fileName.toStdString ()<<endl;
    if(!fileName.isNull ())
    {
        save(fileName);
    }
    else //otherwise save as
    {
        saveAsFile();
    }
}

void MainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if(!fileName.isNull ())
    {
        save(fileName);
        //update the tab title
        tab_widiget->setTabText (tab_widiget->currentIndex (),QFileInfo(fileName).fileName ());
    }
}

void MainWindow::openFile(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "C Files (*.c *.h)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            CodeEditor * editor=CreateEditor();
            tab_widiget->addTab (editor,QFileInfo(file).fileName ());
            editor->setPlainText(file.readAll());
            editor->setFileName (fileName);
            tab_widiget->setCurrentIndex (tab_widiget->currentIndex ()+1);
        }
    }
}

void MainWindow::setupMenuBar ()
{

    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar ()->addMenu (fileMenu);
    fileMenu->addAction(tr("&New"), this, SLOT(newFile()), QKeySequence::New);
    fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()), QKeySequence::Open);
    fileMenu->addAction(tr("&Save..."), this, SLOT(saveFile()), QKeySequence::Save);
    fileMenu->addAction(tr("&Save As..."), this, SLOT(saveAsFile()), QKeySequence::SaveAs);
    fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()), QKeySequence::Quit);


    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(tr("&About"), this, SLOT(about()));
    menuBar()->addMenu(helpMenu);
}

void MainWindow::setupToolBar()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
//! [29] //! [31]
    fileToolBar->addAction(openAct);
//! [31]
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
}

void MainWindow::setupActions()
{
    QPlainTextEdit * textEdit=(QPlainTextEdit *)tab_widiget->currentWidget ();
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

//! [19]
    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));
//! [18] //! [19]

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAsFile()));

//! [20]
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
//! [20]
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

//! [21]
    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
//! [21]
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

//! [22]
    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
//! [22]

//! [23]
    cutAct->setEnabled(false);
//! [23] //! [24]
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));

}

void MainWindow::save(QString fileName)
{
     CodeEditor * editor=(CodeEditor *)tab_widiget->currentWidget ();
    if(!fileName.isNull ())
    {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("tsce"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }
        QTextStream out(&file);
    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif
        out << editor->toPlainText ();
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif
    }
}

void MainWindow::setupTabBar()
{
    tab_widiget =new TabBar();
    tab_widiget->addTab (CreateEditor(),tr("new file"));
}
