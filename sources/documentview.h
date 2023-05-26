/*

Copyright 2014 S. Razi Alavizadeh
Copyright 2012-2014, 2018 Adam Reichold
Copyright 2014 Dorian Scholz
Copyright 2018 Egor Zenkov

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

#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include <QFileInfo>
#include <QGraphicsView>
#include <QMap>
#include <QPersistentModelIndex>

class QDomNode;
class QFileSystemWatcher;
class QPrinter;

#include "renderparam.h"
#include "printoptions.h"

namespace qpdfview
{

namespace Model
{
class Annotation;
class Page;
class Document;
}

class Settings;
class PageItem;
class ThumbnailItem;
class SearchModel;
class SearchTask;
class PresentationView;
class ShortcutHandler;
struct DocumentLayout;

class DocumentView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit DocumentView(QWidget* parent = nullptr);
    ~DocumentView() override;

    DECL_NODISCARD
    const QFileInfo& fileInfo() const { return m_fileInfo; }
    DECL_NODISCARD
    bool wasModified() const { return m_wasModified; }

    DECL_NODISCARD
    int numberOfPages() const { return m_pages.count(); }
    DECL_NODISCARD
    int currentPage() const { return m_currentPage; }

    DECL_NODISCARD
    bool hasFrontMatter() const { return m_firstPage> 1; }

    DECL_NODISCARD
    int firstPage() const { return m_firstPage; }
    void setFirstPage(int firstPage);

    DECL_NODISCARD
    QString defaultPageLabelFromNumber(int number) const;
    DECL_NODISCARD
    QString pageLabelFromNumber(int number) const;
    DECL_NODISCARD
    int pageNumberFromLabel(const QString& label) const;

    DECL_NODISCARD
    QString title() const;

    static QStringList openFilter();
    DECL_NODISCARD
    QStringList saveFilter() const;

    DECL_NODISCARD
    bool canSave() const;

    DECL_NODISCARD
    bool continuousMode() const { return m_continuousMode; }
    void setContinuousMode(bool continuousMode);

    DECL_NODISCARD
    qpdfview::LayoutMode layoutMode() const;
    void setLayoutMode(qpdfview::LayoutMode layoutMode);

    DECL_NODISCARD
    bool rightToLeftMode() const { return m_rightToLeftMode; }
    void setRightToLeftMode(bool rightToLeftMode);

    DECL_NODISCARD
    qpdfview::ScaleMode scaleMode() const { return m_scaleMode; }
    void setScaleMode(qpdfview::ScaleMode scaleMode);

    DECL_NODISCARD
    qreal scaleFactor() const { return m_scaleFactor; }
    void setScaleFactor(qreal scaleFactor);

    DECL_NODISCARD
    qpdfview::Rotation rotation() const { return m_rotation; }
    void setRotation(qpdfview::Rotation rotation);

    DECL_NODISCARD
    qpdfview::RenderFlags renderFlags() const { return m_renderFlags; }
    void setRenderFlags(qpdfview::RenderFlags renderFlags);
    void setRenderFlag(qpdfview::RenderFlag renderFlag, bool enabled = true);

    DECL_NODISCARD
    bool invertColors() const { return m_renderFlags.testFlag(InvertColors); }
    void setInvertColors(bool invertColors) { setRenderFlag(InvertColors, invertColors); }

    DECL_NODISCARD
    bool convertToGrayscale() const { return m_renderFlags.testFlag(ConvertToGrayscale); }
    void setConvertToGrayscale(bool convertToGrayscale) { setRenderFlag(ConvertToGrayscale, convertToGrayscale); }

    DECL_NODISCARD
    bool trimMargins() const { return m_renderFlags.testFlag(TrimMargins); }
    void setTrimMargins(bool trimMargins) { setRenderFlag(TrimMargins, trimMargins); }

    DECL_NODISCARD
    qpdfview::CompositionMode compositionMode() const;
    void setCompositionMode(qpdfview::CompositionMode compositionMode);

    DECL_NODISCARD
    bool highlightAll() const { return m_highlightAll; }
    void setHighlightAll(bool highlightAll);

    DECL_NODISCARD
    qpdfview::RubberBandMode rubberBandMode() const { return m_rubberBandMode; }
    void setRubberBandMode(qpdfview::RubberBandMode rubberBandMode);

    DECL_UNUSED
    DECL_NODISCARD
    QSize thumbnailsViewportSize() const { return m_thumbnailsViewportSize; }
    void setThumbnailsViewportSize(QSize thumbnailsViewportSize);

    DECL_UNUSED
    DECL_NODISCARD
    Qt::Orientation thumbnailsOrientation() const { return m_thumbnailsOrientation; }
    void setThumbnailsOrientation(Qt::Orientation thumbnailsOrientation);

    DECL_NODISCARD
    const QVector<qpdfview::ThumbnailItem*>& thumbnailItems() const { return m_thumbnailItems; }
    DECL_NODISCARD
    QGraphicsScene* thumbnailsScene() const { return m_thumbnailsScene; }

    DECL_NODISCARD
    QAbstractItemModel* outlineModel() const { return m_outlineModel.data(); }
    DECL_NODISCARD
    QAbstractItemModel* propertiesModel() const { return m_propertiesModel.data(); }

    DECL_NODISCARD
    QSet<QByteArray> saveExpandedPaths() const;
    void restoreExpandedPaths(const QSet<QByteArray>& expandedPaths);

    DECL_NODISCARD
    QAbstractItemModel* fontsModel() const;

    DECL_NODISCARD
    bool searchWasCanceled() const;
    DECL_NODISCARD
    int searchProgress() const;

    DECL_NODISCARD
    QString searchText() const;
    DECL_NODISCARD
    bool searchMatchCase() const;
    DECL_NODISCARD
    bool searchWholeWords() const;

    DECL_NODISCARD
    QPair<QString, QString> searchContext(int page, const QRectF& rect) const;

    bool hasSearchResults();

    DECL_NODISCARD
    QString resolveFileName(QString fileName) const;
    DECL_NODISCARD
    QUrl resolveUrl(QUrl url) const;

    struct SourceLink
    {
        QString name;
        int line {};
        int column {};

        explicit operator bool() const { return !name.isNull(); }

    };

    SourceLink sourceLink(QPoint pos);
    void openInSourceEditor(const SourceLink& sourceLink);

signals:
    void documentChanged();
    void documentModified();

    void numberOfPagesChanged(int numberOfPages);
    void currentPageChanged(int currentPage, bool trackChange = false);

    void canJumpChanged(bool backward, bool forward);

    void continuousModeChanged(bool continuousMode);
    void layoutModeChanged(qpdfview::LayoutMode layoutMode);
    void rightToLeftModeChanged(bool rightToLeftMode);
    void scaleModeChanged(qpdfview::ScaleMode scaleMode);
    void scaleFactorChanged(qreal scaleFactor);
    void rotationChanged(qpdfview::Rotation rotation);

    void linkClicked(int page);
    void linkClicked(bool newTab, const QString& filePath, int page);

    void renderFlagsChanged(qpdfview::RenderFlags renderFlags);

    void invertColorsChanged(bool invertColors);
    void convertToGrayscaleChanged(bool convertToGrayscale);
    void trimMarginsChanged(bool trimMargins);

    void compositionModeChanged(qpdfview::CompositionMode compositionMode);

    void highlightAllChanged(bool highlightAll);
    void rubberBandModeChanged(qpdfview::RubberBandMode rubberBandMode);

    void searchFinished();
    void searchProgressChanged(int progress);

public slots:
    void show();

    bool open(const QString& filePath);
    bool refresh();
    bool save(const QString& filePath, bool withChanges);
    bool print(QPrinter* printer, const qpdfview::PrintOptions& printOptions = PrintOptions());

    void previousPage();
    void nextPage();
    DECL_UNUSED
    void firstPage();
    DECL_UNUSED
    void lastPage();

    void jumpToPage(int page, bool trackChange = true, qreal newLeft = qQNaN(), qreal newTop = qQNaN());

    DECL_UNUSED
    DECL_NODISCARD
    bool canJumpBackward() const;
    void jumpBackward();

    DECL_UNUSED
    DECL_NODISCARD
    bool canJumpForward() const;
    void jumpForward();

    DECL_UNUSED
    void temporaryHighlight(int page, const QRectF& highlight);

    DECL_UNUSED
    void startSearch(const QString& text, bool matchCase, bool wholeWords);
    void cancelSearch();

    void clearResults();

    DECL_UNUSED
    void findPrevious();
    void findNext();
    DECL_UNUSED
    void findResult(QModelIndex index);

    void zoomIn();
    void zoomOut();
    void originalSize();

    void rotateLeft();
    void rotateRight();

    void startPresentation();

protected slots:
    void onVerticalScrollBarValueChanged();

    void onAutoRefreshTimeout();
    void onPrefetchTimeout();

    void onTemporaryHighlightTimeout();

    void onSearchTaskProgressChanged(int progress);
    void onSearchTaskResultsReady(int index, const QList<QRectF>& results);

    void onPagesCropRectChanged();
    void onThumbnailsCropRectChanged();

    DECL_UNUSED
    void onPagesLinkClicked(bool newTab, int page, qreal left, qreal top);
    DECL_UNUSED
    void onPagesLinkClicked(bool newTab, const QString& fileName, int page);
    void onPagesLinkClicked(const QString& url);

    void onPagesRubberBandFinished();

    void onPagesZoomToSelection(int page, const QRectF& rect);
    void onPagesOpenInSourceEditor(int page, QPointF pos);

    void onPagesWasModified();

protected:
    void resizeEvent(QResizeEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    Q_DISABLE_COPY(DocumentView)

    static Settings* s_settings;
    static ShortcutHandler* s_shortcutHandler;

    QFileSystemWatcher* m_autoRefreshWatcher;
    QTimer* m_autoRefreshTimer;

    QTimer* m_prefetchTimer;

    Model::Document* m_document;
    QVector<Model::Page*> m_pages;

    QFileInfo m_fileInfo;
    bool m_wasModified;

    int m_currentPage;
    int m_firstPage;

#ifdef WITH_CUPS

    bool printUsingCUPS(QPrinter* printer, const PrintOptions& printOptions, int fromPage, int toPage);

#endif // WITH_CUPS

    bool printUsingQt(QPrinter* printer, const PrintOptions& printOptions, int fromPage, int toPage);

    struct Position
    {
        int page;
        qreal left;
        qreal top;

        Position(int page, qreal left, qreal top) : page(page), left(left), top(top) {}

    };

    QList<Position> m_past;
    QList<Position> m_future;

    void saveLeftAndTop(qreal& left, qreal& top) const;

    QScopedPointer<DocumentLayout> m_layout;

    bool m_continuousMode;
    bool m_rightToLeftMode;
    ScaleMode m_scaleMode;
    qreal m_scaleFactor;
    Rotation m_rotation;

    qpdfview::RenderFlags m_renderFlags;

    bool m_highlightAll;
    RubberBandMode m_rubberBandMode;

    QVector<PageItem*> m_pageItems;
    QVector<ThumbnailItem*> m_thumbnailItems;

    QGraphicsRectItem* m_highlight;

    QSize m_thumbnailsViewportSize;
    Qt::Orientation m_thumbnailsOrientation;

    QGraphicsScene* m_thumbnailsScene;

    QScopedPointer<QAbstractItemModel> m_outlineModel;
    QScopedPointer<QAbstractItemModel> m_propertiesModel;

    bool checkDocument(const QString& filePath, Model::Document* document, QVector<Model::Page*>& pages);

    void loadDocumentDefaults();

    void adjustScrollBarPolicy();

    bool m_verticalScrollBarChangedBlocked;

    class VerticalScrollBarChangedBlocker;

    void prepareDocument(Model::Document* document, const QVector<Model::Page*>& pages);
    void preparePages();
    void prepareThumbnails();
    void prepareBackground();

    void prepareScene();
    void prepareView(qreal newLeft = 0.0, qreal newTop = 0.0, bool forceScroll = true, int scrollToPage = 0);

    void prepareThumbnailsScene();

    void prepareHighlight(int index, const QRectF& highlight);

    // search

    static SearchModel* s_searchModel;

    QPersistentModelIndex m_currentResult;

    SearchTask* m_searchTask;

    void checkResult();
    void applyResult();

};

} // qpdfview

Q_DECLARE_METATYPE(qpdfview::DocumentView::SourceLink)

#endif // DOCUMENTVIEW_H
