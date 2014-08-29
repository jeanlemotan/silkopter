#ifndef __PROFILER_H__
#define __PROFILER_H__

//#define __PROFILER_ENABLED__
#define __PROFILER_FULL_TYPE_EXPANSION__

#undef noinline
#undef fastcall

#if defined(_MSC_VER)
	#undef __PRETTY_FUNCTION__
	#define __PRETTY_FUNCTION__ __FUNCSIG__
	#define PROFILE_CONCAT( a, b ) a "/" b

	#define noinline __declspec(noinline)
	#define fastcall __fastcall
#else
	#define PROFILE_CONCAT( a, b ) b

	#define noinline __attribute__ ((noinline))
	#define fastcall
#endif

#if defined(__PROFILER_FULL_TYPE_EXPANSION__)
	#define PROFILE_FUNCTION() __PRETTY_FUNCTION__
#else
	#define PROFILE_FUNCTION() __FUNCTION__
#endif

#if defined(__PROFILER_ENABLED__)
	// thread
	#define PROFILE_THREAD_START_RAW( text )   Profiler::threadenter( text );
	#define PROFILE_THREAD_START()             PROFILE_THREAD_START_RAW( PROFILE_FUNCTION()  )
	#define PROFILE_THREAD_START_DESC( desc )  PROFILE_THREAD_START_RAW( PROFILE_CONCAT( PROFILE_FUNCTION(), desc ) )

	#define PROFILE_THREAD_SCOPED_RAW( text )  Profiler::ScopedThread profiler##__LINE__ ( text );
	#define PROFILE_THREAD_SCOPED()            PROFILE_THREAD_SCOPED_RAW( PROFILE_FUNCTION() )
	#define PROFILE_THREAD_SCOPED_DESC( desc ) PROFILE_THREAD_SCOPED_RAW( PROFILE_CONCAT( PROFILE_FUNCTION(), desc ) )

	#define PROFILE_THREAD_STOP()              Profiler::threadexit();

	// function
	#define PROFILE_PAUSE()             Profiler::pause();
	#define PROFILE_UNPAUSE()           Profiler::unpause();
	#define PROFILE_PAUSE_SCOPED()      Profiler::ScopedPause profilerpause##__LINE__;

	#define PROFILE_START_RAW( text )   Profiler::enter( text );
	#define PROFILE_START()             PROFILE_START_RAW( PROFILE_FUNCTION()  )
	#define PROFILE_START_DESC( desc )  PROFILE_START_RAW( PROFILE_CONCAT( PROFILE_FUNCTION(), desc ) )

	#define PROFILE_SCOPED_RAW( text )  Profiler::Scoped profiler##__LINE__ ( text );
	#define PROFILE_SCOPED()            PROFILE_SCOPED_RAW( PROFILE_FUNCTION() )
	#define PROFILE_SCOPED_DESC( desc ) PROFILE_SCOPED_RAW( PROFILE_CONCAT( PROFILE_FUNCTION(), desc ) )

	#define PROFILE_STOP()              Profiler::exit();
#else
	#define PROFILE_THREAD_START_RAW( text )
	#define PROFILE_THREAD_START()
	#define PROFILE_THREAD_START_DESC( desc )

	#define PROFILE_THREAD_SCOPED_RAW( text )
	#define PROFILE_THREAD_SCOPED()
	#define PROFILE_THREAD_SCOPED_DESC( desc )

	#define PROFILE_THREAD_STOP()

	#define PROFILE_PAUSE()
	#define PROFILE_UNPAUSE()
	#define PROFILE_PAUSE_SCOPED()

	#define PROFILE_START_RAW( text )
	#define PROFILE_START()
	#define PROFILE_START_DESC( desc )

	#define PROFILE_SCOPED_RAW( text )
	#define PROFILE_SCOPED()
	#define PROFILE_SCOPED_DESC( desc )

	#define PROFILE_STOP()
#endif

#if 0

