#pragma once

#include "IXAudio2.h"

// All structures defined in this file use tight field packing
#pragma pack(push, 1)


/**************************************************************************
 *
 * XAudio2 constants, flags and error codes.
 *
 **************************************************************************/

 // Numeric boundary values
#define XAUDIO20_MAX_BUFFER_BYTES        0x80000000    // Maximum bytes allowed in a source buffer
#define XAUDIO20_MAX_QUEUED_BUFFERS      64            // Maximum buffers allowed in a voice queue
#define XAUDIO20_MAX_BUFFERS_SYSTEM      2             // Maximum buffers allowed for system threads (Xbox 360 only)
#define XAUDIO20_MAX_AUDIO_CHANNELS      64            // Maximum channels in an audio stream
#define XAUDIO20_MIN_SAMPLE_RATE         1000          // Minimum audio sample rate supported
#define XAUDIO20_MAX_SAMPLE_RATE         200000        // Maximum audio sample rate supported
#define XAUDIO20_MAX_VOLUME_LEVEL        16777216.0f   // Maximum acceptable volume level (2^24)
#define XAUDIO20_MIN_FREQ_RATIO          (1/1024.0f)   // Minimum SetFrequencyRatio argument
#define XAUDIO20_MAX_FREQ_RATIO          1024.0f       // Maximum MaxFrequencyRatio argument
#define XAUDIO20_DEFAULT_FREQ_RATIO      2.0f          // Default MaxFrequencyRatio argument
#define XAUDIO20_MAX_FILTER_ONEOVERQ     1.5f          // Maximum XAUDIO2_FILTER_PARAMETERS.OneOverQ
#define XAUDIO20_MAX_FILTER_FREQUENCY    1.0f          // Maximum XAUDIO2_FILTER_PARAMETERS.Frequency
#define XAUDIO20_MAX_LOOP_COUNT          254           // Maximum non-infinite XAUDIO2_BUFFER.LoopCount
#define XAUDIO20_MAX_INSTANCES           8             // Maximum simultaneous XAudio2 objects on Xbox 360

// For XMA voices on Xbox 360 there is an additional restriction on the MaxFrequencyRatio
// argument and the voice's sample rate: the product of these numbers cannot exceed 600000
// for one-channel voices or 300000 for voices with more than one channel.
#define XAUDIO20_MAX_RATIO_TIMES_RATE_XMA_MONO         600000
#define XAUDIO20_MAX_RATIO_TIMES_RATE_XMA_MULTICHANNEL 300000

// Numeric values with special meanings
#define XAUDIO20_COMMIT_NOW              0             // Used as an OperationSet argument
#define XAUDIO20_COMMIT_ALL              0             // Used in IXAudio2::CommitChanges
#define XAUDIO20_INVALID_OPSET           (UINT32)(-1)  // Not allowed for OperationSet arguments
#define XAUDIO20_NO_LOOP_REGION          0             // Used in XAUDIO2_BUFFER.LoopCount
#define XAUDIO20_LOOP_INFINITE           255           // Used in XAUDIO2_BUFFER.LoopCount
#define XAUDIO20_DEFAULT_CHANNELS        0             // Used in CreateMasteringVoice
#define XAUDIO20_DEFAULT_SAMPLERATE      0             // Used in CreateMasteringVoice

// Flags
#define XAUDIO20_DEBUG_ENGINE            0x0001        // Used in XAudio2Create on Windows only
#define XAUDIO20_VOICE_NOPITCH           0x0002        // Used in IXAudio2::CreateSourceVoice
#define XAUDIO20_VOICE_NOSRC             0x0004        // Used in IXAudio2::CreateSourceVoice
#define XAUDIO20_VOICE_USEFILTER         0x0008        // Used in IXAudio2::CreateSource/SubmixVoice
#define XAUDIO20_VOICE_MUSIC             0x0010        // Used in IXAudio2::CreateSourceVoice
#define XAUDIO20_PLAY_TAILS              0x0020        // Used in IXAudio2SourceVoice::Stop
#define XAUDIO20_END_OF_STREAM           0x0040        // Used in XAUDIO2_BUFFER.Flags
#define XAUDIO20_SEND_USEFILTER          0x0080        // Used in XAUDIO2_SEND_DESCRIPTOR.Flags

