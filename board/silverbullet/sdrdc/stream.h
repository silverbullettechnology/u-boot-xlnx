/**
 * \file stream.h
 *
 * \brief AT90USB1287_PDI_programmer
 *
 * Copyright (C) 2009 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#ifndef STREAM_H_
#define STREAM_H_

#include "config.h"
#include <status_codes.h>

struct stream {
	uint8_t size;
	uint16_t in_ptr;
	uint16_t out_ptr;
	uint8_t *data_buffer;
};

/**
 * \brief Flush the buffer
 *
 * \param stream the stream to flush
 * \retval always return STATUS_OK
 */
static inline enum status_code stream_buf_flush(struct stream *stream)
{
	stream->out_ptr = stream->in_ptr;
	return STATUS_OK;
}

enum status_code stream_buf_insert_char(struct stream *stream, uint8_t c);
uint8_t stream_buf_insert_str(struct stream *stream, uint8_t *str);
enum status_code stream_init(struct stream *stream, uint8_t *buffer, uint8_t size);
uint8_t stream_buf_has_data(struct stream *stream);
uint8_t stream_buf_get_char(struct stream *stream);
uint8_t stream_buf_get_available_space(struct stream *stream);

#endif /* STREAM_H_ */
