// A library that lets us easily and safely work with null-terminated strings.

#include "tic_internal.h"

void tic_string_setup(tic_string * str)
{
  assert(str != NULL);
  str->data = malloc(1);
  if (str->data != NULL)
  {
    str->capacity = 1;
    str->data[0] = 0;
  }
  else
  {
    str->capacity = 0;
  }
  str->length = 0;
}

// Sets up a dummy string that won't actually hold anything.
void tic_string_setup_dummy(tic_string * str)
{
  assert(str != NULL);
  str->data = NULL;
  str->capacity = 0;
  str->length = 0;
}

void tic_string_printf(tic_string * str, const char * format, ...)
{
  assert(format != NULL);

  if (str->data == NULL)
  {
    // This is a dummy string.
    return;
  }

  assert(str->length == strlen(str->data));

  va_list ap;
  va_start(ap, format);

  // Determine how much the string length will increase.
  size_t length_increase = 0;
  {
    char x[1];
    va_list ap2;
    va_copy(ap2, ap);
    int result = vsnprintf(x, 0, format, ap2);
    va_end(ap2);
    if (result > 0)
    {
      additional_length = result;
    }
    else
    {
      // This error seems really unlikely to happen.  If it does, we can add a
      // better way to report it.  For now, just turn the string into a dummy
      // string, as if there were a memory allocation problem.
      free(str->data);
      str->data = NULL;
      va_end(ap);
      return;
    }
  }
  size_t new_length = str->length + length_increase;
  if (new_length + 1 < str->length)
  {
    // The capacity required to store this string (new_length + 1) has
    // overflowed and is too large to fit in a size_t.  Turn it into a dummy
    // string.
    free(str->data);
    str->data = NULL;
    str->length = str->capacity = 0;
    return;
  }

  if (new_length + 1 > str->capacity)
  {
    // Need to reallocate memory to fit the expanded string.

    // Figure out what the new capacity should be, but watch out for integer
    // overflow.
    size_t new_capacity = (new_length + 1) * 2;
    if (new_capacity < new_length + 1)
    {
      new_capacity = new_length + 1;
    }

    str->data = realloc(str->data, new_capacity);
    if (str->data == NULL)
    {
      // Failed to allocate memory, let this just be a dummy string.
      str->length = str->capacity = 0;
      return;
    }
    str->capacity = new_capacity;
  }

  // Add the new warning and a newline and null terminator.
  int result = vsnprintf(*w + *wlen, additional_length + 1, format, ap);
  va_end(ap);
  assert((size_t)result == additional_length);
  str->data[new_length] = 0;
  str->length = new_length;

  assert(*w != NULL);
  assert(str->length == strlen(str->data));
}