// Default parameters for the built-in filter
#define XAUDIO20_DEFAULT_FILTER_TYPE     XAUDIO20_LowPassFilter
#define XAUDIO20_DEFAULT_FILTER_FREQUENCY XAUDIO20_MAX_FILTER_FREQUENCY
#define XAUDIO20_DEFAULT_FILTER_ONEOVERQ 1.0f

// Internal XAudio2 constants
#ifdef _XBOX
#define XAUDIO20_QUANTUM_NUMERATOR   2             // On Xbox 360, XAudio2 processes audio
#define XAUDIO20_QUANTUM_DENOMINATOR 375           //  in 5.333ms chunks (= 2/375 seconds)
#else
#define XAUDIO20_QUANTUM_NUMERATOR   1             // On Windows, XAudio2 processes audio
#define XAUDIO20_QUANTUM_DENOMINATOR 100           //  in 10ms chunks (= 1/100 seconds)
#endif
#define XAUDIO20_QUANTUM_MS (1000.0f * XAUDIO20_QUANTUM_NUMERATOR / XAUDIO20_QUANTUM_DENOMINATOR)

// XAudio2 error codes
#define FACILITY_XAUDIO20 0x896
#define XAUDIO20_E_INVALID_CALL          0x88960001    // An API call or one of its arguments was illegal
#define XAUDIO20_E_XMA_DECODER_ERROR     0x88960002    // The XMA hardware suffered an unrecoverable error
#define XAUDIO20_E_XAPO_CREATION_FAILED  0x88960003    // XAudio2 failed to initialize an XAPO effect
#define XAUDIO20_E_DEVICE_INVALIDATED    0x88960004    // An audio device became unusable (unplugged, etc)


/**************************************************************************
 *
 * Forward declarations for the XAudio2 interfaces.
 *
 **************************************************************************/

#ifdef __cplusplus
#define FWD_DECLARE(x) interface x
#else
#define FWD_DECLARE(x) typedef interface x x
#endif

FWD_DECLARE(IXAudio20);
FWD_DECLARE(IXAudio20Voice);
FWD_DECLARE(IXAudio20SourceVoice);
FWD_DECLARE(IXAudio20SubmixVoice);
FWD_DECLARE(IXAudio20MasteringVoice);
FWD_DECLARE(IXAudio20EngineCallback);
FWD_DECLARE(IXAudio20VoiceCallback);


/**************************************************************************
 *
 * XAudio2 structures and enumerations.
 *
 **************************************************************************/

 // Used in IXAudio2::Initialize
#ifdef _XBOX
typedef enum XAUDIO20_XBOX_HWTHREAD_SPECIFIER
{
	XAUDIO20_XboxThread0 = 0x01,
	XAUDIO20_XboxThread1 = 0x02,
	XAUDIO20_XboxThread2 = 0x04,
	XAUDIO20_XboxThread3 = 0x08,
	XAUDIO20_XboxThread4 = 0x10,
	XAUDIO20_XboxThread5 = 0x20,
	XAUDIO20_ANY_PROCESSOR = XboxThread4,
	XAUDIO20_DEFAULT_PROCESSOR = XAUDIO20_ANY_PROCESSOR
} XAUDIO20_XBOX_HWTHREAD_SPECIFIER, XAUDIO20_PROCESSOR;
#else
typedef enum XAUDIO20_WINDOWS_PROCESSOR_SPECIFIER
{
	XAUDIO20_Processor1 = 0x00000001,
	XAUDIO20_Processor2 = 0x00000002,
	XAUDIO20_Processor3 = 0x00000004,
	XAUDIO20_Processor4 = 0x00000008,
	XAUDIO20_Processor5 = 0x00000010,
	XAUDIO20_Processor6 = 0x00000020,
	XAUDIO20_Processor7 = 0x00000040,
	XAUDIO20_Processor8 = 0x00000080,
	XAUDIO20_Processor9 = 0x00000100,
	XAUDIO20_Processor10 = 0x00000200,
	XAUDIO20_Processor11 = 0x00000400,
	XAUDIO20_Processor12 = 0x00000800,
	XAUDIO20_Processor13 = 0x00001000,
	XAUDIO20_Processor14 = 0x00002000,
	XAUDIO20_Processor15 = 0x00004000,
	XAUDIO20_Processor16 = 0x00008000,
	XAUDIO20_Processor17 = 0x00010000,
	XAUDIO20_Processor18 = 0x00020000,
	XAUDIO20_Processor19 = 0x00040000,
	XAUDIO20_Processor20 = 0x00080000,
	XAUDIO20_Processor21 = 0x00100000,
	XAUDIO20_Processor22 = 0x00200000,
	XAUDIO20_Processor23 = 0x00400000,
	XAUDIO20_Processor24 = 0x00800000,
	XAUDIO20_Processor25 = 0x01000000,
	XAUDIO20_Processor26 = 0x02000000,
	XAUDIO20_Processor27 = 0x04000000,
	XAUDIO20_Processor28 = 0x08000000,
	XAUDIO20_Processor29 = 0x10000000,
	XAUDIO20_Processor30 = 0x20000000,
	XAUDIO20_Processor31 = 0x40000000,
	XAUDIO20_Processor32 = 0x80000000,
	XAUDIO20_ANY_PROCESSOR = 0xffffffff,
	XAUDIO20_DEFAULT_PROCESSOR = XAUDIO20_ANY_PROCESSOR
} XAUDIO20_WINDOWS_PROCESSOR_SPECIFIER, XAUDIO20_PROCESSOR;
#endif

