  //=====================
  //=== Format Traits ===
  //=====================

  //=== Function Declarations ===

  #if defined( VULKAN_HPP_CXX_MODULE )
  #define VULKAN_HPP_EXPORT export
  #else
  #define VULKAN_HPP_EXPORT
  #endif

  // The three-dimensional extent of a texel block.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 std::array<uint8_t, 3> blockExtent( Format format );

  // The texel block size in bytes.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 uint8_t blockSize( Format format );

  // The class of the format (can't be just named "class"!)
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 char const * compatibilityClass( Format format );

  // The number of bits in this component, if not compressed, otherwise 0.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 uint8_t componentBits( Format format, uint8_t component );

  // The number of components of this format.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 uint8_t componentCount( Format format );

  // The name of the component
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 char const * componentName( Format format, uint8_t component );

  // The numeric format of the component
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 char const * componentNumericFormat( Format format, uint8_t component );

  // The plane this component lies in.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 uint8_t componentPlaneIndex( Format format, uint8_t component );

  // True, if the components of this format are compressed, otherwise false.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 bool componentsAreCompressed( Format format );

  // A textual description of the compression scheme, or an empty string if it is not compressed
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 char const * compressionScheme( Format format );

  // Get all formats
  VULKAN_HPP_EXPORT std::vector<Format> const & getAllFormats();

  // Get all color with a color component
  VULKAN_HPP_EXPORT std::vector<Format> const & getColorFormats();

  // Get all formats with a depth component
  VULKAN_HPP_EXPORT std::vector<Format> const & getDepthFormats();

  // Get all formats with a depth and a stencil component
  VULKAN_HPP_EXPORT std::vector<Format> const & getDepthStencilFormats();

  // Get all formats with a stencil component
  VULKAN_HPP_EXPORT std::vector<Format> const & getStencilFormats();

  // True, if this format has an alpha component
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 bool hasAlphaComponent( Format format );

  // True, if this format has a blue component
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 bool hasBlueComponent( Format format );

  // True, if this format has a depth component
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 bool hasDepthComponent( Format format );

  // True, if this format has a green component
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 bool hasGreenComponent( Format format );

  // True, if this format has a red component
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 bool hasRedComponent( Format format );

  // True, if this format has a stencil component
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 bool hasStencilComponent( Format format );

  // True, if the format is a color
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 bool isColor( Format format );

  // True, if this format is a compressed one.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 bool isCompressed( Format format );

  // The number of bits into which the format is packed. A single image element in this format can be stored in the same space as a scalar type of this bit
  // width.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 uint8_t packed( Format format );

  // The single-plane format that this plane is compatible with.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 Format planeCompatibleFormat( Format format, uint8_t plane );

  // The number of image planes of this format.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 uint8_t planeCount( Format format );

  // The relative height of this plane. A value of k means that this plane is 1/k the height of the overall format.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 uint8_t planeHeightDivisor( Format format, uint8_t plane );

  // The relative width of this plane. A value of k means that this plane is 1/k the width of the overall format.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 uint8_t planeWidthDivisor( Format format, uint8_t plane );

  // The number of texels in a texel block.
  VULKAN_HPP_EXPORT VULKAN_HPP_CONSTEXPR_14 uint8_t texelsPerBlock( Format format );

  //=== Function Definitions ===

  // The three-dimensional extent of a texel block.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 std::array<uint8_t, 3> blockExtent( Format format )
  {
    switch( format )
    {
${blockExtentCases}
      default: return {{1, 1, 1 }};
    }
  }

  // The texel block size in bytes.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t blockSize( Format format )
  {
    switch( format )
    {
${blockSizeCases}
      default : VULKAN_HPP_ASSERT( false ); return 0;
    }
  }

  // The class of the format (can't be just named "class"!)
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 char const * compatibilityClass( Format format )
  {
    switch( format )
    {
${classCases}
      default : VULKAN_HPP_ASSERT( false ); return "";
    }
  }

  // The number of bits in this component, if not compressed, otherwise 0.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t componentBits( Format format, uint8_t component )
  {
    switch( format )
    {
${componentBitsCases}
      default: return 0;
    }
  }

  // The number of components of this format.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t componentCount( Format format )
  {
    switch( format )
    {
${componentCountCases}
      default: return 0;
    }
  }

  // The name of the component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 char const * componentName( Format format, uint8_t component )
  {
    switch( format )
    {
${componentNameCases}
      default: return "";
    }
  }

  // The numeric format of the component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 char const * componentNumericFormat( Format format, uint8_t component )
  {
    switch( format )
    {
${componentNumericFormatCases}
      default: return "";
    }
  }

  // The plane this component lies in.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t componentPlaneIndex( Format format, uint8_t component )
  {
    switch( format )
    {
${componentPlaneIndexCases}
      default: return 0;
    }
  }

  // True, if the components of this format are compressed, otherwise false.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool componentsAreCompressed( Format format )
  {
    switch( format )
    {
${componentsAreCompressedCases}
        return true;
      default: return false;
    }
  }

  // A textual description of the compression scheme, or an empty string if it is not compressed
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 char const * compressionScheme( Format format )
  {
    switch( format )
    {
${compressionSchemeCases}
      default: return "";
    }
  }

  // Get all formats
  VULKAN_HPP_INLINE std::vector<Format> const & getAllFormats()
  {
    static std::vector<Format> allFormats = { ${allFormats} };
	return allFormats;
  }

  // Get all formats with a color component
  VULKAN_HPP_INLINE std::vector<Format> const & getColorFormats()
  {
    static std::vector<Format> colorFormats = { ${colorFormats} };
	return colorFormats;
  }

  // Get all formats with a depth component
  VULKAN_HPP_INLINE std::vector<Format> const & getDepthFormats()
  {
    static std::vector<Format> depthFormats = { ${depthFormats} };
    return depthFormats;
  }

  // Get all formats with a depth and a stencil component
  VULKAN_HPP_INLINE std::vector<Format> const & getDepthStencilFormats()
  {
    static std::vector<Format> depthStencilFormats = { ${depthStencilFormats} };
    return depthStencilFormats;
  }

  // Get all formats with a stencil component
  VULKAN_HPP_INLINE std::vector<Format> const & getStencilFormats()
  {
    static std::vector<Format> stencilFormats = { ${stencilFormats} };
    return stencilFormats;
  }

  // True, if this format has an alpha component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool hasAlphaComponent( Format format )
  {
    switch( format )
    {
${alphaCases}
        return true;
      default: return false;
    }
  }

  // True, if this format has a blue component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool hasBlueComponent( Format format )
  {
    switch( format )
    {
${blueCases}
        return true;
      default: return false;
    }
  }

  // True, if this format has a depth component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool hasDepthComponent( Format format )
  {
    switch( format )
    {
${depthCases}
        return true;
      default: return false;
    }
  }

  // True, if this format has a green component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool hasGreenComponent( Format format )
  {
    switch( format )
    {
${greenCases}
        return true;
      default: return false;
    }
  }

  // True, if this format has a red component
  VULKAN_HPP_CONSTEXPR_14 bool hasRedComponent( Format format )
  {
    switch( format )
    {
${redCases}
        return true;
      default: return false;
    }
  }

  // True, if this format has a stencil component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool hasStencilComponent( Format format )
  {
    switch( format )
    {
${stencilCases}
        return true;
      default: return false;
    }
  }

  // True, if this format is a color.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool isColor( Format format )
  {
    return hasRedComponent( format ) || hasGreenComponent( format ) || hasBlueComponent( format ) || hasAlphaComponent( format );
  }

  // True, if this format is a compressed one.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool isCompressed( Format format )
  {
    return ( *compressionScheme( format ) != 0 );
  }

  // The number of bits into which the format is packed. A single image element in this format
  // can be stored in the same space as a scalar type of this bit width.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t packed( Format format )
  {
    switch( format )
    {
${packedCases}
      default: return 0;
    }
  }

  // The single-plane format that this plane is compatible with.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 Format planeCompatibleFormat( Format format, uint8_t plane )
  {
    switch( format )
    {
${planeCompatibleCases}
      default: VULKAN_HPP_ASSERT( plane == 0 ); return format;
    }
  }

  // The number of image planes of this format.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t planeCount( Format format )
  {
    switch( format )
    {
${planeCountCases}
      default: return 1;
    }
  }

  // The relative height of this plane. A value of k means that this plane is 1/k the height of the overall format.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t planeHeightDivisor( Format format, uint8_t plane )
  {
    switch( format )
    {
${planeHeightDivisorCases}
      default: VULKAN_HPP_ASSERT( plane == 0 ); return 1;
    }
  }

  // The relative width of this plane. A value of k means that this plane is 1/k the width of the overall format.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t planeWidthDivisor( Format format, uint8_t plane )
  {
    switch( format )
    {
${planeWidthDivisorCases}
      default: VULKAN_HPP_ASSERT( plane == 0 ); return 1;
    }
  }

  // The number of texels in a texel block.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t texelsPerBlock( Format format )
  {
    switch( format )
    {
${texelsPerBlockCases}
      default: VULKAN_HPP_ASSERT( false ); return 0;
    }
  }
