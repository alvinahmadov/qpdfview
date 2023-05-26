/*

Copyright 2015 Adam Reichold

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

#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QCoreApplication>

#include "model.h"

namespace qpdfview
{

class ImagePlugin;

namespace Model
{
    class ImagePage final : public Page
    {
        friend class ImageDocument;

    public:
        DECL_NODISCARD
        QSizeF size() const final;

        DECL_NODISCARD
        QImage render(qreal horizontalResolution, qreal verticalResolution, Rotation rotation, QRect boundingRect) const final;

    private:
        Q_DISABLE_COPY(ImagePage)

        explicit ImagePage(QImage image);

        QImage m_image;

    };

    class ImageDocument final : public Document
    {
        Q_DECLARE_TR_FUNCTIONS(Model::ImageDocument)

        friend class qpdfview::ImagePlugin;

    public:
        DECL_NODISCARD
        int numberOfPages() const final;

        DECL_NODISCARD
        Page* page(int index) const final;

        DECL_NODISCARD
        QStringList saveFilter() const final;

        DECL_NODISCARD
        bool canSave() const final;
        DECL_NODISCARD
        bool save(const QString& filePath, bool withChanges) const final;

        DECL_NODISCARD
        Properties properties() const final;

    private:
        Q_DISABLE_COPY(ImageDocument)

        explicit ImageDocument(QImage image);

        QImage m_image;

    };
}

class ImagePlugin final : public QObject, Plugin
{
    Q_OBJECT
    Q_INTERFACES(qpdfview::Plugin)

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)

    Q_PLUGIN_METADATA(IID "local.qpdfview.Plugin")

#endif // QT_VERSION

public:
    explicit ImagePlugin(QObject* parent = nullptr);

    DECL_NODISCARD
    Model::Document* loadDocument(const QString& filePath) const final;

private:
    Q_DISABLE_COPY(ImagePlugin)

};

} // qpdfview

#endif // IMAGEMODEL_H
