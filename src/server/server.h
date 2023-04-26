/**
 * @brief Create a socket to listen on and return its file descriptor.
 *
 * The socket is opened on localhost and bound to the port given in config.h.
 *
 * @return the file descriptor of the created listener socket
 */
int create_listener_socket();