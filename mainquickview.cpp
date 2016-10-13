#include "mainquickview.h"

MainQuickView::MainQuickView(QWindow *parent)
    : QQuickView(parent)
{
    init();
}

void MainQuickView::setQmlVariantProperty(const QString &name, const QVariant &value)
{
    setQmlPrivateProperty(name, value);
}

void MainQuickView::setQmlObjectProperty(const QString &name, QObject *value)
{
    setQmlPrivateProperty(name, value);
}

void MainQuickView::init()
{
    setResizeMode(SizeRootObjectToView);
}
