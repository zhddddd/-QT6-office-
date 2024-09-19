#ifndef MYWORD_H
#define MYWORD_H

#include <QMainWindow>

// 第二步：添加相关头文件
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewDialog>

class MyChild;

// QAction被创建了，那么就必须将它添加到相关的菜单和工具栏上，然后将它们链接到实现相应action功能的槽函数
class QAction;

// QMenuBar是菜单栏，QMenu是菜单，菜单QMenu是挂载在菜单栏QMenuBar上的，相当于菜单栏QMenuBar是一个容器，
// 菜单QMenu是容器中的其中一项，或者说QMenuBar内的子集全部是菜单QMenu
class QMenu;

// QComboBox提供了一种向用户呈现选项列表的方式，以占用最少的屏幕空间。 组合框是一个显示当前项目的选择小部件，可以弹出可选择项目的列表。
// 组合框可以是可编辑的，允许用户修改列表中的每个项目。 QComboBox 除了显示可见下拉列表外，
// 每个项（item，或称列表项）还可以关联一个 QVariant 类型的变量，用于存储一些不可见数据。
class QComboBox;

// QFontComboBox 字体下拉列表框,QFontComboBox()是QComboBox()的一个子类，但是它的内容是不能被编辑的，主要是用来选择字体。
class QFontComboBox;

// QMdiArea（Multiple Document Interface Area）提供了一个可以同时显示多个文档窗口的区域。
// 本质上来说，QMdiArea就像是多文档窗口的窗口管理器。例如，它将它所管理的窗口绘制在它上面，并且以级联或者平铺的方式放置这些窗口。
// 通常情况下，QMdiArea用作QMainWindow的中心控件，以此来创建一个MDI应用程序。但是，它也可以被放置在任何布局类中。
// QMdiArea支持所有的窗口标志，只要这些窗口标志被当前控件样式支持。
// QMdiArea中的子窗口是QMdiSubWindow的实例化对象。通过调用addSubWindow来将他们加入MDI区域。
// 通常情况下是传递一个QWidget作为内部控件给这个函数。但是直接传递QMdiSubWindow也是可以的。
class QMdiArea;

// 大多数复杂的程序，都使用MDI框架，在Qt designer中可以直接将控件MDI Area拖入使用。
// QMdiArea一般使用于主窗口中，用于容纳多个子窗口QMdiSubWindow
class QMdiSubWindow;

// QSignalMapper类收集了一系列的无参信号，然后使用相对于信号发送者来说的整数、字符串或控件参数来重新发送它们。
// QSignalMapper类可以看成是信号的翻译和转发器。
class QSignalMapper;
class QPrintDialog;


class MyWord : public QMainWindow
{
    Q_OBJECT

public:
    MyWord(QWidget *parent = nullptr);
    ~MyWord();

protected:
    // QCloseEvent事件是指，当你鼠标点击窗口右上角的关闭按钮时，所触发的函数.如果你没有重写virtual closeEvent(QCloseEvent*event);这个虚函数的话，
    // 系统是默认接受关闭事件的，所以就会关闭窗体，
    void closeEvent(QCloseEvent *event); // 可以通过参数event来控制是否让窗体关闭。

private slots:
    void fileNew(); // 新建
    void fileOpen(); // 打开
    void fileSave(); // 保存
    void fileSaveAs(); // 另存为
    void filePrint(); // 打印
    void filePrintPreview(); // 文件打印预览
    void printPreview(QPrinter *); // 打印预览

    void undo(); // 撤消
    void redo(); // 重做
    void cut(); // 剪切
    void copy(); // 复制
    void paste(); // 粘贴


    void about(); // 关于

    void textBold(); // 加粗
    void textItalic(); // 斜体
    void textUnderline(); // 下划线
    void textAlign(QAction *a); // 文本对齐判断函数
    void textStyle(int styleIndex); // 文本样式
    void textFamily(const QString &f); // 函数设置文本格式
    void textSize(const QString &p); // 文本大小
    void textColor(); // 文本颜色


    void updateMenus(); // 更新菜单
    void updateWindowMenu(); // 更新窗口菜单
    MyChild *createMyChild(); // 创建子窗口对象
    void setActiveSubWindow(QWidget *window); // 设置active激活窗口






private:
    void createActions(); // 创建菜单动作
    void createMenus(); // 创建菜单
    void createToolBars(); // 创建工具条
    void createStatusBar(); // 创建状态条
    void enabledText();         //使得【格式】下的各个子菜单项可用
    void fontChanged(const QFont &f); // 字体更改
    void colorChanged(const QColor &c); // 颜色更改
    void alignmentChanged(Qt::Alignment a); // 对齐判断

    MyChild *activeMyChild(); // 激活子窗口
    QMdiSubWindow *findMyChild(const QString &fileName); // 查找子窗口

    QMdiArea *mdiArea; // QMdiArea就像是多文档窗口的窗口管理器

    // QSignalMapper类收集了一系列的无参信号，然后使用相对于信号发送者来说的整数、字符串或控件参数来重新发送它们。
    // QSignalMapper类可以看成是信号的翻译和转发器。
    QSignalMapper *windowMapper;


    //菜单
    QMenu *fileMenu; // 文件菜单
    QMenu *editMenu; // 编辑菜单
    QMenu *formatMenu; // 格式菜单
    QMenu *fontMenu; // 字体子菜单：加粗 倾斜 下划线
    QMenu *alignMenu; // 对齐子菜单：左 右 居中 两端
    QMenu *windowMenu; // 窗口菜单
    QMenu *helpMenu; // 帮助菜单


    //工具栏
    QToolBar *fileToolBar; // 文件工具条
    QToolBar *editToolBar; // 编辑工具条
    QToolBar *formatToolBar; // 格式工具条
    QToolBar *comboToolBar; // 组合工具条
    QComboBox *comboStyle;      //子控件 标准组合框
    QFontComboBox *comboFont;   //子控件 字体组合框
    QComboBox *comboSize;       //子控件 字体大小组合框


    //动作(Action)
    QAction *newAct;            //【文件】主菜单 新建
    QAction *openAct; // 打开
    QAction *saveAct; // 保存
    QAction *saveAsAct; // 另存为
    QAction *printAct; // 打印
    QAction *printPreviewAct; // 打印预览
    QAction *exitAct; // 退出

    QAction *undoAct;           //【编辑】主菜单 撤消
    QAction *redoAct; // 重做
    QAction *cutAct; // 剪切
    QAction *copyAct; // 复制
    QAction *pasteAct; // 粘贴

    QAction *boldAct;           //【格式】主菜单 加粗
    QAction *italicAct; // 斜体
    QAction *underlineAct; // 下划画
    QAction *leftAlignAct; // 左对齐
    QAction *centerAct; // 居中对象
    QAction *rightAlignAct; // 右对象
    QAction *justifyAct; // 两端
    QAction *colorAct; // 颜色

    QAction *closeAct;          //【窗口】主菜单 关闭
    QAction *closeAllAct; // 关闭所有
    QAction *tileAct; // 平铺
    QAction *cascadeAct; // 层叠
    QAction *nextAct; // 下一个
    QAction *previousAct; // 上一个
    QAction *separatorAct; // 分离动作

    QAction *aboutAct;          //【帮助】主菜单 关于
    QAction *aboutQtAct; // 关于Qt


};
#endif // MYWORD_H
