#include "pch.h"
#include <vcclr.h>
#include "Animation.h"
#include <cstdlib>

namespace ImageParser
{
    Animation::~Animation()
    {
        this->!Animation();
    }

    Animation::!Animation()
    {
        CleanUp();
    }

    bool Animation::GetOk()
    {
        return m_Ok_;
    }

    bool Animation::ParseAnimationFrames(String^ file_name)
    {
        CleanUp();
        m_pImage_ = new AnimatedImage();
        pin_ptr<const wchar_t> wfile_name = PtrToStringChars(file_name);
        m_Ok_ = 1 == ReadAnimatedImage(wfile_name, m_pImage_, 0, nullptr);

        return m_Ok_;
    }

    bool Animation::ParseAnimationFrames(array<System::Byte>^ bytes)
    {
        CleanUp();
        m_pImage_ = new AnimatedImage();
        const auto data = static_cast<uint8_t*>(malloc(bytes->Length));
        System::Runtime::InteropServices::Marshal::Copy(bytes, 0, IntPtr(data), bytes->Length);
        m_Ok_ = ReadAnimatedImage(data, bytes->Length, m_pImage_, 0, nullptr);

        return m_Ok_;
    }

    bool Animation::GetFrameAt(int index, System::IntPtr% data_ptr, int% data_size, int% duration)
    {
        if (m_pImage_ == nullptr)
            return false;

        if (index >= m_pImage_->num_frames)
            return false;

        const auto frame = &(m_pImage_->frames[index]);
        if (frame->rgba == nullptr)
            return false;

        data_ptr = IntPtr(frame->rgba);
        data_size = m_pImage_->canvas_width * kNumChannels * m_pImage_->canvas_height;
        duration = frame->duration;

        return true;
    }

    int Animation::GetFrameCount()
    {
        if (m_pImage_ == nullptr)
            return 0;

        return m_pImage_->num_frames;
    }

    int Animation::GetCavWidth()
    {
        if (m_pImage_ == nullptr)
            return 0;

        return  m_pImage_->canvas_width;
    }

    int Animation::GetCavHeight()
    {
        if (m_pImage_ == nullptr)
            return 0;

        return  m_pImage_->canvas_height;
    }

    bool Animation::HasAlpha()
    {
        if (m_pImage_ == nullptr)
            return false;

        return  m_pImage_->has_alpha;
    }

    void Animation::CleanUp()
    {
        if (m_pImage_ != nullptr)
        {
            ClearAnimatedImage(m_pImage_);

            delete m_pImage_;
            m_pImage_ = nullptr;
            m_Ok_ = false;
        }
    }
}


