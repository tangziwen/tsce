/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "codeeditor.h"
#include <iostream>
#include <QFlag>
using namespace std;
//![constructor]

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    setupSearchDlg();
    setupCompleter ();
    setupGotoDlg();
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    highlighter = new Highlighter(this->document());
    this->zoomIn (4);
}

void CodeEditor::setFileName(QString file_name)
{
    this->file_name=file_name;
}

QString CodeEditor::getFileName()
{
    return file_name;
}

//![constructor]

//![extraAreaWidth]

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::showSearchDialog()
{

    findDlg->show();
}


void CodeEditor::searchText()
{
    QString findText = find_textLineEdit->text();
    QFlags<QTextDocument::FindFlag> mask=0;
    if(caseCheckBox->isChecked ())
    {
        mask ^=QTextDocument::FindCaseSensitively;
    }
    if(wholeWordsCheckBox->isChecked ())
    {
        mask ^=QTextDocument::FindWholeWords;
    }
    if(backwardCheckBox->isChecked ())
    {
        mask ^=QTextDocument::FindBackward;
    }
    this->find(findText,mask);
}

void CodeEditor::insertCompletion(const QString &completion)
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

void CodeEditor::showGotoLineDlg()
{
    gotoDlg->show ();
}

void CodeEditor::gotoLine()
{
QTextCursor tc = textCursor();
tc.setPosition (gotoLineEextLineEdit->text ().toInt ()-1);
setTextCursor (tc);
}

QString CodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
    tc.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void CodeEditor::setupCompleter()
{
    stringListModel=new QStringListModel();
    QStringList wordList;
    wordList.insert (0,"tangziwen");
    wordList.insert (1,"tank");
    c=new QCompleter();
    c->setModel (stringListModel);

    stringListModel->setStringList (wordList);
    c->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    c->setWrapAround(false);
    c->setWidget(this);
    c->setCompletionMode (QCompleter::UnfilteredPopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));

}

void CodeEditor::setupSearchDlg()
{
    findDlg = new QDialog(this);
    findDlg->setWindowTitle(tr("search"));
    find_textLineEdit = new QLineEdit(findDlg);
    QPushButton *find_Btn = new QPushButton(tr("&Search"),findDlg);
    wholeWordsCheckBox = new QCheckBox(tr("&Whole words"));
    backwardCheckBox = new QCheckBox(tr("Search &backward"));
    caseCheckBox =new QCheckBox(tr("&Case sensitive"));
    connect(find_Btn,SIGNAL(clicked()),this,SLOT(searchText()));//设置摁钮和其槽函数的连接
    QVBoxLayout* layout = new QVBoxLayout(findDlg);//新建一个垂直布局管理器，并将行编辑器和按钮加入
    layout->addWidget(find_textLineEdit);
    layout->addWidget(find_Btn);
    layout->addWidget (caseCheckBox);
    layout->addWidget (wholeWordsCheckBox);
    layout->addWidget (backwardCheckBox);
}

void CodeEditor::setupGotoDlg()
{
    gotoDlg = new QDialog(this);
    gotoDlg->setWindowTitle(tr("Goto Line"));
    gotoLineEextLineEdit = new QLineEdit(gotoDlg);
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, gotoLineEextLineEdit );
    gotoLineEextLineEdit->setValidator( validator );
    QPushButton *find_Btn = new QPushButton(tr("&Goto"),gotoDlg);
    connect(find_Btn,SIGNAL(clicked()),this,SLOT(gotoLine()));
    QVBoxLayout* layout = new QVBoxLayout(gotoDlg);
    layout->addWidget(gotoLineEextLineEdit);
    layout->addWidget(find_Btn);
}

void CodeEditor::UpdateAutoCompleteList(QString prefix)
{
    QStringList wordList;
    QRegExp rx("[A-Za-z0-9_]+\\s{1,}([A-Za-z0-9_]+)\\s*\\({1,1}[A-Za-z0-9_ ]*\\){1,1}");
    QString str = this->toPlainText ();
    int pos =0;
    while ((pos = rx.indexIn(str, pos)) != -1) {

        if(rx.cap (1).contains(QRegExp("^"+prefix,Qt::CaseInsensitive) ))
        {
            wordList.insert (wordList.size (),rx.cap (1));
        }
        pos +=rx.matchedLength ();
    }
    wordList.removeDuplicates ();
    stringListModel->setStringList (wordList);
}


//![slotUpdateRequest]

//![resizeEvent]

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::contextMenuEvent(QContextMenuEvent *)
{
    QCursor cur=this->cursor();

    QMenu *menu=new QMenu(this);
    menu->addAction(tr("copy"),this,SLOT(copy()));
    menu->addAction(tr("cut"),this,SLOT(cut()));
    menu->addAction(tr("paste"),this,SLOT(paste()));
    menu->addAction(tr("undo"),this,SLOT(undo()));
    menu->addAction(tr("redo"),this,SLOT(redo()));
    menu->addAction(tr("zoom in"),this,SLOT(zoomIn()));
    menu->addAction(tr("zoom out"),this,SLOT(zoomOut()));
    menu->addAction(tr("goto"),this,SLOT(showGotoLineDlg()));
    menu->exec(cur.pos());
    delete menu;
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    //cout<<e->key ();
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
        default:
            break;
        }
    }



    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_F); // CTRL+F
    if(isShortcut)
    {
        showSearchDialog ();
        return;
    }

    isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_G); // CTRL+F
        if(isShortcut)
        {
            showGotoLineDlg ();
            return;
        }

    isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);
    \
    //! [7]

    //! [8]

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
        QString  completionPrefix = textUnderCursor();
        UpdateAutoCompleteList(completionPrefix);
    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                        || eow.contains(e->text().right(1)))) {
        c->popup()->hide();
        return;
    }


    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}



void CodeEditor::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

//![resizeEvent]

//![cursorPositionChanged]

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

//![cursorPositionChanged]



//![extraAreaPaintEvent_0]

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
//![extraAreaPaintEvent_2]

