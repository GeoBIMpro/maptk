/*ckwg +29
 * Copyright 2015 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file
 * \brief C Interface to \p config_block object implementation
 */


#include "config_block.h"

#include <cstdlib>
#include <cstring>
#include <map>
#include <sstream>
#include <string>

#include <maptk/config_block.h>
#include <maptk/exceptions.h>
#include <maptk/logging_macros.h>

#include <maptk/c/helpers/c_utils.h>
#include <maptk/c/helpers/config_block.h>


/// Definition of sptr cache
namespace maptk_c
{

SharedPointerCache< maptk::config_block,
                    maptk_config_block_t > CONFIG_BLOCK_SPTR_CACHE;

}


// Static Constants
char const *maptk_config_block_block_sep = maptk::config_block::block_sep.c_str();
char const *maptk_config_block_global_value = maptk::config_block::global_value.c_str();


/// Create a new, empty \p config_block object
maptk_config_block_t* maptk_config_block_new()
{
  STANDARD_CATCH(
    "C::config_block::new",

    return maptk_config_block_new_named("");

  );
  return 0;
}

/// Create a new, empty \p config_block object with a name
maptk_config_block_t* maptk_config_block_new_named( char const *name )
{
  STANDARD_CATCH(
    "C::config_block::new_named",

    maptk::config_block_sptr cb_sptr = maptk::config_block::empty_config( name );
    maptk_c::CONFIG_BLOCK_SPTR_CACHE.store( cb_sptr );
    return reinterpret_cast<maptk_config_block_t*>(cb_sptr.get());
  );
  return 0;
}

/// Destroy a config block object
unsigned int maptk_config_block_destroy( maptk_config_block_t *cb )
{
  STANDARD_CATCH(
    "C::config_block::destroy",

    return maptk_c::CONFIG_BLOCK_SPTR_CACHE.erase( cb );
  );
  return 0;
}

/// Get the name of the \p config_block instance
char const* maptk_config_block_get_name( maptk_config_block_t *cb )
{
  STANDARD_CATCH(
    "maptk::C::config_block::get_name",

    return maptk_c::CONFIG_BLOCK_SPTR_CACHE
        .get( cb )->get_name().c_str();
  );
  return 0;
}

/// Get a copy of a sub-block of the configuration
maptk_config_block_t* maptk_config_block_subblock( maptk_config_block_t *cb,
                                                   char const *key )
{
  STANDARD_CATCH(
    "maptk::C::config_block::subblock",

    maptk::config_block_sptr cb_sptr = maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb );
    maptk::config_block_sptr sb_sptr = cb_sptr->subblock( key );
    maptk_c::CONFIG_BLOCK_SPTR_CACHE.store( sb_sptr );
    return reinterpret_cast<maptk_config_block_t*>( sb_sptr.get() );
  );
  return 0;
}

/// Get a mutable view of a sub-block within a configuration
maptk_config_block_t* maptk_config_block_subblock_view( maptk_config_block_t *cb,
                                                        char const *key )
{
  STANDARD_CATCH(
    "maptk::C::config_block::subblock_view",

    maptk::config_block_sptr cb_sptr = maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb );
    maptk::config_block_sptr sb_sptr = cb_sptr->subblock_view( key );
    maptk_c::CONFIG_BLOCK_SPTR_CACHE.store( sb_sptr );
    return reinterpret_cast<maptk_config_block_t*>( sb_sptr.get() );
  );
  return 0;
}

/// Get the string value for a key
char const* maptk_config_block_get_value( maptk_config_block_t *cb,
                                          char const* key )
{
  STANDARD_CATCH(
    "maptk::C::config_block::get_value",

    return maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb )
        ->get_value<std::string>( key ).c_str();
  );
  return 0;
}

/// Get the string value for a key if it exists, else the default
char const*  maptk_config_block_get_value_default( maptk_config_block_t *cb,
                                                   char const* key,
                                                   char const* deflt )
{
  STANDARD_CATCH(
    "maptk::C::config_block::get_value_default",

    return maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb )
        ->get_value<std::string>( key, deflt ).c_str();
  );
  return 0;
}

/// Get the description string for a given key
char const* maptk_config_block_get_description( maptk_config_block_t *cb,
                                                char const* key )
{
  STANDARD_CATCH(
    "maptk::C::config_block::get_description",

    return maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb )
        ->get_description( key ).c_str();
  );
  return 0;
}

/// Set the string value for a key
void maptk_config_block_set_value( maptk_config_block_t *cb,
                                   char const* key,
                                   char const* value )
{
  STANDARD_CATCH(
    "maptk::C::config_block::set_value",

    maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb )
        ->set_value<std::string>( key, value );
  );
}

/// Set a string value with an associated description
void maptk_config_block_set_value_descr( maptk_config_block_t *cb,
                                         char const *key,
                                         char const *value,
                                         char const *description )
{
  STANDARD_CATCH(
    "maptk::C::config_block::set_value_descr",

    maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb )
        ->set_value<std::string>( key, value, description );
  );
}

/// Remove a key/value pair from the configuration.
void maptk_config_block_unset_value( maptk_config_block_t *cb,
                                     char const *key )
{
  STANDARD_CATCH(
    "maptk::C::config_block::unset_value",

    maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb )->unset_value( key );
  );
}

/// Query if a value is read-only
bool maptk_config_block_is_read_only( maptk_config_block_t *cb,
                                      char const *key )
{
  STANDARD_CATCH(
    "maptk::C:config_block::is_read_only",

    return maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb )->is_read_only( key );
  );
  return false;
}

/// Mark the given key as read-only
void maptk_config_block_mark_read_only( maptk_config_block_t *cb,
                                        char const *key )
{
  STANDARD_CATCH(
    "maptk::C::config_block::mark_read_only",

    maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb )->mark_read_only( key );
  );
}

/// Merge another \p config_block's entries into this \p config_block
void maptk_config_block_merge_config( maptk_config_block_t *cb,
                                      maptk_config_block_t *other )
{
  STANDARD_CATCH(
    "maptk::C::config_block::merge_config",

    maptk::config_block_sptr cb_sptr = maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb );
    maptk::config_block_sptr other_sptr = maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( other );
    cb_sptr->merge_config( other_sptr );
  );
}

/// Check if a value exists for the given key
bool maptk_config_block_has_value( maptk_config_block_t *cb,
                                   char const *key )
{
  STANDARD_CATCH(
    "maptk::C::config_block::has_key",

    return maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb )->has_value( key );
  );
  return false;
}

/// Return the values available in the configuration.
void maptk_config_block_available_values( maptk_config_block_t *cb,
                                          unsigned int *length,
                                          char ***keys )
{
  STANDARD_CATCH(
    "maptk::C::config_bloc::available_values",

    if ( length == 0 || keys == 0 )
    {
      throw maptk::invalid_value("One or both provided output parameters "
                                 "were a NULL pointer.");
    }

    maptk::config_block_sptr cb_sptr = maptk_c::CONFIG_BLOCK_SPTR_CACHE.get( cb );
    std::vector<std::string> cb_keys = cb_sptr->available_values();
    maptk_c::make_string_list( cb_keys, *length, *keys );
  );
}
