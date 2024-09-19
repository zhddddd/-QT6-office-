#include "myword.h"
#include<QPrintDialog>
#include "mychild.h"
#include <QtWidgets>

const QString srcpaths = ":/new/prefix1/images";

MyWord::MyWord(QWidget *parent)
    : QMainWindow(parent)
{

    mdiArea=new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setCentralWidget(mdiArea);

    connect(mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(updateMenus()));
    windowMapper=new QSignalMapper(this);
    connect(windowMapper,SIGNAL(mapped(QWidget*)),this,SLOT(setActiveSubWindow(QWidget*)));

    createActions(); // 创建菜单操作（动作）
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();


    resize(900,600);

    setWindowTitle(tr("Offce办公自动化文字编辑软件 V2.1"));
    setUnifiedTitleAndToolBarOnMac(true);
}

MyWord::~MyWord()
{
}

void MyWord::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        event->accept();
    }
}

void MyWord::updateMenus() // 更新菜单
{
    //至少有一个子文档打开着的情况
    bool hasMyChild = (activeMyChild()!=0);

    saveAct->setEnabled(hasMyChild);
    saveAsAct->setEnabled(hasMyChild);
    printAct->setEnabled(hasMyChild);
    printPreviewAct->setEnabled(hasMyChild);

    pasteAct->setEnabled(hasMyChild);

    closeAct->setEnabled(hasMyChild);
    closeAllAct->setEnabled(hasMyChild);
    tileAct->setEnabled(hasMyChild);
    cascadeAct->setEnabled(hasMyChild);
    nextAct->setEnabled(hasMyChild);
    previousAct->setEnabled(hasMyChild);
    separatorAct->setVisible(hasMyChild);


    //文档打开着并且其中有内容被选中的情况
    bool hasSelection = (activeMyChild() && activeMyChild()->textCursor().hasSelection());

    cutAct->setEnabled(hasSelection);
    copyAct->setEnabled(hasSelection);

    boldAct->setEnabled(hasSelection);
    italicAct->setEnabled(hasSelection);
    underlineAct->setEnabled(hasSelection);
    leftAlignAct->setEnabled(hasSelection);
    centerAct->setEnabled(hasSelection);
    rightAlignAct->setEnabled(hasSelection);
    justifyAct->setEnabled(hasSelection);
    colorAct->setEnabled(hasSelection);
}

void MyWord::updateWindowMenu() // 更新窗口菜单
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();

    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();

    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());
    //显示当前打开着的文档子窗口项
    for (int i = 0; i < windows.size(); ++i) {
        MyChild *child = qobject_cast<MyChild *>(windows.at(i)->widget());

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        }
        QAction *action  = windowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeMyChild());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }
    enabledText();          //使得字体设置菜单可用
}

MyChild *MyWord::createMyChild() // 创建子窗口对象
{
    MyChild *child = new MyChild;
    mdiArea->addSubWindow(child);

    connect(child, SIGNAL(copyAvailable(bool)),cutAct, SLOT(setEnabled(bool)));
    connect(child, SIGNAL(copyAvailable(bool)),copyAct, SLOT(setEnabled(bool)));

    return child;
}

