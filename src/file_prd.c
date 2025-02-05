/*

    File: file_prd.c

    Copyright (C) 2016 Christophe GRENIER <grenier@cgsecurity.org>

    This software is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write the Free Software Foundation, Inc., 51
    Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 */

#if !defined(SINGLE_FORMAT) || defined(SINGLE_FORMAT_prd)
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <stdio.h>
#include "types.h"
#include "filegen.h"

/*@ requires \valid(file_stat); */
static void register_header_check_prd(file_stat_t *file_stat);

const file_hint_t file_hint_prd = {
  .extension = "prd",
  .description = "Paessler PRTG",
  .max_filesize = PHOTOREC_MAX_FILE_SIZE,
  .recover = 1,
  .enable_by_default = 1,
  .register_header_check = &register_header_check_prd
};

/*@
  @ requires buffer_size >= 0x18;
  @ requires \valid_read(buffer+(0..buffer_size-1));
  @ requires valid_file_recovery(file_recovery);
  @ requires \valid(file_recovery_new);
  @ requires file_recovery_new->blocksize > 0;
  @ requires separation: \separated(&file_hint_prd, buffer+(..), file_recovery, file_recovery_new);
  @ ensures  \result!=0 ==> valid_file_recovery(file_recovery_new);
  @ assigns  *file_recovery_new;
  @*/
static int header_check_prd(const unsigned char *buffer, const unsigned int buffer_size, const unsigned int safe_header_only, const file_recovery_t *file_recovery, file_recovery_t *file_recovery_new)
{
  if(buffer[0x0d] != 0xdb || buffer[0x0e] != 0xe4 || buffer[0x0f] != 0x40 || buffer[0x15] != 0xdb || buffer[0x16] != 0xe4 || buffer[0x17] != 0x40)
    return 0;
  reset_file_recovery(file_recovery_new);
  file_recovery_new->extension = file_hint_prd.extension;
  return 1;
}

static void register_header_check_prd(file_stat_t *file_stat)
{
  static const unsigned char prd_header[8] = {
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  register_header_check(0, prd_header, sizeof(prd_header), &header_check_prd, file_stat);
}
#endif
