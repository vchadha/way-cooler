#include "seat.h"

#include <wlr/types/wlr_seat.h>

#include "cursor.h"
#include "server.h"

static void wc_seat_request_cursor(struct wl_listener* listener, void* data) {
	struct wc_server* server = wl_container_of(listener, server,
			request_set_cursor);
	struct wc_cursor* cursor = server->cursor;
	struct wlr_seat_pointer_request_set_cursor_event* event = data;
	struct wlr_seat_client *focused_client =
		server->seat->pointer_state.focused_client;
	if (focused_client == event->seat_client) {
		cursor->image = NULL;
		wlr_cursor_set_surface(cursor->wlr_cursor,
				event->surface, event->hotspot_x, event->hotspot_y);
	}
}

void init_seat(struct wc_server* server) {
	server->seat = wlr_seat_create(server->wl_display, "seat0");
	server->request_set_cursor.notify = wc_seat_request_cursor;
	wl_signal_add(&server->seat->events.request_set_cursor,
			&server->request_set_cursor);
}
