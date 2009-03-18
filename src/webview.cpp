/*
 * Copyright 2008 Benjamin C. Meyer <ben@meyerhome.net>
 * Copyright 2008 Jason A. Donenfeld <Jason@zx2c4.com>
 * Copyright 2008 Ariya Hidayat <ariya.hidayat@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

/****************************************************************************
**
** Copyright (C) 2007-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "webview.h"

#include "bookmarks.h"
#include "browserapplication.h"
#include "browsermainwindow.h"
#include "cookiejar.h"
#include "downloadmanager.h"
#include "networkaccessmanager.h"
#include "tabwidget.h"

#include <qbuffer.h>
#include <qclipboard.h>
#include <qdesktopservices.h>
#include <qevent.h>
#include <qmenu.h>
#include <qmessagebox.h>
#include <qsettings.h>
#include <qmenubar.h>

#include <qwebframe.h>

#include <qdebug.h>

#include <quiloader.h>

WebPage::WebPage(QObject *parent)
#ifdef HAVE_KDE
    : KWebPage(parent)
#else
    : QWebPage(parent)
#endif
    , m_forceInNewTab(false)
{
    setNetworkAccessManager(BrowserApplication::networkAccessManager());
    connect(this, SIGNAL(unsupportedContent(QNetworkReply *)),
            this, SLOT(handleUnsupportedContent(QNetworkReply *)));
}

BrowserMainWindow *WebPage::mainWindow()
{
    QObject *w = this->parent();
    while (w) {
        if (BrowserMainWindow *mw = qobject_cast<BrowserMainWindow*>(w))
            return mw;
        w = w->parent();
    }
    return BrowserApplication::instance()->mainWindow();
}

TabWidget *WebPage::tabWidget() const
{
    TabWidget *tabWidget = qobject_cast<TabWidget*>(view()->parentWidget()->parentWidget()->parentWidget());
    Q_ASSERT(tabWidget);
    return tabWidget;
}

bool WebPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type)
{
    QString scheme = request.url().scheme();
    if (scheme == QLatin1String("mailto") || scheme == QLatin1String("ftp")) {
        QDesktopServices::openUrl(request.url());
        return false;
    }

    TabWidget::OpenUrlIn openIn = frame ? TabWidget::CurrentTab : TabWidget::NewWindow;
    if (type == QWebPage::NavigationTypeBackOrForward) {
        BrowserApplication::instance()->setEventMouseButtons(qApp->mouseButtons());
        BrowserApplication::instance()->setEventKeyboardModifiers(qApp->keyboardModifiers());
    }

    // A short term hack until QtWebKit can get a reload without cache QAction
    // *FYI* currently type is never NavigationTypeReload
    // See: https://bugs.webkit.org/show_bug.cgi?id=24283
    if (type == QWebPage::NavigationTypeReload
        && (qApp->keyboardModifiers() & Qt::ShiftModifier)) {
        QNetworkRequest newRequest(request);
        newRequest.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                                QNetworkRequest::AlwaysNetwork);
        mainFrame()->load(request);
        return false;
    }
    openIn = TabWidget::modifyWithUserBehavior(openIn);

    // handle the case where we want to do something different then
    // what qwebpage would do
    if ((!frame && openIn == TabWidget::CurrentTab)
        || openIn == TabWidget::NewSelectedTab
        || openIn == TabWidget::NewNotSelectedTab
        || (frame && openIn == TabWidget::NewWindow)) {
        WebView *webView = tabWidget()->getView(openIn,  qobject_cast<WebView*>(this->view()));
        webView->page()->mainFrame()->load(request);
        return false;
    }

    if (frame == mainFrame()) {
        m_loadingUrl = request.url();
        emit loadingUrl(m_loadingUrl);
    }

    #ifdef HAVE_KDE
    return KWebPage::acceptNavigationRequest(frame, request, type);
    #else
    return QWebPage::acceptNavigationRequest(frame, request, type);
    #endif
}

#ifdef HAVE_KDE
KWebPage *WebPage::createWindow(QWebPage::WebWindowType type)
#else
QWebPage *WebPage::createWindow(QWebPage::WebWindowType type)
#endif
{
    Q_UNUSED(type);
    QSettings settings;
    settings.beginGroup(QLatin1String("tabs"));
    TabWidget::OpenUrlIn openIn = TabWidget::NewWindow;
    openIn = TabWidget::OpenUrlIn(settings.value(QLatin1String("openTargetBlankLinksIn"), openIn).toInt());
    openIn = TabWidget::modifyWithUserBehavior(openIn);

    if (m_forceInNewTab) {
        openIn = TabWidget::NewTab;
        m_forceInNewTab = false;
    }
    return tabWidget()->getView(openIn,  qobject_cast<WebView*>(this->view()))->page();
}

#if !defined(QT_NO_UITOOLS)
QObject *WebPage::createPlugin(const QString &classId, const QUrl &url, const QStringList &paramNames, const QStringList &paramValues)
{
    Q_UNUSED(url);
    Q_UNUSED(paramNames);
    Q_UNUSED(paramValues);
    QUiLoader loader;
    return loader.createWidget(classId, view());
}
#endif // !defined(QT_NO_UITOOLS)

void WebPage::handleUnsupportedContent(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::ProtocolUnknownError) {
        QSettings settings;
        settings.beginGroup(QLatin1String("WebView"));
        QStringList externalSchemes;
        externalSchemes = settings.value(QLatin1String("externalSchemes")).toStringList();
        if (externalSchemes.contains(reply->url().scheme()))
            QDesktopServices::openUrl(reply->url());
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        if (reply->header(QNetworkRequest::ContentTypeHeader).isValid())
            BrowserApplication::downloadManager()->handleUnsupportedContent(reply);
        return;
    }

    QFile file(QLatin1String(":/notfound.html"));
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "WebPage::handleUnsupportedContent" << "Unable to open notfound.html";
        return;
    }
    QString title = tr("Error loading page: %1").arg(QString::fromUtf8(reply->url().toEncoded()));
    QString html = QString(QLatin1String(file.readAll()))
                        .arg(title)
                        .arg(reply->errorString())
                        .arg(tr("When connecting to: %1.").arg(reply->url().toString()))
                        .arg(tr("Check the address for errors such as <b>ww</b>.arora-browser.org instead of <b>www</b>.arora-browser.org"))
                        .arg(tr("If the address is correct, try checking the network connection."))
                        .arg(tr("If your computer or network is protected by a firewall or proxy, make sure that the browser is permitted to access the network."));

    QBuffer imageBuffer;
    imageBuffer.open(QBuffer::ReadWrite);
    QIcon icon = view()->style()->standardIcon(QStyle::SP_MessageBoxWarning, 0, view());
    QPixmap pixmap = icon.pixmap(QSize(32, 32));
    if (pixmap.save(&imageBuffer, "PNG")) {
        html.replace(QLatin1String("IMAGE_BINARY_DATA_HERE"),
                     QString(QLatin1String(imageBuffer.buffer().toBase64())));
    }

    QList<QWebFrame*> frames;
    frames.append(mainFrame());
    while (!frames.isEmpty()) {
        QWebFrame *frame = frames.takeFirst();
        if (frame->url() == reply->url()) {
            frame->setHtml(html, reply->url());
            return;
        }
        QList<QWebFrame *> children = frame->childFrames();
        foreach (QWebFrame *frame, children)
            frames.append(frame);
    }
    if (m_loadingUrl == reply->url()) {
        mainFrame()->setHtml(html, reply->url());
    }
}


WebView::WebView(QWidget *parent)
#ifdef HAVE_KDE
    : KWebView(parent)
#else
    : QWebView(parent)
#endif
    , m_progress(0)
    , m_currentZoom(100)
    , m_page(new WebPage(this))
{
    setPage(m_page);
    connect(page(), SIGNAL(statusBarMessage(const QString&)),
            SLOT(setStatusBarText(const QString&)));
    connect(this, SIGNAL(loadProgress(int)),
            this, SLOT(setProgress(int)));
    connect(this, SIGNAL(loadFinished(bool)),
            this, SLOT(loadFinished()));
    connect(page(), SIGNAL(loadingUrl(const QUrl&)),
            this, SIGNAL(urlChanged(const QUrl &)));
    connect(page(), SIGNAL(downloadRequested(const QNetworkRequest &)),
            this, SLOT(downloadRequested(const QNetworkRequest &)));
#if QT_VERSION >= 0x040500
    connect(BrowserApplication::instance(), SIGNAL(zoomTextOnlyChanged(bool)),
            this, SLOT(applyZoom()));
#endif
    page()->setForwardUnsupportedContent(true);
    setAcceptDrops(true);

    // the zoom values (in percent) are chosen to be like in Mozilla Firefox 3
    m_zoomLevels << 30 << 50 << 67 << 80 << 90;
    m_zoomLevels << 100;
    m_zoomLevels << 110 << 120 << 133 << 150 << 170 << 200 << 240 << 300;
}

void WebView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);

    QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());

    if (!r.linkUrl().isEmpty()) {
        menu->addAction(tr("Open in New &Window"), this, SLOT(openLinkInNewWindow()));
        menu->addAction(tr("Open in New &Tab"), this, SLOT(openLinkInNewTab()));
        menu->addSeparator();
        menu->addAction(tr("Save Lin&k"), this, SLOT(downloadLinkToDisk()));
        menu->addAction(tr("&Bookmark This Link"), this, SLOT(bookmarkLink()))->setData(r.linkUrl());
        menu->addSeparator();
        if (!page()->selectedText().isEmpty())
            menu->addAction(pageAction(QWebPage::Copy));
        menu->addAction(tr("&Copy Link Location"), this, SLOT(copyLinkToClipboard()));
        if (page()->settings()->testAttribute(QWebSettings::DeveloperExtrasEnabled))
            menu->addAction(pageAction(QWebPage::InspectElement));
    }

    if (!r.imageUrl().isEmpty()) {
        if (!menu->isEmpty())
            menu->addSeparator();
        menu->addAction(tr("Open Image in New &Window"), this, SLOT(openImageInNewWindow()));
        menu->addAction(tr("Open Image in New &Tab"), this, SLOT(openImageInNewTab()));
        menu->addSeparator();
        menu->addAction(tr("&Save Image"), this, SLOT(downloadImageToDisk()));
        menu->addAction(tr("&Copy Image"), this, SLOT(copyImageToClipboard()));
        menu->addAction(tr("C&opy Image Location"), this, SLOT(copyImageLocationToClipboard()))->setData(r.imageUrl().toString());
    }

#if QT_VERSION >= 0x040500
    if (menu->isEmpty()) {
        delete menu;
        menu = page()->createStandardContextMenu();
    }
#endif

    if (!menu->isEmpty()) {
        if (m_page->mainWindow()->menuBar()->isHidden()) {
            menu->addSeparator();
            menu->addAction(m_page->mainWindow()->showMenuBarAction());
        }

        menu->exec(mapToGlobal(event->pos()));
        delete menu;
        return;
    }
    delete menu;

    #ifdef HAVE_KDE
    KWebView::contextMenuEvent(event);
    #else
    QWebView::contextMenuEvent(event);
    #endif
}

void WebView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15;
        m_currentZoom = m_currentZoom + numSteps * 10;
        applyZoom();
        event->accept();
        return;
    }

    #ifdef HAVE_KDE
    KWebView::wheelEvent(event);
    #else
    QWebView::wheelEvent(event);
    #endif
}

void WebView::resizeEvent(QResizeEvent *event)
{
    int offset = event->size().height() - event->oldSize().height();
    int currentValue = page()->mainFrame()->scrollBarValue(Qt::Vertical);
    setUpdatesEnabled(false);
    page()->mainFrame()->setScrollBarValue(Qt::Vertical, currentValue - offset);
    setUpdatesEnabled(true);

    #ifdef HAVE_KDE
    KWebView::resizeEvent(event);
    #else
    QWebView::resizeEvent(event);
    #endif
}

void WebView::openLinkInNewTab()
{
    m_page->m_forceInNewTab = true;
    pageAction(QWebPage::OpenLinkInNewWindow)->trigger();
}

void WebView::openLinkInNewWindow()
{
    pageAction(QWebPage::OpenLinkInNewWindow)->trigger();
}

void WebView::downloadLinkToDisk()
{
    pageAction(QWebPage::DownloadLinkToDisk)->trigger();
}

void WebView::copyLinkToClipboard()
{
    pageAction(QWebPage::CopyLinkToClipboard)->trigger();
}

void WebView::openImageInNewTab()
{
    m_page->m_forceInNewTab = true;
    pageAction(QWebPage::OpenImageInNewWindow)->trigger();
}

void WebView::openImageInNewWindow()
{
    pageAction(QWebPage::OpenImageInNewWindow)->trigger();
}

void WebView::downloadImageToDisk()
{
    pageAction(QWebPage::DownloadImageToDisk)->trigger();
}

void WebView::copyImageToClipboard()
{
    pageAction(QWebPage::CopyImageToClipboard)->trigger();
}

void WebView::copyImageLocationToClipboard()
{
    if (QAction *action = qobject_cast<QAction*>(sender())) {
        BrowserApplication::clipboard()->setText(action->data().toString());
    }
}

void WebView::bookmarkLink()
{
    if (QAction *action = qobject_cast<QAction*>(sender())) {
        AddBookmarkDialog dialog;
        dialog.setUrl(QString::fromUtf8(action->data().toUrl().toEncoded()));
        dialog.exec();
    }
}

void WebView::setProgress(int progress)
{
    m_progress = progress;
}

int WebView::levelForZoom(int zoom)
{
    int i;

    i = m_zoomLevels.indexOf(zoom);
    if (i >= 0)
        return i;

    for (i = 0 ; i < m_zoomLevels.count(); ++i)
        if (zoom <= m_zoomLevels[i])
            break;

    if (i == m_zoomLevels.count())
        return i - 1;
    if (i == 0)
        return i;

    if (zoom - m_zoomLevels[i-1] > m_zoomLevels[i] - zoom)
        return i;
    else
        return i-1;
}

void WebView::applyZoom()
{
#if QT_VERSION >= 0x040500
    setZoomFactor(qreal(m_currentZoom) / 100.0);
#else
    setTextSizeMultiplier(qreal(m_currentZoom) / 100.0);
#endif
}

void WebView::zoomIn()
{
    int i = levelForZoom(m_currentZoom);

    if (i < m_zoomLevels.count() - 1)
        m_currentZoom = m_zoomLevels[i + 1];
    applyZoom();
}

void WebView::zoomOut()
{
    int i = levelForZoom(m_currentZoom);

    if (i > 0)
        m_currentZoom = m_zoomLevels[i - 1];
    applyZoom();
}

void WebView::resetZoom()
{
    m_currentZoom = 100;
    applyZoom();
}

void WebView::loadFinished()
{
    if (100 != m_progress) {
        qWarning() << "Received finished signal while progress is still:" << progress()
                   << "Url:" << url();
    }
    m_progress = 0;
}

void WebView::loadUrl(const QUrl &url, const QString &title)
{
    if (url.scheme() == QLatin1String("javascript")) {
        QString scriptSource = url.toString().mid(11);
        QVariant result = page()->mainFrame()->evaluateJavaScript(scriptSource);
        if (result.canConvert(QVariant::String)) {
            QString newHtml = result.toString();
            setHtml(newHtml);
        }
        return;
    }
    m_initialUrl = url;
    if (!title.isEmpty())
        emit titleChanged(tr("Loading..."));
    else
        emit titleChanged(title);
    load(url);
}

QString WebView::lastStatusBarText() const
{
    return m_statusBarText;
}

QUrl WebView::url() const
{
    QUrl url = QWebView::url();
    if (!url.isEmpty())
        return url;

    return m_initialUrl;
}

void WebView::mousePressEvent(QMouseEvent *event)
{
    BrowserApplication::instance()->setEventMouseButtons(event->buttons());
    BrowserApplication::instance()->setEventKeyboardModifiers(event->modifiers());

    switch(event->button()) {
    case Qt::XButton1:
        pageAction(WebPage::Back)->trigger();
        break;
    case Qt::XButton2:
        pageAction(WebPage::Forward)->trigger();
        break;
    default:
        #ifdef HAVE_KDE
        KWebView::mousePressEvent(event);
        #else
        QWebView::mousePressEvent(event);
        #endif
        break;
    }
}

void WebView::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void WebView::dragMoveEvent(QDragMoveEvent *event)
{
    if (!event->mimeData()->urls().isEmpty()) {
        event->acceptProposedAction();
    } else {
        QUrl url(event->mimeData()->text());
        if (url.isValid())
            event->acceptProposedAction();
    }
    if (!event->isAccepted()) {
        #ifdef HAVE_KDE
        KWebView::dragMoveEvent(event);
        #else
        QWebView::dragMoveEvent(event);
        #endif
    }
}

void WebView::dropEvent(QDropEvent *event)
{
    #ifdef HAVE_KDE
    KWebView::dropEvent(event);
    #else
    QWebView::dropEvent(event);
    #endif
    if (!event->isAccepted()
        && event->possibleActions() & Qt::CopyAction) {

        QUrl url;
        if (!event->mimeData()->urls().isEmpty())
            url = event->mimeData()->urls().first();
        if (!url.isValid())
            url = event->mimeData()->text();
        if (url.isValid()) {
            loadUrl(url);
            event->acceptProposedAction();
        }
    }
}

void WebView::mouseReleaseEvent(QMouseEvent *event)
{
    #ifdef HAVE_KDE
    KWebView::mouseReleaseEvent(event);
    #else
    QWebView::mouseReleaseEvent(event);
    #endif
    if (!event->isAccepted()
        && (BrowserApplication::instance()->eventMouseButtons() & Qt::MidButton)) {
        QUrl url(QApplication::clipboard()->text(QClipboard::Selection));
        if (!url.isEmpty() && url.isValid() && !url.scheme().isEmpty()) {
            loadUrl(url);
        }
    }
}

void WebView::setStatusBarText(const QString &string)
{
    m_statusBarText = string;
}

void WebView::downloadRequested(const QNetworkRequest &request)
{
    BrowserApplication::downloadManager()->download(request);
}

