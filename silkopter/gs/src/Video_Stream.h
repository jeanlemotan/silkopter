#pragma once

#include "ui_Video_Stream.h"
#include "Video_Client.h"

class Video_Stream : public QWidget
{
public:
	Video_Stream(QWidget *parent = 0);
	~Video_Stream();

	void init(std::shared_ptr<Video_Client> client);

public:
	void process();

private:

	void paintEvent(QPaintEvent* ev);

	Ui::Video_Stream m_ui;

	QPainter m_painter;
	QImage m_image;
	QImage m_image_flipped;
	//Video_Client::Frame_Data m_frame;
	uint32_t m_frame_idx = 0;
	math::vec2u16 m_resolution;
	std::vector<uint8_t> m_data;
	std::shared_ptr<Video_Client> m_client;

	struct FFMPEG
	{
		AVCodec* codec = nullptr;
		AVCodecContext* context = nullptr;
		SwsContext* sws_context = nullptr;
		AVFrame* frame_yuv = nullptr;
		AVFrame* frame_rgb = nullptr;
		AVPicture rgb;
	} m_ffmpeg;
};


