  //=====================
  //=== Format Traits ===
  //=====================

  //=== Function Declarations ===

  // The three-dimensional extent of a texel block.
  VULKAN_HPP_CONSTEXPR_14 std::array<uint8_t, 3> blockExtent( VULKAN_HPP_NAMESPACE::Format format );

  // The texel block size in bytes.
  VULKAN_HPP_CONSTEXPR_14 uint8_t blockSize( VULKAN_HPP_NAMESPACE::Format format );

  // The class of the format (can't be just named "class"!)
  VULKAN_HPP_CONSTEXPR_14 char const * compatibilityClass( VULKAN_HPP_NAMESPACE::Format format );

  // The number of bits in this component, if not compressed, otherwise 0.
  VULKAN_HPP_CONSTEXPR_14 uint8_t componentBits( VULKAN_HPP_NAMESPACE::Format format, uint8_t component );

  // The number of components of this format.
  VULKAN_HPP_CONSTEXPR_14 uint8_t componentCount( VULKAN_HPP_NAMESPACE::Format format );

  // The name of the component
  VULKAN_HPP_CONSTEXPR_14 char const * componentName( VULKAN_HPP_NAMESPACE::Format format, uint8_t component );

  // The numeric format of the component
  VULKAN_HPP_CONSTEXPR_14 char const * componentNumericFormat( VULKAN_HPP_NAMESPACE::Format format, uint8_t component );

  // The plane this component lies in.
  VULKAN_HPP_CONSTEXPR_14 uint8_t componentPlaneIndex( VULKAN_HPP_NAMESPACE::Format format, uint8_t component );

  // True, if the components of this format are compressed, otherwise false.
  VULKAN_HPP_CONSTEXPR_14 bool componentsAreCompressed( VULKAN_HPP_NAMESPACE::Format format );

  // A textual description of the compression scheme, or an empty string if it is not compressed
  VULKAN_HPP_CONSTEXPR_14 char const * compressionScheme( VULKAN_HPP_NAMESPACE::Format format );

  // Get all formats with a depth component
  std::vector<VULKAN_HPP_NAMESPACE::Format> const & getDepthFormats();

  // Get all formats with a depth and a stencil component
  std::vector<VULKAN_HPP_NAMESPACE::Format> const & getDepthStencilFormats();

  // Get all formats with a stencil component
  std::vector<VULKAN_HPP_NAMESPACE::Format> const & getStencilFormats();

  // True, if this format has a depth component
  VULKAN_HPP_CONSTEXPR_14 bool hasDepthComponent( VULKAN_HPP_NAMESPACE::Format format );

  // True, if this format has a stencil component
  VULKAN_HPP_CONSTEXPR_14 bool hasStencilComponent( VULKAN_HPP_NAMESPACE::Format format );

  // True, if this format is a compressed one.
  VULKAN_HPP_CONSTEXPR_14 bool isCompressed( VULKAN_HPP_NAMESPACE::Format format );

  // The number of bits into which the format is packed. A single image element in this format can be stored in the same space as a scalar type of this bit
  // width.
  VULKAN_HPP_CONSTEXPR_14 uint8_t packed( VULKAN_HPP_NAMESPACE::Format format );

  // The single-plane format that this plane is compatible with.
  VULKAN_HPP_CONSTEXPR_14 VULKAN_HPP_NAMESPACE::Format planeCompatibleFormat( VULKAN_HPP_NAMESPACE::Format format, uint8_t plane );

  // The number of image planes of this format.
  VULKAN_HPP_CONSTEXPR_14 uint8_t planeCount( VULKAN_HPP_NAMESPACE::Format format );

  // The relative height of this plane. A value of k means that this plane is 1/k the height of the overall format.
  VULKAN_HPP_CONSTEXPR_14 uint8_t planeHeightDivisor( VULKAN_HPP_NAMESPACE::Format format, uint8_t plane );

  // The relative width of this plane. A value of k means that this plane is 1/k the width of the overall format.
  VULKAN_HPP_CONSTEXPR_14 uint8_t planeWidthDivisor( VULKAN_HPP_NAMESPACE::Format format, uint8_t plane );

  // The number of texels in a texel block.
  VULKAN_HPP_CONSTEXPR_14 uint8_t texelsPerBlock( VULKAN_HPP_NAMESPACE::Format format );

  //=== Function Definitions ===

  // The three-dimensional extent of a texel block.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 std::array<uint8_t, 3> blockExtent( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${blockExtentCases}
      default: return {{1, 1, 1 }};
    }
  }

  // The texel block size in bytes.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t blockSize( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${blockSizeCases}
      default : VULKAN_HPP_ASSERT( false ); return 0;
    }
  }

  // The class of the format (can't be just named "class"!)
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 char const * compatibilityClass( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${classCases}
      default : VULKAN_HPP_ASSERT( false ); return "";
    }
  }

  // The number of bits in this component, if not compressed, otherwise 0.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t componentBits( VULKAN_HPP_NAMESPACE::Format format, uint8_t component )
  {
    switch( format )
    {
${componentBitsCases}
      default: return 0;
    }
  }

  // The number of components of this format.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t componentCount( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${componentCountCases}
      default: return 0;
    }
  }

  // The name of the component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 char const * componentName( VULKAN_HPP_NAMESPACE::Format format, uint8_t component )
  {
    switch( format )
    {
${componentNameCases}
      default: return "";
    }
  }

  // The numeric format of the component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 char const * componentNumericFormat( VULKAN_HPP_NAMESPACE::Format format, uint8_t component )
  {
    switch( format )
    {
${componentNumericFormatCases}
      default: return "";
    }
  }

  // The plane this component lies in.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t componentPlaneIndex( VULKAN_HPP_NAMESPACE::Format format, uint8_t component )
  {
    switch( format )
    {
${componentPlaneIndexCases}
      default: return 0;
    }
  }

  // True, if the components of this format are compressed, otherwise false.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool componentsAreCompressed( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${componentsAreCompressedCases}
        return true;
      default: return false;
    }
  }

  // A textual description of the compression scheme, or an empty string if it is not compressed
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 char const * compressionScheme( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${compressionSchemeCases}
      default: return "";
    }
  }

  // Get all formats with a depth component
  VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::Format> const & getDepthFormats()
  {
    static std::vector<VULKAN_HPP_NAMESPACE::Format> depthFormats = { ${depthFormats} };
    return depthFormats;
  }

  // Get all formats with a depth and a stencil component
  VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::Format> const & getDepthStencilFormats()
  {
    static std::vector<VULKAN_HPP_NAMESPACE::Format> depthStencilFormats = { ${depthStencilFormats} };
    return depthStencilFormats;
  }

  // Get all formats with a stencil component
  VULKAN_HPP_INLINE std::vector<VULKAN_HPP_NAMESPACE::Format> const & getStencilFormats()
  {
    static std::vector<VULKAN_HPP_NAMESPACE::Format> stencilFormats = { ${stencilFormats} };
    return stencilFormats;
  }

  // True, if this format has a depth component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool hasDepthComponent( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${depthCases}
        return true;
      default: return false;
    }
  }

  // True, if this format has a stencil component
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool hasStencilComponent( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${stencilCases}
        return true;
      default: return false;
    }
  }

  // True, if this format is a compressed one.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 bool isCompressed( VULKAN_HPP_NAMESPACE::Format format )
  {
    return ( *VULKAN_HPP_NAMESPACE::compressionScheme( format ) != 0 );
  }

  // The number of bits into which the format is packed. A single image element in this format
  // can be stored in the same space as a scalar type of this bit width.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t packed( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${packedCases}
      default: return 0;
    }
  }

  // The single-plane format that this plane is compatible with.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 VULKAN_HPP_NAMESPACE::Format planeCompatibleFormat( VULKAN_HPP_NAMESPACE::Format format, uint8_t plane )
  {
    switch( format )
    {
${planeCompatibleCases}
      default: VULKAN_HPP_ASSERT( plane == 0 ); return format;
    }
  }

  // The number of image planes of this format.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t planeCount( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${planeCountCases}
      default: return 1;
    }
  }

  // The relative height of this plane. A value of k means that this plane is 1/k the height of the overall format.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t planeHeightDivisor( VULKAN_HPP_NAMESPACE::Format format, uint8_t plane )
  {
    switch( format )
    {
${planeHeightDivisorCases}
      default: VULKAN_HPP_ASSERT( plane == 0 ); return 1;
    }
  }

  // The relative width of this plane. A value of k means that this plane is 1/k the width of the overall format.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t planeWidthDivisor( VULKAN_HPP_NAMESPACE::Format format, uint8_t plane )
  {
    switch( format )
    {
${planeWidthDivisorCases}
      default: VULKAN_HPP_ASSERT( plane == 0 ); return 1;
    }
  }

  // The number of texels in a texel block.
  VULKAN_HPP_INLINE VULKAN_HPP_CONSTEXPR_14 uint8_t texelsPerBlock( VULKAN_HPP_NAMESPACE::Format format )
  {
    switch( format )
    {
${texelsPerBlockCases}
      default: VULKAN_HPP_ASSERT( false ); return 0;
    }
  }
