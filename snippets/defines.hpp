// <tuple> includes <sys/sysmacros.h> through some other header
// this results in major(x) being resolved to gnu_dev_major(x)
// which is an expression in a constructor initializer list.
#if defined( major )
#  undef major
#endif
#if defined( minor )
#  undef minor
#endif

// Windows defines MemoryBarrier which is deprecated and collides
// with the VULKAN_HPP_NAMESPACE::MemoryBarrier struct.
#if defined( MemoryBarrier )
#  undef MemoryBarrier
#endif

// XLib.h defines True/False, which collides with our vk::True/vk::False
// ->  undef them and provide some namepace-secure constexpr
#if defined( True )
#  undef True
constexpr int True = 1;
#endif
#if defined( False )
#  undef False
constexpr int False = 0;
#endif
