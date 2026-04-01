${licenseHeader}

#ifndef VULKAN_HASH_HPP
#  define VULKAN_HASH_HPP

#if !defined( VULKAN_HPP_CXX_MODULE )
#  include <vulkan/${api}.hpp>
#endif

VULKAN_HPP_EXPORT namespace std
{
  //=======================================
  //=== HASH structures for Flags types ===
  //=======================================

  template <typename BitType>
  struct hash<VULKAN_HPP_NAMESPACE::Flags<BitType>>
  {
    std::size_t operator()( VULKAN_HPP_NAMESPACE::Flags<BitType> const & flags ) const VULKAN_HPP_NOEXCEPT
    {
      return std::hash<typename std::underlying_type<BitType>::type>{}(
        static_cast<typename std::underlying_type<BitType>::type>( flags ) );
    }
  };

${handleHashStructures}
${structHashStructures}
} // namespace std
#endif
