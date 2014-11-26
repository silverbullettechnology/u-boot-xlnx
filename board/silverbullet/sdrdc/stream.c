/**
 * \file stream
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

#include "stream.h"

/**
 * \brief Function for putting a single byte into the steam
 *
 * This function takes a single byte and inputs in into the stream
 * at the correct location based on the stream pointers.
 *
 * \param stream The stream to input the byte to
 * \param c the byte to input into the stream
 * \retval the status of the input
 */
enum status_code stream_buf_insert_char(struct stream *stream, uint8_t c)
{
	/* If stream is full do not insert new chars */
	if(stream_buf_get_available_space(stream)){
		*(stream->data_buffer + stream->in_ptr) = c;
		stream->in_ptr++;

		if(stream->in_ptr == stream->size){
			/* Wrap around */
			stream->in_ptr = 0;
		}

		return STATUS_OK;
	}else{
		return ERR_NO_MEMORY;
	}
}

/**
 * \brief Input a string into the stream
 *
 * \param stream The stream to write the string to
 * \param str the pointer to the string
 * \retval the last byte in the string
 */
uint8_t stream_buf_insert_str(struct stream *stream, uint8_t *str)
{
	while(*str != 0){
		stream_buf_insert_char(stream, *str);
		str++;
	}
	/* Insert end of string char */
	stream_buf_insert_char(stream, 0x00);
	return *(stream->data_buffer + stream->in_ptr);
}

/**
 * \brief Check if the stream contains data
 *
 * This function will check if the stream contains data, if it does,
 * it returns the number of bytes in the stream.
 *
 * \param stream The stream that has to be checked
 * \retval size of data in bytes
 */
uint8_t stream_buf_has_data(struct stream *stream)
{
	int8_t size = stream->in_ptr - stream->out_ptr;
	/* Negative value indicate a wrap-around situation */
	if(size < 0)
	{
		size = stream->size - stream->out_ptr + stream->in_ptr;
	}
	return size;
}

/**
 * \brief Get a byte from the stream
 *
 * \param stream The data stream to get a bye from.
 * \retval a data byte from the stream.
 */
uint8_t stream_buf_get_char(struct stream *stream)
{
	uint8_t ret = *(stream->data_buffer + stream->out_ptr);
	stream->out_ptr++;

	if(stream->out_ptr == stream->size){
		stream->out_ptr = 0;
	}
	return ret;
}

/**
 * \brief Initialize the stream
 *
 * This function has to be run to initialize the stream.
 *
 * \param stream the strem to initialize
 * \param buffer the buffer to use to store the stream
 * \param size the size og the buffer
 * \retval always return STATUS_OK regardless of input.
 */
enum status_code stream_init(struct stream *stream, uint8_t *buffer, uint8_t size)
{
	stream->size = size;
	stream->data_buffer = buffer;
	return STATUS_OK;
}

/**
 * \brief Get available space in the buffer
 *
 * This function returns the number of bytes left
 * in the stream.
 *
 * \param stream the stream to operate on.
 * \retval uin16_t number of bytes in the stream
 */
uint8_t stream_buf_get_available_space(struct stream *stream)
{
	uint8_t available_space;
	/* Calculates the available space in the buffer when in pointer
	 * is pointing after out pointer.*/
	available_space = (stream->size - 1) - stream->in_ptr + stream->out_ptr;
	/* If the resulting size is bigger than the size of the buffer we
	 * have a wrap around situation.*/
	if(available_space >= stream->size){
		available_space = stream->out_ptr - stream->in_ptr - 1;
	}
	return available_space;
}
