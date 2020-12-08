// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

#ifndef _FGFXFONT_H_
#define _FGFXFONT_H_

/// Font data stored PER GLYPH - character - (based on Adafruit libarary)
typedef struct 
{
  uint8_t  ascii;         ///< ASCII code for this glyph
  uint16_t bitmapOffset; ///< Pointer into GFXfont->bitmap
  uint8_t  width;         ///< Bitmap dimensions in pixels
  uint8_t  height;        ///< Bitmap dimensions in pixels
  uint8_t  xAdvance;      ///< Distance to advance cursor (x axis)
  int8_t   xOffset;        ///< X dist from cursor pos to UL corner
  int8_t   yOffset;        ///< Y dist from cursor pos to UL corner
} FixedGFXglyph;

/// Extended font data (based on Adafruit libarary)
typedef struct 
{
    uint8_t* bitmap;       ///< Glyph bitmaps, concatenated
    FixedGFXglyph* glyph;  ///< Glyph array
    uint8_t glyphCount;    ///< Actual number of defined glyphs
    int8_t yAdvance;       ///< Newline distance (y axis)
    int8_t xMin;           ///< Leftmost  
    int8_t xMax;           ///< Rightmost
    int8_t yMin;           ///< Bottom Y
    int8_t yMax;           ///< Upper Y
} FixedGFXfont;

#endif
