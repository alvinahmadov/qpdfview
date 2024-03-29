/*

Copyright 2018 S. Razi Alavizadeh
Copyright 2014-2015 Adam Reichold

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

#include "bookmarkmodel.h"

#include <QApplication>

namespace qpdfview
{
DECL_UNUSED
static inline bool operator==(int page, const BookmarkItem& item) { return item.page == page; }
DECL_UNUSED
static inline bool operator!=(int page, const BookmarkItem& item) { return item.page != page; }

QHash< QString, BookmarkModel* > BookmarkModel::s_instances;

BookmarkModel* BookmarkModel::fromPath(const QString& path, bool create)
{
    BookmarkModel* model = s_instances.value(path, 0);

    if(create && model == nullptr)
    {
        model = new BookmarkModel(qApp);

        s_instances.insert(path, model);
    }

    return model;
}

QList< QString > BookmarkModel::paths()
{
    return s_instances.keys();
}

void BookmarkModel::removePath(const QString& path)
{
    delete s_instances.take(path);
}

void BookmarkModel::removeAllPaths()
{
    qDeleteAll(s_instances);
    s_instances.clear();
}

void BookmarkModel::addBookmark(const BookmarkItem& bookmark)
{
    const QVector< BookmarkItem >::iterator at = std::lower_bound(m_bookmarks.begin(), m_bookmarks.end(), bookmark);
    const int row = m_bookmarks.indexOf(*at);

    if(at != m_bookmarks.end() && at->page == bookmark.page)
    {
        *at = bookmark;

        emit dataChanged(createIndex(row, 0), createIndex(row, 1));
    }
    else
    {
        beginInsertRows(QModelIndex(), row, row);

        m_bookmarks.insert(at, bookmark);

        endInsertRows();
    }
}

void BookmarkModel::removeBookmark(const BookmarkItem& bookmark)
{
    QVector< BookmarkItem >::iterator at = std::find(m_bookmarks.begin(), m_bookmarks.end(), bookmark);
    const int row = m_bookmarks.indexOf(*at);

    if(at != m_bookmarks.end())
    {
        beginRemoveRows(QModelIndex(), row, row);

        m_bookmarks.erase(at);

        endRemoveRows();
    }
}

void BookmarkModel::findBookmark(BookmarkItem& bookmark) const
{
    const QVector< BookmarkItem >::const_iterator at = std::find(m_bookmarks.cbegin(), m_bookmarks.cend(), bookmark);

    if(at != m_bookmarks.cend())
    {
        bookmark = *at;
    }
}

Qt::ItemFlags BookmarkModel::flags(const QModelIndex&) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int BookmarkModel::columnCount(const QModelIndex&) const
{
    return 2;
}

int BookmarkModel::rowCount(const QModelIndex& parent) const
{
    return !parent.isValid() ? m_bookmarks.count() : 0;
}

QVariant BookmarkModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid() || index.row() >= m_bookmarks.count())
    {
        return {};
    }

    const BookmarkItem& bookmark = m_bookmarks.at(index.row());

    switch(role)
    {
    default:
        return {};
    case PageRole:
        return bookmark.page;
    case LabelRole:
    case Qt::DisplayRole:
        return index.column() == 0 ? bookmark.label : QString::number(bookmark.page);
    case CommentRole:
        return bookmark.comment;
    case Qt::ToolTipRole:
        return QString("<p>%1</p>").arg(bookmark.comment);
    case ModifiedRole:
        return bookmark.modified;
    case Qt::TextAlignmentRole:
        return index.column() == 0 ? Qt::AlignLeft : Qt::AlignRight;
    }
}

BookmarkModel::BookmarkModel(QObject* parent) : QAbstractListModel(parent),
    m_bookmarks()
{
}

} // qpdfview
