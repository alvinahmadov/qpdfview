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

#ifndef PDFMODEL_H
#define PDFMODEL_H

#include <QCoreApplication>
#include <QMutex>
#include <QScopedPointer>

class QCheckBox;
class QComboBox;
class QFormLayout;
class QSettings;

namespace Poppler
{
class Annotation;
class Document;
class FormField;
class Page;
}

#include "model.h"

namespace qpdfview
{

class PdfPlugin;

namespace Model
{
    class PdfAnnotation : public Annotation
    {
        Q_OBJECT

        friend class PdfPage;

    public:
        ~PdfAnnotation() override;

        QRectF boundary() const override;
        QString contents() const override;

        QWidget* createWidget() override;

    private:
        Q_DISABLE_COPY(PdfAnnotation)

        PdfAnnotation(QMutex* mutex, Poppler::Annotation* annotation);

        mutable QMutex* m_mutex;
        Poppler::Annotation* m_annotation;

    };

    class PdfFormField : public FormField
    {
        Q_OBJECT

        friend class PdfPage;

    public:
        ~PdfFormField() override;

        QRectF boundary() const override;
        QString name() const override;

        QWidget* createWidget() override;

    private:
        Q_DISABLE_COPY(PdfFormField)

        PdfFormField(QMutex* mutex, Poppler::FormField* formField);

        mutable QMutex* m_mutex;
        Poppler::FormField* m_formField;

    };

    class PdfPage final : public Page
    {
        Q_DECLARE_TR_FUNCTIONS(Model::PdfPage)

        friend class PdfDocument;

    public:
        ~PdfPage() final;

        QSizeF size() const final;

        QImage render(qreal horizontalResolution, qreal verticalResolution, Rotation rotation, QRect boundingRect) const final;

        QString label() const final;

        QList< Link* > links() const final;

        QString text(const QRectF& rect) const final;
        QString cachedText(const QRectF& rect) const final;

        QList< QRectF > search(const QString& text, bool matchCase, bool wholeWords) const final;

        QList< Annotation* > annotations() const final;

        bool canAddAndRemoveAnnotations() const final;
        Annotation* addTextAnnotation(const QRectF& boundary, const QColor& color) final;
        Annotation* addHighlightAnnotation(const QRectF& boundary, const QColor& color) final;
        void removeAnnotation(Annotation* annotation) final;

        QList< FormField* > formFields() const final;

    private:
        Q_DISABLE_COPY(PdfPage)

        PdfPage(QMutex* mutex, Poppler::Page* page);

        mutable QMutex* m_mutex;
        Poppler::Page* m_page;

    };

    class PdfDocument final : public Document
    {
        Q_DECLARE_TR_FUNCTIONS(Model::PdfDocument)

        friend class qpdfview::PdfPlugin;

    public:
        ~PdfDocument() final;

        DECL_NODISCARD
        int numberOfPages() const final;

        DECL_NODISCARD
        Page* page(int index) const final;

        DECL_NODISCARD
        bool isLocked() const final;
        DECL_NODISCARD
        bool unlock(const QString& password) final;

        DECL_NODISCARD
        QStringList saveFilter() const final;

        DECL_NODISCARD
        bool canSave() const final;
        DECL_NODISCARD
        bool save(const QString& filePath, bool withChanges) const final;

        DECL_NODISCARD
        bool canBePrintedUsingCUPS() const final;

        void setPaperColor(const QColor& paperColor) final;

        DECL_NODISCARD
        Outline outline() const final;
        DECL_NODISCARD
        Properties properties() const final;

        DECL_NODISCARD
        QAbstractItemModel* fonts() const final;

        DECL_NODISCARD
        bool wantsContinuousMode() const final;
        DECL_NODISCARD
        bool wantsSinglePageMode() const final;
        DECL_NODISCARD
        bool wantsTwoPagesMode() const final;
        DECL_NODISCARD
        bool wantsTwoPagesWithCoverPageMode() const final;
        DECL_NODISCARD
        bool wantsRightToLeftMode() const final;

    private:
        Q_DISABLE_COPY(PdfDocument)

        explicit PdfDocument(Poppler::Document* document);

        mutable QMutex m_mutex;
        Poppler::Document* m_document;

    };
}

class PdfSettingsWidget final : public SettingsWidget
{
    Q_OBJECT

public:
    explicit PdfSettingsWidget(QSettings* settings, QWidget* parent = nullptr);

    void accept() final;
    void reset() final;

private:
    Q_DISABLE_COPY(PdfSettingsWidget)

    QSettings* m_settings;

    QFormLayout* m_layout;

    QCheckBox* m_antialiasingCheckBox;
    QCheckBox* m_textAntialiasingCheckBox;

#ifdef HAS_POPPLER_18

    QComboBox* m_textHintingComboBox;

#else

    QCheckBox* m_textHintingCheckBox;

#endif // HAS_POPPLER_18

#ifdef HAS_POPPLER_35

    QCheckBox* m_ignorePaperColorCheckBox;

#endif // HAS_POPPLER_35

#ifdef HAS_POPPLER_22

    QCheckBox* m_overprintPreviewCheckBox;

#endif // HAS_POPPLER_22

#ifdef HAS_POPPLER_24

    QComboBox* m_thinLineModeComboBox;

#endif // HAS_POPPLER_24

    QComboBox* m_backendComboBox;

};

class PdfPlugin final : public QObject, Plugin
{
    Q_OBJECT
    Q_INTERFACES(qpdfview::Plugin)

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)

    Q_PLUGIN_METADATA(IID "local.qpdfview.Plugin")

#endif // QT_VERSION

public:
    explicit PdfPlugin(QObject* parent = nullptr);

    DECL_NODISCARD
    Model::Document* loadDocument(const QString& filePath) const final;

    DECL_NODISCARD
    SettingsWidget* createSettingsWidget(QWidget* parent) const final;

private:
    Q_DISABLE_COPY(PdfPlugin)

    QSettings* m_settings;

};

} // qpdfview

#endif // PDFMODEL_H
