#include <vxpch.h>
#include "SoundBuffer.h"
#include <miniaudio.h>
#include <inttypes.h>
#include <AL/alext.h>
#include <cstdio>

namespace Vertex
{
    WindowsSoundBuffer* WindowsSoundBuffer::m_Instance = nullptr;

    WindowsSoundBuffer* WindowsSoundBuffer::get()
    {
        if (m_Instance == nullptr)
        {
            m_Instance = new WindowsSoundBuffer();
        }
        return m_Instance;
    }

    void WindowsSoundBuffer::destroy()
    {
        if (m_Instance == nullptr)
        {
            return;
        }
        delete m_Instance;
        m_Instance = nullptr;
    }

    ALuint WindowsSoundBuffer::addSoundEffect(const std::fs::path filename)
    {
        ma_decoder decoder;
        ma_result result = ma_decoder_init_file(filename.generic_string().c_str(), NULL, &decoder);
        if (result != MA_SUCCESS)
        {
            fprintf(stderr, "Failed to load audio file: %s\n", filename.generic_string().c_str());
            return 0;
        }

        // Get the number of bytes per sample based on the format.
        size_t bytes_per_sample = ma_get_bytes_per_sample(decoder.outputFormat);

        // Retrieve the total number of frames in the file.
        ma_uint64 total_frames = 0;
        result = ma_decoder_get_length_in_pcm_frames(&decoder, &total_frames);
        if (result != MA_SUCCESS || total_frames == 0)
        {
            fprintf(stderr, "Failed to get total frames from audio file\n");
            ma_decoder_uninit(&decoder);
            return 0;
        }

        // Allocate enough memory for the full audio data.
        size_t num_bytes = total_frames * decoder.outputChannels * bytes_per_sample;
        short* membuf = static_cast<short*>(malloc(num_bytes));
        if (membuf == nullptr)
        {
            fprintf(stderr, "Failed to allocate memory for audio data\n");
            ma_decoder_uninit(&decoder);
            return 0;
        }

        // Read the entire audio file into the buffer.
        ma_uint64 num_frames;
        result = ma_decoder_read_pcm_frames(&decoder, membuf, total_frames, &num_frames);
        if (result != MA_SUCCESS || num_frames != total_frames)
        {
            fprintf(stderr, "Failed to read all audio frames. Expected: %llu, Read: %llu\n", total_frames, num_frames);
            free(membuf);
            ma_decoder_uninit(&decoder);
            return 0;
        }

        // Update num_bytes based on the actual frames read.
        num_bytes = num_frames * decoder.outputChannels * bytes_per_sample;

        // Determine the OpenAL format.
        ALenum format = AL_NONE;
        switch (decoder.outputFormat)
        {
        case ma_format_u8:
            if (decoder.outputChannels == 1)
                format = AL_FORMAT_MONO8;
            else if (decoder.outputChannels == 2)
                format = AL_FORMAT_STEREO8;
            break;
        case ma_format_s16:
            if (decoder.outputChannels == 1)
                format = AL_FORMAT_MONO16;
            else if (decoder.outputChannels == 2)
                format = AL_FORMAT_STEREO16;
            break;
        default:
            fprintf(stderr, "Unsupported format: %d\n", decoder.outputFormat);
            free(membuf);
            ma_decoder_uninit(&decoder);
            return 0;
        }

        // Generate an OpenAL buffer.
        ALuint buffer = 0;
        alGenBuffers(1, &buffer);
        if (alGetError() != AL_NO_ERROR)
        {
            fprintf(stderr, "Failed to generate OpenAL buffer\n");
            free(membuf);
            ma_decoder_uninit(&decoder);
            return 0;
        }

        // Fill the OpenAL buffer with the audio data.
        alBufferData(buffer, format, membuf, num_bytes, decoder.outputSampleRate);
        ALenum err = alGetError();
        if (err != AL_NO_ERROR)
        {
            fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
            if (buffer && alIsBuffer(buffer))
                alDeleteBuffers(1, &buffer);
            free(membuf);
            ma_decoder_uninit(&decoder);
            return 0;
        }

        // Clean up.
        free(membuf);
        ma_decoder_uninit(&decoder);

        m_SoundEffectsBuffers.push_back(buffer);
        return buffer;
    }

    bool WindowsSoundBuffer::removeSoundEffect(const ALuint& buffer)
    {
        auto it = m_SoundEffectsBuffers.begin();
        while (it != m_SoundEffectsBuffers.end())
        {
            if (*it == buffer)
            {
                alDeleteBuffers(1, &*it);
                it = m_SoundEffectsBuffers.erase(it);
                return true;
            }
            else
            {
                ++it;
            }
        }
        return false; // Couldn't find buffer to remove.
    }

    WindowsSoundBuffer::WindowsSoundBuffer()
    {
        m_SoundEffectsBuffers.clear();
    }

    WindowsSoundBuffer::~WindowsSoundBuffer()
    {
        for (auto Sound : m_SoundEffectsBuffers)
        {
            alDeleteBuffers(1, &Sound);

        };
        m_SoundEffectsBuffers.clear();
    }
}
