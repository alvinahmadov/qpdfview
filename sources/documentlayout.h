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

#ifndef DOCUMENTLAYOUT_H
#define DOCUMENTLAYOUT_H

#include <QMap>
#include <QPair>

#include "global.h"

class QRectF;

namespace qpdfview
{

class Settings;
class PageItem;

struct DocumentLayout
{
    DocumentLayout();
    virtual ~DocumentLayout() = default;

    static DocumentLayout* fromLayoutMode(LayoutMode layoutMode);

    DECL_NODISCARD
    virtual LayoutMode layoutMode() const = 0;

    DECL_NODISCARD
    virtual int currentPage(int page) const = 0;
    DECL_NODISCARD
    virtual int previousPage(int page) const = 0;
    DECL_NODISCARD
    virtual int nextPage(int page, int count) const = 0;

    DECL_NODISCARD
    static bool isCurrentPage(const QRectF& visibleRect, const QRectF& pageRect);

    DECL_NODISCARD
    virtual QPair< int, int > prefetchRange(int page, int count) const = 0;

    DECL_NODISCARD
    virtual int leftIndex(int index) const = 0;
    DECL_NODISCARD
    virtual int rightIndex(int index, int count) const = 0;

    DECL_NODISCARD
    virtual qreal visibleWidth(int viewportWidth) const = 0;
    DECL_NODISCARD
    static qreal visibleHeight(int viewportHeight);

    virtual void prepareLayout(const QVector< PageItem* >& pageItems, bool rightToLeft,
                               qreal& left, qreal& right, qreal& height) = 0;

protected:
    static Settings* s_settings;

};

struct SinglePageLayout : public DocumentLayout
{
    DECL_NODISCARD
    LayoutMode layoutMode() const override { return SinglePageMode; }

    DECL_NODISCARD
    int currentPage(int page) const override;
    DECL_NODISCARD
    int previousPage(int page) const override;
    DECL_NODISCARD
    int nextPage(int page, int count) const override;

    DECL_NODISCARD
    QPair< int, int > prefetchRange(int page, int count) const override;

    DECL_NODISCARD
    int leftIndex(int index) const override;
    DECL_NODISCARD
    int rightIndex(int index, int count) const override;

    DECL_NODISCARD
    qreal visibleWidth(int viewportWidth) const override;

    void prepareLayout(const QVector< PageItem* >& pageItems, bool rightToLeft,
                       qreal& left, qreal& right, qreal& height) override;

};

struct TwoPagesLayout : public DocumentLayout
{
    DECL_NODISCARD
    LayoutMode layoutMode() const override { return TwoPagesMode; }

    DECL_NODISCARD
    int currentPage(int page) const override;
    DECL_NODISCARD
    int previousPage(int page) const override;
    DECL_NODISCARD
    int nextPage(int page, int count) const override;

    DECL_NODISCARD
    QPair< int, int > prefetchRange(int page, int count) const override;

    DECL_NODISCARD
    int leftIndex(int index) const override;
    DECL_NODISCARD
    int rightIndex(int index, int count) const override;

    DECL_NODISCARD
    qreal visibleWidth(int viewportWidth) const override;

    void prepareLayout(const QVector< PageItem* >& pageItems, bool rightToLeft,
                       qreal& left, qreal& right, qreal& height) override;

};

struct TwoPagesWithCoverPageLayout : public TwoPagesLayout
{
    DECL_NODISCARD
    LayoutMode layoutMode() const override { return TwoPagesWithCoverPageMode; }

    DECL_NODISCARD
    int currentPage(int page) const override;

    DECL_NODISCARD
    int leftIndex(int index) const override;
    DECL_NODISCARD
    int rightIndex(int index, int count) const override;

};

struct MultiplePagesLayout : public DocumentLayout
{
    DECL_NODISCARD
    LayoutMode layoutMode() const override { return MultiplePagesMode; }

    DECL_NODISCARD
    int currentPage(int page) const override;
    DECL_NODISCARD
    int previousPage(int page) const override;
    DECL_NODISCARD
    int nextPage(int page, int count) const override;

    DECL_NODISCARD
    QPair< int, int > prefetchRange(int page, int count) const override;

    DECL_NODISCARD
    int leftIndex(int index) const override;
    DECL_NODISCARD
    int rightIndex(int index, int count) const override;

    DECL_NODISCARD
    qreal visibleWidth(int viewportWidth) const override;

    void prepareLayout(const QVector< PageItem* >& pageItems, bool rightToLeft,
                       qreal& left, qreal& right, qreal& height) override;

};

} // qpdfview

#endif // DOCUMENTLAYOUT_H