// Used in XAUDIO2_DEVICE_DETAILS below to describe the types of applications
// that the user has specified each device as a default for.  0 means that the
// device isn't the default for any role.
typedef enum XAUDIO20_DEVICE_ROLE
{
	XAUDIO20_NotDefaultDevice = 0x0,
	XAUDIO20_DefaultConsoleDevice = 0x1,
	XAUDIO20_DefaultMultimediaDevice = 0x2,
	XAUDIO20_DefaultCommunicationsDevice = 0x4,
	XAUDIO20_DefaultGameDevice = 0x8,
	XAUDIO20_GlobalDefaultDevice = 0xf,
	XAUDIO20_InvalidDeviceRole = ~XAUDIO20_GlobalDefaultDevice
} XAUDIO20_DEVICE_ROLE;

// Returned by IXAudio2::GetDeviceDetails
typedef struct XAUDIO20_DEVICE_DETAILS
{
	WCHAR DeviceID[256];                // String identifier for the audio device.
	WCHAR DisplayName[256];             // Friendly name suitable for display to a human.
	XAUDIO20_DEVICE_ROLE Role;           // Roles that the device should be used for.
	WAVEFORMATEXTENSIBLE OutputFormat;  // The device's native PCM audio output format.
} XAUDIO20_DEVICE_DETAILS;

// Returned by IXAudio2Voice::GetVoiceDetails
typedef struct XAUDIO20_VOICE_DETAILS
{
	UINT32 CreationFlags;               // Flags the voice was created with.
	UINT32 InputChannels;               // Channels in the voice's input audio.
	UINT32 InputSampleRate;             // Sample rate of the voice's input audio.
} XAUDIO20_VOICE_DETAILS;

// Used in XAUDIO2_VOICE_SENDS below
typedef struct XAUDIO20_SEND_DESCRIPTOR
{
	UINT32 Flags;                       // Either 0 or XAUDIO2_SEND_USEFILTER.
	IXAudio20Voice* pOutputVoice;        // This send's destination voice.
} XAUDIO20_SEND_DESCRIPTOR;

// Used in the voice creation functions and in IXAudio2Voice::SetOutputVoices
typedef struct XAUDIO20_VOICE_SENDS
{
	UINT32 SendCount;                   // Number of sends from this voice.
	XAUDIO20_SEND_DESCRIPTOR* pSends;    // Array of SendCount send descriptors.
} XAUDIO20_VOICE_SENDS;

// Used in XAUDIO2_EFFECT_CHAIN below
typedef struct XAUDIO20_EFFECT_DESCRIPTOR
{
	IUnknown* pEffect;                  // Pointer to the effect object's IUnknown interface.
	BOOL InitialState;                  // TRUE if the effect should begin in the enabled state.
	UINT32 OutputChannels;              // How many output channels the effect should produce.
} XAUDIO20_EFFECT_DESCRIPTOR;

