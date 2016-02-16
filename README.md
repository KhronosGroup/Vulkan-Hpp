# Open-Source Vulkan C++ API

Vulkan is a C API and as such inherits all common pitfalls of using a general C programming library. The motivation of a low-level Vulkan C++ API is to avoid these common pitfalls by applying commonly known C++ features while keeping the overall structure of a Vulkan program and preserving the full freedom it provides as low-level graphics API. An additional guideline we followed was not to introduce additional runtime overhead by providing a header-only library with inline functions.

Have a look at the following piece of code which creates a VkImage:

<pre>
<code>
  VkImageCreateInfo ci;
  ci.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.flags = ...some flags...;
  ci.imageType = VK_IMAGE_TYPE_2D;
  ci.format = VK_FORMAT_R8G8B8A8_UNORM;
  ci.extent = VkExtent3D { width, height, 1 };
  ci.mipLevels = 1;
  ci.arrayLayers = 1;
  ci.samples = VK_SAMPLE_COUNT_1_BIT;
  ci.tiling = VK_IMAGE_TILING_OPTIMAL;
  ci.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  ci.queueFamilyIndexCount = 0;
  ci.pQueueFamilyIndices = 0;
  ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  vkCreateImage(device, &ci, allocator, &image));
</code>
</pre>

There may be some issues that can happen when filling the structure which cannot be caught at compile time:

* initialization of <code>ci.sType</code> using wrong enum values
* uninitialized data fields (e.g. missing initialization of <code>ci.mipLevels</code>) 
* use of invalid bits for <code>ci.flags</code> (no type-safety for bits)
* use of incorrect enums for fields (no type-safety for enums)

These initializations will most likely show up as random runtime errors, which usually are nasty and time-consuming to debug.
Our auto-generated, C++ 11-conform layer uses commonly known C++ features like implicit initialization through constructors
to avoid incorrect or missing  initializations and introduces type-safety with scoped enums to turn explicit initialization
errors into compile errors. Following is a list of features and conventions introduced by our Vulkan C++ layer:

* works along the official C version of the API
* defines all symbols within the 'vk' namespace and to avoid redundancy the vk/Vk/VK_ prefixes have been removed from all symbols, i.e. <code>vk::commandBindPipeline</code> for vkCommandBindPipeline.
* camel case syntax with an 'e' prefix has been introduced for all enums, i.e. <code>vk::ImageType::e2D</code> (the prefix was a compromise, more about that later) removes the 'BIT' suffix from all flag related enums, i.e. <code>vk::ImageUsage::eColorAttachment</code>.
* introduces constructors for all structs, which by default set the appropriate <code>sType</code> and all other values to zero.
* encapsulates member variables of the structs with getter and setter functions, i.e. <code>ci.imageType()</code> to get a value and <code>ci.imageType(vk::ImageType::e2D)</code> to set a value.

With those changes applied, the updated code snippet looks like this:

<pre>
<code>
vk::ImageCreateInfo ci;
ci.flags(...some flags...);
ci.imageType(vk::ImageType::e2D);
ci.format(vk::Format::eR8G8B8A8Unorm);
ci.extent(vk::Extent3D { width, height, 1 });
ci.mipLevels(1);
ci.arrayLayers(1);
ci.samples(1);
ci.tiling(vk::ImageTiling::eOptimal);
ci.usage(vk::ImageUsage::eColorAttachment);
ci.sharingMode(vk::SharingMode::eExclusive);
  // ci.queueFamilyIndexCount(0)	// no need to set, already initialized
  // ci.pQueueFamilyIndices(0)	// no need to set, already initialized
ci.initialLayout(vk::ImageLayout::eUndefined);
vk::createImage(device, &ci, allocator, &image));
</code>
</pre>

Which is a total of 13 lines of code, versus 17 lines for the C version. In addition, this code is more robust as described above.

# Type-safe Enums

Splitting up the C enums into a namespace and scoped enums resulted in two compilation issues. 
First some enums started with a digit like <code>vk::ImageType::1D</code> which resulted in a compilation error. 
Second, there's the risk that upper symbols like <code>vk::CompositeAlphaFlagBitsKHR::OPAQUE</code> do clash with preprocessor defines.
In the given example <code>OPAQUE</code> has been defined in win32gdi.h resulting a compilation error.

To overcome those two issues the symbols have been converted to camel case and the prefix 'e' has been added so that each enum starts with a letter.

# Improvements to Bit Flags

After those changes the code might look more familiar to C++ developers, but there is still no gain with regards to safety.
 With C++ features available we replaced all Vulkan enums with scoped enums to achieve type safety which already uncovered
