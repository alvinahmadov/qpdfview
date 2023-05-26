/*

Copyright 2018 S. Razi Alavizadeh
Copyright 2015 Martin Banky
Copyright 2014-2015, 2018 Adam Reichold

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

#include "fitzmodel.h"

#include <QFile>
#include <qmath.h>
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QMessageBox>
#include <QSettings>

extern "C"
{

#include <mupdf/fitz/stream.h>
#include <mupdf/fitz/color.h>
#include <mupdf/fitz/bidi.h>
#include <mupdf/fitz/output.h>
#include <mupdf/fitz/display-list.h>
#include <mupdf/fitz/document.h>
#include <mupdf/fitz/pool.h>
#include <mupdf/fitz/structured-text.h>

typedef struct pdf_document_s pdf_document;

pdf_document* pdf_specifics(fz_context*, fz_document*);

}

std::string getData(const QVariant& value)
{
	return value.toString().toStdString();
}

template<typename ... Args>
const char* join(std::string& format, Args&& ... args)
{
	const size_t maxLen = 255;
	auto data = new char[maxLen];
	snprintf(data, maxLen, format.data(), std::forward<Args>(args)...);

	return const_cast<const char*>(data);
}

static inline bool operator==(const fz_matrix& a, const fz_matrix& b)
{
    return a.a == b.a && a.b == b.b && a.c == b.c && a.d == b.d && a.e == b.e && a.f == b.f;
}

std::string fontStyleSheet = "body { margin: %spx %spx; font-family: %s!important; font-size: %spt;} pre, code {font-size: %spt;}";

namespace
{

using namespace qpdfview;
using namespace qpdfview::Model;

namespace Defaults
{

	const QString defaultFontStandard = "serif";

	const int fontSize = 8;
	const int monospaceFontSize = 8;

	const int horizontalMargin = 20;
	const int verticalMargin = 20;


} // Defaults

QString removeFilePrefix(const char* uri)
{
    QString url = QString::fromUtf8(uri);

    if(url.startsWith("file://", Qt::CaseInsensitive))
    {
        url = url.mid(7);
    }

    return url;
}

Outline loadOutline(fz_outline* item)
{
    Outline outline;

    for(; item; item = item->next)
    {
        outline.push_back(Section());
        Section& section = outline.back();
        section.title = QString::fromUtf8(item->title);

        if(item->page != -1)
        {
            section.link.page = item->page + 1;
        }
        else if (item->uri != nullptr)
        {
            section.link.urlOrFileName = removeFilePrefix(item->uri);
        }

        if(fz_outline* childItem = item->down)
        {
            section.children = loadOutline(childItem);
        }
    }

    return outline;
}

QSpinBox* addSpinBox(QWidget* parent, int rangeMin, int rangeMax, int value, const QString& suffix = "px", int step = 1)
{
	auto spinBox = new QSpinBox(parent);
	spinBox->setRange(rangeMin, rangeMax);
	spinBox->setSingleStep(step);
	spinBox->setSuffix(suffix);
	spinBox->setValue(value);

	return spinBox;
}

QComboBox* addComboBox(QWidget* parent,
                       const QStringList& text, const QStringList& data,
                       const QString& current)
{
	auto comboBox = new QComboBox(parent);

	for (int index = 0, count = text.count(); index < count; ++index)
	{
		comboBox->addItem(text.at(index), data.at(index));
	}
	comboBox->setCurrentIndex(comboBox->findData(current));
	return comboBox;
}
} // anonymous

namespace qpdfview
{

namespace Model
{

struct FitzPage::DisplayList
{
    fz_matrix matrix;
    fz_display_list* displayList;
    int refCount;
};

DECL_UNUSED
FitzPage::FitzPage(const FitzDocument* parent, fz_page* page) :
    m_parent(parent),
    m_page(page),
    m_boundingRect(fz_bound_page(m_parent->m_context, m_page)),
    m_displayList()
{
}

FitzPage::~FitzPage()
{
    fz_drop_page(m_parent->m_context, m_page);
}

QSizeF FitzPage::size() const
{
    return {m_boundingRect.x1 - m_boundingRect.x0, m_boundingRect.y1 - m_boundingRect.y0};
}

QImage FitzPage::render(qreal horizontalResolution, qreal verticalResolution, Rotation rotation, QRect boundingRect) const
{
    float hRes {static_cast<float>(horizontalResolution)};
    float vRes {static_cast<float>(verticalResolution)};

    fz_matrix matrix = fz_scale(hRes / 72.0f, vRes / 72.0f);

    switch(rotation)
    {
    default:
    case RotateBy0:
        matrix = fz_pre_rotate(matrix, 0.0);
        break;
    case RotateBy90:
        matrix = fz_pre_rotate(matrix, 90.0);
        break;
    case RotateBy180:
        matrix = fz_pre_rotate(matrix, 180.0);
        break;
    case RotateBy270:
        matrix = fz_pre_rotate(matrix, 270.0);
        break;
    }

    const fz_rect rect = fz_transform_rect(m_boundingRect, matrix);
    const fz_irect irect = fz_round_rect(rect);

    fz_display_list* display_list;
    fz_context* context;

    {
        QMutexLocker mutexLocker(&m_parent->m_mutex);

        if(m_displayList != nullptr && m_displayList->matrix == matrix)
        {
            display_list = m_displayList->displayList;
            ++m_displayList->refCount;
        }
        else
        {
            display_list = fz_new_display_list(m_parent->m_context, rect);

            fz_device* device = fz_new_list_device(m_parent->m_context, display_list);
            fz_run_page(m_parent->m_context, m_page, device, matrix, nullptr);
            fz_close_device(m_parent->m_context, device);
            fz_drop_device(m_parent->m_context, device);

            if(m_displayList == nullptr)
            {
                m_displayList = new DisplayList;
                memcpy(&m_displayList->matrix, &matrix, sizeof(fz_matrix));
                m_displayList->displayList = display_list;
                m_displayList->refCount = 1;
            }
        }

        context = fz_clone_context(m_parent->m_context);
    }

    fz_matrix tileMatrix = fz_translate(-rect.x0, -rect.y0);
    fz_rect tileRect = fz_infinite_rect;

    int tileWidth = irect.x1 - irect.x0;
    int tileHeight = irect.y1 - irect.y0;

    if(!boundingRect.isNull())
    {
        auto x = static_cast<float>(boundingRect.x()),
             y = static_cast<float>(boundingRect.y());
        auto r = static_cast<float>(boundingRect.right()),
             b = static_cast<float>(boundingRect.bottom());
    	// NoBreak
	    tileMatrix = fz_pre_translate(tileMatrix, -x, -y);

        tileRect.x0 = x;
        tileRect.y0 = y;

        tileRect.x1 = r;
        tileRect.y1 = b;

        tileWidth = boundingRect.width();
        tileHeight = boundingRect.height();
    }

    QImage image(tileWidth, tileHeight, QImage::Format_RGB32);
    image.fill(m_parent->m_paperColor);

    auto pixmap = fz_new_pixmap_with_data(context, fz_device_bgr(context), image.width(), image.height(), nullptr, 1, image.bytesPerLine(), image.bits());

    fz_device* device = fz_new_draw_device(context, tileMatrix, pixmap);
    fz_run_display_list(context, display_list, device, fz_identity, tileRect, nullptr);
    fz_close_device(context, device);
    fz_drop_device(context, device);

    fz_drop_pixmap(context, pixmap);
    fz_drop_context(context);

    {
        QMutexLocker mutexLocker(&m_parent->m_mutex);

        if(m_displayList != nullptr && m_displayList->displayList == display_list)
        {
            if(--m_displayList->refCount == 0)
            {
                fz_drop_display_list(m_parent->m_context, display_list);

                delete m_displayList;
                m_displayList = nullptr;
            }
        }
    }

    return image;
}

QList< Link* > FitzPage::links() const
{
    QMutexLocker mutexLocker(&m_parent->m_mutex);

    QList< Link* > links;

    const qreal width = qAbs(m_boundingRect.x1 - m_boundingRect.x0);
    const qreal height = qAbs(m_boundingRect.y1 - m_boundingRect.y0);

    fz_link* first_link = fz_load_links(m_parent->m_context, m_page);

    for(fz_link* link = first_link; link != nullptr; link = link->next)
    {
        const QRectF boundary = QRectF(link->rect.x0 / width, link->rect.y0 / height, (link->rect.x1 - link->rect.x0) / width, (link->rect.y1 - link->rect.y0) / height).normalized();

        if (link->uri != nullptr)
        {
            if (fz_is_external_link(m_parent->m_context, link->uri) == 0)
            {
                float left;
                float top;
                const fz_location location = fz_resolve_link(m_parent->m_context, m_parent->m_document, link->uri, &left, &top);
                const int page = location.page;

                if (page != -1)
                {
                    links.append(new Link(boundary, page + 1, left / width, top / height));
                }
            }
            else
            {
                links.append(new Link(boundary, removeFilePrefix(link->uri)));
            }
        }
    }

    fz_drop_link(m_parent->m_context, first_link);

    return links;
}

QString FitzPage::text(const QRectF &rect) const
{
    QMutexLocker mutexLocker(&m_parent->m_mutex);


    fz_stext_page* textPage = fz_new_stext_page(m_parent->m_context, m_boundingRect);
    fz_device* device = fz_new_stext_device(m_parent->m_context, textPage, nullptr);
    fz_run_page(m_parent->m_context, m_page, device, fz_identity, nullptr);
    fz_close_device(m_parent->m_context, device);
    fz_drop_device(m_parent->m_context, device);

    fz_point topLeft;
    topLeft.x = static_cast<float>(rect.x());
    topLeft.y = static_cast<float>(rect.y());

    fz_point bottomRight;
    bottomRight.x = static_cast<float>(rect.right());
    bottomRight.y = static_cast<float>(rect.bottom());

    char* selection = fz_copy_selection(m_parent->m_context, textPage, topLeft, bottomRight, 0);
    QString text = QString::fromUtf8(selection);
    ::free(selection);

    fz_drop_stext_page(m_parent->m_context, textPage);

    return text;
}

QList<QRectF> FitzPage::search(const QString& text, bool matchCase, bool wholeWords) const
{
    Q_UNUSED(matchCase)
    Q_UNUSED(wholeWords)

    QMutexLocker mutexLocker(&m_parent->m_mutex);

    fz_rect rectangle = fz_bound_page(m_parent->m_context, m_page);

    fz_stext_page* textPage = fz_new_stext_page(m_parent->m_context, rectangle);
    fz_device* device = fz_new_stext_device(m_parent->m_context, textPage, nullptr);
    fz_run_page(m_parent->m_context, m_page, device, fz_identity, nullptr);
    fz_close_device(m_parent->m_context, device);
    fz_drop_device(m_parent->m_context, device);

    const QByteArray needle = text.toUtf8();

    QVector< fz_quad > hits(32);
    int numberOfHits = fz_search_stext_page(m_parent->m_context, textPage, needle.constData(), reinterpret_cast<fz_quad*>(hits.data()), hits.size());

    while(numberOfHits == hits.size())
    {
        hits.resize(2 * hits.size());
        numberOfHits = fz_search_stext_page(m_parent->m_context, textPage, needle.constData(), reinterpret_cast<fz_quad*>(hits.data()), hits.size());
    }

    hits.resize(numberOfHits);

    fz_drop_stext_page(m_parent->m_context, textPage);

    QList< QRectF > results;
    results.reserve(hits.size());

    foreach(fz_quad rect, hits)
    {
        results.append(QRectF(rect.ul.x, rect.ul.y, rect.ur.x - rect.ul.x, rect.ll.y - rect.ul.y));
    }

    return results;
}

DECL_UNUSED
FitzDocument::FitzDocument(fz_context* context, fz_document* document) :
    m_mutex(),
    m_context(context),
    m_document(document),
    m_paperColor(Qt::white)
{
}

FitzDocument::~FitzDocument()
{
    fz_drop_document(m_context, m_document);
    fz_drop_context(m_context);
}

int FitzDocument::numberOfPages() const
{
    QMutexLocker mutexLocker(&m_mutex);

    return fz_count_pages(m_context, m_document);
}

Page* FitzDocument::page(int index) const
{
    QMutexLocker mutexLocker(&m_mutex);

    if(fz_page* page = fz_load_page(m_context, m_document, index))
    {
        return new FitzPage(this, page);
    }

    return nullptr;
}

bool FitzDocument::canBePrintedUsingCUPS() const
{
    QMutexLocker mutexLocker(&m_mutex);

    return pdf_specifics(m_context, m_document) != nullptr;
}

void FitzDocument::setPaperColor(const QColor& paperColor)
{
    m_paperColor = paperColor;
}

Outline FitzDocument::outline() const
{
    Outline outline;

    QMutexLocker mutexLocker(&m_mutex);

    if(fz_outline* rootItem = fz_load_outline(m_context, m_document))
    {
        outline = loadOutline(rootItem);

        fz_drop_outline(m_context, rootItem);
    }

    return outline;
}

} // Model

FitzSettingsWidget::FitzSettingsWidget(QSettings* settings, QWidget* parent) : SettingsWidget(parent),
    m_settings(settings)
{
	m_layout = new QFormLayout(this);

	m_defaultFontComboBox = addComboBox(this, QStringList() << "Sans Serif" << "Serif",
			QStringList() << "sans-serif" << "serif",
			m_settings->value("defaultFontStandard", Defaults::defaultFontStandard).toString());

	m_layout->addRow(tr("Standard font:"), m_defaultFontComboBox);
	// Default font size

	m_defaultFontSizeSpinBox = addSpinBox(this, 2, 100,
	                                      m_settings->value("defaultFontSize", Defaults::fontSize).toInt(), "pt");

	m_layout->addRow(tr("Default font size:"), m_defaultFontSizeSpinBox);

	// Monospace font size

	m_monospaceFontSizeSpinBox = addSpinBox(this, 2, 100,
	                                        m_settings->value("monospaceFontSize", Defaults::monospaceFontSize).toInt(), "pt");

	m_layout->addRow(tr("Monospace font size:"), m_monospaceFontSizeSpinBox);

	// Horizontal margin

	m_horizontalMarginSpinBox = addSpinBox(this, 8, 40,
	                                       m_settings->value("horizontalMargin", Defaults::horizontalMargin).toInt());

	m_layout->addRow(tr("Horizontal margin:"), m_horizontalMarginSpinBox);
	// Vertical margin

	m_verticalMarginSpinBox = addSpinBox(this, 8, 40,
	                                     m_settings->value("verticalMargin", Defaults::verticalMargin).toInt());

	m_layout->addRow(tr("Vertical margin:"), m_verticalMarginSpinBox);
}

void FitzSettingsWidget::accept()
{
	m_settings->setValue("defaultFontStandard", m_defaultFontComboBox->currentData().toString());
	m_settings->setValue("defaultFontSize", m_defaultFontSizeSpinBox->value());
	m_settings->setValue("monospaceFontSize", m_monospaceFontSizeSpinBox->value());
	m_settings->setValue("horizontalMargin", m_horizontalMarginSpinBox->value());
	m_settings->setValue("verticalMargin", m_verticalMarginSpinBox->value());
}

void FitzSettingsWidget::reset()
{
	m_defaultFontComboBox->setCurrentIndex(m_defaultFontComboBox->findData(Defaults::defaultFontStandard));
	m_defaultFontSizeSpinBox->setValue(Defaults::fontSize);
	m_monospaceFontSizeSpinBox->setValue(Defaults::monospaceFontSize);
	m_horizontalMarginSpinBox->setValue(Defaults::horizontalMargin);
	m_verticalMarginSpinBox->setValue(Defaults::verticalMargin);
}

FitzPlugin::FitzPlugin(QObject* parent) : QObject(parent),
      m_locksContext()
{
    setObjectName("FitzPlugin");

	m_settings = new QSettings("qpdfview", "epub-plugin", this);

    m_locksContext.user = this;
    m_locksContext.lock = FitzPlugin::lock;
    m_locksContext.unlock = FitzPlugin::unlock;

    m_context = fz_new_context(nullptr, &m_locksContext, FZ_STORE_DEFAULT);

    fz_register_document_handlers(m_context);
}

FitzPlugin::~FitzPlugin()
{
    fz_drop_context(m_context);
}

Model::Document* FitzPlugin::loadDocument(const QString& filePath) const
{
    fz_context* context = fz_clone_context(m_context);

    if(context == nullptr)
    {
        return nullptr;
    }

#ifdef _MSC_VER

    fz_document* document = fz_open_document(context, filePath.toUtf8());

#else

    fz_document* document = fz_open_document(context, QFile::encodeName(filePath));

#endif // _MSC_VER

    if(document == nullptr)
    {
        fz_drop_context(context);

        return nullptr;
    }

    auto defaultFontStandard = getData(m_settings->value("defaultFontStandard", Defaults::defaultFontStandard));

	auto horizontalMargin = getData(m_settings->value("horizontalMargin", Defaults::horizontalMargin));
	auto verticalMargin = getData(m_settings->value("verticalMargin", Defaults::verticalMargin));

	auto defaultFontSize = getData(m_settings->value("defaultFontSize", Defaults::fontSize));
	auto monoFontSize = getData(m_settings->value("monospaceFontSize", Defaults::monospaceFontSize));

	auto data = join(fontStyleSheet,
	                 verticalMargin.data(), horizontalMargin.data(),
	                 defaultFontStandard.data(),
	                 defaultFontSize.data(), monoFontSize.data());

	fz_set_user_css(context, data);

    return new Model::FitzDocument(context, document);
}

SettingsWidget* FitzPlugin::createSettingsWidget(QWidget* parent) const
{
	return new FitzSettingsWidget(m_settings, parent);
}

void FitzPlugin::lock(void* user, int lock)
{
    static_cast< FitzPlugin* >(user)->m_mutex[lock].lock();
}

void FitzPlugin::unlock(void* user, int lock)
{
    static_cast< FitzPlugin* >(user)->m_mutex[lock].unlock();
}

} // qpdfview

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)

Q_EXPORT_PLUGIN2(qpdfview_fitz, qpdfview::FitzPlugin)

#endif // QT_VERSION
