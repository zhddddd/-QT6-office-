#include "mychild.h"

#include <QtWidgets>

MyChild::MyChild()
{
    setAttribute(Qt::WA_DeleteOnClose); // 关闭窗口时销毁，
    isUntitled = true;
}

void MyChild::newFile() // 新建word文件
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("Word文档-%1").arg(sequenceNumber++);
    setWindowTitle(curFile);
}

bool MyChild::loadFile(const QString &fileName) // 导入文件
{
    if (!fileName.isEmpty())
    {
        if (!QFile::exists(fileName))
            return false;
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly))
            return false;

        // 提供一个字节数组,QByteArray可用于存储原始字节（包括“\ 0” ）和传统的8位 “\ 0” 端接字符串 .
        QByteArray data = file.readAll();

        // Qt 使用 Unicode 来存储、绘制、操作字符串。 在许多情况下，可能希望处理使用不同编码的数据。
        QTextCodec *codec = Qt::codecForHtml(data);
        QString str = codec->toUnicode(data);

        if (Qt::mightBeRichText(str)) { // //如果是富文本就设置为HTml
            this->setHtml(str);
        } else { //如果不是富文本设置为简单字符串格式
            str = QString::fromLocal8Bit(data);
            this->setPlainText(str);
        }
        setCurrentFile(fileName);
        connect(document(), SIGNAL(contentsChanged()),this, SLOT(documentWasModified()));
        return true;
    }
}

void MyChild::documentWasModified()
{
    // 在设置改变时，设置窗口已修改
    setWindowModified(document()->isModified());
}

QString MyChild::strippedName(const QString &fullFileName) // 脱离文件名称
{
    return QFileInfo(fullFileName).fileName();
}

QString MyChild::userFriendlyCurrentFile() // 用户友好型当前文件
{
    return strippedName(curFile);
}

void MyChild::setCurrentFile(const QString &fileName) // 设置当前文件
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

void MyChild::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

bool MyChild::maybeSave() // 判断是否修改且保存文档 // 是否可以保存
{
    if (!document()->isModified())
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Myself Qt Word"),tr("文档'%1'已被修改,保存吗?").arg(userFriendlyCurrentFile()),QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return save();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

bool MyChild::save() // 保存
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MyChild::saveAs() // 另存为
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"),curFile,tr("HTML 文档 (*.htm *.html);;所有文件 (*.*)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MyChild::saveFile(QString fileName) // 保存文件
{
    if (!(fileName.endsWith(".htm", Qt::CaseInsensitive) || fileName.endsWith(".html", Qt::CaseInsensitive))) {
        fileName += ".html"; // 默认保存为 HTML 文档
    }
    QTextDocumentWriter writer(fileName);
    bool success = writer.write(this->document());
    if (success)
        setCurrentFile(fileName);
    return success;
}

//格式设置
void MyChild::mergeFormatOnWordOrSelection(const QTextCharFormat &format) //格式字体设置
{
    QTextCursor cursor = this->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    this->mergeCurrentCharFormat(format);
}

//段落对齐设置
void MyChild::setAlign(int align) //段落对齐设置
{
    if (align == 1)
        this->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute); // 水平方向靠左。绝对
    else if (align == 2)
        this->setAlignment(Qt::AlignHCenter); // 水平方向居中。
    else if (align == 3)
        this->setAlignment(Qt::AlignRight | Qt::AlignAbsolute); // 水平方向靠右。
    else if (align == 4)
        this->setAlignment(Qt::AlignJustify); // 水平方向调整间距两端对齐。
}

//段落标号、编号
void MyChild::setStyle(int style)
{
    // 多行文本框文本光标插入文本
    QTextCursor cursor = this->textCursor();

    if (style != 0) {
        QTextListFormat::Style stylename = QTextListFormat::ListDisc; // # 样式为圆圈

        switch (style) {
        default:
        case 1:
            stylename = QTextListFormat::ListDisc; // # 样式为圆圈
            break;
        case 2:
            stylename = QTextListFormat::ListCircle; // # 样式为空心圆圈
            break;
        case 3:
            stylename = QTextListFormat::ListSquare;// 样式为方块
            break;
        case 4:
            stylename = QTextListFormat::ListDecimal; // 小写阿拉伯数字，最大4999
            break;
        case 5:
            stylename = QTextListFormat::ListLowerAlpha; // 小写拉丁字符，按字母顺序
            break;
        case 6:
            stylename = QTextListFormat::ListUpperAlpha; // 大写拉丁字符，按字母顺序
            break;
        case 7:
            stylename = QTextListFormat::ListLowerRoman; // 小写罗马数字，最大4999
            break;
        case 8:
            stylename = QTextListFormat::ListUpperRoman; // 大写罗马数字，最大4999
            break;
        }

        cursor.beginEditBlock();

        // QTextEdit类就是一个富文本编辑器,所以在构造QTexrEdit类的对象时就已经构建了一个QTextDocument类对象和
        // 一个QTextCursor类对象，只须调用它们进行相应的操作即可
        QTextBlockFormat blockFmt = cursor.blockFormat();

        QTextListFormat listFmt;

        if (cursor.currentList()) {
            listFmt = cursor.currentList()->format();
        } else {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }




        listFmt.setStyle(stylename);

        cursor.createList(listFmt);

        cursor.endEditBlock();
    } else {
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}
