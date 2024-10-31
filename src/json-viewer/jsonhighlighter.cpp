#include "jsonhighlighter.h"

JsonHighlighter::JsonHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    keyFormat.setForeground(Qt::darkBlue);
    keyFormat.setFontWeight(QFont::Bold);

    stringFormat.setForeground(Qt::darkGreen);

    numberFormat.setForeground(Qt::darkRed);

    booleanFormat.setForeground(Qt::darkYellow);
}

void JsonHighlighter::highlightBlock(const QString &text) {
    highlightNumbers(text);
    highlightBooleans(text);
    highlightStrings(text);
    highlightStringsKeys(text);
}

void JsonHighlighter::highlightStrings(const QString &text) {
    QRegularExpression re(QStringLiteral("\"(.*?)\""));
    QRegularExpressionMatchIterator matchIterator = re.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), stringFormat);
    }
}

void JsonHighlighter::highlightStringsKeys(const QString &text) {
    QRegularExpression re(QStringLiteral("\"[^\"]+\"\\s*:"));
    QRegularExpressionMatchIterator matchIterator = re.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), keyFormat);
    }
}

void JsonHighlighter::highlightNumbers(const QString &text) {
    QRegularExpression re(QStringLiteral("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?"));
    QRegularExpressionMatchIterator matchIterator = re.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), numberFormat);
    }
}

void JsonHighlighter::highlightBooleans(const QString &text) {
    QRegularExpression re(QStringLiteral("(true|false)"));
    QRegularExpressionMatchIterator matchIterator = re.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), booleanFormat);
    }
}
