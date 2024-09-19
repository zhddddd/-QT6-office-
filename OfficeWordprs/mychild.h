#ifndef MYCHILD_H
#define MYCHILD_H
#include<QTextCodec>

// 第一步：实现子类mychild

#include <QTextEdit> // 文件编辑框类:QTextEdit
#include <QPrinter> // Qt使用QPrinter生成多页pdf文档


class MyChild : public QTextEdit
{
    Q_OBJECT
public:
    MyChild();

    void newFile(); // 新建word文件
    bool loadFile(const QString &fileName); // 导入文件
    bool save(); // 保存
    bool saveAs(); // 另存为
    bool saveFile(QString fileName); // 保存文件
    QString userFriendlyCurrentFile();// 用户友好型当前文件
    QString currentFile() { return curFile; } // 当前文件
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);   //格式字体设置
    void setAlign(int align);                                           //段落对齐设置
    void setStyle(int style);                                           //段落标号、编号
protected:
    void closeEvent(QCloseEvent *event); // 可以通过参数event来控制是否让窗体关闭。
private slots:
    void documentWasModified(); // 修改文件
private:
    bool maybeSave(); // 是否可以保存
    void setCurrentFile(const QString &fileName); // 设置当前文件
    QString strippedName(const QString &fullFileName); // 脱离文件名称

    QString curFile; // 当前文件
    bool isUntitled; // 判断是否命名


};

#endif // MYCHILD_H
