#include "cameraview.h"

CameraView::CameraView(QQuickItem *parent) :
    QQuickItem(parent) {
	setFlag(QQuickItem::ItemHasContents);
	if (!_capture.open(0)) {
		qDebug() << "Could not open capture device.";
		// TODO: throw exception.
	}
}

CameraView::~CameraView() {
	_capture.release();
}

void CameraView::captureFrame() {
	const int ROWS = _newestFrame.rows;
	const int COLS = _newestFrame.cols;
	cv::Mat frame;
	_capture >> frame;
	cv::flip(frame, frame, 0);
	if (_captureDelta) {
		_newestFrame = frame - _newestFrame;
	}
	else {
		_newestFrame = frame;
	}
	if (_newestFrame.rows != ROWS)
		emit captureWidthChanged();
	if (_newestFrame.cols != COLS)
		emit captureHeightChanged();

	if (!_newestFrame.data) {
		qDebug() << "Could not capture frame.";
		// TODO: throw exception.
	}
//	qDebug() << "brightness:" << _capture.get(cv::CAP_PROP_BRIGHTNESS)
//	         << "contrast:" << _capture.get(cv::CAP_PROP_CONTRAST)
//	         << "saturation:" << _capture.get(cv::CAP_PROP_SATURATION)
//	         << "gain:" << _capture.get(cv::CAP_PROP_GAIN)
//	         << "white balance:" << _capture.get(cv::CAP_PROP_WHITE_BALANCE_RED_V);
}

void CameraView::updateView() {
	if (!_newestFrame.data)
		return;
	cv::Mat frame = _newestFrame.clone();
	if (_applyKernel)
		cv::filter2D(frame, frame, frame.depth(), _kernel->mat());
	if (_greyscale) {
		cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
		_img = QImage(frame.data,
		              frame.cols,
		              frame.rows,
		              QImage::Format_Grayscale8);
	}
	else {
		_img = QImage(frame.data,
		              frame.cols,
		              frame.rows,
		              QImage::Format_BGR888);
	}
	update();
}

int CameraView::captureWidth() const {
	return _capture.get(cv::CAP_PROP_FRAME_WIDTH);
}

void CameraView::setCaptureWidth(int width) {
	if (width == captureWidth())
		return;
	_capture.set(cv::CAP_PROP_FRAME_WIDTH, width);
	emit captureWidthChanged();
}

int CameraView::captureHeight() const {
	return _capture.get(cv::CAP_PROP_FRAME_HEIGHT);
}

void CameraView::setCaptureHeight(int height) {
	if (height == captureHeight())
		return;
	_capture.set(cv::CAP_PROP_FRAME_HEIGHT, height);
	emit captureHeightChanged();
}

bool CameraView::greyscale() const{
return _greyscale;
}

void CameraView::setGreyscale(bool greyscale) {
	if (greyscale == _greyscale)
		return;
	_greyscale = greyscale;
	// TODO: Check camera support for monochrome.
	//_capture.set(cv::CAP_PROP_MONOCHROME, 1);
	emit greyscaleChanged();
	updateView();
}

bool CameraView::captureDelta() const {
	return _captureDelta;
}

void CameraView::setCaptureDelta(bool capture) {
	if (capture == _captureDelta)
		return;
	_captureDelta = capture;
	emit captureDeltaChanged();
}

double CameraView::brightness() const {
	return _capture.get(cv::CAP_PROP_BRIGHTNESS);
}

void CameraView::setBrightness(double value) {
	if (value == brightness())
		return;
	_capture.set(cv::CAP_PROP_BRIGHTNESS, value);
	emit brightnessChanged();
}

double CameraView::contrast() const {
	return _capture.get(cv::CAP_PROP_CONTRAST);
}

void CameraView::setContrast(double value) {
	if (value == contrast())
		return;
	_capture.set(cv::CAP_PROP_CONTRAST, value);
	emit contrastChanged();
}

double CameraView::saturation() const {
	return _capture.get(cv::CAP_PROP_SATURATION);
}

void CameraView::setSaturation(double value) {
	if (value == saturation())
		return;
	_capture.set(cv::CAP_PROP_SATURATION, value);
	emit saturationChanged();
}

double CameraView::whiteBalance() const {
	return _capture.get(cv::CAP_PROP_WHITE_BALANCE_RED_V);
}

void CameraView::setWhiteBalance(double value) {
	if (value == whiteBalance())
		return;
	_capture.set(cv::CAP_PROP_WHITE_BALANCE_RED_V, value);
	emit whiteBalanceChanged();
}

KernelModel *CameraView::kernel() const {
	return _kernel;
}

void CameraView::setKernel(KernelModel *kernel) {
	if (kernel == _kernel)
		return;
	_kernel = kernel;
	emit kernelChanged();
}

bool CameraView::applyKernel() const {
	return _applyKernel;
}

void CameraView::setApplyKernel(bool apply) {
	if (apply == _applyKernel)
		return;
	_applyKernel = apply;
	emit applyKernelChanged();
	updateView();
}

bool CameraView::live() const {
	return _live;
}

void CameraView::setLive(bool live) {
	if (live == _live)
		return;
	_live = live;
	if (live)
		_timerID = startTimer(_refreshInterval);
	else if (_timerID != 0)
		killTimer(_timerID);
	emit liveChanged();
}

uint CameraView::refreshInterval() const {
	return _refreshInterval;
}

void CameraView::setRefreshInterval(const uint interval) {
	if (interval == _refreshInterval)
		return;
	_refreshInterval = interval;
	if (_live) {
		killTimer(_timerID);
		_timerID = startTimer(_refreshInterval);
	}
	emit refreshIntervalChanged();
}

void CameraView::timerEvent(QTimerEvent *event) {
	if (_live) {
		captureFrame();
		updateView();
	}
}

QSGNode *CameraView::updatePaintNode(QSGNode *oldNode,
                                     QQuickItem::UpdatePaintNodeData *data) {
	Q_UNUSED(data);
	QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>(oldNode);
	if (!node) {
		node = new QSGSimpleTextureNode;
	}
	QSGTexture *texture = window()->createTextureFromImage(_img);
	node->setOwnsTexture(true);
	node->setRect(boundingRect());
	node->setTexture(texture);
	node->markDirty(QSGNode::DirtyForceUpdate);
	return node;
}
