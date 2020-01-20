/*
 * Copyright 2003-2010 Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Marcus Overhagen
 */
#ifndef _MIXER_CORE_H
#define _MIXER_CORE_H


#include "MixerSettings.h"

#include <Buffer.h>
#include <Locker.h>
#include <TimeSource.h>


class AudioMixer;
class BBufferGroup;
class MixerInput;
class MixerOutput;
class Resampler;


// The number of "enum media_multi_channels" types from MediaDefs.h
// XXX should be 18, but limited to 12 here
#define MAX_CHANNEL_TYPES	12
// XXX using a dedicated mono channel, this should be channel type 31
//     but for now we redefine type 12
#define B_CHANNEL_MONO		B_CHANNEL_TOP_CENTER

#define MIXER_PROCESS_EVENT BTimedEventQueue::B_USER_EVENT+10
#define MIXER_SCHEDULE_EVENT BTimedEventQueue::B_USER_EVENT+11


class MixerCore {
public:
								MixerCore(AudioMixer* node);
	virtual						~MixerCore();

			MixerSettings*		Settings();
			void				UpdateResamplingAlgorithm();

	// To avoid calling Settings()->AttenuateOutput() for every outgoing
	// buffer, this setting is cached in fOutputGain and must be set by
	// the audio mixer node using SetOutputAttenuation()
			void				SetOutputAttenuation(float gain);
			MixerInput*			AddInput(const media_input& input);
			MixerOutput*		AddOutput(const media_output& output);

			bool				RemoveInput(int32 inputID);
			bool				RemoveOutput();
			int32				CreateInputID();

	// index = 0 to count-1, NOT inputID
			MixerInput*			Input(int index);
			MixerOutput*		Output();

			bool				Lock();
			bool				LockWithTimeout(bigtime_t timeout);
			bool				IsLocked() const;
			void				Unlock();

			void				Process();
			bigtime_t			PickEvent();

			void				BufferReceived(BBuffer* buffer,
									bigtime_t lateness);

			void				InputFormatChanged(int32 inputID,
									const media_multi_audio_format& format);
			void				OutputFormatChanged(
									const media_multi_audio_format& format);

			void				SetOutputBufferGroup(BBufferGroup* group);
			void				SetTimingInfo(BTimeSource* timeSource,
									bigtime_t downstreamLatency);
			void				EnableOutput(bool enabled);
			bool				Start();
			bool				Stop();

			void				StartMixThread();
			void				StopMixThread();
			uint32				OutputChannelCount();

private:
			void				_UpdateResamplers(
									const media_multi_audio_format& format);
			void				_ApplyOutputFormat();
	static	int32				_MixThreadEntry(void* arg);
			void				_MixThread();

private:
			BLocker*			fLocker;
			BList*				fInputs;
			MixerOutput*		fOutput;
			int32				fNextInputID;
			bool				fRunning;
									// true = the mix thread is running

			bool				fStarted;
									// true = mix thread should be
									// started of it is not running

			bool				fOutputEnabled;
									// true = mix thread should be
									// started of it is not running

			Resampler**			fResampler; // array
			float*				fMixBuffer;
			int32				fMixBufferFrameRate;
			int32				fMixBufferFrameCount;
			int32				fMixBufferChannelCount;
			int32*				fMixBufferChannelTypes; //array
			bool				fDoubleRateMixing;
			bigtime_t			fDownstreamLatency;
			MixerSettings*		fSettings;
			AudioMixer*			fNode;
			BBufferGroup*		fBufferGroup;
			BTimeSource*		fTimeSource;
			thread_id			fMixThread;
			sem_id				fMixThreadWaitSem;
			bool				fHasEvent;
			bigtime_t			fEventTime;
			bigtime_t			fEventLatency;
			float				fOutputGain;

	friend class MixerInput;
		// NOTE: debug only
};


inline bool
MixerCore::Lock()
{
	return fLocker->Lock();
}


inline bool
MixerCore::LockWithTimeout(bigtime_t timeout)
{
	return fLocker->LockWithTimeout(timeout) == B_OK;
}


inline bool
MixerCore::IsLocked() const
{
	return fLocker->IsLocked();
}


inline void
MixerCore::Unlock()
{
	fLocker->Unlock();
}


inline void
MixerCore::Process()
{
	release_sem(fMixThreadWaitSem);
}


inline bigtime_t
MixerCore::PickEvent()
{
	return fTimeSource->RealTimeFor(fEventTime, 0)
		- fEventLatency - fDownstreamLatency;
}


#endif // _MIXER_CORE_H