void MyWord::createActions() // 创建菜单操作（动作）
{
    // 【文件】菜单动作
    // 新建
    newAct=new QAction(QIcon(srcpaths+"/filenew.png"),tr("新建(&N)"),this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setToolTip("新建"); // 设置工具栏按钮的提示文本信息
    newAct->setStatusTip(tr("创建一个新的word文档.")); // 设置状态栏提示文本信息
    connect(newAct,SIGNAL(triggered()),this,SLOT(fileNew()));

    // 打开
    openAct=new QAction(QIcon(srcpaths+"/fileopen.png"),tr("打开(&O)"),this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setToolTip("打开"); // 设置工具栏按钮的提示文本信息
    openAct->setStatusTip(tr("打开已经存在的word文档.")); // 设置状态栏提示文本信息
    connect(openAct,SIGNAL(triggered()),this,SLOT(fileOpen()));

    // 保存
    saveAct=new QAction(QIcon(srcpaths+"/filesave.png"),tr("保存(&S)"),this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setToolTip("保存");
    saveAct->setStatusTip(tr("将当前word文档存盘."));
    connect(saveAct,SIGNAL(triggered()),this,SLOT(fileSave()));

    // 另存为
    saveAsAct=new QAction(tr("另存为(&A)..."),this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    // saveAsAct->setToolTip("另存为"); // 此工具栏按钮大家根据自已需要决定是保留
    saveAsAct->setStatusTip(tr("用另一个新的文件名称，保存当前word文档."));
    connect(saveAsAct,SIGNAL(triggered()),this,SLOT(fileSaveAs()));

    // 打印
    printAct=new QAction(QIcon(srcpaths+"/fileprint.png"),tr("打印(&P)"),this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setToolTip("打印");
    printAct->setStatusTip(tr("打印当前word文档."));
    connect(printAct,SIGNAL(triggered()),this,SLOT(filePrint()));

    // 打印预览
    printPreviewAct=new QAction(tr("打印预览..."),this);
    printPreviewAct->setStatusTip(tr("打印预览当前word文档效果."));
    connect(printPreviewAct,SIGNAL(triggered()),this,SLOT(filePrintPreview()));

    // 退出操作
    exitAct=new QAction(tr("退出(X)"),this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip("退出Word文件应用程序");
    connect(exitAct,SIGNAL(triggered()),qApp,SLOT(closeAllWindows()));


    // 【编辑】菜单动作
    // 撤销
    undoAct=new QAction(QIcon(srcpaths+"/editundo.png"),tr("撤销(&U)"),this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setToolTip("撤销");
    undoAct->setStatusTip(tr("撤销当前Word文档操作."));
    connect(undoAct,SIGNAL(triggered()),this,SLOT(undo()));

    // 重做
    redoAct=new QAction(QIcon(srcpaths+"/editredo.png"),tr("重做(&R)"),this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setToolTip("重做");
    redoAct->setStatusTip(tr("恢复之前的Word文档操作."));
    connect(redoAct,SIGNAL(triggered()),this,SLOT(redo()));

    // 剪切
    cutAct=new QAction(QIcon(srcpaths+"/editcut.png"),tr("剪切(&T)"),this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setToolTip("剪切");
    cutAct->setStatusTip(tr("从Word文档中裁剪所选择的倥，将它存放到剪贴板."));
    connect(cutAct,SIGNAL(triggered()),this,SLOT(cut()));

    // 复制
    copyAct=new QAction(QIcon(srcpaths+"/editcopy.png"),tr("复制(&C)"),this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setToolTip("复制");
    copyAct->setStatusTip(tr("复制当前所选中的内容，将它存放到剪贴板."));
    connect(copyAct,SIGNAL(triggered()),this,SLOT(copy()));

    // 粘贴
    pasteAct=new QAction(QIcon(srcpaths+"/editpaste.png"),tr("粘贴(&P)"),this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setToolTip("粘贴");
    pasteAct->setStatusTip(tr("将剪贴板里面的内容粘贴到当前word文档."));
    connect(pasteAct,SIGNAL(triggered()),this,SLOT(paste()));


    // 【格式】-->（字体）菜单动作
    // 加粗
    boldAct=new QAction(QIcon(srcpaths+"/textbold.png"),tr("加粗(&B)"),this);
    boldAct->setCheckable(true);
    boldAct->setShortcut(Qt::CTRL + Qt::Key_B);
    boldAct->setToolTip("加粗");
    boldAct->setStatusTip(tr("将你所选择的文字进行加粗处理."));

    QFont bold;
    bold.setBold(true);
    boldAct->setFont(bold);
    connect(boldAct,SIGNAL(triggered()),this,SLOT(textBold()));

    // 倾斜
    italicAct=new QAction(QIcon(srcpaths+"/textitalic.png"),tr("倾斜(&I)"),this);
    italicAct->setCheckable(true);
    italicAct->setShortcut(Qt::CTRL + Qt::Key_I);
    italicAct->setToolTip("倾斜");
    italicAct->setStatusTip(tr("将你所选择的文字进行倾斜处理."));

    QFont italic;
    italic.setItalic(true);
    italicAct->setFont(italic);
    connect(italicAct,SIGNAL(triggered()),this,SLOT(textItalic()));

    // 下划线
    underlineAct=new QAction(QIcon(srcpaths+"/textunder.png"),tr("下划线(&U)"),this);
    underlineAct->setCheckable(true);
    underlineAct->setShortcut(Qt::CTRL + Qt::Key_U);
    underlineAct->setToolTip("下划线");
    underlineAct->setStatusTip(tr("将你所选择的文字添加下划线."));

    QFont underline;
    underline.setUnderline(true);
    underlineAct->setFont(underline);
    connect(underlineAct,SIGNAL(triggered()),this,SLOT(textUnderline()));


    // 【格式】-->（段落）子菜单下各项为同一个菜单项组，我们只能选中其中的一项
    QActionGroup *grp=new QActionGroup(this);
    connect(grp,SIGNAL(triggered(QAction*)),this,SLOT(textAlign(QAction*)));

    if(QApplication::isLeftToRight()){
        leftAlignAct=new QAction(QIcon(srcpaths + "/textleft.png"),tr("左对齐(&L)"),grp);
        centerAct=new QAction(QIcon(srcpaths + "/textcenter.png"),tr("居中对齐(&E)"),grp);
        rightAlignAct=new QAction(QIcon(srcpaths + "/textright.png"),tr("右对齐(&R)"),grp);
    }
    else
    {
        rightAlignAct=new QAction(QIcon(srcpaths + "/textright.png"),tr("右对齐(&R)"),grp);
        centerAct=new QAction(QIcon(srcpaths + "/textcenter.png"),tr("居中对齐(&E)"),grp);
        leftAlignAct=new QAction(QIcon(srcpaths + "/textleft.png"),tr("左对齐(&L)"),grp);
    }
    justifyAct=new QAction(QIcon(srcpaths + "/textjustify.png"),tr("两端对齐(&J)"),grp);


    leftAlignAct->setShortcut(Qt::CTRL + Qt::Key_L);
    leftAlignAct->setCheckable(true);
    leftAlignAct->setToolTip("左对齐");
    leftAlignAct->setStatusTip(tr("将选择文字进行左对齐."));

    centerAct->setShortcut(Qt::CTRL + Qt::Key_E);
    centerAct->setCheckable(true);
    centerAct->setToolTip("居中对齐");
    centerAct->setStatusTip(tr("将选择文字进行居中对齐."));

    rightAlignAct->setShortcut(Qt::CTRL + Qt::Key_R);
    rightAlignAct->setCheckable(true);
    rightAlignAct->setToolTip("右对齐");
    rightAlignAct->setStatusTip(tr("将选择文字进行右对齐."));

    justifyAct->setShortcut(Qt::CTRL + Qt::Key_J);
    justifyAct->setCheckable(true);
    justifyAct->setToolTip("两端对齐");
    justifyAct->setStatusTip(tr("将选择文字进行两端对齐."));

    QPixmap pix(16,16);
    pix.fill(Qt::red);
    colorAct=new QAction(pix,tr("颜色(&C)..."),this);
    colorAct->setToolTip("颜色");
    colorAct->setStatusTip(tr("将你选择的文字，设置对应的颜色."));
    connect(colorAct,SIGNAL(triggered()),this,SLOT(textColor()));


    // 【窗口】菜单
    closeAct=new QAction(tr("关闭(&O)"),this);
    closeAct->setStatusTip(tr("关闭活动word文档子窗口."));
    connect(closeAct,SIGNAL(triggered()),mdiArea,SLOT(closeActiveSubWindow()));

    closeAllAct=new QAction(tr("关闭所有(&A)"),this);
    closeAllAct->setStatusTip(tr("关闭活动word文档所有子窗口."));
    connect(closeAllAct,SIGNAL(triggered()),mdiArea,SLOT(closeAllSubWindows()));

    tileAct=new QAction(tr("平铺(&T)"),this);
    tileAct->setStatusTip(tr("平铺子窗口."));
    connect(tileAct,SIGNAL(triggered()),mdiArea,SLOT(tileSubWindows()));

    cascadeAct=new QAction(tr("层叠(&C)"),this);
    cascadeAct->setStatusTip(tr("层叠子窗口."));
    connect(cascadeAct,SIGNAL(triggered()),mdiArea,SLOT(cascadeSubWindows()));

    nextAct=new QAction(tr("下一个(&T)"),this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("移动焦点到下一个子窗口."));
    connect(nextAct,SIGNAL(triggered()),mdiArea,SLOT(activateNextSubWindow()));

    previousAct=new QAction(tr("前一个(&V)"),this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("移动焦点到前一个子窗口."));
    connect(previousAct,SIGNAL(triggered()),mdiArea,SLOT(activatePreviousSubWindow()));

    separatorAct=new QAction(this);
    separatorAct->setSeparator(true);



    // 【帮助】菜单
    aboutAct=new QAction(tr("关于(&A)"),this);
    aboutAct->setStatusTip("关于Office Word相关信息.");
    connect(aboutAct,SIGNAL(triggered()),this,SLOT(about()));

    aboutQtAct=new QAction(tr("关于 Qt(&Qt)"),this);
    aboutQtAct->setStatusTip("关于 Qt库相关信息.");
    connect(aboutQtAct,SIGNAL(triggered()),this,SLOT(aboutQt()));
}

void MyWord::createMenus() // 创建菜单
{

    //【文件】主菜单
    fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();   //分隔线
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();   //分隔线
    fileMenu->addAction(printAct);
    fileMenu->addAction(printPreviewAct);
    fileMenu->addSeparator();   //分隔线
    fileMenu->addAction(exitAct);

    //【编辑】主菜单
    editMenu = menuBar()->addMenu(tr("编辑(&E)"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();   //分隔线
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    //【格式】主菜单
    formatMenu = menuBar()->addMenu(tr("格式(&O)"));
    fontMenu = formatMenu->addMenu(tr("字体(&D)"));   //【字体】子菜单
    fontMenu->addAction(boldAct);
    fontMenu->addAction(italicAct);
    fontMenu->addAction(underlineAct);

    alignMenu = formatMenu->addMenu(tr("段落"));      //【段落】子菜单
    alignMenu->addAction(leftAlignAct);
    alignMenu->addAction(centerAct);
    alignMenu->addAction(rightAlignAct);
    alignMenu->addAction(justifyAct);

    formatMenu->addAction(colorAct);

    //【窗口】主菜单
    windowMenu = menuBar()->addMenu(tr("窗口(&W)"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
    menuBar()->addSeparator();

    //【帮助】主菜单
    helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    helpMenu->addAction(aboutAct);
    helpMenu->addSeparator();   //分隔线
    helpMenu->addAction(aboutQtAct);

}

void MyWord::createToolBars() // 创建工具条
{
    //"文件"工具栏
    fileToolBar = addToolBar(tr("文件"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addSeparator();        //分隔条
    fileToolBar->addAction(printAct);

    //"编辑"工具栏
    editToolBar = addToolBar(tr("编辑"));
    editToolBar->addAction(undoAct);
    editToolBar->addAction(redoAct);
    editToolBar->addSeparator();        //分隔条
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);

    //"格式"工具栏
    formatToolBar = addToolBar(tr("格式"));
    formatToolBar->addAction(boldAct);
    formatToolBar->addAction(italicAct);
    formatToolBar->addAction(underlineAct);
    formatToolBar->addSeparator();      //分隔条
    formatToolBar->addAction(leftAlignAct);
    formatToolBar->addAction(centerAct);
    formatToolBar->addAction(rightAlignAct);
    formatToolBar->addAction(justifyAct);
    formatToolBar->addSeparator();      //分隔条
    formatToolBar->addAction(colorAct);

    //组合工具栏
    addToolBarBreak(Qt::TopToolBarArea);    //使这个工具条在界面上另起一行显示
    comboToolBar = addToolBar(tr("组合选择"));
    comboStyle = new QComboBox();
    comboToolBar->addWidget(comboStyle);
    comboStyle->addItem("标准");
    comboStyle->addItem("项目符号 (●)");
    comboStyle->addItem("项目符号 (○)");
    comboStyle->addItem("项目符号 (■)");
    comboStyle->addItem("编号 (⒈⒉⒊)");
    comboStyle->addItem("编号 ( a.b.c.)");
    comboStyle->addItem("编号 ( A.B.C.)");
    comboStyle->addItem("编号 (ⅰ.ⅱ.ⅲ.)");
    comboStyle->addItem("编号 (Ⅰ.Ⅱ.Ⅲ.)");
    comboStyle->setStatusTip("段落加标号或编号");
    connect(comboStyle, SIGNAL(activated(int)), this, SLOT(textStyle(int)));




    comboFont = new QFontComboBox();
    comboToolBar->addWidget(comboFont);
    comboFont->setStatusTip("更改字体");
    connect(comboFont, SIGNAL(activated(QString)), this, SLOT(textFamily(QString)));

    comboSize = new QComboBox();
    comboToolBar->addWidget(comboSize);
    comboSize->setEditable(true);
    comboSize->setStatusTip("更改字号");

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(activated(QString)), this, SLOT(textSize(QString)));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font().pointSize())));

}

void MyWord::createStatusBar() // 创建状态条
{
    statusBar()->showMessage(tr("就绪状态"));
}

MyChild *MyWord::activeMyChild() // 激活子窗口
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<MyChild *>(activeSubWindow->widget());
    return 0;
}

//打开文件用
QMdiSubWindow *MyWord::findMyChild(const QString &fileName) // 查找子窗口
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        MyChild *myChild = qobject_cast<MyChild *>(window->widget());
        if (myChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}
void MyWord::setActiveSubWindow(QWidget *window) // 设置active激活窗口
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

















void MyWord::fileNew() // 新建
{
    MyChild *child=createMyChild();
    child->newFile();
    child->show();
    enabledText(); // 使得字体设置菜单可用的
}

void MyWord::fileOpen() // 打开
{
    QString filename=QFileDialog::getOpenFileName(this,tr("打开"),QString(),tr("HTML 文档(*.htm *.html);;所有文件(*.*)"));

    if(!filename.isEmpty()){
        QMdiSubWindow *existing=findMyChild(filename);

        if(existing)
        {
            mdiArea->setActiveSubWindow(existing);
            return ;
        }

        MyChild *child=createMyChild();
        if(child->loadFile(filename))
        {
            statusBar()->showMessage(tr("文件已经加载"),2000);
            child->show();
            enabledText();
        }
        else
        {
            child->close();
        }
    }
}

void MyWord::fileSave() // 保存
{
    if(activeMyChild() && activeMyChild()->save())
        statusBar()->showMessage(tr("Word文档保存成功."),2000);
}

void MyWord::fileSaveAs() // 另存为
{
    if(activeMyChild() && activeMyChild()->saveAs())
        statusBar()->showMessage(tr("Word文档另存为成功."));
}

void MyWord::undo() // 撤消
{
    if(activeMyChild())
        activeMyChild()->undo();
}

void MyWord::redo() // 重做
{
    if(activeMyChild())
        activeMyChild()->redo();
}

void MyWord::cut() // 剪切
{
    if(activeMyChild())
        activeMyChild()->cut();

}

void MyWord::copy() // 复制
{
    if(activeMyChild())
        activeMyChild()->copy();

}

void MyWord::paste() // 粘贴
{
    if(activeMyChild())
        activeMyChild()->paste();

}

void MyWord::enabledText()  //使得【格式】下的各个子菜单项可用
{
    boldAct->setEnabled(true);
    italicAct->setEnabled(true);
    underlineAct->setEnabled(true);
    leftAlignAct->setEnabled(true);
    centerAct->setEnabled(true);
    rightAlignAct->setEnabled(true);
    justifyAct->setEnabled(true);
    colorAct->setEnabled(true);
}

void MyWord::textBold() // 加粗
{
    QTextCharFormat fmt;
    fmt.setFontWeight(boldAct->isChecked()?QFont::Bold:QFont::Normal);
    if(activeMyChild())
        activeMyChild()->mergeFormatOnWordOrSelection(fmt);

}

void MyWord::textItalic() // 斜体
{
    QTextCharFormat fmt;
    fmt.setFontItalic(italicAct->isChecked());

    if(activeMyChild())
        activeMyChild()->mergeFormatOnWordOrSelection(fmt);

}

void MyWord::textUnderline() // 下划线
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(underlineAct->isChecked());

    if(activeMyChild())
        activeMyChild()->mergeFormatOnWordOrSelection(fmt);

}

void MyWord::textAlign(QAction *a) // 文本对齐判断函数
{
    if(activeMyChild())
    {
        if(a==leftAlignAct)
            activeMyChild()->setAlign(1);
        else if(a==centerAct)
            activeMyChild()->setAlign(2);
        else if(a==rightAlignAct)
            activeMyChild()->setAlign(3);
        else if(a==justifyAct)
            activeMyChild()->setAlign(4);
    }
}

void MyWord::textStyle(int styleIndex) // 文本样式
{
    if(activeMyChild())
        activeMyChild()->setStyle(styleIndex);
}

void MyWord::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    if(activeMyChild())
        activeMyChild()->mergeFormatOnWordOrSelection(fmt);
}

void MyWord::textSize(const QString &p) // 文本大小
{
    qreal pointsize=p.toFloat();
    if(p.toFloat()>0){
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointsize);
        if(activeMyChild())
            activeMyChild()->mergeFormatOnWordOrSelection(fmt);
    }
}

void MyWord::textColor() // 文本颜色
{
    if(activeMyChild())
    {
        QColor color=QColorDialog::getColor(activeMyChild()->textColor(),this);

        if(!color.isValid())
            return ;
        QTextCharFormat fmt;
        fmt.setForeground(color);
        activeMyChild()->mergeFormatOnWordOrSelection(fmt);
        colorChanged(color);
    }

}

void MyWord::fontChanged(const QFont &f) // 字体更改
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));

    boldAct->setChecked(f.bold());
    italicAct->setChecked(f.italic());
    underlineAct->setChecked(f.underline());

}

void MyWord::colorChanged(const QColor &c) // 颜色更改
{
    QPixmap pix(16,16);
    pix.fill(c);
    colorAct->setIcon(pix);

}

void MyWord::alignmentChanged(Qt::Alignment a) // 对齐判断
{
    if(a & Qt::AlignLeft)
        leftAlignAct->setChecked(true);
    else if(a & Qt::AlignCenter)
        centerAct->setChecked(true);
    else if(a & Qt::AlignRight)
        rightAlignAct->setChecked(true);
    else if(a & Qt::AlignJustify)
        justifyAct->setChecked(true);

}

void MyWord::filePrint() // 打印文档
{
    // QPrinter printer(QPrinter::HighResolution);
    // QPrintDialog *pdlg=new QPrintDialog(&printer,this);
    // if(activeMyChild()->textCursor().hasSelection())
    //     pdlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    // pdlg->setWhatsThis(tr("打印文档"));
    // if(pdlg->exec()==QDialog::Accepted)
    //     activeMyChild()->print(&printer);

    // delete pdlg;
}

void MyWord::filePrintPreview() // 文件打印预览
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer,this);
    connect(&preview,SIGNAL(paintRequested(QPinter*)),SLOT(printPreview(QPrinter*)));

}

void MyWord::printPreview(QPrinter *printer) // 打印预览
{
    activeMyChild()->print(printer);
}

void MyWord::about() // 关于
{
    QMessageBox::about(this,tr("关于"),tr("此软件是基于Qt5实现的文字处理软件！！！"));

}





















