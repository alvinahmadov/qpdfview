/*

Copyright 2014 S. Razi Alavizadeh
Copyright 2013-2014 Adam Reichold

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

#include "documentlayout.h"

#include "settings.h"
#include "pageitem.h"

namespace
{

using namespace qpdfview;

const qreal viewportPadding = 6.0f;

} // anonymous

namespace qpdfview
{

Settings* DocumentLayout::s_settings = nullptr;

DocumentLayout::DocumentLayout()
{
    if(s_settings == nullptr)
    {
        s_settings = Settings::instance();
    }
}

DocumentLayout* DocumentLayout::fromLayoutMode(LayoutMode layoutMode)
{
    switch(layoutMode)
    {
    default:
    case SinglePageMode: return new SinglePageLayout;
    case TwoPagesMode: return new TwoPagesLayout;
    case TwoPagesWithCoverPageMode: return new TwoPagesWithCoverPageLayout;
    case MultiplePagesMode: return new MultiplePagesLayout;
    }
}

bool DocumentLayout::isCurrentPage(const QRectF& visibleRect, const QRectF& pageRect)
{
    // Works with vertically scrolling layouts, i.e. all currently implemented layouts.
    const qreal pageVisibleHeight = pageRect.intersected(visibleRect).height();
    const qreal pageTopOffset = pageRect.top() - visibleRect.top();

    if(visibleRect.height() > 2.0f * pageRect.height()) // Are more than two pages visible?
    {
        const qreal halfPageHeight = 0.5f * pageRect.height();

        return pageVisibleHeight >= halfPageHeight && pageTopOffset < halfPageHeight && pageTopOffset >= -halfPageHeight;
    }
    else
    {
        return pageVisibleHeight >= 0.5f * visibleRect.height();
    }
}

qreal DocumentLayout::visibleHeight(int viewportHeight)
{
    const qreal pageSpacing = s_settings->documentView().pageSpacing();

    return viewportHeight - 2.0f * pageSpacing;
}


int SinglePageLayout::currentPage(int page) const
{
    return page;
}

int SinglePageLayout::previousPage(int page) const
{
    return std::max(page - 1, 1);
}

int SinglePageLayout::nextPage(int page, int count) const
{
    return std::min(page + 1, count);
}

QPair< int, int > SinglePageLayout::prefetchRange(int page, int count) const
{
    const int prefetchDistance = s_settings->documentView().prefetchDistance();

    return qMakePair(std::max(page - prefetchDistance / 2, 1),
                     std::min(page + prefetchDistance, count));
}

int SinglePageLayout::leftIndex(int index) const
{
    return index;
}

int SinglePageLayout::rightIndex(int index, int count) const
{
    Q_UNUSED(count)

    return index;
}

qreal SinglePageLayout::visibleWidth(int viewportWidth) const
{
    const qreal pageSpacing = s_settings->documentView().pageSpacing();

    return viewportWidth - viewportPadding - 2.0f * pageSpacing;
}

void SinglePageLayout::prepareLayout(const QVector< PageItem* >& pageItems, bool /* rightToLeft */,
                                     qreal& left, qreal& right, qreal& height)
{
    const qreal pageSpacing = s_settings->documentView().pageSpacing();

    for(int index = 0; index < pageItems.count(); ++index)
    {
        PageItem* page = pageItems.at(index);
        const QRectF boundingRect = page->boundingRect();

        page->setPos(-boundingRect.left() - 0.5f * boundingRect.width(), height - boundingRect.top());

        qreal pageHeight = boundingRect.height();
        qreal pageWidth = boundingRect.width();

        left = std::min(left, -0.5f * pageWidth - pageSpacing);
        right = std::max(right, 0.5f * pageWidth + pageSpacing);
        height += pageHeight + pageSpacing;
    }
}


int TwoPagesLayout::currentPage(int page) const
{
    return page % 2 != 0 ? page : page - 1;
}

int TwoPagesLayout::previousPage(int page) const
{
    return std::max(page - 2, 1);
}

int TwoPagesLayout::nextPage(int page, int count) const
{
    return std::min(page + 2, count);
}

QPair< int, int > TwoPagesLayout::prefetchRange(int page, int count) const
{
    const int prefetchDistance = s_settings->documentView().prefetchDistance();

    return qMakePair(std::max(page - prefetchDistance, 1),
                     std::min(page + 2 * prefetchDistance + 1, count));
}

int TwoPagesLayout::leftIndex(int index) const
{
    return index % 2 == 0 ? index : index - 1;
}

int TwoPagesLayout::rightIndex(int index, int count) const
{
    return std::min(index % 2 == 0 ? index + 1 : index, count - 1);
}

qreal TwoPagesLayout::visibleWidth(int viewportWidth) const
{
    const qreal pageSpacing = s_settings->documentView().pageSpacing();

    return (viewportWidth - viewportPadding - 3.0f * pageSpacing) / 2.0f;
}