namespace Profiler {
	/*
	=============
	Types that won't conflict with the rest of the system
	=============
	*/
	typedef float f32;
	typedef double f64;
	typedef unsigned char uint8_t;
	typedef unsigned short uint16_t;
	typedef unsigned int u32;
	#if defined(_MSC_VER)
		typedef unsigned __int64 uint64_t;
	#else
		typedef unsigned long long uint64_t;
	#endif

	template< class type1, class type2 >
	f64 average( type1 sum, type2 count ) {
		return ( count ) ? f64(sum)/f64(count) : 0;
	}

	/*
	=============
	Timer
	=============
	*/
	#pragma pack(push,1)
	struct Timer {
		Timer() { Reset(); }

		inline bool IsEmpty() const { return ticks == 0; }
		inline bool IsPaused() const { return paused; }
		inline void Unpause( uint64_t curticks ) { started = curticks; paused = false; }
		inline void Unpause() { Unpause( getticks() ); }
		inline void Pause( uint64_t curticks ) { ticks += ( curticks - started ); paused = true; }
		inline void Pause() { Pause( getticks() ); }		
		inline void Start() { ++calls; started = getticks(); }
		inline void Stop() { ticks += ( getticks() - started ); }
		inline void Reset() { ticks = started = calls = 0; paused = false; }
		inline void SoftStop() { if ( !paused ) { uint64_t t = getticks(); ticks += ( t - started ); started = t; } }
		inline void SoftReset() { ticks = 0; calls = 0; started = getticks(); }

		template< class type > static f64 ms( const type &t ) { return f64( t ) / 1000000.0; }
		f64 millicycles() { return ms( ticks ); }
		f64 currentmillicycles() { return ms( ticks + ( getticks() - started ) ); }
		f64 avg() { return average( ticks, calls ); }
		f64 avgms() { return ms( average( ticks, calls ) ); }

		void operator+= ( const Timer &b ) {
			ticks += b.ticks;
			calls += b.calls;
		}

		static inline uint64_t getticks_serial() {
	#if defined(__GNUC__)
			asm volatile("cpuid" : : : "%eax", "%ebx", "%ecx", "%edx" );
	#else
			__asm cpuid;
	#endif
			return getticks();			
		}

	#if defined(__GNUC__)
		static inline uint64_t getticks() {
			u32 __a,__d;
			asm volatile("rdtsc" : "=a" (__a), "=d" (__d));
			return ( uint64_t(__a) | uint64_t(__d) << 32 );
		}
	#elif defined(__ICC) || defined(__ICL)
		static inline uint64_t getticks() { return _rdtsc(); }
	#else
		static inline uint64_t getticks() { __asm { rdtsc }; }
	#endif

		uint64_t ticks, started;
		u32 calls;
		bool paused;
	};
	#pragma pack(pop)


	/*
	=============
	Interface functions
	=============
	*/

	void detect( int argc, const char *argv[] );
	void detect( const char *commandLine );
	void dump();
	void dumphtml();
	void fastcall enter( const char *name );
	void fastcall exit();
	void fastcall pause();
	void fastcall unpause();
	void threadenter( const char *name );
	void threadexit();
	void reset();

	struct Scoped {
		Scoped( const char *name ) { PROFILE_START_RAW( name ) }
		~Scoped() { PROFILE_STOP() }
	};

	struct ScopedPause {
		ScopedPause() { PROFILE_PAUSE() }
		~ScopedPause() { PROFILE_UNPAUSE() }
	};

	struct ScopedThread {
		ScopedThread( const char *name ) { PROFILE_THREAD_START_RAW( name ); }
		~ScopedThread() { PROFILE_THREAD_STOP() }
	};
}; // namespace Profiler


struct ScopedTimer {
	ScopedTimer( Profiler::Timer &t ) : mTimer(t) { mTimer.Start(); }
	~ScopedTimer() { mTimer.Stop(); }
protected:
	Profiler::Timer &mTimer;
};

#endif

#endif // __PROFILER_H__