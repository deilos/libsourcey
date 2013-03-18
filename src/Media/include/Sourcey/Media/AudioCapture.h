//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_MEDIA_AudioCapture_H
#define SOURCEY_MEDIA_AudioCapture_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/ICapture.h"
#include "Sourcey/Media/Format.h"
#include "Sourcey/Signal.h"

#include "RtAudio.h"

#include <iostream>
#include <queue>
#include <map>


namespace Sourcey {
namespace Media {

	

typedef signed short AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT16
/*
typedef char AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT8

typedef signed long AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT24

typedef signed long AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT32

typedef float AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_FLOAT32

typedef double AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_FLOAT64
*/


DefinePolymorphicDelegateWithArg(audioDelegate, IPacket, PacketDelegateBase, Void, NULL)


class AudioCapture: public ICapture
{
public:	
	AudioCapture(int deviceId, int channels, int sampleRate, RtAudioFormat format = RTAUDIO_SINT16);
	virtual ~AudioCapture();
	
  	virtual void open(); //int channels, int sampleRate, RtAudioFormat format = RTAUDIO_SINT16)
  	virtual void close();

  	virtual void start();
  	virtual void stop();
	
	virtual void attach(const PacketDelegateBase& delegate);
	virtual bool detach(const PacketDelegateBase& delegate);

	virtual int deviceId() const;
	virtual int sampleRate() const;
	virtual int numChannels() const;
	virtual bool isRunning() const;
	virtual bool isOpen() const;
	virtual RtAudioFormat format() const;

protected:
	virtual void setError(const std::string& message);
		/// Sets the error message and throws and exception.

	static int callback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
					    double streamTime, RtAudioStreamStatus status, void *data);

private:
	mutable Poco::FastMutex		_mutex;
	RtAudio::StreamParameters	_iParams;
	RtAudioFormat _format;
	RtAudio		_audio;
	int			_deviceId; 
	int			_channels;
	int			_sampleRate;
	bool		_isOpen;
	std::string _error;
};


typedef std::map<int, AudioCapture*> AudioCaptureMap;


inline void AllocateRtAudioInputFormat(const AudioCapture* capture, Format& format) 
	/// Allocates an OpenCV compatible input format for
	/// our encoders.
{
	assert(capture);
	format.audio.sampleFmt = "s16"; //RTAUDIO_SINT16; // TODO: Convert from RtAudioFormat to SampleFormat
	format.audio.channels = capture->numChannels();
	format.audio.sampleRate = capture->sampleRate();
	format.audio.enabled = true;
}


} } // namespace Sourcey::Media


#endif




/*
struct Samples 
{
	unsigned char* data; 
	int size;
	double time;
};


/// This is the base class for all instantiations of
/// the AudioCallback template.
class AbstractAudioCallback
{
public:
	AbstractAudioCallback() {};
	virtual ~AbstractAudioCallback() {};
	
	virtual void invoke(Samples& frame) const = 0;
	virtual AbstractAudioCallback* clone() const = 0;
	virtual void* object() const = 0;
};


typedef std::vector<AbstractAudioCallback*> CallbackList;


/// This template class implements an adapter that sits between
/// a AudioCapture and an object's method invoked by the capture.
template <class C> 
class AudioCallback: public AbstractAudioCallback
{
public:
	typedef void (C::*Callback)(Samples&);

	AudioCallback(C& object, Callback method) : 
		_object(&object), _method(method) {}
	AudioCallback(const AudioCallback& callback) : 
		_object(callback._object), _method(callback._method) {}

	void invoke(Samples& frame) const
	{
		(_object->*_method)(frame);
	}

	AbstractAudioCallback* clone() const
	{
		return new AudioCallback(*this);
	}

	void* object() const
	{
		return _object;
	}

private:
	AudioCallback() {};

	C*       _object;
	Callback _method;
};
	

template <class C>
static Callback<C, const AudioPacket, false> AudioCallback(C* object, void (C::*Method)(const AudioPacket&)) 
	/// Defines a callback for audio packets.
{
	return Callback<C, const AudioPacket, false>(object, Method);
};
*/