void TwoPagesLayout::prepareLayout(const QVector< PageItem* >& pageItems, bool rightToLeft,
                                   qreal& left, qreal& right, qreal& height)
{
    const qreal pageSpacing = s_settings->documentView().pageSpacing();
    qreal pageHeight = 0.0f;

    for(int index = 0; index < pageItems.count(); ++index)
    {
        PageItem* page = pageItems.at(index);
        const QRectF boundingRect = page->boundingRect();

        const qreal leftPos = -boundingRect.left() - boundingRect.width() - 0.5f * pageSpacing;
        const qreal rightPos = -boundingRect.left() + 0.5f * pageSpacing;

        if(index == leftIndex(index))
        {
            page->setPos(rightToLeft ? rightPos : leftPos, height - boundingRect.top());

            pageHeight = boundingRect.height();

            if(rightToLeft)
            {
                right = std::max(right, boundingRect.width() + 1.5f * pageSpacing);
            }
            else
            {
                left = std::min(left, -boundingRect.width() - 1.5f * pageSpacing);
            }

            if(index == rightIndex(index, pageItems.count()))
            {
                right = std::max(right, 0.5f * pageSpacing);
                height += pageHeight + pageSpacing;
            }
        }
        else
        {
            page->setPos(rightToLeft ? leftPos : rightPos, height - boundingRect.top());

            pageHeight = std::max(pageHeight, boundingRect.height());

            if(rightToLeft)
            {
                left = std::min(left, -boundingRect.width() - 1.5f * pageSpacing);
            }
            else
            {
                right = std::max(right, boundingRect.width() + 1.5f * pageSpacing);
            }

            height += pageHeight + pageSpacing;
        }
    }
}


int TwoPagesWithCoverPageLayout::currentPage(int page) const
{
    return page == 1 ? page : (page % 2 == 0 ? page : page - 1);
}

int TwoPagesWithCoverPageLayout::leftIndex(int index) const
{
    return index == 0 ? index : (index % 2 != 0 ? index : index - 1);
}

int TwoPagesWithCoverPageLayout::rightIndex(int index, int count) const
{
    return std::min(index % 2 != 0 ? index + 1 : index, count - 1);
}


int MultiplePagesLayout::currentPage(int page) const
{
    const int pagesPerRow = s_settings->documentView().pagesPerRow();

    return page - ((page - 1) % pagesPerRow);
}

int MultiplePagesLayout::previousPage(int page) const
{
    const int pagesPerRow = s_settings->documentView().pagesPerRow();

    return std::max(page - pagesPerRow, 1);
}

int MultiplePagesLayout::nextPage(int page, int count) const
{
    const int pagesPerRow = s_settings->documentView().pagesPerRow();

    return std::min(page + pagesPerRow, count);
}

QPair<int, int> MultiplePagesLayout::prefetchRange(int page, int count) const
{
    const int prefetchDistance = s_settings->documentView().prefetchDistance();
    const int pagesPerRow = s_settings->documentView().pagesPerRow();

    return qMakePair(std::max(page - pagesPerRow * (prefetchDistance / 2), 1),
                     std::min(page + pagesPerRow * (prefetchDistance + 1) - 1, count));
}

int MultiplePagesLayout::leftIndex(int index) const
{
    const int pagesPerRow = s_settings->documentView().pagesPerRow();

    return index - (index % pagesPerRow);
}

int MultiplePagesLayout::rightIndex(int index, int count) const
{
    const int pagesPerRow = s_settings->documentView().pagesPerRow();

    return std::min(index - (index % pagesPerRow) + pagesPerRow - 1, count - 1);
}

qreal MultiplePagesLayout::visibleWidth(int viewportWidth) const
{
    const qreal pageSpacing = s_settings->documentView().pageSpacing();
    const int pagesPerRow = s_settings->documentView().pagesPerRow();

    return (viewportWidth - viewportPadding - (pagesPerRow + 1) * pageSpacing) / pagesPerRow;
}

void MultiplePagesLayout::prepareLayout(const QVector< PageItem* >& pageItems, bool rightToLeft,
                                        qreal& left, qreal& right, qreal& height)
{
    const qreal pageSpacing = s_settings->documentView().pageSpacing();
    qreal pageHeight = 0.0;

    for(int index = 0; index < pageItems.count(); ++index)
    {
        PageItem* page = pageItems.at(index);
        const QRectF boundingRect = page->boundingRect();

        const qreal leftPos = left - boundingRect.left() + pageSpacing;
        const qreal rightPos = right - boundingRect.left() - boundingRect.width() - pageSpacing;

        page->setPos(rightToLeft ? rightPos : leftPos, height - boundingRect.top());

        pageHeight = std::max(pageHeight, boundingRect.height());

        if(rightToLeft)
        {
            right -= boundingRect.width() + pageSpacing;
        }
        else
        {
            left += boundingRect.width() + pageSpacing;
        }

        if(index == rightIndex(index, pageItems.count()))
        {
            height += pageHeight + pageSpacing;
            pageHeight = 0.0f;

            if(rightToLeft)
            {
                left = std::min(left, right - pageSpacing);
                right = 0.0f;
            }
            else
            {
                right = std::max(right, left + pageSpacing);
                left = 0.0f;
            }
        }
    }
}

} // qpdfview