// Used in the voice creation functions and in IXAudio2Voice::SetEffectChain
typedef struct XAUDIO20_EFFECT_CHAIN
{
	UINT32 EffectCount;                 // Number of effects in this voice's effect chain.
	XAUDIO20_EFFECT_DESCRIPTOR* pEffectDescriptors; // Array of effect descriptors.
} XAUDIO20_EFFECT_CHAIN;

// Used in XAUDIO2_FILTER_PARAMETERS below
typedef enum XAUDIO20_FILTER_TYPE
{
	XAUDIO20_LowPassFilter,                      // Attenuates frequencies above the cutoff frequency.
	XAUDIO20_BandPassFilter,                     // Attenuates frequencies outside a given range.
	XAUDIO20_HighPassFilter,                     // Attenuates frequencies below the cutoff frequency.
	XAUDIO20_NotchFilter                         // Attenuates frequencies inside a given range.
} XAUDIO20_FILTER_TYPE;

// Used in IXAudio2Voice::Set/GetFilterParameters and Set/GetOutputFilterParameters
typedef struct XAUDIO20_FILTER_PARAMETERS
{
	XAUDIO20_FILTER_TYPE Type;           // Low-pass, band-pass or high-pass.
	float Frequency;                    // Radian frequency (2 * sin(pi*CutoffFrequency/SampleRate));
										//  must be >= 0 and <= XAUDIO2_MAX_FILTER_FREQUENCY
										//  (giving a maximum CutoffFrequency of SampleRate/6).
	float OneOverQ;                     // Reciprocal of the filter's quality factor Q;
										//  must be > 0 and <= XAUDIO2_MAX_FILTER_ONEOVERQ.
} XAUDIO20_FILTER_PARAMETERS;

// Used in IXAudio2SourceVoice::SubmitSourceBuffer
typedef struct XAUDIO20_BUFFER
{
	UINT32 Flags;                       // Either 0 or XAUDIO2_END_OF_STREAM.
	UINT32 AudioBytes;                  // Size of the audio data buffer in bytes.
	const BYTE* pAudioData;             // Pointer to the audio data buffer.
	UINT32 PlayBegin;                   // First sample in this buffer to be played.
	UINT32 PlayLength;                  // Length of the region to be played in samples,
										//  or 0 to play the whole buffer.
	UINT32 LoopBegin;                   // First sample of the region to be looped.
	UINT32 LoopLength;                  // Length of the desired loop region in samples,
										//  or 0 to loop the entire buffer.
	UINT32 LoopCount;                   // Number of times to repeat the loop region,
										//  or XAUDIO2_LOOP_INFINITE to loop forever.
	void* pContext;                     // Context value to be passed back in callbacks.
} XAUDIO20_BUFFER;

// Used in IXAudio2SourceVoice::SubmitSourceBuffer when submitting XWMA data.
// NOTE: If an XWMA sound is submitted in more than one buffer, each buffer's
// pDecodedPacketCumulativeBytes[PacketCount-1] value must be subtracted from
// all the entries in the next buffer's pDecodedPacketCumulativeBytes array.
// And whether a sound is submitted in more than one buffer or not, the final
// buffer of the sound should use the XAUDIO2_END_OF_STREAM flag, or else the
// client must call IXAudio2SourceVoice::Discontinuity after submitting it.
typedef struct XAUDIO20_BUFFER_WMA
{
	const UINT32* pDecodedPacketCumulativeBytes; // Decoded packet's cumulative size array.
												 //  Each element is the number of bytes accumulated
												 //  when the corresponding XWMA packet is decoded in
												 //  order.  The array must have PacketCount elements.
	UINT32 PacketCount;                          // Number of XWMA packets submitted. Must be >= 1 and
												 //  divide evenly into XAUDIO2_BUFFER.AudioBytes.
} XAUDIO20_BUFFER_WMA;

// Returned by IXAudio2SourceVoice::GetState
typedef struct XAUDIO20_VOICE_STATE
{
	void* pCurrentBufferContext;        // The pContext value provided in the XAUDIO2_BUFFER
										//  that is currently being processed, or NULL if
										//  there are no buffers in the queue.
	UINT32 BuffersQueued;               // Number of buffers currently queued on the voice
										//  (including the one that is being processed).
	UINT64 SamplesPlayed;               // Total number of samples produced by the voice since
										//  it began processing the current audio stream.
} XAUDIO20_VOICE_STATE;

