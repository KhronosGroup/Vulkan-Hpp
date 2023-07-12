#if !defined( VULKAN_HPP_NO_SMART_HANDLE )
template <typename Type, typename Dispatch>
class UniqueHandleTraits;

class Instance;
class NoParent;

template <class HandleType>
using default_vk_deleter = typename UniqueHandleTraits<HandleType, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>::deleter;

template <class HandleType>
VULKAN_HPP_CONSTEXPR VULKAN_HPP_INLINE bool has_parent = !std::is_same<default_vk_deleter<HandleType>, default_vk_deleter<Instance>>::value;

template <class HandleType>
struct parent_of
{
  using parent = typename std::conditional<has_parent<HandleType>, decltype( std::declval<default_vk_deleter<HandleType>>().getOwner() ), NoParent>::type;
};

template <>
struct parent_of<NoParent>
{
  using parent = NoParent;
};

template <class HandleType>
using parent_of_t = typename parent_of<HandleType>::parent;

#endif