a few small issues in our code. The good thing with scoped enums is that there is no implicit casts to integer types anymore.
The downside is that OR'ing the bits for the flags does not work anymore without an explicit cast. As a solution to this problem
we have introduced a new <code>vk::Flags&lt;T&gt;</code> template which is used for all flags. This class supports the standard
operations one usually needs on bitmasks like &=, |=, & and |. Except for the initialization with 0, which is being replaced by
the default constructor, the <code>vk::Flags&lt;T&gt;</code> class works exactly like a normal bitmask with the improvement that
it is impossible to set bits not specified by the corresponding enum. To generate a bit mask with two bits set write:

<pre>
<code>
ci.usage = vk::ImageUsage::eColorAttachment | vk::ImageUsage::eStorage;
</code>
</pre>

By adding the scoped enums and <code>vk::Flags&lt;T&gt;</code> the C++ API provides type safety for all enums and flags which is a
big improvement. This leaves the remaining issue that the compiler might not detect uninitialized fields in structs. As a solution
we have added constructors to all structs which accept all values defined by the corresponding struct. 

<pre>
<code>
vk::ImageCreateInfo ci(
  ...some flags..., vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm,
  vk::Extent3D { width, height, 1 }, 1, 1,
  vk::SampleCount::e1, vk::ImageTiling::eOptimal,
  vk::ImageUsage::eColorAttachment, vk::SharingMode::eExclusive,
  0, 0, vk::ImageLayout::eUndefined);
</code>
</pre>

# Alternative Initialization of Structs

Another nice feature of those constructors is that sType is being initialized internally and thus is always correct.

Finally, we have added a default constructor to each struct which initializes all values to 0 to allow setting the values with the named parameter idiom which is similar to the designated initializer list of C99.

<pre>
<code>
vk::ImageCreateInfo ci = vk::ImageCreateInfo()
  .flags(...some flags...)
  .imageType(vk::ImageType::e2D)
  .format(vk::Format::eR8G8B8A8Unorm)
  .extent(vk::Extent3D { width, height, 1 })
  .mipLevels(1)
  .arrayLayers(1)
  .samples(1)
  .tiling(vk::ImageTiling::eOptimal)
  .usage(vk::ImageUsage::eColorAttachment)
  .sharingMode(vk::SharingMode::eExclusive)
  // .queueFamilyIndexCount(0)	// no need to set, already initialized
  // .pQueueFamilyIndices(0)	// no need to set, already initialized
  .initialLayout(vk::ImageLayout::eUndefined);
vk::createImage(device, &ci, allocator, &image));
</code>
</pre>


# Enhancements beyond the API
While mapping the Vulkan API to C++ without adding new functions is already a big help, one can do even more by adding new functionality. For example several C++ developers tend to use std::string and std::vector in their code, therefore we have added some more optional convenience features:

* Use <code>std::string</code> instead of <code>const char *</code> for strings
* Use <code>std::vector</code> instead of <code>(count, ptr)</code> for sized arrays
* Throw exceptions instead of error return values (in progress)
* Return handles/vectors where applicable, i.e. for the create* functions

As example let's examine the device extension property enumeration in Vulkan:

<pre>
<code>
    uint32_t count;
    VK_VERIFY(vk::enumerateDeviceExtensionProperties(physicalDevice, layerName.c_str(), &count, nullptr));

    std::vector<vk::ExtensionProperties> properties(count);
    VK_VERIFY(vk::enumerateDeviceExtensionProperties(physicalDevice, layerName.c_str(), &count, properties.data()));
</code>
</pre>
    
Luckily the official Khronos-provided vk.xml has enough information to figure out which pair of values represents a sized array or strings, so that it is possible to generate a function which allows you to write the following line of code instead:

<pre>
<code>
    std::vector&lt;ExtensionProperties&gt; properties = vk::enumerateDeviceExtensionProperties(physicalDevice, layerName);
</code>
</pre>

# Usage
To start with the C++ version of the Vulkan API download header from GIT, put it in a vulkan subdirectory and add
<code>#include &lt;vulkan/vk_cpp.h&gt;</code> to your source code.

To build the header for a given vk.xml specification continue with the following steps:

* Build VkCppGenerator
* Grab your favourite version vk.xml from Khronos
* Version 1.0 of the API has a tiny bug. The <require> section of the VK_KHR_display extension is missing one symbol which
can easily be fixed by adding the following line
<pre>
                &lt;type name="VkDisplayPlaneAlphaFlagsKHR"/&gt;
</pre>
before this line:
<pre>
                &lt;type name="VkDisplayPlaneAlphaFlagBitsKHR"/&gt;
</pre>
* Excute VkCppGenerator &lt;vk.xml&gt; to generate vk_cpp.h in the current working directory.

# Build instructions for VkCppGenerator

* Clone the repository
* Download tinyxml from http://www.grinninglizard.com/tinyxml/index.html and extract it in the source folder
* Use CMake to generate a solution or makefile for your favourite build environment
* Launch the build
