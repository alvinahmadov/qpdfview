/*

Copyright 2014 S. Razi Alavizadeh
Copyright 2012-2018 Adam Reichold
Copyright 2018 Pavel Sanda
Copyright 2012 Michał Trybus
Copyright 2012 Alexander Volkov

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPointer>

#ifdef WITH_DBUS

#include <QDBusAbstractAdaptor>

class QDBusInterface;

#endif // WITH_DBUS

class QCheckBox;
class QDateTime;
class QGraphicsView;
class QFileInfo;
class QModelIndex;
class QShortcut;
class QTableView;
class QToolButton;
class QTreeView;
class QWidgetAction;

#include "renderparam.h"

namespace qpdfview
{

class Settings;
class DocumentView;
class TabWidget;
class TreeView;
class ComboBox;
class MappingSpinBox;
class SearchLineEdit;
class SearchableMenu;
class RecentlyUsedMenu;
class RecentlyClosedMenu;
class BookmarkModel;
class Database;
class ShortcutHandler;
class SearchModel;
class HelpDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class MainWindowAdaptor;

public:
    explicit MainWindow(QWidget* parent = nullptr);

    QSize sizeHint() const override;
    QMenu* createPopupMenu() override;

public slots:
    void show();

    bool open(const QString& filePath, int page = -1, const QRectF& highlight = QRectF(), bool quiet = false);
    bool openInNewTab(const QString& filePath, int page = -1, const QRectF& highlight = QRectF(), bool quiet = false);

    bool jumpToPageOrOpenInNewTab(const QString& filePath, int page = -1, bool refreshBeforeJump = false, const QRectF& highlight = QRectF(), bool quiet = false);

    void startSearch(const QString& text);

    void saveDatabase();

protected slots:
    void onTabWidgetCurrentChanged();
    void onTabWidgetTabCloseRequested(int index);
    void onTabWidgetTabDragRequested(int index);
    void onTabWidgetTabContextMenuRequested(QPoint globalPos, int index);

    void onCurrentTabDocumentChanged();
    void onCurrentTabDocumentModified();

    void onCurrentTabNumberOfPagesChaned(int numberOfPages);
    void onCurrentTabCurrentPageChanged(int currentPage);

    void onCurrentTabCanJumpChanged(bool backward, bool forward);

    void onCurrentTabContinuousModeChanged(bool continuousMode);
    void onCurrentTabLayoutModeChanged(qpdfview::LayoutMode layoutMode);
    void onCurrentTabRightToLeftModeChanged(bool rightToLeftMode);
    void onCurrentTabScaleModeChanged(qpdfview::ScaleMode scaleMode);
    void onCurrentTabScaleFactorChanged(qreal scaleFactor);
    void onCurrentTabRotationChanged(qpdfview::Rotation rotation);

    void onCurrentTabLinkClicked(int page);
    void onCurrentTabLinkClicked(bool newTab, const QString& filePath, int page);

    void onCurrentTabRenderFlagsChanged(qpdfview::RenderFlags renderFlags);

    void onCurrentTabInvertColorsChanged(bool invertColors);
    void onCurrentTabConvertToGrayscaleChanged(bool convertToGrayscale);
    void onCurrentTabTrimMarginsChanged(bool trimMargins);

    void onCurrentTabCompositionModeChanged(qpdfview::CompositionMode compositionMode);

    void onCurrentTabHighlightAllChanged(bool highlightAll);
    void onCurrentTabRubberBandModeChanged(qpdfview::RubberBandMode rubberBandMode);

    void onCurrentTabSearchFinished();
    void onCurrentTabSearchProgressChanged(int progress);

    void onCurrentTabCustomContextMenuRequested(QPoint pos);

    void onSplitViewSplitHorizontallyTriggered();
    void onSplitViewSplitVerticallyTriggered();
    void onSplitViewSplitTriggered(Qt::Orientation orientation, int index);
    void onSplitViewCloseCurrentTriggered();
    void onSplitViewCloseCurrentTriggered(int index);
    void onSplitViewCurrentWidgetChanged(QWidget* currentWidget);

    void onCurrentPageEditingFinished();
    void onCurrentPageReturnPressed();

    void onScaleFactorActivated(int index);
    void onScaleFactorEditingFinished();
    void onScaleFactorReturnPressed();

    void onOpenTriggered();
    void onOpenInNewTabTriggered();
    void onOpenCopyInNewTabTriggered();
    void onOpenCopyInNewTabTriggered(const qpdfview::DocumentView* tab);
    void onOpenCopyInNewWindowTriggered();
    static void onOpenCopyInNewWindowTriggered(const qpdfview::DocumentView* tab);
    void onOpenContainingFolderTriggered();
    static void onOpenContainingFolderTriggered(const qpdfview::DocumentView* tab);
    void onMoveToInstanceTriggered();
    void onMoveToInstanceTriggered(qpdfview::DocumentView* tab);
    void onRefreshTriggered();
    void onSaveTriggered();
    void onSaveAsTriggered();
    void onSaveCopyTriggered();
    void onPrintTriggered();

    void onRecentlyUsedOpenTriggered(const QString& filePath);

    void onPreviousPageTriggered();
    void onNextPageTriggered();
    void onFirstPageTriggered();
    void onLastPageTriggered();

    void onSetFirstPageTriggered();

    void onJumpToPageTriggered();

    void onJumpBackwardTriggered();
    void onJumpForwardTriggered();

    void onSearchTriggered();
    void onFindPreviousTriggered();
    void onFindNextTriggered();
    void onCancelSearchTriggered();

    void onCopyToClipboardModeTriggered(bool checked);
    void onAddAnnotationModeTriggered(bool checked);

    void onSettingsTriggered();

    void onContinuousModeTriggered(bool checked);
    void onTwoPagesModeTriggered(bool checked);
    void onTwoPagesWithCoverPageModeTriggered(bool checked);
    void onMultiplePagesModeTriggered(bool checked);

    void onRightToLeftModeTriggered(bool checked);

    void onZoomInTriggered();
    void onZoomOutTriggered();
    void onOriginalSizeTriggered();

    void onFitToPageWidthModeTriggered(bool checked);
    void onFitToPageSizeModeTriggered(bool checked);

    void onRotateLeftTriggered();
    void onRotateRightTriggered();

    void onInvertColorsTriggered(bool checked);
    void onConvertToGrayscaleTriggered(bool checked);
    void onTrimMarginsTriggered(bool checked);
    void onDarkenWithPaperColorTriggered(bool checked);
    void onLightenWithPaperColorTriggered(bool checked);

    void onFontsTriggered();

    void onFullscreenTriggered(bool checked);
    void onPresentationTriggered();

    void onPreviousTabTriggered();
    void onNextTabTriggered();
    void onCloseTabTriggered();
    void onCloseAllTabsTriggered();
    void onCloseAllTabsButCurrentTabTriggered();
    void onCloseAllTabsButThisOneTriggered(int thisIndex);
    void onCloseAllTabsToTheLeftTriggered(int ofIndex);
    void onCloseAllTabsToTheRightTriggered(int ofIndex);
    void onCloseTabsTriggered(const QVector<qpdfview::DocumentView*>& tabs);

    void onRestoreMostRecentlyClosedTabTriggered();

    void onRecentlyClosedTabActionTriggered(QAction* tabAction);

    void onTabActionTriggered();
    void onTabShortcutActivated();

    void onPreviousBookmarkTriggered();
    void onNextBookmarkTriggered();
    void onAddBookmarkTriggered();
    void onRemoveBookmarkTriggered();
    void onRemoveAllBookmarksTriggered();

    void onBookmarksMenuAboutToShow();

    void onBookmarkOpenTriggered(const QString& absoluteFilePath);
    void onBookmarkOpenInNewTabTriggered(const QString& absoluteFilePath);
    void onBookmarkJumpToPageTriggered(const QString& absoluteFilePath, int page);
    void onBookmarkRemoveBookmarkTriggered(const QString& absoluteFilePath);

    void onContentsTriggered();
    void onAboutTriggered();

    void onFocusCurrentPageActivated();
    void onFocusScaleFactorActivated();

    void onToggleToolBarsTriggered(bool checked);
    void onToggleMenuBarTriggered(bool checked);

    void onSearchInitiated(const QString& text, bool modified);
    void onHighlightAllClicked(bool checked);

    void onDockLocationChanged(Qt::DockWidgetArea area);

    void onOutlineSectionCountChanged();
    void onOutlineClicked(const QModelIndex& index);

    void onPropertiesSectionCountChanged();

    void onThumbnailsDockLocationChanged(Qt::DockWidgetArea area);
    void onThumbnailsVerticalScrollBarValueChanged(int value);

    void onBookmarksSectionCountChanged();
    void onBookmarksClicked(const QModelIndex& index);
    void onBookmarksContextMenuRequested(QPoint pos);

    void onSearchSectionCountChanged();
    void onSearchDockLocationChanged(Qt::DockWidgetArea area);
    void onSearchVisibilityChanged(bool visible);
    void onSearchClicked(const QModelIndex& index);
    void onSearchRowsInserted(const QModelIndex& parent, int first, int last);

    void onSaveDatabaseTimeout();

protected:
    bool eventFilter(QObject* target, QEvent* event) override;

    void closeEvent(QCloseEvent* event) override;

    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    Q_DISABLE_COPY(MainWindow)

    static Settings* s_settings;
    static Database* s_database;
    static ShortcutHandler* s_shortcutHandler;
    static SearchModel* s_searchModel;

    static void prepareStyle();

    TabWidget* m_tabWidget;

    DocumentView* currentTab() const;
    DocumentView* currentTab(int index) const;
    QVector< DocumentView* > allTabs(int index) const;
    QVector< DocumentView* > allTabs() const;

    bool senderIsCurrentTab() const;

    bool m_currentTabChangedBlocked;

    class CurrentTabChangeBlocker;

    void addTab(DocumentView* tab);
    void addTabAction(DocumentView* tab);
    void connectTab(DocumentView* tab);

    void restorePerFileSettings(DocumentView* tab);

    bool saveModifications(DocumentView* tab);
    void closeTab(DocumentView* tab);

    void setWindowTitleForCurrentTab();
    void setCurrentPageSuffixForCurrentTab();

    BookmarkModel* bookmarkModelForCurrentTab(bool create = false);

    QAction* sourceLinkActionForCurrentTab(QObject* parent, QPoint pos);

    class RestoreTab;

    QTimer* m_saveDatabaseTimer;

    void prepareDatabase();

    void scheduleSaveDatabase();
    void scheduleSaveTabs();
    void scheduleSaveBookmarks();
    void scheduleSavePerFileSettings();

    class TextValueMapper;

    MappingSpinBox* m_currentPageSpinBox;
    QWidgetAction* m_currentPageAction;

    ComboBox* m_scaleFactorComboBox;
    QWidgetAction* m_scaleFactorAction;

    SearchLineEdit* m_searchLineEdit;
    QCheckBox* m_matchCaseCheckBox;
    QCheckBox* m_wholeWordsCheckBox;
    QCheckBox* m_highlightAllCheckBox;
    QToolButton* m_findPreviousButton;
    QToolButton* m_findNextButton;
    QToolButton* m_cancelSearchButton;

    void createWidgets();

    QAction* m_openAction;
    QAction* m_openInNewTabAction;
    QAction* m_refreshAction;
    QAction* m_saveAction;
    QAction* m_saveAsAction;
    QAction* m_saveCopyAction;
    QAction* m_printAction;
    QAction* m_exitAction;

    QAction* m_previousPageAction;
    QAction* m_nextPageAction;
    QAction* m_firstPageAction;
    QAction* m_lastPageAction;

    QAction* m_setFirstPageAction;

    QAction* m_jumpToPageAction;

    QAction* m_jumpBackwardAction;
    QAction* m_jumpForwardAction;

    QAction* m_searchAction;
    QAction* m_findPreviousAction;
    QAction* m_findNextAction;
    QAction* m_cancelSearchAction;

    QAction* m_copyToClipboardModeAction;
    QAction* m_addAnnotationModeAction;

    QAction* m_settingsAction;

    QAction* m_continuousModeAction;
    QAction* m_twoPagesModeAction;
    QAction* m_twoPagesWithCoverPageModeAction;
    QAction* m_multiplePagesModeAction;

    QAction* m_rightToLeftModeAction;

    QAction* m_zoomInAction;
    QAction* m_zoomOutAction;
    QAction* m_originalSizeAction;

    QAction* m_fitToPageWidthModeAction;
    QAction* m_fitToPageSizeModeAction;

    QAction* m_rotateLeftAction;
    QAction* m_rotateRightAction;

    QAction* m_invertColorsAction;
    QAction* m_convertToGrayscaleAction;
    QAction* m_trimMarginsAction;
    QAction* m_darkenWithPaperColorAction;
    QAction* m_lightenWithPaperColorAction;

    QAction* m_fontsAction;

    QAction* m_fullscreenAction;
    QAction* m_presentationAction;

    QAction* m_previousTabAction;
    QAction* m_nextTabAction;

    QAction* m_closeTabAction;
    QAction* m_closeAllTabsAction;
    QAction* m_closeAllTabsButCurrentTabAction;

    QAction* m_restoreMostRecentlyClosedTabAction;

    QShortcut* m_tabShortcuts[9];

    QAction* m_previousBookmarkAction;
    QAction* m_nextBookmarkAction;

    QAction* m_addBookmarkAction;
    QAction* m_removeBookmarkAction;
    QAction* m_removeAllBookmarksAction;

    QAction* m_contentsAction;
    QAction* m_aboutAction;

    QAction* m_openCopyInNewTabAction;
    QAction* m_openCopyInNewWindowAction;
    QAction* m_openContainingFolderAction;
    QAction* m_moveToInstanceAction;
    QAction* m_splitViewHorizontallyAction;
    QAction* m_splitViewVerticallyAction;
    QAction* m_closeCurrentViewAction;

    QAction* createAction(const QString& text, const QString& objectName, const QIcon& icon, const QList< QKeySequence >& shortcuts, const char* member, bool checkable = false, bool checked = false);
    QAction* createAction(const QString& text, const QString& objectName, const QIcon& icon, const QKeySequence& shortcut, const char* member, bool checkable = false, bool checked = false);
    QAction* createAction(const QString& text, const QString& objectName, const QString& iconName, const QList< QKeySequence >& shortcuts, const char* member, bool checkable = false, bool checked = false);
    QAction* createAction(const QString& text, const QString& objectName, const QString& iconName, const QKeySequence& shortcut, const char* member, bool checkable = false, bool checked = false);

    void createActions();

    QToolBar* m_fileToolBar;
    QToolBar* m_editToolBar;
    QToolBar* m_viewToolBar;

    QShortcut* m_focusCurrentPageShortcut;
    QShortcut* m_focusScaleFactorShortcut;

    QToolBar* createToolBar(const QString& text, const QString& objectName, const QStringList& actionNames, const QList< QAction* >& actions, bool child = true);

    void createToolBars();

    QDockWidget* m_outlineDock;
    TreeView* m_outlineView;

    QDockWidget* m_propertiesDock;
    QTableView* m_propertiesView;

    QDockWidget* m_thumbnailsDock;
    QGraphicsView* m_thumbnailsView;

    QDockWidget* m_bookmarksDock;
    QTableView* m_bookmarksView;

	QDockWidget* m_toolbarDock;
	QWidget* m_toolbarWidget;

    QDockWidget* m_searchDock;
    QTreeView* m_searchView;
    QWidget* m_searchWidget;

    QDockWidget* createDock(const QString& text, const QString& objectName, const QKeySequence& toggleViewShortcut);
	void createToolbarDock();
    void createSearchDock();

    void createDocks();

    QMenu* m_fileMenu;
    RecentlyUsedMenu* m_recentlyUsedMenu;
    QMenu* m_editMenu;
    QMenu* m_viewMenu;
    QMenu* m_compositionModeMenu;
    SearchableMenu* m_tabsMenu;
    RecentlyClosedMenu* m_recentlyClosedMenu;
    SearchableMenu* m_bookmarksMenu;
    QMenu* m_helpMenu;

    bool m_bookmarksMenuIsDirty;

    void createMenus();

    int m_tabBarHadPolicy;

    bool m_fileToolBarWasVisible;
    bool m_editToolBarWasVisible;
    bool m_viewToolBarWasVisible;

    QAction* m_toggleToolBarsAction;
    QAction* m_toggleMenuBarAction;

    QPointer< HelpDialog > m_helpDialog;

};

#ifdef WITH_DBUS

class MainWindowAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "local.qpdfview.MainWindow")

    explicit MainWindowAdaptor(MainWindow* mainWindow);

public:
    static QDBusInterface* createInterface(const QString& instanceName = QString());
    static MainWindowAdaptor* createAdaptor(MainWindow* mainWindow);

public slots:
    Q_NOREPLY void raiseAndActivate();

    bool open(const QString& absoluteFilePath, int page = -1, const QRectF& highlight = QRectF(), bool quiet = false);
    bool openInNewTab(const QString& absoluteFilePath, int page = -1, const QRectF& highlight = QRectF(), bool quiet = false);

    bool jumpToPageOrOpenInNewTab(const QString& absoluteFilePath, int page = -1, bool refreshBeforeJump = false, const QRectF& highlight = QRectF(), bool quiet = false);

    Q_NOREPLY void startSearch(const QString& text);

    Q_NOREPLY void saveDatabase();


    int currentPage() const;
    Q_NOREPLY void jumpToPage(int page);

    Q_NOREPLY void previousPage();
    Q_NOREPLY void nextPage();
    Q_NOREPLY void firstPage();
    Q_NOREPLY void lastPage();

    Q_NOREPLY void previousBookmark();
    Q_NOREPLY void nextBookmark();

    bool jumpToBookmark(const QString& label);


    Q_NOREPLY void continuousMode(bool checked);
    Q_NOREPLY void twoPagesMode(bool checked);
    Q_NOREPLY void twoPagesWithCoverPageMode(bool checked);
    Q_NOREPLY void multiplePagesMode(bool checked);

    Q_NOREPLY void fitToPageWidthMode(bool checked);
    Q_NOREPLY void fitToPageSizeMode(bool checked);

    Q_NOREPLY void invertColors(bool checked);
    Q_NOREPLY void convertToGrayscale(bool checked);
    Q_NOREPLY void trimMargins(bool checked);

    Q_NOREPLY void fullscreen(bool checked);
    Q_NOREPLY void presentation();


    Q_NOREPLY void closeTab();
    Q_NOREPLY void closeAllTabs();
    Q_NOREPLY void closeAllTabsButCurrentTab();

    bool closeTab(const QString& absoluteFilePath);

private:
    MainWindow* mainWindow() const;

    static QString serviceName(QString instanceName = QString());

};

#endif // WITH_DBUS

} // qpdfview

#endif // MAINWINDOW_H
