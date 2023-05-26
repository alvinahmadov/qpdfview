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

#ifndef DOCUMENTMODEL_H
#define DOCUMENTMODEL_H

#include <QList>
#include <QPainterPath>
#include <QWidget>
#include <QtPlugin>
#include <QWidget>
#include <QVector>

class QAbstractItemModel;
class QColor;
class QImage;
class QPrinter;
class QSizeF;

#include "global.h"

namespace qpdfview
{

namespace Model
{
    struct Link
    {
        QPainterPath boundary;

        int page;
        qreal left;
        qreal top;

        QString urlOrFileName;

        Link() : boundary(), page(-1), left(qQNaN()), top(qQNaN()), urlOrFileName() {}

        Link(const QPainterPath& boundary, int page, qreal left = qQNaN(), qreal top = qQNaN()) : boundary(boundary), page(page), left(left), top(top), urlOrFileName() {}
        Link(const QRectF& boundingRect, int page, qreal left = qQNaN(), qreal top = qQNaN()) : boundary(), page(page), left(left), top(top), urlOrFileName() { boundary.addRect(boundingRect); }

        Link(const QPainterPath& boundary, QString url) : boundary(boundary), page(-1), left(qQNaN()), top(qQNaN()), urlOrFileName(std::move(url)) {}
        Link(const QRectF& boundingRect, QString url) : boundary(), page(-1), left(qQNaN()), top(qQNaN()), urlOrFileName(std::move(url)) { boundary.addRect(boundingRect); }

        Link(const QPainterPath& boundary, QString fileName, int page) : boundary(boundary), page(page), left(qQNaN()), top(qQNaN()), urlOrFileName(std::move(fileName)) {}
        Link(const QRectF& boundingRect, QString fileName, int page) : boundary(), page(page), left(qQNaN()), top(qQNaN()), urlOrFileName(std::move(fileName)) { boundary.addRect(boundingRect); }

    };

    struct Section;

    typedef QVector< Section > Outline;

    struct Section
    {
        QString title;
        Link link;

        Outline children;

    };

    typedef QVector< QPair< QString, QString > > Properties;

    class Annotation : public QObject
    {
        Q_OBJECT

    public:
        explicit Annotation(QObject* parent = nullptr) : QObject(parent) {}
        ~Annotation() override = default;

        DECL_NODISCARD
        virtual QRectF boundary() const = 0;
        DECL_NODISCARD
        virtual QString contents() const = 0;

        virtual QWidget* createWidget() = 0;

    signals:
        void wasModified();
    };

    class FormField : public QObject
    {
        Q_OBJECT

    public:
        explicit FormField(QObject* parent = nullptr) : QObject(parent) {}
        ~FormField() override = default;

        DECL_NODISCARD
        virtual QRectF boundary() const = 0;
        DECL_NODISCARD
        virtual QString name() const = 0;

        virtual QWidget* createWidget() = 0;

    signals:
        void wasModified();
    };

    class Page
    {
    public:
        virtual ~Page() = default;

        DECL_NODISCARD
        virtual QSizeF size() const = 0;

        DECL_NODISCARD
        virtual QImage render(qreal horizontalResolution = 72.0, qreal verticalResolution = 72.0, Rotation rotation = RotateBy0, QRect boundingRect = QRect()) const = 0;

        DECL_NODISCARD
        virtual QString label() const { return {}; }

        DECL_NODISCARD
        virtual QList< Link* > links() const { return {}; }

        DECL_NODISCARD
        virtual QString text(const QRectF& rect) const { Q_UNUSED(rect) return {}; }

        DECL_NODISCARD
        virtual QString cachedText(const QRectF& rect) const { return text(rect); }

        DECL_NODISCARD
        virtual QList< QRectF > search(const QString& text, bool matchCase, bool wholeWords) const
        { Q_UNUSED(text) Q_UNUSED(matchCase) Q_UNUSED(wholeWords) return {}; }

        DECL_NODISCARD
        virtual QList< Annotation* > annotations() const { return {}; }

        DECL_NODISCARD
        virtual bool canAddAndRemoveAnnotations() const { return false; }
        virtual Annotation* addTextAnnotation(const QRectF& boundary, const QColor& color) { Q_UNUSED(boundary) Q_UNUSED(color) return nullptr; }
        virtual Annotation* addHighlightAnnotation(const QRectF& boundary, const QColor& color) { Q_UNUSED(boundary) Q_UNUSED(color) return nullptr; }
        virtual void removeAnnotation(Annotation* annotation) { Q_UNUSED(annotation) }

        DECL_NODISCARD
        virtual QList< FormField* > formFields() const { return {}; }
    };

    class Document
    {
    public:
        virtual ~Document() = default;

        DECL_NODISCARD
        virtual int numberOfPages() const = 0;

        DECL_NODISCARD
        virtual Page* page(int index) const = 0;

        DECL_NODISCARD
        virtual bool isLocked() const { return false; }
        virtual bool unlock(const QString& password) { Q_UNUSED(password) return false; }

        DECL_NODISCARD
        virtual QStringList saveFilter() const { return {}; }

        DECL_NODISCARD
        virtual bool canSave() const { return false; }
        DECL_NODISCARD
        virtual bool save(const QString& filePath, bool withChanges) const { Q_UNUSED(filePath) Q_UNUSED(withChanges) return false; }

        DECL_NODISCARD
        virtual bool canBePrintedUsingCUPS() const { return false; }

        virtual void setPaperColor(const QColor& paperColor) { Q_UNUSED(paperColor) }

        enum
        {
            PageRole = Qt::UserRole + 1,
            LeftRole,
            TopRole,
            FileNameRole,
            ExpansionRole
        };

        DECL_NODISCARD
        virtual Outline outline() const { return {}; }
        DECL_NODISCARD
        virtual Properties properties() const { return {}; }

        DECL_NODISCARD
        virtual QAbstractItemModel* fonts() const { return nullptr; }

        DECL_NODISCARD
        virtual bool wantsContinuousMode() const { return false; }
        DECL_NODISCARD
        virtual bool wantsSinglePageMode() const { return false; }
        DECL_NODISCARD
        virtual bool wantsTwoPagesMode() const { return false; }
        DECL_NODISCARD
        virtual bool wantsTwoPagesWithCoverPageMode() const { return false; }
        DECL_NODISCARD
        virtual bool wantsRightToLeftMode() const { return false; }
    };
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget* parent = nullptr) : QWidget(parent) {}

    virtual void accept() = 0;
    virtual void reset() = 0;
};

class Plugin
{
public:
    virtual ~Plugin() = default;

    DECL_NODISCARD
    virtual Model::Document* loadDocument(const QString& filePath) const = 0;

    virtual SettingsWidget* createSettingsWidget(QWidget* parent = nullptr) const
    {
        Q_UNUSED(parent)
        return nullptr;
    }
};

} // qpdfview

Q_DECLARE_INTERFACE(qpdfview::Plugin, "local.qpdfview.Plugin")

#endif // DOCUMENTMODEL_H
