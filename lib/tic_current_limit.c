#include "tic_internal.h"

// Use a current limit code as an index in this table to look up the nominal
// current limit in milliamps.  This table is generated by
// ruby/tic03a_current_table.rb, except we tweaked the second value so it is not
// 0 and can therefore be selected in the GUI.
static const uint16_t tic03a_current_table[33] =
{
  0,
  1,
  174,
  343,
  495,
  634,
  762,
  880,
  990,
  1092,
  1189,
  1281,
  1368,
  1452,
  1532,
  1611,
  1687,
  1762,
  1835,
  1909,
  1982,
  2056,
  2131,
  2207,
  2285,
  2366,
  2451,
  2540,
  2634,
  2734,
  2843,
  2962,
  3093,
};

static const uint8_t tic03a_recommended_codes[33] =
{
  0,   1,   2,   3,   4,   5,   6,   7,
  8,   9,   10,  11,  12,  13,  14,  15,
  16,  17,  18,  19,  20,  21,  22,  23,
  24,  25,  26,  27,  28,  29,  30,  31,
  32,
};

static const uint8_t tic01a_recommended_codes[64] =
{
  0,   1,   2,   3,   4,   5,   6,   7,
  8,   9,   10,  11,  12,  13,  14,  15,
  16,  17,  18,  19,  20,  21,  22,  23,
  24,  25,  26,  27,  28,  29,  30,  31,
  32,  34,  36,  38,  40,  42,  44,  46,
  48,  50,  52,  54,  56,  58,  60,  62,
  64,  68,  72,  76,  80,  84,  88,  92,
  96,  100, 104, 108, 112, 116, 120, 124,
};

static const uint8_t tic06a_recommended_codes[128] =
{
  0,   1,   2,   3,   4,   5,   6,   7,
  8,   9,   10,  11,  12,  13,  14,  15,
  16,  17,  18,  19,  20,  21,  22,  23,
  24,  25,  26,  27,  28,  29,  30,  31,
  32,  33,  34,  35,  36,  37,  38,  39,
  40,  41,  42,  43,  44,  45,  46,  47,
  48,  49,  50,  51,  52,  53,  54,  55,
  56,  57,  58,  59,  60,  61,  62,  63,
  64,  65,  66,  67,  68,  69,  70,  71,
  72,  73,  74,  75,  76,  77,  78,  79,
  80,  81,  82,  83,  84,  85,  86,  87,
  88,  89,  90,  91,  92,  93,  94,  95,
  96,  97,  98,  99,  100, 101, 102, 103,
  104, 105, 106, 107, 108, 109, 110, 111,
  112, 113, 114, 115, 116, 117, 118, 119,
  120, 121, 122, 123, 124, 125, 126, 127,
};

tic_error * tic_get_settings_needed_to_convert_current_limits(
  tic_handle * handle, tic_settings ** settings)
{
  // TODO
}

const uint8_t * tic_get_recommended_current_limit_codes(
  uint8_t product, size_t * code_count)
{
  size_t count = 0;
  const uint8_t * table = 0;

  switch (product)
  {
  case TIC_PRODUCT_T500:
    table = tic03a_recommended_codes;
    count = sizeof(tic03a_recommended_codes);
    break;

  case TIC_PRODUCT_T834:
    // Only use part of the table to avoid exceeding the maximum allowed
    // current.
    table = tic01a_recommended_codes;
    count = 60;  // Only use part of the table
    break;

  case TIC_PRODUCT_T249:
    // Only use part of the table to avoid exceeding the maximum allowed
    // current.
    table = tic01a_recommended_codes;
    count = 61;
    break;

  case TIC_PRODUCT_T825:
  case TIC_PRODUCT_N825:
    table = tic01a_recommended_codes;
    count = sizeof(tic01a_recommended_codes);
    break;

  case TIC_PRODUCT_TIC06A:
    table = tic06a_recommended_codes;
    count = sizeof(tic06a_recommended_codes);
    break;

  default:
    table = NULL;
    count = 0;
    break;
  }

  if (code_count) { *code_count = count; }
  return table;
}

// Deprecated: Without access to a tic_settings object, this function cannot
// to do the right thing for tic06a.
uint32_t tic_get_max_allowed_current(uint8_t product)
{
  switch (product)
  {
  case TIC_PRODUCT_T500:
    return TIC_MAX_ALLOWED_CURRENT_T500;
  case TIC_PRODUCT_T834:
    return TIC_MAX_ALLOWED_CURRENT_T834;
  case TIC_PRODUCT_T825:
  case TIC_PRODUCT_N825:
    return TIC_MAX_ALLOWED_CURRENT_T825;
  case TIC_PRODUCT_T249:
    return TIC_MAX_ALLOWED_CURRENT_T249;
  default:
    return 0;
  }
}

// Deprecated: Without access to a tic_settings object, this function cannot
// to do the right thing for tic06a.
uint32_t tic_current_limit_code_to_ma(uint8_t product, uint8_t code)
{
  if (product == TIC_PRODUCT_T500)
  {
    if (code > TIC_MAX_ALLOWED_CURRENT_CODE_T500)
    {
      code = TIC_MAX_ALLOWED_CURRENT_CODE_T500;
    }
    return tic03a_current_table[code];
  }
  else if (product == TIC_PRODUCT_T825 ||
    product == TIC_PRODUCT_N825 ||
    product == TIC_PRODUCT_T834 ||
    product == TIC_PRODUCT_T249)
  {
    uint8_t units = TIC_CURRENT_LIMIT_UNITS_MA;
    if (product == TIC_PRODUCT_T249)
    {
      units = TIC_CURRENT_LIMIT_UNITS_MA_T249;
    }

    uint8_t max = tic_get_max_allowed_current(product) / units;

    if (code > max) { code = max; }
    else if (code > 64) { code &= ~3; }
    else if (code > 32) { code &= ~1; }

    return code * units;
  }
  else
  {
    // Other products are not supported by this deprecated function.
    return 0;
  }
}

// Deprecated: Without access to a tic_settings object, this function cannot
// to do the right thing for tic06a.
uint8_t tic_current_limit_ma_to_code(uint8_t product, uint32_t ma)
{
  if (product == TIC_PRODUCT_T500 ||
    product == TIC_PRODUCT_T825 ||
    product == TIC_PRODUCT_N825 ||
    product == TIC_PRODUCT_T834 ||
    product == TIC_PRODUCT_T249)
  {
    size_t count;
    const uint8_t * table = tic_get_recommended_current_limit_codes(
      product, &count);

    // Assumption: The table is in ascending order, so we want to return the last
    // one that is less than or equal to the desired current.
    // Assumption: 0 is a valid code and a good default to use.
    uint8_t code = 0;
    for (size_t i = 0; i < count; i++)
    {
      if (tic_current_limit_code_to_ma(product, table[i]) <= ma)
      {
        code = table[i];
      }
      else
      {
        break;
      }
    }
    return code;
  }
  else
  {
    // Other products are not supported by this deprecated function.
    return 0;
  }
}
