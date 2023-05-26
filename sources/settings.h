/*

Copyright 2015 S. Razi Alavizadeh
Copyright 2012-2015, 2018 Adam Reichold
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>
#include <QIcon>
#include <QKeySequence>
#include <QObject>
#include <QPrinter>

class QSettings;

#include "global.h"
#include "printoptions.h"

namespace qpdfview
{

class Settings : public QObject
{
    Q_OBJECT

public:
    static Settings* instance();
    ~Settings() override;

    // page item

    class PageItem
    {
    public:
        void sync();

        DECL_NODISCARD
        int cacheSize() const { return m_cacheSize; }
        void setCacheSize(int cacheSize);

        DECL_NODISCARD
        bool useTiling() const { return m_useTiling; }
        void setUseTiling(bool useTiling);

        DECL_NODISCARD
        int tileSize() const { return m_tileSize; }

        DECL_NODISCARD
        const QIcon& progressIcon() const { return m_progressIcon; }
        void setProgressIcon(const QIcon& progressIcon) { m_progressIcon = progressIcon; }

        DECL_NODISCARD
        const QIcon& errorIcon() const { return m_errorIcon; }
        void setErrorIcon(const QIcon& errorIcon) { m_errorIcon = errorIcon; }

        DECL_NODISCARD
        bool keepObsoletePixmaps() const { return m_keepObsoletePixmaps; }
        void setKeepObsoletePixmaps(bool keepObsoletePixmaps);

        DECL_NODISCARD
        bool useDevicePixelRatio() const { return m_useDevicePixelRatio; }
        void setUseDevicePixelRatio(bool useDevicePixelRatio);

        DECL_NODISCARD
        bool decoratePages() const { return m_decoratePages; }
        void setDecoratePages(bool decoratePages);

        DECL_NODISCARD
        bool decorateLinks() const { return m_decorateLinks; }
        void setDecorateLinks(bool decorateLinks);

        DECL_NODISCARD
        bool decorateFormFields() const { return m_decorateFormFields; }
        void setDecorateFormFields(bool decorateFormFields);

        DECL_NODISCARD
        const QColor& backgroundColor() const { return m_backgroundColor; }
        void setBackgroundColor(const QColor& backgroundColor);

        DECL_NODISCARD
        const QColor& paperColor() const { return m_paperColor; }
        void setPaperColor(const QColor& paperColor);

        DECL_NODISCARD
        const QColor& highlightColor() const { return m_highlightColor; }
        void setHighlightColor(const QColor& highlightColor);

        DECL_NODISCARD
        QColor annotationColor() const;
        void setAnnotationColor(const QColor& annotationColor);

        DECL_NODISCARD
        Qt::KeyboardModifiers copyToClipboardModifiers() const;
        void setCopyToClipboardModifiers(Qt::KeyboardModifiers modifiers);

        DECL_NODISCARD
        Qt::KeyboardModifiers addAnnotationModifiers() const;
        void setAddAnnotationModifiers(Qt::KeyboardModifiers modifiers);

        DECL_NODISCARD
        Qt::KeyboardModifiers zoomToSelectionModifiers() const;
        void setZoomToSelectionModifiers(Qt::KeyboardModifiers modifiers);

        DECL_NODISCARD
        Qt::KeyboardModifiers openInSourceEditorModifiers() const;
        void setOpenInSourceEditorModifiers(Qt::KeyboardModifiers modifiers);

        DECL_NODISCARD
        bool annotationOverlay() const;
        void setAnnotationOverlay(bool overlay);

        DECL_NODISCARD
        bool formFieldOverlay() const;
        void setFormFieldOverlay(bool overlay);

    private:
        explicit PageItem(QSettings* settings);
        friend class Settings;

        QSettings* m_settings;

        int m_cacheSize;

        bool m_useTiling;
        int m_tileSize;

        QIcon m_progressIcon;
        QIcon m_errorIcon;

        bool m_keepObsoletePixmaps;
        bool m_useDevicePixelRatio;

        bool m_decoratePages;
        bool m_decorateLinks;
        bool m_decorateFormFields;

        QColor m_backgroundColor;
        QColor m_paperColor;

        QColor m_highlightColor;

    };

    // presentation view

    class PresentationView
    {
    public:
        DECL_NODISCARD
        bool synchronize() const;
        void setSynchronize(bool synchronize);

        DECL_NODISCARD
        int screen() const;
        void setScreen(int screen);

        DECL_NODISCARD
        QColor backgroundColor() const;
        void setBackgroundColor(const QColor& backgroundColor);

    private:
        explicit PresentationView(QSettings* settings);
        friend class Settings;

        QSettings* m_settings;

    };

    // document view

    class DocumentView
    {
    public:
        void sync();

        DECL_NODISCARD
        bool openUrl() const;
        void setOpenUrl(bool openUrl);

        DECL_NODISCARD
        bool autoRefresh() const;
        void setAutoRefresh(bool autoRefresh);

        DECL_NODISCARD
        int autoRefreshTimeout() const;

        DECL_NODISCARD
        bool prefetch() const { return m_prefetch; }
        void setPrefetch(bool prefetch);

        DECL_NODISCARD
        int prefetchDistance() const { return m_prefetchDistance; }
        void setPrefetchDistance(int prefetchDistance);

        DECL_NODISCARD
        int prefetchTimeout() const;

        DECL_NODISCARD
        int pagesPerRow() const { return m_pagesPerRow; }
        void setPagesPerRow(int pagesPerRow);

        DECL_NODISCARD
        bool minimalScrolling() const { return m_minimalScrolling; }
        void setMinimalScrolling(bool minimalScrolling);

        DECL_NODISCARD
        bool highlightCurrentThumbnail() const { return m_highlightCurrentThumbnail; }
        void setHighlightCurrentThumbnail(bool highlightCurrentThumbnail);

        DECL_NODISCARD
        bool limitThumbnailsToResults() const { return m_limitThumbnailsToResults; }
        void setLimitThumbnailsToResults(bool limitThumbnailsToResults);

        DECL_NODISCARD
        qreal minimumScaleFactor() const;
        DECL_NODISCARD
        qreal maximumScaleFactor() const;

        DECL_NODISCARD
        qreal zoomFactor() const;
        void setZoomFactor(qreal zoomFactor);

        DECL_NODISCARD
        qreal pageSpacing() const { return m_pageSpacing; }
        void setPageSpacing(qreal pageSpacing);

        DECL_NODISCARD
        qreal thumbnailSpacing() const { return m_thumbnailSpacing; }
        void setThumbnailSpacing(qreal thumbnailSpacing);

        DECL_NODISCARD
        qreal thumbnailSize() const { return m_thumbnailSize; }
        void setThumbnailSize(qreal thumbnailSize);

        DECL_NODISCARD
        bool matchCase() const;
        void setMatchCase(bool matchCase);

        DECL_NODISCARD
        bool wholeWords() const;
        void setWholeWords(bool wholeWords);

        DECL_NODISCARD
        bool parallelSearchExecution() const;
        void setParallelSearchExecution(bool parallelSearchExecution);

        DECL_NODISCARD
        int highlightDuration() const;
        void setHighlightDuration(int highlightDuration);

        DECL_NODISCARD
        QString sourceEditor() const;
        void setSourceEditor(const QString& sourceEditor);

        DECL_NODISCARD
        Qt::KeyboardModifiers zoomModifiers() const;
        void setZoomModifiers(Qt::KeyboardModifiers zoomModifiers);

        DECL_NODISCARD
        Qt::KeyboardModifiers rotateModifiers() const;
        void setRotateModifiers(Qt::KeyboardModifiers rotateModifiers);

        DECL_NODISCARD
        Qt::KeyboardModifiers scrollModifiers() const;
        void setScrollModifiers(Qt::KeyboardModifiers scrollModifiers);

        // per-tab settings

        DECL_NODISCARD
        bool continuousMode() const;
        void setContinuousMode(bool continuousMode);

        DECL_NODISCARD
        LayoutMode layoutMode() const;
        void setLayoutMode(LayoutMode layoutMode);

        DECL_NODISCARD
        bool rightToLeftMode() const;
        void setRightToLeftMode(bool rightToLeftMode);

        DECL_NODISCARD
        ScaleMode scaleMode() const;
        void setScaleMode(ScaleMode scaleMode);

        DECL_NODISCARD
        qreal scaleFactor() const;
        void setScaleFactor(qreal scaleFactor);

        DECL_NODISCARD
        Rotation rotation() const;
        void setRotation(Rotation rotation);

        DECL_NODISCARD
        bool invertColors() const;
        void setInvertColors(bool invertColors);

        DECL_NODISCARD
        bool convertToGrayscale() const;
        void setConvertToGrayscale(bool convertToGrayscale);

        DECL_NODISCARD
        bool trimMargins() const;
        void setTrimMargins(bool trimMargins);

        DECL_NODISCARD
        CompositionMode compositionMode() const;
        void setCompositionMode(CompositionMode compositionMode);

        DECL_NODISCARD
        bool highlightAll() const;
        void setHighlightAll(bool highlightAll);

    private:
        explicit DocumentView(QSettings* settings);
        friend class Settings;

        QSettings* m_settings;

        bool m_prefetch;
        int m_prefetchDistance;

        int m_pagesPerRow;

        bool m_minimalScrolling;

        bool m_highlightCurrentThumbnail;
        bool m_limitThumbnailsToResults;

        qreal m_pageSpacing;
        qreal m_thumbnailSpacing;

        qreal m_thumbnailSize;

    };

    // main window

    class MainWindow
    {
    public:
        DECL_NODISCARD
        bool trackRecentlyUsed() const;
        void setTrackRecentlyUsed(bool trackRecentlyUsed);

        DECL_NODISCARD
        int recentlyUsedCount() const;
        void setRecentlyUsedCount(int recentlyUsedCount);

        DECL_NODISCARD
        QStringList recentlyUsed() const;
        void setRecentlyUsed(const QStringList& recentlyUsed);

        DECL_NODISCARD
        bool keepRecentlyClosed() const;
        void setKeepRecentlyClosed(bool keepRecentlyClosed);

        DECL_NODISCARD
        int recentlyClosedCount() const;
        void setRecentlyClosedCount(int recentlyClosedCount);

        DECL_NODISCARD
        bool restoreTabs() const;
        void setRestoreTabs(bool restoreTabs);

        DECL_NODISCARD
        bool restoreBookmarks() const;
        void setRestoreBookmarks(bool restoreBookmarks);

        DECL_NODISCARD
        bool restorePerFileSettings() const;
        void setRestorePerFileSettings(bool restorePerFileSettings);

        DECL_NODISCARD
        int perFileSettingsLimit() const;

        DECL_NODISCARD
        int saveDatabaseInterval() const;
        void setSaveDatabaseInterval(int saveDatabaseInterval);

        DECL_NODISCARD
        int currentTabIndex() const;
        void setCurrentTabIndex(int currentTabIndex);

        DECL_NODISCARD
        int backgroundMode() const;
	    void setBackgroundMode(BackgroundMode backgroundMode);

        DECL_NODISCARD
        int tabPosition() const;
        void setTabPosition(int tabPosition);

        DECL_NODISCARD
        int tabVisibility() const;
        void setTabVisibility(int tabVisibility);

        DECL_NODISCARD
        bool spreadTabs() const;
        void setSpreadTabs(bool spreadTabs);

        DECL_NODISCARD
        bool newTabNextToCurrentTab() const;
        void setNewTabNextToCurrentTab(bool newTabNextToCurrentTab);

        DECL_NODISCARD
        bool exitAfterLastTab() const;
        void setExitAfterLastTab(bool exitAfterLastTab);

        DECL_NODISCARD
        bool documentTitleAsTabTitle() const;
        void setDocumentTitleAsTabTitle(bool documentTitleAsTabTitle);

        DECL_NODISCARD
        bool currentPageInWindowTitle() const;
        void setCurrentPageInWindowTitle(bool currentPageInWindowTitle);

        DECL_NODISCARD
        bool instanceNameInWindowTitle() const;
        void setInstanceNameInWindowTitle(bool instanceNameInWindowTitle);

        DECL_NODISCARD
        bool extendedSearchDock() const;
        void setExtendedSearchDock(bool extendedSearchDock);

        DECL_NODISCARD
        bool usePageLabel() const;
        void setUsePageLabel(bool usePageLabel);

        DECL_NODISCARD
        bool synchronizeOutlineView() const;
        void setSynchronizeOutlineView(bool synchronizeOutlineView);

        DECL_NODISCARD
        bool synchronizeSplitViews() const;
        void setSynchronizeSplitViews(bool synchronizeSplitViews);

        DECL_NODISCARD
        QStringList fileToolBar() const;
        void setFileToolBar(const QStringList& fileToolBar);

        DECL_NODISCARD
        QStringList editToolBar() const;
        void setEditToolBar(const QStringList& editToolBar);

        DECL_NODISCARD
        QStringList viewToolBar() const;
        void setViewToolBar(const QStringList& viewToolBar);

        DECL_NODISCARD
        QStringList documentContextMenu() const;
        void setDocumentContextMenu(const QStringList& documentContextMenu);

        DECL_NODISCARD
        QStringList tabContextMenu() const;
        void setTabContextMenu(const QStringList& tabContextMenu);

        DECL_NODISCARD
        bool scrollableMenus() const;
        void setScrollableMenus(bool scrollableMenus);

        DECL_NODISCARD
        bool searchableMenus() const;
        void setSearchableMenus(bool searchableMenus);

        DECL_NODISCARD
        bool toggleToolAndMenuBarsWithFullscreen() const;
        void setToggleToolAndMenuBarsWithFullscreen(bool toggleToolAndMenuBarsWithFullscreen) const;

        DECL_NODISCARD
        bool hasIconTheme() const;
        DECL_NODISCARD
        QString iconTheme() const;

        DECL_NODISCARD
        bool hasStyleSheet() const;
        DECL_NODISCARD
        QString styleSheet() const;

        DECL_NODISCARD
        QByteArray geometry() const;
        void setGeometry(const QByteArray& geometry);

        DECL_NODISCARD
        QByteArray state() const;
        void setState(const QByteArray& state);

        DECL_NODISCARD
        QString openPath() const;
        void setOpenPath(const QString& openPath);

        DECL_NODISCARD
        QString savePath() const;
        void setSavePath(const QString& savePath);

        DECL_NODISCARD
        QSize settingsDialogSize(QSize sizeHint = {}) const;
        void setSettingsDialogSize(QSize settingsDialogSize);

        DECL_NODISCARD
        QSize fontsDialogSize(QSize sizeHint = {}) const;
        void setFontsDialogSize(QSize fontsDialogSize = {});

        DECL_NODISCARD
        QSize contentsDialogSize(QSize sizeHint) const;
        void setContentsDialogSize(QSize contentsDialogSize);

    private:
        explicit MainWindow(QSettings* settings);
        friend class Settings;

        QSettings* m_settings;

    };

    // print dialog

    class PrintDialog
    {
    public:
        DECL_NODISCARD
        bool collateCopies() const;
        void setCollateCopies(bool collateCopies);

        DECL_NODISCARD
        QPrinter::PageOrder pageOrder() const;
        void setPageOrder(QPrinter::PageOrder pageOrder);

        DECL_NODISCARD
        QPageLayout::Orientation orientation() const;
        void setOrientation(QPageLayout::Orientation orientation);

        DECL_NODISCARD
        QPrinter::ColorMode colorMode() const;
        void setColorMode(QPrinter::ColorMode colorMode);

        DECL_NODISCARD
        QPrinter::DuplexMode duplex() const;
        void setDuplex(QPrinter::DuplexMode duplex);

        DECL_NODISCARD
        bool fitToPage() const;
        void setFitToPage(bool fitToPage);

#if QT_VERSION < QT_VERSION_CHECK(5,2,0)

        PrintOptions::PageSet pageSet() const;
        void setPageSet(PrintOptions::PageSet pageSet);

        PrintOptions::NumberUp numberUp() const;
        void setNumberUp(PrintOptions::NumberUp numberUp);

        PrintOptions::NumberUpLayout numberUpLayout() const;
        void setNumberUpLayout(PrintOptions::NumberUpLayout numberUpLayout);

#endif // QT_VERSION

    private:
        explicit PrintDialog(QSettings* settings);
        friend class Settings;

        QSettings* m_settings;

    };

    void sync();

    PageItem& pageItem() { return m_pageItem; }
    PresentationView& presentationView() { return m_presentationView; }
    DocumentView& documentView() { return m_documentView; }
    MainWindow& mainWindow() { return m_mainWindow; }
    PrintDialog& printDialog() { return m_printDialog; }

private:
    Q_DISABLE_COPY(Settings)

    static Settings* s_instance;
    explicit Settings(QObject* parent = nullptr);

    QSettings* m_settings;

    PageItem m_pageItem;
    PresentationView m_presentationView;
    DocumentView m_documentView;
    MainWindow m_mainWindow;
    PrintDialog m_printDialog;

};

// defaults

class Defaults
{
public:
    class PageItem
    {
    public:
        static int cacheSize() { return 32 * 1024; }

        static bool useTiling() { return false; }
        static int tileSize() { return 1024; }

        static bool keepObsoletePixmaps() { return false; }
        static bool useDevicePixelRatio() { return false; }

        static bool decoratePages() { return true; }
        static bool decorateLinks() { return true; }
        static bool decorateFormFields() { return true; }

        static QColor backgroundColor() { return Qt::darkGray; }
        static QColor paperColor() { return Qt::white; }

        static QColor highlightColor() { return Qt::yellow; }
        static QColor annotationColor() { return Qt::yellow; }

        static Qt::KeyboardModifiers copyToClipboardModifiers() { return Qt::ShiftModifier; }
        static Qt::KeyboardModifiers addAnnotationModifiers() { return Qt::ControlModifier; }
        static Qt::KeyboardModifiers zoomToSelectionModifiers() { return Qt::ShiftModifier | Qt::ControlModifier; }
        static Qt::KeyboardModifiers openInSourceEditorModifiers() { return Qt::NoModifier; }

        static bool annotationOverlay() { return false; }
        static bool formFieldOverlay() { return true; }

    private:
        PageItem() = default;

    };

    class PresentationView
    {
    public:
        static bool synchronize() { return false; }
        static int screen() { return -1; }

        static QColor backgroundColor() { return {}; }

    private:
        PresentationView() = default;

    };

    class DocumentView
    {
    public:
        static bool openUrl() { return false; }

        static bool autoRefresh() { return false; }

        static int autoRefreshTimeout() { return 750; }

        static bool prefetch() { return false; }
        static int prefetchDistance() { return 1; }

        static int prefetchTimeout() { return 250; }

        static int pagesPerRow() { return 3; }

        static bool minimalScrolling() { return false; }

        static bool highlightCurrentThumbnail() { return false; }
        static bool limitThumbnailsToResults() { return false; }

        static qreal minimumScaleFactor() { return 0.1; }
        static qreal maximumScaleFactor() { return 50.0; }

        static qreal zoomFactor() { return 1.1; }

        static qreal pageSpacing() { return 5.0; }
        static qreal thumbnailSpacing() { return 3.0; }

        static qreal thumbnailSize() { return 150.0; }

        static CompositionMode compositionMode() { return DefaultCompositionMode; }

        static bool matchCase() { return false; }
        static bool wholeWords() { return false; }
        static bool parallelSearchExecution() { return false; }

        static int highlightDuration() { return 5 * 1000; }
        static QString sourceEditor() { return {}; }

        static Qt::KeyboardModifiers zoomModifiers() { return Qt::ControlModifier; }
        static Qt::KeyboardModifiers rotateModifiers() { return Qt::ShiftModifier; }
        static Qt::KeyboardModifiers scrollModifiers() { return Qt::AltModifier; }

        // per-tab defaults

        static bool continuousMode() { return false; }
        static LayoutMode layoutMode() { return SinglePageMode; }
        static bool rightToLeftMode();

        static ScaleMode scaleMode() { return ScaleFactorMode; }
        static qreal scaleFactor() { return 1.0; }
        static Rotation rotation() { return RotateBy0; }

        static bool invertColors() { return false; }
        static bool convertToGrayscale() { return false; }
        static bool trimMargins() { return false; }

        static bool highlightAll() { return false; }

    private:
        DocumentView() = default;
    };

    class MainWindow
    {
    public:
        static bool trackRecentlyUsed() { return false; }
        static int recentlyUsedCount() { return 10; }

        static bool keepRecentlyClosed() { return false; }
        static int recentlyClosedCount() { return 5; }

        static bool restoreTabs() { return false; }
        static bool restoreBookmarks() { return false; }
        static bool restorePerFileSettings() { return false; }

        static int perFileSettingsLimit() { return 1000; }

        static int saveDatabaseInterval() { return 5 * 60 * 1000; }

	    static int backgroundMode() { return 0; };

        static int tabPosition() { return 0; }
        static int tabVisibility() { return 0; }

        static bool spreadTabs() { return false; }

        static bool newTabNextToCurrentTab() { return true; }
        static bool exitAfterLastTab() { return false; }

        static bool documentTitleAsTabTitle() { return true; }

        static bool currentPageInWindowTitle() { return false; }
        static bool instanceNameInWindowTitle() { return false; }

        static bool extendedSearchDock() { return false; }

        static bool usePageLabel() { return true; }

        static bool synchronizeOutlineView() { return false; }
        static bool synchronizeSplitViews() { return true; }

        static QStringList fileToolBar()
        {
        	return QStringList()
        	<< "open"
        	<< "openInNewTab"
        	<< "openContainingFolder"
        	<< "refresh";
        }
       
        static QStringList editToolBar()
        {
        	return QStringList()
        	<< "firstPage"
        	<< "jumpBackward"
        	<< "previousPage"
        	<< "currentPage"
        	<< "nextPage"
        	<< "jumpForward"
        	<< "jumpToPage"
        	<< "lastPage";
        }
        
        static QStringList viewToolBar()
        {
        	return QStringList()
        	<< "scaleFactor"
        	<< "originalSize"
        	<< "zoomIn"
        	<< "zoomOut"
        	<< "fitToPageWidthMode"
        	<< "fitToPageSizeMode"
        	<< "continuousMode"
        	<< "twoPagesMode"
        	<< "twoPagesWithCoverPageMode"
        	<< "multiplePagesMode"
        	<< "rotateLeft"
        	<< "rotateRight"
        	<< "fullscreen";
        }

        static QStringList documentContextMenu()
        {
        	return QStringList()
        	<< "previousPage"
        	<< "nextPage"
        	<< "firstPage"
        	<< "lastPage"
        	<< "separator"
        	<< "jumpToPage"
        	<< "jumpBackward"
        	<< "jumpForward"
        	<< "separator"
        	<< "setFirstPage"
        	<< "separator"
        	<< "findPrevious"
        	<< "findNext"
        	<< "cancelSearch";
        }
        static QStringList tabContexntMenu()
        {
        	return QStringList()
        	<< "openCopyInNewTab"
        	<< "openCopyInNewWindow"
        	<< "openContainingFolder"
        	<< "separator"
        	<< "splitViewHorizontally"
        	<< "splitViewVertically"
        	<< "closeCurrentView"
        	<< "separator"
        	<< "closeAllTabs"
        	<< "closeAllTabsButThisOne"
        	<< "closeAllTabsToTheLeft"
        	<< "closeAllTabsToTheRight";
        }

        static bool scrollableMenus() { return false; }
        static bool searchableMenus() { return false; }

        static bool toggleToolAndMenuBarsWithFullscreen() { return false; }

        static QString path();

    private:
        MainWindow() = default;

    };

    class PrintDialog
    {
    public:
        static bool collateCopies() { return false; }

        static QPrinter::PageOrder pageOrder() { return QPrinter::FirstPageFirst; }

        static QPrinter::Orientation orientation() { return QPrinter::Portrait; }

        static QPrinter::ColorMode colorMode() { return QPrinter::Color; }

        static QPrinter::DuplexMode duplex() { return QPrinter::DuplexNone; }

        static bool fitToPage() { return false; }

#if QT_VERSION < QT_VERSION_CHECK(5,2,0)

        static PrintOptions::PageSet pageSet() { return PrintOptions::AllPages; }

        static PrintOptions::NumberUp numberUp() { return PrintOptions::SinglePage; }
        static PrintOptions::NumberUpLayout numberUpLayout() { return PrintOptions::LeftRightTopBottom; }

#endif // QT_VERSION

    private:
        PrintDialog() = default;
    };

private:
    Defaults() = default;
};

} // qpdfview

#endif // SETTINGS_H