// Returned by IXAudio2::GetPerformanceData
typedef struct XAUDIO20_PERFORMANCE_DATA
{
	// CPU usage information
	UINT64 AudioCyclesSinceLastQuery;   // CPU cycles spent on audio processing since the
										//  last call to StartEngine or GetPerformanceData.
	UINT64 TotalCyclesSinceLastQuery;   // Total CPU cycles elapsed since the last call
										//  (only counts the CPU XAudio2 is running on).
	UINT32 MinimumCyclesPerQuantum;     // Fewest CPU cycles spent processing any one
										//  audio quantum since the last call.
	UINT32 MaximumCyclesPerQuantum;     // Most CPU cycles spent processing any one
										//  audio quantum since the last call.

	// Memory usage information
	UINT32 MemoryUsageInBytes;          // Total heap space currently in use.

	// Audio latency and glitching information
	UINT32 CurrentLatencyInSamples;     // Minimum delay from when a sample is read from a
										//  source buffer to when it reaches the speakers.
	UINT32 GlitchesSinceEngineStarted;  // Audio dropouts since the engine was started.

	// Data about XAudio2's current workload
	UINT32 ActiveSourceVoiceCount;      // Source voices currently playing.
	UINT32 TotalSourceVoiceCount;       // Source voices currently existing.
	UINT32 ActiveSubmixVoiceCount;      // Submix voices currently playing/existing.

	UINT32 ActiveResamplerCount;        // Resample xAPOs currently active.
	UINT32 ActiveMatrixMixCount;        // MatrixMix xAPOs currently active.

	// Usage of the hardware XMA decoder (Xbox 360 only)
	UINT32 ActiveXmaSourceVoices;       // Number of source voices decoding XMA data.
	UINT32 ActiveXmaStreams;            // A voice can use more than one XMA stream.
} XAUDIO20_PERFORMANCE_DATA;

// Used in IXAudio2::SetDebugConfiguration
typedef struct XAUDIO20_DEBUG_CONFIGURATION
{
	UINT32 TraceMask;                   // Bitmap of enabled debug message types.
	UINT32 BreakMask;                   // Message types that will break into the debugger.
	BOOL LogThreadID;                   // Whether to log the thread ID with each message.
	BOOL LogFileline;                   // Whether to log the source file and line number.
	BOOL LogFunctionName;               // Whether to log the function name.
	BOOL LogTiming;                     // Whether to log message timestamps.
} XAUDIO20_DEBUG_CONFIGURATION;

// Values for the TraceMask and BreakMask bitmaps.  Only ERRORS and WARNINGS
// are valid in BreakMask.  WARNINGS implies ERRORS, DETAIL implies INFO, and
// FUNC_CALLS implies API_CALLS.  By default, TraceMask is ERRORS and WARNINGS
// and all the other settings are zero.
#define XAUDIO20_LOG_ERRORS     0x0001   // For handled errors with serious effects.
#define XAUDIO20_LOG_WARNINGS   0x0002   // For handled errors that may be recoverable.
#define XAUDIO20_LOG_INFO       0x0004   // Informational chit-chat (e.g. state changes).
#define XAUDIO20_LOG_DETAIL     0x0008   // More detailed chit-chat.
#define XAUDIO20_LOG_API_CALLS  0x0010   // Public API function entries and exits.
#define XAUDIO20_LOG_FUNC_CALLS 0x0020   // Internal function entries and exits.
#define XAUDIO20_LOG_TIMING     0x0040   // Delays detected and other timing data.
#define XAUDIO20_LOG_LOCKS      0x0080   // Usage of critical sections and mutexes.
#define XAUDIO20_LOG_MEMORY     0x0100   // Memory heap usage information.
#define XAUDIO20_LOG_STREAMING  0x1000   // Audio streaming information.


/**************************************************************************
 *
 * IXAudio2: Top-level XAudio2 COM interface.
 *
 **************************************************************************/

 // Use default arguments if compiling as C++
#ifdef __cplusplus
#define X20DEFAULT(x) =x
#else
#define X20DEFAULT(x)
#endif

