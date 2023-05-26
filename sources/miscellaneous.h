/*

Copyright 2014 S. Razi Alavizadeh
Copyright 2012-2018 Adam Reichold
Copyright 2018 Pavel Sanda
Copyright 2014 Dorian Scholz

This file is part of qpdfview.

qpdfview is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

qpdfview is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with qpdfview.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef MISCELLANEOUS_H
#define MISCELLANEOUS_H

#include <QComboBox>
#include <QGraphicsEffect>
#include <QLineEdit>
#include <QMenu>
#include <QPainter>
#include <QProxyStyle>
#include <QSpinBox>
#include <QSplitter>
#include <QTreeView>

#include "macros.h"

class QTextLayout;

namespace qpdfview
{

// graphics composition mode effect

class GraphicsCompositionModeEffect : public QGraphicsEffect
{
    Q_OBJECT

public:
    explicit GraphicsCompositionModeEffect(QPainter::CompositionMode compositionMode, QObject* parent = nullptr);

protected:
    void draw(QPainter* painter) override;

private:
    QPainter::CompositionMode m_compositionMode;

};

// proxy style

class ProxyStyle : public QProxyStyle
{
    Q_OBJECT

public:
    ProxyStyle();

    DECL_UNUSED
    DECL_NODISCARD
    bool scrollableMenus() const;
    void setScrollableMenus(bool scrollableMenus);

    int styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const override;

private:
    Q_DISABLE_COPY(ProxyStyle)

    bool m_scrollableMenus;

};

// tool tip menu

class ToolTipMenu : public QMenu
{
    Q_OBJECT

public:
    explicit ToolTipMenu(QWidget* parent = nullptr);
    explicit ToolTipMenu(const QString& title, QWidget* parent = nullptr);

protected:
    bool event(QEvent* event) override;

};

// searchable menu

class SearchableMenu : public ToolTipMenu
{
    Q_OBJECT

public:
    explicit SearchableMenu(const QString& title, QWidget* parent = nullptr);

    DECL_UNUSED
    DECL_NODISCARD
    bool isSearchable() const;
    void setSearchable(bool searchable);

protected:
    void hideEvent(QHideEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    bool m_searchable;
    QString m_text;

};

// tab bar

class TabBar : public QTabBar
{
    Q_OBJECT

public:
    explicit TabBar(QWidget* parent = nullptr);

signals:
    void tabDragRequested(int index);

protected:
    DECL_NODISCARD
    QSize tabSizeHint(int index) const override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    Q_DISABLE_COPY(TabBar)

    int m_dragIndex;
    QPoint m_dragPos;

};

// tab widget

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit TabWidget(QWidget* parent = nullptr);

    DECL_NODISCARD
    bool hasCurrent() const { return currentIndex() != -1; }

    DECL_NODISCARD
    QString currentTabText() const { return tabText(currentIndex()); }
    void setCurrentTabText(const QString& text) { setTabText(currentIndex(), text); }

    DECL_UNUSED
    DECL_NODISCARD
    QString currentTabToolTip() const { return tabToolTip(currentIndex()); }
    void setCurrentTabToolTip(const QString& toolTip) { setTabToolTip(currentIndex(), toolTip); }

    int addTab(QWidget* widget, bool nextToCurrent,
               const QString& label, const QString& toolTip);

    enum TabBarPolicy
    {
        TabBarAsNeeded = 0,
        TabBarAlwaysOn = 1,
        TabBarAlwaysOff = 2
    };

    DECL_NODISCARD
    TabBarPolicy tabBarPolicy() const;
    void setTabBarPolicy(TabBarPolicy tabBarPolicy);

    DECL_NODISCARD
    bool spreadTabs() const;
    void setSpreadTabs(bool spreadTabs);

public slots:
    void previousTab();
    void nextTab();

signals:
    void tabDragRequested(int index);
    void tabContextMenuRequested(QPoint globalPos, int index);

protected slots:
    void onTabBarCustomContextMenuRequested(QPoint pos);

protected:
    void tabInserted(int index) override;
    void tabRemoved(int index) override;

private:
    Q_DISABLE_COPY(TabWidget)

    TabBarPolicy m_tabBarPolicy;
    bool m_spreadTabs;

};

// tree view

class TreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit TreeView(int expansionRole, QWidget* parent = nullptr);

public slots:
    void expandAbove(const QModelIndex& child);

    void expandAll(const QModelIndex& index = QModelIndex());
    void collapseAll(const QModelIndex& index = QModelIndex());

    int expandedDepth(const QModelIndex& index);

    void expandToDepth(const QModelIndex& index, int depth);
    void collapseFromDepth(const QModelIndex& index, int depth);

    void restoreExpansion(const QModelIndex& index = QModelIndex());

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void contextMenuEvent(QContextMenuEvent* event) override;

protected slots:
    void on_expanded(const QModelIndex& index);
    void on_collapsed(const QModelIndex& index);

private:
    Q_DISABLE_COPY(TreeView)

    int m_expansionRole;

};

// line edit

class LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit LineEdit(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    Q_DISABLE_COPY(LineEdit)

};

// combo box

class ComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit ComboBox(QWidget* parent = nullptr);

private:
    Q_DISABLE_COPY(ComboBox)

};

// spin box

class SpinBox : public QSpinBox
{
    Q_OBJECT

public:
    explicit SpinBox(QWidget* parent = nullptr);

signals:
    void returnPressed();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    Q_DISABLE_COPY(SpinBox)

};

// mapping spin box

class MappingSpinBox : public SpinBox
{
    Q_OBJECT

public:
    struct TextValueMapper
    {
        virtual ~TextValueMapper() = default;

        virtual QString textFromValue(int val, bool& ok) const = 0;
        virtual int valueFromText(const QString& text, bool& ok) const = 0;
    };

    explicit MappingSpinBox(TextValueMapper* mapper, QWidget* parent = nullptr);

protected:
    DECL_NODISCARD
    QString textFromValue(int val) const override;
    DECL_NODISCARD
    int valueFromText(const QString& text) const override;

    QValidator::State validate(QString& input, int& pos) const override;

private:
    Q_DISABLE_COPY(MappingSpinBox)

    QScopedPointer< TextValueMapper > m_mapper;

};

int getMappedNumber(MappingSpinBox::TextValueMapper* mapper,
                    QWidget* parent, const QString& title, const QString& caption,
                    int value = 0, int min = -2147483647, int max = 2147483647,
                    bool* ok = nullptr, Qt::WindowFlags flags = {});

// progress line edit

class ProgressLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit ProgressLineEdit(QWidget* parent = nullptr);

    DECL_NODISCARD
    int progress() const;
    void setProgress(int progress);

signals:
    void returnPressed(Qt::KeyboardModifiers modifiers);

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    Q_DISABLE_COPY(ProgressLineEdit)

    int m_progress;

};

// search line edit

class SearchLineEdit : public ProgressLineEdit
{
    Q_OBJECT

public:
    explicit SearchLineEdit(QWidget* parent = nullptr);

public slots:
    void startSearch();

    DECL_UNUSED
    void startTimer();
    void stopTimer();

signals:
    void searchInitiated(const QString& text, bool modified = false);

protected slots:
    void on_timeout();
    void on_returnPressed(Qt::KeyboardModifiers modifiers);

private:
    Q_DISABLE_COPY(SearchLineEdit)

    QTimer* m_timer;

};

// splitter

class Splitter : public QSplitter
{
    Q_OBJECT

public:
    explicit Splitter(Qt::Orientation orientation, QWidget* parent = nullptr);

    DECL_NODISCARD
    QWidget* currentWidget() const;
    void setCurrentWidget(QWidget* const currentWidget);

    void setUniformSizes();

signals:
    void currentWidgetChanged(QWidget* currentWidget);

protected slots:
    void on_focusChanged(QWidget* old, QWidget* now);

private:
    Q_DISABLE_COPY(Splitter)

    int m_currentIndex;

};

// fallback icons

inline QIcon loadIconWithFallback(const QString& name)
{
    QIcon icon = QIcon::fromTheme(name);

    if(icon.isNull())
    {
        icon = QIcon(QLatin1String(":icons/") + name);
    }

    return icon;
}

void openInNewWindow(const QString& filePath, int page);

} // qpdfview

#endif // MISCELLANEOUS_H
