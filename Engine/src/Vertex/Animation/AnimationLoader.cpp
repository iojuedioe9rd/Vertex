#include <vxpch.h>
/*#include "Vertex/Animation/AnimationLoader.h"

#include <ffmpegcpp.h>

using namespace ffmpegcpp;

struct VideoS
{
	Vertex::Video video_ptr;
	glm::ivec3 info;
};

class RawFrameSink : public VideoFrameSink, public FrameWriter
{
public:
    RawFrameSink()
    {
    }

    FrameSinkStream* CreateStream()
    {
        stream = new FrameSinkStream(this, 0);
        return stream;
    }

    void ResizeVideo(Vertex::Video& video, int newFrameCount)
    {
        // Allocate a new array of Frame pointers
        Vertex::Frame* newVideo = new Vertex::Frame[newFrameCount];

        // Copy existing frame pointers to the new array
        if (video)
        {
            for (int i = 0; i < newFrameCount; ++i)
            {
                newVideo[i] = video[i];
            }
            delete[] video; // Free the old array of Frame pointers
        }

        video = newVideo; // Update the video pointer
    }


    // Process the raw frame data
    virtual void WriteFrame(int streamIndex, AVFrame* frame, StreamData* streamData)
    {


        frameWidth = frame->width;
        frameHeight = frame->height;
        lineSize = frame->linesize[0];

        if (!video_ptr) {  return; };

        if (!swsContext)
        {
            swsContext = sws_getContext(
                frameWidth, frameHeight, (AVPixelFormat)frame->format,
                frameWidth, frameHeight, AV_PIX_FMT_RGB24,
                SWS_BILINEAR, nullptr, nullptr, nullptr);
        }

        (*video_ptr)[frameNumber] = new uint8_t[frameWidth * frameHeight * 3];

        uint8_t* rgbData[1] = { (*video_ptr)[frameNumber] };
        int rgbStride[1] = { 3 * frameWidth };

        sws_scale(swsContext, frame->data, frame->linesize, 0, frameHeight, rgbData, rgbStride);

        // You can process rawData here (e.g., pass it to another library or store it in memory)

        // For demonstration, just print the first 10 bytes of raw data

        ++frameNumber;

    }

    virtual void Close(int streamIndex)
    {
        delete stream;

    }

    virtual bool IsPrimed()
    {
        return true;
    }

public:
    int frameWidth = 0;
    int lineSize = 0;
    int frameHeight = 0;
    int frameNumber = 0;
    FrameSinkStream* stream;
    Vertex::Video* video_ptr = 0;
    SwsContext* swsContext = nullptr;


};
std::unordered_map<std::filesystem::path, VideoS> Videos;

namespace Vertex {

    

	bool AnimationLoader::video_reader_open(Video* video_ptr, glm::ivec3* info, const char* path)
	{
		
        VX_INFO(path);

        Demuxer demuxer = Demuxer(path);
        VX_INFO(demuxer.GetFileName());

        

		RawFrameSink* sink = new RawFrameSink();
        
        

        
        demuxer.PreparePipeline();

        *video_ptr = (Video)malloc(demuxer.GetFrameCount(0));
        sink->video_ptr = video_ptr;

        demuxer.DecodeBestVideoStream(sink);
        

		(*info).x = sink->frameWidth;
		(*info).y = sink->frameHeight;
		//(*info).z = frameCount;

		while (!demuxer.IsDone())
		{
            
			demuxer.Step();
		}
		Videos[path] = { *video_ptr, *info };

		return true;
        
	}

	void AnimationLoader::video_reader_close(Video* video_ptr)
	{
	}
}
*/