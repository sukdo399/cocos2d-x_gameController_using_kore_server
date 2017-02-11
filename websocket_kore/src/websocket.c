/*
 * Copyright (c) 2014 Joris Vink <joris@coders.se>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "common.h"

int        page(struct http_request *);
int        page_ws_connect(struct http_request *);

void        websocket_connect(struct connection *);
void        websocket_disconnect(struct connection *);
void        websocket_message(struct connection *,
            u_int8_t, void *, size_t);

/* Websocket callbacks. */
struct kore_wscbs wscbs = {
    websocket_connect,
    websocket_message,
    websocket_disconnect
};

/* Called whenever we get a new websocket connection. */
void
websocket_connect(struct connection *c)
{
    printf("[%s %d] ================================ \n", __FUNCTION__, __LINE__);
    printf("%p: connected\n", c);
    kore_msg_send(KORE_MSG_PARENT, MESSAGE_BUTTON_ID, "connect", 7);
}

void
websocket_message(struct connection *c, u_int8_t op, void *data, size_t len)
{
    char temp[1024] = {0, };

    strncpy(temp, data, len);
    printf("[%s %d] ================================ data: %s, len: %d \n", __FUNCTION__, __LINE__, (char *)temp, len);

    kore_msg_send(KORE_MSG_PARENT, MESSAGE_BUTTON_ID, data, len);
}

void
websocket_disconnect(struct connection *c)
{
    printf("[%s %d] ================================ \n", __FUNCTION__, __LINE__);
    printf("%p: disconnecting\n", c);
    kore_msg_send(KORE_MSG_PARENT, MESSAGE_BUTTON_ID, "disconnect", 10);
}

int
page(struct http_request *req)
{
    printf("[%s %d] ================================ \n", __FUNCTION__, __LINE__);

    http_response_header(req, "content-type", "text/html");
    http_response(req, 200, asset_frontend_html, asset_len_frontend_html);

    return (KORE_RESULT_OK);
}

int
page_ws_connect(struct http_request *req)
{
    printf("[%s %d] ================================ \n", __FUNCTION__, __LINE__);

    /* Perform the websocket handshake, passing our callbacks. */
    kore_websocket_handshake(req, &wscbs);

    return (KORE_RESULT_OK);
}
