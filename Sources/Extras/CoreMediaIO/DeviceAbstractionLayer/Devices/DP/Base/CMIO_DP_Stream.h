/*
	    File: CMIO_DP_Stream.h
	Abstract: n/a
	 Version: 1.2

*/

#if !defined(__CMIO_DP_Stream_h__)
#define __CMIO_DP_Stream_h__

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Super Class Includes
#include "CMIO_DP_Object.h"

// Internal Includes
#include "CMIO_DP_Property_FormatList.h"
#include "CMIO_DP_Property_Clock.h"

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Types in CMIO::DP namespace
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Device;
}}

namespace CMIO { namespace DP
{
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Stream
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	class Stream : public Object
	{
	// Construction/Destruction
	public:
									Stream(CMIOStreamID cmioStreamID, PlugIn& plugIn, Device& owningDevice, CMIOObjectPropertyScope scope, UInt32 startingDeviceChannelNumber);
									Stream(CMIOStreamID cmioStreamID, PlugIn& plugIn, Device& owningDevice, CMIOObjectPropertyScope scope, UInt32 startingDeviceChannelNumber, UInt32 startingDeviceChannelNumberOffset);
		virtual						~Stream();
		
		virtual void				Initialize();
		virtual void				Teardown();

	// Attributes
	public:
		virtual CAMutex*			GetObjectStateMutex();
		virtual CFStringRef			CopyStreamName() const;
		virtual CFStringRef			CopyStreamManufacturerName() const;
		virtual CFStringRef			CopyElementFullName(const CMIOObjectPropertyAddress& address) const;
		virtual CFStringRef			CopyElementCategoryName(const CMIOObjectPropertyAddress& address) const;
		virtual CFStringRef			CopyElementNumberName(const CMIOObjectPropertyAddress& address) const;
		Device&						GetOwningDevice() const { return mOwningDevice; }
		bool						IsInput() const { return mIsInput; }
		bool						IsOutput() const { return not mIsInput; }
		CMIOObjectPropertyScope		GetDevicePropertyScope() const { return mIsInput ? kCMIODevicePropertyScopeInput : kCMIODevicePropertyScopeOutput; }
		CMIOObjectPropertyScope		GetOppositeDevicePropertyScope() const	{ return mIsInput ? kCMIODevicePropertyScopeOutput : kCMIODevicePropertyScopeInput; }
		virtual UInt32				GetTerminalType() const;
		UInt32						GetStartingDeviceChannelNumber() const  { return mStartingDeviceChannelNumber + mStartingDeviceChannelNumberOffset; }
		void						SetStartingDeviceChannelNumberOffset(UInt32 startingDeviceChannelNumberOffset) { mStartingDeviceChannelNumberOffset = startingDeviceChannelNumberOffset; }
		virtual UInt32				GetLatency() const;
		virtual void				SetNoDataTimeout(UInt32 noDataTimeout);
		virtual void				SetDeviceSyncTimeout(UInt32 deviceSyncTimeout);
		virtual void				SetEndOfData(UInt32 endOfData);

	protected:
		Device&						mOwningDevice;
		bool						mIsInput;
		UInt32						mStartingDeviceChannelNumber;
		UInt32						mStartingDeviceChannelNumberOffset;

	// Property Access
	public:
		void						ChangeDevicePropertyAddressIntoStreamPropertyAddress(CMIOObjectPropertyAddress& address) const;
		void						ChangeStreamPropertyAddressIntoDevicePropertyAddress(CMIOObjectPropertyAddress& address) const;
		
		virtual bool				HasProperty(const CMIOObjectPropertyAddress& address) const;
		virtual bool				IsPropertySettable(const CMIOObjectPropertyAddress& address) const;
		virtual UInt32				GetPropertyDataSize(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData) const;
		virtual void				GetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, UInt32& dataUsed, void* data) const;
		virtual void				SetPropertyData(const CMIOObjectPropertyAddress& address, UInt32 qualifierDataSize, const void* qualifierData, UInt32 dataSize, const void* data);
		virtual UInt32				GetStreamID() const;

	//  Basic Operations
	public:
		virtual void				Show() const;

	// Format Operations
	public:
		UInt32						GetCurrentNumberChannels() const { return mFormatList->GetCurrentNumberChannels(); }
		CMFormatDescriptionRef		GetCurrentFormat() const { return mFormatList->GetCurrentFormat(); }

	protected:
		Property::FormatList*		mFormatList;
		Property::Clock*			mClock;

	// Deck Control
	public:
		virtual void				DeckPlay() { throw CAException(kCMIOHardwareIllegalOperationError); }
		virtual void				DeckStop() { throw CAException(kCMIOHardwareIllegalOperationError); }
		virtual void				DeckJog(SInt32 speed) { throw CAException(kCMIOHardwareIllegalOperationError); }
		virtual void				DeckCueTo(Float64 requestedTimecode, Boolean playOnCue) { throw CAException(kCMIOHardwareIllegalOperationError); }
		
	// Hardware Operations
	public:
		virtual bool				TellHardwareToSetFormatDescription(CMFormatDescriptionRef format);
		virtual bool				TellHardwareToSetFrameRate(Float64 frameRate);
		virtual bool				TellHardwareToSetMinimumFrameRate(Float64 frameRate);
		virtual CMSampleBufferRef	TellHardwareToGetStillImage(CMFormatDescriptionRef description);
		virtual bool				TellHardwareToSetPreferredFormatDescription(CMFormatDescriptionRef preferredFormat);
		virtual bool				TellHardwareToSetPreferredFrameRate(Float64 preferredFrameRate);

	// IO Management
	public:
		virtual void				Start() = 0;
		virtual void				Stop() = 0;
		virtual bool				Streaming() const { return mStreaming; };
		virtual CMSimpleQueueRef	CopyBufferQueue(CMIODeviceStreamQueueAlteredProc queueAlteredProc, void* queueAlteredRefCon) = 0;
		
	protected:
		bool						mStreaming;
	};
}}
#endif
