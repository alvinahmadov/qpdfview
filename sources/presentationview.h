/*

Copyright 2012-2013 Adam Reichold

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

#ifndef PRESENTATIONVIEW_H
#define PRESENTATIONVIEW_H

#include <QGraphicsView>

#include "renderparam.h"

namespace qpdfview
{

namespace Model
{
class Page;
}

class Settings;
class PageItem;

class PresentationView final : public QGraphicsView
{
    Q_OBJECT

public:
    explicit PresentationView(const QVector< Model::Page* >& pages, QWidget* parent = nullptr);
    ~PresentationView() final;

    DECL_NODISCARD
    int numberOfPages() const { return m_pages.count(); }
    DECL_NODISCARD
    int currentPage() const { return m_currentPage; }

    DECL_NODISCARD
    ScaleMode scaleMode() const { return m_scaleMode; }
    void setScaleMode(ScaleMode scaleMode);

    DECL_NODISCARD
    qreal scaleFactor() const { return m_scaleFactor; }
    void setScaleFactor(qreal scaleFactor);

    DECL_NODISCARD
    Rotation rotation() const { return m_rotation; }
    void setRotation(Rotation rotation);

    DECL_NODISCARD
    qpdfview::RenderFlags renderFlags() const { return m_renderFlags; }
    void setRenderFlags(qpdfview::RenderFlags renderFlags);

signals:
    void currentPageChanged(int currentPage, bool trackChange = false);

    void scaleModeChanged(qpdfview::ScaleMode scaleMode);
    void scaleFactorChanged(qreal scaleFactor);
    void rotationChanged(qpdfview::Rotation rotation);

    void renderFlagsChanged(qpdfview::RenderFlags renderFlags);
    
public slots:
    void show();

    void previousPage();
    void nextPage();
    void firstPage();
    void lastPage();

    void jumpToPage(int page, bool trackChange = true);

    void jumpBackward();
    void jumpForward();

    void zoomIn();
    void zoomOut();
    void originalSize();

    void rotateLeft();
    void rotateRight();

protected slots:
    void onPrefetchTimeout();

    void onPagesCropRectChanged();

    void onPagesLinkClicked(bool newTab, int page, qreal left, qreal top);

protected:
    void resizeEvent(QResizeEvent* event) final;

    void keyPressEvent(QKeyEvent* event) final;
    void wheelEvent(QWheelEvent* event) final;

private:
    Q_DISABLE_COPY(PresentationView)

    static Settings* s_settings;

    QTimer* m_prefetchTimer;

    QVector< Model::Page* > m_pages;

    int m_currentPage;

    QList< int > m_past;
    QList< int > m_future;

    ScaleMode m_scaleMode;
    qreal m_scaleFactor;
    Rotation m_rotation;

    qpdfview::RenderFlags m_renderFlags;

    QVector< PageItem* > m_pageItems;

    void preparePages();
    void prepareBackground();

    void prepareScene();
    void prepareView();
    
};

} // qpdfview

#endif // PRESENTATIONVIEW_H