DECLARE_INTERFACE_(IXAudio20, IUnknown)
{
    // NAME: IXAudio2::QueryInterface
    // DESCRIPTION: Queries for a given COM interface on the XAudio2 object.
    //              Only IID_IUnknown and IID_IXAudio2 are supported.
    //
    // ARGUMENTS:
    //  riid - IID of the interface to be obtained.
    //  ppvInterface - Returns a pointer to the requested interface.
    //
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, __deref_out void** ppvInterface) PURE;

    // NAME: IXAudio2::AddRef
    // DESCRIPTION: Adds a reference to the XAudio2 object.
    //
    STDMETHOD_(ULONG, AddRef) (THIS) PURE;

    // NAME: IXAudio2::Release
    // DESCRIPTION: Releases a reference to the XAudio2 object.
    //
    STDMETHOD_(ULONG, Release) (THIS) PURE;

    // NAME: IXAudio2::GetDeviceCount
    // DESCRIPTION: Returns the number of audio output devices available.
    //
    // ARGUMENTS:
    //  pCount - Returns the device count.
    //
    STDMETHOD(GetDeviceCount) (THIS_ __out UINT32* pCount) PURE;

    // NAME: IXAudio2::GetDeviceDetails
    // DESCRIPTION: Returns information about the device with the given index.
    //
    // ARGUMENTS:
    //  Index - Index of the device to be queried.
    //  pDeviceDetails - Returns the device details.
    //
    STDMETHOD(GetDeviceDetails) (THIS_ UINT32 Index, __out XAUDIO20_DEVICE_DETAILS* pDeviceDetails) PURE;

    // NAME: IXAudio2::Initialize
    // DESCRIPTION: Sets global XAudio2 parameters and prepares it for use.
    //
    // ARGUMENTS:
    //  Flags - Flags specifying the XAudio2 object's behavior.  Currently unused.
    //  XAudio2Processor - An XAUDIO2_PROCESSOR enumeration value that specifies
    //  the hardware thread (Xbox) or processor (Windows) that XAudio2 will use.
    //  The enumeration values are platform-specific; platform-independent code
    //  can use XAUDIO2_DEFAULT_PROCESSOR to use the default on each platform.
    //
    STDMETHOD(Initialize) (THIS_ UINT32 Flags X20DEFAULT(0),
                           XAUDIO20_PROCESSOR XAudio2Processor X20DEFAULT(XAUDIO20_DEFAULT_PROCESSOR)) PURE;

    // NAME: IXAudio2::RegisterForCallbacks
    // DESCRIPTION: Adds a new client to receive XAudio2's engine callbacks.
    //
    // ARGUMENTS:
    //  pCallback - Callback interface to be called during each processing pass.
    //
    STDMETHOD(RegisterForCallbacks) (__in IXAudio20EngineCallback* pCallback) PURE;

    // NAME: IXAudio2::UnregisterForCallbacks
    // DESCRIPTION: Removes an existing receiver of XAudio2 engine callbacks.
    //
    // ARGUMENTS:
    //  pCallback - Previously registered callback interface to be removed.
    //
    STDMETHOD_(void, UnregisterForCallbacks) (__in IXAudio20EngineCallback* pCallback) PURE;

    // NAME: IXAudio2::CreateSourceVoice
    // DESCRIPTION: Creates and configures a source voice.
    //
    // ARGUMENTS:
    //  ppSourceVoice - Returns the new object's IXAudio2SourceVoice interface.
    //  pSourceFormat - Format of the audio that will be fed to the voice.
    //  Flags - XAUDIO2_VOICE flags specifying the source voice's behavior.
    //  MaxFrequencyRatio - Maximum SetFrequencyRatio argument to be allowed.
    //  pCallback - Optional pointer to a client-provided callback interface.
    //  pSendList - Optional list of voices this voice should send audio to.
    //  pEffectChain - Optional list of effects to apply to the audio data.
    //
    STDMETHOD(CreateSourceVoice) (THIS_ __deref_out IXAudio20SourceVoice** ppSourceVoice,
                                  __in const WAVEFORMATEX* pSourceFormat,
                                  UINT32 Flags X20DEFAULT(0),
                                  float MaxFrequencyRatio X20DEFAULT(XAUDIO20_DEFAULT_FREQ_RATIO),
                                  __in_opt IXAudio20VoiceCallback* pCallback X20DEFAULT(NULL),
                                  __in_opt const XAUDIO20_VOICE_SENDS* pSendList X20DEFAULT(NULL),
                                  __in_opt const XAUDIO20_EFFECT_CHAIN* pEffectChain X20DEFAULT(NULL)) PURE;

    // NAME: IXAudio2::CreateSubmixVoice
    // DESCRIPTION: Creates and configures a submix voice.
    //
    // ARGUMENTS:
    //  ppSubmixVoice - Returns the new object's IXAudio2SubmixVoice interface.
    //  InputChannels - Number of channels in this voice's input audio data.
    //  InputSampleRate - Sample rate of this voice's input audio data.
    //  Flags - XAUDIO2_VOICE flags specifying the submix voice's behavior.
    //  ProcessingStage - Arbitrary number that determines the processing order.
    //  pSendList - Optional list of voices this voice should send audio to.
    //  pEffectChain - Optional list of effects to apply to the audio data.
    //
    STDMETHOD(CreateSubmixVoice) (THIS_ __deref_out IXAudio20SubmixVoice** ppSubmixVoice,
                                  UINT32 InputChannels, UINT32 InputSampleRate,
                                  UINT32 Flags X20DEFAULT(0), UINT32 ProcessingStage X20DEFAULT(0),
                                  __in_opt const XAUDIO20_VOICE_SENDS* pSendList X20DEFAULT(NULL),
                                  __in_opt const XAUDIO20_EFFECT_CHAIN* pEffectChain X20DEFAULT(NULL)) PURE;


    // NAME: IXAudio2::CreateMasteringVoice
    // DESCRIPTION: Creates and configures a mastering voice.
    //
    // ARGUMENTS:
    //  ppMasteringVoice - Returns the new object's IXAudio2MasteringVoice interface.
    //  InputChannels - Number of channels in this voice's input audio data.
    //  InputSampleRate - Sample rate of this voice's input audio data.
    //  Flags - XAUDIO2_VOICE flags specifying the mastering voice's behavior.
    //  DeviceIndex - Identifier of the device to receive the output audio.
    //  pEffectChain - Optional list of effects to apply to the audio data.
    //
    STDMETHOD(CreateMasteringVoice) (THIS_ __deref_out IXAudio20MasteringVoice** ppMasteringVoice,
                                     UINT32 InputChannels X20DEFAULT(XAUDIO20_DEFAULT_CHANNELS),
                                     UINT32 InputSampleRate X20DEFAULT(XAUDIO20_DEFAULT_SAMPLERATE),
                                     UINT32 Flags X20DEFAULT(0), UINT32 DeviceIndex X20DEFAULT(0),
                                     __in_opt const XAUDIO20_EFFECT_CHAIN* pEffectChain X20DEFAULT(NULL)) PURE;

    // NAME: IXAudio2::StartEngine
    // DESCRIPTION: Creates and starts the audio processing thread.
    //
    STDMETHOD(StartEngine) (THIS) PURE;

    // NAME: IXAudio2::StopEngine
    // DESCRIPTION: Stops and destroys the audio processing thread.
    //
    STDMETHOD_(void, StopEngine) (THIS) PURE;

    // NAME: IXAudio2::CommitChanges
    // DESCRIPTION: Atomically applies a set of operations previously tagged
    //              with a given identifier.
    //
    // ARGUMENTS:
    //  OperationSet - Identifier of the set of operations to be applied.
    //
    STDMETHOD(CommitChanges) (THIS_ UINT32 OperationSet) PURE;

    // NAME: IXAudio2::GetPerformanceData
    // DESCRIPTION: Returns current resource usage details: memory, CPU, etc.
    //
    // ARGUMENTS:
    //  pPerfData - Returns the performance data structure.
    //
    STDMETHOD_(void, GetPerformanceData) (THIS_ __out XAUDIO20_PERFORMANCE_DATA* pPerfData) PURE;

    // NAME: IXAudio2::SetDebugConfiguration
    // DESCRIPTION: Configures XAudio2's debug output (in debug builds only).
    //
    // ARGUMENTS:
    //  pDebugConfiguration - Structure describing the debug output behavior.
    //  pReserved - Optional parameter; must be NULL.
    //
    STDMETHOD_(void, SetDebugConfiguration) (THIS_ __in_opt const XAUDIO20_DEBUG_CONFIGURATION* pDebugConfiguration,
                                             __in_opt __reserved void* pReserved X20DEFAULT(NULL)) PURE;
};

// Undo the #pragma pack(push, 1) directive at the top of this file
#pragma pack(pop)
