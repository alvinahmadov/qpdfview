/*

Copyright 2014 S. Razi Alavizadeh
Copyright 2012-2014 Adam Reichold

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

#include "bookmarkmenu.h"

#include "miscellaneous.h"

#include <QFileInfo>

namespace qpdfview
{

BookmarkMenu::BookmarkMenu(const QFileInfo& fileInfo, QWidget* parent) : QMenu(parent)
{
    QAction* const action = menuAction();

    action->setText(fileInfo.completeBaseName());
    action->setToolTip(fileInfo.absoluteFilePath());

    action->setData(fileInfo.absoluteFilePath());

    m_openAction = addAction(tr("&Open"));
    setVisibleIcon(m_openAction, loadIconWithFallback(QLatin1String("document-open")));
    connect(m_openAction, SIGNAL(triggered()), SLOT(onOpenTriggered()));

    m_openInNewTabAction = addAction(tr("Open in new &tab"));
    setVisibleIcon(m_openInNewTabAction, loadIconWithFallback(QLatin1String("tab-new")));
    connect(m_openInNewTabAction, SIGNAL(triggered()), SLOT(onOpenInNewTabTriggered()));

    m_jumpToPageActionGroup = new QActionGroup(this);
    connect(m_jumpToPageActionGroup, SIGNAL(triggered(QAction*)), SLOT(onJumpToPageTriggered(QAction*)));

    m_separatorAction = addSeparator();

    m_removeBookmarkAction = addAction(tr("&Remove bookmark"));
    connect(m_removeBookmarkAction, SIGNAL(triggered()), SLOT(onRemoveBookmarkTriggered()));
}

void BookmarkMenu::addJumpToPageAction(int page, const QString& label)
{
    QAction* before = m_separatorAction;

    foreach(QAction* action, m_jumpToPageActionGroup->actions())
    {
        if(action->data().toInt() == page)
        {
            action->setText(label);

            return;
        }
        else if(action->data().toInt() > page)
        {
            before = action;

            break;
        }
    }

    auto action = new QAction(label, this);
    setVisibleIcon(action, loadIconWithFallback(QLatin1String("go-jump")));
    action->setData(page);

    insertAction(before, action);
    m_jumpToPageActionGroup->addAction(action);
}

DECL_UNUSED
void BookmarkMenu::removeJumpToPageAction(int page)
{
    foreach(QAction* action, m_jumpToPageActionGroup->actions())
    {
        if(action->data().toInt() == page)
        {
            delete action;

            break;
        }
    }
}

void BookmarkMenu::onOpenTriggered()
{
    emit openTriggered(absoluteFilePath());
}

void BookmarkMenu::onOpenInNewTabTriggered()
{
    emit openInNewTabTriggered(absoluteFilePath());
}

void BookmarkMenu::onJumpToPageTriggered(QAction* action)
{
    emit jumpToPageTriggered(absoluteFilePath(), action->data().toInt());
}

void BookmarkMenu::onRemoveBookmarkTriggered()
{
    emit removeBookmarkTriggered(absoluteFilePath());
}

} // qpdfview
