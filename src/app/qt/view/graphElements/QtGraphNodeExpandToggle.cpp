#include "qt/view/graphElements/QtGraphNodeExpandToggle.h"

#include "utility/logging/logging.h"
#include "utility/messaging/type/MessageGraphNodeExpand.h"

#include "qt/graphics/QtRoundedRectItem.h"
#include "qt/utility/QtDeviceScaledPixmap.h"

QtGraphNodeExpandToggle::QtGraphNodeExpandToggle(bool expanded, int invisibleSubNodeCount)
	: m_allVisible(invisibleSubNodeCount == 0)
{
	if (!expanded && !invisibleSubNodeCount)
	{
		LOG_ERROR("ExpandToggle shouldn't be visible");
		return;
	}

	const int iconHeight = 4;
	m_icon = new QGraphicsPixmapItem(this);

	QtDeviceScaledPixmap pixmap("data/gui/graph_view/images/arrow.png");
	pixmap.scaleToHeight(iconHeight);

	if (invisibleSubNodeCount)
	{
		QString numberStr = QString::number(invisibleSubNodeCount);
		m_text->setText(numberStr);
	}
	else
	{
		pixmap.mirror();
	}

	m_icon->setPixmap(pixmap.pixmap());
}

QtGraphNodeExpandToggle::~QtGraphNodeExpandToggle()
{
}

bool QtGraphNodeExpandToggle::isExpandToggleNode() const
{
	return true;
}

void QtGraphNodeExpandToggle::onClick()
{
	QtGraphNode* parent = getParent();

	if (parent && parent->getData())
	{
		MessageGraphNodeExpand(parent->getData()->getId()).dispatch();
	}
}

void QtGraphNodeExpandToggle::updateStyle()
{
	GraphViewStyle::NodeStyle style = GraphViewStyle::getStyleOfExpandToggleNode();
	setStyle(style);

	m_text->setPos(
		(m_rect->rect().width() - QFontMetrics(m_text->font()).width(m_text->text())) / 2,
		5
	);

	m_icon->setPos(
		(m_rect->rect().width() - m_icon->pixmap().width() / QtDeviceScaledPixmap::devicePixelRatio()) / 2,
		(m_allVisible ? 9 : 14)
	);
}